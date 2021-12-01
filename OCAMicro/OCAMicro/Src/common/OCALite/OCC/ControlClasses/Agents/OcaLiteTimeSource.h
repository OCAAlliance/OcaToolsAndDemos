/*
*  By downloading or using this file, the user agrees to be bound by the terms of the license
*  agreement located in the LICENSE file in the root of this project
*  as an original contracting party.
*
*  Description         : OcaLiteTimeSource
*
*/
#ifndef OCALITETIMESOURCE_H
#define OCALITETIMESOURCE_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteString.h>

// ---- Include local include files ----
#include "OcaLiteAgent.h"

// ---- Referenced classes and types ----
class OcaDeviceTimeManager;

// ---- Helper types and constants ----

/**
 * Types of time sources.
 */
 enum OcaLiteTimeProtocol    /* maps onto OcaUint8 */
{
    /** Time protocol is undefined. */
    OCATIMEPROTOCOL_UNDEFINED       = 0,
    /** Time reference is inside device, so no network time protocol is used. */
    OCATIMEPROTOCOL_NONE            = 1,
    /** Time protocol is not a public standard. */
    OCATIMEPROTOCOL_PRIVATE         = 2,
    /** Time protocol is Network Time Protocol. */
    OCATIMEPROTOCOL_NTP             = 3,
    /** Time protocol is Simple Network Time Protocol. */
    OCATIMEPROTOCOL_SNTP            = 4,
    /** Time protocol is PTP (IEEE 1588), older version. */
    OCATIMEPROTOCOL_IEEE1588_2002   = 5,
    /** Time protocol is PTP (IEEE 1588), newer version. */
    OCATIMEPROTOCOL_IEEE1588_2008   = 6,
    /** Time protocol is IEEE 802.1as. */
    OCATIMEPROTOCOL_IEEE_AVB        = 7,
    /** Time protocol is AES11. */
    OCATIMEPROTOCOL_AES11           = 8,
    /** Time protocol is Genlock. */
    OCATIMEPROTOCOL_GENLOCK         = 9,
    /** Maximum value used for range checking */
    OCATIMEPROTOCOL_MAXIMUM
};

/**
 * Types of time references.
 */
enum OcaLiteTimeReferenceType   /* maps onto OcaUint8 */
{
    /** Time reference is undefined. */
    OCATIMEREFERENCETYPE_UNDEFINED  = 0,
    /** Time reference is internal to device. */
    OCATIMEREFERENCETYPE_INTERNAL   = 1,
    /** Time reference is private to the application. */
    OCATIMEREFERENCETYPE_PRIVATE    = 2,
    /** Time reference is the GPS satellite navigation system. */
    OCATIMEREFERENCETYPE_GPS        = 3,
    /** Time reference is the Galileo satellite navigation system. */
    OCATIMEREFERENCETYPE_GALILEO    = 4,
    /** Time reference is the GLONASS satellite navigation system. */
    OCATIMEREFERENCETYPE_GLONASS    = 5,
    /** Maximum value used for range checking */
    OCATIMEREFERENCETYPE_MAXIMUM
};

/**
 * States of time sources.
 */
enum OcaLiteTimeSourceAvailability  /* maps onto OcaUint8 */
{
    /** Time source is unavailable. */
    OCATIMESOURCEAVAILABILITY_UNAVAILABLE   = 0,
    /** Time source is available. */
    OCATIMESOURCEAVAILABILITY_AVAILABLE     = 1,
    /** Maximum value used for range checking */
    OCATIMESOURCEAVAILABILITY_MAXIMUM
};

/**
 * Synchronization statuses.
 */
enum OcaLiteTimeSourceSyncStatus    /* maps onto OcaUint8 */
{
    /** Lock state is undefined. */
    OCATIMESOURCESYNCSTATUS_UNDEFINED       = 0,
    /** Time source is not synchronized to reference. */
    OCATIMESOURCESYNCSTATUS_UNSYNCHRONIZED  = 1,
    /** Time source is attempting to synchronize to reference. */
    OCATIMESOURCESYNCSTATUS_SYNCHRONIZING   = 2,
    /** Time source is synchronized with reference. */
    OCATIMESOURCESYNCSTATUS_SYNCHRONIZED    = 3,
    /** Maximum value used for range checking */
    OCATIMESOURCESYNCSTATUS_MAXIMUM
};

/**
 * The classID used for initialization.
 */
#define OCA_TIMESOURCE_CLASSID          OCA_AGENT_CLASSID,static_cast< ::OcaUint16>(16)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * A time source, internal or external. See RFC 7273 for a detailed discussion of time sources.
 */
class OcaLiteTimeSource : public ::OcaLiteAgent
{
public:
    /**
    * Constructor
    *
    * @param[in]   objectNumber            Object number of this instance.
    * @param[in]   lockable                Indicates whether or not the object
    *                                      is lockable.
    * @param[in]   role                    The role of this instance.
    * @param[in]   availability            Availability of this time source.
    * @param[in]   protocol                Time transport protocol used by this time source.
    * @param[in]   parameters              Parameters (identifiers, modifiers, etc.) for this time source.
    *                                      Content is an SDP timestamp reference specification as defined in RFC7273, section 4.8.
    * @param[in]   referenceType           Type of time reference to which this time source is synced, if any.
    * @param[in]   referenceID             Identifier of reference to which this time source is synced, if any.
    *                                      Not needed for all reference types.
    * @param[in]   syncStatus              Synchronization status of this time source.
    */
    OcaLiteTimeSource(::OcaONo objectNumber, 
                      ::OcaBoolean lockable,
                      const ::OcaLiteString& role,
                      ::OcaLiteTimeSourceAvailability availability,
                      ::OcaLiteTimeProtocol protocol,
                      const ::OcaLiteString& parameters, 
                      ::OcaLiteTimeReferenceType referenceType,
                      const ::OcaLiteString& referenceID,
                      ::OcaLiteTimeSourceSyncStatus syncStatus);

    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetAvailability() */
        GET_AVAILABILITY            = 1,
        /** GetProtocol() */
        GET_PROTOCOL                = 2,
        /** SetProtocol() */
        SET_PROTOCOL                = 3,
        /** GetParameters() */
        GET_PARAMETERS              = 4,
        /** SetParameters() */
        SET_PARAMETERS              = 5,
        /** GetRefefenceType() */
        GET_REFERENCE_TYPE          = 6,
        /** SetReferenceType() */
        SET_REFERENCE_TYPE          = 7,
        /** GetReferenceID() */
        GET_REFERENCE_ID            = 8,
        /** SetReferenceID() */
        SET_REFERENCE_ID            = 9,
        /** GetSyncStatus() */
        GET_SYNC_STATUS             = 10,
        /** Reset() */
        RESET                       = 11
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Availability of this time source. */
        OCA_PROP_AVAILABILITY       = 1,
        /** Time transport protocol used by this time source. */
        OCA_PROP_PROTOCOL           = 2,
        /** Parameters (identifiers, modifiers, etc.) for this time source.
            Content is an SDP timestamp reference specification as defined in RFC7273, section 4.8. */
        OCA_PROP_PARAMETERS         = 3,
        /** Type of time reference to which this time source is synced, if any. */
        OCA_PROP_REFERENCE_TYPE     = 4,
        /** Identifier of reference to which this time source is synced, if any. Not needed for all reference types. */
        OCA_PROP_REFERENCE_ID       = 5,
        /** Synchronization status of this time source. */
        OCA_PROP_SYNC_STATUS        = 6
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteTimeSource();

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods ----

    /**
     * Gets the value of the Availability property.
     *
     * @param[out]  availability    The availability.
     * @return Indicates whether the value was successfully retrieved.
     */
    ::OcaLiteStatus GetAvailability(::OcaLiteTimeSourceAvailability& availability) const;

    /**
     * Gets the value of the Protocol property.
     *
     * @param[out]  protocol        The protocol.
     * @return Indicates whether the value was successfully retrieved.
     */
    ::OcaLiteStatus GetProtocol(::OcaLiteTimeProtocol& protocol) const;

    /**
     * Sets the value of the Protocol property.
     *
     * @param[in]   protocol        The protocol.
     * @return Indicates whether the value was successfully set.
     */
    ::OcaLiteStatus SetProtocol(::OcaLiteTimeProtocol protocol);

    /**
     * Gets the value of the Parameters property.
     *
     * @param[out]  parameters      The parameters.
     * @return Indicates whether the value was successfully retrieved.
     */
    ::OcaLiteStatus GetParameters(::OcaLiteString& parameters) const;

    /**
     * Sets the value of the Parameters property.
     *
     * @param[in]   parameters      The parameters.
     * @return Indicates whether the value was successfully set.
     */
    ::OcaLiteStatus SetParameters(const ::OcaLiteString& parameters);

    /**
     * Gets the time reference type.
     *
     * @param[out]  referenceType   The reference type.
     * @return Indicates whether the value was successfully retrieved.
     */
    ::OcaLiteStatus GetReferenceType(::OcaLiteTimeReferenceType& referenceType) const;

    /**
     * Sets the time reference type.
     *
     * @param[in]   referenceType   The reference type.
     * @return Indicates whether the value was successfully set.
     */
    ::OcaLiteStatus SetReferenceType(::OcaLiteTimeReferenceType referenceType);

    /**
     * Gets the timing source ID.
     *
     * @param[out]  id              The timing source ID.
     * @return Indicates whether the value was successfully retrieved.
     */
    ::OcaLiteStatus GetReferenceID(::OcaLiteString& id) const;

    /**
     * Sets the timing source ID.
     *
     * @param[in]   id              The timing source ID
     * @return Indicates whether the value was successfully set.
     */
    ::OcaLiteStatus SetReferenceID(const ::OcaLiteString& id);

    /**
     * Gets the synchronization status of this time source.
     *
     * @param[out]  syncStatus      The synchronization status
     * @return Indicates whether the value was successfully retrieved.
     */
    ::OcaLiteStatus GetSyncStatus(::OcaLiteTimeSourceSyncStatus& syncStatus) const;

    /**
     * Resets this time source.
     *
     * @return Indicates whether the reset was successful.
     */
    ::OcaLiteStatus Reset();

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);


protected:

    virtual const ::OcaLiteClassID& GetClassID() const;

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    /**
     * Sets the timing source ID.
     * @note Since this functionality is optional, the default implementaion of this method
     *       will return OCASTATUS_NOT_IMPLEMENTED.
     *
     * @param[in]   id              The timing source ID.
     * @return Indicates whether the value was successfully set.
     */
    virtual ::OcaLiteStatus SetReferenceIDValue(const ::OcaLiteString& id);

    /**
     * Sets the synchronization status of this time source.
     *
     * @param[in]   syncStatus      The synchroniziation status
     */
    void SetSyncStatusValue(::OcaLiteTimeSourceSyncStatus syncStatus);

private:

    /** The availability */
    ::OcaLiteTimeSourceAvailability     m_availability;

    /** The time protocol */
    ::OcaLiteTimeProtocol               m_protocol;

    /** The parameters */
    ::OcaLiteString                     m_parameters;

    /** The reference type */
    ::OcaLiteTimeReferenceType          m_referenceType;

    /** The timing source ID */
    ::OcaLiteString                     m_referenceID;

    /** The synchronization status */
    ::OcaLiteTimeSourceSyncStatus       m_syncStatus;

    /** private copy constructor, no copying of object allowed */
    OcaLiteTimeSource(const ::OcaLiteTimeSource&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteTimeSource& operator=(const ::OcaLiteTimeSource&);
};

// ---- Specialized Template Function Definition ----
template <>
void MarshalValue< ::OcaLiteTimeProtocol>(const ::OcaLiteTimeProtocol& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaLiteTimeReferenceType>(const ::OcaLiteTimeReferenceType& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaLiteTimeSourceAvailability>(const ::OcaLiteTimeSourceAvailability& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaLiteTimeSourceSyncStatus>(const ::OcaLiteTimeSourceSyncStatus& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::OcaLiteTimeProtocol>(::OcaLiteTimeProtocol& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaLiteTimeReferenceType>(::OcaLiteTimeReferenceType& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaLiteTimeSourceAvailability>(::OcaLiteTimeSourceAvailability& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaLiteTimeSourceSyncStatus>(::OcaLiteTimeSourceSyncStatus& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteTimeProtocol>(const ::OcaLiteTimeProtocol& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteTimeReferenceType>(const ::OcaLiteTimeReferenceType& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteTimeSourceAvailability>(const ::OcaLiteTimeSourceAvailability& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteTimeSourceSyncStatus>(const ::OcaLiteTimeSourceSyncStatus& value, const ::IOcaLiteWriter& writer);

#endif // OCATIMESOURCE_H
