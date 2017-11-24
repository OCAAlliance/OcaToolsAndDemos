/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteNetworkSignalChannelID
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteNetworkSignalChannelID.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteNetworkSignalChannelID::OcaLiteNetworkSignalChannelID()
    : ::IOcaLiteMarshal(),
      m_value()
{
}

OcaLiteNetworkSignalChannelID::OcaLiteNetworkSignalChannelID(const ::OcaLiteBlob& value)
    : ::IOcaLiteMarshal(),
      m_value(value)
{
}

OcaLiteNetworkSignalChannelID::OcaLiteNetworkSignalChannelID(const ::OcaLiteNetworkSignalChannelID& source)
    : ::IOcaLiteMarshal(source),
      m_value(source.m_value)
{
}

OcaLiteNetworkSignalChannelID::~OcaLiteNetworkSignalChannelID()
{
}

::OcaLiteNetworkSignalChannelID& OcaLiteNetworkSignalChannelID::operator=(const ::OcaLiteNetworkSignalChannelID& source)
{
    if (this != &source)
    {
        m_value = source.m_value;
    }

    return *this;
}

bool OcaLiteNetworkSignalChannelID::operator==(const ::OcaLiteNetworkSignalChannelID& rhs) const
{
    return (m_value == rhs.m_value);
}

bool OcaLiteNetworkSignalChannelID::operator!=(const ::OcaLiteNetworkSignalChannelID& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteNetworkSignalChannelID::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    m_value.Marshal(destination, writer);
}

bool OcaLiteNetworkSignalChannelID::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(m_value.Unmarshal(bytesLeft, source, reader));

    if (!result)
    {
        m_value = ::OcaLiteBlob();
    }

    return result;
}

::OcaUint32 OcaLiteNetworkSignalChannelID::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(m_value.GetSize(writer));

    return length;
}
