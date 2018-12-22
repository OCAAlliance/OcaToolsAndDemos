/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Ocp1LiteNetwork
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
#include <OCP.1/Ocp1LiteConnectParameters.h>

// ---- Include local include files ----
#include "Ocp1LiteNetwork.h"
#include "Ocp1LiteNetworkSystemInterfaceID.h"

// ---- Helper types and constants ----
const UINT8 Ocp1LiteNetwork::SOCKETBACKLOG(10);

#define OCA_INSECURE_REGISTRATION_TYPE  "_oca._tcp"
#define OCA_LOCAL_REGISTRATION_DOMAIN   "local"

#ifndef OCP1_MAX_NR_CONNECTIONS
#define OCP1_MAX_NR_CONNECTIONS 4
#endif

const std::string Ocp1LiteNetwork::INSECURE_REGISTRATION_TYPE(OCA_INSECURE_REGISTRATION_TYPE);
const std::string Ocp1LiteNetwork::LOCAL_REGISTRATION_DOMAIN(OCA_LOCAL_REGISTRATION_DOMAIN);

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

Ocp1LiteNetwork::Ocp1LiteNetwork(::OcaONo objectNumber,
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
      m_pTcpListenSocket(-1),
#ifdef OCA_LITE_CONTROLLER
      m_ocaDeviceSocketList(),
#endif
      m_ocaSocketList(),
      m_newConnections(),
      m_lostConnections(),
      m_messageAvailableConnections(),
      m_notifiedLostConnections(),
#ifdef OCA_TRACK_KEEPALIVE_RECEIVED
      m_receivedKeepalives(),
#endif //OCA_TRACK_KEEPALIVE_RECEIVED
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

Ocp1LiteNetwork::~Ocp1LiteNetwork()
{
    assert(NULL == m_pDataBuffer);

    delete m_pMsgCmd;
    delete m_pMsgKeepAlive;
    delete m_pMsgNtf;
    delete m_pMsgRsp;
}

::OcaBoolean Ocp1LiteNetwork::Initialize()
{
    ::OcaBoolean bSuccess(OcaLiteNetwork::Initialize());

    if (bSuccess && (OCANETWORKSTATUS_UNKNOWN == m_status))
    {
        // Data buffer
        m_pDataBuffer = new ::OcaUint8[static_cast< size_t>(OCA_BUFFER_SIZE)];

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

::OcaLiteStatus Ocp1LiteNetwork::CreateMessageBuffer(void)
{
    // Data buffer
    m_pDataBuffer = new ::OcaUint8[static_cast< size_t>(OCA_BUFFER_SIZE)];

    return OCASTATUS_OK;
}

void Ocp1LiteNetwork::Teardown()
{
    if ((m_status == OCANETWORKSTATUS_STOPPED) || (m_status == OCANETWORKSTATUS_UNKNOWN))
    {
        for (OcaSocketList::iterator sIter(m_ocaSocketList.begin()); sIter != m_ocaSocketList.end(); ++sIter)
        {
            sIter->second->Shutdown();
            delete sIter->second;
        }
        m_ocaSocketList.clear();

#ifdef OCA_LITE_CONTROLLER
        for (OcaSocketList::iterator sIter(m_ocaDeviceSocketList.begin()); sIter != m_ocaDeviceSocketList.end(); ++sIter)
        {
            sIter->second->Shutdown();
            delete sIter->second;
        }
        m_ocaDeviceSocketList.clear();
#endif

        if (NULL != m_pDataBuffer)
        {
            delete [] m_pDataBuffer;
            m_pDataBuffer = NULL;
        }

        m_status = OCANETWORKSTATUS_UNKNOWN;
    }
    else
    {
        OCA_LOG_WARNING_PARAMS("Invalid network state %d, expected state %d", m_status, OCANETWORKSTATUS_STOPPED);
    }

    OcaLiteNetwork::Teardown();
}

::OcaLiteStatus Ocp1LiteNetwork::GetStatus(::OcaLiteNetworkStatus& status) const
{
    status = m_status;
    return OCASTATUS_OK;
}

::OcaLiteStatus Ocp1LiteNetwork::SetStatus(::OcaLiteNetworkStatus status)
{
    m_status = status;
    return OCASTATUS_OK;
}

::OcaLiteStatus Ocp1LiteNetwork::Startup()
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    if (OCANETWORKSTATUS_STOPPED == m_status)
    {
        m_status = OCANETWORKSTATUS_STARTING_UP;
        bool bSuccess(true);

        if (-1 == m_pTcpListenSocket)
        {
            // Start listening to the insecure port.
            m_pTcpListenSocket = ::Ocp1LiteHostInterfaceRetrieveSocket(IPV4, PROTOCOL_TCP);
            if (-1 != m_pTcpListenSocket)
            {
                bSuccess = Ocp1LiteSocketOpen(m_pTcpListenSocket);
                bSuccess = bSuccess && Ocp1LiteSocketBind(m_pTcpListenSocket, static_cast<UINT16>(m_requestedInsecurePort));
                bSuccess = bSuccess && Ocp1LiteSocketListen(m_pTcpListenSocket, SOCKETBACKLOG);
                if (!bSuccess)
                {
                    OCA_LOG_ERROR("Failed to open/bind/listen");

                    static_cast<void>(Ocp1LiteSocketClose(m_pTcpListenSocket));
                    m_pTcpListenSocket = -1;
                }
            }
            else
            {
                OCA_LOG_ERROR("Unable to retrieve TCP listen socket for insecure connections");
                bSuccess = false;
            }
        }

        if (bSuccess)
        {
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

::OcaLiteStatus Ocp1LiteNetwork::Shutdown()
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

        if (-1 != m_pTcpListenSocket)
        {
            static_cast<void>(Ocp1LiteSocketClose(m_pTcpListenSocket));
            m_pTcpListenSocket = -1;
        }

        for (OcaSocketList::iterator sIter(m_ocaSocketList.begin()); sIter != m_ocaSocketList.end(); ++sIter)
        {
            sIter->second->Shutdown();
        }

#ifdef OCA_LITE_CONTROLLER
        for (OcaSocketList::iterator sIter(m_ocaDeviceSocketList.begin()); sIter != m_ocaDeviceSocketList.end(); ++sIter)
        {
            sIter->second->Shutdown();
        }
#endif

        m_status = OCANETWORKSTATUS_STOPPED;
    }
    else
    {
        // Invalid state
        rc = OCASTATUS_PROCESSING_FAILED;
    }

    return rc;
}

void Ocp1LiteNetwork::AddSelectables(INT32& highest,
                                     OcfLiteSelectableSet& readSet,
                                     OcfLiteSelectableSet& writeSet,
                                     OcfLiteSelectableSet& exceptSet)
{
    if (-1 != m_pTcpListenSocket)
    {
        FD_SET(m_pTcpListenSocket, &readSet);
        if (highest < m_pTcpListenSocket)
        {
            highest = m_pTcpListenSocket;
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

#ifdef OCA_LITE_CONTROLLER
    cIter = m_ocaDeviceSocketList.begin();
    while (cIter != m_ocaDeviceSocketList.end())
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
#endif

}

void Ocp1LiteNetwork::HandleSelectables(const OcfLiteSelectableSet& readSet,
                                        const OcfLiteSelectableSet& writeSet,
                                        const OcfLiteSelectableSet& exceptSet)
{
    // Handle the controllers that want to connect via insecure link
    if ((-1 != m_pTcpListenSocket) &&
        FD_ISSET(m_pTcpListenSocket, &readSet))
    {
        // A controller wants to connect to this device

        // First try to get a new socket connection. Keep alive timeout must be 0. The remote controller will specify
        // the keep alive timeout.
        ::OcaSessionID newSessionID(::OcaLiteCommandHandler::GetInstance().CreateSessionID());
        ::Ocp1LiteSocketConnection* sConnection(new ::Ocp1LiteSocketConnection(*this, static_cast< ::OcaUint32>(OCA_BUFFER_SIZE)));
        if ((OCA_INVALID_SESSIONID != newSessionID) && (m_ocaSocketList.size() < OCP1_MAX_NR_CONNECTIONS))
        {
            sConnection->SetSocketConnectionParameters(newSessionID, static_cast< ::OcaUint16>(0));

            // Try to accept the connection
            if (Ocp1LiteSocketAccept(m_pTcpListenSocket, *(sConnection->GetSocket())))
            {
                m_ocaSocketList[newSessionID] = sConnection;
                m_newConnections.push_back(newSessionID);
            }
            else
            {
                // Accept failed, so delete the socket connection
                delete sConnection;

                OCA_LOG_WARNING("Accepting connection failed");
            }
        }
        else
        {
            delete sConnection;
            if (Ocp1LiteSocketReject(m_pTcpListenSocket))
            {
                OCA_LOG_WARNING("New connection is rejected, since there are no more socket connections available");
            }
            else
            {
                OCA_LOG_WARNING("Rejecting new connection failed");
            }
        }
    }
    
    HandleControllers(m_ocaSocketList, readSet);
#ifdef OCA_LITE_CONTROLLER
    HandleDevices(m_ocaDeviceSocketList, readSet);
#endif
}

::OcaBoolean Ocp1LiteNetwork::HasPendingMessage() const
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

void Ocp1LiteNetwork::GetFirstPendingMessage(::OcaLiteMessageSessionID& messageSession, ::OcaBoolean& moreComing)
{
    messageSession.sessionID = OCA_INVALID_SESSIONID;
    messageSession.message = NULL;
    moreComing = static_cast< ::OcaBoolean>(true);

    while ((messageSession.message == NULL) && moreComing)
    {
        if (!m_messageAvailableConnections.empty())
        {
            ::OcaSessionID sessionId(*m_messageAvailableConnections.begin());
            static_cast<void>(m_messageAvailableConnections.erase(m_messageAvailableConnections.begin()));

            ::Ocp1LiteSocketConnection* pSocketConnection(FindSocketConnection(sessionId));
            if (NULL != pSocketConnection)
            {
                messageSession.message = pSocketConnection->GetFirstPendingMessage();
                if (NULL != messageSession.message)
                {
                    messageSession.sessionID = sessionId;
                }
            }
        }
        else 
        {
            moreComing = static_cast< ::OcaBoolean>(false);
        }
    }
}

void Ocp1LiteNetwork::GetNewConnections(::OcaSessionList& sessions)
{
    sessions = m_newConnections;
    m_newConnections.clear();
}

void Ocp1LiteNetwork::GetConnectionsLost(::OcaSessionList& sessions)
{
    sessions = m_lostConnections;
    m_lostConnections.clear();
}

#ifdef OCA_TRACK_KEEPALIVE_RECEIVED
void Ocp1LiteNetwork::GetReceivedKeepAlives(::OcaSessionList& sessions)
{
    sessions = m_receivedKeepalives;
    m_receivedKeepalives.clear();
}
#endif

::OcaLiteStatus Ocp1LiteNetwork::SendNotification(::OcaLiteNotificationDeliveryMode deliveryMode,
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
                    m_ocp1Writer.Write(::Ocp1LiteSocketConnection::MESSAGESYNCVAL, &pB);
                    header.Marshal(&pB, m_ocp1Writer);
                    message->Marshal(&pB, m_ocp1Writer);
                    INT32 size(static_cast<INT32>(sizeof(::Ocp1LiteSocketConnection::MESSAGESYNCVAL)) + static_cast<INT32>(header.GetMessageSize()));
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

#ifdef OCA_LITE_CONTROLLER
::OcaLiteStatus Ocp1LiteNetwork::SendResetMessage(::OcaLiteBlobFixedLen<static_cast< ::OcaUint16>(16)> resetKey,
                                                  const ::OcaLiteNetworkAddress networkAddress)
{
    ::OcaLiteStatus status(OCASTATUS_PROCESSING_FAILED);

    // Use the notification socket to send this message
    if ((-1 != m_pUdpNotificationSendSocket) &&
        (NULL != m_pDataBuffer))
    {

        ::Ocp1LiteNetworkAddress* ocp1NetworkAddress(::Ocp1LiteNetworkAddress::CreateFromBase(networkAddress));

        if (NULL != ocp1NetworkAddress)
        {
            // Write sentinel DEAF DADA CAFE BABE
            ::OcaUint8* pMessage(m_pDataBuffer);
            GetWriter().Write(static_cast< ::OcaUint16>(0xDEAF), &pMessage);
            GetWriter().Write(static_cast< ::OcaUint16>(0xDADA), &pMessage);
            GetWriter().Write(static_cast< ::OcaUint16>(0xCAFE), &pMessage);
            GetWriter().Write(static_cast< ::OcaUint16>(0xBABE), &pMessage);

            // Write Key
            resetKey.Marshal(&pMessage, m_ocp1Writer);

            // Send message
            INT32 size(static_cast<INT32>(pMessage - m_pDataBuffer));
            if (0 > Ocp1LiteSocketSendTo(m_pUdpNotificationSendSocket, 
                                         m_pDataBuffer, 
                                         size, 
                                         ocp1NetworkAddress->GetDestHostOrIPAddress().GetString(), 
                                         static_cast<UINT16>(ocp1NetworkAddress->GetPort())))
            {
                status = OCASTATUS_DEVICE_ERROR;
            }
            else
            {
                status = OCASTATUS_OK;
            }
        }
    }

    return status;
}
#endif //OCA_LITE_CONTROLLER

::OcaLiteMessageGeneral* Ocp1LiteNetwork::RetrieveMessage(::OcaLiteHeader::OcaLiteMessageType msgType)
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

void Ocp1LiteNetwork::ReturnMessage(::OcaLiteMessageGeneral* msg)
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

#ifdef OCA_LITE_CONTROLLER
OcaSessionID Ocp1LiteNetwork::Connect(const ::OcaLiteConnectParameters& connectParameters)
{
    ::OcaSessionID sessionId(OCA_INVALID_SESSIONID);

    const ::Ocp1LiteConnectParameters* ocp1ConnectParameters(static_cast<const ::Ocp1LiteConnectParameters*>(&connectParameters));
    if (NULL != ocp1ConnectParameters)
    {
        ::OcaSessionID newSessionID(::OcaLiteCommandHandler::GetInstance().CreateSessionID());
        ::Ocp1LiteSocketConnection* sConnection(new ::Ocp1LiteSocketConnection(*this, static_cast< ::OcaUint32>(OCA_BUFFER_SIZE)));
        sConnection->SetSocketConnectionParameters(newSessionID, 
                                                   ocp1ConnectParameters->GetKeepAliveTimeout(), 
                                                   ocp1ConnectParameters->GetbKeepAliveTimeoutInMs());

        if (Ocp1LiteSocketConnect(ocp1ConnectParameters->GetHostOrIp().c_str(),
                                  ocp1ConnectParameters->GetPort(),
                                  *sConnection->GetSocket()))
        {
            sessionId = newSessionID;
            // Run, this forces a keep alive
            ::OcaBoolean bReceivedKeepAlive;
            static_cast<void>(sConnection->Run(false, static_cast< ::OcaUint32>(OCA_BUFFER_SIZE), m_pDataBuffer, bReceivedKeepAlive));
            m_ocaDeviceSocketList[newSessionID] = sConnection;
            m_newConnections.push_back(newSessionID);
        }
        else
        {
            // Connect failed, so delete the socket connection.
            delete sConnection;

            OCA_LOG_WARNING("Connecting failed");

        }
    }

    return sessionId;
}

bool Ocp1LiteNetwork::Disconnect(::OcaSessionID sessionID)
{
    bool bResult(false);

    OcaSocketList::iterator cIter(m_ocaDeviceSocketList.find(sessionID));
    if (cIter != m_ocaDeviceSocketList.end())
    {
        cIter->second->Shutdown();
        static_cast<void>(m_ocaDeviceSocketList.erase(cIter));

        m_lostConnections.push_back(sessionID);

        bResult = true;
    }

    return bResult;
}
#endif

::OcaLiteStatus Ocp1LiteNetwork::SendOcaMessage(::OcaSessionID sessionID, const ::OcaLiteMessageGeneral& ocaMessage)
{
    ::OcaLiteStatus result(OCASTATUS_DEVICE_ERROR);

    ::Ocp1LiteSocketConnection* socketConnection(FindSocketConnection(sessionID));

    if (NULL != socketConnection)
    {
        result = socketConnection->SendOcaMessage(&ocaMessage, m_ocp1Writer, static_cast< ::OcaUint32>(OCA_BUFFER_SIZE), m_pDataBuffer);
    }
    return result;
}

::Ocp1LiteSocketConnection* Ocp1LiteNetwork::FindSocketConnection(::OcaSessionID sessionID)
{
    ::Ocp1LiteSocketConnection* socketConnection(NULL);

    OcaSocketList::iterator cIter(m_ocaSocketList.find(sessionID));
    if (cIter != m_ocaSocketList.end())
    {
        socketConnection = cIter->second;
    }
#ifdef OCA_LITE_CONTROLLER
    else
    {
        cIter = m_ocaDeviceSocketList.find(sessionID);
        if (cIter != m_ocaDeviceSocketList.end())
        {
            socketConnection = cIter->second;
        }
    }
#endif

    return socketConnection;
}

::OcaLiteStatus Ocp1LiteNetwork::GetSystemInterfacesValue(::OcaLiteList< ::OcaLiteNetworkSystemInterfaceID>& interfaces) const
{
    interfaces.Clear();
    interfaces.Add(m_ocp1Interface);

    return OCASTATUS_OK;
}

::OcaLiteStatus Ocp1LiteNetwork::GetIdAdvertisedValue(::OcaLiteNetworkNodeID& nodeId) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);
    nodeId = ::Ocp1LiteNetworkNodeID(m_nodeID);
    return rc;
}

void Ocp1LiteNetwork::HandleControllers(OcaSocketList& controllerList, const OcfLiteSelectableSet& readSet)
{
    OcaSocketList::iterator cIter(controllerList.begin());
    while (cIter != controllerList.end())
    {
        ::OcaSessionID sessionID(cIter->first);
        ::Ocp1LiteSocketConnection* pSocketConnection(cIter->second);
        ::OcaBoolean dataAvailable(static_cast< ::OcaBoolean>((FD_ISSET(*(pSocketConnection->GetSocket()), &readSet)) != 0) ? true : false);
        if (pSocketConnection->IsConnected())
        {
            ::OcaBoolean bReceivedKeepAlive;
            switch (pSocketConnection->Run(dataAvailable, static_cast< ::OcaUint32>(OCA_BUFFER_SIZE), m_pDataBuffer, bReceivedKeepAlive))
            {
                case ::Ocp1LiteSocketConnection::NO_MESSAGE_RECEIVED:
                    ++cIter;
                    break;
                case ::Ocp1LiteSocketConnection::MESSAGE_RECEIVED:
                    {
                        // One or more messages were received, will be read-out later
                        m_messageAvailableConnections.push_back(sessionID);
                        ++cIter;
                    }
                    break;
                case ::Ocp1LiteSocketConnection::CONNECTION_CORRUPT:
                case ::Ocp1LiteSocketConnection::CONNECTION_LOST:
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
                    OCA_LOG_ERROR_PARAMS("Unknown return from Ocp1LiteSocketConnection::Run for controller %i", sessionID);
                    assert(false);
                    break;
            }

#ifdef OCA_TRACK_KEEPALIVE_RECEIVED
            if (bReceivedKeepAlive)
            {
                // Add session id to list of received keepalives
                if (m_receivedKeepalives.end() == std::find(m_receivedKeepalives.begin(), m_receivedKeepalives.end(), sessionID))
                {
                    m_receivedKeepalives.push_back(sessionID);
                }
            }
#endif //OCA_TRACK_KEEPALIVE_RECEIVED
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

#ifdef OCA_LITE_CONTROLLER
void Ocp1LiteNetwork::HandleDevices(OcaSocketList& deviceList, const OcfLiteSelectableSet& readSet)
{
    OcaSocketList::iterator cIter(deviceList.begin());
    while (cIter != deviceList.end())
    {
        ::OcaSessionID sessionID(cIter->first);
        ::Ocp1LiteSocketConnection* pSocketConnection(cIter->second);
        ::OcaBoolean dataAvailable(static_cast< ::OcaBoolean>((FD_ISSET(*(pSocketConnection->GetSocket()), &readSet)) != 0) ? true : false);
        if (pSocketConnection->IsConnected())
        {
            ::OcaBoolean bReceivedKeepAlive;
            switch (pSocketConnection->Run(dataAvailable, static_cast< ::OcaUint32>(OCA_BUFFER_SIZE), m_pDataBuffer, bReceivedKeepAlive))
            {
            case ::Ocp1LiteSocketConnection::NO_MESSAGE_RECEIVED:
                ++cIter;
                break;
            case ::Ocp1LiteSocketConnection::MESSAGE_RECEIVED:
            {
                // One or more messages were received, will be read-out later
                m_messageAvailableConnections.push_back(sessionID);
                ++cIter;
            }
            break;
            case ::Ocp1LiteSocketConnection::CONNECTION_CORRUPT:
            case ::Ocp1LiteSocketConnection::CONNECTION_LOST:
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
                static_cast<void>(deviceList.erase(iterCopy));
            }
            break;
            default:
                OCA_LOG_ERROR_PARAMS("Unknown return from Ocp1LiteSocketConnection::Run for controller %i", sessionID);
                assert(false);
                break;
            }
#ifdef OCA_TRACK_KEEPALIVE_RECEIVED
            if (bReceivedKeepAlive)
            {
                // Add session id to list of received keepalives
                if (m_receivedKeepalives.end() == std::find(m_receivedKeepalives.begin(), m_receivedKeepalives.end(), sessionID))
                {
                    m_receivedKeepalives.push_back(sessionID);
                }
            }
#endif //OCA_TRACK_KEEPALIVE_RECEIVED
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
            static_cast<void>(deviceList.erase(iterCopy));
        }
    }
}
#endif

::OcaLiteStatus Ocp1LiteNetwork::FillTxtRecords(std::vector<std::string>& txtRecords) const
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

::OcaLiteStatus Ocp1LiteNetwork::RegisterRegistrationServices()
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
