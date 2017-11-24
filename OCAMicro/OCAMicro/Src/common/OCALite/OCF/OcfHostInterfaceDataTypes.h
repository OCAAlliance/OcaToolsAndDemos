/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Host Interface specific data types.
 *
 */

#ifndef OCFHOSTINTERFACEDATATYPES_H
#define OCFHOSTINTERFACEDATATYPES_H

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

/**
 * @addtogroup OCFHostInterface
 * @{
 */

/**
 * Enumeration of Host Interface reset causes.
 */
//lint -strong(AJX, ResetCause)
enum ResetCause
{
    /** Reset due to powering up. */
    POWER_ON         = 0,
    /** Reset due to internal error. */
    INTERNAL_ERROR   = 1,
    /** Reset following upgrade of firmware. */
    UPGRADE          = 2,
    /**
     * Reset due to an external request (i.e. Reset method of DeviceManager
     * or hardware reset pin).
     */
    EXTERNAL_REQUEST = 3
};

/** @} */ // end addtogroup OcfHostInterface

// ---- Helper functions ----

#endif /* OCFHOSTINTERFACEDATATYPES_H */
