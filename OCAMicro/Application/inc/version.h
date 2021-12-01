/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

//============================================================================
//  File Name: version.h
//
//  Description:
//      This header file contains the firmware version and prototype for
//      initializing the version field to report the version to various
//      interfaces.
//
// Modification History:
//     $Id:  $
//     $Log:  $
//
//
//
//============================================================================
#ifndef VERSION_H
#define VERSION_H

// IMPORTANT NOTE:
// this file is included in STM32_Startup.s, which is an assembly file
// do not include anything other than defines as the assembler will "puke".

#define VERSION_MAJOR (99)
#define VERSION_MINOR (01)

#endif

