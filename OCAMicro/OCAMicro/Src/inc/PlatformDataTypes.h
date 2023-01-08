/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OCA specific data types.
 *
 */

#ifndef PLATFORMDATATYPES_H
#define PLATFORMDATATYPES_H

/* Generic definitions according to the platform used */
#include <string.h>    // memset
#include <stdio.h>

#define DLL_EXPORT
#define DLL_IMPORTf

#if defined(__cplusplus)
#include <string>
#include <list>
#include <map>
#include <set>
#include <vector>

using namespace std;
extern "C" {
#endif

/*---------------------------------------------------------------*/
/* Global, platform independent definitions                      */
/*---------------------------------------------------------------*/

#if !defined (TARGET_WINDOWS)
/** A 4 byte unsigned integer */
typedef unsigned int        UINT32;
/** A 4 byte signed integer */
typedef signed int          INT32;
#endif
#ifndef __cplusplus
/** Boolean for C */
enum _boolean
{
    false   = 0,
    true    = 1
};
typedef enum _boolean       bool;
#endif
/** A 1 byte unsigned integer */
typedef unsigned char       UINT8;
/** A 1 byte signed integer */
typedef signed char         INT8;
/** A 2 byte unsigned integer */
typedef unsigned short      UINT16;
/** A 2 byte signed integer */
typedef signed short        INT16;
/** A 8 byte unsigned integer */
typedef unsigned long long  UINT64;
/** A 8 byte signed integer */
typedef signed long long    INT64;
/** 8 byte IEEE754 double */
typedef double              DOUBLE;
/** 4 byte IEEE754 float */
typedef float               FLOAT;

#if defined(__cplusplus)
}
#endif

/* Use the compiler to make sure the basic data types are of the correct size */
struct CAP_TypeSizeChecks
{
    char assert00[(sizeof(UINT8)  == 1) ? 1 : -1];
    char assert01[(sizeof(INT8)   == 1) ? 1 : -1];
    char assert02[(sizeof(UINT16) == 2) ? 1 : -1];
    char assert03[(sizeof(INT16)  == 2) ? 1 : -1];
    char assert04[(sizeof(UINT32) == 4) ? 1 : -1];
    char assert05[(sizeof(INT32)  == 4) ? 1 : -1];
    char assert06[(sizeof(UINT64) == 8) ? 1 : -1];
    char assert07[(sizeof(INT64)  == 8) ? 1 : -1];
    char assert08[(sizeof(DOUBLE) == 8) ? 1 : -1];
    char assert09[(sizeof(FLOAT)  == 4) ? 1 : -1];
    char assert10[(sizeof(void*)  >= 2) ? 1 : -1];
};

#endif /* PLATFORMDATATYPES_H */
