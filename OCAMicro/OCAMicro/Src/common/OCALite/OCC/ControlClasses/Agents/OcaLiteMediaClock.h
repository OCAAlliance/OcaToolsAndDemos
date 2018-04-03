/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMediaClock
 *
 */

#ifndef OCALITEMEDIACLOCK_H
#define OCALITEMEDIACLOCK_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteList.h>
#include <OCC/ControlDataTypes/OcaLiteMediaClockRate.h>
#include <OCF/OcaLiteCommandHandler.h>

// ---- Include local include files ----
#include "OcaLiteAgent.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/**
 * Lock states of media clocks.
 */
enum OcaLiteMediaClockLockState     /* maps onto OcaUint8 */
{
    /** Lock state is undefined. */
    OCAMEDIACLOCKLOCKSTATE_UNDEFINED        = 0,
    /** Media clock is locked. */
    OCAMEDIACLOCKLOCKSTATE_LOCKED           = 1,
    /** Media clock is attempting to lock. */
    OCAMEDIACLOCKLOCKSTATE_SYNCHRONIZING    = 2,
    /** Media clock is free-running. */
    OCAMEDIACLOCKLOCKSTATE_FREERUN          = 3,
    /** Media clock is stopped. */
    OCAMEDIACLOCKLOCKSTATE_STOPPED          = 4,
    /** Maximum value used for range checking. */
    OCAMEDIACLOCKLOCKSTATE_MAXIMUM
};

/**
 * Types of media clocks.
 */
enum OcaLiteMediaClockType      /* maps onto OcaUint8 */
{
    /** No network */
    OCAMEDIACLOCKTYPE_NONE      = 0,
    /** Internal media clock */
    OCAMEDIACLOCKTYPE_INTERNAL  = 1,
    /** Network media clock */
    OCAMEDIACLOCKTYPE_NETWORK   = 2,
    /** External media clock */
    OCAMEDIACLOCKTYPE_EXTERNAL  = 3,
    /** Maximum value used for range checking. */
    OCAMEDIACLOCKTYPE_MAXIMUM
};

/**
 * The classID used for initialization.
 */
#define OCA_MEDIACLOCK_CLASSID      OCA_AGENT_CLASSID,static_cast< ::OcaUint16>(6)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * A media clock, internal or external
 */
class OcaLiteMediaClock : public ::OcaLiteAgent
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetType() */
        GET_TYPE            = 1,
        /** SetType() */
        SET_TYPE            = 2,
        /** GetDomainID() */
        GET_DOMAIN_ID       = 3,
        /** SetDomainID() */
        SET_DOMAIN_ID       = 4,
        /** GetRatesSupported() */
        GET_RATES_SUPPORTED = 5,
        /** GetRate() */
        GET_RATE            = 6,
        /** SetRate() */
        SET_RATE            = 7,
        /** GetLockState() */
        GET_LOCK_STATE      = 8,
		/** GetTypesSupported() */
		GET_TYPES_SUPPORTED = 9,
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** The type of the clock. */
        OCA_PROP_TYPE               = 1,
        /** Clock domain ID. Arbitrary value. */
        OCA_PROP_DOMAIN_ID          = 2,
        /** List of supported rates. */
        OCA_PROP_RATES_SUPPORTED    = 3,
        /** Current clock rate. */
        OCA_PROP_CURRENT_RATE       = 4,
        /** Lock state of clock. */
        OCA_PROP_LOCK_STATE         = 5
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteMediaClock();

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods ----

    /**
     * Gets the value of the Type property.
     *
     * @param[out] type The media clock type.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetType(::OcaLiteMediaClockType& type) const;

    /**
     * Sets the value of the Type property.
     *
     * @param[in] type  The media clock type.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus SetType(::OcaLiteMediaClockType type);

    /**
     * Gets the value of the DomainID property.
     *
     * @param[out] id    The domain ID.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetDomainID(::OcaUint16& id) const;

    /**
     * Sets the value of the DomainID property.
     *
     * @param[in] id    The domain ID.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus SetDomainID(::OcaUint16 id);

	/**
     * Gets the list of supported clock types.
     *
     * @param[out] types The supported clock types.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetTypesSupported(::OcaLiteList< ::OcaLiteMediaClockType>& types) const;

    /**
     * Gets the list of supported sampling rates.
     *
     * @param[out] rates The supported samples rates.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetRatesSupported(::OcaLiteList< ::OcaLiteMediaClockRate>& rates) const;

    /**
     * Gets the current sampling rate.
     *
     * @param[out] rate The sampling rate.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetRate(::OcaLiteMediaClockRate& rate) const;

    /**
     * Sets the sampling rate
     *
     * @param[in] rate  The sampling rate.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus SetRate(const ::OcaLiteMediaClockRate& rate);

    /**
     * Gets the current media clock lock state.
     *
     * @param[out] state    The media clock lock state.
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetLockState(::OcaLiteMediaClockLockState& state) const;

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

protected:
    /**
     * Constructor
     *
     * @param[in]   objectNumber            Object number of this instance.
     * @param[in]   lockable                Indicates whether or not the object
     *                                      is lockable.
     * @param[in]   role                    The role of this instance.
     * @param[in]   type                    The clock type.
     * @param[in]   domainID                The domain ID value.
     * @param[in]   ratesSupported          List with the supported rates of this instance.
     */
    OcaLiteMediaClock(::OcaONo objectNumber,
                  ::OcaBoolean lockable,
                  const ::OcaLiteString& role,
                  ::OcaLiteMediaClockType type,
                  ::OcaUint16 domainID,
                  const ::OcaLiteList< ::OcaLiteMediaClockRate>& ratesSupported);

    /**
     * Constructor
     *
     * @param[in]   objectNumber            Object number of this instance.
     * @param[in]   lockable                Indicates whether or not the object
     *                                      is lockable.
     * @param[in]   role                    The role of this instance.
     * @param[in]   type                    The clock type.
	 * @param[in]   typesSupported			List with the supported clock types of this instance.
     * @param[in]   domainID                The domain ID value.
     * @param[in]   ratesSupported          List with the supported rates of this instance.
     */
    OcaLiteMediaClock(::OcaONo objectNumber,
                  ::OcaBoolean lockable,
                  const ::OcaLiteString& role,
                  ::OcaLiteMediaClockType type,
				  const ::OcaLiteList< ::OcaLiteMediaClockType> typesSupported,
                  ::OcaUint16 domainID,
                  const ::OcaLiteList< ::OcaLiteMediaClockRate>& ratesSupported);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    /**
     * Gets the current media clock type.
     *
     * @param[out] type  The media clock type.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetTypeValue(::OcaLiteMediaClockType& type) const;

    /**
     * Sets the value of the Type property.
     *
     * @param[in] type  The media clock type.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetTypeValue(::OcaLiteMediaClockType type) = 0;

    /**
     * Gets the current domain ID.
     *
     * @param[out] id    The domain ID.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetDomainIDValue(::OcaUint16& id) const;

    /**
     * Sets the value of the DomainID property.
     *
     * @param[in] id    The domain ID.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetDomainIDValue(::OcaUint16 id) = 0;

    /**
     * Gets the current sampling rate.
     *
     * @param[out] rate The sampling rate.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetRateValue(::OcaLiteMediaClockRate& rate) const = 0;

    /**
     * Sets the sampling rate
     *
     * @param[in] rate  The sampling rate.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetRateValue(const ::OcaLiteMediaClockRate& rate) = 0;

    /**
     * Gets the current media clock lock state.
     *
     * @param[out] state    The media clock lock state.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetLockStateValue(::OcaLiteMediaClockLockState& state) const = 0;

    /**
     * Event that is emitted when the LockState property has changed.
     *
     * @param[in] state      The value of lock state.
     */
    void LockStateChanged(::OcaLiteMediaClockLockState state);

    /**
     * Event that is emitted when the ClockType property has changed.
     *
     * @param[in] type      The value of clock type.
     */
    void ClockTypeChanged(::OcaLiteMediaClockType type);

private:
    /** The media clock type */
    ::OcaLiteMediaClockType             m_clockType;

    /** The domain ID */
    ::OcaUint16                     m_domainID;

    /** The supported sampling rates */
    ::OcaLiteList< ::OcaLiteMediaClockRate> m_ratesSupported;

	/** The supported clock types */
	::OcaLiteList< ::OcaLiteMediaClockType> m_typesSupported;

    /** private copy constructor, no copying of object allowed */
    OcaLiteMediaClock(const ::OcaLiteMediaClock&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteMediaClock& operator=(const ::OcaLiteMediaClock&);
};

// ---- Specialized Template Function Definition ----
template <>
void MarshalValue< ::OcaLiteMediaClockLockState>(const ::OcaLiteMediaClockLockState& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaLiteMediaClockType>(const ::OcaLiteMediaClockType& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::OcaLiteMediaClockLockState>(::OcaLiteMediaClockLockState& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaLiteMediaClockType>(::OcaLiteMediaClockType& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteMediaClockLockState>(const ::OcaLiteMediaClockLockState& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteMediaClockType>(const ::OcaLiteMediaClockType& value, const ::IOcaLiteWriter& writer);

#endif // OCALITEMEDIACLOCK_H
