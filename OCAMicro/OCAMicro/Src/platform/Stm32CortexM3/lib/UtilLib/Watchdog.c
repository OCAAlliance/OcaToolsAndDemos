/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : Contains the watchdog functionality
 *
 */

//ToDo (aaa)

// ---- Include system wide include files ----
#include <stm32f2xx.h>

// ---- Include local include files ----
#include "Watchdog.h"

//----- Definitions ----
/** 
 * Possible Watchdog values:
 * Prescale | Min Timeout | Max Timeout
 *     /4         0.1ms       409.6ms           
 *     /8         0.2ms       819.2ms
 *    /16         0.4ms      1638.4ms
 *    /32         0.8ms      3276.8ms
 *    /64         1.6ms      6553.6ms
 *   /128         3.2ms     13107.2ms
 *   /256         6.4ms     26214.4ms
 */

/** The max reload value of the watchdog */
#define MAX_RELOAD_VALUE 0x0FFF
/** The minimum step for the watchdog timeout calculation */
#define MIN_TIMEOUT_STEP 6.4
/** The maximum timeout value of the watchdog */
#define MAX_TIMEOUT_VALUE 26214
/** The prescaler to use, prescaler 256 max timeout is 26214.4ms */
#define WATCHDOG_PRESCALER IWDG_Prescaler_256
//----- Type declaration ----

//----- Global variables ----

//----- Local variables ----

//----- Local Function declarations ----

//****************************************************************************

void WatchdogInit(UINT16 timeout)
{
	UINT16 reloadValue = MAX_RELOAD_VALUE;
    if (MAX_TIMEOUT_VALUE > timeout)
    {
        reloadValue = (UINT16)(timeout / MIN_TIMEOUT_STEP);
    }
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetReload(reloadValue);
    IWDG_SetPrescaler(WATCHDOG_PRESCALER);
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
}

void StartWatchdog(void)
{
    IWDG_Enable();
}

void RefreshWatchdog(void)
{
    IWDG_ReloadCounter();
}
