/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteRoot
 *
 */
#ifndef OCALITEROOT_H
#define OCALITEROOT_H

// ---- Include system wide include files --
#include <OCC/ControlDataTypes/OcaLiteBaseDataTypes.h>
#include <OCC/ControlDataTypes/OcaLiteClassID.h>
#include <OCC/ControlDataTypes/OcaLiteEventID.h>
#include <OCC/ControlDataTypes/OcaLiteFrameworkDataTypes.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----
class IOcaLiteReader;
class IOcaLiteWriter;
class OcaLiteClassIdentification;
class OcaLiteEventData;
class OcaLiteMessageResponse;
class OcaLiteMethodID;

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCA_ROOT_CLASSID            static_cast< ::OcaUint16>(1)

/**
 * The minimum object number for device-specific objects.
 */
#define OCA_MINIMUM_DEVICE_OBJECT_ONO   static_cast< ::OcaONo>(4096)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * The abstract root class of which all OCA classes derive. It offers basic OCA functionality such as locking
 * an object and generalized data access.
 */
class OcaLiteRoot
{
public:
    class IEventDelegate
    {
    public:
        /**
         * Destructor.
         */
        virtual ~IEventDelegate() {}

        // ---- Interface methods ----
        /**
         * Internal event handler. This method notifies a generated message
         * to the delegate.
         *
         * @param[in] eventData         The event data that belongs to the event.
         */
        virtual void OnEvent(const ::OcaLiteEventData& eventData) = 0;
    };

    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetClassIdentification() */
        GET_CLASS_IDENTIFICATION    = 1,
        /** GetLockable() */
        GET_LOCKABLE                = 2,
        /** Lock() */
        LOCK                        = 3,
        /** Unlock() */
        UNLOCK                      = 4,
        /** GetRole() */
        GET_ROLE                    = 5,
        /** LockReadOnly() */
        LOCK_READONLY               = 6
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Number that uniquely identifies the class. Note that this differs from the object number, which
            identifies the instantiated object. This is a class property instead of an object property */
        OCA_PROP_CLASS_ID       = 1,
        /** Identifies the interface version of the class. Any change to the class definition leads to a higher
            class version. */
        OCA_PROP_CLASS_VERSION  = 2,
        /** The object number that uniquely identifies the instantiated object. */
        OCA_PROP_OBJECT_NUMBER  = 3,
        /** Read-only property that indicates whether the object is lockable or non-lockable. */
        OCA_PROP_LOCKABLE       = 4,
        /** Read-only text property that describes object's role in the device. */
        OCA_PROP_ROLE           = 5
    };

    /** Event indexes for supported events. */
    enum EventIndex
    {
        /**
         * General event that is emitted when a property changes. In each setter method (of derived
         * classes) this event must be raised with the proper derived event data structure.
         */
        OCA_EVENT_PROPERTY_CHANGED  = 1
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteRoot();

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods ----
    /**
     * Gets the class identification, a structure that contains the ClassID and ClassVersion.
     *
     * @param[out]  classIdentification     The classidentification
     * @return Indicates whether the property was successfully retrieved.
     */
    ::OcaLiteStatus GetClassIdentification(::OcaLiteClassIdentification& classIdentification) const;

    /**
     * Gets the value of the Lockable property.
     *
     * @param[out]  lockable            Lockable property.
     * @return Indicates whether the property was successfully retrieved.
     */
    ::OcaLiteStatus GetLockable(::OcaBoolean& lockable) const;

    /**
     * Locks the object so that it can only be accessed by the lock holder.
     *
     * @param[in]   sessionID           The identifier of the network session.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus Lock(::OcaSessionID sessionID);

    /**
     * Unlocks the object so that it can only be freely accessed again.
     * This method can only succeed if it is called by the lockholder.
     *
     * @param[in]   sessionID           The identifier of the network session.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus Unlock(::OcaSessionID sessionID);

    /**
     * Gets the value of the Role property.
     *
     * @param[out]  role            Output parameter that holds the value of
     *                              Role property if the method succeeds.
     * @return Indicates whether the property was successfully retrieved.
     */
    ::OcaLiteStatus GetRole(::OcaLiteString& role) const;

    // ---- Miscellaneous methods ----
    /**
     * Execute the method of the passed method ID. Each derived class must
     * override this method and add the execution of its own methods. This
     * method is for internal use (i.e. inside the device) only.
     *
     * @param[in]   reader              The reader to use
     * @param[in]   writer              The writer to use
     * @param[in]   sessionID           The identifier of the network session.
     * @param[in]   methodID            The method ID of the method that must
     *                                  be executed.
     * @param[in]   parmsIn             Parameters of the methods
     * @param[out]  msgResponse         Response message to be sent. May be NULL if no
     *                                  parameters can be sent (e.g. for a notification).
     *
     * @return Return value of the executed function or relevant error.
     */
    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

    /**
     * Gets the value of the ObjectNumber property.
     *
     * @return ObjectNumber property.
     */
    ::OcaONo GetObjectNumber() const
    {
        return m_objectNumber;
    }

    /**
     * Method that the command handler uses to indicate a network session was lost.
     * It is checked if the object was locked on this session and if so, the
     * object is unlocked. If derived classes need to perform extra processing
     * when losing a session they must override this method (e.g. subscription
     * manager to clear the subscriptions that were made on the session); they
     * must always explicitly call the implementation of their base class though.
     * @note Worker and Agent objects may not perform an action on their containing block
     * (owner) inside the SessionLost implementation, doing so could lead to a recursive
     * lock!
     *
     * @param[in]   sessionID           The identifier of the network session that was lost.
     */
    virtual void SessionLost(::OcaSessionID sessionID);

    /**
     * Check if this object is locked for others than the owner of the object.
     *
     * @param[in]   sessionID           The identifier of the network session.
     * @return true if the object is locked, false otherwise.
     */
    ::OcaBoolean IsLocked(::OcaSessionID sessionID) const;

    /**
     * Indicates if this object has a lock or not.
     *
     * @return true if the object is locked, false otherwise.
     */
    ::OcaBoolean HasLock() const;

    /**
     * Add an eventhandler to the indicated event.
     * This method is only to be called by the SubscriptionManager!
     *
     * @param[in]   eventID         Event to register on.
     * @param[in]   eventDelegate   Class with eventhandler to call.
     * @return true if succesfully added, false otherwise.
     */
    virtual ::OcaBoolean AddEventSubscription(const ::OcaLiteEventID& eventID, IEventDelegate& eventDelegate);

    /**
     * Remove an eventhandler from the indicated event.
     * This method is only to be called by the SubscriptionManager!
     *
     * @param[in]   eventID         Event to register on.
     */
    virtual void RemoveEventSubscription(const ::OcaLiteEventID& eventID);

protected:
    /**
     * Constructor
     *
     * @param[in]   objectNumber        Object number of this instance.
     * @param[in]   lockable            Indicates whether or not the object
     *                                  is lockable.
     * @param[in]   role                The role of this instance.
     */
    OcaLiteRoot(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role);

    /**
     * General event that is emitted when a property changes. This event
     * does not require explicit acknowledgement from the event handler(s).
     * If a derived class needs an acknowledged event for a specific
     * property change it must define its own event.
     *
     * @param[in]   eventData           The event data of the event.
     */
    void PropertyChanged(const ::OcaLiteEventData& eventData);

    /**
     * Retrieves the class ID of the class.
     *
     * @return The class ID of the class.
     */
    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    /**
     * Retrieves the version of the class.
     *
     * @return The version of the class.
     */
    virtual ::OcaClassVersionNumber GetClassVersion() const;

private:
    /**
     * The object number that uniquely identifies the instantiated object. This read-only property must be
     * set at creation of the object. Derived objects can hardcode the object number in its constructor, or
     * offer a constructor with object number parameter for dynamic allocation of object numbers.
     */
    ::OcaONo                                m_objectNumber;

    /**
     * Read-only property that indicates whether the object is lockable or non-lockable. The property
     * value must be set during construction of the object.
     */
    ::OcaBoolean                            m_lockable;

    /**
     * Read-only text property that describes object's role in the device. Particularly useful for workers,
     * e.g. "Input 1 Gain".
     */
    ::OcaLiteString                         m_role;

    /** The session ID of the lockholder. */
    ::OcaSessionID                          m_lockHolder;

    /** Pointer to class with eventhandler to call when the
        PropertyChangedEvent is generated. */
    IEventDelegate*                         m_pPropertyChangedEventDelegate;

    /** private copy constructor, no copying of object allowed */
    OcaLiteRoot(const ::OcaLiteRoot&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteRoot& operator=(const ::OcaLiteRoot&);
};

#endif // OCALITEROOT_H
