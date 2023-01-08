/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : Test suite
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "TestContext.h"

// ---- Helper types and constants ----
/** The default technical verison to use */
#define DEFAULT_TECHNICAL_VERSION 0x0104
// ---- Helper functions ----

// ---- Local data ----
::TestContext* TestContext::m_pSingleton(NULL);

// ---- Function Implementation ----
TestContext::TestContext()
    : m_deviceName(),
    m_testVersion(DEFAULT_TECHNICAL_VERSION),
    m_supportedProtocols(static_cast<TestContext::SupportedProtocols>(OCA_TCP | OCA_UDP | OCA_TCP_SEC)),
    m_supportedDeviceTypes(SupportedDeviceTypes::ALL),
    m_ocp1TcpNetworkObjectNumber(OCA_INVALID_ONO),
#ifdef HAS_SECURE_STACK
    m_ocp1SecureTcpNetworkObjectNumber(OCA_INVALID_ONO),
#endif
    m_ocp1UdpNetworkObjectNumber(OCA_INVALID_ONO)
{

}

TestContext::~TestContext()
{

}

::TestContext& TestContext::GetInstance()
{
    if (NULL == TestContext::m_pSingleton)
    {
        TestContext::m_pSingleton = new TestContext();
    }

    return *TestContext::m_pSingleton;
}

void TestContext::FreeInstance()
{
    if (TestContext::m_pSingleton != NULL)
    {
        delete m_pSingleton;
        m_pSingleton = NULL;
    }
}

void TestContext::SetTestDeviceName(const std::string& deviceName)
{
    m_deviceName = deviceName;
}

void TestContext::SetTestVersion(int testVersion)
{
    m_testVersion = testVersion;
}

void TestContext::SetSupportedProtocols(SupportedProtocols supportedProtocols)
{
    m_supportedProtocols = supportedProtocols;
}

void TestContext::SetSupportedDeviceTypes(SupportedDeviceTypes supportedDeviceTypes)
{
    m_supportedDeviceTypes = supportedDeviceTypes;
}

void TestContext::SetOcp1TcpNetworkObjectNumber(::OcaONo ocp1NetworkObjectNumber)
{
    m_ocp1TcpNetworkObjectNumber = ocp1NetworkObjectNumber;
}

#ifdef HAS_SECURE_STACK
void TestContext::SetOcp1SecureTcpNetworkObjectNumber(::OcaONo ocp1NetworkObjectNumber)
{
    m_ocp1SecureTcpNetworkObjectNumber = ocp1NetworkObjectNumber;
}
#endif

void TestContext::SetOcp1UdpNetworkObjectNumber(::OcaONo ocp1NetworkObjectNumber)
{
    m_ocp1UdpNetworkObjectNumber = ocp1NetworkObjectNumber;
}
