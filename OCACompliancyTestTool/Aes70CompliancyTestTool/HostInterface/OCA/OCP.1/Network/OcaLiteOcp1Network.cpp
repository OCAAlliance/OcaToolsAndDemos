/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
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

// ---- Include local include files ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----
static bool m_bInitialized = false;

// ---- Class Implementation ----

bool Ocp1LiteNetworkInitialize()
{
    if (!m_bInitialized)
    {
        int iResult;
        WSADATA wsaData;
        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) 
        {
            OCA_LOG_ERROR_PARAMS("WSAStartup failed: %d", iResult);
        }
        else
        {
            m_bInitialized = true;
        }
    }

    return true;
}

bool Ocp1LiteNetworkGetMACAddress(UINT8* pBuffer, UINT8 bufferSize)
{
    pBuffer[0] = 0xDE;
    pBuffer[1] = 0xAD;
    pBuffer[2] = 0xBE;
    pBuffer[3] = 0xEE;
    pBuffer[4] = 0xEE;
    pBuffer[5] = 0xEF;
    return true;
}

bool Ocp1LiteNetworkGetNetworkConfiguration(IPParametersType& ipParametersType, std::string& ipAddress, UINT8& subnetMaskLength,
                                            std::string& defaultGateway, std::string& dnsServer, std::string& domainName, UINT64& networkSpeed, bool& isUp)
{
    bool success(false);

    if (m_bInitialized)
    {
        ipParametersType = IPParametersType::NETWORK_CONFIGURATION_DHCP;
        ipAddress.assign("1.2.3.4");
        subnetMaskLength = 10;
        defaultGateway.assign("2.3.4.5");
        dnsServer.assign("8.8.8.8");
        domainName.assign("local");
        networkSpeed = 1000;
        isUp = true;
        success = true;
    }
    return success;
}

