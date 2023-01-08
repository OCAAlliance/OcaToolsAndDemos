/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteNetworkAddress
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteNetworkAddress.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteNetworkAddress::OcaLiteNetworkAddress()
    : ::IOcaLiteMarshal(),
      m_value()
{
}

OcaLiteNetworkAddress::OcaLiteNetworkAddress(const ::OcaLiteBlob& value)
    : ::IOcaLiteMarshal(),
      m_value(value)
{
}

OcaLiteNetworkAddress::OcaLiteNetworkAddress(const ::OcaLiteNetworkAddress& source)
    : ::IOcaLiteMarshal(source),
      m_value(source.m_value)
{
}

OcaLiteNetworkAddress::~OcaLiteNetworkAddress()
{
}

::OcaLiteNetworkAddress& OcaLiteNetworkAddress::operator=(const ::OcaLiteNetworkAddress& source)
{
    if (this != &source)
    {
        m_value = source.m_value;
    }

    return *this;
}

bool OcaLiteNetworkAddress::operator==(const ::OcaLiteNetworkAddress& rhs) const
{
    return (m_value == rhs.m_value);
}

bool OcaLiteNetworkAddress::operator!=(const ::OcaLiteNetworkAddress& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteNetworkAddress::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    m_value.Marshal(destination, writer);
}

bool OcaLiteNetworkAddress::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(m_value.Unmarshal(bytesLeft, source, reader));

    if (!result)
    {
        m_value = ::OcaLiteBlob();
    }

    return result;
}

::OcaUint32 OcaLiteNetworkAddress::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(m_value.GetSize(writer));

    return length;
}
