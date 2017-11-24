/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

//=========================================================================
//  File Name: rowley_locks.h
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
#ifndef __ROWLEY_LOCKS_H__
#define __ROWLEY_LOCKS_H__

#ifdef Rowley_Crossworks

#ifdef __cplusplus
extern "C" {
#endif

void initRowleyLocks(void);

void __heap_lock(void);

void __heap_unlock(void);

void __printf_lock(void);

void __printf_unlock(void);

void __scanf_lock(void);

void __scanf_unlock(void);

void __debug_io_lock(void);

void __debug_io_unlock(void);

#ifdef __cplusplus
}
#endif

#endif

#endif //  __ROWLEY_LOCKS_H__

