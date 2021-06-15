/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteDeviceTimeManager
 *
 */
#ifndef OcaLiteDeviceTimeManager_H
#define OcaLiteDeviceTimeManager_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteList.h>
#include <OCC/ControlDataTypes/OcaLiteTimePTP.h>
#include <OCC/ControlDataTypes/OcaLiteWorkerDataTypes.h>

// ---- Include local include files ----
#include "OcaLiteManager.h"

// ---- Referenced classes and types ----
class OcaLiteTimeSource;

// ---- Helper types and constants ----


/**
 * The classID used for initialization.
 */
#define OCA_DEVICE_TIME_MANAGER_CLASSID      OCA_MANAGER_CLASSID,static_cast< ::OcaUint16>(10)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Optional manager that allows controlling and monitoring a device's time-of-day clock.
 */
class OcaLiteDeviceTimeManager : public ::OcaLiteManager
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
		/** GetDeviceTimeNTP() */
        GET_DEVICE_TIME_NTP             = 1,
        /** SetDeviceTimeNTP() */
        SET_DEVICE_TIME_NTP             = 2,
        /** GetTimeSources() */
        GET_TIME_SOURCES                = 3,
        /** GetCurrentDeviceTimeSource() */
        GET_CURRENT_DEVICE_TIME_SOURCE  = 4,
        /** SetCurrentDeviceTimeSource() */
        SET_CURRENT_DEVICE_TIME_SOURCE  = 5,
        /** GetDeviceTimePTP() */
        GET_DEVICE_TIME_PTP             = 6,
        /** SetDeviceTimePTP() */
        SET_DEVICE_TIME_PTP             = 7
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
		/**
         * The list of ONos of OcaTimeSource objects in this device
         * @since AES70-2018.
         */
        OCA_PROP_TIME_SOURCES               = 1,
        /**
         * The current time source for this device's device time, or zero if none.
         * @since AES70-2018.
         */
        OCA_PROP_CURRENT_DEVICE_TIME_SOURCE = 2
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
    static const ::OcaLiteClassID CLASS_ID;

	virtual ::OcaBoolean Initialize();

	virtual ::OcaBoolean Shutdown();

    // ---- Interface methods ----
    /**
     * Get current value of device time-of-day clock in NTP format.
     *
     * @param[out] deviceTime     The value of device time-of-day clock in NTP format
     * @return Indicates whether the data was successfully retrieved
     * @deprecated This method is no longer used since AES70-2018. It is retained for backwards compatibility.
     */
    ::OcaLiteStatus GetDeviceTimeNTP(::OcaLiteTimeNTP& deviceTime) const;

    /**
     * Set device time-of-day clock in NTP format.
     *
     * @param[in] deviceTime     The new value of the device time-of-day clock in NTP format
     * @return Indicates whether the property was successfully set
     * @deprecated This method is no longer used since AES70-2018. It is retained for backwards compatibility.
     */
    ::OcaLiteStatus SetDeviceTimeNTP(::OcaLiteTimeNTP deviceTime);

    /**
     * Returns list of object numbers of OcaTimeSource instances in this device.
     * @since AES70-2018.
     *
     * @param[out] timeSources  Object numbers of the OcaTimeSource instances.
     * @return Indicates whether list was successfully retrieved.
     */
    ::OcaLiteStatus GetTimeSources(::OcaLiteList< ::OcaONo>& timeSources) const;

    /**
     * Retrieves ONo of current time source object, or zero if none.
     * @since AES70-2018.
     *
     * @param[out] timeSource   Object numbers of the current device time source instance.
     * @return Indicates whether value was successfully retrieved.
     */
    ::OcaLiteStatus GetCurrentDeviceTimeSource(::OcaONo& timeSource) const;

    /**
     * Sets ONo of current time source object, or zero if none.
     * @since AES70-2018.
     *
     * @param[in]  timeSource   Object numbers of the current device time source instance.
     * @return Indicates whether value was successfully set.
     */
    ::OcaLiteStatus SetCurrentDeviceTimeSource(::OcaONo timeSource);

    /**
     * Get current value of device time-of-day clock in PTP format.
     * @since AES70-2018.
     *
     * @param[out] deviceTime     The value of device time-of-day clock in PTP format
     * @return Indicates whether the data was successfully retrieved
     */
    ::OcaLiteStatus GetDeviceTimePTP(::OcaLiteTimePTP& deviceTime) const;

    /**
     * Set device time-of-day clock.
     * @since AES70-2018.
     *
     * @param[in] deviceTime     The new value of the device time-of-day clock in PTP format
     * @return Indicates whether the property was successfully set
     */
    ::OcaLiteStatus SetDeviceTimePTP(const ::OcaLiteTimePTP& deviceTime);
	
    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

    /**
     * Add a time source to the list of time sources.
     * Only for internal use.
     *
     * @param[in] timeSource    The time source to add.
     * @return Indicates whether the time source object number was added successfully; false if adding
     *         the time source object number failed or the object number was already in use.
     */
    ::OcaBoolean AddTimeSource(const ::OcaLiteTimeSource& timeSource);

    /**
     * Remove a time source from the list of time sources.
     * Only for internal use.
     *
     * @param[in] timeSource    The time source that is removed.
     */
    void RemoveTimeSource(const ::OcaLiteTimeSource& timeSource);

protected:
    /** Constructor */
    OcaLiteDeviceTimeManager();

    /** Destructor. */
    virtual ~OcaLiteDeviceTimeManager();

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;
	
	/**
     * Get the current device time-of-day clock in seconds since 00:00 hours, Jan 1, 1970 UTC. Can be overridden by derived classes
     * that actually implement this class. By default it returns the value of IOcfTimer::GetTimeNow
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[out] seconds      The number of elapsed seconds since 00:00 hours, Jan 1, 1970 UTC.
     * @param[out] nanoSeconds  The number of elapsed nano seconds since 00:00 hours, Jan 1, 1970 UTC.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetDeviceTimeValue(::OcaUint64& seconds, ::OcaUint32& nanoSeconds) const;

    /**
     * Set device time-of-day clock in seconds since 00:00 hours, Jan 1, 1970 UTC. Must be implemented by derived classes
     * that actually implement this class.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[in]  seconds      The number of elapsed seconds since 00:00 hours, Jan 1, 1970 UTC.
     * @param[in]  nanoSeconds  The number of elapsed nano seconds since 00:00 hours, Jan 1, 1970 UTC.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetDeviceTimeValue(::OcaUint64 seconds, ::OcaUint32 nanoSeconds) = 0;

    /**
     * Gets the value of the current device time source property. Can be overridden by derived classes
     * that actually implement this class.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[out] timeSource   The value of the current device time source property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetCurrentDeviceTimeSourceValue(::OcaONo& timeSource) const;

    /**
     * Set the value of the current device time source property. Must be implemented by derived classes
     * that actually implement this class. If OCA_INVALID_ONO is passed, the time source is cleared
     * and this method should always succeed.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[in]  timeSource   Input parameter that holds the value of the current device time source property.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetCurrentDeviceTimeSourceValue(::OcaONo timeSource) = 0;

private:
    /**
     * Gets the list of object numbers of OcaLiteTimeSource instances in this device.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[out] timeSources  Object numbers of the OcaLiteTimeSource instances.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus InternalGetTimeSources(::OcaLiteList< ::OcaONo>& timeSources) const;

    /** List with time source object numbers. */
    typedef std::vector< ::OcaONo>	TimeSourceONoList;
    TimeSourceONoList                   m_timeSourceList;

    /** The current device time source. */
    ::OcaONo							m_currentDeviceTimeSource;

    /** Boolean which indicates whether the current class is initialized. */
    bool                                m_bInitialized;

    /** private copy constructor, no copying of object allowed */
    OcaLiteDeviceTimeManager(const ::OcaLiteDeviceTimeManager&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteDeviceTimeManager& operator=(const ::OcaLiteDeviceTimeManager&);
};


#endif // OcaLiteDeviceTimeManager_H
