/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : Contains common timer functionality.
 *
 */

// ---- Include system wide include files ----
#include <stm32f2xx.h>

// ---- Include local include files ----
#include "FreeRTOS.h"
#include "task.h"
#include "Timer.h"

//----- Definitions ----

//----- Type declaration ----

//----- Global variables ----

//----- Local variables ----

//----- Local Function declarations ----

//****************************************************************************


unsigned long TimerGetSysTick()
{
    return (unsigned long)xTaskGetTickCount();
}
