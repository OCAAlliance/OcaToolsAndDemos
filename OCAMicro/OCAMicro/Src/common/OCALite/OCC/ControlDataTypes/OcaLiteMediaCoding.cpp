/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMediaCoding
 *
*/

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteMediaCoding.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMediaCoding::OcaLiteMediaCoding()
    : ::IOcaLiteMarshal(),
    m_codingSchemeID(static_cast< ::OcaLiteMediaCodingSchemeID>(0)),
    m_codecParameters(),
    m_clockONo(static_cast< ::OcaONo>(OCA_INVALID_ONO))
{
}

OcaLiteMediaCoding::OcaLiteMediaCoding(::OcaLiteMediaCodingSchemeID codingSchemeID, const ::OcaLiteString& codecParameters, ::OcaONo clockONo)
    : ::IOcaLiteMarshal(),
    m_codingSchemeID(codingSchemeID),
    m_codecParameters(codecParameters),
    m_clockONo(clockONo)
{
}

OcaLiteMediaCoding::OcaLiteMediaCoding(const ::OcaLiteMediaCoding& source)
    : ::IOcaLiteMarshal(source),
    m_codingSchemeID(source.m_codingSchemeID),
    m_codecParameters(source.m_codecParameters),
    m_clockONo(source.m_clockONo)
{
}

OcaLiteMediaCoding::~OcaLiteMediaCoding()
{
}

::OcaLiteMediaCoding& OcaLiteMediaCoding::operator=(const ::OcaLiteMediaCoding& source)
{
    if (this != &source)
    {
        m_codingSchemeID = source.m_codingSchemeID;
        m_codecParameters = source.m_codecParameters;
        m_clockONo = source.m_clockONo;
    }

    return *this;
}

bool OcaLiteMediaCoding::operator==(const ::OcaLiteMediaCoding& rhs) const
{
    bool bResult(m_codingSchemeID == rhs.m_codingSchemeID);
    bResult = bResult && (m_codecParameters == rhs.m_codecParameters);
    bResult = bResult && (m_clockONo == rhs.m_clockONo);
    return bResult;
}

bool OcaLiteMediaCoding::operator!=(const ::OcaLiteMediaCoding& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteMediaCoding::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(m_codingSchemeID, destination);
    m_codecParameters.Marshal(destination, writer);
    MarshalValue< ::OcaONo>(m_clockONo, destination, writer);
}

bool OcaLiteMediaCoding::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(reader.Read(bytesLeft, source, m_codingSchemeID));
    result = result && m_codecParameters.Unmarshal(bytesLeft, source, reader);
    result = result && UnmarshalValue< ::OcaONo>(m_clockONo, bytesLeft, source, reader);

    if (!result)
    {
        (*this) = ::OcaLiteMediaCoding();
    }

    return result;
}

::OcaUint32 OcaLiteMediaCoding::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(writer.GetSize(m_codingSchemeID));
    length += m_codecParameters.GetSize(writer);
    length += GetSizeValue< ::OcaONo>(m_clockONo, writer);

    return length;
}
