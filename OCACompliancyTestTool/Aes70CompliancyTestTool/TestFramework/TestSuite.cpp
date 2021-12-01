/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : Test suite
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>

#include <time.h>
#include <shlobj.h>
#include <winerror.h>
#include <comutil.h>
#include <iostream>
#pragma comment(lib, "comsuppw")

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "TestSuite.h"
#ifdef _DEBUG
#include "../Tests/DummyTest.h"
#endif
#include "../Tests/MinimumObjectCompliancyTest/MinimumObjectCompliancyTest.h"
#include "../Tests/OCC/ObjectCompliancyTest.h"
#include "../Tests/OCP.1/DeviceResetTest.h"
#include "../Tests/OCP.1/DeviceServiceDiscoveryTest.h"
#include "../Tests/OCP.1/KeepAliveTest.h"

// ---- Helper types and constants ----

// ---- Helper functions ----
static std::string GetLogPath();

// ---- Local data ----
::TestSuite* TestSuite::m_pSingleton(NULL);

// ---- Function Implementation ----
TestSuite::TestSuite() :
    TestLogger("Test Suite"),
    m_testCases()
{
    // Determine the log file path
    time_t now(time(0));
    char buf[96] = { 0 };
    struct tm tstruct;
    ::localtime_s(&tstruct, &now);
    ::strftime(buf, sizeof(buf), "%Y-%m-%d-%H-%M", &tstruct);
        
    m_baseLogPath = GetLogPath() + std::string(buf) + "-" + ::TestContext::GetInstance().GetTestDeviceName();

}

TestSuite::~TestSuite()
{

}

::TestSuite& TestSuite::GetInstance()
{
    if (NULL == TestSuite::m_pSingleton)
    {
        TestSuite::m_pSingleton = new TestSuite();
    }

    return *TestSuite::m_pSingleton;
}

void TestSuite::FreeInstance()
{
    if (TestSuite::m_pSingleton != NULL)
    {
        delete m_pSingleton;
        m_pSingleton = NULL;
    }
}

bool TestSuite::Initialize()
{
    OCA_LOG_ERROR_PARAMS("Logs will be written to %s", GetLogPath().c_str());
#ifdef _DEBUG
    //AddTestCase(new ::DummyTest);
#endif //_DEBUG

    // OCP.1
    AddTestCase(new ::DeviceServiceDiscoveryTest);
    AddTestCase(new ::DeviceResetTest);
    AddTestCase(new ::KeepAliveTest);

    // Minimum object compliancy
    AddTestCase(new ::MinimumObjectCompliancyTest);

    // OCC
    AddTestCase(new ::ObjectCompliancyTest);

    return true;
}

bool TestSuite::Teardown()
{
    bool bOverAllSuccessFull(true);
    int succesCounter(0);
    int failedCounter(0);
    
    OCA_LOG_ERROR("[#####] TestResults:");
    OCA_LOG_ERROR_PARAMS("[#####]   - TestCase Count %d", m_testCases.size());

    TestClassList::iterator iter(m_testCases.begin());
    while (iter != m_testCases.end())
    {
        bOverAllSuccessFull = bOverAllSuccessFull && (*iter)->IsTestSuccesfull();
        if ((*iter)->IsTestSuccesfull())
        {
            succesCounter++;
        }
        else
        {
            failedCounter++;
        }

        OCA_LOG_ERROR_PARAMS("[#####] Test result : %s, test result: %s",
            (*iter)->GetTestName().c_str(),
            (*iter)->IsTestSuccesfull() ? "Passed" : "Failed");

        std::vector<std::string>::const_iterator resultIter((*iter)->GetTestResults().begin());
        while (resultIter != (*iter)->GetTestResults().end())
        {
            OCA_LOG_ERROR_PARAMS("[#####]    %s", (*resultIter).c_str());
            ++resultIter;
        }
        OCA_LOG_ERROR("[#####]");

        delete *iter;
        ++iter;
    }
    m_testCases.clear();

    AddTestResult("");
    AddTestResult("Test Result Summary for device %s:",
        ::TestContext::GetInstance().GetTestDeviceName().c_str());
    AddTestResult("   %d tests executed", succesCounter + failedCounter);
    AddTestResult("   %d tests passed", succesCounter);
    AddTestResult("   %d tests failed", failedCounter);

    AddTestResult("");
    AddTestResult("Device %s is considered to be %s to %s",
        ::TestContext::GetInstance().GetTestDeviceName().c_str(),
        bOverAllSuccessFull ? "COMPLIANT" : "NON-COMPLIANT",
        ::BaseTestClass::GetVersionAsString(::TestContext::GetInstance().GetTestVersion()).c_str());

    AppendToFile(true, m_baseLogPath + "-TestResult.txt");
    AppendToFile(false, m_baseLogPath + "-TestLogs.txt");

    return bOverAllSuccessFull;
}

void TestSuite::AddTestCase(::BaseTestClass* testCase)
{
    m_testCases.push_back(testCase);
}

bool TestSuite::ExecuteTests(bool stopOnError)
{
    bool bTestExecutionSucceeded(true);

    AddTestResult("ExecuteTests:");
    AddTestResult("   - TestCase Count %d", m_testCases.size());

    TestClassList::iterator iter(m_testCases.begin());
    while (iter != m_testCases.end())
    {
        AddTestResult("Executing test: %s", (*iter)->GetTestName().c_str(), (*iter)->GetTestName().c_str());
        
        // Execute test.
        bool bTestSuccess((*iter)->PrepareTest());
        AddTestResult("   - Prepare test (%s) result: %s", (*iter)->GetTestName().c_str(), bTestSuccess ? "Passed" : "Failed");
        if (bTestSuccess)
        {
            bTestSuccess = (*iter)->ExecuteTest();
            AddTestResult("   - Execute test (%s) result: %s", (*iter)->GetTestName().c_str(), bTestSuccess ? "Passed" : "Failed");

            (*iter)->CleanupTest();
            AddTestResult("   - Cleanup test %s done", (*iter)->GetTestName().c_str());

            AddTestResult("   - Overall test %s result: %s", (*iter)->GetTestName().c_str(), (*iter)->IsTestSuccesfull() ? "Passed" : "Failed");
        }

        if (stopOnError) 
        {
            if ((!bTestSuccess) || !(*iter)->IsTestSuccesfull())
            {
                AddTestResult("%s StopOnError!", (*iter)->GetTestName().c_str());

                // Log before we exit the test because of the error
                (*iter)->AppendToFile(true, m_baseLogPath + "-TestResult.txt");
                (*iter)->AppendToFile(false, m_baseLogPath + "-TestLogs.txt");

                break;
            }
        }

        // Log the test result
        (*iter)->AppendToFile(true, m_baseLogPath + "-TestResult.txt");
        (*iter)->AppendToFile(false, m_baseLogPath + "-TestLogs.txt");

        // On to next test..
        ++iter;
    }

    return bTestExecutionSucceeded;
}

// ---- Helper function ----

/**
 * Get the base path to log to.
 *
 * @ return The base log path
 */
static std::string GetLogPath()
{
    LPWSTR wszPath(NULL);
    HRESULT hr(SHGetKnownFolderPath(FOLDERID_ProgramData, KF_FLAG_CREATE, NULL, &wszPath));
    if (SUCCEEDED(hr))
    {
        _bstr_t bstrPath(wszPath);
        std::string strPath(static_cast<char*>(bstrPath));
        strPath += "\\OCAAlliance\\CompliancyTestTool\\";

        std::wstring wStrPath(strPath.begin(), strPath.end());
        if (SHCreateDirectoryEx(NULL, wStrPath.c_str(), NULL) == ERROR_SUCCESS)
        {
            OCA_LOG_TRACE_PARAMS("Directory created (%s)", strPath.c_str());
        }
        else if (ERROR_ALREADY_EXISTS == GetLastError())
        {
            OCA_LOG_TRACE_PARAMS("Directory (%s) already exists (%d)", strPath.c_str(), GetLastError());
        }
        else
        {
            OCA_LOG_ERROR_PARAMS("Directory (%s) creation failed for some other reason (%d)", strPath.c_str(), GetLastError());
        }

        return strPath;
    }

    return "";
}
