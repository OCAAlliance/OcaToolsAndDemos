/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : The timer implementation of the Host Interface for
 *                        a platform running FreeRTOS.
 *
 */

// ---- Include system wide include files ----
#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#endif

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include <HostInterfaceLite/OCA/OCF/Timer/IOcfLiteTimer.h>

// ---- Helper types and constants ----

// Platform-specific implementation of static method 'GetTimerTickCount' of base class
UINT32 OcfLiteTimerGetTimerTickCount(void)
{
#ifdef _WIN32
    return ::GetTickCount();
#else
    struct timeval start;
    gettimeofday(&start, NULL);
    return (UINT32)((start.tv_sec) * 1000 + start.tv_usec/1000.0);
#endif
}
