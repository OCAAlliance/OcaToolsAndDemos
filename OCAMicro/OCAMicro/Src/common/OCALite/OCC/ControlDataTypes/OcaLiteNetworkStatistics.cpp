/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteNetworkStatistics
 *
 */

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteNetworkStatistics.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteNetworkStatistics::OcaLiteNetworkStatistics()
    : ::IOcaLiteMarshal(),
      m_txPacketErrors(static_cast< ::OcaUint32>(0)),
      m_rxPacketErrors(static_cast< ::OcaUint32>(0))
{
}

OcaLiteNetworkStatistics::OcaLiteNetworkStatistics(::OcaUint32 txPacketErrors, ::OcaUint32 rxPacketErrors)
    : ::IOcaLiteMarshal(),
      m_txPacketErrors(txPacketErrors),
      m_rxPacketErrors(rxPacketErrors)
{
}

OcaLiteNetworkStatistics::OcaLiteNetworkStatistics(const ::OcaLiteNetworkStatistics& source)
    : ::IOcaLiteMarshal(source),
      m_txPacketErrors(source.m_txPacketErrors),
      m_rxPacketErrors(source.m_rxPacketErrors)
{
}

OcaLiteNetworkStatistics::~OcaLiteNetworkStatistics()
{
}

::OcaLiteNetworkStatistics& OcaLiteNetworkStatistics::operator=(const ::OcaLiteNetworkStatistics& source)
{
    if (this != &source)
    {
        m_txPacketErrors = source.m_txPacketErrors;
        m_rxPacketErrors = source.m_rxPacketErrors;
    }

    return *this;
}

bool OcaLiteNetworkStatistics::operator==(const ::OcaLiteNetworkStatistics& rhs) const
{
    bool result(m_txPacketErrors == rhs.m_txPacketErrors);
    result = result && (m_rxPacketErrors == rhs.m_rxPacketErrors);

    return result;
}

bool OcaLiteNetworkStatistics::operator!=(const ::OcaLiteNetworkStatistics& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteNetworkStatistics::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(m_txPacketErrors, destination);
    writer.Write(m_rxPacketErrors, destination);
}

bool OcaLiteNetworkStatistics::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(reader.Read(bytesLeft, source, m_txPacketErrors));
    result = result && reader.Read(bytesLeft, source, m_rxPacketErrors);

    if (!result)
    {
        m_txPacketErrors = static_cast< ::OcaUint32>(0);
        m_rxPacketErrors = static_cast< ::OcaUint32>(0);
    }

    return result;
}

::OcaUint32 OcaLiteNetworkStatistics::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(writer.GetSize(m_txPacketErrors));
    length += writer.GetSize(m_rxPacketErrors);

    return length;
}
