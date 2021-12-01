/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : The entry point to the Host Interface.
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcfLiteHostInterface.h"
#include "Configuration/IOcfLiteConfigure.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

bool OcfLiteHostInterfaceInitialize(void)
{
    return ::OcfLiteConfigureInitialize();
}
