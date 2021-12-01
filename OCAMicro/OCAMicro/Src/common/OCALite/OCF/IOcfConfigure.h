/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteRoot
 *
 */

#ifndef IOCFCONFIGURE_H
#define IOCFCONFIGURE_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "../OcfHostInterfaceDataTypes.h"
#include "../OcfHostInterfaceConstants.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Interface for the configuration of the device data that is used in the
 * layer(s) below OCA, but is configured via OCA objects.
 * This device data consists of the PSK(s) for secure TLS connections and device
 * properties (for example message, user inventory code, role).
 * @ingroup OCFHostInterface
 */
class OCAHOSTINTERFACEINTERFACEDECLSPEC IOcfConfigure
{
public:
    /**
     * Destructor.
     */
    virtual ~IOcfConfigure() {}

    // ---- Interface methods ----
    /**
     * Initialize the configuration object by loading all
     * configuration data from persistent storage.
     *
     * @return Indicates whether the configuration data
     *         was successfully read from persistent storage.
     */
    virtual bool Initialize() = 0;

    /**
     * Terminate the configuration object by persistently
     * storing the configuration data and shutting down.
     */
    virtual void Terminate() = 0;

    /**
     * Configure a new PSK. The PSK should not yet exist in the stored PSKs. If it does
     * exists this call will fail.
     *
     * @param[in]  pskIdentity       The pskIdentity of the PSK.
     * @param[in]  key               The new PSK the device must use for the passed pskIdentity.
     * @param[in]  keyLength         Size (in number of bytes) of the new PSK.
     *
     * @return Indicates whether the operation succeeded.
     */
    virtual bool AddPsk(const std::string& pskIdentity, const UINT8* key, UINT16 keyLength) = 0;

    /**
     * Removes a PSK. The passed key should be equal to the key which is stored.
     *
     * @param[in]  pskIdentity       The pskIdentity of the PSK.
     * @param[in]  key               The stored PSK key.
     * @param[in]  keyLength         Size (in number of bytes) of the PSK.
     *
     * @return Indicates whether the operation succeeded.
     */
    virtual bool RemovePsk(const std::string& pskIdentity, const UINT8* key, UINT16 keyLength) = 0;

    /**
     * Get the PSK for the passed pskIdentity.
     * @note The caller is responsible for providing a large enough block of memory. If not the call will fail.
     *
     * @param[in]   pskIdentity The pskIdentity of key which should be gathered.
     * @param[out]  key         The key
     * @param[in]   bufferSize  The size of the passed buffer.
     *
     * @return Size of the written key; if the call does not succeed because either the key is not present
     *              or the buffer isn't big enough the result will be 0.
     */
    virtual UINT16 GetPsk(const std::string& pskIdentity, UINT8* key, UINT16 bufferSize) const = 0;

    /**
     * Gets the model GUID of the device.
     *
     * @param[out] mnfrCode     The 24-bit manufacturer code.
     * @param[out] modelCode    The 32-bit model code.
     *
     * @return True if succeeded, false if not.
     */
    virtual bool GetModelGUID(UINT32& mnfrCode, UINT32& modelCode) const = 0;

    /**
     * Gets an user friendly description of this device model.
     *
     * @param[out] manufacturer     Name of manufacturer.
     * @param[out] name             Name of this model.
     * @param[out] version          Text name for the version of this model, e.g. "1.2.1a"
     */
    virtual bool GetModelDescription(std::string& manufacturer, std::string& name, std::string& version) const = 0;

    /**
     * Gets the serial number of the device.
     *
     * @return The serial number of the device.
     */
    virtual const std::string& GetSerialNumber() const = 0;

    /**
     * Gets the device name of the device.
     *
     * @return The device name of the device.
     */
    virtual const std::string& GetDeviceName() const = 0;

    /**
     * Sets the device name of the device.
     *
     * @param[in]  deviceName  The value to which the device name
     *                          property of the device must be set.
     * @return Indicates whether the device name was successfully set.
     */
    virtual bool SetDeviceName(const std::string& deviceName) = 0;

    /**
     * Gets the role of the device.
     *
     * @return The role of the device.
     */
    virtual const std::string& GetRole() const = 0;

    /**
     * Sets the role of the device.
     *
     * @param[in]  role  The value to which the role
     *                   property of the device must be set.
     * @return Indicates whether the role was successfully set.
     */
    virtual bool SetRole(const std::string& role) = 0;

    /**
     * Gets the user inventory code of the device.
     *
     * @return The user inventory code of the device.
     */
    virtual const std::string& GetUserInventoryCode() const = 0;

    /**
     * Sets the user inventory code of the device.
     *
     * @param[in]  userInventoryCode  The value to which the uic property
     *                                of the device must be set.
     * @return Indicates whether the user inventory code was successfully set.
     */
    virtual bool SetUserInventoryCode(const std::string& userInventoryCode) = 0;

    /**
     * Gets the message of the device.
     *
     * @return The message of the device.
     */
    virtual const std::string& GetMessage() const = 0;

    /**
     * Sets the message of the device.
     *
     * @param[in] message The value to which the message property of
     *                    the device must be set.
     * @return Indicates whether the message was succesfully set.
     */
    virtual bool SetMessage(const std::string& message) = 0;

    /**
     * Gets the enabled flag of the device, which indicates
     * whether the device is enabled.
     *
     * @return The enabled flag of the device.
     */
    virtual bool GetEnabled() const = 0;

    /**
     * Sets the enabled flag of the device.
     *
     * @param[in]  enabled  The value to which the enabled
     *                      property of the device must be set.
     * @return Indicates whether the enabled flag was successfully set.
     */
    virtual bool SetEnabled(bool enabled) = 0;

    /**
     * Gets the reset cause of the device.
     *
     * @return The reset cause of the device.
     */
    virtual ::ResetCause GetResetCause() const = 0;

    /**
     * Sets the reset cause of the device.
     *
     * @param[in]  resetCause  The value to which the reset cause
     *                         property of the device must be set.
     * @return Indicates whether the reset cause was successfully set.
     */
    virtual bool SetResetCause(::ResetCause resetCause) = 0;

    /**
     * Clears the reset cause of the device (i.e. sets it to
     * POWER_ON).
     *
     * @return Indicates whether the reset cause was successfully cleared.
     */
    virtual bool ClearResetCause() = 0;

    /**
     * Gets the security enabled flag of the device.
     *
     * @return The security enabled flag of the device.
     */
    virtual bool GetSecurityEnabled() const = 0;

    /**
     * Sets the security enabled flag of the device.
     *
     * @param[in]   enabled     The value to which the security enabled
     *                          property of the device must be set.
     * @return Indicates whether the security enabled flag was successfully set.
     */
    virtual bool SetSecurityEnabled(bool enabled) = 0;

    /**
     * Reset the settings back to default.
     */
    virtual bool ResetToDefaultSettings() = 0;

protected:
    /**
     * Default constructor. Made protected since this is an interface
     * class which should not be directly created.
     */
    IOcfConfigure() {}

private:
    /**
     * Private copy c'tor to prevent copying.
     */
    IOcfConfigure(const ::IOcfConfigure&);

    /**
     * Private assignment operator to prevent assignment.
     */
    ::IOcfConfigure& operator=(const ::IOcfConfigure&);
};

#endif // IOCFCONFIGURE_H
