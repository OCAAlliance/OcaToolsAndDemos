/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

 /*
 *  Description         : The host interface Ocf Configuration object that can be
 *                        used on any OcaLite enabled platform.
 *
 */

// ---- Include system wide include files ----
#include <assert.h>
#include <HostInterfaceLite/OCA/OCF/Configuration/IOcfLiteConfigure.h>
#ifdef _WIN32
#include <Winsock2.h>
#else
#endif

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "../../../../VersionDefs.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- External data ----
static bool gs_bEnabled(true);

// ---- Class Implementation ----
bool OcfLiteConfigureInitialize(void)
{
    return true;
}

bool OcfLiteConfigureGetModelGUID(UINT32& mnfrCode, UINT32& modelCode)
{
    mnfrCode = static_cast<UINT32>(0xFA2EE9/*OCA Alliance*/);
    modelCode = static_cast<UINT32>(0x1);
    
    return true;
}

bool OcfLiteConfigureGetModelDescription(std::string& manufacturer, std::string& name, std::string& version)
{
    static_cast<void>(manufacturer.assign("OCA Alliance"));
    static_cast<void>(name.assign("Compliany Test Tool"));
    static_cast<void>(version.assign("2.0.0"));

    return true;
}

std::string OcfLiteConfigureGetSerialNumber(void)
{
    return "Not Applicable";
}

std::string OcfLiteConfigureGetDeviceName(void)
{
    char hostname[128] = "";
    gethostname(hostname, sizeof(hostname));
    return std::string(hostname);
}

bool OcfLiteConfigureGetEnabled(void)
{
    return gs_bEnabled;
}

bool OcfLiteConfigureSetEnabled(bool enabled)
{
    gs_bEnabled = enabled;

    return true;
}
