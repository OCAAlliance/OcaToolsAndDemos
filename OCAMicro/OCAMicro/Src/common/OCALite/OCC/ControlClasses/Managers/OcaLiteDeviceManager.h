/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteDeviceManager
 *
 */
#ifndef OCALITEDEVICEMANAGER_H
#define OCALITEDEVICEMANAGER_H

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/Configuration/IOcfLiteConfigure.h>
#include <OCC/ControlDataTypes/OcaLiteBlobFixedLen.h>
#include <OCC/ControlDataTypes/OcaLiteList.h>
#include <OCC/ControlDataTypes/OcaLiteNetworkAddress.h>

// ---- Include local include files ----
#include "OcaLiteManager.h"

// ---- Referenced classes and types ----
class OcaLiteModelDescription;
class OcaLiteModelGUID;
class OcaLiteManagerDescriptor;

// ---- Helper types and constants ----

/**
 * Enumeration defining bit flags that indicate the device states OCA devices can be in. The state is returned
 * by the device's Device Manager on request. Any combination of the flags may be returned, unless
 * specified otherwise for the specific flag. A value '0' indicates the device is fully operational.
 * @ingroup ManagementDataTypes
 */
//lint -strong(AJX, OcaLiteDeviceState)
enum OcaLiteDeviceState    /* maps onto OcaUint16 */
{
    /** If set the device is operational. This state 'logically follows'
        from the fact that the 'initializing' and 'Updating' flags are not set
        (since this is not a bit flag itself).*/
    OCADEVICESTATE_OPERATIONAL  = 0x0001,
    /** If set the device is disabled.  */
    OCADEVICESTATE_DISABLED     = 0x0002,
    /** If set the device is in error. */
    OCADEVICESTATE_ERROR        = 0x0004,
    /** The device is initializing. This bitflag cannot
        be combined with the 'Updating' flag. */
    OCADEVICESTATE_INITIALIZING = 0x0008,
    /** Device is upgrading firmware. This bitflag cannot
        be combined with the 'Initializing' flag */
    OCADEVICESTATE_UPDATING     = 0x0010
};

/**
 * The classID used for initialization.
 * @ingroup Managers
 */
#define OCA_DEVICEMANAGER_CLASSID      OCA_MANAGER_CLASSID,static_cast< ::OcaUint16>(1)
/** The length of the reset key. */
#define RESET_KEY_LENGTH static_cast< ::OcaUint16>(16)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Manadatory class that manages information relevant to the whole
 * device and its network connection(s).
 * @ingroup Managers
 */
class OcaLiteDeviceManager : public ::OcaLiteManager
{
public:
    /** Method indexes for the supported methods. */
    //lint -e(578) Hides inherited symbol
    enum MethodIndex
    {
        /** GetOcaVersion() */
        GET_OCA_VERSION         = 1,
        /** GetModelGUID() */
        GET_MODEL_GUID          = 2,
        /** GetSerialNumber() */
        GET_SERIAL_NUMBER       = 3,
        /** GetDeviceName() */
        GET_DEVICE_NAME         = 4,
        /** SetDeviceName() */
        SET_DEVICE_NAME         = 5,
        /** GetModelDescription() */
        GET_MODEL_DESCRIPTION   = 6,
        /** GetDeviceRole() */
        GET_DEVICE_ROLE         = 7,
        /** SetDeviceRole() */
        SET_DEVICE_ROLE         = 8,
        /** GetUserInventoryCode() */
        GET_USER_INVENTORY_CODE = 9,
        /** SetUserInventoryCode() */
        SET_USER_INVENTORY_CODE = 10,
        /** GetEnabled() */
        GET_ENABLED             = 11,
        /** SetEnabled() */
        SET_ENABLED             = 12,
        /** GetState() */
        GET_STATE               = 13,
        /** SetResetKey() */
        SET_RESET_KEY           = 14,
        /** GetResetCause() */
        GET_RESET_CAUSE         = 15,
        /** ClearResetCase() */
        CLEAR_RESET_CAUSE       = 16,
        /** GetMessage() */
        GET_MESSAGE             = 17,
        /** Setmessage() */
        SET_MESSAGE             = 18,
        /** GetManagers() */
        GET_MANAGERS            = 19
    };

    /** Property indexes for the supported properties. */
    //lint -e(578) Hides inherited symbol
    enum PropertyIndex
    {
        /** Read-only property that identifies the model of the device. */
        OCA_PROP_MODEL_GUID             = 1,
        /** Read-only property that indentifies the serial number of the Device. */
        OCA_PROP_SERIAL_NUMBER          = 2,
        /** Read-only property that contains text names for this model,
            its manufacturer, and its version. */
        OCA_PROP_MODEL_DESCRIPTION      = 3,
        /** Name of the device. */
        OCA_PROP_DEVICE_NAME            = 4,
        /** Read-only property that indicates the OCA version number. */
        OCA_PROP_OCA_VERSION            = 5,
        /** Role of the device in application (arbitrary). */
        OCA_PROP_ROLE                   = 6,
        /** Code used for equipment tracking. */
        OCA_PROP_USER_INVENTORY_CODE    = 7,
        /** Indicates whether the device is enabled (and therefore operational). */
        OCA_PROP_ENABLED                = 8,
        /** Read-only property that indicates the current state of the device */
        OCA_PROP_STATE                  = 9,
        /** True if device is working on something and is not available for
            OCA command activity. Read-only. */
        OCA_PROP_BUSY                   = 10,
        /** Read-only attribute that indicates the reset cause of the last reset. */
        OCA_PROP_RESET_CAUSE            = 11,
        /** Arbritary text message provided by controller. Display and handling
            of the text is devicedependent */
        OCA_PROP_MESSAGE                = 12,
        /** List of all manager object instantiated in this device */
        OCA_PROP_MANAGERS               = 13
    };

    /** Device operational state. */
    //lint -strong(AJX, OperationalState)
    enum OperationalState
    {
        /** The device is functioning normally. */
        OCA_OPSTATE_OPERATIONAL             = 0,
        /** The device is initializing. */
        OCA_OPSTATE_INITIALIZING            = 1,
        /** Device is upgrading firmware. */
        OCA_OPSTATE_UPDATING                = 2,
        /** Device is shutting down. */
        OCA_OPSTATE_SHUTTING_DOWN           = 3
    };

    /**
     * The object number of the device manager
     */
    static const ::OcaONo OBJECT_NUMBER;

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    //lint -e(1516) Hides inherited member
    static const ::OcaLiteClassID CLASS_ID;

    /**
     * Getter for the OcaLiteDeviceManager singleton.
     *
     * @return Reference to the OcaLiteDeviceManager.
     */
    static ::OcaLiteDeviceManager& GetInstance();

    /**
     * Cleans up the singleton instance.
     */
    static void FreeInstance();

    // ---- Interface methods ----
    /**
     * Gets the value of the OcaLiteVersion property.
     *
     * @param[out] ocaVersion   Output parameter that holds the value of
     *                          OcaLiteVersion property if the method succeeds.
     * @return Indicates whether the property was successfully retrieved.
     */
    ::OcaLiteStatus GetOcaVersion(::OcaUint16& ocaVersion) const;

    /**
     * Gets the value of the OcaLiteModelGUID property.
     *
     * @param[out] modelGUID    Output parameter that holds the value of the
     *                          OcaLiteModelGUID property if the method succeeds.
     * @return Indicates whether the property was successfully retrieved.
     */
    ::OcaLiteStatus GetModelGUID(::OcaLiteModelGUID& modelGUID) const;

    /**
     * Gets the value of the serial property.
     *
     * @param[out] serialNumber     Output parameter that holds the value of the
     *                              OcaSerialNumber property if the method succeeds
     * @return Indicates whether the property was successfully retrieved.
     */
    ::OcaLiteStatus GetSerialNumber(::OcaLiteString& serialNumber) const;

    /**
     * Gets the value of the deviceName property.
     *
     * @param[out] deviceName   Output parameter that holds the value of the
     *                          deviceName property.
     * @return Indicates whether the property was successfully retrieved.
     */
    ::OcaLiteStatus GetDeviceName(::OcaLiteString& deviceName) const;

    /**
     * Gets the value of the modelDescription property.
     *
     * @param[out] modelDescription     Output parameter that holds the value of
     *                                  the OcaLiteModelDescription property.
     * @return Indicates whether the property was successfully retrieved.
     */
    ::OcaLiteStatus GetModelDescription(::OcaLiteModelDescription& modelDescription) const;

    /**
     * Gets the value of the enabled property.
     *
     * @param[out] enabled  Output parameter that holds the value of
     *                      the enabled property.
     * @return Indicates whether the property was successfully retrieved.
     */
    ::OcaLiteStatus GetEnabled(::OcaBoolean& enabled) const;

    /**
     * Sets the value of the enabled property.
     *
     * @param[in] enabled   The value of the enabled property.
     * @return Indicates whether the property was successfully set.
     */
    ::OcaLiteStatus SetEnabled(::OcaBoolean enabled);

    /**
     * Gets the value of the state property.
     *
     * @param[out] state    Output parameter that holds the value of
     *                      the state property.
     * @return Indicates whether the property was successfully retrieved.
     */
    ::OcaLiteStatus GetState(::OcaLiteDeviceState& state) const;

    /**
     * Retrieve the list of descriptors of managers instantiated in this device.
     *
     * @param[out] managers Output parameter that holds value
     *                      of the managers property.
     * @return Indicates whether the property was successfully retrieved.
     */
    ::OcaLiteStatus GetManagers(::OcaLiteList< ::OcaLiteManagerDescriptor>& managers) const;

    // ---- Miscellaneous methods ----
    /**
     * Adds an manager to the manager object list.
     * Only for internal use.
     *
     * @param[in] rObject   Reference to the instance of the object.
     * @return Indicates whether the manager object was added successfully; if adding
     *         the manager object failed the object number was already in use.
     */
    ::OcaBoolean AddManager(::OcaLiteManager& rObject);

    /**
     * Deletes the manager object with the given object number.
     * The object number is released and can be re-used later on.
     * Only for internal use.
     *
     * @param[in] rObject   Reference to the instance of the object.
     */
    void RemoveManager(const ::OcaLiteManager& rObject);

    /**
     * Gets the object pointer of the object that has the specified object number.
     * Only for internal use.
     *
     * @param[in] oNo   The object number of the object.
     * @return Pointer to the object. If no object with that number exists
     *         a null pointer is returned.
     */
    ::OcaLiteManager* GetManager(::OcaONo oNo);

    /**
     * Gets the value of the error state.
     *
     * @return The value of the error state.
     */
    ::OcaBoolean GetErrorState() const;

    /**
     * Sets the error state.
     *
     * @param[in] error     True if the device is in error, false is otherwise.
     */
    void SetErrorState(::OcaBoolean error);

    /**
     * Gets the value of the operational state.
     *
     * @return The value of the operational state.
     */
    OperationalState GetOperationalState() const;

    /**
     * Sets the operational state.
     *
     * @param[in]  state   The operational state.
     */
    void SetOperationalState(OperationalState state);

    /**
     * Sets the error and operation state.
     *
     * @param[in] error     True if the device is in error, false is otherwise.
     * @param[in]  state   The operational state.
     */
    void SetErrorAndOperationalState(::OcaBoolean error, OperationalState state);

    /**
     * Indicates if the device is busy.
     *
     * @return True if the device is busy, false otherwise.
     */
    ::OcaBoolean GetBusy() const;

    /**
     * Sets the busy flag of the device.
     *
     * @param[in]   busy    True if the device is busy, false otherwise.
     */
    void SetBusy(::OcaBoolean busy);

    virtual ::OcaLiteStatus Lock(::OcaSessionID sessionID);

    virtual void SessionLost(::OcaSessionID sessionID);

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

protected:
    /** Constructor */
    OcaLiteDeviceManager();

    /** Destructor. */
    virtual ~OcaLiteDeviceManager();

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

private:

    /** Singleton instance of the server */
    static ::OcaLiteDeviceManager*          m_pSingleton;

    /** The map of manager object numbers to object pointers */
    typedef map< ::OcaONo, ::OcaLiteManager*> OcaManagerList;
    OcaManagerList                          m_managerObjects;

    /** Boolean which indicates the error state. */
    ::OcaBoolean                            m_bErrorState;

    /** Operational state of the device. */
    OperationalState                        m_operationalState;

    /** private copy constructor, no copying of object allowed */
    OcaLiteDeviceManager(const ::OcaLiteDeviceManager&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteDeviceManager& operator=(const ::OcaLiteDeviceManager&);
};

// ---- Specialized Template Function Definition ----

//lint -save -e1576 Explicit specialization does not occur in the same file as corresponding function template

template <>
void MarshalValue< ::OcaLiteDeviceState>(const ::OcaLiteDeviceState& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::OcaLiteDeviceState>(::OcaLiteDeviceState& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteDeviceState>(const ::OcaLiteDeviceState& value, const ::IOcaLiteWriter& writer);

//lint -restore

#endif // OCALITEDEVICEMANAGER_H
