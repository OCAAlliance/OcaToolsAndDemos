/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLite ResolveResult implementation.
 *
 */

// ---- Include system wide include files ----
#include <Winsock2.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "ResolveResult.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

ResolveResult::ResolveResult()
    : m_interfaceIndex(0),
      m_fullName(),
      m_hostTarget(),
      m_port(),
      m_txtRecords()
{
}

void ResolveResult::SetResolveResult(UINT32 interfaceIndex, const char* fullName, const char* hosttarget,
                                     UINT16 port, UINT16 txtLength, const unsigned char* txtRecord)
{
    m_interfaceIndex = interfaceIndex;
    m_fullName = fullName;
    m_hostTarget = hosttarget;
    m_port = htons(port);
    ParseTxtRecords(txtLength, txtRecord);
}


UINT32 ResolveResult::GetInterfaceIndex() const
{
    return m_interfaceIndex;
}

const std::string& ResolveResult::GetFullName() const
{
    return m_fullName;
}

const std::string& ResolveResult::GetHostTarget() const
{
    return m_hostTarget;
}

UINT16 ResolveResult::GetPort() const
{
    return m_port;
}

const std::vector<std::string>& ResolveResult::GetTxtRecords() const
{
    return m_txtRecords;
}

void ResolveResult::ParseTxtRecords(UINT16 txtLength, const unsigned char* txtRecord)
{
    m_txtRecords.clear();

    if (txtLength > 0)
    {
        UINT16 currentPos(0);
        
        while (currentPos < txtLength)
        {
            UINT8 recordLength(txtRecord[currentPos]);
            currentPos++;

            std::string record(reinterpret_cast<const char*>(&txtRecord[currentPos]), recordLength);
            m_txtRecords.push_back(record);
            currentPos += recordLength;
        }
    }
}