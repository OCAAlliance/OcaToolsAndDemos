/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : Ocp.1LiteUdpNetwork
 *
 */

// ---- Include system wide include files ----
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <HostInterfaceLite/OCA/OCF/Selection/OcfLiteSelectableSet.h>
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <HostInterfaceLite/OCA/OCP.1/Ocp1LiteHostInterface.h>
#include <HostInterfaceLite/OCA/OCP.1/Network/IOcp1LiteNetwork.h>
#include <HostInterfaceLite/OCA/OCP.1/Network/IOcp1LiteSocket.h>
#include <OCC/ControlDataTypes/OcaLiteNetworkStatistics.h>
#include <OCC/ControlDataTypes/OcaLiteStringInABlob.h>
#include <OCF/OcaLiteCommandHandler.h>

// ---- Include local include files ----
#include "Ocp1LiteUdpNetwork.h"
#include "Ocp1LiteNetworkSystemInterfaceID.h"

// ---- Helper types and constants ----

#define OCA_INSECURE_REGISTRATION_TYPE  "_oca._udp"
#define OCA_LOCAL_REGISTRATION_DOMAIN   "local"

#ifndef OCP1_MAX_NR_CONNECTIONS
#define OCP1_MAX_NR_CONNECTIONS 4
#endif

const std::string Ocp1LiteUdpNetwork::INSECURE_REGISTRATION_TYPE(OCA_INSECURE_REGISTRATION_TYPE);
const std::string Ocp1LiteUdpNetwork::LOCAL_REGISTRATION_DOMAIN(OCA_LOCAL_REGISTRATION_DOMAIN);

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

Ocp1LiteUdpNetwork::Ocp1LiteUdpNetwork(::OcaONo objectNumber,
                                 ::OcaBoolean lockable,
                                 const ::OcaLiteString& role,
                                 const ::Ocp1LiteNetworkNodeID& idAdvertised,
                                 const ::Ocp1LiteNetworkSystemInterfaceID& systemInterfaces,
                                 const std::vector<std::string>& txtRecords,
                                 const ::OcaLiteString& domainName,
                                ::OcaUint16 insecureListenPort)
    : OcaLiteNetwork(objectNumber,
                     lockable,
                     role,
                     OCANETWORKLINKTYPE_ETHERNET_WIRED,
                     idAdvertised,
                     OCANETWORKCONTROLPROTOCOL_OCP01,
                     ::OcaLiteNetworkSystemInterfaceID()),
      m_ocp1Writer(),
      m_ocp1Reader(),
      m_pUdpListenSocket(-1),
      m_pMessageReceiveBuffer(NULL),
      m_ocaSocketList(),
      m_newConnections(),
      m_lostConnections(),
      m_messageAvailableConnections(),
      m_notifiedLostConnections(),
      m_status(OCANETWORKSTATUS_UNKNOWN),
      m_requestedInsecurePort(insecureListenPort),
      m_txtRecordList(txtRecords),
      m_pUdpNotificationSendSocket(-1),
      m_pDataBuffer(NULL),
      m_nodeID(),
      m_domainName(domainName),
      m_ocp1Interface(systemInterfaces),
      m_pMsgCmd(new ::Ocp1LiteMessageCommand),
      m_pMsgKeepAlive(new ::Ocp1LiteMessageKeepAlive),
      m_pMsgNtf(new ::Ocp1LiteMessageNotification),
      m_pMsgRsp(new ::Ocp1MessageResponse)
#ifdef _DEBUG
      ,m_bMsgCmd(false),
      m_bMsgKeepAlive(false),
      m_bMsgNtf(false),
      m_bMsgRsp(false)
#endif
{
    if (idAdvertised.GetStringValue().GetLength() > static_cast< ::OcaUint16>(0))
    {
        // Node ID is correctly passed
        m_nodeID = idAdvertised.GetStringValue();
    }
    else
    {
        // If node name is not correctly passed, take the device name.
        ::OcaLiteStatus rc(::OcaLiteDeviceManager::GetInstance().GetDeviceName(m_nodeID));
        if (OCASTATUS_OK != rc)
        {
            OCA_LOG_WARNING_PARAMS("Failed to get DeviceName (rc = %d)", rc);
        }
    }
}

Ocp1LiteUdpNetwork::~Ocp1LiteUdpNetwork()
{
    assert(NULL == m_pDataBuffer);
    assert(NULL == m_pMessageReceiveBuffer);
    delete m_pMsgCmd;
    delete m_pMsgKeepAlive;
    delete m_pMsgNtf;
    delete m_pMsgRsp;
}

::OcaBoolean Ocp1LiteUdpNetwork::Initialize()
{
    ::OcaBoolean bSuccess(OcaLiteNetwork::Initialize());

    if (bSuccess && (OCANETWORKSTATUS_UNKNOWN == m_status))
    {
        assert(NULL == m_pDataBuffer);
        assert(NULL == m_pMessageReceiveBuffer);

        // Data buffer
        m_pDataBuffer = new ::OcaUint8[static_cast< size_t>(OCA_BUFFER_SIZE)];
        m_pMessageReceiveBuffer = new ::OcaUint8[static_cast< size_t>(OCA_BUFFER_SIZE)];

        UINT32 interfaceIndex = static_cast< UINT32>(m_ocp1Interface.GetInterfaceIndex());
        UINT8 macAddressBuffer[MAC_ADDRESS_SIZE];
        IPParametersType ipParametersType;
        std::string ipAddress;
        UINT8 subnetMaskLength;
        std::string defaultGateway;
        std::string dnsServer;
        std::string domainName;
        UINT64 networkSpeed;
        bool isUp;

        if ((Ocp1LiteNetworkGetMACAddress(macAddressBuffer, MAC_ADDRESS_SIZE)) &&
            (Ocp1LiteNetworkGetNetworkConfiguration(ipParametersType, ipAddress,
                                                    subnetMaskLength, defaultGateway, dnsServer, domainName, networkSpeed, isUp)))
        {
            m_ocp1Interface = ::Ocp1LiteNetworkSystemInterfaceID(static_cast< ::OcaUint32>(interfaceIndex),
                                                                 ::OcaLiteString(ipAddress.c_str()),
                                                                 static_cast< ::OcaUint8>(subnetMaskLength),
                                                                 ::OcaLiteString(defaultGateway.c_str()),
                                                                 ::OcaLiteString(dnsServer.c_str()),
                                                                 ::OcaLiteString(domainName.c_str()),
                                                                 static_cast< ::OcaBoolean>(isUp),
                                                                 static_cast< ::OcaUint64>(networkSpeed),
                                                                 static_cast< ::Ocp1LiteIPParametersType>(ipParametersType),
                                                                 ::OcaLiteBlobFixedLen<MAC_ADDRESS_LENGTH>(static_cast< ::OcaUint8*>(&(macAddressBuffer[0]))));
        }
        else
        {
            bSuccess = static_cast< ::OcaBoolean>(false);
        }

        if (bSuccess)
        {
            m_status = OCANETWORKSTATUS_STOPPED;
        }
    }

    return bSuccess;
}

::OcaLiteStatus Ocp1LiteUdpNetwork::CreateMessageBuffer(void)
{
    // Data buffer
    if (NULL == m_pDataBuffer)
    {
        m_pDataBuffer = new ::OcaUint8[static_cast<size_t>(OCA_BUFFER_SIZE)];
    }

    if (NULL == m_pMessageReceiveBuffer)
    {
        m_pMessageReceiveBuffer = new ::OcaUint8[static_cast<size_t>(OCA_BUFFER_SIZE)];
    }

    return OCASTATUS_OK;
}

void Ocp1LiteUdpNetwork::Teardown()
{
    if ((m_status == OCANETWORKSTATUS_STOPPED) || (m_status == OCANETWORKSTATUS_UNKNOWN))
    {
        for (OcaSocketList::iterator sIter(m_ocaSocketList.begin()); sIter != m_ocaSocketList.end(); ++sIter)
        {
            sIter->second->Shutdown();
            delete sIter->second;
        }
        m_ocaSocketList.clear();

        if (NULL != m_pDataBuffer)
        {
            delete [] m_pDataBuffer;
            m_pDataBuffer = NULL;
        }

        if (NULL != m_pMessageReceiveBuffer)
        {
            delete[] m_pMessageReceiveBuffer;
            m_pMessageReceiveBuffer = NULL;
        }

        m_status = OCANETWORKSTATUS_UNKNOWN;
    }
    else
    {
        OCA_LOG_WARNING_PARAMS("Invalid network state %d, expected state %d", m_status, OCANETWORKSTATUS_STOPPED);
    }

    OcaLiteNetwork::Teardown();
}

::OcaLiteStatus Ocp1LiteUdpNetwork::GetStatus(::OcaLiteNetworkStatus& status) const
{
    status = m_status;
    return OCASTATUS_OK;
}

::OcaLiteStatus Ocp1LiteUdpNetwork::SetStatus(::OcaLiteNetworkStatus status)
{
    m_status = status;
    return OCASTATUS_OK;
}

::OcaLiteStatus Ocp1LiteUdpNetwork::Startup()
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    if (OCANETWORKSTATUS_STOPPED == m_status)
    {
        m_status = OCANETWORKSTATUS_STARTING_UP;
        bool bSuccess(true);

        if (-1 == m_pUdpListenSocket)
        {
            // Start listening to the insecure port.
            m_pUdpListenSocket = ::Ocp1LiteHostInterfaceRetrieveSocket(IPV4, PROTOCOL_UDP);
            if (-1 != m_pUdpListenSocket)
            {
                bSuccess = Ocp1LiteSocketOpen(m_pUdpListenSocket);
                bSuccess = bSuccess && Ocp1LiteSocketBind(m_pUdpListenSocket, static_cast<UINT16>(m_requestedInsecurePort));
                if (!bSuccess)
                {
                    OCA_LOG_ERROR("Failed to open/bind");

                    static_cast<void>(Ocp1LiteSocketClose(m_pUdpListenSocket));
                    m_pUdpListenSocket = -1;
                }
            }
            else
            {
                OCA_LOG_ERROR("Unable to retrieve TCP listen socket for insecure connections");
                bSuccess = false;
            }
        }

        if (bSuccess)
        {/*
            if (-1 == m_pUdpNotificationSendSocket)
            {
                m_pUdpNotificationSendSocket = ::Ocp1LiteHostInterfaceRetrieveSocket(IPV4, PROTOCOL_UDP);
                if (-1 != m_pUdpNotificationSendSocket)
                {
                    bSuccess = Ocp1LiteSocketOpen(m_pUdpNotificationSendSocket);
                    if (!bSuccess)
                    {
                        OCA_LOG_ERROR("Failed to open");

                        static_cast<void>(Ocp1LiteSocketClose(m_pUdpNotificationSendSocket));
                        m_pUdpNotificationSendSocket = -1;
                    }
                }
                else
                {
                    OCA_LOG_ERROR("Unable to retrieve UDP notification send socket");
                    bSuccess = false;
                }
            }
            */
        }

        if (bSuccess)
        {
            // Register on all network interfaces.
            rc = RegisterRegistrationServices();

            bSuccess = (OCASTATUS_OK == rc);
            if (!bSuccess)
            {
                Ocp1LiteServiceDispose();
            }
        }

        if (bSuccess)
        {
            m_status = OCANETWORKSTATUS_READY;
        }
        else
        {
            rc = OCASTATUS_PROCESSING_FAILED;
            m_status = OCANETWORKSTATUS_STOPPED;
        }
    }
    else
    {
        // Invalid state
        rc = OCASTATUS_PROCESSING_FAILED;
    }
    return rc;
}

::OcaLiteStatus Ocp1LiteUdpNetwork::Shutdown()
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    if (OCANETWORKSTATUS_READY == m_status)
    {
        Ocp1LiteServiceDispose();

        if (-1 != m_pUdpNotificationSendSocket)
        {
            static_cast<void>(Ocp1LiteSocketClose(m_pUdpNotificationSendSocket));
            m_pUdpNotificationSendSocket = -1;
        }

        if (-1 != m_pUdpListenSocket)
        {
            static_cast<void>(Ocp1LiteSocketClose(m_pUdpListenSocket));
            m_pUdpListenSocket = -1;
        }

        for (OcaSocketList::iterator sIter(m_ocaSocketList.begin()); sIter != m_ocaSocketList.end(); ++sIter)
        {
            sIter->second->Shutdown();
        }

        m_status = OCANETWORKSTATUS_STOPPED;
    }
    else
    {
        // Invalid state
        rc = OCASTATUS_PROCESSING_FAILED;
    }

    return rc;
}

void Ocp1LiteUdpNetwork::AddSelectables(INT32& highest,
                                        OcfLiteSelectableSet& readSet,
                                        OcfLiteSelectableSet& writeSet,
                                        OcfLiteSelectableSet& exceptSet)
{
    if (-1 != m_pUdpListenSocket)
    {
        FD_SET(m_pUdpListenSocket, &readSet);
        if (highest < m_pUdpListenSocket)
        {
            highest = m_pUdpListenSocket;
        }
    }

    OcaSocketList::iterator cIter(m_ocaSocketList.begin());
    while(cIter != m_ocaSocketList.end())
    {
        if (cIter->second->IsConnected())
        {
            INT32 controllerSocket(*cIter->second->GetSocket());
            if (-1 != controllerSocket)
            {
                FD_SET(controllerSocket, &readSet);
                if (highest < controllerSocket)
                {
                    highest = controllerSocket;
                }
            }
        }
        ++cIter;
    }
}

void Ocp1LiteUdpNetwork::HandleSelectables(const OcfLiteSelectableSet& readSet,
                                           const OcfLiteSelectableSet& writeSet,
                                           const OcfLiteSelectableSet& exceptSet)
{
    // Handle the controllers that want to connect via insecure link
    if ((-1 != m_pUdpListenSocket) &&
        FD_ISSET(m_pUdpListenSocket, &readSet))
    {
        std::string recvFromIpAddress;
        UINT16 recvFromPort(0);

        INT32 bytesReceived(Ocp1LiteSocketReceiveFrom(m_pUdpListenSocket, m_pMessageReceiveBuffer, static_cast<INT32>(OCA_BUFFER_SIZE), recvFromIpAddress, recvFromPort));
        if (bytesReceived > 0)
        {
            ::Ocp1LiteUdpSocketConnection* pConnection(FindConnection(recvFromIpAddress, recvFromPort));
            if (NULL == pConnection)
            {
                // A controller wants to connect to this device

                ::OcaSessionID newSessionID(::OcaLiteCommandHandler::GetInstance().CreateSessionID());
                ::Ocp1LiteUdpSocketConnection* sConnection(new ::Ocp1LiteUdpSocketConnection(*this, static_cast< ::OcaUint32>(OCA_BUFFER_SIZE)));
                if ((OCA_INVALID_SESSIONID != newSessionID) && (NULL != sConnection))
                {
                    sConnection->SetSocketConnectionParameters(newSessionID, static_cast< ::OcaUint16>(0));
                    sConnection->InitializeSocket(m_pUdpListenSocket);
                    // Copy the received udp datagram to corresponding connection class data member variable
                    sConnection->UpdateReceiveBuffer(m_pMessageReceiveBuffer, bytesReceived);
                    // Update the corresponding connection class remote ip address and remote port data member variable
                    sConnection->SetRemoteAddressAndPort(recvFromIpAddress, recvFromPort);
                    m_ocaSocketList[newSessionID] = sConnection;
                    m_newConnections.push_back(newSessionID);
                }
                else
                {
                    if (NULL != sConnection)
                    {
                        delete sConnection;
                    }
                    OCA_LOG_WARNING("New connection is rejected, since there are no more socket connections available");
                }
            }
            else
            {
                pConnection->UpdateReceiveBuffer(m_pMessageReceiveBuffer, bytesReceived);
            }
        }
    }
    
    HandleControllers(m_ocaSocketList, readSet);
}

::OcaBoolean Ocp1LiteUdpNetwork::HasPendingMessage() const
{
    ::OcaBoolean result(static_cast< ::OcaBoolean>(false));
    OcaSocketList::const_iterator cIter(m_ocaSocketList.begin());
    while(cIter != m_ocaSocketList.end())
    {
        if (cIter->second->HasPendingMessage())
        {
            result = static_cast< ::OcaBoolean>(true);
            break;
        }
        ++cIter;
    }
    return result;
}

void Ocp1LiteUdpNetwork::GetFirstPendingMessage(::OcaLiteMessageSessionID& messageSession, ::OcaBoolean& moreComing)
{
    messageSession.sessionID = OCA_INVALID_SESSIONID;
    messageSession.message = NULL;
    moreComing = static_cast< ::OcaBoolean>(true);

    while ((messageSession.message == NULL) && moreComing)
    {
        if (!m_messageAvailableConnections.empty())
        {
            ::OcaBoolean bContinue(static_cast< ::OcaBoolean>(false));
            ::OcaSessionID sessionId(*m_messageAvailableConnections.begin());
            ::Ocp1LiteUdpSocketConnection* pSocketConnection(FindSocketConnection(sessionId));
            if (NULL != pSocketConnection)
            {
                messageSession.message = pSocketConnection->GetFirstPendingMessage(bContinue);
                if (NULL != messageSession.message)
                {
                    messageSession.sessionID = sessionId;
                }
            }
            if (!bContinue)
            {
                static_cast<void>(m_messageAvailableConnections.erase(m_messageAvailableConnections.begin()));
            }
        }
        else
        {
            moreComing = static_cast< ::OcaBoolean>(false);
        }
    }
}

void Ocp1LiteUdpNetwork::GetNewConnections(::OcaSessionList& sessions)
{
    sessions = m_newConnections;
    m_newConnections.clear();
}

void Ocp1LiteUdpNetwork::GetConnectionsLost(::OcaSessionList& sessions)
{
    sessions = m_lostConnections;
    m_lostConnections.clear();
}

::OcaLiteStatus Ocp1LiteUdpNetwork::SendNotification(::OcaLiteNotificationDeliveryMode deliveryMode,
                                                  ::OcaSessionID sessionID,
                                                  const ::OcaLiteNetworkAddress& networkAddress,
                                                  const ::OcaLiteMessageNotification* message)
{
    ::OcaLiteStatus status(OCASTATUS_PARAMETER_ERROR);

    if (OCANOTIFICATIONDELIVERYMODE_RELIABLE == deliveryMode)
    {
        // Use the socket connection to send this message
        status = SendOcaMessage(sessionID, *message);
    }
    else if (OCANOTIFICATIONDELIVERYMODE_FAST == deliveryMode)
    {
        // Use the notification socket to send this message
        if ((-1 != m_pUdpNotificationSendSocket) &&
            (NULL != m_pDataBuffer))
        {
            ::Ocp1LiteNetworkAddress* ocp1NetworkAddress(::Ocp1LiteNetworkAddress::CreateFromBase(networkAddress));

            if (NULL != ocp1NetworkAddress)
            {
                status = OCASTATUS_OK;
                    
                ::Ocp1LiteMessageHeader header;
                header.WriteOcp1Parameters(message->GetMessageType(),
                                            static_cast< ::OcaUint16>(1),
                                            header.GetSize(m_ocp1Writer) + message->GetSize(m_ocp1Writer));

                if (header.GetMessageSize() <= static_cast< ::OcaUint32>(OCA_BUFFER_SIZE))
                {
                    ::OcaUint8* pB(m_pDataBuffer);
                    m_ocp1Writer.Write(::Ocp1LiteUdpSocketConnection::MESSAGESYNCVAL, &pB);
                    header.Marshal(&pB, m_ocp1Writer);
                    message->Marshal(&pB, m_ocp1Writer);
                    INT32 size(static_cast<INT32>(sizeof(::Ocp1LiteUdpSocketConnection::MESSAGESYNCVAL)) + static_cast<INT32>(header.GetMessageSize()));
                    if (0 > Ocp1LiteSocketSendTo(m_pUdpNotificationSendSocket, m_pDataBuffer, size, ocp1NetworkAddress->GetDestHostOrIPAddress().GetString(), static_cast<UINT16>(ocp1NetworkAddress->GetPort())))
                    {
                        status = OCASTATUS_DEVICE_ERROR;
                    }
                }

                delete ocp1NetworkAddress;
            }
            else
            {
                OCA_LOG_WARNING("Incorrect network address for fast notification delivery");
            }
        }
        else
        {
            OCA_LOG_WARNING("Unable to deliver fast notification");
        }
    }

    return status;
}

::OcaLiteMessageGeneral* Ocp1LiteUdpNetwork::RetrieveMessage(::OcaLiteHeader::OcaLiteMessageType msgType)
{
    ::OcaLiteMessageGeneral* result(NULL);

    switch (msgType)
    {
    case ::OcaLiteHeader::OCA_MSG_CMD:
    case ::OcaLiteHeader::OCA_MSG_CMD_RRQ:
#ifdef _DEBUG
        if (!m_bMsgCmd)
        {
#endif
            result = m_pMsgCmd;
            result->SetMessageType(msgType);
#ifdef _DEBUG
            m_bMsgCmd = true;
        }
        else
        {
            OCA_LOG_ERROR("No command message available");
        }
#endif
        break;
    case ::OcaLiteHeader::OCA_MSG_KEEP_ALIVE:
#ifdef _DEBUG
        if (!m_bMsgKeepAlive)
        {
#endif
            result = m_pMsgKeepAlive;
#ifdef _DEBUG
            m_bMsgKeepAlive = true;
        }
        else
        {
            OCA_LOG_ERROR("No keepalive message available");
        }
#endif
        break;
    case ::OcaLiteHeader::OCA_MSG_NTF:
#ifdef _DEBUG
        if (!m_bMsgNtf)
        {
#endif
            result = m_pMsgNtf;
#ifdef _DEBUG
            m_bMsgNtf = true;
        }
        else
        {
            OCA_LOG_ERROR("No notification message available");
        }
#endif
        break;
    case ::OcaLiteHeader::OCA_MSG_RSP:
#ifdef _DEBUG
        if (!m_bMsgRsp)
        {
#endif
            result = m_pMsgRsp;
#ifdef _DEBUG
            m_bMsgRsp = true;
        }
        else
        {
            OCA_LOG_ERROR("No message response available");
        }
#endif
        break;
    default:
        OCA_LOG_ERROR_PARAMS("Unknown message type %d requested", msgType);
        break;
    }

    return result;
}

void Ocp1LiteUdpNetwork::ReturnMessage(::OcaLiteMessageGeneral* msg)
{
#ifdef _DEBUG
    if (NULL != msg)
    {
        switch (msg->GetMessageType())
        {
        case ::OcaLiteHeader::OCA_MSG_CMD:
        case ::OcaLiteHeader::OCA_MSG_CMD_RRQ:
            {
                if (m_bMsgCmd)
                {
                    m_bMsgCmd = false;
                }
                else
                {
                    OCA_LOG_ERROR("Returning incorrect message. Command message not in use");
                }
            }
            break;
        case ::OcaLiteHeader::OCA_MSG_KEEP_ALIVE:
            {
                if (m_bMsgKeepAlive)
                {
                    m_bMsgKeepAlive = false;
                }
                else
                {
                    OCA_LOG_ERROR("Returning incorrect message. Keepalive message not in use");
                }
            }
            break;
        case ::OcaLiteHeader::OCA_MSG_NTF:
            {
                if (m_bMsgNtf)
                {
                    m_bMsgNtf = false;
                }
                else
                {
                    OCA_LOG_ERROR("Returning incorrect message. Notification message not in use");
                }
            }
            break;
        case ::OcaLiteHeader::OCA_MSG_RSP:
            {
                if (m_bMsgRsp)
                {
                    m_bMsgRsp = false;
                }
                else
                {
                    OCA_LOG_ERROR("Returning incorrect message. Response message not in use");
                }
            }
            break;
        default:
            assert(false);  // Should not happen
            break;
        }
    }
#endif
}

::OcaLiteStatus Ocp1LiteUdpNetwork::SendOcaMessage(::OcaSessionID sessionID, const ::OcaLiteMessageGeneral& ocaMessage)
{
    ::OcaLiteStatus result(OCASTATUS_DEVICE_ERROR);

    ::Ocp1LiteUdpSocketConnection* socketConnection(FindSocketConnection(sessionID));

    if (NULL != socketConnection)
    {
        result = socketConnection->SendOcaMessage(&ocaMessage, m_ocp1Writer, static_cast< ::OcaUint32>(OCA_BUFFER_SIZE), m_pDataBuffer);
    }
    return result;
}

::Ocp1LiteUdpSocketConnection* Ocp1LiteUdpNetwork::FindSocketConnection(::OcaSessionID sessionID)
{
    ::Ocp1LiteUdpSocketConnection* socketConnection(NULL);

    OcaSocketList::iterator cIter(m_ocaSocketList.find(sessionID));
    if (cIter != m_ocaSocketList.end())
    {
        socketConnection = cIter->second;
    }

    return socketConnection;
}

::OcaLiteStatus Ocp1LiteUdpNetwork::GetSystemInterfacesValue(::OcaLiteList< ::OcaLiteNetworkSystemInterfaceID>& interfaces) const
{
    interfaces.Clear();
    interfaces.Add(m_ocp1Interface);

    return OCASTATUS_OK;
}

::OcaLiteStatus Ocp1LiteUdpNetwork::GetIdAdvertisedValue(::OcaLiteNetworkNodeID& nodeId) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);
    nodeId = ::Ocp1LiteNetworkNodeID(m_nodeID);
    return rc;
}

void Ocp1LiteUdpNetwork::HandleControllers(OcaSocketList& controllerList, const OcfLiteSelectableSet& readSet)
{
    OcaSocketList::iterator cIter(controllerList.begin());
    while (cIter != controllerList.end())
    {
        ::OcaSessionID sessionID(cIter->first);
        ::Ocp1LiteUdpSocketConnection* pSocketConnection(cIter->second);
        ::OcaBoolean dataAvailable(static_cast< ::OcaBoolean>((FD_ISSET(*(pSocketConnection->GetSocket()), &readSet)) != 0) ? true : false);
        if (pSocketConnection->IsConnected())
        {
            switch (pSocketConnection->Run(dataAvailable, static_cast< ::OcaUint32>(OCA_BUFFER_SIZE), m_pDataBuffer))
            {
                case ::Ocp1LiteUdpSocketConnection::NO_MESSAGE_RECEIVED:
                    ++cIter;
                    break;
                case ::Ocp1LiteUdpSocketConnection::MESSAGE_RECEIVED:
                    {
                        // One or more messages were received, will be read-out later
                        m_messageAvailableConnections.push_back(sessionID);
                        ++cIter;
                    }
                    break;
                case ::Ocp1LiteUdpSocketConnection::CONNECTION_CORRUPT:
                case ::Ocp1LiteUdpSocketConnection::CONNECTION_LOST:
                    {
                        // Add session id to list of lost connections
                        if (m_notifiedLostConnections.end() == std::find(m_notifiedLostConnections.begin(), m_notifiedLostConnections.end(), sessionID))
                        {
                            assert(m_lostConnections.end() == std::find(m_lostConnections.begin(), m_lostConnections.end(), sessionID));
                            m_lostConnections.push_back(sessionID);
                        }

                        // Shutdown the socket connection
                        pSocketConnection->Shutdown();

                        delete pSocketConnection;
                        OcaSocketList::iterator iterCopy(cIter);
                        ++cIter;
                        static_cast<void>(controllerList.erase(iterCopy));
                    }
                    break;
                default:
                    OCA_LOG_ERROR_PARAMS("Unknown return from Ocp1LiteUdpSocketConnection::Run for controller %i", sessionID);
                    assert(false);
                    break;
            }
        }
        else
        {
            // Add session id to list of lost connections
            if (m_notifiedLostConnections.end() == std::find(m_notifiedLostConnections.begin(), m_notifiedLostConnections.end(), sessionID))
            {
                assert(m_lostConnections.end() == std::find(m_lostConnections.begin(), m_lostConnections.end(), sessionID));
                m_lostConnections.push_back(sessionID);
            }

            // Shutdown the socket connection
            pSocketConnection->Shutdown();

            delete pSocketConnection;
            OcaSocketList::iterator iterCopy(cIter);
            ++cIter;
            static_cast<void>(controllerList.erase(iterCopy));
        }
    }
}

::OcaLiteStatus Ocp1LiteUdpNetwork::FillTxtRecords(std::vector<std::string>& txtRecords) const
{
    // Make sure the data is empty
    txtRecords.clear();

    // Reserve space for the application specific TXT data and the protocol required data
    txtRecords.reserve(2 + m_txtRecordList.size());

    // Get the protocol version
    ::OcaUint16 ocaVersion(static_cast< ::OcaUint16>(0));
    ::OcaLiteStatus rc(::OcaLiteDeviceManager::GetInstance().GetOcaVersion(ocaVersion));

    if (OCASTATUS_OK == rc)
    {
        txtRecords.push_back("txtvers=1");
        char protoversion[50];
        static_cast<void>(::sprintf(protoversion, "protovers=%u", ocaVersion));
        txtRecords.push_back(std::string(protoversion));

        // Add the user txt records
        static_cast<void>(txtRecords.insert(txtRecords.end(), m_txtRecordList.begin(), m_txtRecordList.end()));
    }

    return rc;
}

::OcaLiteStatus Ocp1LiteUdpNetwork::RegisterRegistrationServices()
{
    std::vector<std::string> txtRecords;
    ::OcaLiteStatus rc(FillTxtRecords(txtRecords));
    if (OCASTATUS_OK == rc)
    {
        if (!Ocp1LiteServiceRegister(m_nodeID.GetString(),
                                     INSECURE_REGISTRATION_TYPE,
                                     static_cast<UINT16>(m_requestedInsecurePort),
                                     txtRecords,
                                     m_domainName.GetLength() > static_cast< ::OcaUint16>(0) ?
                                         m_domainName.GetString() : m_ocp1Interface.GetDnsDomainName().GetString()))
        {
            OCA_LOG_ERROR("Failed to register service");

            Ocp1LiteServiceDispose();
            
            rc = OCASTATUS_PROCESSING_FAILED;
        }
    }

    return rc;
}

::Ocp1LiteUdpSocketConnection* Ocp1LiteUdpNetwork::FindConnection(const std::string& ipAddress, const UINT16 port)
{
    ::Ocp1LiteUdpSocketConnection* pConnection(NULL);

    OcaSocketList::const_iterator cIter(m_ocaSocketList.begin());
    while (cIter != m_ocaSocketList.end())
    {
        if ((0 == cIter->second->GetRemoteIpAddress().compare(ipAddress)) &&
            ((cIter->second)->GetRemotePort() == port))
        {
            pConnection = cIter->second;
            break;
        }
        ++cIter;
    }
    return pConnection;
}
