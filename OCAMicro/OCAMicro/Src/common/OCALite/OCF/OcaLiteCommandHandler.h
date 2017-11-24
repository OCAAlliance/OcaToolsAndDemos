/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The OcaLiteCommandHandler class.
 *
 */

#ifndef OCALITECOMMANDHANDLER_H
#define OCALITECOMMANDHANDLER_H

// ---- Include system wide include files ----
#include <OCC/ControlClasses/OcaLiteRoot.h>
#include <OCC/ControlClasses/Agents/OcaLiteNetwork.h>
#include <OCC/ControlDataTypes/OcaLiteEventSubscriptionDataTypes.h>
#include <OCC/ControlDataTypes/OcaLiteList.h>
#include <OCC/ControlDataTypes/OcaLiteFrameworkDataTypes.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLiteNetworkAddress.h>
#include <OCC/ControlDataTypes/OcaLiteNetworkDataTypes.h>

// ---- Include local include files ----
#include "Messages/OcaLiteMessageResponse.h"

// ---- Referenced classes and types ----
class OcaLiteEventData;
class OcaLiteMessageGeneral;
class OcaLiteMethodID;

// ---- Helper types and constants ----

/**
 * Representation of an OCA notification. Only for internal use
 * therefore it does not need to implement the IOcaLiteMarshal interface.
 */
class OcaLiteNotificationData
{
public:
    /** Default constructor */
    OcaLiteNotificationData()
        : m_targetONo(OCA_INVALID_ONO),
          m_methodID(),
          m_context(),
          m_eventData(NULL)
    {
    }

    /**
     * Constructor
     *
     * @param[in]  targetONo       Target object number
     * @param[in]  methodID        Method id
     * @param[in]  context         Subscriber context
     * @param[in]  eventData       Event data
     */
    OcaLiteNotificationData(::OcaONo targetONo,
                            const ::OcaLiteMethodID& methodID,
                            const ::OcaLiteBlob& context,
                            const ::OcaLiteEventData* eventData)
        : m_targetONo(targetONo),
          m_methodID(methodID),
          m_context(context),
          m_eventData(eventData)
    {
    }

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteNotificationData(const ::OcaLiteNotificationData& source)
        : m_targetONo(source.m_targetONo),
        m_methodID(source.m_methodID),
        m_context(source.m_context),
        m_eventData(source.m_eventData) //lint !e1554 Direct pointer copy
    {
    }

    /** Destructor */
    virtual ~OcaLiteNotificationData()
    {
        m_eventData = NULL;
    }

    ::OcaONo GetTargetONo() const
    {
        return m_targetONo;
    }

    const ::OcaLiteMethodID& GetMethodID() const
    {
        return m_methodID;
    }

    /**
     * Getter for the context
     *
     * @return The context
     */
    const ::OcaLiteBlob& GetContext() const
    {
        return m_context;
    }

    /**
     * Getter for the event data
     *
     * @return The event data
     */
    const ::OcaLiteEventData* GetEventData() const
    {
        return m_eventData;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteNotificationData& operator=(const ::OcaLiteNotificationData& source);

private:
    ::OcaONo m_targetONo;
    ::OcaLiteMethodID m_methodID;
    ::OcaLiteBlob m_context;
    const ::OcaLiteEventData* m_eventData;
};



// ---- Helper functions ----

// ---- Class Definition ----
/**
 * OcaLiteCommandHandler is responsible for all device related CommandHandler stuff.
 * This includes:
 * - receiving and invoking commands/events and sending back the response if
 *   needed (controller and device side)
 * - Monitoring network links for connection loss
 * The command handler uses the object list manager to get access to objects
 * it needs to invoke a command on. The command handler keeps track of all
 * network sessions that are active. If a network connection is lost it automatically
 * informs all OCA objects of this connection loss.
 * @ingroup OCF
 */
class  OcaLiteCommandHandler
{
public:

    /**
     * Class definition used for ConnectionEstablished events
     */
    class IConnectionEstablishedDelegate
    {
    public:
        /**
         * Destructor
         */
        virtual ~IConnectionEstablishedDelegate() {}

        /**
         * Called when connection is established
         *
         * @param[in] sessionID     The ID of the session which is connected.
         */
        virtual void OnConnectionEstablished(::OcaSessionID sessionID) = 0;
    };

    /**
     * Class definition used for ConnectionLost events
     */
    class IConnectionLostDelegate
    {
    public:
        /**
         * Destructor
         */
        virtual ~IConnectionLostDelegate() {}

        /**
         * Called when connection is lost
         *
         * @param[in] sessionID     The ID of the session which is lost.
         */
        virtual void OnConnectionLost(::OcaSessionID sessionID) = 0;
    };

   /**
     * Getter for the OcaLiteCommandHandler singleton.
     *
     * @return  Reference to the OcaLiteCommandHandler.
     */
    static ::OcaLiteCommandHandler& GetInstance();

    /**
     * Cleans up the singleton instance.
     */
    static void FreeInstance();

    /**
     * Initialize the CommandHandler.
     *
     * @return true if succesfully initialized, false otherwise.
     */
    virtual ::OcaBoolean Initialize();

    /**
     * Shutdown the CommandHandler.
     */
    virtual void Shutdown();

    // ---- Interface methods ----
    /**
     * Send a reliable OCA notification message. The data is marshaled and a message
     * is sent. Used on the device side (can be used by a controller if someone
     * subscribes to controller events, but the controller then acts as system
     * device to another controller).
     *
     * @param[in] deliveryMode   Whether the notification message are to be delivered
     *                           by reliable means or fast means.
     * @param[in] sessionID      The session ID of the network session on which the
     *                           notification message must be sent.
     * @param[in] notification   notification that must be sent
     * @param[in] networkAddress Network destination for the notification
     * @return Indicates whether sending the notification message succeeded.
     */
    ::OcaBoolean SendNotification(::OcaLiteNotificationDeliveryMode deliveryMode,
                                  ::OcaSessionID sessionID,
                                  const ::OcaLiteNotificationData& notification,
                                  const ::OcaLiteNetworkAddress& networkAddress) const;

    /**
     * The actual application that implements OCA is responsible for calling this
     * function on a regular basis.
     *
     * @param[in]   timeout     Maximum time in ms to wait for a message to be received
     *                          on any network. It is the resposibility of the calling
     *                          application to use a value that is smaller than the
     *                          KeepAlive timeout.
     */
    virtual void RunWithTimeout(::OcaUint32 timeout);

    /**
     * Get all selectables which are being used by the OCA later to perform a select.
     * A call to this function should be followed by a select. The result should be provided to 
     * RunWithSelectSet. 
     *
     * @param[in/out]   highest		The highest file descriptor.
     * @param[in/out]   readSet		The read set.
     * @param[in/out]   writeSet	The write set.
     * @param[in/out]   exceptSet	The except set.
     *
     * @return True if there are already messages pending in the socket layer. False if not.
     */
    virtual bool AddSelectables(INT32& highest, OcfLiteSelectableSet& readSet, OcfLiteSelectableSet& writeSet, OcfLiteSelectableSet& exceptSet);

   /**
     * The actual application that implements OCA is responsible for calling this
     * function on a regular basis. The parameters which must be provided should be the result of a select.
     * The select sets can be filled with a call to AddSelectables.
     *
     * @param[in]   readSet		The read set.
     * @param[in]   writeSet	The write set.
     * @param[in]   exceptSet	The except set.
     */
    virtual void RunWithSelectSet(OcfLiteSelectableSet readSet, OcfLiteSelectableSet writeSet, OcfLiteSelectableSet exceptSet);

    /**
     * Create and return a new session id
     *
     * @return New session id
     */
    ::OcaSessionID CreateSessionID();

    /**
     * Registers a connectionestablished delegate which will be used when the connection
     * to a device is established.
     *
     * @param[in]   connEstablishedDelegate     The delegate that will be called
     * @return  false when already registered
     */
    bool RegisterConnectionEstablishedEventHandler(IConnectionEstablishedDelegate* connEstablishedDelegate);

    /**
     * Remove connectionestablished delegate
     *
     * @param[in]   connEstablishedDelegate     The delegate that will be removed
     * @return  false when given delegate was not registered, true otherwise
     */
    bool UnregisterConnectionEstablishedEventHandler(const IConnectionEstablishedDelegate* connEstablishedDelegate);

    /**
     * Registers a connectionlost delegate which will be used when the connection
     * to a device is lost.
     *
     * @param[in]   connLostDelegate            The delegate that will be called
     * @return  false when already registered
     */
    bool RegisterConnectionLostEventHandler(IConnectionLostDelegate* connLostDelegate);

    /**
     * Remove connectionlost delegate
     *
     * @param[in]   connLostDelegate            The delegate that will be removed
     * @return  false when given delegate was not registered, true otherwise
     */
    bool UnregisterConnectionLostEventHandler(const IConnectionLostDelegate* connLostDelegate);

    /**
     * Get a buffer to use with response data.
     * This buffer needs to be filled with the parameter data of the response
     * marshaled into a byte array. This parameter data represents the output
     * parameters of the method the response belongs to.
     *
     * @param[in] bufferSize    Size of the buffer requested.
     * @return pointer to the reserved buffer. null if no buffer is available.
     */
    ::OcaUint8* GetResponseBuffer(::OcaUint32 bufferSize);

protected:
    /**
     * Constructor
     */
    OcaLiteCommandHandler();

    /**
     * Destructor.
     */
    virtual ~OcaLiteCommandHandler();

    /**
     * Handle connections and messaging of all networks
     *
     *  @param[in] timeout   Timeout
     */
    virtual void HandleNetworks(::OcaUint32 timeout);

    /**
     * Retrieves a pointer to the device object with the given object number.
     *
     * @param[in]  ocaONo       The object number of the object to retrieve.
     * @return The pointer to the object or NULL of the object is not found.
     */
    static ::OcaLiteRoot* GetDeviceObject(::OcaONo ocaONo);

    /** Singleton instance of the server */
    static ::OcaLiteCommandHandler* m_pSingleton;

    /** List for network sessions */
    typedef std::list< ::OcaSessionID> OcaNetworkSessionList;

    /** The list of network sessions */
    OcaNetworkSessionList        m_ocaNetworkSessions;

private:
    /**
     * Connection established event that is raised when a connection is
     * established. The sessionID of the device to which the connection
     * has been made is passed as event argument.
     *
     * @param[in] sessionID   The sessionID of the device which is connected.
     */
    void ConnectionEstablished(::OcaSessionID sessionID);

    /**
     * Connection lost event that is raised when a connection is lost.
     * The sessionID of the device to which the connection was made is
     * passed as event argument.
     *
     * @param[in] sessionID   The sessionID of the device which is lost.
     */
    void ConnectionLost(::OcaSessionID sessionID);

    /**
     * Handle messages
     */
    void HandleMessages();

    /** The response buffer. */
    UINT8* m_pResponseBuffer;

    /** The current response size */
    UINT16 m_responseBufferSize;

    /** Last sessionId */
    ::OcaSessionID                  m_sessionId;

    /** Listener to ConnectionEstablished events */
    IConnectionEstablishedDelegate* m_pConnectionEstablishedDelegate;

    /** Listener to ConnectionLost events */
    IConnectionLostDelegate*        m_pConnectionLostDelegate;

    /** Session list */
    ::OcaSessionList                m_sessionList;

    /** private copy constructor, no copying of object allowed */
    OcaLiteCommandHandler(const ::OcaLiteCommandHandler&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteCommandHandler& operator=(const ::OcaLiteCommandHandler&);
};

#endif // OCALITECOMMANDHANDLER_H
