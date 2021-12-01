/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 */

//=========================================================================
//  File Name: rowley_locks.c
//
//  Description:
//      Helper functions for using Rowley C library in multi-thread 
//      environment
//  
//  Modification History:
//      $Id: $
//      $Log:  $
//
//=========================================================================
#include "FreeRTOS.h"
#include "semphr.h"
#include "rowley_locks.h"

/* from the Crossworks manual:

The CrossWorks C library ships as standard with callouts to functions that provide 
thread-safety in a multithreaded application. If your application has a single 
thread of execution, the default implementation of these functions does nothing 
and your application will run without modification.

If your application is intended for a multithreaded environment and you wish 
to use the CrossWorks C library, you must implement the following locking functions:

    __heap_lock and __heap_unlock to provide thread-safety for all heap operations 
    such as malloc, free, and realloc.
    __printf_lock and __printf_unlock to provide thread-safety for printf and relatives.
    __scanf_lock and __scanf_unlock to provide thread-safety for scanf and relatives.
    __debug_io_lock and __debug_io_unlock to provide thread-safety for semi-hosting 
    support in the CrossStudio I/O function.

If you create a CTL project using the New Project wizard, CrossWorks provides 
implementations of these using CTL event sets. You're free to reimplement them 
as you see fit.

If you use a third-party RTOS with the CrossWorks C library, you will need to use 
whatever your RTOS provides for mutual exclusion, typically a semaphore, a mutex, 
or an event set.

*/
#ifdef Rowley_Crossworks

static xSemaphoreHandle s_heap_lockSem     = NULL;
static xSemaphoreHandle s_printf_lockSem   = NULL;
static xSemaphoreHandle s_scanf_lockSem    = NULL;
static xSemaphoreHandle s_debug_io_lockSem = NULL;


void initRowleyLocks(void)
{
    vSemaphoreCreateBinary( s_heap_lockSem );
    vSemaphoreCreateBinary( s_printf_lockSem );
    vSemaphoreCreateBinary( s_scanf_lockSem );
    vSemaphoreCreateBinary( s_debug_io_lockSem );
}

void __heap_lock(void)
{
    // with FreeRTOS, malloc is called (and therefore heap_lock) when the 
    // semaphr is trying to be created.  But it doesn't exist yet, so things go bad
    // if the heap_sem is stiull null we assume this is the init call
    if (NULL != s_heap_lockSem)
    {
        (void) xSemaphoreTake(s_heap_lockSem, portMAX_DELAY);
    }
}

void __heap_unlock(void)
{
    if (NULL != s_heap_lockSem)
    {
        (void) xSemaphoreGive(s_heap_lockSem);
    }
}

void __printf_lock(void)
{
    (void) xSemaphoreTake(s_printf_lockSem, portMAX_DELAY);
}

void __printf_unlock(void)
{
    (void) xSemaphoreGive(s_printf_lockSem);
}

void __scanf_lock(void)
{
    (void) xSemaphoreTake(s_scanf_lockSem, portMAX_DELAY);
}

void __scanf_unlock(void)
{
    (void) xSemaphoreGive(s_scanf_lockSem);
}

void __debug_io_lock(void)
{
    (void) xSemaphoreTake(s_debug_io_lockSem, portMAX_DELAY);
}

void __debug_io_unlock(void)
{
    (void) xSemaphoreGive(s_debug_io_lockSem);
}

#endif

