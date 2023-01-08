/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteMediaClockManager
 *
 */
#ifndef OCALITEMEDIACLOCKMANAGER_H
#define OCALITEMEDIACLOCKMANAGER_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "OcaLiteManager.h"

// ---- Referenced classes and types ----
#include <OCC/ControlClasses/Agents/OcaLiteMediaClock.h>
#include <OCC/ControlClasses/Agents/OcaLiteMediaClock3.h>
#include <OCC/ControlDataTypes/OcaLiteList.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- Helper types and constants ----

/**
 * The classID used for initialization.
 */
#define OCA_MEDIACLOCKMANAGER_CLASSID      OCA_MANAGER_CLASSID,static_cast< ::OcaUint16>(7)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Anchor manager for all media clocks that the device uses and/or sources.
 * Must be instantiated once for every device that has media streaming capabilities.
 * May be instantiated once for a device without such capabilities.
 */
class OcaLiteMediaClockManager : public ::OcaLiteManager
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetClocks() */
        GET_CLOCKS                          = 1,
        /** GetMediaClockTypesSupported() */
        GET_MEDIACLOCKTYPESSUPPORTED        = 2,
        /** GetClock3s() */
        GET_CLOCK3S                         = 3
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** List of clock source types supported by this device */
        OCA_PROP_CLOCK_SOURCE_TYPES_SUPPORTED  = 1,
        /** Object numbers of OcaLiteMediaClock objects,
            one for each clock which this device uses and/or sources. */
        OCA_PROP_CLOCKS                        = 2,
        /**
         * Object numbers of OcaMediaClock3 objects,
         * one for each clock which this device uses and/or sources.
         */
        OCA_PROP_CLOCK3S                       = 3
    };

    /**
     * The object number of the media clock manager
     */
    static const ::OcaONo OBJECT_NUMBER;

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    /**
     * Getter for the OcaLiteMediaClockManager singleton.
     *
     * @return Reference to the OcaLiteMediaClockManager.
     */
    static ::OcaLiteMediaClockManager& GetInstance();

    /**
     * Cleans up the singleton instance.
     */
    static void FreeInstance();

    virtual ::OcaBoolean Initialize();

    virtual ::OcaBoolean Shutdown();

    // ---- Interface methods ----

    /**
     * Gets the list of object numbers of OcaLiteMediaClock instances in this device.
     *
     * @param[out] clocks  Object numbers of the OcaLiteMediaClock instances.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetClocks(::OcaLiteList< ::OcaONo>& clocks) const;

    /**
    * Gets the list of object numbers of OcaLiteMediaClock3 instances in this device.
    *
    * @param[out] clocks  Object numbers of the OcaLiteMediaClock3 instances.
    * @return Indicates whether the operation succeeded.
    */
    ::OcaLiteStatus GetClock3s(::OcaLiteList< ::OcaONo>& clocks) const;

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

    /**
     * Add a media clock to the list of media clocks.
     * Only for internal use.
     *
     * @param[in] clock     The media clock to add.
     * @return Indicates whether the media clock object number was added successfully; false if adding
     *         the media clock object number failed or the object number was already in use.
     */
    ::OcaBoolean AddMediaClock(::OcaLiteMediaClock& clock);

    /**
     * Remove a media clock from the list of media clocks.
     * Only for internal use.
     *
     * @param[in] clock     The media clock that is removed.
     */
    void RemoveMediaClock(const ::OcaLiteMediaClock& clock);

    /**
     * Add a media clock3 to the list of media clocks.
     * Only for internal use.
     *
     * @param[in] clock     The media clock to add.
     * @return Indicates whether the media clock object number was added successfully; false if adding
     *         the media clock object number failed or the object number was already in use.
     */
    ::OcaBoolean AddMediaClock3(::OcaLiteMediaClock3& clock);

    /**
     * Remove a media clock3 from the list of media clocks.
     * Only for internal use.
     *
     * @param[in] clock     The media clock that is removed.
     */
    void RemoveMediaClock3(const ::OcaLiteMediaClock3& clock);

protected:
    /**
     * Constructor
     */
    OcaLiteMediaClockManager();

    /**
     * Destructor.
     */
    virtual ~OcaLiteMediaClockManager();

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

private:
    /**
     * Notifies the supported media clock types property changed.
     *
     * @param[in]   changeType          The change type to notify.
     */
    void NotifySupportedClockTypesChanged(::OcaLitePropertyChangeType changeType);

    /** Singleton instance of the server */
    static ::OcaLiteMediaClockManager*          m_pSingleton;

    /** List with media clocks. */
    typedef std::map< ::OcaONo, ::OcaLiteMediaClock*> MediaClockList;
    MediaClockList                          m_mediaClockList;

    /** List with media clocks. */
    typedef std::map< ::OcaONo, ::OcaLiteMediaClock3*> MediaClock3List;
    MediaClock3List                         m_mediaClock3List;

    /** Boolean which indicates whether the current class is initialized. */
    bool                                    m_bInitialized;

    /** Map of media clock types and the number of clocks of that type registered. */
    typedef std::map< ::OcaLiteMediaClockType, int> MediaClockTypeList;
    MediaClockTypeList                      m_mediaClockTypeList;

    /** private copy constructor, no copying of object allowed */
    OcaLiteMediaClockManager(const ::OcaLiteMediaClockManager&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteMediaClockManager& operator=(const ::OcaLiteMediaClockManager&);
};

#endif // OCALITEMEDIACLOCKMANAGER_H
