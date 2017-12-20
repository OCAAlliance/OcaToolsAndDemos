/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteAES67NetworkAddress
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteAES67NetworkAddress.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteAES67NetworkAddress::OcaLiteAES67NetworkAddress()
    : ::OcaLiteNetworkAddress(),
      m_destHostOrIPAddress(),
      m_port(static_cast< ::OcaUint16>(0))
{
    UpdateBlob();
}

OcaLiteAES67NetworkAddress::OcaLiteAES67NetworkAddress(const ::OcaLiteString& destHostOrIPAddress,
                                       ::OcaUint16 port)
    : ::OcaLiteNetworkAddress(),
      m_destHostOrIPAddress(destHostOrIPAddress),
      m_port(port)
{
    UpdateBlob();
}

OcaLiteAES67NetworkAddress::OcaLiteAES67NetworkAddress(const ::OcaLiteAES67NetworkAddress& source)
    : ::OcaLiteNetworkAddress(source),
      m_destHostOrIPAddress(source.m_destHostOrIPAddress),
      m_port(source.m_port)
{
    UpdateBlob();
}

OcaLiteAES67NetworkAddress::~OcaLiteAES67NetworkAddress()
{
}

const ::OcaLiteString& OcaLiteAES67NetworkAddress::GetDestHostOrIPAddress() const
{
    return m_destHostOrIPAddress;
}

::OcaUint16 OcaLiteAES67NetworkAddress::GetPort() const
{
    return m_port;
}

::OcaLiteAES67NetworkAddress& OcaLiteAES67NetworkAddress::operator=(const ::OcaLiteAES67NetworkAddress& source)
{
    if (this != &source)
    {
        OcaLiteNetworkAddress::operator=(source);
        m_destHostOrIPAddress = source.m_destHostOrIPAddress;
        m_port = source.m_port;
    }

    return *this;
}

::OcaLiteAES67NetworkAddress* OcaLiteAES67NetworkAddress::CreateFromBase(const ::OcaLiteNetworkAddress& baseClass)
{
    ::OcaLiteAES67NetworkAddress* result(NULL);
    ::OcaLiteString destHostOrIPAddress;
    ::OcaUint16 port;

    if (GetValuesFromBase(baseClass, destHostOrIPAddress, port))
    {
        result = new ::OcaLiteAES67NetworkAddress(destHostOrIPAddress, port);
    }
    else
    {
        OCA_LOG_WARNING("Network address not an OcaLiteAES67NetworkAddress");
    }

    return result;
}

bool OcaLiteAES67NetworkAddress::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool success(OcaLiteNetworkAddress::Unmarshal(bytesLeft, source, reader));
    success = success && GetValuesFromBase(*this, m_destHostOrIPAddress, m_port);
    return success;
}

bool OcaLiteAES67NetworkAddress::GetValuesFromBase(const ::OcaLiteNetworkAddress& baseClass, ::OcaLiteString& destHostOrIp, ::OcaUint16& port)
{
    bool success(false);

    const ::OcaUint8* buffer(baseClass.GetValue().GetData());

    if (baseClass.GetValue().GetDataSize() >= static_cast< ::OcaUint16>(sizeof(UINT16)))
    {
        UINT16 length(0);

        for (unsigned int byteNr(0); byteNr < sizeof(UINT16); byteNr++)
        {
            length <<= 8;
            length |= static_cast<UINT16>(*buffer);
            buffer++;
        }

        std::string value;

        static_cast<void>(value.assign(reinterpret_cast<const char*>(buffer), length));

        buffer += length;

        UINT16 valuePort(0);
        for (unsigned int byteNr(0); byteNr < sizeof(UINT16); byteNr++)
        {
            valuePort <<= 8;
            valuePort |= static_cast<UINT16>(*buffer);
            buffer++;
        }

        destHostOrIp = ::OcaLiteString(value.c_str());
        port = static_cast< ::OcaUint16>(valuePort);

        success = true;
    }

    return success;
}

void OcaLiteAES67NetworkAddress::UpdateBlob()
{
    const std::string value(m_destHostOrIPAddress.GetString());

    UINT16 length(static_cast<UINT16>(value.length()));
    UINT16 totSize(length + sizeof(UINT16) + sizeof(UINT16));

    ::OcaUint8* buffer(new ::OcaUint8[static_cast<size_t>(totSize)]);
    ::OcaUint8* pBuffer(buffer);

    // Write the length of the string
    *pBuffer = static_cast< ::OcaUint8>((length >> 8) & 0x00FF);
    (pBuffer)++;
    *pBuffer = static_cast< ::OcaUint8>(length & 0x00FF);
    (pBuffer)++;

    // Write the string itself
    ::strncpy(reinterpret_cast<char *>(pBuffer), value.c_str(), length);
    pBuffer += length;

    // Write the port 
    *pBuffer = static_cast< ::OcaUint8>((static_cast<UINT16>(m_port) >> 8) & 0x00FF);
    (pBuffer)++;
    *pBuffer = static_cast< ::OcaUint8>(static_cast<UINT16>(m_port)& 0x00FF);

    ::OcaLiteBlob ocaBlob(static_cast< ::OcaUint16>(totSize), buffer);
    OcaLiteNetworkAddress::SetValue(ocaBlob);

    delete[] buffer;
}
