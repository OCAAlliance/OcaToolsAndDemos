/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The timer interface of the Host Interface.
 *
 */

#ifndef IOCFLITETIMER_H
#define IOCFLITETIMER_H

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Retrieves the number of milliseconds that have elapsed since the
 * system was started.
 * @note The implementation of this method needs to be included in the host interface
 *       implementation of the specific platform.
 *
 * @return The number of milliseconds that have elapsed since the
 *         system was started.
 */
UINT32 OcfLiteTimerGetTimerTickCount(void);

#endif // IOCFLITETIMER_H
