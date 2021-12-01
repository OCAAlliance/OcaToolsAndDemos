/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : Ocp1LiteNetworkAddress
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "Ocp1LiteReader.h"
#include "Ocp1LiteWriter.h"
#include "Ocp1LiteNetworkAddress.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

Ocp1LiteNetworkAddress::Ocp1LiteNetworkAddress()
    : ::OcaLiteNetworkAddress(),
      m_destHostOrIPAddress(),
      m_port(static_cast< ::OcaUint16>(0))
{
    UpdateBlob();
}

Ocp1LiteNetworkAddress::Ocp1LiteNetworkAddress(const ::OcaLiteString& destHostOrIPAddress,
                                       ::OcaUint16 port)
    : ::OcaLiteNetworkAddress(),
      m_destHostOrIPAddress(destHostOrIPAddress),
      m_port(port)
{
    UpdateBlob();
}

Ocp1LiteNetworkAddress::Ocp1LiteNetworkAddress(const ::Ocp1LiteNetworkAddress& source)
    : ::OcaLiteNetworkAddress(source),
      m_destHostOrIPAddress(source.m_destHostOrIPAddress),
      m_port(source.m_port)
{
    UpdateBlob();
}

Ocp1LiteNetworkAddress::~Ocp1LiteNetworkAddress()
{
}

const ::OcaLiteString& Ocp1LiteNetworkAddress::GetDestHostOrIPAddress() const
{
    return m_destHostOrIPAddress;
}

::OcaUint16 Ocp1LiteNetworkAddress::GetPort() const
{
    return m_port;
}

::Ocp1LiteNetworkAddress& Ocp1LiteNetworkAddress::operator=(const ::Ocp1LiteNetworkAddress& source)
{
    if (this != &source)
    {
        OcaLiteNetworkAddress::operator=(source);
        m_destHostOrIPAddress = source.m_destHostOrIPAddress;
        m_port = source.m_port;
    }

    return *this;
}

::Ocp1LiteNetworkAddress* Ocp1LiteNetworkAddress::CreateFromBase(const ::OcaLiteNetworkAddress& baseClass)
{
    ::Ocp1LiteNetworkAddress* result(NULL);
    ::OcaLiteString destHostOrIPAddress;
    ::OcaUint16 port;

    if (GetValuesFromBase(baseClass, destHostOrIPAddress, port))
    {
        result = new ::Ocp1LiteNetworkAddress(destHostOrIPAddress, port);
    }
    else
    {
        OCA_LOG_WARNING("Network address not an Ocp1LiteNetworkAddress");
    }

    return result;
}

bool Ocp1LiteNetworkAddress::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool success(OcaLiteNetworkAddress::Unmarshal(bytesLeft, source, reader));
    success = success && GetValuesFromBase(*this, m_destHostOrIPAddress, m_port);
    return success;
}

bool Ocp1LiteNetworkAddress::GetValuesFromBase(const ::OcaLiteNetworkAddress& baseClass, ::OcaLiteString& destHostOrIp, ::OcaUint16& port)
{
    ::OcaUint32 bytesLeft(static_cast< ::OcaUint32>(baseClass.GetValue().GetDataSize()));

    ::Ocp1LiteReader reader;
    const ::OcaUint8* buffer(baseClass.GetValue().GetData());
    bool success(destHostOrIp.Unmarshal(bytesLeft, &buffer, reader));
    success = success && reader.Read(bytesLeft, &buffer, port);

    return success;
}

void Ocp1LiteNetworkAddress::UpdateBlob()
{
    ::Ocp1LiteWriter writer;
    ::OcaUint32 size(m_destHostOrIPAddress.GetSize(writer) + writer.GetSize(m_port));
    ::OcaUint8* buffer(new ::OcaUint8[static_cast<size_t>(size)]);
    ::OcaUint8* pBuffer(buffer);

    m_destHostOrIPAddress.Marshal(&pBuffer, writer);
    writer.Write(m_port, &pBuffer);

    ::OcaLiteBlob ocaBlob(static_cast< ::OcaUint16>(size), buffer);
    OcaLiteNetworkAddress::SetValue(ocaBlob);

    delete[] buffer;
}
