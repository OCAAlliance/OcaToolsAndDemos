/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Contains common watchdog functionality
 *
 */

#ifndef WATCHDOG_H
#define WATCHDOG_H

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---- Definitions ----

// ---- Type declaration ----

// ---- Global variables ----

// ---- Function declarations ----

/**
 * Function to be called to initialize the Watchdog part of the UtilLib.
 *
 * @param[in] timeout The timeout value of the watchdog in ms.
 */
void WatchdogInit(UINT16 timeout);

/**
 * Function to be called to start the watchdog.
 */
void StartWatchdog(void);

/**
 * Function to be called each main cycle to refresh the watchdog, to prevent
 * the watchdog to reset the device.
 */
void RefreshWatchdog(void);

#ifdef __cplusplus
}
#endif

#endif // WATCHDOG_H
