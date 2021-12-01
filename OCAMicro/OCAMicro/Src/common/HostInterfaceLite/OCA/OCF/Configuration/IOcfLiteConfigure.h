/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The device configuration interface of the Host Interface.
 *
 */

#ifndef IOCFLITECONFIGURE_H
#define IOCFLITECONFIGURE_H

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
    
/**
 * Initialize the configuration object by loading all
 * configuration data from persistent storage.
 *
 * @return Indicates whether the configuration data
 *         was successfully read from persistent storage.
 */
bool OcfLiteConfigureInitialize(void);

/**
 * Gets the model GUID of the device.
 *
 * @param[out] mnfrCode     The 24-bit manufacturer code.
 * @param[out] modelCode    The 32-bit model code.
 *
 * @return True if succeeded, false if not.
 */
bool OcfLiteConfigureGetModelGUID(UINT32& mnfrCode, UINT32& modelCode);

/**
 * Gets an user friendly description of this device model.
 *
 * @param[out] manufacturer     Name of manufacturer.
 * @param[out] name             Name of this model.
 * @param[out] version          Text name for the version of this model, e.g. "1.2.1a"
 */
bool OcfLiteConfigureGetModelDescription(std::string& manufacturer, std::string& name, std::string& version);

/**
 * Gets the serial number of the device.
 *
 * @return The serial number of the device.
 */
std::string OcfLiteConfigureGetSerialNumber(void);

/**
 * Gets the device name of the device.
 *
 * @return The device name of the device.
 */
std::string OcfLiteConfigureGetDeviceName(void);

/**
 * Gets the enabled flag of the device, which indicates
 * whether the device is enabled.
 *
 * @return The enabled flag of the device.
 */
bool OcfLiteConfigureGetEnabled(void);

/**
 * Sets the enabled flag of the device.
 *
 * @param[in]  enabled  The value to which the enabled
 *                      property of the device must be set.
 * @return Indicates whether the enabled flag was successfully set.
 */
bool OcfLiteConfigureSetEnabled(bool enabled);

/**
 * Sets the model GUID of the device.  Should only be done at initialization
 *
 * @param[out] mnfrCode     The 24-bit manufacturer code.
 * @param[out] modelCode    The 32-bit model code.
 *
 * @return True if succeeded, false if not.
 */
bool OcfLiteConfigureSetModelGUID(UINT32 mnfrCode, UINT32 modelCode);

/**
 * Sets a user friendly description of this device model.
 *
 * @param[out] manufacturer     Name of manufacturer.
 * @param[out] name             Name of this model.
 * @param[out] version          Text name for the version of this model, e.g. "1.2.1a"
 */
bool OcfLiteConfigureSetModelDescription(std::string& manufacturer, std::string& name, std::string& version);

/**
 * Sets the Serial Number string of the device.
 *
 * @param[in] serial            The serial string.
 */
bool OcfLiteConfigureSetSerialNumber(std::string& serial);

/**
 * Sets the Device name string.
 *
 * @param[in] name                The device name.
 */
bool OcfLiteConfigureSetDeviceName(std::string& name);

#endif // IOCFLITECONFIGURE_H
