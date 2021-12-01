/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Cortex-M4 Processor Exceptions Handlers 
 *
 */

// ---- Include system wide include files ----
//#include <CMSIS/RTOS_1.02/cmsis_os.h> 

// ---- Include local include files ----

//----- Definitions ----
#ifdef _WIN32
#define INTERRUPT
#else
#define INTERRUPT __attribute__ ((interrupt))
#endif

//----- Type declaration ----

//----- Global variables ----

//----- Local variables ----


#ifdef __cplusplus
extern "C" {
#endif

//----- Local Function declarations ----

//****************************************************************************


#ifdef __cplusplus
}
#endif
