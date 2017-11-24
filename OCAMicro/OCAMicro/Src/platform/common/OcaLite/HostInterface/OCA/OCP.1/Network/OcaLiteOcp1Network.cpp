/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : Small Oca Ocp1Network implementation.
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <HostInterfaceLite/OCA/OCP.1/Network/IOcp1LiteNetwork.h>
#include <HostInterfaceLite/OCA/OCP.1/Ocp1LiteHostInterface.h>
#include <StandardLib/StandardLib.h>
#include <common/OcaLite/LogicalDevices/Switch/Switch.h>
#include <common/OcaLite/HostInterface/OCA/OCP.1/LwIPAdapterOcaLite.h>

// ---- Include local include files ----

// ---- Helper types and constants ----

// ---- Helper functions ----
static std::string CreateIpAddress(UINT32 ipAddress);

// ---- Local data ----
static bool m_bInitialized;

// ---- Class Implementation ----

bool Ocp1LiteNetworkInitialize()
{
    LwIPAdapterOcaLite::GetInstance().SetDhcpMode(true);
    
    m_bInitialized = true;

    return true;
}

bool Ocp1LiteNetworkGetMACAddress(UINT8* pBuffer, UINT8 bufferSize)
{
    ::memcpy(pBuffer, LwIPAdapterOcaLite::GetInstance().GetMacAddress(), MAC_ADDRESS_SIZE);

    return true;
}

bool Ocp1LiteNetworkGetNetworkConfiguration(IPParametersType& ipParametersType, std::string& ipAddress, UINT8& subnetMaskLength,
                                            std::string& defaultGateway, std::string& dnsServer, std::string& domainName, UINT64& networkSpeed, bool& isUp)
{
    bool success(false);

    if (m_bInitialized)
    {
        UINT32 netMask = LwIPAdapterOcaLite::GetInstance().GetNetMask();

//        ipParametersType = IPParametersType::NETWORK_CONFIGURATION_UNKNOWN;
        ipParametersType = NETWORK_CONFIGURATION_UNKNOWN;
        static_cast<void>(ipAddress.assign(CreateIpAddress(ConvertFromNetworkOrderToHostByteOrder(LwIPAdapterOcaLite::GetInstance().GetIpAddress()))));
        subnetMaskLength = 0;
        for (UINT32 i(0x80000000); i != 0; i >>= 1)
        {
            if (0 != (netMask & i))
            {
                subnetMaskLength++;
            }
            else
            {
                break;
            }
        }
        static_cast<void>(defaultGateway.assign(CreateIpAddress(ConvertFromNetworkOrderToHostByteOrder(LwIPAdapterOcaLite::GetInstance().GetDefaultGateway()))));
        static_cast<void>(dnsServer.assign("0:0:0:0"));
        static_cast<void>(domainName.assign("local"));
        networkSpeed = Switch::GetInstance().GetLinkSpeed();
        isUp = ::Switch::GetInstance().GetLinkUp();

        success = true;
    }
    return success;
}

/**
 * Create IP address string from 4-byte
 *
 * @param[in] ipAddress  4-byte IP address
 *
 * @return IP address string
 */
std::string CreateIpAddress(UINT32 ipAddress)
{
    char cPtr[17/*Maximum size xxx.xxx.xxx.xxx\0*/];
    UINT8 numbers[sizeof(UINT32)] = {0};

    numbers[0] = static_cast<UINT8>((ipAddress >> 24) & 0xFF);
    numbers[1] = static_cast<UINT8>((ipAddress >> 16) & 0xFF);
    numbers[2] = static_cast<UINT8>((ipAddress >> 8) & 0xFF);
    numbers[3] = static_cast<UINT8>(ipAddress & 0xFF);

    ::PrintIpAddress(cPtr, numbers);
    
    return std::string(cPtr);
}
