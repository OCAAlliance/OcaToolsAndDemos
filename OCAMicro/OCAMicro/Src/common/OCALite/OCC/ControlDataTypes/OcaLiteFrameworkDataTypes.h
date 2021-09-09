/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OCC framework data types.
 *
 */

#ifndef OCALITEFRAMEWORKDATATYPES_H
#define OCALITEFRAMEWORKDATATYPES_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "OcaLiteBaseDataTypes.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

/** Class version number, ascending from 1. */
typedef ::OcaUint16     OcaClassVersionNumber;

/** Object number of an OCA object. */
typedef ::OcaUint32     OcaONo;

/** The invalid object number */
#define OCA_INVALID_ONO static_cast< ::OcaONo>(0)

/** Standard status codes returned from method calls. */
enum OcaLiteStatus  /* maps onto OcaUint8 */
{
    /** Operation succeeded. */
    OCASTATUS_OK                        = 0,
    /** OCA version conflict between device and message. */
    OCASTATUS_PROTOCOL_VERSION_ERROR    = 1,
    /** Internal error in device. */
    OCASTATUS_DEVICE_ERROR              = 2,
    /** Object locked by another process, operation aborted. */
    OCASTATUS_LOCKED                    = 3,
    /** Format error in the message. */
    OCASTATUS_BAD_FORMAT                = 4,
    /** Addressed object does not exist in device. */
    OCASTATUS_BAD_ONO                   = 5,
    /** Illegal or missing parameter(s) in the message. */
    OCASTATUS_PARAMETER_ERROR           = 6,
    /** The value of a passed parameter is out of range. */
    OCASTATUS_PARAMETER_OUT_OF_RANGE    = 7,
    /** Requested action not implemented in target object. */
    OCASTATUS_NOT_IMPLEMENTED           = 8,
    /** Invalid request for current context. */
    OCASTATUS_INVALID_REQUEST           = 9,
    /** Requested processing failed. */
    OCASTATUS_PROCESSING_FAILED         = 10,
    /** Addressed object does not support the method. */
    OCASTATUS_BAD_METHOD                = 11,
    /** Operation partially succeeded (e.g. routing setup). */
    OCASTATUS_PARTIALLY_SUCCEEDED       = 12,
    /** Operation failed because of a timeout. */
    OCASTATUS_TIMEOUT                   = 13,
    /** Operation failed because of a buffer overflow. */
    OCASTATUS_BUFFER_OVERFLOW           = 14,
    /** The user does not have permission to execute this method */
    OCASTATUS_PERMISSION_DENIED         = 15,
    /** Maximum value used for range checking. */
    OCASTATUS_MAXIMUM
};

/** Transport layer session ID. */
typedef ::OcaUint16     OcaSessionID;

/** The invalid session ID */
#define OCA_INVALID_SESSIONID   static_cast< ::OcaSessionID>(0)

// ---- Helper functions ----

// ---- Specialized Template Function Definition ----

template <>
void MarshalValue< ::OcaLiteStatus>(const ::OcaLiteStatus& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::OcaLiteStatus>(::OcaLiteStatus& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteStatus>(const ::OcaLiteStatus& value, const ::IOcaLiteWriter& writer);

#endif /* OCALITEFRAMEWORKDATATYPES_H */
