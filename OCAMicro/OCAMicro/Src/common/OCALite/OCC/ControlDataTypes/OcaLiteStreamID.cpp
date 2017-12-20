/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteStreamID
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteStreamID.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteStreamID::OcaLiteStreamID()
    : ::IOcaLiteMarshal(),
      m_value()
{
}

OcaLiteStreamID::OcaLiteStreamID(const ::OcaLiteBlob& value)
    : ::IOcaLiteMarshal(),
      m_value(value)
{
}

OcaLiteStreamID::OcaLiteStreamID(const ::OcaLiteStreamID& source)
    : ::IOcaLiteMarshal(source),
      m_value(source.m_value)
{
}

OcaLiteStreamID::~OcaLiteStreamID()
{
}

::OcaLiteStreamID& OcaLiteStreamID::operator=(const ::OcaLiteStreamID& source)
{
    if (this != &source)
    {
        m_value = source.m_value;
    }

    return *this;
}

bool OcaLiteStreamID::operator==(const ::OcaLiteStreamID& rhs) const
{
    return (m_value == rhs.m_value);
}

bool OcaLiteStreamID::operator!=(const ::OcaLiteStreamID& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteStreamID::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    m_value.Marshal(destination, writer);
}

bool OcaLiteStreamID::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(m_value.Unmarshal(bytesLeft, source, reader));

    if (!result)
    {
        m_value = ::OcaLiteBlob();
    }

    return result;
}

::OcaUint32 OcaLiteStreamID::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(m_value.GetSize(writer));

    return length;
}
