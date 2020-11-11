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
#elif __APPLE__
#include <unistd.h>
#endif
// ---- FileInfo Macro ----

// ---- Include local include files ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- External data ----
static bool gs_bEnabled(true);
static UINT32 OcfManufacturerCode = 0x123456;
static UINT32 OcfModelCode		  = 0x0000;
static std::string OcfFriendlyNameManufacturer("Manufacturer");
static std::string OcfFriendlyNameModel("Model Name");
static std::string OcfFriendlyNameVersion("Version string");

// ---- Class Implementation ----
bool OcfLiteConfigureInitialize(void)
{
    return true;
}

bool OcfLiteConfigureGetModelGUID(UINT32& mnfrCode, UINT32& modelCode)
{
    mnfrCode  = OcfManufacturerCode;
    modelCode = OcfModelCode;
    
    return true;
}

bool OcfLiteConfigureGetModelDescription(std::string& manufacturer, std::string& name, std::string& version)
{
	manufacturer.assign(OcfFriendlyNameManufacturer.c_str());
	name.assign(OcfFriendlyNameModel.c_str());
	version.assign(OcfFriendlyNameVersion.c_str());

    return true;
}

std::string OcfLiteConfigureGetSerialNumber(void)
{
    char hostname[128] = "";
    gethostname(hostname, sizeof(hostname));
    return std::string(hostname);
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

bool OcfLiteConfigureSetModelGUID(UINT32 mnfrCode, UINT32 modelCode)
{
    OcfManufacturerCode = mnfrCode;
    OcfModelCode = modelCode;

    return true;
}

bool OcfLiteConfigureSetModelDescription(std::string& manufacturer, std::string& name, std::string& version)
{
	OcfFriendlyNameManufacturer = manufacturer;
	OcfFriendlyNameModel		= name;
	OcfFriendlyNameVersion		= version;

	return true;
}
