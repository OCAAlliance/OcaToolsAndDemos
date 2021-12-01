/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
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

/**
 * Retrieves the current time in seconds since 00:00 hours, Jan 1, 1970 UTC.
 * @note The implementation of this method needs to be included in the host interface
 *       implementation of the specific platform.
 *
 * @param[out]  seconds     The number of elapsed seconds since 00:00 hours, Jan 1, 1970 UTC.
 * @param[out]  nanoSeconds The number of elapsed nano seconds since 00:00 hours, Jan 1, 1970 UTC.
 * @return true if the current time was successfully retrieved.
 */
bool OcfLiteTimerGetTimeNow(UINT64& seconds, UINT32& nanoSeconds);

#endif // IOCFLITETIMER_H
