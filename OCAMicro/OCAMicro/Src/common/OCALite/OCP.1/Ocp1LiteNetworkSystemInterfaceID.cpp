/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : Ocp1LiteNetworkSystemInterfaceID
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "Ocp1LiteReader.h"
#include "Ocp1LiteWriter.h"
#include "Ocp1LiteNetworkSystemInterfaceID.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

Ocp1LiteNetworkSystemInterfaceID::Ocp1LiteNetworkSystemInterfaceID()
    : ::OcaLiteNetworkSystemInterfaceID(),
      m_interfaceIndex(static_cast< ::OcaUint32>(0)),
      m_ipAddress(),
      m_subnetMaskLength(static_cast< ::OcaUint8>(0)),
      m_defaultGateway(),
      m_dnsServer(),
      m_dnsDomainName(),
      m_linkUp(static_cast< ::OcaBoolean>(false)),
      m_adapterSpeed(static_cast< ::OcaUint64>(0)),
      m_ipParametersType(static_cast< ::Ocp1LiteIPParametersType>(0)),
      m_macAddress()
{
    UpdateBlob();
}

Ocp1LiteNetworkSystemInterfaceID::Ocp1LiteNetworkSystemInterfaceID(::OcaUint32 interfaceIndex)
    : ::OcaLiteNetworkSystemInterfaceID(),
      m_interfaceIndex(interfaceIndex),
      m_ipAddress(),
      m_subnetMaskLength(static_cast< ::OcaUint8>(0)),
      m_defaultGateway(),
      m_dnsServer(),
      m_dnsDomainName(),
      m_linkUp(static_cast< ::OcaBoolean>(false)),
      m_adapterSpeed(static_cast< ::OcaUint64>(0)),
      m_ipParametersType(static_cast< ::Ocp1LiteIPParametersType>(0)),
      m_macAddress()
{
    UpdateBlob();
}


Ocp1LiteNetworkSystemInterfaceID::Ocp1LiteNetworkSystemInterfaceID(::OcaUint32 interfaceIndex,
                                                           const ::OcaLiteString& ipAddress,
                                                           ::OcaUint8 subnetMaskLength,
                                                           const ::OcaLiteString& defaultGateway,
                                                           const ::OcaLiteString& dnsServer,
                                                           const ::OcaLiteString& dnsDomainName,
                                                           ::OcaBoolean linkUp,
                                                           ::OcaUint64 adapterSpeed,
                                                           ::Ocp1LiteIPParametersType type,
                                                           const ::OcaLiteBlobFixedLen<MAC_ADDRESS_LENGTH>& macAddress)
    : ::OcaLiteNetworkSystemInterfaceID(),
      m_interfaceIndex(interfaceIndex),
      m_ipAddress(ipAddress),
      m_subnetMaskLength(subnetMaskLength),
      m_defaultGateway(defaultGateway),
      m_dnsServer(dnsServer),
      m_dnsDomainName(dnsDomainName),
      m_linkUp(linkUp),
      m_adapterSpeed(adapterSpeed),
      m_ipParametersType(type),
      m_macAddress(macAddress)
{
    UpdateBlob();
}

Ocp1LiteNetworkSystemInterfaceID::Ocp1LiteNetworkSystemInterfaceID(const ::Ocp1LiteNetworkSystemInterfaceID& source)
    : ::OcaLiteNetworkSystemInterfaceID(source),
      m_interfaceIndex(source.m_interfaceIndex),
      m_ipAddress(source.m_ipAddress),
      m_subnetMaskLength(source.m_subnetMaskLength),
      m_defaultGateway(source.m_defaultGateway),
      m_dnsServer(source.m_dnsServer),
      m_dnsDomainName(source.m_dnsDomainName),
      m_linkUp(source.m_linkUp),
      m_adapterSpeed(source.m_adapterSpeed),
      m_ipParametersType(source.m_ipParametersType),
      m_macAddress(source.m_macAddress)
{
    UpdateBlob();
}

Ocp1LiteNetworkSystemInterfaceID::~Ocp1LiteNetworkSystemInterfaceID()
{
}

::Ocp1LiteNetworkSystemInterfaceID& Ocp1LiteNetworkSystemInterfaceID::operator=(const ::Ocp1LiteNetworkSystemInterfaceID& source)
{
    if (this != &source)
    {
        OcaLiteNetworkSystemInterfaceID::operator=(source);
        m_interfaceIndex = source.m_interfaceIndex;
        m_ipAddress = source.m_ipAddress;
        m_subnetMaskLength = source.m_subnetMaskLength;
        m_defaultGateway = source.m_defaultGateway;
        m_dnsServer = source.m_dnsServer;
        m_dnsDomainName = source.m_dnsDomainName;
        m_linkUp = source.m_linkUp;
        m_adapterSpeed = source.m_adapterSpeed;
        m_ipParametersType = source.m_ipParametersType;
        m_macAddress = source.m_macAddress;
    }

    return *this;
}

::OcaUint32 Ocp1LiteNetworkSystemInterfaceID::GetInterfaceIndex() const
{
    return m_interfaceIndex;
}

const ::OcaLiteString& Ocp1LiteNetworkSystemInterfaceID::GetIPAddress() const
{
    return m_ipAddress;
}

::OcaUint8 Ocp1LiteNetworkSystemInterfaceID::GetSubnetMaskLength() const
{
    return m_subnetMaskLength;
}

const ::OcaLiteString& Ocp1LiteNetworkSystemInterfaceID::GetDefaultGateway() const
{
    return m_defaultGateway;
}

const ::OcaLiteString& Ocp1LiteNetworkSystemInterfaceID::GetDnsServer() const
{
    return m_dnsServer;
}

const ::OcaLiteString& Ocp1LiteNetworkSystemInterfaceID::GetDnsDomainName() const
{
    return m_dnsDomainName;
}

::OcaBoolean Ocp1LiteNetworkSystemInterfaceID::GetLinkUp() const
{
    return m_linkUp;
}

::OcaUint64 Ocp1LiteNetworkSystemInterfaceID::GetAdapterSpeed() const
{
    return m_adapterSpeed;
}

::Ocp1LiteIPParametersType Ocp1LiteNetworkSystemInterfaceID::GetIPParametersType() const
{
    return m_ipParametersType;
}

const ::OcaLiteBlobFixedLen<MAC_ADDRESS_LENGTH>& Ocp1LiteNetworkSystemInterfaceID::GetMacAddress() const
{
    return m_macAddress;
}

::Ocp1LiteNetworkSystemInterfaceID* Ocp1LiteNetworkSystemInterfaceID::CreateFromBase(const ::OcaLiteNetworkSystemInterfaceID& baseClass)
{
    ::Ocp1LiteNetworkSystemInterfaceID* result(NULL);    

    ::OcaUint32 interfaceIndex(static_cast< ::OcaUint32>(0));
    ::OcaLiteString ipAddress;
    ::OcaUint8 subnetMaskLength(static_cast< ::OcaUint8>(0));
    ::OcaLiteString defaultGateway;
    ::OcaLiteString dnsServer;
    ::OcaLiteString dnsDomainName;
    ::OcaBoolean bLinkUp(static_cast< ::OcaBoolean>(false));
    ::OcaUint64 adapterSpeed(static_cast< ::OcaUint64>(0));
    ::Ocp1LiteIPParametersType ipParametersType(OCP1IPPARAMETERS_TYPE_UNKNOWN);
    ::OcaLiteBlobFixedLen<MAC_ADDRESS_LENGTH> macAddress;

    if (GetValuesFromBase(baseClass, interfaceIndex, ipAddress, subnetMaskLength, defaultGateway, dnsServer, dnsDomainName, 
                          bLinkUp, adapterSpeed, ipParametersType, macAddress))
    {
        result = new ::Ocp1LiteNetworkSystemInterfaceID(interfaceIndex,
                                                    ipAddress,
                                                    subnetMaskLength,
                                                    defaultGateway,
                                                    dnsServer,
                                                    dnsDomainName,
                                                    bLinkUp,
                                                    adapterSpeed,
                                                    ipParametersType,
                                                    macAddress);
    }
    else
    {
        OCA_LOG_WARNING("Network System Interface ID not an Ocp1LiteNetworkSystemInterfaceID.");
    }

    return result;
}

bool Ocp1LiteNetworkSystemInterfaceID::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool success(OcaLiteNetworkSystemInterfaceID::Unmarshal(bytesLeft, source, reader));
    success = success && GetValuesFromBase(*this, m_interfaceIndex, m_ipAddress, m_subnetMaskLength, m_defaultGateway, m_dnsServer,
                                           m_dnsDomainName, m_linkUp, m_adapterSpeed, m_ipParametersType, m_macAddress);
    return success;
}

bool Ocp1LiteNetworkSystemInterfaceID::GetValuesFromBase(const ::OcaLiteNetworkSystemInterfaceID& baseClass, ::OcaUint32& interfaceIndex, ::OcaLiteString& ipAddress,
                                  ::OcaUint8& subnetMaskLength, ::OcaLiteString& defaultGateway, ::OcaLiteString& dnsServer,
                                  ::OcaLiteString& dnsDomainName, ::OcaBoolean& bLinkUp, ::OcaUint64& adapterSpeed,
                                  ::Ocp1LiteIPParametersType& ipParametersType, ::OcaLiteBlobFixedLen<MAC_ADDRESS_LENGTH>& macAddress)
{
    ::OcaUint32 bytesLeft(static_cast< ::OcaUint32>(baseClass.GetMyNetworkAddress().GetValue().GetDataSize()));

    ::Ocp1LiteReader reader;
    const ::OcaUint8* buffer(baseClass.GetMyNetworkAddress().GetValue().GetData());
    bool success(ipAddress.Unmarshal(bytesLeft, &buffer, reader));

    if (success && (static_cast< ::OcaUint32>(0) == bytesLeft))
    {
        bytesLeft = static_cast< ::OcaUint32>(baseClass.GetSystemInterfaceHandle().GetDataSize());
        buffer = baseClass.GetSystemInterfaceHandle().GetData();

        success = success && reader.Read(bytesLeft, &buffer, interfaceIndex);
        success = success && reader.Read(bytesLeft, &buffer, subnetMaskLength);
        success = success && defaultGateway.Unmarshal(bytesLeft, &buffer, reader);
        success = success && dnsServer.Unmarshal(bytesLeft, &buffer, reader);
        success = success && dnsDomainName.Unmarshal(bytesLeft, &buffer, reader);
        success = success && reader.Read(bytesLeft, &buffer, bLinkUp);
        success = success && reader.Read(bytesLeft, &buffer, adapterSpeed);
        success = success && UnmarshalValue< ::Ocp1LiteIPParametersType>(ipParametersType, bytesLeft, &buffer, reader);
        success = success && macAddress.Unmarshal(bytesLeft, &buffer, reader);
        success = success && (static_cast< ::OcaUint32>(0) == bytesLeft);
    }
    return success;
}

void Ocp1LiteNetworkSystemInterfaceID::UpdateBlob()
{
    ::Ocp1LiteWriter writer;
    ::OcaUint32 size(m_ipAddress.GetSize(writer));
    ::OcaUint8* buffer(new ::OcaUint8[static_cast<size_t>(size)]);
    ::OcaUint8* pBuffer(buffer);

    m_ipAddress.Marshal(&pBuffer, writer);

    ::OcaLiteNetworkAddress networkAddress(::OcaLiteBlob(static_cast< ::OcaUint16>(size), buffer));
    OcaLiteNetworkSystemInterfaceID::SetNetworkAddress(networkAddress);

    delete [] buffer;

    size = (writer.GetSize(m_interfaceIndex) + writer.GetSize(m_subnetMaskLength) +
            m_defaultGateway.GetSize(writer) + m_dnsServer.GetSize(writer) +
            m_dnsDomainName.GetSize(writer) + writer.GetSize(m_linkUp) +
            writer.GetSize(m_adapterSpeed) + GetSizeValue< ::Ocp1LiteIPParametersType>(m_ipParametersType, writer) +
            m_macAddress.GetSize(writer));

    buffer = new ::OcaUint8[static_cast<size_t>(size)];
    pBuffer = &buffer[0];

    writer.Write(m_interfaceIndex, &pBuffer);
    writer.Write(m_subnetMaskLength, &pBuffer);
    m_defaultGateway.Marshal(&pBuffer, writer);
    m_dnsServer.Marshal(&pBuffer, writer);
    m_dnsDomainName.Marshal(&pBuffer, writer);
    writer.Write(m_linkUp, &pBuffer);
    writer.Write(m_adapterSpeed, &pBuffer);
    MarshalValue< ::Ocp1LiteIPParametersType>(m_ipParametersType, &pBuffer, writer);
    m_macAddress.Marshal(&pBuffer, writer);

    ::OcaLiteBlob systemInterfaceHandle(static_cast< ::OcaUint16>(size), buffer);
    OcaLiteNetworkSystemInterfaceID::SetSystemInterfaceHandle(systemInterfaceHandle);

    delete [] buffer;
}

// ---- Function Implementation ----

template <>
void MarshalValue< ::Ocp1LiteIPParametersType>(const ::Ocp1LiteIPParametersType& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
bool UnmarshalValue< ::Ocp1LiteIPParametersType>(::Ocp1LiteIPParametersType& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 ipParametersType(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, ipParametersType));
    if (result)
    {
        if (ipParametersType < static_cast< ::OcaUint8>(OCP1IPPARAMETERS_TYPE_MAXIMUM))
        {
            value = static_cast< ::Ocp1LiteIPParametersType>(ipParametersType);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
::OcaUint32 GetSizeValue< ::Ocp1LiteIPParametersType>(const ::Ocp1LiteIPParametersType& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}
