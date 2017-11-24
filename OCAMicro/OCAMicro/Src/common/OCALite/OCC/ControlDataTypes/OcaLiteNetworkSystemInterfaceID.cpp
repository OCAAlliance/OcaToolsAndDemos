/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteNetworkSystemInterfaceID
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteNetworkSystemInterfaceID.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteNetworkSystemInterfaceID::OcaLiteNetworkSystemInterfaceID()
    : ::IOcaLiteMarshal(),
      m_systemInterfaceHandle(),
      m_myNetworkAddress()
{
}

OcaLiteNetworkSystemInterfaceID::OcaLiteNetworkSystemInterfaceID(const ::OcaLiteBlob& systemInterfaceHandle,
                                                         const ::OcaLiteNetworkAddress& myNetworkAddress)
    : ::IOcaLiteMarshal(),
      m_systemInterfaceHandle(systemInterfaceHandle),
      m_myNetworkAddress(myNetworkAddress)
{
}

OcaLiteNetworkSystemInterfaceID::OcaLiteNetworkSystemInterfaceID(const ::OcaLiteNetworkSystemInterfaceID& source)
    : ::IOcaLiteMarshal(source),
      m_systemInterfaceHandle(source.m_systemInterfaceHandle),
      m_myNetworkAddress(source.m_myNetworkAddress)
{
}

OcaLiteNetworkSystemInterfaceID::~OcaLiteNetworkSystemInterfaceID()
{
}

::OcaLiteNetworkSystemInterfaceID& OcaLiteNetworkSystemInterfaceID::operator=(const ::OcaLiteNetworkSystemInterfaceID& source)
{
    if (this != &source)
    {
        m_systemInterfaceHandle = source.m_systemInterfaceHandle;
        m_myNetworkAddress = source.m_myNetworkAddress;
    }

    return *this;
}

bool OcaLiteNetworkSystemInterfaceID::operator==(const ::OcaLiteNetworkSystemInterfaceID& rhs) const
{
    bool result(m_systemInterfaceHandle == rhs.m_systemInterfaceHandle);
    result = result && (m_myNetworkAddress == rhs.m_myNetworkAddress);

    return result;
}

bool OcaLiteNetworkSystemInterfaceID::operator!=(const ::OcaLiteNetworkSystemInterfaceID& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteNetworkSystemInterfaceID::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    m_systemInterfaceHandle.Marshal(destination, writer);
    m_myNetworkAddress.Marshal(destination, writer);
}

bool OcaLiteNetworkSystemInterfaceID::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(m_systemInterfaceHandle.Unmarshal(bytesLeft, source, reader));
    result = result && m_myNetworkAddress.Unmarshal(bytesLeft, source, reader);

    if (!result)
    {
        m_systemInterfaceHandle = ::OcaLiteBlob();
        m_myNetworkAddress = ::OcaLiteNetworkAddress();
    }

    return result;
}

::OcaUint32 OcaLiteNetworkSystemInterfaceID::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(m_systemInterfaceHandle.GetSize(writer));
    length += m_myNetworkAddress.GetSize(writer);

    return length;
}
