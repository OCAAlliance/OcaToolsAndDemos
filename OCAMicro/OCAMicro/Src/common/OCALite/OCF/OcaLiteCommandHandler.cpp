/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The OcaLiteCommandHandler.
 *
 */

// ---- Include system wide include files ----
#include <assert.h>
#include <stdlib.h>
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <OCC/ControlClasses/Agents/OcaLiteNetwork.h>
#include <OCC/ControlClasses/Managers/OcaLiteDeviceManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteNetworkManager.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlDataTypes/OcaLiteEventData.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteCommandHandler.h"
#include "Messages/OcaLiteMessageCommand.h"
#include "Messages/OcaLiteMessageNotification.h"

// ---- Helper types and constants ----

/** The singleton reference to the server. */
::OcaLiteCommandHandler* OcaLiteCommandHandler::m_pSingleton(NULL);

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteCommandHandler::OcaLiteCommandHandler()
    : m_ocaNetworkSessions(),
      m_pResponseBuffer(new UINT8[OCA_BUFFER_SIZE]),
      m_responseBufferSize(0),
      m_sessionId(OCA_INVALID_SESSIONID),
      m_pConnectionEstablishedDelegate(NULL),
      m_pConnectionLostDelegate(NULL),
      m_sessionList()
{
}

OcaLiteCommandHandler::~OcaLiteCommandHandler()
{
    m_pConnectionEstablishedDelegate = NULL;
    m_pConnectionLostDelegate = NULL;
}

::OcaLiteCommandHandler& OcaLiteCommandHandler::GetInstance()
{
    if (NULL == OcaLiteCommandHandler::m_pSingleton)
    {
        OcaLiteCommandHandler::m_pSingleton = new OcaLiteCommandHandler();
    }

    return *OcaLiteCommandHandler::m_pSingleton;
}

void OcaLiteCommandHandler::FreeInstance()
{
    if (OcaLiteCommandHandler::m_pSingleton != NULL)
    {
        delete m_pSingleton;
        m_pSingleton = NULL;
    }
}

::OcaBoolean OcaLiteCommandHandler::Initialize()
{
    return static_cast< ::OcaBoolean>(true);
}

void OcaLiteCommandHandler::Shutdown()
{
    // Clear connected controller list
    m_ocaNetworkSessions.clear();
}

void OcaLiteCommandHandler::RunWithTimeout(::OcaUint32 timeout)
{
    // Handle networks
    HandleNetworks(timeout);

    // Handle messages from networks
    HandleMessages();

}

bool OcaLiteCommandHandler::AddSelectables(INT32& highest, OcfLiteSelectableSet& readSet, OcfLiteSelectableSet& writeSet, OcfLiteSelectableSet& exceptSet)
{
    bool bPending(false);

    // Let the network add the selectables to the sets
    ::OcaLiteNetwork* pNetwork(::OcaLiteNetworkManager::GetInstance().GetNetwork());
    if (NULL != pNetwork)
    {
        pNetwork->AddSelectables(highest, readSet, writeSet, exceptSet);

        bPending = pNetwork->HasPendingMessage();
    }

    return bPending;
}

void OcaLiteCommandHandler::RunWithSelectSet(OcfLiteSelectableSet readSet, OcfLiteSelectableSet writeSet, OcfLiteSelectableSet exceptSet)
{
    // Let the network add the selectables to the sets
    ::OcaLiteNetwork* pNetwork(::OcaLiteNetworkManager::GetInstance().GetNetwork());
    if (NULL != pNetwork)
    {
        // Let the network handle the selected objects
        pNetwork->HandleSelectables(readSet, writeSet, exceptSet);

        // Update controller list with new connections
        pNetwork->GetNewConnections(m_sessionList);
        for (::OcaSessionList::iterator sessionIter(m_sessionList.begin()); sessionIter != m_sessionList.end(); ++sessionIter)
        {
            // Add to session administration (if not already present)
            if (m_ocaNetworkSessions.end() == std::find(m_ocaNetworkSessions.begin(), m_ocaNetworkSessions.end(), *sessionIter))
            {
                static_cast<void>(m_ocaNetworkSessions.push_back(*sessionIter));
            }

            // Raise the ConnectionEstablished event
            ConnectionEstablished(*sessionIter);
        }

        // Update controller list with lost connections
        pNetwork->GetConnectionsLost(m_sessionList);
        for (::OcaSessionList::iterator sessionIter(m_sessionList.begin()); sessionIter != m_sessionList.end(); ++sessionIter)
        {
            // Notify OcaLiteDeviceManager and OcaLiteBlock
            ::OcaLiteDeviceManager::GetInstance().SessionLost(*sessionIter);
            ::OcaLiteBlock::GetRootBlock().SessionLost(*sessionIter);

            // Raise the ConnectionLost event
            ConnectionLost(*sessionIter);

            // Remove from session administration
            static_cast<void>(m_ocaNetworkSessions.erase(std::find(m_ocaNetworkSessions.begin(), m_ocaNetworkSessions.end(), *sessionIter)));
        }
    }

    // Handle messages from networks
    HandleMessages();
}

void OcaLiteCommandHandler::HandleNetworks(::OcaUint32 timeout)
{
    OcfLiteSelectableSet readSet;
    OcfLiteSelectableSet writeSet;
    OcfLiteSelectableSet exceptSet;
    bool pendingMessageAvailable(false);

    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);
    FD_ZERO(&exceptSet);

    // Let the network add the selectables to the sets
    ::OcaLiteNetwork* pNetwork(::OcaLiteNetworkManager::GetInstance().GetNetwork());
    if (NULL != pNetwork)
    {
        INT32 highest(0);
        pNetwork->AddSelectables(highest, readSet, writeSet, exceptSet);
        pendingMessageAvailable = static_cast<bool>(pNetwork->HasPendingMessage());

        // Perform a select on all the selectable objects. We are not interested in the actual result
        // and will always call the network to handle the selected objects. This is done so the network
        // can perform processing that needs to be done on a regular basis.
        static_cast<void>(::OcfLiteHostInterfaceSelect(highest,
                                                       readSet,
                                                       writeSet,
                                                       exceptSet,
                                                       pendingMessageAvailable ? 0 : static_cast<INT32>(timeout)));
   
        // Let the network handle the selected objects
        pNetwork->HandleSelectables(readSet, writeSet, exceptSet);

        // Update controller list with new connections
        pNetwork->GetNewConnections(m_sessionList);
        for (::OcaSessionList::iterator sessionIter(m_sessionList.begin()); sessionIter != m_sessionList.end(); ++sessionIter)
        {
            // Add to session administration (if not already present)
            if (m_ocaNetworkSessions.end() == std::find(m_ocaNetworkSessions.begin(), m_ocaNetworkSessions.end(), *sessionIter))
            {
                static_cast<void>(m_ocaNetworkSessions.push_back(*sessionIter));
            }

            // Raise the ConnectionEstablished event
            ConnectionEstablished(*sessionIter);
        }

        // Update controller list with lost connections
        pNetwork->GetConnectionsLost(m_sessionList);
        for (::OcaSessionList::iterator sessionIter(m_sessionList.begin()); sessionIter != m_sessionList.end(); ++sessionIter)
        {
            // Notify OcaLiteDeviceManager and OcaLiteBlock
            ::OcaLiteDeviceManager::GetInstance().SessionLost(*sessionIter);
            ::OcaLiteBlock::GetRootBlock().SessionLost(*sessionIter);

            // Raise the ConnectionLost event
            ConnectionLost(*sessionIter);

            // Remove from session administration
            static_cast<void>(m_ocaNetworkSessions.erase(std::find(m_ocaNetworkSessions.begin(), m_ocaNetworkSessions.end(), *sessionIter)));
        }
    }
}



::OcaBoolean OcaLiteCommandHandler::SendNotification(::OcaLiteNotificationDeliveryMode deliveryMode,
                                                     ::OcaSessionID sessionID,
                                                     const ::OcaLiteNotificationData& notification,
                                                     const ::OcaLiteNetworkAddress& networkAddress) const
{
    bool bSuccess(false);

    // Find network and send notification
    ::OcaLiteNetwork* pOcaLiteNetwork(::OcaLiteNetworkManager::GetInstance().GetNetwork());
    if (NULL != pOcaLiteNetwork)
    {
        // Retrieve a notification message
        ::OcaLiteMessageGeneral* pMsg(pOcaLiteNetwork->RetrieveMessage(::OcaLiteHeader::OCA_MSG_NTF));
        if (NULL != pMsg)
        {
            ::OcaLiteMessageNotification* pMsgNotification(static_cast< ::OcaLiteMessageNotification*>(pMsg));
            if (pMsgNotification->WriteParameters(notification.GetTargetONo(), notification.GetMethodID(),
                                                  notification.GetContext(), *(notification.GetEventData()),
                                                  pOcaLiteNetwork->GetWriter()))
            {
                bSuccess = (OCASTATUS_OK == pOcaLiteNetwork->SendNotification(deliveryMode, sessionID,
                                                                              networkAddress, pMsgNotification));
            }
            pOcaLiteNetwork->ReturnMessage(const_cast< ::OcaLiteMessageNotification*>(pMsgNotification));
        }
    }
    else
    {
        OCA_LOG_ERROR_PARAMS("Unable to find network for sessionID %d", sessionID);
    }
    
    return static_cast< ::OcaBoolean>(bSuccess);
}

void OcaLiteCommandHandler::HandleMessages()
{
    // Check if new data is available to read
    ::OcaLiteNetwork* pNetwork(::OcaLiteNetworkManager::GetInstance().GetNetwork());
    if (NULL != pNetwork)
    {
        ::OcaBoolean bContinue(static_cast< ::OcaBoolean>(true));
        ::OcaLiteMessageSessionID ocaMessageSessionID;
        while (bContinue)
        {
            pNetwork->GetFirstPendingMessage(ocaMessageSessionID, bContinue);
            if (NULL != ocaMessageSessionID.message)
            {
                // One or more messages were received
                ::OcaLiteMessageGeneral* msg(ocaMessageSessionID.message);
                ::OcaSessionID sessionID(ocaMessageSessionID.sessionID);

                if (NULL != msg)
                {
                    switch (msg->GetMessageType())
                    {
                    case ::OcaLiteHeader::OCA_MSG_CMD:
                        {
                            ::OcaLiteMessageCommand* cmdMess(static_cast< ::OcaLiteMessageCommand*>(msg));

                            // Find the object to execute the command on
                            ::OcaLiteRoot* pOcaLiteRoot(GetDeviceObject(cmdMess->GetTargetONo()));

                            if (NULL != pOcaLiteRoot)
                            {
                                // Ignore return value, no response expected/required
                                OcaUint8* response(NULL);

                                ::OcaLiteStatus rc(pOcaLiteRoot->Execute(pNetwork->GetReader(),
                                                                         pNetwork->GetWriter(),
                                                                         sessionID,
                                                                         cmdMess->GetMethodID(),
                                                                         cmdMess->GetParametersSize(),
                                                                         cmdMess->GetParameters(),
                                                                         &response));

                                if ((OCASTATUS_OK != rc) && (OCASTATUS_NOT_IMPLEMENTED != rc)) // Not implemented is not an actual failure. Don't log this.
                                {
                                    OCA_LOG_ERROR_PARAMS("OCA_MSG_CMD failure execution rc = %i", rc);
                                }
                            }
                        }
                        break;
                    case ::OcaLiteHeader::OCA_MSG_CMD_RRQ:
                        {
                            const ::OcaLiteMessageCommand* cmdMess(static_cast< ::OcaLiteMessageCommand*>(msg));

                            // Find the object to execute the command on
                            ::OcaLiteRoot* pOcaLiteRoot(GetDeviceObject(cmdMess->GetTargetONo()));

                            // Retrieve a new response message
                            ::OcaLiteMessageResponse* pMsgResponse(static_cast< ::OcaLiteMessageResponse*>(pNetwork->RetrieveMessage(::OcaLiteHeader::OCA_MSG_RSP)));
                            
                            if (NULL != pMsgResponse)
                            {
                                if (NULL != pOcaLiteRoot)
                                {
                                    OcaUint8* response(NULL);

                                    ::OcaLiteStatus rc(pOcaLiteRoot->Execute(pNetwork->GetReader(),
                                                                             pNetwork->GetWriter(),
                                                                             sessionID,
                                                                             cmdMess->GetMethodID(),
                                                                             cmdMess->GetParametersSize(),
                                                                             cmdMess->GetParameters(),
                                                                             &response));
                                    if ((OCASTATUS_OK != rc) &&
                                        (OCASTATUS_NOT_IMPLEMENTED != rc))
                                    {
                                        OCA_LOG_ERROR_PARAMS("OCA_MSG_CMD_RRQ failure execution rc = %d (targetONo %d, method %d, %d)", 
                                            rc, cmdMess->GetTargetONo(), cmdMess->GetMethodID().GetDefLevel(), cmdMess->GetMethodID().GetMethodIndex());
                                    }
                                    
                                    // Clear the response parameters
                                    if (OCASTATUS_OK != rc)
                                    {
                                        pMsgResponse->WriteParameters(cmdMess->GetHandle(), NULL, 0, rc);
                                    }
                                    else
                                    {
                                        pMsgResponse->WriteParameters(cmdMess->GetHandle(), response, m_responseBufferSize, rc);
                                    }
                                }
                                else
                                {
                                    pMsgResponse->WriteParameters(cmdMess->GetHandle(), NULL, 0, OCASTATUS_BAD_ONO);
                                }

                                ::OcaLiteStatus responseStatus(pNetwork->SendOcaMessage(sessionID, *pMsgResponse));
                                if (OCASTATUS_OK != responseStatus)
                                {
                                    OCA_LOG_ERROR_PARAMS("Sending response for command with handle %u failed (rc = %d)",
                                                            cmdMess->GetHandle(), responseStatus);
                                }

                                pNetwork->ReturnMessage(pMsgResponse);
                            }
                        }
                        break;
                    case ::OcaLiteHeader::OCA_MSG_NTF:
                    case ::OcaLiteHeader::OCA_MSG_RSP:
                        break;
                    default:
                        break;
                    }

                    pNetwork->ReturnMessage(msg);
                }
            }
            else
            {
                bContinue = static_cast< ::OcaBoolean>(false);
            }
        }
    }
}

::OcaUint8* OcaLiteCommandHandler::GetResponseBuffer(::OcaUint32 bufferSize)
{
    if (bufferSize <= OCA_BUFFER_SIZE)
    {
        m_responseBufferSize = bufferSize;
        return m_pResponseBuffer;
    }
    else
    {
        // Response does not fit into response buffer.
        return NULL;
    }
}

::OcaSessionID OcaLiteCommandHandler::CreateSessionID()
{
    ::OcaSessionID lastSessionId(m_sessionId);
    m_sessionId++;
    while (((OCA_INVALID_SESSIONID == m_sessionId) ||
           ((m_ocaNetworkSessions.end() != std::find(m_ocaNetworkSessions.begin(), m_ocaNetworkSessions.end(), m_sessionId)))) &&
           (m_sessionId != lastSessionId))
    {
        m_sessionId++;
    }

    ::OcaSessionID newSessionId(OCA_INVALID_SESSIONID);
    if (m_sessionId != lastSessionId)
    {
        newSessionId = m_sessionId;
    }
    else
    {
        OCA_LOG_ERROR("No available session IDs");
    }

    return newSessionId;
}

void OcaLiteCommandHandler::ConnectionEstablished(::OcaSessionID sessionID)
{
    if (NULL != m_pConnectionEstablishedDelegate)
    {
        IConnectionEstablishedDelegate* pDelegate(m_pConnectionEstablishedDelegate);
        pDelegate->OnConnectionEstablished(sessionID);
    }
}

bool OcaLiteCommandHandler::RegisterConnectionEstablishedEventHandler(IConnectionEstablishedDelegate* connEstablishedDelegate)
{
    bool bSuccess(false);

    if (NULL == m_pConnectionEstablishedDelegate)
    {
        m_pConnectionEstablishedDelegate = connEstablishedDelegate;
        bSuccess = true;
    }
    return bSuccess;
}

bool OcaLiteCommandHandler::UnregisterConnectionEstablishedEventHandler(const IConnectionEstablishedDelegate* connEstablishedDelegate)
{
    bool bSuccess(false);

    if (m_pConnectionEstablishedDelegate == connEstablishedDelegate)
    {
        m_pConnectionEstablishedDelegate = NULL;
        bSuccess = true;
    }

    return bSuccess;
}

void OcaLiteCommandHandler::ConnectionLost(::OcaSessionID sessionID)
{
    if (NULL != m_pConnectionLostDelegate)
    {
        IConnectionLostDelegate* pDelegate(m_pConnectionLostDelegate);

        pDelegate->OnConnectionLost(sessionID);
    }
}

bool OcaLiteCommandHandler::RegisterConnectionLostEventHandler(IConnectionLostDelegate* connLostDelegate)
{
    bool bSuccess(false);

    if (NULL == m_pConnectionLostDelegate)
    {
        m_pConnectionLostDelegate = connLostDelegate;
        bSuccess = true;
    }

    return bSuccess;
}

bool OcaLiteCommandHandler::UnregisterConnectionLostEventHandler(const IConnectionLostDelegate* connLostDelegate)
{
    bool bSuccess(false);
    if (m_pConnectionLostDelegate == connLostDelegate)
    {
        m_pConnectionLostDelegate = NULL;
        bSuccess = true;
    }
    return bSuccess;
}

::OcaLiteRoot* OcaLiteCommandHandler::GetDeviceObject(::OcaONo ocaONo)
{
    ::OcaLiteRoot* pOcaLiteRoot(NULL);

    if ((ocaONo == OCA_ROOT_BLOCK_ONO) ||
        (ocaONo >= OCA_MINIMUM_DEVICE_OBJECT_ONO))
    {
        pOcaLiteRoot = OcaLiteBlock::GetRootBlock().GetObject(ocaONo);
    }
    else
    {
        pOcaLiteRoot = OcaLiteDeviceManager::GetInstance().GetManager(ocaONo);
    }

    return pOcaLiteRoot;
}

