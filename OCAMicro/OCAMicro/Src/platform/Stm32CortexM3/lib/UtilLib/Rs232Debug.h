/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Module for debugging via Rs232
 *
 */

#ifndef RS232DEBUG_H
#define RS232DEBUG_H


// ---- Include system wide include files ----
#include <PlatformDataTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---- Definitions ----
#define RS232_DEBUG_BUFFER          (120)
#define RS232_DEBUG_WORDS           (RS232_DEBUG_BUFFER / 4) // words of 4 bytes

// ---- Type declaration ----

// ---- Global variables ----

// ---- Function declarations ----

/**
 * Function to initialize RS232 for debugging.
 * Baudrate = 115200, 1 stop bit, none parity, 8 data bits
 */
void Rs232DebugInit(void);

/**
 * Function to write a string to the serial port.
 *
 * @param[in]  formatString   Format string like printf uses.
 * @param[in]  ...            used to pass a variable number of arguments
 */
void Rs232DebugPrint(const char* formatString, ...);

/**
 * Function to write a single character. Usefull if Rs232DebugPrint does 
 * not fit into your flash.
 *
 * @param[in] pString       String to print
 * @param[in] strLen        Length of string
 */
void Rs232DebugPrintString(char *pString, int strLen);

/**
 * Function to write a single character. Usefull if Rs232DebugPrint does 
 * not fit into your flash.
 *
 * @param[in] character     Single character to print
 */
void Rs232DebugPrintChar(char character);

/**
 * Function to put a single character. Usefull to log in ISRs.
 *
 * @param[in] character     Single character to put
 */
void Rs232DebugPutChar(char character);

/**
 * Function to check and, if available, get a single character.
 *
 * @param[in] pCharacter     Single character to return
 * @return 1 if succesful, 0 otherwise.
*/
INT32 Rs232DebugCheckAndGetChar(char* pCharacter);

#ifdef __cplusplus
}
#endif
#endif // RS232DEBUG_H
