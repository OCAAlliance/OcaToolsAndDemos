/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Ocp.1LiteUdpNetwork
 *
 */
#ifndef OCP1LITEUDPNETWORK_H
#define OCP1LITEUDPNETWORK_H

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCP.1/Ocp1LiteHostInterface.h>
#include <HostInterfaceLite/OCA/OCP.1/ZeroConf/IOcp1LiteService.h>
#include <OCC/ControlClasses/Agents/OcaLiteNetwork.h>
#include <OCC/ControlDataTypes/OcaLiteBlobFixedLen.h>
#include <OCC/ControlDataTypes/OcaLiteStringInABlob.h>

// ---- Include local include files ----
#include "Ocp1LiteNetworkAddress.h"
#include "Ocp1LiteReader.h"
#include "Ocp1LiteUdpSocketConnection.h"
#include "Ocp1LiteWriter.h"
#include "Ocp1LiteNetworkSystemInterfaceID.h"
#include "Messages/Ocp1LiteMessageCommand.h"
#include "Messages/Ocp1LiteMessageHeader.h"
#include "Messages/Ocp1LiteMessageKeepAlive.h"
#include "Messages/Ocp1LiteMessageNotification.h"
#include "Messages/Ocp1LiteMessageResponse.h"
#include <HostInterfaceLite/OCA/OCF/Selection/OcfLiteSelectableSet.h>

// ---- Referenced classes and types ----
class IOcp1LiteSocket;

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCP1_NETWORK_CLASSID    OCA_NETWORK_CLASSID

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Specific network class for the TCP/IP UDP network. This is a control only network.
 * @note This class currently is using only IPv4.
 */
class Ocp1LiteUdpNetwork : public ::OcaLiteNetwork
{
public:
    /** The insecure registration type. */
    static const std::string INSECURE_REGISTRATION_TYPE;

    /** The local registration domain. */
    static const std::string LOCAL_REGISTRATION_DOMAIN;

    /**
     * Constructor
     *
     * @param[in]   objectNumber        Object number of this instance.
     * @param[in]   lockable            Indicates whether or not the object
     *                                  is lockable.
     * @param[in]   role                The role of this instance.
     * @param[in]   idAdvertised        The advertised ID, if empty the already configured name will be used.
     * @param[in]   systemInterface     system interface used by the network. Only the interfaceIndex needs to be passed as the network
     *                                  will retreive all other information from the device during initialization.
     * @param[in]   txtRecords          The application specific TXT records for the service registrations.
     *                                  These will be appended to the default TXT records required for
     *                                  the protocol.
     * @param[in]   domainName          The domain name the OCP.1 network should work at. If an empty string is passed
     *                                  the system interface ID specific domain will be used at registration time.
     * @param[in]   insecureListenPort  The requested port to accept insecure connections on. This port is only used
     *                                  when insecure connections are allowed by the application. When 0 is passed,
     *                                  the port will be determined dynamically.
     */
    Ocp1LiteUdpNetwork(::OcaONo objectNumber,
                    ::OcaBoolean lockable,
                    const ::OcaLiteString& role,
                    const ::Ocp1LiteNetworkNodeID& idAdvertised,
                    const ::Ocp1LiteNetworkSystemInterfaceID& systemInterface,
                    const std::vector<std::string>& txtRecords,
                    const ::OcaLiteString& domainName,
                    ::OcaUint16 insecureListenPort);

    /** Destructor. */
    virtual ~Ocp1LiteUdpNetwork();

    virtual ::OcaBoolean Initialize();

    virtual void Teardown();

    virtual ::OcaLiteStatus GetStatus(::OcaLiteNetworkStatus& status) const;

    virtual ::OcaLiteStatus Startup();    

    virtual ::OcaLiteStatus Shutdown();

    virtual void AddSelectables(INT32& highest,
                                ::OcfLiteSelectableSet& readSet,
                                ::OcfLiteSelectableSet& writeSet,
                                ::OcfLiteSelectableSet& exceptSet);

    virtual void HandleSelectables(const ::OcfLiteSelectableSet& readSet,
                                   const ::OcfLiteSelectableSet& writeSet,
                                   const ::OcfLiteSelectableSet& exceptSet);

    virtual ::OcaLiteStatus SendOcaMessage(::OcaSessionID sessionID, const ::OcaLiteMessageGeneral& ocaMessage);

    virtual ::OcaBoolean HasPendingMessage() const;

    virtual void GetFirstPendingMessage(::OcaLiteMessageSessionID& messageSession, ::OcaBoolean& moreComing);

    virtual void GetNewConnections(::OcaSessionList& sessions);

    virtual void GetConnectionsLost(::OcaSessionList& sessions);

    virtual ::OcaLiteStatus SendNotification(::OcaLiteNotificationDeliveryMode deliveryMode,
                                             ::OcaSessionID sessionID,
                                             const ::OcaLiteNetworkAddress& networkAddress,
                                             const ::OcaLiteMessageNotification* messages);

    virtual const ::IOcaLiteWriter& GetWriter() const
    {
        return m_ocp1Writer;
    }

    virtual const ::IOcaLiteReader& GetReader() const
    {
        return m_ocp1Reader;
    }

    virtual ::OcaLiteMessageGeneral* RetrieveMessage(::OcaLiteHeader::OcaLiteMessageType msgType);

    virtual void ReturnMessage(::OcaLiteMessageGeneral* msg);

protected:

    virtual ::OcaLiteStatus GetSystemInterfacesValue(::OcaLiteList< ::OcaLiteNetworkSystemInterfaceID>& interfaces) const;

    virtual ::OcaLiteStatus GetIdAdvertisedValue(::OcaLiteNetworkNodeID& nodeId) const;

    virtual ::OcaLiteStatus SetStatus(::OcaLiteNetworkStatus status);

    virtual ::OcaLiteStatus CreateMessageBuffer(void);

    /**
     * Register a registration service.
     *
     * @return The result of operation.
     */
    virtual ::OcaLiteStatus RegisterRegistrationServices();

private:

    /** Map with connected controllers. Key is sessionId */
    typedef std::map< ::OcaSessionID, ::Ocp1LiteUdpSocketConnection*> OcaSocketList;

    /**
     * Called to check if connection for the remote device has already been created.
     *
     * @param[in]  ipAddress          ipAddress of the remote device
     * @param[in]  port               port number of the remote device
     *
     * @return True when the session exist, false otherwise
     */
    ::Ocp1LiteUdpSocketConnection* FindConnection(const std::string& ipAddress, const UINT16 port);

    /**
     * Do a run on the controllers in indicated list
     *
     * @param[in] controllerList    List with controllers to handle.
     * @param[in] readSet           The read set with the selected objects.
     */
    void HandleControllers(OcaSocketList& controllerList, const OcfLiteSelectableSet& readSet);

    /**
     * Find socket connection by socket id
     *
     * @param[in] sessionID         Network session ID
     * return Pointer to the connection found by session ID
     */
    ::Ocp1LiteUdpSocketConnection* FindSocketConnection(::OcaSessionID sessionID);

    /**
     * Fills the TXT record data for a service registration.
     *
     * @param[out]  txtRecords      The complete TXT record data.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus FillTxtRecords(std::vector<std::string>& txtRecords) const;

    /** The binary writer to use */
    ::Ocp1LiteWriter                    m_ocp1Writer;

    /** The binary reader to use */
    ::Ocp1LiteReader                    m_ocp1Reader;

    /** Pointer to TCP listen socket on insecure port */
    INT32                               m_pUdpListenSocket;

    /** Buffer to use for storing received messages */
    ::OcaUint8*                         m_pMessageReceiveBuffer;

    /** The map of controller sockets connected via insecure links */
    OcaSocketList                       m_ocaSocketList;

    /** List of new connections */
    ::OcaSessionList                    m_newConnections;

    /** List of connections lost */
    ::OcaSessionList                    m_lostConnections;

    /** List of connections with message available */
    ::OcaSessionList                    m_messageAvailableConnections;

    /** List of notified lost connections */
    ::OcaSessionList                    m_notifiedLostConnections;

    /** Operational status of the network. */
    ::OcaLiteNetworkStatus              m_status;

    /** The requested insecure listening port */
    ::OcaUint16                         m_requestedInsecurePort;

    /** The application specific TXT Record data for the service as a string list. */
    std::vector<std::string>            m_txtRecordList;

    /** Pointer to the UDP socket for sending notifications */
    INT32                               m_pUdpNotificationSendSocket;

    /** General purpose data buffer */
    ::OcaUint8*                         m_pDataBuffer;

    /** Network node id */
    ::OcaLiteString                     m_nodeID;

    /** The domain name to use */
    ::OcaLiteString                     m_domainName;

    /** The system interface ID that this network will use. */
    ::Ocp1LiteNetworkSystemInterfaceID  m_ocp1Interface; 

    /** Messages */
    ::Ocp1LiteMessageCommand*           m_pMsgCmd;
    ::Ocp1LiteMessageKeepAlive*         m_pMsgKeepAlive;
    ::Ocp1LiteMessageNotification*      m_pMsgNtf;
    ::Ocp1MessageResponse*              m_pMsgRsp;
#ifdef _DEBUG
    /** Boolean used in debug to track usage of messages */
    bool                                m_bMsgCmd;
    bool                                m_bMsgKeepAlive;
    bool                                m_bMsgNtf;
    bool                                m_bMsgRsp;
#endif

    /** private copy constructor, no copying of object allowed */
    Ocp1LiteUdpNetwork(const ::Ocp1LiteUdpNetwork&);
    /** private assignment operator, no assignment of object allowed */
    Ocp1LiteUdpNetwork& operator=(const ::Ocp1LiteUdpNetwork&);
};

#endif // Ocp1LiteUdpNetwork_H
