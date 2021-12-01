/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : Ocp1LiteSocketConnection
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCP.1/Ocp1LiteHostInterface.h>
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <HostInterfaceLite/OCA/OCF/Timer/IOcfLiteTimer.h>
#include <HostInterfaceLite/OCA/OCP.1/Network/IOcp1LiteSocket.h>
#include <OCF/OcaLiteCommandHandler.h>

// ---- Include local include files ----
#include "Ocp1LiteNetwork.h"
#include "Ocp1LiteSocketConnection.h"

#ifndef MAX_KEEPALIVE_MISSED
#define MAX_KEEPALIVE_MISSED    3
#endif

// ---- Helper types and constants ----
const ::OcaUint8 Ocp1LiteSocketConnection::MESSAGESYNCVAL(static_cast< ::OcaUint8>(0x3B));

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

Ocp1LiteSocketConnection::Ocp1LiteSocketConnection(::Ocp1LiteNetwork& network,
                                                   ::OcaUint32 bufferSize)
    : m_parent(network),
      m_sessionID(OCA_INVALID_SESSIONID),
      m_socket(-1),
      m_socketState(SOCKET_NOT_CONNECTED),
      m_isKeepAliveInMilliseconds(false),
      m_pMessageReceiveBuffer(new ::OcaUint8[static_cast<size_t>(bufferSize)]),
      m_messageBufferSize(bufferSize),
      m_lastMessageSentTime(static_cast< ::OcaUint32>(0)),
      m_lastMessageReceivedTime(static_cast< ::OcaUint32>(0)),
      m_totalLength(static_cast< ::OcaUint32>(0)),
      m_bytesLeft(static_cast< ::OcaUint32>(0)),
      m_keepAliveTimeOut(static_cast< ::OcaUint32>(0)),
      m_messageState(OCA_MSG_STATE_SYNC_FIND),
      m_pCurrentHeader(new ::Ocp1LiteMessageHeader),
      m_pduBytesLeft(static_cast< ::OcaUint32>(0)),
      m_pduMessagesLeft(static_cast< ::OcaUint16>(0)),
      m_messageBytesLeft(static_cast< ::OcaUint32>(0))
{
    // A message header should be available for this connection
    assert(NULL != m_pCurrentHeader);
}

Ocp1LiteSocketConnection::~Ocp1LiteSocketConnection()
{
    delete[] m_pMessageReceiveBuffer;
    m_pMessageReceiveBuffer = NULL;

    delete m_pCurrentHeader;
    m_pCurrentHeader = NULL;
}

void Ocp1LiteSocketConnection::Shutdown()
{
    static_cast<void>(Ocp1LiteSocketShutdown(m_socket));
    static_cast<void>(Ocp1LiteSocketClose(m_socket));

    m_socketState = SOCKET_NOT_CONNECTED;
}

Ocp1LiteSocketConnection::OcaLiteSocketRetVal Ocp1LiteSocketConnection::Run(::OcaBoolean dataAvailable, ::OcaUint32 messageSendBufferSize, ::OcaUint8* pMessageSendBuffer, ::OcaBoolean& bKeepAliveReceived)
{
    OcaLiteSocketRetVal retval(CONNECTION_LOST);

    if (SOCKET_CONNECTED == m_socketState)
    {
        retval = NO_MESSAGE_RECEIVED;
        ReceiveFromSocket(dataAvailable, bKeepAliveReceived);
        if (m_socketState == SOCKET_NOT_CONNECTED)
        {
            retval = CONNECTION_LOST;
        }
        if ((m_totalLength >= m_messageBytesLeft) && (m_messageBytesLeft != static_cast< ::OcaUint32>(0)) &&
            (m_messageState == ::Ocp1LiteSocketConnection::OCA_MSG_STATE_MESSAGE_READ))
        {
            // Always return MESSAGE_RECEIVED when there are pending messages
            retval = MESSAGE_RECEIVED;
        }
        if (!HandleKeepAlive(messageSendBufferSize, pMessageSendBuffer))
        {
            retval = CONNECTION_LOST;
        }
    }

    // Corrupt socket overrides everything.
    if (SOCKET_CORRUPT == m_socketState)
    {
        retval = CONNECTION_CORRUPT;
    }

    // Reset message state machine when not connected
    if (SOCKET_CONNECTED != m_socketState)
    {
        m_messageState = OCA_MSG_STATE_SYNC_FIND;
    }

    return retval;
}

OcaLiteMessageGeneral* Ocp1LiteSocketConnection::GetFirstPendingMessage()
{
    OcaLiteMessageGeneral* message(NULL);

    if ((m_totalLength >= m_messageBytesLeft) && (m_messageBytesLeft != static_cast< ::OcaUint32>(0)) &&
            (m_messageState == ::Ocp1LiteSocketConnection::OCA_MSG_STATE_MESSAGE_READ))
    {
        message = m_parent.RetrieveMessage(m_pCurrentHeader->GetMessageType());
        if (NULL != message)
        {
            const ::OcaUint8* pMessageData(m_pMessageReceiveBuffer + (m_totalLength - m_bytesLeft));
            if (!message->Unmarshal(m_bytesLeft, &pMessageData, m_parent.GetReader()))
            {
                m_parent.ReturnMessage(message);
                message = NULL;
            }

            ::memmove(m_pMessageReceiveBuffer, &m_pMessageReceiveBuffer[m_totalLength - m_bytesLeft], static_cast<size_t>(m_bytesLeft));
            m_totalLength = m_bytesLeft;
    
            m_pduBytesLeft -= m_messageBytesLeft;
            m_messageBytesLeft = static_cast< ::OcaUint32>(0);
            m_lastMessageReceivedTime = static_cast< ::OcaUint32>(::OcfLiteTimerGetTimerTickCount());
            m_pduMessagesLeft--;
            if (m_pduMessagesLeft == static_cast< ::OcaUint16>(0))
            {
                assert(m_pduBytesLeft == static_cast< ::OcaUint32>(0));
                    
                // On to the next pdu message
                m_messageState = OCA_MSG_STATE_SYNC_FIND;
            }
        }
        else
        {
            // No message available to load, so stop reading. The message will be handled
            // in the next run.
        }
    }
    
    return message;
}

bool Ocp1LiteSocketConnection::HasPendingMessage() const
{
    // If we still have data left in our buffer, we could have a message pending
    return (m_totalLength > static_cast< ::OcaUint32>(0));
}

::OcaLiteStatus Ocp1LiteSocketConnection::SendOcaMessage(const ::OcaLiteMessageGeneral* msg, const ::IOcaLiteWriter& writer, 
                                                         ::OcaUint32 messageSendBufferSize, ::OcaUint8* pMessageSendBuffer)
{
    ::OcaLiteStatus result(OCASTATUS_DEVICE_ERROR);

    ::Ocp1LiteMessageHeader pHeader;

    // Calculate message length
    ::OcaUint32 msgLen(pHeader.GetSize(writer) + msg->GetSize(writer));
    pHeader.WriteOcp1Parameters(msg->GetMessageType(),
                                static_cast< ::OcaUint16>(1),
                                msgLen);
    if (SOCKET_CONNECTED == m_socketState)
    {
        if ((pHeader.GetMessageSize() <= messageSendBufferSize) && (NULL != pMessageSendBuffer))
        {
            ::OcaUint8* pB(pMessageSendBuffer);
            m_parent.GetWriter().Write(MESSAGESYNCVAL, &pB);
            pHeader.Marshal(&pB, writer);
            msg->Marshal(&pB, writer);

            INT32 size(static_cast<INT32>(sizeof(MESSAGESYNCVAL)) + static_cast<INT32>(pHeader.GetMessageSize()));

            INT32 sendResult(Ocp1LiteSocketSend(m_socket, pMessageSendBuffer, size));
            if (sendResult == size)
            {
                m_lastMessageSentTime = static_cast< ::OcaUint32>(OcfLiteTimerGetTimerTickCount());
                result = OCASTATUS_OK;
            }
            else if (sendResult > 0)
            {
                OCA_LOG_ERROR("Failed to send complete message");
                result = OCASTATUS_PARTIALLY_SUCCEEDED;
            }
            else if (0 == sendResult)
            {
                OCA_LOG_ERROR("Failed to send message due to overflow of socket");
                result = OCASTATUS_BUFFER_OVERFLOW;
            }
            else
            {
                OCA_LOG_ERROR_PARAMS("Send returned negative value %d", sendResult);
                result = OCASTATUS_PROCESSING_FAILED;
            }
        }
        else
        {
            result = OCASTATUS_BUFFER_OVERFLOW;
        }
    }
    else
    {
        result = OCASTATUS_PROCESSING_FAILED;
    }
    return result;
}

void Ocp1LiteSocketConnection::SetSocketConnectionParameters(::OcaSessionID sessionID,
                                                             ::OcaUint32 timeout, 
                                                             bool keepAliveInMilliseconds)
{
    m_sessionID = sessionID;
    m_keepAliveTimeOut = timeout;
    m_isKeepAliveInMilliseconds = keepAliveInMilliseconds;

    // Reset the connection handling members
    m_socketState = SOCKET_CONNECTED;    
    m_lastMessageReceivedTime = static_cast< ::OcaUint32>(0);
    m_lastMessageSentTime = static_cast< ::OcaUint32>(0);
    m_totalLength = static_cast< ::OcaUint32>(0);
    m_bytesLeft = static_cast< ::OcaUint32>(0);
    m_messageState = OCA_MSG_STATE_SYNC_FIND;
    m_pduBytesLeft = static_cast< ::OcaUint32>(0);
    m_pduMessagesLeft = static_cast< ::OcaUint16>(0);
    m_messageBytesLeft = static_cast< ::OcaUint32>(0);
}

::OcaLiteStatus Ocp1LiteSocketConnection::SendKeepAlive(::OcaUint32 heartBeatTimeOut, ::OcaUint32 messageSendBufferSize, 
                                                        ::OcaUint8* pMessageSendBuffer)
{
    ::OcaLiteStatus status(OCASTATUS_PROCESSING_FAILED);
    if (SOCKET_CONNECTED == m_socketState)
    {
        ::OcaLiteMessageGeneral* pMsg(m_parent.RetrieveMessage(::OcaLiteHeader::OCA_MSG_KEEP_ALIVE));
        if (NULL != pMsg)
        {
            ::Ocp1LiteMessageKeepAlive* pMsgKeepAlive(static_cast< ::Ocp1LiteMessageKeepAlive*>(pMsg));
            if (m_isKeepAliveInMilliseconds)
            {
                pMsgKeepAlive->WriteParameters(heartBeatTimeOut);
            }
            else
            {
                pMsgKeepAlive->WriteParameters(static_cast< ::OcaUint16>(heartBeatTimeOut));
            }
            status = SendOcaMessage(pMsgKeepAlive, m_parent.GetWriter(), messageSendBufferSize, pMessageSendBuffer);

            m_parent.ReturnMessage(pMsg);
        }
    }

    return status;
}

bool Ocp1LiteSocketConnection::HandleKeepAlive(::OcaUint32 messageSendBufferSize, 
                                               ::OcaUint8* pMessageSendBuffer)
{
    bool connectionValid(true);
    if (m_keepAliveTimeOut > static_cast< ::OcaUint32>(0))
    {
        if (static_cast< ::OcaUint32>(0) == m_lastMessageReceivedTime)
        {
            // First keep alive is sent, mark last receive time as current time.
            m_lastMessageReceivedTime = static_cast< ::OcaUint32>(::OcfLiteTimerGetTimerTickCount());
        }
        UINT32 multiplier(1000);
        if (m_isKeepAliveInMilliseconds)
        {
            multiplier = 1;
        }
        if ((OcfLiteTimerGetTimerTickCount() - static_cast<UINT32>(m_lastMessageReceivedTime)) >= static_cast<UINT32>(static_cast<UINT32>(m_keepAliveTimeOut) * multiplier * MAX_KEEPALIVE_MISSED))
        {
            OCA_LOG_ERROR_PARAMS("Not received any message for %i secs on session %u", static_cast<int>(m_keepAliveTimeOut) * MAX_KEEPALIVE_MISSED, m_sessionID);
            connectionValid = false;
            m_socketState = SOCKET_NOT_CONNECTED;
        }
        else if (OcfLiteTimerGetTimerTickCount() - static_cast<UINT32>(m_lastMessageSentTime) >= static_cast<UINT32>(m_keepAliveTimeOut) * multiplier)
        {
            // Send a keep alive message
            if (OCASTATUS_PROCESSING_FAILED == SendKeepAlive(static_cast< ::OcaUint16>(m_keepAliveTimeOut), messageSendBufferSize, pMessageSendBuffer))
            {
                // Only if sending really fails the connection is lost, in other cases (e.g. buffer overflow) it is not lost yet
                connectionValid = false;
            }
        }
    }
    return connectionValid;
}

void Ocp1LiteSocketConnection::ReceiveFromSocket(::OcaBoolean dataAvailable, ::OcaBoolean& bKeepAliveReceived)
{
    assert(NULL != m_pMessageReceiveBuffer);
    bKeepAliveReceived = static_cast< ::OcaBoolean>(false);

    // Ready to read data
    if (SOCKET_CONNECTED == m_socketState)
    {
        INT32 bytesReceived(0);
        if (dataAvailable)
        {
            bytesReceived = Ocp1LiteSocketReceive(m_socket, &m_pMessageReceiveBuffer[m_totalLength], static_cast<INT32>(m_messageBufferSize - m_totalLength));
            if (bytesReceived > 0)
            {
                // Increase total length
                m_totalLength += static_cast< ::OcaUint32>(bytesReceived);
            }
            else
            {
                m_socketState = SOCKET_NOT_CONNECTED;
                OCA_LOG_WARNING_PARAMS("Lost connection to session %u (received bytes %d)", m_sessionID, bytesReceived);
            }
        }

        m_bytesLeft = m_totalLength;
        bool bContinue(true);
        while (bContinue && (m_bytesLeft > static_cast< ::OcaUint32>(0)) && (SOCKET_CONNECTED == m_socketState))
        {
            switch (m_messageState)
            {
            case OCA_MSG_STATE_SYNC_FIND:
                {
                    bool bSyncValFound(false);

                    for (::OcaUint32 messageBufferIndex(static_cast< ::OcaUint32>(0)); messageBufferIndex < m_totalLength; messageBufferIndex++)
                    {
                        if (m_pMessageReceiveBuffer[messageBufferIndex] == MESSAGESYNCVAL)
                        {
                            // Move the message to the front of the message buffer
                            ::memmove(m_pMessageReceiveBuffer,
                                      &m_pMessageReceiveBuffer[messageBufferIndex + static_cast< ::OcaUint32>(1)],
                                      static_cast<size_t>(m_totalLength - (messageBufferIndex + static_cast< ::OcaUint32>(1))));
                            m_totalLength -= messageBufferIndex + static_cast< ::OcaUint32>(1);
                            m_bytesLeft -= messageBufferIndex + static_cast< ::OcaUint32>(1);

                            bSyncValFound = true;
                            m_messageState = OCA_MSG_STATE_HEADER_READ;

                            break;
                        }
                    }

                    if (!bSyncValFound)
                    {
                        OCA_LOG_ERROR_PARAMS("Invalid stream detected on session %u", m_sessionID);
                        bContinue = false;
                        m_totalLength = static_cast< ::OcaUint32>(0);
                    }
                    break;
                }
            case OCA_MSG_STATE_HEADER_READ:
                {
                    // Enough data to read the header
                    if (m_bytesLeft >= m_pCurrentHeader->GetSize(m_parent.GetWriter()))
                    {
                        const ::OcaUint8* pBuff(&m_pMessageReceiveBuffer[m_totalLength - m_bytesLeft]);
                        if (m_pCurrentHeader->Unmarshal(m_bytesLeft, &pBuff, m_parent.GetReader()))
                        {
                            m_pduBytesLeft = m_pCurrentHeader->GetMessageSize() - m_pCurrentHeader->GetSize(m_parent.GetWriter());
                            m_pduMessagesLeft = m_pCurrentHeader->GetMessageCount();
                            m_messageState = OCA_MSG_STATE_MESSAGE_READ;

                            ::memmove(m_pMessageReceiveBuffer, &m_pMessageReceiveBuffer[m_totalLength - m_bytesLeft], static_cast<size_t>(m_bytesLeft));
                            m_totalLength = m_bytesLeft;
                        }
                        else
                        {
                            OCA_LOG_ERROR_PARAMS("Invalid header detected on session %u", m_sessionID);
                            m_messageState = OCA_MSG_STATE_SYNC_FIND;
                            m_totalLength = static_cast< ::OcaUint32>(0);
                            bContinue = false;
                        }
                    }
                    else
                    {
                        // Wait for more data
                        bContinue = false;
                    }
                    break;
                }
            case OCA_MSG_STATE_MESSAGE_READ:
                {
                    if (::OcaLiteHeader::OCA_MSG_KEEP_ALIVE == m_pCurrentHeader->GetMessageType())
                    {
                        ::OcaUint32 keepAliveSize(m_pCurrentHeader->GetMessageSize() - m_pCurrentHeader->GetSize(m_parent.GetWriter()));
                        if (m_bytesLeft >= keepAliveSize)
                        {
                            ::OcaLiteMessageGeneral* mess(m_parent.RetrieveMessage(::OcaLiteHeader::OCA_MSG_KEEP_ALIVE));
                            if (NULL != mess)
                            {
                                const ::OcaUint8* pMessageData(m_pMessageReceiveBuffer + (m_totalLength - m_bytesLeft));
                                // Keep alive heartbeat size to be passed for unmarshalling
                                ::OcaUint32 keepAliveSizeCopy(keepAliveSize);
                                if (mess->Unmarshal(keepAliveSizeCopy, &pMessageData, m_parent.GetReader()))
                                {
                                    if (static_cast< ::OcaUint32>(0) == m_keepAliveTimeOut)
                                    {
                                        ::OcaLiteMessageKeepAlive* keepAliveMessage(static_cast< ::OcaLiteMessageKeepAlive*>(mess));
                                        if (keepAliveMessage->GetHeartBeatTimeInMilliseconds() != (static_cast< ::OcaUint32>(0)))
                                        {
                                            m_isKeepAliveInMilliseconds = true;
                                            m_keepAliveTimeOut = keepAliveMessage->GetHeartBeatTimeInMilliseconds();
                                        }
                                        else if (keepAliveMessage->GetHeartBeatTime() != (static_cast< ::OcaUint16>(0)))
                                        {
                                            m_isKeepAliveInMilliseconds = false;
                                            m_keepAliveTimeOut = static_cast< ::OcaUint32>(keepAliveMessage->GetHeartBeatTime());
                                        }                                   
                                    }

                                    bKeepAliveReceived = static_cast< ::OcaBoolean>(true);
                                }
                                m_parent.ReturnMessage(mess);

                                m_bytesLeft -= keepAliveSize;
                                m_pduBytesLeft -= keepAliveSize;
                                m_messageBytesLeft = static_cast< ::OcaUint32>(0);
                                m_lastMessageReceivedTime = static_cast< ::OcaUint32>(::OcfLiteTimerGetTimerTickCount());
                                m_pduMessagesLeft--;
                                assert(static_cast< ::OcaUint16>(0) == m_pduMessagesLeft);
                                assert(static_cast< ::OcaUint32>(0) == m_pduBytesLeft);

                                // Move current message to front of the buffer
                                ::memmove(m_pMessageReceiveBuffer, &m_pMessageReceiveBuffer[m_totalLength - m_bytesLeft], static_cast<size_t>(m_bytesLeft));
                                m_totalLength = m_bytesLeft;

                                // On to the next pdu message
                                m_messageState = OCA_MSG_STATE_SYNC_FIND;
                            }
                        }
                        else
                        {
                            // Not enough data to unmarshal keep alive
                            bContinue = false;
                        }
                    }
                    else if (static_cast< ::OcaUint32>(0) == m_messageBytesLeft)
                    {
                        ::OcaUint32 messageSize(static_cast< ::OcaUint32>(0));
                        const ::OcaUint8* pBuff(&m_pMessageReceiveBuffer[m_totalLength - m_bytesLeft]);
                        ::OcaUint32 tmpBytesLeft(m_bytesLeft);
                        if (m_parent.GetReader().Read(tmpBytesLeft, &pBuff, messageSize))
                        {
                            m_messageBytesLeft = messageSize;

                            if (messageSize > m_messageBufferSize)
                            {
                                // Message too large for the buffer
                                // Fetch the handle and send response
                                m_messageState = OCA_MSG_STATE_MESSAGE_SKIP;
                                m_messageBytesLeft -= m_bytesLeft;
                                m_totalLength = static_cast< ::OcaUint32>(0);
                                bContinue = false;
                            }
                            else if (m_bytesLeft >= messageSize)
                            {
                                // We have received enough bytes to construct a message, delay this to the moment we actual need the message.
                                bContinue = false;
                            }
                            else
                            {
                                // Wait for more data
                                bContinue = false;
                            }
                        }
                        else
                        {
                            // Not enough bytes to unmarshal size
                            bContinue = false;
                        }
                    }
                    else
                    {
                        // Message header is read, could be waiting for additional data
                        bContinue = false;
                    }
                    break;
                }
            case OCA_MSG_STATE_MESSAGE_SKIP:
                {
                    if (m_messageBytesLeft >= m_totalLength)
                    {
                        m_messageBytesLeft -= m_totalLength;
                        m_pduBytesLeft -= m_totalLength;

                        m_totalLength = static_cast< ::OcaUint32>(0);
                        bContinue = false;
                    }
                    else
                    {
                        m_bytesLeft -= m_messageBytesLeft;
                        m_pduBytesLeft -= m_messageBytesLeft;
                        m_pduMessagesLeft--;

                        if (m_pduMessagesLeft == static_cast< ::OcaUint16>(0))
                        {
                            assert(m_pduBytesLeft != static_cast< ::OcaUint32>(0));

                            // On to the next pdu message
                            m_messageState = OCA_MSG_STATE_SYNC_FIND;
                        }
                        else
                        {
                            m_messageState = OCA_MSG_STATE_MESSAGE_READ;
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
    }
}
