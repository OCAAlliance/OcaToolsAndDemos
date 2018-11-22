/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteNetworkNodeID
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteNetworkNodeID.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteNetworkNodeID::OcaLiteNetworkNodeID()
    : ::IOcaLiteMarshal(),
      m_value()
{
}

OcaLiteNetworkNodeID::OcaLiteNetworkNodeID(const ::OcaLiteBlob& value)
    : ::IOcaLiteMarshal(),
      m_value(value)
{
}

OcaLiteNetworkNodeID::OcaLiteNetworkNodeID(const ::OcaLiteNetworkNodeID& source)
    : ::IOcaLiteMarshal(source),
      m_value(source.m_value)
{
}

OcaLiteNetworkNodeID::~OcaLiteNetworkNodeID()
{
}

::OcaLiteNetworkNodeID& OcaLiteNetworkNodeID::operator=(const ::OcaLiteNetworkNodeID& source)
{
    if (this != &source)
    {
        m_value = source.m_value;
    }

    return *this;
}

bool OcaLiteNetworkNodeID::operator==(const ::OcaLiteNetworkNodeID& rhs) const
{
    return (m_value == rhs.m_value);
}

bool OcaLiteNetworkNodeID::operator!=(const ::OcaLiteNetworkNodeID& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteNetworkNodeID::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    m_value.Marshal(destination, writer);
}

bool OcaLiteNetworkNodeID::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(m_value.Unmarshal(bytesLeft, source, reader));

    if (!result)
    {
        m_value = ::OcaLiteBlob();
    }

    return result;
}

::OcaUint32 OcaLiteNetworkNodeID::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(m_value.GetSize(writer));

    return length;
}
