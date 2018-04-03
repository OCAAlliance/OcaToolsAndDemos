/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The network configuration interface of the Host Interface.
 *
 */

#ifndef IOCP1LITENETWORK_H
#define IOCP1LITENETWORK_H

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>

// ---- Include local include files ----
#include "../Ocp1LiteHostInterfaceConstants.h"
#include "../Ocp1LiteHostInterfaceDataTypes.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

/**
 * The IP Parameter types. The way the device
 * gathered it's IP configuration
 */
enum IPParametersType
{
    /**
     * Unknown Ip address type.
     */
    NETWORK_CONFIGURATION_UNKNOWN       = 0,
    /**
     * Link-local automatic IP Address assignment.
     */
    NETWORK_CONFIGURATION_LINK_LOCAL    = 1,
    /**
     * IP Address via the DHCP server.
     */
    NETWORK_CONFIGURATION_DHCP          = 2,
    /**
     * Statically assigned IP Parameters.
     */
    NETWORK_CONFIGURATION_STATIC        = 3,
    MAX_NETWORK_CONFIGURATION
};

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Initialize the network object.
 *
 * @return Indicates whether the network object is correcly loaded.
 */
bool Ocp1LiteNetworkInitialize(void);

/**
 * Gets the MAC address of the device.
 *
 * @param[out]  pBuffer     Pointer to a data buffer to which the
 *                          MAC address of the device will be copied.
 * @param[in]   bufferSize  The size of the buffer; must be at least
 *                          MAC_ADDRESS_SIZE.
 * @return Indicates whether the MAC address was successfully copied
 *         to pBuffer.
 */
bool Ocp1LiteNetworkGetMACAddress(UINT8* pBuffer, UINT8 bufferSize);

/**
 * Gets the network configuration of the device.
 *
 * @param[out] ipParametersType The type on which the IP Parameters are assigned
 * @param[out] ipAddress        IP address of this node.
 * @param[out] subnetMaskLength The configured subnet mask length.
 * @param[out] defaultGateway   The default gateway.
 * @param[out] dnsServer        The address of the DNSServer used.
 * @param[out] domainName       The domain name used.
 * @param[out] networkSpeed     The network speed in bits per second.
 * @param[out] isUp             True if the interface is up, false if not.
 * @return  Indicates whether the configuration was successfully retrieved.
 */
bool Ocp1LiteNetworkGetNetworkConfiguration(IPParametersType& ipParametersType, std::string& ipAddress, UINT8& subnetMaskLength,
                                            std::string& defaultGateway, std::string& dnsServer, std::string& domainName, UINT64& networkSpeed, bool& isUp);


#endif // IOCP1LITENETWORK_H
