/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMediaClock3
 *
 */

#ifndef OCALITEMEDIACLOCK3_H
#define OCALITEMEDIACLOCK3_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteList.h>
#include <OCC/ControlDataTypes/OcaLiteMediaClockRate.h>
#include <OCF/OcaLiteCommandHandler.h>

// ---- Include local include files ----
#include "OcaLiteAgent.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

/**
 * Availability states of media clocks.
 */
enum OcaLiteMediaClockAvailability  /* maps onto OcaUint8 */
{
    /** Media clock is unavailable. */
    OCAMEDIACLOCKAVAILABILITY_UNAVAILABLE   = 0,
    /** Media clock is available. */
    OCAMEDIACLOCKAVAILABILITY_AVAILABLE     = 1,
    /** Maximum value used for range checking */
    OCAMEDIACLOCKAVAILABILITY_MAXIMUM
};

/**
 * Clock offset in NTP format which is 32-bits for the number
 * of seconds followed by a 32-bit part for the fractional second.
 */
typedef ::OcaUint64     OcaLiteTimeOffset;

/**
 * The classID used for initialization.
 */
#define OCA_MEDIACLOCK3_CLASSID      OCA_AGENT_CLASSID,static_cast< ::OcaUint16>(15)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * A media clock, internal or external
 */
class OcaLiteMediaClock3 : public ::OcaLiteAgent
{
public:

    /**
    * Constructor
    *
    * @param[in]   objectNumber         Object number of this instance.
    * @param[in]   lockable             Indicates whether or not the object
    *                                   is lockable.
    * @param[in]   role                 The role of this instance.
    * @param[in]   availability         The media clock availability.   
    * @param[in]   rate                 The current media clock rate.
    * @param[in]   timeSourceONo        The time source object number.
    * @param[in]   offset               The time offset.
    * @param[in]   ratesSupported       List with the supported rates of this instance.
    */
    OcaLiteMediaClock3(::OcaONo objectNumber,
                       ::OcaBoolean lockable,
                       const ::OcaLiteString& role,
                       ::OcaLiteMediaClockAvailability availablility,
                       ::OcaLiteMediaClockRate& rate,
                       ::OcaONo timeSourceONo,
                       ::OcaLiteTimeOffset offset,
                       const ::OcaLiteList< ::OcaLiteMediaClockRate>& ratesSupported);

    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetAvailability() */
        GET_AVAILABILITY    = 1,
        /** SetAvailability() */
        SET_AVAILABILITY    = 2,
        /** GetCurrentRate() */
        GET_CURRENT_RATE    = 3,
        /** SetCurrentRate() */
        SET_CURRENT_RATE    = 4,
        /** GetOffset() */
        GET_OFFSET          = 5,
        /** SetOffset() */
        SET_OFFSET          = 6,
        /** GetSupportedRates() */
        GET_SUPPORTED_RATES = 7
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Availability of media clock. */
        OCA_PROP_AVAILABILITY       = 1,
        /** ONo of time source that drives this media clock. */
        OCA_PROP_TIME_SROUCE_ONO    = 2,
        /** Offset of media clock time from reference time. MediaClockTime = ReferenceTime + Offset. */
        OCA_PROP_OFFSET             = 3,
        /** Current clock rate. */
        OCA_PROP_CURRENT_RATE       = 4
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteMediaClock3();

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods ----

    ::OcaLiteStatus GetAvailability(::OcaLiteMediaClockAvailability& availablility) const;

    ::OcaLiteStatus SetAvailability(::OcaLiteMediaClockAvailability availablility);

    ::OcaLiteStatus GetCurrentRate(::OcaLiteMediaClockRate& rate, ::OcaONo& timeSourceONo) const;

    ::OcaLiteStatus SetCurrentRate(::OcaLiteMediaClockRate rate, ::OcaONo timeSourceONo);

    ::OcaLiteStatus GetOffset(::OcaLiteTimeOffset& offset);

    ::OcaLiteStatus SetOffset(::OcaLiteTimeOffset offset);

    ::OcaLiteStatus GetSupportedRates(::OcaLiteList< ::OcaLiteMediaClockRate> ratesSupported);

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

protected:

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

private:
    /** The media clock availability */
    ::OcaLiteMediaClockAvailability         m_availablility;
    /** The current clock rate */
    ::OcaLiteMediaClockRate                 m_rate;
    /** The current time source */
    ::OcaONo                                m_timeSourceONo;
    /** The time offset */
    ::OcaLiteTimeOffset                     m_offset;
    /** The supported sampling rates */
    ::OcaLiteList< ::OcaLiteMediaClockRate> m_ratesSupported;

    /** private copy constructor, no copying of object allowed */
    OcaLiteMediaClock3(const ::OcaLiteMediaClock3&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteMediaClock3& operator=(const ::OcaLiteMediaClock3&);
};

// ---- Specialized Template Function Definition ----
template <>
void MarshalValue< ::OcaLiteMediaClockAvailability>(const ::OcaLiteMediaClockAvailability& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::OcaLiteMediaClockAvailability>(::OcaLiteMediaClockAvailability& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteMediaClockAvailability>(const ::OcaLiteMediaClockAvailability& value, const ::IOcaLiteWriter& writer);

#endif // OCALITEMEDIACLOCK_H
