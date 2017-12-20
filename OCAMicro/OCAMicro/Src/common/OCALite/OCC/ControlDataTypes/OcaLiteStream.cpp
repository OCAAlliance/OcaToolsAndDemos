/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteStream
 *
 */

// ---- Include system wide include files ----
#include <assert.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteStream.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteStream::OcaLiteStream()
    : ::IOcaLiteMarshal(),
      m_id(),
      m_label(),
      m_index(static_cast< ::OcaLiteStreamIndex>(0)),
      m_type(OCASTREAMTYPE_NONE),
      m_localConnectorONo(OCA_INVALID_ONO),
      m_remoteConnectorId(),
      m_priority(static_cast< ::OcaUint16>(0)),      
      m_secure(static_cast< ::OcaBoolean>(false)),
      m_streamParameters(),
      m_errorNumber(static_cast< ::OcaUint16>(0)),
      m_status()
{
}

OcaLiteStream::OcaLiteStream(::OcaLiteStreamID id,
                             const ::OcaLiteString& label,
                             ::OcaUint16 index,
                             ::OcaLiteStreamType type,
                             ::OcaONo localConnector,
                             ::OcaLiteStreamConnectorID remoteConnectorId,
                             ::OcaUint16 priority,
                             ::OcaBoolean secure,
                             const ::OcaLiteStreamParameters& streamParameters,
                             ::OcaUint16 errorNumber,
                             ::OcaLiteStreamStatus status)
    : ::IOcaLiteMarshal(),
      m_id(id),
      m_label(label),
      m_index(index),
      m_type(type),
      m_localConnectorONo(localConnector),
      m_remoteConnectorId(remoteConnectorId),
      m_priority(priority),
      m_secure(secure),
      m_streamParameters(streamParameters),
      m_errorNumber(errorNumber),
      m_status(status)
{
    assert(m_status < OCASTREAMSTATUS_MAXIMUM);
}

OcaLiteStream::OcaLiteStream(const ::OcaLiteStream& source)
    : ::IOcaLiteMarshal(source),
    m_id(source.m_id),
    m_label(source.m_label),
    m_index(source.m_index),
    m_type(source.m_type),
    m_localConnectorONo(source.m_localConnectorONo),
    m_remoteConnectorId(source.m_remoteConnectorId),
    m_priority(source.m_priority),
    m_secure(source.m_secure),
    m_streamParameters(source.m_streamParameters),
    m_errorNumber(source.m_errorNumber),
    m_status(source.m_status)
{
}

OcaLiteStream::~OcaLiteStream()
{
}

::OcaLiteStream& OcaLiteStream::operator=(const ::OcaLiteStream& source)
{
    if (this != &source)
    {
        m_id = source.m_id;
        m_label = source.m_label;
        m_index = source.m_index;
        m_type = source.m_type;
        m_localConnectorONo = source.m_localConnectorONo;
        m_remoteConnectorId = source.m_remoteConnectorId;
        m_priority = source.m_priority;
        m_secure = source.m_secure;
        m_streamParameters = source.m_streamParameters;
        m_errorNumber = source.m_errorNumber;
        m_status = source.m_status;
    }

    return *this;
}

bool OcaLiteStream::operator==(const ::OcaLiteStream& rhs) const
{
    bool result(m_id == rhs.m_id);
    result = result && (m_label == rhs.m_label);
    result = result && (m_index == rhs.m_index);
    result = result && (m_type == rhs.m_type);
    result = result && (m_localConnectorONo == rhs.m_localConnectorONo);
    result = result && (m_remoteConnectorId == rhs.m_remoteConnectorId);
    result = result && (m_priority == rhs.m_priority);
    result = result && (m_secure == rhs.m_secure);
    result = result && (m_streamParameters == rhs.m_streamParameters);
    result = result && (m_errorNumber == rhs.m_errorNumber);
    result = result && (m_status == rhs.m_status);
    return result;
}

bool OcaLiteStream::operator!=(const ::OcaLiteStream& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteStream::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    MarshalValue< ::OcaLiteStreamID>(m_id, destination, writer);
    m_label.Marshal(destination, writer);
    writer.Write(m_index, destination);
    MarshalValue< ::OcaLiteStreamType>(m_type, destination, writer);
    MarshalValue< ::OcaONo>(m_localConnectorONo, destination, writer);
    m_remoteConnectorId.Marshal(destination, writer);
    writer.Write(m_priority, destination);
    writer.Write(m_secure, destination);
    m_streamParameters.Marshal(destination, writer);
    writer.Write(m_errorNumber, destination);
    MarshalValue< ::OcaLiteStreamStatus>(m_status, destination, writer);
}

bool OcaLiteStream::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(UnmarshalValue< ::OcaLiteStreamID>(m_id, bytesLeft, source, reader));
    result = result && m_label.Unmarshal(bytesLeft, source, reader);
    result = result && reader.Read(bytesLeft, source, m_index);
    result = result && UnmarshalValue< ::OcaLiteStreamType>(m_type, bytesLeft, source, reader);
    result = result && UnmarshalValue< ::OcaONo>(m_localConnectorONo, bytesLeft, source, reader);
    result = result && m_remoteConnectorId.Unmarshal(bytesLeft, source, reader);
    result = result && reader.Read(bytesLeft, source, m_priority);
    result = result && reader.Read(bytesLeft, source, m_secure);
    result = result && m_streamParameters.Unmarshal(bytesLeft, source, reader);
    result = result && reader.Read(bytesLeft, source, m_errorNumber);
    result = result && UnmarshalValue< ::OcaLiteStreamStatus>(m_status, bytesLeft, source, reader);

    if (!result)
    {
        m_id = ::OcaLiteStreamID();
        m_label = ::OcaLiteString();
        m_index = static_cast< ::OcaUint16>(0);
        m_type = OCASTREAMTYPE_NONE;
        m_localConnectorONo = OCA_INVALID_ONO;
        m_remoteConnectorId = ::OcaLiteStreamConnectorID();
        m_priority = static_cast< ::OcaUint16>(0);
        m_secure = static_cast< ::OcaBoolean>(false);
        m_streamParameters = ::OcaLiteStreamParameters();
        m_errorNumber = static_cast< ::OcaUint16>(0);
        m_status = OCASTREAMSTATUS_NOT_CONNECTED;
    }

    return result;
}

::OcaUint32 OcaLiteStream::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(GetSizeValue< ::OcaLiteStreamID>(m_id, writer));
    length += m_label.GetSize(writer);
    length += writer.GetSize(m_index);
    length += GetSizeValue< ::OcaLiteStreamType>(m_type, writer);
    length += GetSizeValue< ::OcaONo>(m_localConnectorONo, writer);
    length += GetSizeValue< ::OcaLiteStreamConnectorID>(m_remoteConnectorId, writer);
    length += writer.GetSize(m_priority);
    length += writer.GetSize(m_secure);
    length += m_streamParameters.GetSize(writer);
    length += writer.GetSize(m_errorNumber);
    length += GetSizeValue< ::OcaLiteStreamStatus>(m_status, writer);

    return length;
}

template <>
void MarshalValue< ::OcaLiteStreamType>(const ::OcaLiteStreamType& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
bool UnmarshalValue< ::OcaLiteStreamType>(::OcaLiteStreamType& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 status(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, status));
    if (result)
    {
        if (status < static_cast< ::OcaUint8>(OCASTREAMTYPE_MAXIMUM))
        {
            value = static_cast< ::OcaLiteStreamType>(status);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteStreamType>(const ::OcaLiteStreamType& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}
