/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteStreamConnectorID
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteStreamConnectorID.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteStreamConnectorID::OcaLiteStreamConnectorID()
    : ::IOcaLiteMarshal(),
      m_value()
{
}

OcaLiteStreamConnectorID::OcaLiteStreamConnectorID(const ::OcaLiteBlob& value)
    : ::IOcaLiteMarshal(),
      m_value(value)
{
}

OcaLiteStreamConnectorID::OcaLiteStreamConnectorID(const ::OcaLiteStreamConnectorID& source)
    : ::IOcaLiteMarshal(source),
      m_value(source.m_value)
{
}

OcaLiteStreamConnectorID::~OcaLiteStreamConnectorID()
{
}

::OcaLiteStreamConnectorID& OcaLiteStreamConnectorID::operator=(const ::OcaLiteStreamConnectorID& source)
{
    if (this != &source)
    {
        m_value = source.m_value;
    }

    return *this;
}

bool OcaLiteStreamConnectorID::operator==(const ::OcaLiteStreamConnectorID& rhs) const
{
    return (m_value == rhs.m_value);
}

bool OcaLiteStreamConnectorID::operator!=(const ::OcaLiteStreamConnectorID& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteStreamConnectorID::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    m_value.Marshal(destination, writer);
}

bool OcaLiteStreamConnectorID::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(m_value.Unmarshal(bytesLeft, source, reader));

    if (!result)
    {
        m_value = ::OcaLiteBlob();
    }

    return result;
}

::OcaUint32 OcaLiteStreamConnectorID::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(m_value.GetSize(writer));

    return length;
}
