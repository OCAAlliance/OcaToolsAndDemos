/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteStreamParameters
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteStreamParameters.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteStreamParameters::OcaLiteStreamParameters()
    : ::IOcaLiteMarshal(),
      m_value()
{
}

OcaLiteStreamParameters::OcaLiteStreamParameters(const ::OcaLiteBlob& value)
    : ::IOcaLiteMarshal(),
      m_value(value)
{
}

OcaLiteStreamParameters::OcaLiteStreamParameters(const ::OcaLiteStreamParameters& source)
    : ::IOcaLiteMarshal(source),
      m_value(source.m_value)
{
}

OcaLiteStreamParameters::~OcaLiteStreamParameters()
{
}

::OcaLiteStreamParameters& OcaLiteStreamParameters::operator=(const ::OcaLiteStreamParameters& source)
{
    if (this != &source)
    {
        m_value = source.m_value;
    }

    return *this;
}

bool OcaLiteStreamParameters::operator==(const ::OcaLiteStreamParameters& rhs) const
{
    return (m_value == rhs.m_value);
}

bool OcaLiteStreamParameters::operator!=(const ::OcaLiteStreamParameters& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteStreamParameters::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    m_value.Marshal(destination, writer);
}

bool OcaLiteStreamParameters::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(m_value.Unmarshal(bytesLeft, source, reader));

    if (!result)
    {
        m_value = ::OcaLiteBlob();
    }

    return result;
}

::OcaUint32 OcaLiteStreamParameters::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(m_value.GetSize(writer));

    return length;
}
