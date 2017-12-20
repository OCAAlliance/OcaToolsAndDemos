/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteBlobDataType
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteBlobDataType.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteBlobDataType::OcaLiteBlobDataType()
    : ::IOcaLiteMarshal(),
      m_value()
{
}

OcaLiteBlobDataType::OcaLiteBlobDataType(const ::OcaLiteBlob& value)
    : ::IOcaLiteMarshal(),
      m_value(value)
{
}

OcaLiteBlobDataType::OcaLiteBlobDataType(const ::OcaLiteBlobDataType& source)
    : ::IOcaLiteMarshal(source),
      m_value(source.m_value)
{
}

OcaLiteBlobDataType::~OcaLiteBlobDataType()
{
}

::OcaLiteBlobDataType& OcaLiteBlobDataType::operator=(const ::OcaLiteBlobDataType& source)
{
    if (this != &source)
    {
        m_value = source.m_value;
    }

    return *this;
}

bool OcaLiteBlobDataType::operator==(const ::OcaLiteBlobDataType& rhs) const
{
    return (m_value == rhs.m_value);
}

bool OcaLiteBlobDataType::operator!=(const ::OcaLiteBlobDataType& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteBlobDataType::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    m_value.Marshal(destination, writer);
}

bool OcaLiteBlobDataType::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(m_value.Unmarshal(bytesLeft, source, reader));

    if (!result)
    {
        m_value = ::OcaLiteBlob();
    }

    return result;
}

::OcaUint32 OcaLiteBlobDataType::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(m_value.GetSize(writer));

    return length;
}
