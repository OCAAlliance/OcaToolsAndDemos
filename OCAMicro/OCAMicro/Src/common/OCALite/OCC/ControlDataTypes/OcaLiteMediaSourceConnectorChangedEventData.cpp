/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMediaSourceConnectorChangedEventData
 *
 */

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteMediaSourceConnectorChangedEventData.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMediaSourceConnectorChangedEventData::OcaLiteMediaSourceConnectorChangedEventData()
    : ::OcaLiteEventData(),
    m_sourceConnector(),
    m_changeType(),
    m_changedElement()
{
}

OcaLiteMediaSourceConnectorChangedEventData::OcaLiteMediaSourceConnectorChangedEventData(const ::OcaLiteEvent& event, const ::OcaLiteMediaSourceConnector& sourceConnector, ::OcaLitePropertyChangeType changeType, ::OcaLiteMediaConnectorElement changedElement)
    : ::OcaLiteEventData(event),
    m_sourceConnector(sourceConnector),
    m_changeType(changeType),
    m_changedElement(changedElement)
{
}

OcaLiteMediaSourceConnectorChangedEventData::OcaLiteMediaSourceConnectorChangedEventData(const ::OcaLiteMediaSourceConnectorChangedEventData& source)
    : ::OcaLiteEventData(source),
    m_sourceConnector(source.m_sourceConnector),
    m_changeType(source.m_changeType),
    m_changedElement(source.m_changedElement)
{
}

OcaLiteMediaSourceConnectorChangedEventData::~OcaLiteMediaSourceConnectorChangedEventData()
{
}

::OcaLiteMediaSourceConnectorChangedEventData& OcaLiteMediaSourceConnectorChangedEventData::operator=(const ::OcaLiteMediaSourceConnectorChangedEventData& source)
{
    if (this != &source)
    {
        OcaLiteEventData::operator=(source);
        m_sourceConnector = source.m_sourceConnector;
        m_changeType = source.m_changeType;
        m_changedElement = source.m_changedElement;
    }

    return *this;
}

bool OcaLiteMediaSourceConnectorChangedEventData::operator==(const ::OcaLiteMediaSourceConnectorChangedEventData& rhs) const
{
    bool result(OcaLiteEventData::operator==(rhs));
    result = result && (m_sourceConnector == rhs.m_sourceConnector);
    result = result && (m_changeType == rhs.m_changeType);
    result = result && (m_changedElement == rhs.m_changedElement);

    return result;
}

bool OcaLiteMediaSourceConnectorChangedEventData::operator!=(const ::OcaLiteMediaSourceConnectorChangedEventData& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteMediaSourceConnectorChangedEventData::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    OcaLiteEventData::Marshal(destination, writer);
    m_sourceConnector.Marshal(destination, writer);
    MarshalValue< ::OcaLitePropertyChangeType>(m_changeType, destination, writer);
    MarshalValue< ::OcaLiteMediaConnectorElement>(m_changedElement, destination, writer);
}

bool OcaLiteMediaSourceConnectorChangedEventData::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(OcaLiteEventData::Unmarshal(bytesLeft, source, reader));
    result = result && m_sourceConnector.Unmarshal(bytesLeft, source, reader);
    result = result && UnmarshalValue< ::OcaLitePropertyChangeType>(m_changeType, bytesLeft, source, reader);
    result = result && UnmarshalValue< ::OcaLiteMediaConnectorElement>(m_changedElement, bytesLeft, source, reader);
    if (!result)
    {
        (*this) = ::OcaLiteMediaSourceConnectorChangedEventData();
    }

    return result;
}

::OcaUint32 OcaLiteMediaSourceConnectorChangedEventData::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(OcaLiteEventData::GetSize(writer));
    length += m_sourceConnector.GetSize(writer);
    length += GetSizeValue< ::OcaLitePropertyChangeType>(m_changeType, writer);
    length += GetSizeValue< ::OcaLiteMediaConnectorElement>(m_changedElement, writer);

    return length;
}
