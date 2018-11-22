/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Test suite
 */

// ---- Include system wide include files ----
#include <OCALite/OCP.1/Ocp1LiteConnectParameters.h>
#include <HostInterfaceLite/OCA/OCF/Timer/IOcfLiteTimer.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----

// ---- Helper types and constants ----
/** The number of keepalives to test */
#define EXPECTED_NUMBER_OF_KEEPALIVES 4

// ---- Helper functions ----

// ---- Local data ----
#include "KeepAliveTest.h"

// ---- Function Implementation ----
KeepAliveTest::KeepAliveTest()
    : BaseTestClass("OCP.1 KeepAlive mechanism"),
    m_sessionID(OCA_INVALID_SESSIONID),
    m_receivedKeepaliveCount(0),
    m_keepaliveTimestamps()
{

}

bool KeepAliveTest::PrepareTest()
{
    bool bSuccess(BaseTestClass::PrepareTest());
    bSuccess = bSuccess && ::OcaLiteCommandHandlerController::GetInstance().RegisterKeepAliveReceivedEventHandler(this);
    return bSuccess;
}

bool KeepAliveTest::ExecuteTest()
{
    bool bTestExecuted(true);
    bool bTestResult(true);

    m_receivedKeepaliveCount = 0;
    m_keepaliveTimestamps.clear();
    if (GetContext().GetSupportedProtocols() & ::TestContext::OCA_TCP)
    {
        AddTestLog("Verify keep-alive mechanism for OCP.1 TCP");

        m_sessionID = GetSession(GetContext().GetTestDeviceName(),
                                 OCA_TCP_SERVICE,
                                 KEEP_ALIVE_TIMEOUT,
                                 3,
                                 RESOLVE_TIMEOUT);

        if (OCA_INVALID_SESSIONID != m_sessionID)
        {
            UINT32 timeoutTime(::OcfLiteTimerGetTimerTickCount() + (((KEEP_ALIVE_TIMEOUT * 1000) * EXPECTED_NUMBER_OF_KEEPALIVES) * 2));
            while ((m_receivedKeepaliveCount < EXPECTED_NUMBER_OF_KEEPALIVES) &&
                    ::OcfLiteTimerGetTimerTickCount() < timeoutTime)
            {
                ::OcaLiteCommandHandlerController::GetInstance().RunWithTimeout(14);
            }

            if (m_receivedKeepaliveCount < EXPECTED_NUMBER_OF_KEEPALIVES)
            {
                // Timeout
                bTestExecuted = false;
            }
            else
            {
                bTestResult = VerifyKeepalives(OCA_TCP_SERVICE);
            }
        }
        else
        {
            AddTestLog("Failed to resolve test device");

            bTestExecuted = false;
        }

        ::OcaLiteCommandHandlerController::GetInstance().Disconnect(m_sessionID, GetNetworkObjectNumberFromServiceName(OCA_TCP_SERVICE));
        m_sessionID = OCA_INVALID_SESSIONID;
    }

#ifdef HAS_SECURE_STACK
    m_receivedKeepaliveCount = 0;
    m_keepaliveTimestamps.clear();
    if (GetContext().GetSupportedProtocols() & ::TestContext::OCA_TCP_SEC)
    {
        AddTestLog("Verify keep-alive mechanism for OCP.1 (secured) TCP");

        m_sessionID = GetSession(GetContext().GetTestDeviceName(),
                                 OCA_SECURE_TCP_SERVICE,
                                 KEEP_ALIVE_TIMEOUT,
                                 3,
                                 RESOLVE_TIMEOUT);

        if (OCA_INVALID_SESSIONID != m_sessionID)
        {
            UINT32 timeoutTime(::OcfLiteTimerGetTimerTickCount() + (((KEEP_ALIVE_TIMEOUT * 1000) * EXPECTED_NUMBER_OF_KEEPALIVES) * 2));
            while ((m_receivedKeepaliveCount < EXPECTED_NUMBER_OF_KEEPALIVES) &&
                :  :OcfLiteTimerGetTimerTickCount() < timeoutTime)
            {
                ::OcaLiteCommandHandlerController::GetInstance().RunWithTimeout(14);
            }

            if (m_receivedKeepaliveCount < EXPECTED_NUMBER_OF_KEEPALIVES)
            {
                // Timeout
                bTestExecuted = false;
            }
            else
            {
                bTestResult = VerifyKeepalives(OCA_SECURE_TCP_SERVICE);
            }
        }
        else
        {
            AddTestLog("Failed to resolve test device");

            bTestExecuted = false;
        }

        ::OcaLiteCommandHandlerController::GetInstance().Disconnect(m_sessionID, GetNetworkObjectNumberFromServiceName(OCA_SECURE_TCP_SERVICE));
        m_sessionID = OCA_INVALID_SESSIONID;
    }
#endif

    m_receivedKeepaliveCount = 0;
    m_keepaliveTimestamps.clear();
    if ((GetContext().GetSupportedProtocols() & ::TestContext::OCA_UDP) && bTestResult)
    {
        AddTestLog("Verify keep-alive mechanism for OCP.1 UDP");

        m_sessionID = GetSession(GetContext().GetTestDeviceName(),
                                 OCA_UDP_SERVICE,
                                 KEEP_ALIVE_TIMEOUT,
                                 3,
                                 RESOLVE_TIMEOUT);

        if (OCA_INVALID_SESSIONID != m_sessionID)
        {
            UINT32 timeoutTime(::OcfLiteTimerGetTimerTickCount() + (((KEEP_ALIVE_TIMEOUT * 1000) * EXPECTED_NUMBER_OF_KEEPALIVES) * 2));
            while ((m_receivedKeepaliveCount < EXPECTED_NUMBER_OF_KEEPALIVES) &&
                    ::OcfLiteTimerGetTimerTickCount() < timeoutTime)
            {
                ::OcaLiteCommandHandlerController::GetInstance().RunWithTimeout(14);
            }

            if (m_receivedKeepaliveCount < EXPECTED_NUMBER_OF_KEEPALIVES)
            {
                // Timeout
                bTestExecuted = false;
            }
            else
            {
                bTestResult = VerifyKeepalives(OCA_UDP_SERVICE);
            }
        }
        else
        {
            AddTestLog("Failed to resolve test device");

            bTestExecuted = false;
        }

        ::OcaLiteCommandHandlerController::GetInstance().Disconnect(m_sessionID, GetNetworkObjectNumberFromServiceName(OCA_UDP_SERVICE));
        m_sessionID = OCA_INVALID_SESSIONID;
    }

    SetTestResult(bTestExecuted);

    return bTestExecuted;
}

void KeepAliveTest::CleanupTest()
{
    ::OcaLiteCommandHandlerController::GetInstance().UnregisterKeepAliveReceivedEventHandler(this);

    BaseTestClass::CleanupTest();
}

void KeepAliveTest::OnKeepAliveReceived(::OcaSessionID sessionID)
{
    UINT32 timestamp(::OcfLiteTimerGetTimerTickCount());
    AddTestLog("OnKeepAliveReceived: sessionID %d (waiting for %d). Timestamp %d", sessionID, m_sessionID, timestamp);
    
    if (m_sessionID == sessionID)
    {
        m_keepaliveTimestamps.push_back(timestamp);

        m_receivedKeepaliveCount++;
    }
}

bool KeepAliveTest::VerifyKeepalives(const std::string& protocol)
{
    AddTestResult("VerifyKeepalives with %d samples for protocol %s", m_keepaliveTimestamps.size(), protocol.c_str());

    bool bVerified(true);
    UINT32 comparingTimestamp(0);
    std::vector<UINT32>::iterator iter(m_keepaliveTimestamps.begin());

    while ((m_keepaliveTimestamps.end() != iter) && 
           bVerified)
    {
        if (0 != comparingTimestamp)
        {
            UINT32 difference = (*iter - comparingTimestamp);
            bVerified = (difference >= ((KEEP_ALIVE_TIMEOUT * 1000) - 500)) && (difference <= (KEEP_ALIVE_TIMEOUT * 1000) + 500);

            AddTestResult("   Difference between received keepalives for protocol %s is %d ms. Should be between %d and %d. Result %s",
                            protocol.c_str(),
                            difference,
                            (KEEP_ALIVE_TIMEOUT * 1000) - 500,
                            (KEEP_ALIVE_TIMEOUT * 1000) + 500,
                            bVerified ? "Passed" : "Failed");

        }

        // Store it for the next comparison
        comparingTimestamp = *iter;
        ++iter;
    }

    return bVerified;
}
