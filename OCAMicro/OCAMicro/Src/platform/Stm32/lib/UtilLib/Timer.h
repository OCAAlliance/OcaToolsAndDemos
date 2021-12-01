/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : Contains common timer functionality.
 *
 */

#ifndef TIMER_H
#define TIMER_H

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---- Definitions ----

#define TIMER_EXTERNAL_SOURCE (true)
#define TIMER_INTERNAL_SOURCE (false)
	
/** Macro which is used to check that the timer is elapsed. */
#define INTERVAL_ELAPSED( startTime, interval )       \
    (((unsigned long)(TimerGetSysTick() - (startTime))) >= (interval))

// ---- Type declaration ----

// ---- Global variables ----

// ---- Function declarations ----


/**
 * Function that returns the current SysTick value.
 *
 * @return The current system tick count.
 */
unsigned long TimerGetSysTick(void);

#ifdef __cplusplus
}
#endif

#endif // TIMER_H

