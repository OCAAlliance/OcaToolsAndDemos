/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : The timer implementation of the Host Interface for
 *                        a platform running FreeRTOS.
 *
 */

// ---- Include system wide include files ----
#include <Stm32CortexM3/lib/UtilLib/Timer.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include <HostInterfaceLite/OCA/OCF/Timer/IOcfLiteTimer.h>

// ---- Helper types and constants ----

// Platform-specific implementation of static method 'GetTimerTickCount' of base class
UINT32 OcfLiteTimerGetTimerTickCount(void)
{
    return ::TimerGetSysTick();
}
