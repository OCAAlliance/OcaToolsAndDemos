/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMediaConnection
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteMediaConnection.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMediaConnection::OcaLiteMediaConnection()
    : ::IOcaLiteMarshal(),
    m_secure(static_cast< ::OcaBoolean>(false)),
    m_streamParameters(),
    m_streamCastMode(OCAMEDIASTREAMCASTMODE_NONE)
{
}

OcaLiteMediaConnection::OcaLiteMediaConnection(::OcaBoolean secure, ::OcaLiteMediaStreamParameters streamParameters, ::OcaLiteMediaStreamCastMode streamCastMode)
    : ::IOcaLiteMarshal(),
    m_secure(secure),
    m_streamParameters(streamParameters),
    m_streamCastMode(streamCastMode)
{
}

OcaLiteMediaConnection::OcaLiteMediaConnection(const ::OcaLiteMediaConnection& source)
    : ::IOcaLiteMarshal(source),
    m_secure(source.m_secure),
    m_streamParameters(source.m_streamParameters),
    m_streamCastMode(source.m_streamCastMode)
{
}

OcaLiteMediaConnection::~OcaLiteMediaConnection()
{
}

::OcaLiteMediaConnection& OcaLiteMediaConnection::operator=(const ::OcaLiteMediaConnection& source)
{
    if (this != &source)
    {
        m_secure = source.m_secure;
        m_streamParameters = source.m_streamParameters;
        m_streamCastMode = source.m_streamCastMode;
    }

    return *this;
}

bool OcaLiteMediaConnection::operator==(const ::OcaLiteMediaConnection& rhs) const
{
    bool bResult(m_secure == rhs.m_secure);
    bResult = bResult && (m_streamParameters == rhs.m_streamParameters);
    bResult = bResult && (m_streamCastMode == rhs.m_streamCastMode);
    return bResult;
}

bool OcaLiteMediaConnection::operator!=(const ::OcaLiteMediaConnection& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteMediaConnection::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(m_secure, destination);
    m_streamParameters.Marshal(destination, writer);
    MarshalValue< ::OcaLiteMediaStreamCastMode>(m_streamCastMode, destination, writer);
}

bool OcaLiteMediaConnection::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(reader.Read(bytesLeft, source, m_secure));
    result = result && m_streamParameters.Unmarshal(bytesLeft, source, reader);
    result = result && UnmarshalValue< ::OcaLiteMediaStreamCastMode>(m_streamCastMode, bytesLeft, source, reader);

    if (!result)
    {
        (*this) = ::OcaLiteMediaConnection();
    }

    return result;
}

::OcaUint32 OcaLiteMediaConnection::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(writer.GetSize(m_secure));
    length += m_streamParameters.GetSize(writer);
    length += GetSizeValue< ::OcaLiteMediaStreamCastMode>(m_streamCastMode, writer);
    return length;
}
