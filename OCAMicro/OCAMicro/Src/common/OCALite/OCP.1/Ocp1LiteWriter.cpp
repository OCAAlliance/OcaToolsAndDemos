/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : Ocp1LiteWriter
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCP.1/Ocp1LiteHostInterface.h>

// ---- Include local include files ----
#include "Ocp1LiteWriter.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

void Ocp1LiteWriter::Write(::OcaBoolean value, ::OcaUint8** destination) const
{
    Write(static_cast< ::OcaUint8>(value), destination);
}

void Ocp1LiteWriter::Write(::OcaInt8 value, ::OcaUint8** destination) const
{
    ::OcaInt8* pValue(reinterpret_cast< ::OcaInt8*>(*destination));
    *pValue = value;
    (*destination) += sizeof(value);
}

void Ocp1LiteWriter::Write(::OcaInt16 value, ::OcaUint8** destination) const
{
    UINT16 rawValue(::ConvertFromHostByteOrderToNetworkOrder(static_cast<UINT16>(value)));
    ::memcpy(*destination, &rawValue, sizeof(rawValue));
    (*destination) += sizeof(value);
}

void Ocp1LiteWriter::Write(::OcaInt32 value, ::OcaUint8** destination) const
{
    UINT32 rawValue(::ConvertFromHostByteOrderToNetworkOrder(static_cast<UINT32>(value)));
    ::memcpy(*destination, &rawValue, sizeof(rawValue));
    (*destination) += sizeof(value);
}

void Ocp1LiteWriter::Write(::OcaInt64 value, ::OcaUint8** destination) const
{
    UINT64 rawValue(::ConvertFromHostByteOrderToNetworkOrder(static_cast<UINT64>(value)));
    ::memcpy(*destination, &rawValue, sizeof(rawValue));
    (*destination) += sizeof(value);
}

void Ocp1LiteWriter::Write(::OcaUint8 value, ::OcaUint8** destination) const
{
    ::OcaUint8* pValue(reinterpret_cast< ::OcaUint8*>(*destination));
    *pValue = value;
    (*destination) += sizeof(value);
}

void Ocp1LiteWriter::Write(::OcaUint16 value, ::OcaUint8** destination) const
{
    UINT16 rawValue(::ConvertFromHostByteOrderToNetworkOrder(static_cast<UINT16>(value)));
    ::memcpy(*destination, &rawValue, sizeof(rawValue));
    (*destination) += sizeof(value);
}

void Ocp1LiteWriter::Write(::OcaUint32 value, ::OcaUint8** destination) const
{
    UINT32 rawValue(::ConvertFromHostByteOrderToNetworkOrder(static_cast<UINT32>(value)));
    ::memcpy(*destination, &rawValue, sizeof(rawValue));
    (*destination) += sizeof(value);
}

void Ocp1LiteWriter::Write(::OcaUint64 value, ::OcaUint8** destination) const
{
    UINT64 rawValue(::ConvertFromHostByteOrderToNetworkOrder(static_cast<UINT64>(value)));
    ::memcpy(*destination, &rawValue, sizeof(rawValue));
    (*destination) += sizeof(value);
}

void Ocp1LiteWriter::Write(::OcaFloat32 value, ::OcaUint8** destination) const
{
    UINT32 tmpValue(::ConvertFromHostByteOrderToNetworkOrder(*reinterpret_cast<UINT32*>(&value)));
    ::memcpy(*destination, &tmpValue, sizeof(tmpValue));
    (*destination) += sizeof(value);
}

void Ocp1LiteWriter::Write(const ::OcaUint8* values, ::OcaUint32 nrOfBytes, ::OcaUint8** destination) const
{
    ::memcpy(*destination, values, static_cast<size_t>(nrOfBytes));
    (*destination) += nrOfBytes;
}

::OcaUint32 Ocp1LiteWriter::GetSize(::OcaBoolean value) const
{
    return static_cast< ::OcaUint32>(1); // OCP.1 defines that an OcaBoolean in an OCP.1 message has a size of one byte independent of the effective size of an OcaBoolean in the target platform
}

::OcaUint32 Ocp1LiteWriter::GetSize(::OcaInt8 value) const
{
    return static_cast< ::OcaUint32>(sizeof(value));
}

::OcaUint32 Ocp1LiteWriter::GetSize(::OcaInt16 value) const
{
    return static_cast< ::OcaUint32>(sizeof(value));
}

::OcaUint32 Ocp1LiteWriter::GetSize(::OcaInt32 value) const
{
    return static_cast< ::OcaUint32>(sizeof(value));
}

::OcaUint32 Ocp1LiteWriter::GetSize(::OcaInt64 value) const
{
    return static_cast< ::OcaUint32>(sizeof(value));
}

::OcaUint32 Ocp1LiteWriter::GetSize(::OcaUint8 value) const
{
    return static_cast< ::OcaUint32>(sizeof(value));
}

::OcaUint32 Ocp1LiteWriter::GetSize(::OcaUint16 value) const
{
    return static_cast< ::OcaUint32>(sizeof(value));
}

::OcaUint32 Ocp1LiteWriter::GetSize(::OcaUint32 value) const
{
    return static_cast< ::OcaUint32>(sizeof(value));
}

::OcaUint32 Ocp1LiteWriter::GetSize(::OcaUint64 value) const
{
    return static_cast< ::OcaUint32>(sizeof(value));
}

::OcaUint32 Ocp1LiteWriter::GetSize(::OcaFloat32 value) const
{
    return static_cast< ::OcaUint32>(sizeof(value));
}

::OcaUint32 Ocp1LiteWriter::GetSize(const ::OcaUint8* values, ::OcaUint32 nrOfBytes) const
{
    return nrOfBytes;
}
