/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Test logger
 */

// ---- Include system wide include files ----
#include <algorithm>
#include <stdarg.h>
#include <fstream>
#include <iostream>
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "TestLogger.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----
/** Array to store full log messages temporary */
static char m_fullLogMessage[1024];

// ---- Function Implementation ----

TestLogger::TestLogger(const std::string& testName)
    : m_testName(testName)
{
}

const std::string& TestLogger::GetTestName() const
{
    return m_testName;
}

const TestLogger::StringVector& TestLogger::GetTestResults()
{
    return m_testResult;
}

const TestLogger::StringVector& TestLogger::GetTestLogs()
{
    return m_testLog;
}

void TestLogger::AddTestResult(const char* text, ...)
{
    // Format and print the message
    va_list argList;
    va_start(argList, text);

    // Check if the message fits into the message buffer, add 1 for terminating '\0'
    UINT32 messageLength(static_cast<UINT32>(::vsnprintf(NULL, 0, text, argList) + 1));
    if (messageLength <= sizeof(m_fullLogMessage))
    {
        INT32 result(::vsprintf_s(m_fullLogMessage, text, argList));
        bool bAddCarrageReturn = false;

        if (static_cast<UINT32>(result) <= messageLength)
        {
            // Decrease message indent

            if (messageLength >= 2)
            {
                if ((m_fullLogMessage[messageLength - 2] != '\r') &&
                    (m_fullLogMessage[messageLength - 2] != '\n'))
                {
                    bAddCarrageReturn = true;
                }
            }
            else
            {
                bAddCarrageReturn = true;
            }

            printf("[%s][Result] %s%s", GetTestName().c_str(), &m_fullLogMessage[0], bAddCarrageReturn ? "\r\n" : "");
            m_testResult.push_back("[" + GetTestName() + "]" + m_fullLogMessage + (bAddCarrageReturn ? "\r\n" : ""));

            // Also add this to the test log, this makes it easier to debug.
            m_testLog.push_back("[" + GetTestName() + "]" + m_fullLogMessage + (bAddCarrageReturn ? "\r\n" : ""));
        }
    }
    va_end(argList);
}

void TestLogger::AddTestLog(const char* text, ...)
{
    // Format and print the message
    va_list argList;
    va_start(argList, text);

    // Check if the message fits into the message buffer, add 1 for terminating '\0'
    UINT32 messageLength(static_cast<UINT32>(::vsnprintf(NULL, 0, text, argList) + 1));
    if (messageLength <= sizeof(m_fullLogMessage))
    {
        INT32 result(::vsprintf_s(m_fullLogMessage, text, argList));
        bool bAddCarrageReturn = false;

        if (static_cast<UINT32>(result) <= messageLength)
        {
            // Decrease message indent
            if (messageLength >= 2)
            {
                if ((m_fullLogMessage[messageLength - 2] != '\r') &&
                    (m_fullLogMessage[messageLength - 2] != '\n'))
                {
                    bAddCarrageReturn = true;
                }
            }
            else
            {
                bAddCarrageReturn = true;
            }

            OCA_LOG_INFO_PARAMS("[%s][TLog  ] %s%s", GetTestName().c_str(), &m_fullLogMessage[0], bAddCarrageReturn ? "\r\n" : "");
            m_testLog.push_back("[" + GetTestName() + "]" + m_fullLogMessage + (bAddCarrageReturn ? "\r\n" : ""));
        }
    }
    va_end(argList);
}

bool TestLogger::AppendToFile(bool bTestResults, const std::string& fileName)
{
    bool bResult(false);
    StringVector logs(bTestResults ? m_testResult : m_testLog);
    OCA_LOG_INFO_PARAMS("Appending %d log items to file %s", logs.size(), fileName.c_str());

    std::ofstream os(fileName, std::fstream::out|std::fstream::app);
    if (os)
    {
        std::vector<std::string>::const_iterator iter(logs.begin());
        while (iter != logs.end())
        {
            os << *iter;
            ++iter;
        }

        bResult = true;
    }

    return bResult;
}
