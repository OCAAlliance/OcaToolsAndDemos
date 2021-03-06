/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Main entry point for the AES70 Compliancy Test Tool
 *
 */

// ---- Include system wide include files ----
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include <rapidxml/rapidxml.hpp>
using namespace rapidxml;
using namespace std;

#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <HostInterfaceLite/OCA/OCP.1/Ocp1LiteHostInterface.h>

#include <OCC/ControlDataTypes/OcaLiteStringInABlob.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlClasses/Managers/OcaLiteDeviceManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteNetworkManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteSubscriptionManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteFirmwareManager.h>\

#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/OcaLiteCommandHandlerController.h>

#include <OCP.1/Ocp1LiteNetwork.h>
#include <OCP.1/Ocp1LiteUdpNetwork.h>
#include <OCP.1/Ocp1LiteConnectParameters.h>
#include <OCP.1/Ocp1LiteNetworkSystemInterfaceID.h>

#include <StandardLib/StandardLib.h>

#include <DNSSDResolver.h>

// ---- Include local include files ----
#include "TestFramework/TestSuite.h"
#include "TestFramework/TestContext.h"
#include "Tests/DummyTest.h"
#include "VersionDefs.h"

// ---- Helper functions ----
bool StartOCALibrary();
static const char* getCmdLineOption(char** begin, char** end, const std::string& option);
static bool getCmdLineOptionExist(char** begin, char** end, const std::string& option);
extern void Ocp1LiteServiceRun();

// ---- Local data ----
static unsigned int gs_connectionPort = 65000;

// ---- Defines ----
#define NETWORK_OBJECT_NUMBER static_cast< ::OcaONo>(9000)
#define UDP_NETWORK_OBJECT_NUMBER static_cast< ::OcaONo>(9002)

// ---- Class Implementation ----

/**
 * Main entry point
 */
int main(int argc, char * argv[])
{
    if (!::OcfLiteHostInterfaceInitialize())
    {
        printf("[MAIN ] Failed to initialize hosinterface. Contact the OCA alliance for support.\r\n");
    }

    ::OcfLiteLogSetLogLevel(OCA_LOG_LVL_INFO);

    OCA_LOG_INFO("Copyright @ OCA Alliance 2018\r\n");
    OCA_LOG_INFO_PARAMS("Starting AES70 Compliancy Test Tool version %d.%d.%d\r\n\r\n",
        MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION);

    if ((getCmdLineOptionExist(argv, argv + argc, "-h")) || argc == 1)
    {
        OCA_LOG_INFO_PARAMS("[MAIN ] Usage: %s -d <deviceName> -s <protocols> -v <aes70 version> [optional parameters]", argv[0]);
        // Print help
        OCA_LOG_INFO("[MAIN ] Mandatory parameters:\r\n");
        OCA_LOG_INFO("[MAIN ]     -d <device name>\r\n");
        OCA_LOG_INFO("[MAIN ]     -s <list of supported services tcp|tcp-sec|udp, e.g. tcp|udp>\r\n");
        OCA_LOG_INFO("[MAIN ]     -v <Aes70 version, AES70-2015 or AES70-2018>\r\n");
        OCA_LOG_INFO("[MAIN ] \r\n");
        OCA_LOG_INFO("[MAIN ] Optional parameters:\r\n");
        OCA_LOG_INFO("[MAIN ]     -l <loglevel - trace|info|warning|error (default error)\r\n");
        OCA_LOG_INFO("[MAIN ]     -t <device type, streaming,secure> (default general)\r\n");
    }
    else
    {
        // Start program

        // Parse (mandatory) input parameters
        std::string testDeviceName(getCmdLineOption(argv, argv + argc, "-d"));
        if (testDeviceName.empty())
        {
            OCA_LOG_ERROR("[MAIN ] Mandatory parameter device name (-d) not passed. exit\r\n");
            exit(-1);
        }
        else
        {
            ::TestContext::GetInstance().SetTestDeviceName(testDeviceName);
        }

        std::string supportedServices(getCmdLineOption(argv, argv + argc, "-s"));
        if (supportedServices.empty())
        {
            OCA_LOG_ERROR("[MAIN ] Mandatory parameter supported services (-s) not passed. exit\r\n");
            exit(-1);
        }
        else
        {
            ::TestContext::SupportedProtocols supportedProtocols(::TestContext::NONE);
            // Parse the services.
            std::string service;
            std::istringstream serviceStream(supportedServices);
            while (std::getline(serviceStream, service, '|'))
            {
                // Lower case the input
                std::transform(service.begin(), service.end(), service.begin(), ::tolower);
                if ("tcp" == service)
                {
                    supportedProtocols = static_cast< ::TestContext::SupportedProtocols>(::TestContext::OCA_TCP | supportedProtocols);
                    OCA_LOG_INFO("[MAIN ]    Adding supported service TCP");
                }
                else if ("tcp-sec" == service)
                {
                    supportedProtocols = static_cast< ::TestContext::SupportedProtocols>(::TestContext::OCA_TCP_SEC | supportedProtocols);
                    OCA_LOG_INFO("[MAIN ]    Adding supported service TCP (Secure)");
                }
                else if ("udp" == service)
                {
                    supportedProtocols = static_cast< ::TestContext::SupportedProtocols>(::TestContext::OCA_UDP | supportedProtocols);
                    OCA_LOG_INFO("[MAIN ]    Adding supported service UDP");
                }
            }

            ::TestContext::GetInstance().SetSupportedProtocols(supportedProtocols);
        }

        std::string deviceTypes(getCmdLineOption(argv, argv + argc, "-t"));
        if (deviceTypes.empty())
        {
            OCA_LOG_ERROR("[MAIN ] Optional parameter device type (-t) not passed.\r\n");
        }
        else
        {
            ::TestContext::SupportedDeviceTypes supportedDeviceTypes(::TestContext::ALL);
            // Parse the services.
            std::string deviceType;
            std::istringstream deviceTypeStream(deviceTypes);
            while (std::getline(deviceTypeStream, deviceType, '|'))
            {
                // Lower case the input
                std::transform(deviceType.begin(), deviceType.end(), deviceType.begin(), ::tolower);

                if ("secure" == deviceType)
                {
                    supportedDeviceTypes = static_cast< ::TestContext::SupportedDeviceTypes>(::TestContext::OCA_SECURE | supportedDeviceTypes);
                    OCA_LOG_INFO("[MAIN ]    Adding supported device type secure");
                }
                else if ("streaming" == deviceType)
                {
                    supportedDeviceTypes = static_cast< ::TestContext::SupportedDeviceTypes>(::TestContext::OCA_STREAMING | supportedDeviceTypes);
                    OCA_LOG_INFO("[MAIN ]    Adding supported device type streaming");
                }
            }

            ::TestContext::GetInstance().SetSupportedDeviceTypes(supportedDeviceTypes);
        }

        std::string testVersion(getCmdLineOption(argv, argv + argc, "-v"));
        if (testVersion.empty())
        {   
            OCA_LOG_ERROR("[MAIN ] Mandatory parameter Aes70 version (-v) not passed. exit\r\n");
            exit(-1);
        }
        else
        {
            std::transform(testVersion.begin(), testVersion.end(), testVersion.begin(), ::toupper);

            // Parse the version.
            int testVersionInt(::BaseTestClass::GetVersionAsInt(testVersion));
            if (-1 == testVersionInt)
            {
                OCA_LOG_ERROR_PARAMS("[MAIN ] Invalid version %s passed\r\n", testVersion.c_str());
                exit(-1);
            }
            else
            {
                OCA_LOG_INFO_PARAMS("Using test version %s. Technical version %0x", testVersion.c_str(), testVersionInt);
                ::TestContext::GetInstance().SetTestVersion(testVersionInt);
            }
        }
        
        // Parse (optional) input parameters
        bool bStopOnError(false);
        std::string sStopOnError(getCmdLineOption(argv, argv + argc, "-e"));
        if (!sStopOnError.empty())
        {
            // Lower case the input
            std::transform(sStopOnError.begin(), sStopOnError.end(), sStopOnError.begin(), ::tolower);

            if (sStopOnError == "true") { bStopOnError = true;  OCA_LOG_INFO("[MAIN ]    stop on error -> true"); }
            else if (sStopOnError == "false") { bStopOnError = false;  OCA_LOG_INFO("[MAIN ]    stop on error -> false"); }
            else
            {
                OCA_LOG_ERROR_PARAMS("[MAIN ] Unknown option (%s) passed for stop on error. Expecting true|false\r\n", sStopOnError.c_str());
                exit(-1);
            }
        }

        UINT8 logLevel(OCA_LOG_LVL_ERROR);
        std::string sLogLevel(getCmdLineOption(argv, argv + argc, "-l"));
        if (!sLogLevel.empty())
        {
            // Lower case the input
            std::transform(sLogLevel.begin(), sLogLevel.end(), sLogLevel.begin(), ::tolower);

            if (sLogLevel == "trace") { logLevel = OCA_LOG_LVL_TRACE; OCA_LOG_INFO("[MAIN ]    loglevel -> trace"); }
            else if (sLogLevel == "info") { logLevel = OCA_LOG_LVL_INFO; OCA_LOG_INFO("[MAIN ]    loglevel -> info"); }
            else if (sLogLevel == "warning") { logLevel = OCA_LOG_LVL_WARNING;  OCA_LOG_INFO("[MAIN ]    loglevel -> warning"); }
            else if (sLogLevel == "error") { logLevel = OCA_LOG_LVL_ERROR; OCA_LOG_INFO("[MAIN ]    loglevel -> error"); }
            else
            {
                OCA_LOG_ERROR_PARAMS("[MAIN ] Unknown option (%s) passed for loglevel. Expecting trace|info|warning|error\r\n", sLogLevel.c_str());
                exit(-1);
            }
        }
        ::OcfLiteLogSetLogLevel(logLevel);

        OCA_LOG_TRACE_PARAMS("[MAIN ] Validating bonjour..");
        // Verify preconditions
        bool bContinue(::DNSSDResolver::bIsBonjourInstalled());
        if (!bContinue)
        {
            OCA_LOG_ERROR("[MAIN ] Bonjour not installed or running. exit\r\n");
            exit(-1);
        }
        else
        {
            OCA_LOG_TRACE_PARAMS("[MAIN ]      ..bonjour installed!");
        }
        
        // Load the test suite
        OCA_LOG_TRACE_PARAMS("[MAIN ] Loading testsuite...");
        bContinue = bContinue && TestSuite::GetInstance().Initialize();
        if (!bContinue)
        {
            OCA_LOG_ERROR("[MAIN ] Unable to load testsuite. exit\r\n");
            exit(-1);
        }
        else
        {
            OCA_LOG_TRACE_PARAMS("[MAIN ]      ..testsuite loaded!");
        }

        // Start the OCA library
        OCA_LOG_TRACE_PARAMS("[MAIN ] Starting OCA library...");
        bContinue = bContinue && StartOCALibrary();
        if (!bContinue)
        {
            OCA_LOG_ERROR("[MAIN ] Unable to start OCA library. exit\r\n");
            exit(-1);
        }
        else
        {
            OCA_LOG_TRACE_PARAMS("[MAIN ]       ..OCA library started!");

            // Run a bit to startup the application.
            for (int i(0); i < 100; i++)
            {
                ::OcaLiteCommandHandler::GetInstance().RunWithTimeout(10);
                ::Ocp1LiteServiceRun();
            }
        }
        // Execute the tests
        OCA_LOG_INFO("[MAIN ] Start executing tests");
        if (::TestSuite::GetInstance().ExecuteTests(bStopOnError))
        {
            OCA_LOG_INFO("[MAIN ]       ..Tests executed");
        }
        else
        {
            OCA_LOG_ERROR("[MAIN ] Failed to execute tests. Inspect logs for possible errors");
        }

        // Teardown
        bool bTestResult(::TestSuite::GetInstance().Teardown());
        ::TestSuite::FreeInstance();

        return bTestResult ? 0 : -1;
    }
}

/** 
 * Startup the OCA Layer.
 *
 * @return True iff successfull
 */
bool StartOCALibrary()
{
	// Initialize the host interface (OcfHostInterface already initialized)
    bool bSuccess(::Ocp1LiteHostInterfaceInitialize());

	// Initialize Oca Device
	static_cast<void>(::OcaLiteBlock::GetRootBlock());
	bSuccess = bSuccess && static_cast<bool>(::OcaLiteNetworkManager::GetInstance().Initialize());
	bSuccess = bSuccess && static_cast<bool>(::OcaLiteSubscriptionManager::GetInstance().SetNrEvents(1/*OCA_NR_EVENTS*/));
	bSuccess = bSuccess && static_cast<bool>(::OcaLiteSubscriptionManager::GetInstance().Initialize());
	bSuccess = bSuccess && static_cast<bool>(::OcaLiteDeviceManager::GetInstance().Initialize());
	bSuccess = bSuccess && static_cast<bool>(::OcaLiteFirmwareManager::GetInstance().Initialize());

    if (bSuccess)
    {
        Ocp1LiteNetworkSystemInterfaceID interfaceId(::Ocp1LiteNetworkSystemInterfaceID(static_cast< ::OcaUint32>(0)));
        std::vector<std::string> txtRecords;
        txtRecords.push_back("deviceType=Aes70CompliancyTestTool");

        ::OcaLiteString nodeId(::OcaLiteString("Aes70CompliancyTestTool@" + OcfLiteConfigureGetDeviceName()));

        if (bSuccess)
        { 
            ::Ocp1LiteNetwork* ocp1Network(new ::Ocp1LiteNetwork(NETWORK_OBJECT_NUMBER,
                                                                 static_cast< ::OcaBoolean>(true),
                                                                 ::OcaLiteString("Ocp1LiteNetwork"),
                                                                 ::Ocp1LiteNetworkNodeID(nodeId),
                                                                 interfaceId,
                                                                 txtRecords,
                                                                 ::OcaLiteString("local"),
                                                                 (OcaUint16)gs_connectionPort));
            if (ocp1Network->Initialize())
            {
                if (::OcaLiteBlock::GetRootBlock().AddObject(*ocp1Network))
                {
                    // Store the object number in the test context.
                    ::TestContext::GetInstance().SetOcp1TcpNetworkObjectNumber(ocp1Network->GetObjectNumber());

                    OcaLiteStatus rc(ocp1Network->Startup());
                    if (OCASTATUS_OK != rc)
                    {
                        OCA_LOG_ERROR_PARAMS("[OCA  ] Failed to startup Ocp1Network. Status %d", rc);
                    }
                }
                else
                {
                    OCA_LOG_ERROR("[OCA  ] Failed to add Ocp1Network to the rootBlock");
                    bSuccess = false;
                }
            }
            else
            {
                OCA_LOG_ERROR("[OCA  ] Failed to initialize Ocp1Network");
                bSuccess = false;
            }
        }

        if (bSuccess)
        {
            ::Ocp1LiteUdpNetwork* ocp1Network(new ::Ocp1LiteUdpNetwork(UDP_NETWORK_OBJECT_NUMBER,
                                                                       static_cast< ::OcaBoolean>(true),
                                                                       ::OcaLiteString("Ocp1LiteUdpNetwork"),
                                                                       ::Ocp1LiteNetworkNodeID(nodeId),
                                                                       interfaceId,
                                                                       txtRecords,
                                                                       ::OcaLiteString("local"),
                                                                       (OcaUint16)gs_connectionPort));
            if (ocp1Network->Initialize())
            {
                if (::OcaLiteBlock::GetRootBlock().AddObject(*ocp1Network))
                {
                    // Store the object number in the test context.
                    ::TestContext::GetInstance().SetOcp1UdpNetworkObjectNumber(ocp1Network->GetObjectNumber());

                    OcaLiteStatus rc(ocp1Network->Startup());
                    if (OCASTATUS_OK != rc)
                    {
                        OCA_LOG_ERROR_PARAMS("[OCA  ] Failed to startup Ocp1Network. Status %d", rc);
                    }
                }
                else
                {
                    OCA_LOG_ERROR("[OCA  ] Failed to add Ocp1Network to the rootBlock");
                    bSuccess = false;
                }
            }
            else
            {
                OCA_LOG_ERROR("[OCA  ] Failed to initialize Ocp1Network");
                bSuccess = false;
            }
        }


        if (bSuccess)
        {
            bSuccess = bSuccess && ::OcaLiteCommandHandlerController::GetInstance().Initialize();
            ::OcaLiteDeviceManager::GetInstance().SetErrorAndOperationalState(static_cast< ::OcaBoolean>(!bSuccess), ::OcaLiteDeviceManager::OCA_OPSTATE_OPERATIONAL);
            ::OcaLiteDeviceManager::GetInstance().SetEnabled(static_cast< ::OcaBoolean>(bSuccess));
        }
    }
    else
    {
        OCA_LOG_ERROR("[OCA  ] Failed to create OCA layer");
    }

	return bSuccess;
}

/**
 * Get a cmd line option
 *
 * @param[in] begin     Pointer to the first char
 * @param[in] end       Pointer to the last char
 * @param[in] option    The option to find
 *
 * @return empty string if the option is not found, else a pointer to the option
 */
static const char* getCmdLineOption(char** begin, char** end, const std::string& option)
{
    char** itr(std::find(begin, end, option));
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    static std::string error("");
    return error.c_str();
}

/**
 * Get a cmd line option
 *
 * @param[in] begin     Pointer to the first char
 * @param[in] end       Pointer to the last char
 * @param[in] option    The option to find
 *
 * @return empty string if the option is not found, else a pointer to the option
 */
static bool getCmdLineOptionExist(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}
