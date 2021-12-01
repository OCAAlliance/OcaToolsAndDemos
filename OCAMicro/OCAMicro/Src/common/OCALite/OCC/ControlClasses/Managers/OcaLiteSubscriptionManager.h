/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The OcaLiteSubscriptionManager class.
 *
 */

#ifndef OCALITESUBSCRIPTIONMANAGER_H
#define OCALITESUBSCRIPTIONMANAGER_H

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/Timer/IOcfLiteTimer.h>
#include <OCC/ControlDataTypes/OcaLiteEvent.h>
#include <OCC/ControlDataTypes/OcaLiteList.h>
#include <OCC/ControlDataTypes/OcaLiteMethod.h>
#include <OCC/ControlDataTypes/OcaLitePropertyID.h>
#include <OCF/OcaLiteCommandHandler.h>

// ---- Include local include files ----
#include "OcaLiteManager.h"

// ---- Referenced classes and types ----
class OcaLiteBlob;
class OcaLiteNetworkAddress;

/**
 * Comparator structure to compare events for use in a map
 */
struct event_comparator : public binary_function< ::OcaLiteEvent, ::OcaLiteEvent, bool>
{
    bool operator()(const ::OcaLiteEvent& lhs, const ::OcaLiteEvent& rhs) const;
};

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCA_SUBSCRIPTION_MANAGER_CLASSID      OCA_MANAGER_CLASSID,static_cast< ::OcaUint16>(4)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Mandatory class that manages the event subscriptions of the device.
 * Subscriptions can be added and removed. Event-based subscriptions are
 * handled by registering the correct local event handler of the
 * SubscriptionManager and letting this event handler send a notification
 * message via the CommandHandler.
 */
class OcaLiteSubscriptionManager : public ::OcaLiteManager
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** AddSubscription() */
        ADD_SUBSCRIPTION                        = 1,
        /** RemoveSubscription() */
        REMOVE_SUBSCRIPTION                     = 2,
        /** DisableNotifications() */
        DISABLE_NOTIFICATIONS                   = 3,
        /** ReEnableNotifications() */
        RE_ENABLE_NOTIFICATIONS                 = 4,
        /** AddPropertyChangeSubscription() */
        ADD_PROPERTY_CHANGE_SUBSCRIPTION        = 5,
        /** RemovePropertyChangeSubscription() */
        REMOVE_PROPERTY_CHANGE_SUBSCRIPTION     = 6,
        /** GetMaximumSubscriberContextLength() */
        GET_MAXIMUM_SUBSCRIBER_CONTEXT_LENGTH   = 7
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Events enabled or disabled. */
        OCA_PROP_STATE                  = 1
    };

    /** Event indexes for supported events. */
    enum EventIndex
    {
        /**
         * Event that is raised when the DisableEvents() method is called. This way all subscribers are
         * notified of the fact that the device will temporarily not emit notifications.
         */
        OCA_EVENT_EVENTS_DISABLED       = 1,
        /**
         * Event that is raised when the ReEnableNotifications() method is called. All subscribers are
         * notified of the fact that the device will start emitting event-notifications again, and are
         * given lists of which objects changed state during the period that notifications were disabled.
         */
         OCA_EVENT_SYNCHRONIZE_STATE    = 2
    };

    /**
     * The object number of the network manager
     */
    static const ::OcaONo OBJECT_NUMBER;

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    /**
     * Getter for the OcaLiteSubscriptionManager singleton.
     *
     * @return  Reference to the OcaLiteSubscriptionManager.
     */
    static OcaLiteSubscriptionManager& GetInstance();

    /**
     * Cleans up the singleton instance.
     */
    static void FreeInstance();

    virtual ::OcaBoolean Initialize();

    virtual ::OcaBoolean Shutdown();

    // ---- Interface methods ----
    /**
     * Add a subscription to an event. The local administration is updated and
     * the OnEvent callback method is registered with the actual event of the
     * OCA object.
     *
     * @param[in]  sessionID        The network session
     * @param[in]  ocaEvent         The event
     * @param[in]  subscriber       Method subscriber
     * @param[in]  context          Subscriber context
     * @param[in]  deliveryMode     Notification delivery mode
     * @param[in]  destInfo         Destination information
     * @return Indicates whether the subscription succeeded.
     */
    ::OcaLiteStatus AddSubscription(::OcaSessionID sessionID,
                                    const ::OcaLiteEvent& ocaEvent,
                                    const ::OcaLiteMethod& subscriber,
                                    const ::OcaLiteBlob& context,
                                    ::OcaLiteNotificationDeliveryMode deliveryMode,
                                    const ::OcaLiteNetworkAddress& destInfo);

    /**
     * Removes a subscription to an event.
     *
     * @param[in]  sessionID        The network session
     * @param[in]  ocaEvent         The event
     * @param[in]  subscriber       The subscriber method
     * @return Indicates whether the subscription was successfully removed.
     */
    ::OcaLiteStatus RemoveSubscription(::OcaSessionID sessionID,
                                       const ::OcaLiteEvent& ocaEvent,
                                       const ::OcaLiteMethod& subscriber);

    /**
     * Add a subscription to a property change event. The local administration is updated and
     * the OnEvent callback method is registered with the actual event of the
     * OCA object.
     *
     * @param[in]  sessionID        The network session
     * @param[in]  emitter          The objectnumber of the event emitter
     * @param[in]  property         The property identifier
     * @param[in]  subscriber       Method subscriber
     * @param[in]  context          Subscriber context
     * @param[in]  deliveryMode     Notification delivery mode
     * @param[in]  destInfo         Destination information
     * @return Indicates whether the subscription succeeded.
     */
    ::OcaLiteStatus AddPropertyChangeSubscription(::OcaSessionID sessionID,
                                                  ::OcaONo emitter,
                                                  const ::OcaLitePropertyID& property,
                                                  const ::OcaLiteMethod& subscriber,
                                                  const ::OcaLiteBlob& context,
                                                  ::OcaLiteNotificationDeliveryMode deliveryMode,
                                                  const ::OcaLiteNetworkAddress& destInfo);

    /**
     * Removes a subscription to an event.
     *
     * @param[in]  sessionID        The network session
     * @param[in]  emitter          The objectnumber of the event emitter
     * @param[in]  property         The property identifier
     * @param[in]  subscriber       The subscriber method
     * @return Indicates whether the subscription was successfully removed.
     */
    ::OcaLiteStatus RemovePropertyChangeSubscription(::OcaSessionID sessionID,
                                                     ::OcaONo emitter,
                                                     const ::OcaLitePropertyID& property,
                                                     const ::OcaLiteMethod& subscriber);

    // ---- Miscellaneous methods ----
    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

    virtual ::OcaBoolean AddEventSubscription(const ::OcaLiteEventID& eventID, IEventDelegate& eventDelegate);

    virtual void RemoveEventSubscription(const ::OcaLiteEventID& eventID);

    virtual void SessionLost(::OcaSessionID sessionID);

    /**
     * Sets the maximum number of events that can be subscribed to at once.
     * @note This method is intended to be called only by the OcaBuilder class.
     *
     * @param[in]   nrEvents        The maximum number of events that can be subscribed to at once.
     *                              An event is defined by the combination of the object number and
     *                              the event ID. The maximum number of subscribers to a certain event
     *                              is not influenced by this parameter. Pass a value of 0 for
     *                              dynamic object creation and deletion.
     * @return true if succesfully initialized, false otherwise.
     */
    ::OcaBoolean SetNrEvents(::OcaUint16 nrEvents);

    /**
     * Adds an object to the list of changed objects. This method should only be called by the class' event controllers.
     *
     * @param[in]   oNo         The object number of the object to add.
     */
    void AddChangedObject(::OcaONo oNo);

    /**
     * Class that manages the event subscriptions of the device.
     * Subscriptions can be added and removed. Event-based subscriptions are
     * handled by registering the correct local event handler of the
     * SubscriptionManager and letting this event handler send a notification
     * message via the CommandHandler.
     */
    class OcaEventController : public ::OcaLiteRoot::IEventDelegate
    {
    public:
        /** Constructor
         *
         * @param[in]  subscriptionManager    The subscription reference
         */
        explicit OcaEventController(::OcaLiteSubscriptionManager& subscriptionManager);

        /** Destructor */
        virtual ~OcaEventController();

        // ---- Interface methods ----
        virtual void OnEvent(const ::OcaLiteEventData& eventData);
        virtual void OnEvent(const ::OcaLiteEventData& eventData, const ::OcaLitePropertyID& propertyID);

        /**
         * Add a subscription to an event. The local administration is updated and
         * the OnEvent callback method is registered with the actual event of the
         * OCA object.
         *
         * @param[in]  sessionID        The network session
         * @param[in]  ocaEvent         The event
         * @param[in]  subscriber       Method subscriber
         * @param[in]  context          Subscriber context
         * @param[in]  deliveryMode     Notification delivery mode
         * @param[in]  destInfo         Destination information
         * @return Indicates whether the subscription succeeded.
         */
        ::OcaLiteStatus AddSubscription(::OcaSessionID sessionID,
                                        const ::OcaLiteEvent& ocaEvent,
                                        const ::OcaLiteMethod& subscriber,
                                        const ::OcaLiteBlob& context,
                                        ::OcaLiteNotificationDeliveryMode deliveryMode,
                                        const ::OcaLiteNetworkAddress& destInfo);

        /**
         * Removes a subscription to an event.
         *
         * @param[in]  sessionID        The network session
         * @param[in]  ocaEvent         The event
         * @param[in]  subscriber       The subscriber method
         * @return Indicates whether the subscription was successfully removed.
         */
        ::OcaLiteStatus RemoveSubscription(::OcaSessionID sessionID,
                                           const ::OcaLiteEvent& ocaEvent,
                                           const ::OcaLiteMethod& subscriber);

        /**
         * Add a subscription to a property change event. The local administration is updated and
         * the OnEvent callback method is registered with the actual event of the
         * OCA object.
         *
         * @param[in]  sessionID        The network session
         * @param[in]  emitter          The objectnumber of the event emitter
         * @param[in]  property         The property identifier
         * @param[in]  subscriber       Method subscriber
         * @param[in]  context          Subscriber context
         * @param[in]  deliveryMode     Notification delivery mode
         * @param[in]  destInfo         Destination information
         * @return Indicates whether the subscription succeeded.
         */
        ::OcaLiteStatus AddPropertyChangeSubscription(::OcaSessionID sessionID,
                                                      ::OcaONo emitter,
                                                      const ::OcaLitePropertyID& property,
                                                      const ::OcaLiteMethod& subscriber,
                                                      const ::OcaLiteBlob& context,
                                                      ::OcaLiteNotificationDeliveryMode deliveryMode,
                                                      const ::OcaLiteNetworkAddress& destInfo);

        /**
         * Removes a subscription to an event.
         *
         * @param[in]  sessionID        The network session
         * @param[in]  emitter          The objectnumber of the event emitter
         * @param[in]  property         The property identifier
         * @param[in]  subscriber       The subscriber method
         * @return Indicates whether the subscription was successfully removed.
         */
        ::OcaLiteStatus RemovePropertyChangeSubscription(::OcaSessionID sessionID,
                                                         ::OcaONo emitter,
                                                         const ::OcaLitePropertyID& property,
                                                         const ::OcaLiteMethod& subscriber);


        /**
         * When the session is lost all of the event subscriptions belonging to the
         * session are removed.
         *
         * @param[in]  sessionID        ID of the session which is lost.
         */
        void SessionLost(::OcaSessionID sessionID);

        /**
         * Checks whether or not this event can be deleted.
         *
         * @return true if there are no destinations, false otherwise.
         */
        bool CanBeDeleted() const
        {
            return m_eventDestinations.empty();
        }

    private:
        /**
         * Representation of an OCA subscription. Only for internal use
         * therefore it does not need to implement the IOcaLiteMarshal interface.
         */
        class OcaSubscription
        {
        public:
            /**
             * Constructor
             *
             * @param[in]  ocaEvent         The event
             * @param[in]  subscriber       The subscriber
             * @param[in]  methodID         The method ID
             */
            OcaSubscription(const ::OcaLiteEvent& ocaEvent,
                            ::OcaONo subscriber,
                            const ::OcaLiteMethodID& methodID)
                : m_ocaEvent(ocaEvent),
                  m_subscriber(subscriber),
                  m_methodID(methodID)
            {
            }

            /** Destructor */
            virtual ~OcaSubscription();

            /**
             * Getter for the event
             *
             * @return The event
             */
            const ::OcaLiteEvent& GetEvent() const
            {
                return m_ocaEvent;
            }

            /**
             * Getter for the subscriber
             *
             * @return The subscriber
             */
            ::OcaONo GetSubscriber() const
            {
                return m_subscriber;
            }

            /**
             * Getter for the methodID
             *
             * @return The methodID
             */
            const ::OcaLiteMethodID& GetMethodID() const
            {
                return m_methodID;
            }

        private:
            /** The OCA event */
            ::OcaLiteEvent    m_ocaEvent;

            /** The subscriber */
            ::OcaONo      m_subscriber;

            /** The method ID */
            ::OcaLiteMethodID m_methodID;
        };

        /**
         * Representation of an OCA event destination. Only for internal use
         * therefore it does not need to implement the IOcaLiteMarshal interface.
         */
        class OcaEventDestination
        {
        public:
            /**
             * Constructor
             *
             * @param[in]  subscriber       Method subscriber
             * @param[in]  OcaSessionID     The network session
             * @param[in]  deliveryMode     Notification delivery mode
             * @param[in]  destInfo         Destination information
             */
            OcaEventDestination(const ::OcaLiteMethod& subscriber,
                                ::OcaSessionID OcaSessionID,
                                ::OcaLiteNotificationDeliveryMode deliveryMode,
                                const ::OcaLiteNetworkAddress& destInfo)
                : m_subscriber(subscriber),
                  m_OcaLiteSessionID(OcaSessionID),
                  m_deliveryMode(deliveryMode),
                  m_destInfo(destInfo),
                  m_propertyIDFilter()
            {
            }

                        /**
             * Constructor
             *
             * @param[in]  subscriber       Method subscriber
             * @param[in]  OcaSessionID     The network session
             * @param[in]  deliveryMode     Notification delivery mode
             * @param[in]  destInfo         Destination information
             * @param[in]  propertyIDFilter The property ID filter
             */
            OcaEventDestination(const ::OcaLiteMethod& subscriber,
                                ::OcaSessionID OcaSessionID,
                                ::OcaLiteNotificationDeliveryMode deliveryMode,
                                const ::OcaLiteNetworkAddress& destInfo,
                                const ::OcaLitePropertyID& propertyID)
                : m_subscriber(subscriber),
                  m_OcaLiteSessionID(OcaSessionID),
                  m_deliveryMode(deliveryMode),
                  m_destInfo(destInfo),
                  m_propertyIDFilter(propertyID)
            {
            }

            /** Destructor */
            virtual ~OcaEventDestination() { }

            /**
             * Getter for the subscriber
             *
             * @return The subscriber
             */
            const ::OcaLiteMethod& GetSubscriber() const
            {
                return m_subscriber;
            }

            /**
             * Getter for the sessionID
             *
             * @return The sessionID
             */
            ::OcaSessionID GetSessionID() const
            {
                return m_OcaLiteSessionID;
            }

            /**
             * Getter for the delivery mode
             *
             * @return The delivery mode
             */
            ::OcaLiteNotificationDeliveryMode GetDeliveryMode() const
            {
                return m_deliveryMode;
            }

            /**
             * Getter for the destination information
             *
             * @return The destination information
             */
            const ::OcaLiteNetworkAddress& GetDestinationInfo() const
            {
                return m_destInfo;
            }

            /**
             * Getter for the property ID filter 
             *
             * @return The property ID filter
             */
            const ::OcaLitePropertyID& GetPropertyIDFilter() const
            {
                return m_propertyIDFilter;
            }

        private:
            /** Method of the event handler at the subscriber side. */
            ::OcaLiteMethod                     m_subscriber;
            /** Session ID of the network session on which the OCA notification message
                must be sent.*/
            ::OcaSessionID                      m_OcaLiteSessionID;
            /** Network notification delivery mode */
            ::OcaLiteNotificationDeliveryMode   m_deliveryMode;
            /** Destination information */
            ::OcaLiteNetworkAddress             m_destInfo;
            /** Property ID filter */
            ::OcaLitePropertyID                 m_propertyIDFilter;
        };

        /**
         * Remove event subscription from the m_eventDestinations
         *
         * @return Whether the removal of the event succeeded.
         */
        ::OcaLiteStatus RemoveEventSubscription();

        /**
         * Method to fetch the object pointer from an object number
         *
         * @param[in]  oNo          The object number
         * @return The object pointer if succeeded, otherwise NULL
         */
        ::OcaLiteRoot* GetOcaRootObject(::OcaONo oNo) const;

        /** The event to which this controller is related. */
        ::OcaLiteEvent                          m_event;
        /** List with OcaEventDestinations. */
        typedef std::vector< OcaEventDestination> OcaEventDestinationList;
        OcaEventDestinationList                 m_eventDestinations;
        /** The subscription manager */
        ::OcaLiteSubscriptionManager&           m_subscriptionManager;
    };
protected:
    /**
     * Constructor
     */
    OcaLiteSubscriptionManager();

    /**
     * Destructor.
     */
    virtual ~OcaLiteSubscriptionManager();

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

private:

    /** Singleton instance of the server */
    static ::OcaLiteSubscriptionManager*        m_pSingleton;

    /** The maximum number of events that can be subscribed to at once. */
    ::OcaUint16                                 m_nrEvents;

    /** The map of events to event controllers, which are used to dispatch
        the event from the emitter to the subscriber(s). */
    typedef std::map< ::OcaLiteEvent, OcaEventController*, ::event_comparator> OcaEventHandlerMap;
    OcaEventHandlerMap                          m_eventHandlers;

    /** private copy constructor, no copying of object allowed */
    OcaLiteSubscriptionManager(const ::OcaLiteSubscriptionManager&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteSubscriptionManager& operator=(const ::OcaLiteSubscriptionManager&);
};

#endif // OCALITESUBSCRIPTIONMANAGER_H
