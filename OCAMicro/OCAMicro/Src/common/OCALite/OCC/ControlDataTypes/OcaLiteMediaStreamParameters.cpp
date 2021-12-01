/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteMediaStreamParameters
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteMediaStreamParameters.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMediaStreamParameters::OcaLiteMediaStreamParameters()
    : ::IOcaLiteMarshal(),
      m_value()
{
}

OcaLiteMediaStreamParameters::OcaLiteMediaStreamParameters(const ::OcaLiteBlob& value)
    : ::IOcaLiteMarshal(),
      m_value(value)
{
}

OcaLiteMediaStreamParameters::OcaLiteMediaStreamParameters(const ::OcaLiteMediaStreamParameters& source)
    : ::IOcaLiteMarshal(source),
      m_value(source.m_value)
{
}

OcaLiteMediaStreamParameters::~OcaLiteMediaStreamParameters()
{
}

::OcaLiteMediaStreamParameters& OcaLiteMediaStreamParameters::operator=(const ::OcaLiteMediaStreamParameters& source)
{
    if (this != &source)
    {
        m_value = source.m_value;
    }

    return *this;
}

bool OcaLiteMediaStreamParameters::operator==(const ::OcaLiteMediaStreamParameters& rhs) const
{
    return (m_value == rhs.m_value);
}

bool OcaLiteMediaStreamParameters::operator!=(const ::OcaLiteMediaStreamParameters& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteMediaStreamParameters::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    m_value.Marshal(destination, writer);
}

bool OcaLiteMediaStreamParameters::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(m_value.Unmarshal(bytesLeft, source, reader));

    if (!result)
    {
        m_value = ::OcaLiteBlob();
    }

    return result;
}

::OcaUint32 OcaLiteMediaStreamParameters::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(m_value.GetSize(writer));

    return length;
}
