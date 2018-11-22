/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Device reset test
 */

// ---- Include system wide include files ----
#include <iostream>
#include <fstream>
#include <vector>
#include <HostInterfaceLite/OCA/OCF/Timer/IOcfLiteTimer.h>
#include <OCALite/Proxy/GeneralProxy.h>
#include <OCALite/OCC/ControlClasses/Managers/OcaLiteNetworkManager.h>
#include <OCALite/OCP.1/Ocp1LiteNetworkAddress.h>
#include <OCALite/OCP.1/Ocp1LiteNetwork.h>
#include <rapidxml/rapidxml.hpp>
using namespace rapidxml;
using namespace std;

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "DeviceResetTest.h"

// ---- Helper types and constants ----
/** Reset key K1 */
static ::OcaUint8 s_resetKeyValue_k1[16] = { 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF };
static ::OcaLiteBlobFixedLen<16> s_resetKey_k1(s_resetKeyValue_k1);
/** Reset key K2 */
static ::OcaUint8 s_resetKeyValue_k2[16] = { 0x11, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0x1F };
static ::OcaLiteBlobFixedLen<16> s_resetKey_k2(s_resetKeyValue_k2);

/** Unicast address to use */
static ::Ocp1LiteNetworkAddress s_unicastAddress(::OcaLiteString("0.0.0.0"), 55655);
/** Multicast address to use */
static ::Ocp1LiteNetworkAddress s_multicastAddress(::OcaLiteString("224.0.0.140"), 55655);
/** The number of MS to wait after a reset succeeded */
static int s_waitAfterReset = 20 * 1000;

/** The timeout to wait for a session lost */
#define WAIT_FOR_SESSION_LOST_TIMEOUT static_cast<int>(KEEP_ALIVE_TIMEOUT * 4.5)

/** XML defines */
#define XML_UNICAST_ADDRESS_NODE "UnicastAddress"
#define XML_MULTICAST_ADDRESS_NODE "MulticastAddress"
#define XML_PORT_NODE "Port"
#define XML_HOST_NODE "Host"
#define XML_WAIT_AFTER_RESET "WaitAfterReset"
// ---- Helper functions ----

// ---- Local data ----


// ---- Function Implementation ----
DeviceResetTest::DeviceResetTest()
    : BaseTestClass("OCP.1 device reset mechanism"),
    m_sessionId(OCA_INVALID_SESSIONID)
{
}

bool DeviceResetTest::PrepareTest()
{
    bool bResult(BaseTestClass::PrepareTest());

    xml_document<> doc;

    ifstream theFile(XML_CONFIGURATION_FILE);
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    doc.parse<0>(&buffer[0]);
    if (NULL != doc.first_node(XML_CONFIGURATION_FILE_ROOT_NODE))
    {
        xml_node<>* deviceResetTestRootNode(doc.first_node(XML_CONFIGURATION_FILE_ROOT_NODE)->first_node(XML_CONFIGURATION_FILE_DEVICERESETTEST));
        if (NULL != deviceResetTestRootNode)
        {
            xml_node<>* unicastAddressNode(deviceResetTestRootNode->first_node(XML_UNICAST_ADDRESS_NODE));
            if (NULL != unicastAddressNode)
            {
                if ((NULL != unicastAddressNode->first_attribute(XML_HOST_NODE)) &&
                    (unicastAddressNode->first_attribute(XML_HOST_NODE)->value_size() > 0) &&
                    (NULL != unicastAddressNode->first_attribute(XML_PORT_NODE)) &&
                    (unicastAddressNode->first_attribute(XML_PORT_NODE)->value_size() > 0))
                {
                    s_unicastAddress = ::Ocp1LiteNetworkAddress(
                        ::OcaLiteString(unicastAddressNode->first_attribute(XML_HOST_NODE)->value()),
                        static_cast< ::OcaUint16>(std::stoi(unicastAddressNode->first_attribute(XML_PORT_NODE)->value())));
                }
            }

            xml_node<>* multicastAddressNode(deviceResetTestRootNode->first_node(XML_MULTICAST_ADDRESS_NODE));
            if (NULL != multicastAddressNode)
            {
                if ((NULL != multicastAddressNode->first_attribute(XML_HOST_NODE)) &&
                    (multicastAddressNode->first_attribute(XML_HOST_NODE)->value_size() > 0) &&
                    (NULL != multicastAddressNode->first_attribute(XML_PORT_NODE)) &&
                    (multicastAddressNode->first_attribute(XML_PORT_NODE)->value_size() > 0))
                {
                    s_multicastAddress = ::Ocp1LiteNetworkAddress(
                        ::OcaLiteString(multicastAddressNode->first_attribute(XML_HOST_NODE)->value()), 
                        static_cast< ::OcaUint16>(std::stoi(multicastAddressNode->first_attribute(XML_PORT_NODE)->value())));
                }
            }

            xml_node<>* waitAfterReset(deviceResetTestRootNode->first_node(XML_WAIT_AFTER_RESET));
            if (NULL != waitAfterReset)
            {
                s_waitAfterReset = std::stoi(waitAfterReset->value());
            }
        }
    }


    AddTestLog("Using unicast device reset address = %s:%d", s_unicastAddress.GetDestHostOrIPAddress().GetString().c_str(), s_unicastAddress.GetPort());
    AddTestLog("Using multicast device reset address = %s:%d", s_multicastAddress.GetDestHostOrIPAddress().GetString().c_str(), s_multicastAddress.GetPort());
    AddTestLog("Using wait time after reset = %d", s_waitAfterReset);
    return bResult;
}

bool DeviceResetTest::ExecuteTest()
{
    bool bTCPSuccess(true);
    bool bUDPSuccess(true);

    if (GetContext().GetSupportedProtocols() & ::TestContext::OCA_TCP)
    {
        bTCPSuccess = false;

        AddTestLog("Setup insecure TCP session to test device");

        m_sessionId = GetSession(GetContext().GetTestDeviceName(), OCA_TCP_SERVICE);
        if (OCA_INVALID_SESSIONID != m_sessionId)
        {
            ::GeneralProxy* pProxy(new ::GeneralProxy(m_sessionId, GetNetworkObjectNumberFromServiceName(OCA_TCP_SERVICE)));
            assert(NULL != pProxy);

            ::OcaLiteStatus status(pProxy->OcaDeviceManager_SetResetKey(s_resetKey_k1, s_unicastAddress));

            delete pProxy;

            if (OCASTATUS_NOT_IMPLEMENTED == status)
            {
                // Not implemented. done testing.
                bTCPSuccess = true;
                AddTestResult("SetResetKey on TCP session returned NotImplemented. Skipping test with Result success.");
            }
            else
            {
                bTCPSuccess = PerformResetKeyTest(OCA_TCP_SERVICE);
                AddTestResult("Reset device functionality test via TCP : %s.", bUDPSuccess ? "Succeeded" : "Failed");
            }
        }
        else
        {
            AddTestResult("Failed to setup insecure TCP session to test device.");
        }
    }

#ifdef HAS_SECURE_STACK
    if (GetContext().GetSupportedProtocols() & ::TestContext::OCA_TCP_SEC)
    {
        bTCPSuccess = false;

        AddTestLog("Setup secure TCP session to test device");

        m_sessionId = GetSession(GetContext().GetTestDeviceName(), OCA_SECURE_TCP_SERVICE);
        if (OCA_INVALID_SESSIONID != m_sessionId)
        {
            ::GeneralProxy* pProxy(new ::GeneralProxy(m_sessionId, GetNetworkObjectNumberFromServiceName(OCA_SECURE_TCP_SERVICE)));
            assert(NULL != pProxy);

            ::OcaLiteStatus status(pProxy->OcaDeviceManager_SetResetKey(s_resetKey_k1, s_unicastAddress));

            delete pProxy;

            if (OCASTATUS_NOT_IMPLEMENTED == status)
            {
                // Not implemented. done testing.
                bTCPSuccess = true;
                AddTestResult("SetResetKey on (secure) TCP session returned NotImplemented. Skipping test with Result success.");
            }
            else
            {
                bTCPSuccess = PerformResetKeyTest(OCA_SECURE_TCP_SERVICE);
                AddTestResult("Reset device functionality test via (secured) TCP : %s.", bUDPSuccess ? "Succeeded" : "Failed");
            }
        }
        else
        {
            AddTestResult("Failed to setup secure TCP session to test device.");
        }
    }
#endif

    if (GetContext().GetSupportedProtocols() & ::TestContext::OCA_UDP)
    {
        bUDPSuccess = false;

        m_sessionId = GetSession(GetContext().GetTestDeviceName(), OCA_UDP_SERVICE);
        if (OCA_INVALID_SESSIONID != m_sessionId)
        {
            ::GeneralProxy* pProxy(new ::GeneralProxy(m_sessionId, GetNetworkObjectNumberFromServiceName(OCA_UDP_SERVICE)));
            assert(NULL != pProxy);

            ::OcaLiteStatus status(pProxy->OcaDeviceManager_SetResetKey(s_resetKey_k1, s_unicastAddress));

            delete pProxy;

            if (OCASTATUS_NOT_IMPLEMENTED == status)
            {
                // Not implemented. done testing.
                bUDPSuccess = true;
                AddTestResult("SetResetKey on UDP session returned NotImplemented. Skipping test with Result success.");
            }
            else
            {
                bUDPSuccess = PerformResetKeyTest(OCA_UDP_SERVICE);
                AddTestResult("Reset device functionality test via UDP : %s.", bUDPSuccess ? "Succeeded" : "Failed");
            }
        }
        else
        {
            AddTestResult("Failed to setup UDP session to test device.");
        }
    }

    bool bSuccess(bUDPSuccess && bTCPSuccess);

    SetTestResult(bSuccess);

    return bSuccess;
}


void DeviceResetTest::OnConnectionLost(::OcaSessionID sessionID)
{
    AddTestLog("DeviceResetTest::OnConnectionLost(%d), waiting for sessionId %d", sessionID, m_sessionId);

    if (m_sessionId == sessionID)
    {
        m_sessionId = OCA_INVALID_SESSIONID;
    }

}

bool DeviceResetTest::SingleResetTest(const std::string& serviceName, bool unicast, bool tryWithWrongKey)
{
    AddTestLog("Execute SingleResetTest. ServiceName? %s, Unicast/Multicast? %s, TryWithWrongKey? %d",
            serviceName.c_str(), unicast ? "unicast" : "multicast", tryWithWrongKey);

    ::OcaLiteStatus status(OCASTATUS_PROCESSING_FAILED);

    // Get the session
    m_sessionId = GetSession(GetContext().GetTestDeviceName(), serviceName);

    AddTestLog("Setup new session ID %d", m_sessionId);
    
    if (OCA_INVALID_SESSIONID != m_sessionId)
    {
        // Create a proxy
        ::GeneralProxy* pProxy(new ::GeneralProxy(m_sessionId, GetNetworkObjectNumberFromServiceName(serviceName)));
        assert(NULL != pProxy);

        AddTestLog("Try to clear reset cause");

        // Try to clear the current reset cause.
        static_cast<void>(pProxy->OcaDeviceManager_ClearResetCause());

        AddTestLog("Set reset key");

        ::Ocp1LiteNetworkAddress unicastAddress(s_unicastAddress);
        if (unicast)
        {
            ::ResolveResult resolveResult;
            ::DNSSDResolver resolver(GetContext().GetTestDeviceName(), OCA_TCP_SERVICE, RESOLVE_TIMEOUT);
            if (resolver.StartResolving(resolveResult))
            {
                unicastAddress = ::Ocp1LiteNetworkAddress(::OcaLiteString(resolveResult.GetHostTarget()), s_unicastAddress.GetPort());
            }
        }

        // Set the reset key
        status = pProxy->OcaDeviceManager_SetResetKey(s_resetKey_k1, unicast ? unicastAddress : s_multicastAddress);
        if (OCASTATUS_OK == status)
        {
            AddTestLog("Reset key set! Now reset the device.");

            ::OcaLiteNetwork* pNetwork(::OcaLiteNetworkManager::GetInstance().GetNetwork(GetNetworkObjectNumberFromServiceName(serviceName)));
            assert(NULL != pNetwork);

            if (tryWithWrongKey)
            {
                for (size_t i = 0; (i < 3) && (OCASTATUS_OK == status); i++)
                {
                    status = pNetwork->SendResetMessage(s_resetKey_k2, unicast ? unicastAddress : s_multicastAddress);
                    AddTestLog("SendResetMessage (with incorrect key) result %d", status);
                }

                if (OCASTATUS_OK == status)
                {
                    AddTestLog("Waiting for NOT session %d lost", m_sessionId);

                    if (!WaitForSessionLost(WAIT_FOR_SESSION_LOST_TIMEOUT))
                    {
                        AddTestLog("Session NOT lost. Test succeeded.");

                        // Session not lost, device not rebooted as expected.
                        assert(status == OCASTATUS_OK);
                    }
                    else
                    {
                        AddTestLog("Session lost. Test failed.");

                        // Detected session lost, should not have been the case since the key is wrong!
                        status = OCASTATUS_DEVICE_ERROR;

                        AddTestLog("Waiting for device to reboot..");
                        UINT32 endTime(::OcfLiteTimerGetTimerTickCount() + s_waitAfterReset);
                        while (endTime > ::OcfLiteTimerGetTimerTickCount())
                        {
                            ::OcaLiteCommandHandlerController::GetInstance().RunWithTimeout(100);
                        }
                        AddTestLog("..done");
                    }
                }
            }

            if (OCASTATUS_OK == status)
            {
                for (size_t i = 0; (i < 3) && (OCASTATUS_OK == status); i++)
                {
                    status = pNetwork->SendResetMessage(s_resetKey_k1, unicast ? unicastAddress : s_multicastAddress);
                    AddTestLog("SendResetMessage result %d", status);
                }

                if (OCASTATUS_OK == status)
                {
                    AddTestLog("Waiting for session %d lost", m_sessionId);


                    if (WaitForSessionLost(WAIT_FOR_SESSION_LOST_TIMEOUT))
                    {
                        AddTestLog("Session lost. Test succeeded.");

                        // Session lost, device probably rebooted as expected.
                        AddTestLog("Waiting for device to reboot..");
                        UINT32 endTime(::OcfLiteTimerGetTimerTickCount() + s_waitAfterReset);
                        while (endTime > ::OcfLiteTimerGetTimerTickCount())
                        {
                            ::OcaLiteCommandHandlerController::GetInstance().RunWithTimeout(100);
                        }
                        AddTestLog("..done");
                    }
                    else
                    {
                        AddTestLog("Session NOT lost. Test failed.");

                        // Not detected session lost
                        status = OCASTATUS_DEVICE_ERROR;
                    }
                }
            }
        }
        else
        {
            // Failed to set reset key
            AddTestLog("Failed to set reset key. Result %d", status);

        }

        delete pProxy;
    }
    else
    {
        AddTestLog("Session NOT created. Test failed.");

        // Failed to setup (new) session
    }

    AddTestResult("   Device reset test status: %s. Tested %s with %s", 
        (status == OCASTATUS_OK) ? "Succeeded" : "Failed",
        unicast ? "unicast reset" : "multicast reset",
        tryWithWrongKey ? "wrong key" : "correct key");

    return (status == OCASTATUS_OK);
}

bool DeviceResetTest::PerformResetKeyTest(const std::string& serviceName)
{
    // Unicast
    bool bSuccess(SingleResetTest(serviceName, true, false));
    // Multicast
    bSuccess = SingleResetTest(serviceName, false, false) && bSuccess;
    // Unicast with wrong key
    bSuccess = SingleResetTest(serviceName, true, true) && bSuccess;
    // Multicast with wrong key
    bSuccess = SingleResetTest(serviceName, false, true) && bSuccess;

    return bSuccess;
}

bool DeviceResetTest::WaitForSessionLost(int timeout)
{
    UINT32 endTime(::OcfLiteTimerGetTimerTickCount() + ((timeout * 1000)/* Convert ms to S*/ + 1000));
    while ((endTime > ::OcfLiteTimerGetTimerTickCount()) &&
           (m_sessionId != OCA_INVALID_SESSIONID))
    {
        ::OcaLiteCommandHandlerController::GetInstance().RunWithTimeout(100);
    }

    return (m_sessionId == OCA_INVALID_SESSIONID);
}

