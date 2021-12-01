/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Test Device Service Discovery
 */

// ---- Include system wide include files ----
#include <DNSSDResolver.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlDataTypes/OcaLiteBlockMember.h>
#include <OCC/ControlDataTypes/OcaLiteManagerDescriptor.h>
#include <OCP.1/Ocp1LiteConnectParameters.h>
#include <Proxy/GeneralProxy.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----

// ---- Helper types and constants ----
/** The number of keepalives to test */
#define EXPECTED_NUMBER_OF_KEEPALIVES 4
/** The start of the defined dynamic port range */
#define START_DYNAMIC_PORT_RANGE 49152
/** The end of the defined dynamic port range */
#define END_DYNAMIC_PORT_RANGE 65535

// ---- Helper functions ----

// ---- Local data ----
#include "DeviceServiceDiscoveryTest.h"

// ---- Function Implementation ----

DeviceServiceDiscoveryTest::DeviceServiceDiscoveryTest() :
    BaseTestClass("OCA Service Discovery")
{

}

bool DeviceServiceDiscoveryTest::ExecuteTest()
{
    bool bTCPSuccesfull(true);
    bool bUDPSuccesfull(true);

    bool bTCPInsecure(false);
    if (GetContext().GetSupportedProtocols() & ::TestContext::OCA_TCP)
    {
        AddTestLog("Start TCP (insecure) device resolving");

        ::DNSSDResolver insecureTCPResolver(GetContext().GetTestDeviceName(),
                                            OCA_TCP_SERVICE,
                                            RESOLVE_TIMEOUT);

        ::ResolveResult insecureResolveResult;
        if (insecureTCPResolver.StartResolving(insecureResolveResult))
        {
            AddTestLog("Resolved device on insecure TCP service.");

            bTCPInsecure = true;

            bTCPSuccesfull = TestResolveResult(insecureResolveResult);

            bTCPSuccesfull = TestBasicConnection(insecureResolveResult, OCA_TCP_SERVICE) & bTCPSuccesfull;
        }
        else
        {
            AddTestLog("Not able to resolve device on insecure TCP service.");
        }
    }

    bool bTCPSecure(false);    
    if (GetContext().GetSupportedProtocols() & ::TestContext::OCA_TCP_SEC)
    {
        AddTestLog("Start TCP (secure) device resolving");

        ::DNSSDResolver secureTCPResolver(GetContext().GetTestDeviceName(),
                                          OCA_SECURE_TCP_SERVICE,
                                          RESOLVE_TIMEOUT);

        ::ResolveResult secureResolveResult;
        if (secureTCPResolver.StartResolving(secureResolveResult))
        {
            AddTestLog("Resolved device on secure TCP service.");

            bTCPSecure = true;
            
            bTCPSuccesfull = TestResolveResult(secureResolveResult) && bTCPSuccesfull;

#ifdef HAS_SECURE_STACK
            bTCPSuccesfull = TestBasicConnection(secureResolveResult, OCA_SECURE_TCP_SERVICE) & bTCPSuccesfull;
#endif
        }
        else
        {
            AddTestLog("Not able to resolve device on secure TCP service.");
        }

    }

    if ((GetContext().GetSupportedProtocols() & ::TestContext::OCA_TCP) ||
        (GetContext().GetSupportedProtocols() & ::TestContext::OCA_TCP_SEC))
    {
        if (bTCPSecure || bTCPInsecure)
        {
            // At least one service is resolved.
            AddTestResult("Found at least one TCP service (insecure=%s, secure=%s)",
                bTCPInsecure ? "found" : "not found", bTCPSecure ? "found" : "not found");

            if (bTCPSuccesfull)
            {
                // For the resolve services all test succeeded
                AddTestResult("Test successfull for TCP ",
                    bTCPInsecure ? "found" : "not found", bTCPSecure ? "found" : "not found");
            }
            else
            {
                // Not all test succeeded for the resolved devices
                AddTestResult("Not all tests succeeded for resolved service(s).");
            }
        }
        else
        {
            // No service resolved within the timeout
            AddTestResult("No TCP service resolved within timeout.");
            bTCPSuccesfull = false;
        }
    }

    if (GetContext().GetSupportedProtocols() & ::TestContext::OCA_UDP)
    {
        bool bUDP(false);
        AddTestLog("Start UDP device resolving");

        ::DNSSDResolver UDPResolver(GetContext().GetTestDeviceName(),
                                    OCA_UDP_SERVICE,
                                    RESOLVE_TIMEOUT);

        ::ResolveResult resolveResult;
        if (UDPResolver.StartResolving(resolveResult))
        {
            AddTestLog("Resolved device on UDP service.");

            bUDP = true;

            bUDPSuccesfull = TestResolveResult(resolveResult);

            bUDPSuccesfull = TestBasicConnection(resolveResult, OCA_UDP_SERVICE) & bUDPSuccesfull;

            if (bUDP)
            {
                // At least one service is resolved.
                AddTestResult("Found UDP service");

                if (bUDPSuccesfull)
                {
                    // For the resolve services all test succeeded
                    AddTestResult("Test successfull for UDP service");
                }
                else
                {
                    // Not all test succeeded for the resolved devices
                    AddTestResult("Not all tests succeeded for resolved service.");
                }
            }
            else
            {
                // No service resolved within the timeout
                AddTestResult("No UDP service resolved within timeout.");
                bUDPSuccesfull = false;
            }
        }
        else
        {
            AddTestLog("Not able to resolve device on UDP service.");
        }
    }

    SetTestResult(bUDPSuccesfull && bTCPSuccesfull);

    return bUDPSuccesfull && bTCPSuccesfull;
}

bool DeviceServiceDiscoveryTest::TestResolveResult(const ::ResolveResult& resolveResult)
{
    AddTestResult("Testing DNS-SD for resolved service %s", resolveResult.GetFullName().c_str());
    bool bTestResult(true);

    AddTestLog("Testing OCP.1 port range (found port %d), should be between %d-%d", resolveResult.GetPort(), START_DYNAMIC_PORT_RANGE, END_DYNAMIC_PORT_RANGE);
    if ((resolveResult.GetPort() >= START_DYNAMIC_PORT_RANGE) && (resolveResult.GetPort() <= END_DYNAMIC_PORT_RANGE))
    {
        AddTestResult("   Port for service %s validated (port %d)", resolveResult.GetFullName().c_str(), resolveResult.GetPort());
    }
    else
    {
        AddTestResult("   Port %d not within dynamic port range (%d-%d) for service %s",
            resolveResult.GetPort(), START_DYNAMIC_PORT_RANGE, END_DYNAMIC_PORT_RANGE, resolveResult.GetFullName().c_str());

        bTestResult = false;
    }

    AddTestLog("Testing TXT records");
    bool bFoundTxtvers(false);
    bool bFoundProtovers(false);
    UINT8 txtvers(0);
    UINT8 protovers(0);

    std::vector<std::string> txtRecords(resolveResult.GetTxtRecords());
    bTestResult = bTestResult && (txtRecords.size() >= 2);
    AddTestResult("   At least 2 txt records must be present. Result %s", txtRecords.size() >= 2 ? "success" : "failed");

    std::vector<std::string>::iterator iter(txtRecords.begin());
    while (iter != txtRecords.end())
    {
        AddTestLog("Parsing txt record %s", (*iter).c_str());
        if (1 == sscanf_s((*iter).c_str(), "txtvers=%hhd", &txtvers))
        {
            AddTestLog("Found txt version %d", txtvers);
            bFoundTxtvers = true;
        }
        else if (1 == sscanf_s((*iter).c_str(), "protovers=%hhd", &protovers))
        {
            AddTestLog("Found protocol version %d", protovers);
            bFoundProtovers = true;
        }
        ++iter;
    }

    // Currently we are only aware of version 1.
    if (bFoundTxtvers &&
        (txtvers == 1) &&
        bFoundProtovers &&
        (protovers == 1))
    {
        AddTestResult("   Successfully validated TXT records");
    }
    else
    {
        AddTestResult("   Failed to validate TXT records. txt version (present %d / value is %d, expected 1). protocol version (present %d / value is %d, expected 1)",
            bFoundTxtvers, txtvers, bFoundProtovers, protovers);

        bTestResult = false;
    }

    return bTestResult;
}

::OcaLiteStatus DeviceServiceDiscoveryTest::HandleMembers(::OcaLiteList< ::OcaLiteObjectIdentification> members, ::GeneralProxy* pProxy)
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    for (::OcaUint16 i(0); ((i < members.GetCount()) && (OCASTATUS_OK == rc)); i++)
    {
        ::OcaLiteClassID classId(members.GetItem(i).GetClassIdentification().GetClassID());
        if ((classId.GetFieldCount() >= ::OcaLiteBlock::CLASS_ID.GetFieldCount()) &&
            (0 == memcmp(classId.GetFields(),
                         ::OcaLiteBlock::CLASS_ID.GetFields(),
                         ::OcaLiteBlock::CLASS_ID.GetFieldCount() * sizeof(::OcaLiteBlock::CLASS_ID.GetFields()[0]))))
        {
            ::OcaLiteList< ::OcaLiteObjectIdentification> newMembers;
             rc = pProxy->OcaBlock_GetMembers(members.GetItem(i).GetONo(), newMembers);
             if (OCASTATUS_OK == rc)
             {
                 rc = HandleMembers(newMembers, pProxy);
             }
        }
    }

    return rc;
}

bool DeviceServiceDiscoveryTest::TestBasicConnection(const ::ResolveResult& resolveResult, const std::string& serviceName)
{
    AddTestResult("Testing basic connection for resolved service %s", resolveResult.GetFullName().c_str());

    bool bTestResult(true);
    OcaSessionID sessionId(::OcaLiteCommandHandlerController::GetInstance().Connect(::Ocp1LiteConnectParameters(resolveResult.GetHostTarget(),
                                                                                                                resolveResult.GetPort(),
                                                                                                                KEEP_ALIVE_TIMEOUT,
                                                                                                                false),
                                                                                    GetNetworkObjectNumberFromServiceName(serviceName)));

    if (OCA_INVALID_SESSIONID != sessionId)
    {

        ::OcaLiteStatus rc;
        AddTestResult("   Established connection (%d) for service %s", sessionId, resolveResult.GetFullName().c_str());

        // Query the rootblock (return OK)
        ::GeneralProxy* pProxy(new ::GeneralProxy(sessionId, GetNetworkObjectNumberFromServiceName(serviceName)));
        assert(NULL != pProxy);

        ::OcaLiteList< ::OcaLiteBlockMember> membersRecursive;
        rc = pProxy->OcaBlock_GetMembersRecursive(OCA_ROOT_BLOCK_ONO, membersRecursive);
        bool bTestRootBlock((rc == OCASTATUS_OK) && (membersRecursive.GetCount() > 0));
        if (bTestRootBlock)
        {
            AddTestResult("   OcaBlock.GetMembersRecursive returns status %d, nr members %d. Expecting OK. Test %s",
                rc, membersRecursive.GetCount(), bTestRootBlock ? "Passed" : "Failed");

        }
        else
        {
            // GetMembersRecursive failed, this is acceptable if we can query the members ourselves recursive
            ::OcaLiteList< ::OcaLiteObjectIdentification> members;
            rc = pProxy->OcaBlock_GetMembers(OCA_ROOT_BLOCK_ONO, members);
            if (OCASTATUS_OK == rc)
            {
                rc = HandleMembers(members, pProxy);
            }
            
            bTestRootBlock = (rc == OCASTATUS_OK);

            AddTestResult("   OcaBlock.GetMembers returns status %d. Expecting OK. Test %s",
                rc, bTestRootBlock ? "Passed" : "Failed");
        }

        bTestResult = bTestResult && bTestRootBlock;

        // Query device manager . get managers (return OK or NotImplemented)
        ::OcaLiteList< ::OcaLiteManagerDescriptor> managers;
        rc = pProxy->OcaDeviceManager_GetManagers(managers);

        bool bTestDeviceManager((rc == OCASTATUS_OK || rc == OCASTATUS_NOT_IMPLEMENTED) && (managers.GetCount() > 0));
        AddTestResult("   OcaDeviceManager.GetManagers returns status %d, nr managers %d. Expecting OK or NotImplemented. Test %s",
            rc, managers.GetCount() ,bTestDeviceManager ? "Passed" : "Failed");

        bTestResult = bTestResult && bTestDeviceManager;

        delete pProxy;
        static_cast<void>(::OcaLiteCommandHandlerController::GetInstance().Disconnect(sessionId, GetNetworkObjectNumberFromServiceName(serviceName)));
    }
    else
    {
        bTestResult = false;
    }

    return bTestResult;
}
