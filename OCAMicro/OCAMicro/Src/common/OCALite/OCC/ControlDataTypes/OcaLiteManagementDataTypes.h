/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OCC management data types.
 *
 */

#ifndef OCALITEMANAGEMENTDATATYPES_H
#define OCALITEMANAGEMENTDATATYPES_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "OcaLiteBaseDataTypes.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

/**
 * @addtogroup ManagementDataTypes
 * @{
 */

/**
 * Enumeration of software and firmware components in the device. Except for the
 * boot loader, all other values of this enumeration are device-specific and will
 * be specified by subclassing this enumeration.
 */
//lint -strong(AJX, OcaLiteComponent)
enum OcaLiteComponent /* maps onto OcaUint16 */
{
    /** The boot loader image. */
    OCACOMPONENT_BOOT_LOADER            = 0
};

/** @} */ /* end of adding to group OCA */

// ---- Helper functions ----

// ---- Specialized Template Function Definition ----

//lint -save -e1576 Explicit specialization does not occur in the same file as corresponding function template

template <>
void MarshalValue< ::OcaLiteComponent>(const ::OcaLiteComponent& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::OcaLiteComponent>(::OcaLiteComponent& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteComponent>(const ::OcaLiteComponent& value, const ::IOcaLiteWriter& writer);

//lint -restore

#endif /* OCALITEMANAGEMENTDATATYPES_H */
