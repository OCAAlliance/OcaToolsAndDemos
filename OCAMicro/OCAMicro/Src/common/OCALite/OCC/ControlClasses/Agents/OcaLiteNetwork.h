/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteNetwork
 *
 */

#ifndef OCALITENETWORK_H
#define OCALITENETWORK_H

// ---- Include system wide include files ----
#include <OCC/ControlClasses/Managers/OcaLiteDeviceManager.h>
#include <OCC/ControlDataTypes/OcaLiteEventSubscriptionDataTypes.h>
#include <OCC/ControlDataTypes/OcaLiteList.h>
#include <OCC/ControlDataTypes/OcaLiteNetworkDataTypes.h>
#include <OCC/ControlDataTypes/OcaLiteBlobDataType.h>
#include <OCC/ControlDataTypes/OcaLiteNetworkSystemInterfaceID.h>
#include <OCC/ControlDataTypes/OcaLiteConnectParameters.h>
#include <OCF/Messages/OcaLiteHeader.h>
#include <OCF/Messages/OcaLiteMessageNotification.h>
#include <HostInterfaceLite/OCA/OCF/Selection/OcfLiteSelectableSet.h>

// ---- Include local include files ----
#include "OcaLiteAgent.h"

// ---- Referenced classes and types ----
class OcaLiteMessageGeneral;
class OcaLiteMessageResponse;
class OcaLiteNetworkMediaPort;
class OcaLiteNetworkStatistics;

// ---- Helper types and constants ----

typedef std::vector< ::OcaSessionID> OcaSessionList;

/**
 * Structure holding a session ID and message
 */
struct OcaLiteMessageSessionID
{
    ::OcaSessionID sessionID;
    ::OcaLiteMessageGeneral* message;
};

/**
 * The classID used for initialization.
 */
#define OCA_NETWORK_CLASSID      OCA_AGENT_CLASSID,static_cast< ::OcaUint16>(1)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Abstract base class for defining network classes to which this device belongs. May be a media transport
 * network, a control and monitoring network, or a network that does both.
 * This class will be subclassed to support networks of various types.
 */
class OcaLiteNetwork : public ::OcaLiteAgent
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetLinkType() */
        GET_LINK_TYPE               = 1,
        /** GetIDAdvertised() */
        GET_ID_ADVERTISED           = 2,
        /** SetIDAdvertised() */
        SET_ID_ADVERTISED           = 3,
        /** GetControlProtocol() */
        GET_CONTROL_PROTOCOL        = 4,
        /** GetMediaProtocol() */
        GET_MEDIA_PROTOCOL          = 5,
        /** GetStatus() */
        GET_STATUS                  = 6,
        /** GetStatistics() */
        GET_STATISTICS              = 7,
        /** ResetStatistics() */
        RESET_STATISTICS            = 8,
        /** GetSystemInterfaces() */
        GET_SYSTEM_INTERFACES       = 9,
        /** SetSystemInterfaces() */
        SET_SYSTEM_INTERFACES       = 10,
        /** GetMediaPorts() */
        GET_MEDIA_PORTS             = 11,
        /** Startup() */
        STARTUP                     = 12,        
        /** Shutdown() */
        SHUTDOWN                    = 13
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Network link type - e.g. wired Ethernet, USB, ... See the OcaNetworkType enum for details. This
            is a read-only property whose value is fixed to the class that is inherited from OcaNetwork to
            implement each specific type of network. */
        OCA_PROP_LINK_TYPE          = 1,
        /** ID by which this device is known on the network, i.e. the host name or GUID that this device
            publishes in the network's directory/discovery system. */
        OCA_PROP_ID_ADVERTISED      = 2,
        /** Type of control protocol used by the network (OCAnn) or NONE if this network is not used for
            control. */
        OCA_PROP_CONTROL_PROTOCOL   = 3,
        /** Type of media transport protocol used by the network, or NONE if this network is not used for
            media transport. */
        OCA_PROP_MEDIA_PROTOCOL     = 4,
        /** Operational status of the network. */
        OCA_PROP_STATUS             = 5,
        /** Collection of identifiers of system interface(s) used by the network. A "system interface" is the
            system service through which network traffic passes into and out of the device -- e.g. a socket.
            The identifier format is system and network dependent; for OCA purposes, it is maintained as a
            variable-length blob which the protocol does not inspect. */
        OCA_PROP_SYSTEM_INTERFACES  = 6,
        /** List of OcaLiteNetworkMediaPort objects connected via this network. */
        OCA_PROP_MEDIA_PORTS        = 7,
        /** Error statistics for this network */
        OCA_PROP_STATISTICS         = 8
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteNetwork();

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods ----
	
	/**
     * Sets the network's IDAdvertised
     *
     * @param[in]   name    The network's IDAdvertised
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus SetIdAdvertised(const ::OcaLiteNetworkNodeID& idAdvertised);
    /**
     * Gets the network's status
     *
     * @param[out]  status  The network's status
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetStatus(::OcaLiteNetworkStatus& status) const = 0;

    /**
     * Gets the list of system interface IDs that this network is using.
     *
     * @param[out]  interfaces  The list of system interface IDs that this network is using
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetSystemInterfaces(::OcaLiteList< ::OcaLiteNetworkSystemInterfaceID>& interfaces) const;

    /**
     * Start up this network
     *
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus Startup() = 0;    

    /**
     * Shut down this network.
     *
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus Shutdown() = 0;

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

    /**
     * Initialize this network.
     *
     * @return True if the initialization succeeded, false otherwise.
     */
    virtual ::OcaBoolean Initialize();

    /**
     * Teardown this network
     */
    virtual void Teardown();

    /**
     * Add all selectables of the network to the respective sets.
     * @note The sets should not be cleared inside this method, to prevent overwrites of selectables added
     *       by other networks.
     *
     * @param[in,out]   readSet         The set of selectables that must be checked for readability.
     * @param[in,out]   writeSet        The set of selectables that must be checked for writeability.
     * @param[in,out]   exceptSet       The set of selectables that must be checked for exceptions.
     */
    virtual void AddSelectables(INT32& highest,
                                OcfLiteSelectableSet& readSet,
                                OcfLiteSelectableSet& writeSet,
                                OcfLiteSelectableSet& exceptSet) = 0;

    /**
     * Handle the objects that were selected by a select operation.
     * @note The selectables in the sets contain all selected objects from all networks. There might be no objects in the
     *       sets that are meant for this specific network.
     *
     * @param[in]   readSet         The set of selectables that can be read without blocking.
     * @param[in]   writeSet        The set of selectables that can be written without blocking.
     * @param[in]   exceptSet       The set of selectables that have exceptions.
     */
    virtual void HandleSelectables(const OcfLiteSelectableSet& readSet,
                                   const OcfLiteSelectableSet& writeSet,
                                   const OcfLiteSelectableSet& exceptSet) = 0;

    /**
     * Send an OcaMessage
     *
     * @param[in]   sessionID   The sessionID that identifies the device to send the message to
     * @param[in]   ocaMessage  The message to be sent
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SendOcaMessage(::OcaSessionID sessionID, const ::OcaLiteMessageGeneral& ocaMessage) = 0;

    /**
     * Indicates if the network has a pending message.
     *
     * @return True if the network has a pending message, false if not.
     */
    virtual ::OcaBoolean HasPendingMessage() const = 0;

    /**
     * Gets the first pending message. If no message was received or is left, the message is set to NULL.
     *
     * @param[out]  messageSession  Information about the session and the received message
     * @param[out]  moreComing      Are there more messages available at this network which needs immediate processing.
     */
    virtual void GetFirstPendingMessage(::OcaLiteMessageSessionID& messageSession, ::OcaBoolean& moreComing) = 0;

    /**
     * Gets list of new connections
     *
     * @param[out]  sessions    List of sessionIDs of new connections
     */
    virtual void GetNewConnections(::OcaSessionList& sessions) = 0;

    /**
     * Gets list of lost connections
     *
     * @param[out]  sessions    List of sessionIDs of lost connections
     */
    virtual void GetConnectionsLost(::OcaSessionList& sessions) = 0;

#ifdef OCA_TRACK_KEEPALIVE_RECEIVED
    /**
     * Get list of connections on which a keep alive is received
     *
     * @param[out] sessions     List of sessionIDs of connection on which a keepalive is received
     */
    virtual void GetReceivedKeepAlives(::OcaSessionList& sessions) = 0;
#endif //OCA_TRACK_KEEPALIVE_RECEIVED

    /**
     * Get the writer for this network.
     *
     * @return The writer for the network.
     */
    virtual const ::IOcaLiteWriter& GetWriter() const = 0;

    /**
     * Get the reader for this network.
     *
     * @return The reader for the network.
     */
    virtual const ::IOcaLiteReader& GetReader() const = 0;

    /**
     * Send notification
     *
     * @param[in]   deliveryMode            Whether the notification message are to be delivered
     *                                      by reliable means or fast means.
     * @param[in]   sessionID               The ID of the active network session on which
     *                                      the notification should be sent.
     * @param[in]   networkAddress          The network destination for the notification.
     * @param[in]   message                 The notification message to send.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SendNotification(::OcaLiteNotificationDeliveryMode deliveryMode,
                                              ::OcaSessionID sessionID,
                                              const ::OcaLiteNetworkAddress& networkAddress,
                                              const ::OcaLiteMessageNotification* message) = 0;

#ifdef OCA_LITE_CONTROLLER
    /** 
     * Send a device reset message to the provided network address.
     *
     * @param[in] resetKey          The reset key.
     * @param[in] networkAddress    The address.
     *
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SendResetMessage(::OcaLiteBlobFixedLen<static_cast< ::OcaUint16>(16)> resetKey,
                                             const ::OcaLiteNetworkAddress networkAddress) = 0;
#endif //OCA_LITE_CONTROLLER
    /**
     * Retrieves a message object
     *
     * @param[in]   msgType     The message type to retrieve.
     * @return A message parameters object.
     */
    virtual ::OcaLiteMessageGeneral* RetrieveMessage(::OcaLiteHeader::OcaLiteMessageType msgType) = 0;

    /**
     * Returns a message object
     *
     * @param[in]   msg         The message to return
     */
    virtual void ReturnMessage(::OcaLiteMessageGeneral* msg) = 0;

    /** 
     * Connect to a remote device.
     *
     * @param[in] connectParameters Information about the connection
     *
     * @return The sessionId, OCA_INVALID_SESSIONID if not session could be established.
     */
    virtual OcaSessionID Connect(const ::OcaLiteConnectParameters& connectParameters)
    {
        return OCA_INVALID_SESSIONID;
    }

    /** 
     * Disconnect the session.
     *
     * @param[in] sessionId    The session ID to disconnect.
     *
     * @return True iff successfull.
     */
    virtual bool Disconnect(::OcaSessionID sessionId)
    {
        return false;
    }

protected:
    /**
     * Constructor
     *
     * @param[in]   objectNumber            Object number of this instance.
     * @param[in]   lockable                Indicates whether or not the object
     *                                      is lockable.
     * @param[in]   role                    The role of this instance.
     * @param[in]   linkType                The network link type
     * @param[in]   idAdvertised            Default name by which this device
     *                                      advertises itself on this network.
     * @param[in]   controlProtocol         The network control protocol
     * @param[in]   systemInterface         Identifier of system interface used by
     *                                      the network. A "system interface" is the system service through
     *                                      which network traffic passes into and out of the device
     *                                      -- e.g. a socket. The identifier format is system and network
     *                                      dependent; for OCA purposes, it is maintained as a
     *                                      variable-length blob which the protocol does not inspect.
     */
    OcaLiteNetwork(::OcaONo objectNumber,
                   ::OcaBoolean lockable,
                   const ::OcaLiteString& role,
                   ::OcaLiteNetworkLinkType linkType,
                   const ::OcaLiteNetworkNodeID& idAdvertised,
                   ::OcaLiteNetworkControlProtocol controlProtocol,
                   const ::OcaLiteNetworkSystemInterfaceID& systemInterface);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    /**
     * Gets the list of system interface IDs that this network is using.
     *
     * @param[in]   interfaces  List of system interfaces that this network will use
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetSystemInterfacesValue(::OcaLiteList< ::OcaLiteNetworkSystemInterfaceID>& interfaces) const;

    /**
     * Gets the list of system interface IDs that this network is using.
     *
     * @param[in]   interfaces  List of system interfaces that this network will use
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetIdAdvertisedValue(::OcaLiteNetworkNodeID& nodeId) const;

	/**
     * Sets the ID advertised in the network.
     *
     * @param[in]   name  The new ID advertised.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetIdAdvertisedValue(const ::OcaLiteNetworkNodeID& name) = 0;

    /**
     * Event that is emitted when the interfaces have changed.
     *
     * @param[in]   interfaces  The value of the interfaces property.
     */
    void SystemInterfaceIDsChanged(const ::OcaLiteList< ::OcaLiteNetworkSystemInterfaceID>& interfaces);

private:
    /** The network's link type. */
    ::OcaLiteNetworkLinkType                        m_networkLinkType;

    /** The network's advertised ID. */
    ::OcaLiteNetworkNodeID                          m_idAdvertised;

    /** The network's control protocol. */
    ::OcaLiteNetworkControlProtocol                 m_networkControlProtocol;

    /** The list of system interface IDs that this network will use. */
    ::OcaLiteNetworkSystemInterfaceID               m_interface;

    /** private copy constructor, no copying of object allowed */
    OcaLiteNetwork(const ::OcaLiteNetwork&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteNetwork& operator=(const ::OcaLiteNetwork&);
};

#endif // OCALITENETWORK_H
