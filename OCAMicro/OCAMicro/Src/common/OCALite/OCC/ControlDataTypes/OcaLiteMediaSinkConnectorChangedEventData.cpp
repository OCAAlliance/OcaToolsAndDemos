/*  By downloading or using this file, the user agrees to be bound by the terms of the license
*  agreement located at http://ocaalliance.com/EULA as an original contracting party.
*
*  Description         : OcaLiteMediaSinkConnectorChangedEventData
*
*/

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteMediaSinkConnectorChangedEventData.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMediaSinkConnectorChangedEventData::OcaLiteMediaSinkConnectorChangedEventData()
    : ::OcaLiteEventData(),
    m_sinkConnector(),
    m_changeType(),
    m_changedElement()
{
}

OcaLiteMediaSinkConnectorChangedEventData::OcaLiteMediaSinkConnectorChangedEventData(const ::OcaLiteEvent& event, const ::OcaLiteMediaSinkConnector& sinkConnector, ::OcaLitePropertyChangeType changeType, ::OcaLiteMediaConnectorElement changedElement)
    : ::OcaLiteEventData(event),
    m_sinkConnector(sinkConnector),
    m_changeType(changeType),
    m_changedElement(changedElement)
{
}

OcaLiteMediaSinkConnectorChangedEventData::OcaLiteMediaSinkConnectorChangedEventData(const ::OcaLiteMediaSinkConnectorChangedEventData& source)
    : ::OcaLiteEventData(source),
    m_sinkConnector(source.m_sinkConnector),
    m_changeType(source.m_changeType),
    m_changedElement(source.m_changedElement)
{
}

OcaLiteMediaSinkConnectorChangedEventData::~OcaLiteMediaSinkConnectorChangedEventData()
{
}

::OcaLiteMediaSinkConnectorChangedEventData& OcaLiteMediaSinkConnectorChangedEventData::operator=(const ::OcaLiteMediaSinkConnectorChangedEventData& source)
{
    if (this != &source)
    {
        OcaLiteEventData::operator=(source);
        m_sinkConnector = source.m_sinkConnector;
        m_changeType = source.m_changeType;
        m_changedElement = source.m_changedElement;
    }

    return *this;
}

bool OcaLiteMediaSinkConnectorChangedEventData::operator==(const ::OcaLiteMediaSinkConnectorChangedEventData& rhs) const
{
    bool result(OcaLiteEventData::operator==(rhs));
    result = result && (m_sinkConnector == rhs.m_sinkConnector);
    result = result && (m_changeType == rhs.m_changeType);
    result = result && (m_changedElement == rhs.m_changedElement);

    return result;
}

bool OcaLiteMediaSinkConnectorChangedEventData::operator!=(const ::OcaLiteMediaSinkConnectorChangedEventData& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteMediaSinkConnectorChangedEventData::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    OcaLiteEventData::Marshal(destination, writer);
    m_sinkConnector.Marshal(destination, writer);
    MarshalValue< ::OcaLitePropertyChangeType>(m_changeType, destination, writer);
    MarshalValue< ::OcaLiteMediaConnectorElement>(m_changedElement, destination, writer);
}

bool OcaLiteMediaSinkConnectorChangedEventData::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(OcaLiteEventData::Unmarshal(bytesLeft, source, reader));
    result = result && m_sinkConnector.Unmarshal(bytesLeft, source, reader);
    result = result && UnmarshalValue< ::OcaLitePropertyChangeType>(m_changeType, bytesLeft, source, reader);
    result = result && UnmarshalValue< ::OcaLiteMediaConnectorElement>(m_changedElement, bytesLeft, source, reader);
    if (!result)
    {
        (*this) = ::OcaLiteMediaSinkConnectorChangedEventData();
    }

    return result;
}

::OcaUint32 OcaLiteMediaSinkConnectorChangedEventData::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(OcaLiteEventData::GetSize(writer));
    length += m_sinkConnector.GetSize(writer);
    length += GetSizeValue< ::OcaLitePropertyChangeType>(m_changeType, writer);
    length += GetSizeValue< ::OcaLiteMediaConnectorElement>(m_changedElement, writer);

    return length;
}
