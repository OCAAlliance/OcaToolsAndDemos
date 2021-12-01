/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : Interface to the platform specific functions.
 */

#ifndef REDUCED_STD_LIB_H
#define REDUCED_STD_LIB_H

/* ---- Include system wide include files ---- */

/* ---- Include local include files ---- */
#include <PlatformDataTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Definitions ---- */

/* ---- Type declaration ---- */

/* ---- Global variables ---- */

/* ---- Function declaration ---- */

/**
 * The ScanIpAddress function.
 *
 * @param[in] buffer     The buffer containing the IP address string.
 * @param[out] numbers   The found address numbers.
 * @return The number of found IP address numbers.
 */
int ScanIpAddress(const char *buffer, int numbers[]);

/**
 * The PrintIpAddress function.
 *
 * @param[out] buffer    The buffer containing the IP address string.
 * @param[in] numbers    The address numbers.
 */
void PrintIpAddress(char *buffer, const UINT8 numbers[]);

/**
 * The PrintMacAsSerialNumber function.
 *
 * @param[out] buffer    The buffer containing the IP address string.
 * @param[in] numbers    The mac address numbers.
 */
void PrintMacAsSerialNumber(char *buffer, const UINT8 numbers[]);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef REDUCED_STD_LIB_H */
