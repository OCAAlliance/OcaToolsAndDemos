/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMediaConnectorStatusChangedEventData
 *
 */

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteMediaConnectorStatusChangedEventData.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMediaConnectorStatusChangedEventData::OcaLiteMediaConnectorStatusChangedEventData()
    : ::OcaLiteEventData(),
    m_connectorStatus()
{
}

OcaLiteMediaConnectorStatusChangedEventData::OcaLiteMediaConnectorStatusChangedEventData(const ::OcaLiteEvent& event, const ::OcaLiteMediaConnectorStatus& connectorStatus)
    : ::OcaLiteEventData(event),
    m_connectorStatus(connectorStatus)
{
}

OcaLiteMediaConnectorStatusChangedEventData::OcaLiteMediaConnectorStatusChangedEventData(const ::OcaLiteMediaConnectorStatusChangedEventData& source)
    : ::OcaLiteEventData(source),
    m_connectorStatus(source.m_connectorStatus)
{
}

OcaLiteMediaConnectorStatusChangedEventData::~OcaLiteMediaConnectorStatusChangedEventData()
{
}

::OcaLiteMediaConnectorStatusChangedEventData& OcaLiteMediaConnectorStatusChangedEventData::operator=(const ::OcaLiteMediaConnectorStatusChangedEventData& source)
{
    if (this != &source)
    {
        OcaLiteEventData::operator=(source);
        m_connectorStatus = source.m_connectorStatus;
    }

    return *this;
}

bool OcaLiteMediaConnectorStatusChangedEventData::operator==(const ::OcaLiteMediaConnectorStatusChangedEventData& rhs) const
{
    bool result(OcaLiteEventData::operator==(rhs));
    result = result && (m_connectorStatus == rhs.m_connectorStatus);

    return result;
}

bool OcaLiteMediaConnectorStatusChangedEventData::operator!=(const ::OcaLiteMediaConnectorStatusChangedEventData& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteMediaConnectorStatusChangedEventData::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    OcaLiteEventData::Marshal(destination, writer);
    m_connectorStatus.Marshal(destination, writer);
}

bool OcaLiteMediaConnectorStatusChangedEventData::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(OcaLiteEventData::Unmarshal(bytesLeft, source, reader));
    result = result && m_connectorStatus.Unmarshal(bytesLeft, source, reader);

    if (!result)
    {
        (*this) = ::OcaLiteMediaConnectorStatusChangedEventData();
    }

    return result;
}

::OcaUint32 OcaLiteMediaConnectorStatusChangedEventData::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(OcaLiteEventData::GetSize(writer));
    length += m_connectorStatus.GetSize(writer);

    return length;
}
