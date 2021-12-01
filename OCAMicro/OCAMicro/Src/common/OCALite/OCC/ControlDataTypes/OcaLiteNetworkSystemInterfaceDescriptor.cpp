/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteNetworkSystemInterfaceDescriptor
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteNetworkSystemInterfaceDescriptor.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteNetworkSystemInterfaceDescriptor::OcaLiteNetworkSystemInterfaceDescriptor()
    : ::IOcaLiteMarshal(),
      m_systemInterfaceHandle(),
      m_myNetworkAddress()
{
}

OcaLiteNetworkSystemInterfaceDescriptor::OcaLiteNetworkSystemInterfaceDescriptor(const ::OcaLiteBlob& systemInterfaceHandle,
                                                                                 const ::OcaLiteNetworkAddress& myNetworkAddress)
    : ::IOcaLiteMarshal(),
      m_systemInterfaceHandle(systemInterfaceHandle),
      m_myNetworkAddress(myNetworkAddress)
{
}

OcaLiteNetworkSystemInterfaceDescriptor::OcaLiteNetworkSystemInterfaceDescriptor(const ::OcaLiteNetworkSystemInterfaceDescriptor& source)
    : ::IOcaLiteMarshal(source),
      m_systemInterfaceHandle(source.m_systemInterfaceHandle),
      m_myNetworkAddress(source.m_myNetworkAddress)
{
}

OcaLiteNetworkSystemInterfaceDescriptor::~OcaLiteNetworkSystemInterfaceDescriptor()
{
}

::OcaLiteNetworkSystemInterfaceDescriptor& OcaLiteNetworkSystemInterfaceDescriptor::operator=(const ::OcaLiteNetworkSystemInterfaceDescriptor& source)
{
    if (this != &source)
    {
        m_systemInterfaceHandle = source.m_systemInterfaceHandle;
        m_myNetworkAddress = source.m_myNetworkAddress;
    }

    return *this;
}

bool OcaLiteNetworkSystemInterfaceDescriptor::operator==(const ::OcaLiteNetworkSystemInterfaceDescriptor& rhs) const
{
    bool result(m_systemInterfaceHandle == rhs.m_systemInterfaceHandle);
    result = result && (m_myNetworkAddress == rhs.m_myNetworkAddress);

    return result;
}

bool OcaLiteNetworkSystemInterfaceDescriptor::operator!=(const ::OcaLiteNetworkSystemInterfaceDescriptor& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteNetworkSystemInterfaceDescriptor::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    m_systemInterfaceHandle.Marshal(destination, writer);
    m_myNetworkAddress.Marshal(destination, writer);
}

bool OcaLiteNetworkSystemInterfaceDescriptor::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
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

::OcaUint32 OcaLiteNetworkSystemInterfaceDescriptor::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(m_systemInterfaceHandle.GetSize(writer));
    length += m_myNetworkAddress.GetSize(writer);

    return length;
}
