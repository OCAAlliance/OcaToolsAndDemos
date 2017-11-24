/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OCC base data types.
 *
 */

#ifndef OCALITEBASEDATATYPES_H
#define OCALITEBASEDATATYPES_H

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

/**
 * @addtogroup DataTypes
 * @{
 */

/** True or false */
/*lint -strong(AJX, OcaBoolean)*/
typedef bool            OcaBoolean;

/** Generic 8-bit integer parameter */
/*lint -strong(AJX, OcaInt8)*/
typedef INT8            OcaInt8;

/** Generic 16-bit integer parameter */
/*lint -strong(AJX, OcaInt16)*/
typedef INT16           OcaInt16;

/** Generic 32-bit integer parameter */
/*lint -strong(AJX, OcaInt32)*/
typedef INT32           OcaInt32;

/** Generic 64-bit integer parameter */
/*lint -strong(AJX, OcaInt64)*/
typedef INT64           OcaInt64;

/** Generic 8-bit unsigned integer parameter */
/*lint -strong(AJX, OcaUint8)*/
typedef UINT8           OcaUint8;

/** Generic 16-bit unsigned integer parameter */
/*lint -strong(AJX, OcaUint16)*/
typedef UINT16          OcaUint16;

/** Generic 32-bit unsigned integer parameter */
/*lint -strong(AJX, OcaUint32)*/
typedef UINT32          OcaUint32;

/** Generic 64-bit unsigned integer parameter */
/*lint -strong(AJX, OcaUint64)*/
typedef UINT64          OcaUint64;

/** Generic 32-bit float parameter */
/*lint -strong(AJX, OcaFloat32)*/
typedef FLOAT           OcaFloat32;

/** @} */ /* end of adding to group OCA */

/** Enumeration that describes all available base data types. */
//lint -strong(AJX, OcaLiteBaseDataType)
enum OcaLiteBaseDataType    /* maps onto OcaUint8 */
{
    /** Undefined */
    OCABASEDATATYPE_NONE                = 0,
    /** True or false */
    OCABASEDATATYPE_BOOLEAN             = 1,
    /** Generic 8-bit integer parameter */
    OCABASEDATATYPE_INT8                = 2,
    /** Generic 16-bit integer parameter */
    OCABASEDATATYPE_INT16               = 3,
    /** Generic 32-bit integer parameter */
    OCABASEDATATYPE_INT32               = 4,
    /** Generic 64-bit integer parameter */
    OCABASEDATATYPE_INT64               = 5,
    /** Generic 8-bit unsigned integer parameter */
    OCABASEDATATYPE_UINT8               = 6,
    /** Generic 16-bit unsigned integer parameter */
    OCABASEDATATYPE_UINT16              = 7,
    /** Generic 32-bit unsigned integer parameter */
    OCABASEDATATYPE_UINT32              = 8,
    /** Generic 64-bit unsigned integer parameter */
    OCABASEDATATYPE_UINT64              = 9,
    /** Generic 32-bit float parameter */
    OCABASEDATATYPE_FLOAT32             = 10,
    /** Generic 64-bit float parameter */
    OCABASEDATATYPE_FLOAT64             = 11,
    /** String */
    OCABASEDATATYPE_STRING              = 12,
    /** Bit string */
    OCABASEDATATYPE_BIT_STRING          = 13,
    /** Binary Large Object */
    OCABASEDATATYPE_BLOB                = 14,
    /** BLOB with a fixed length */
    OCABASEDATATYPE_BLOB_FIXED_LEN      = 15,
    /** Maximum value used for range checking */
    OCABASEDATATYPE_MAXIMUM
};

// ---- Helper functions ----

/** Accuracy for comparing floats */
#define FLOAT_COMPARE_ACCURACY          1e-16f

#endif /* OCALITEBASEDATATYPES_H */
