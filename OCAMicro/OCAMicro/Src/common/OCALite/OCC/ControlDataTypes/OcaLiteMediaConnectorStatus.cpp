/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMediaConnectorStatus
 *
 */

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteMediaConnectorStatus.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMediaConnectorStatus::OcaLiteMediaConnectorStatus()
    : ::IOcaLiteMarshal(),
    m_connectorId(static_cast< ::OcaUint16>(0)),
    m_connectorState(OCAMEDIACONNECTORSTATE_STOPPED),
    m_errorCode(static_cast< ::OcaUint16>(0))
{
}

OcaLiteMediaConnectorStatus::OcaLiteMediaConnectorStatus(::OcaLiteMediaConnectorID connectorId, ::OcaLiteMediaConnectorState state, ::OcaUint16 errorCode)
    : ::IOcaLiteMarshal(),
    m_connectorId(connectorId),
    m_connectorState(state),
    m_errorCode(errorCode)
{
}

OcaLiteMediaConnectorStatus::OcaLiteMediaConnectorStatus(const ::OcaLiteMediaConnectorStatus& source)
    : ::IOcaLiteMarshal(source),
    m_connectorId(source.m_connectorId),
    m_connectorState(source.m_connectorState),
    m_errorCode(source.m_errorCode)
{
}

OcaLiteMediaConnectorStatus::~OcaLiteMediaConnectorStatus()
{
}

::OcaLiteMediaConnectorStatus& OcaLiteMediaConnectorStatus::operator=(const ::OcaLiteMediaConnectorStatus& source)
{
    if (this != &source)
    {
        m_connectorId = source.m_connectorId;
        m_connectorState = source.m_connectorState;
        m_errorCode = source.m_errorCode;
    }

    return *this;
}

bool OcaLiteMediaConnectorStatus::operator==(const ::OcaLiteMediaConnectorStatus& rhs) const
{
    bool result(m_connectorId == rhs.m_connectorId);
    result = result && (m_connectorState == rhs.m_connectorState);
    result = result && (m_errorCode == rhs.m_errorCode);

    return result;
}

bool OcaLiteMediaConnectorStatus::operator!=(const ::OcaLiteMediaConnectorStatus& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteMediaConnectorStatus::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(m_connectorId, destination);
    MarshalValue< ::OcaLiteMediaConnectorState>(m_connectorState, destination, writer);
    writer.Write(m_errorCode, destination);
}

bool OcaLiteMediaConnectorStatus::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(reader.Read(bytesLeft, source, m_connectorId));
    result = result && UnmarshalValue< ::OcaLiteMediaConnectorState>(m_connectorState, bytesLeft, source, reader);
    result = result && reader.Read(bytesLeft, source, m_errorCode);

    if (!result)
    {
        (*this) = ::OcaLiteMediaConnectorStatus();
    }

    return result;
}

::OcaUint32 OcaLiteMediaConnectorStatus::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(writer.GetSize(m_connectorId));
    length += GetSizeValue< ::OcaLiteMediaConnectorState>(m_connectorState, writer);
    length += writer.GetSize(m_errorCode);

    return length;
}
