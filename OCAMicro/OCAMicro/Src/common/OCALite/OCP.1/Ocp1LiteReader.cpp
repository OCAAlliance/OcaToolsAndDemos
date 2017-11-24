/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : Ocp1LiteReader
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCP.1/Ocp1LiteHostInterface.h>

// ---- Include local include files ----
#include "Ocp1LiteReader.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

bool Ocp1LiteReader::Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaBoolean& value) const
{
    bool success(bytesLeft >= static_cast< ::OcaUint32>(sizeof(::OcaUint8)));

    if (success)
    {
        value = ((**source > static_cast< ::OcaUint8>(0)) ? static_cast< ::OcaBoolean>(true) : static_cast< ::OcaBoolean>(false));
        (*source) += 1; // OCP.1 defines that an OcaBoolean in an OCP.1 message has a size of one byte independent of the effective size of an OcaBoolean in the target platform
        bytesLeft -= static_cast< ::OcaUint32>(1);
    }

    return success;
}

bool Ocp1LiteReader::Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaInt8& value) const
{
    bool success(bytesLeft >= static_cast< ::OcaUint32>(sizeof(value)));

    if (success)
    {
        const ::OcaInt8* pValue(reinterpret_cast<const ::OcaInt8*>(*source));
        value = *pValue;
        (*source) += sizeof(value);
        bytesLeft -= static_cast< ::OcaUint32>(sizeof(value));
    }
    return success;
}

bool Ocp1LiteReader::Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaInt16& value) const
{
    bool success(bytesLeft >= static_cast< ::OcaUint32>(sizeof(value)));

    if (success)
    {
        UINT16 rawValue;
        ::memcpy(&rawValue, *source, sizeof(rawValue));
        value = static_cast< ::OcaInt16>(::ConvertFromNetworkOrderToHostByteOrder(rawValue));
        *source += sizeof(value);
        bytesLeft -= static_cast< ::OcaUint32>(sizeof(value));
    }

    return success;
}


bool Ocp1LiteReader::Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaInt32& value) const
{
    bool success(bytesLeft >= static_cast< ::OcaUint32>(sizeof(value)));

    if (success)
    {
        UINT32 rawValue;
        ::memcpy(&rawValue, *source, sizeof(rawValue));
        value = static_cast< ::OcaInt32>(::ConvertFromNetworkOrderToHostByteOrder(rawValue));
        *source += sizeof(value);
        bytesLeft -= static_cast< ::OcaUint32>(sizeof(value));
    }

    return success;
}

bool Ocp1LiteReader::Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaInt64& value) const
{
    bool success(bytesLeft >= static_cast< ::OcaUint32>(sizeof(value)));

    if (success)
    {
        UINT64 rawValue;
        ::memcpy(&rawValue, *source, sizeof(rawValue));
        value = static_cast< ::OcaInt64>(::ConvertFromNetworkOrderToHostByteOrder(rawValue));
        *source += sizeof(value);
        bytesLeft -= static_cast< ::OcaUint32>(sizeof(value));
    }
    return success;
}

bool Ocp1LiteReader::Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaUint8& value) const
{
    bool success(bytesLeft >= static_cast< ::OcaUint32>(sizeof(value)));

    if (success)
    {
        value = static_cast< ::OcaUint8>(**source);
        *source += sizeof(value);
        bytesLeft -= static_cast< ::OcaUint32>(sizeof(value));
    }
    return success;
}

bool Ocp1LiteReader::Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaUint16& value) const
{
    bool success(bytesLeft >= static_cast< ::OcaUint32>(sizeof(value)));

    if (success)
    {
        ::OcaUint16 rawValue;
        ::memcpy(&rawValue, *source, sizeof(rawValue));
        value = static_cast< ::OcaUint16>(::ConvertFromNetworkOrderToHostByteOrder(static_cast< UINT16>(rawValue)));
        *source += sizeof(value);
        bytesLeft -= static_cast< ::OcaUint32>(sizeof(value));
    }
    return success;
}

bool Ocp1LiteReader::Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaUint32& value) const
{
    bool success(bytesLeft >= static_cast< ::OcaUint32>(sizeof(value)));

    if (success)
    {
        ::OcaUint32 rawValue;
        ::memcpy(&rawValue, *source, sizeof(rawValue));
        value = static_cast< ::OcaUint32>(::ConvertFromNetworkOrderToHostByteOrder(static_cast< UINT32>(rawValue)));
        *source += sizeof(value);
        bytesLeft -= static_cast< ::OcaUint32>(sizeof(value));
    }
    return success;
}

bool Ocp1LiteReader::Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaUint64& value) const
{
    bool success(bytesLeft >= static_cast< ::OcaUint32>(sizeof(value)));

    if (success)
    {
        ::OcaUint64 rawValue;
        ::memcpy(&rawValue, *source, sizeof(rawValue));
        value = static_cast< ::OcaUint64>(::ConvertFromNetworkOrderToHostByteOrder(static_cast< UINT64>(rawValue)));
        *source += sizeof(value);
        bytesLeft -= static_cast< ::OcaUint32>(sizeof(value));
    }
    return success;
}

bool Ocp1LiteReader::Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaFloat32& value) const
{
    bool success(bytesLeft >= static_cast< ::OcaUint32>(sizeof(value)));

    if (success)
    {
        UINT32 tmpValue;
        ::memcpy(&tmpValue, *source, sizeof(tmpValue));
        tmpValue = (::ConvertFromNetworkOrderToHostByteOrder(tmpValue));
        value = *reinterpret_cast< ::OcaFloat32 *>(&tmpValue);
        *source += sizeof(value);
        bytesLeft -= static_cast< ::OcaUint32>(sizeof(value));
    }
    return success;
}

bool Ocp1LiteReader::Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaUint32 nrOfBytes, ::OcaUint8* values) const
{
    bool success(bytesLeft >= nrOfBytes);

    if (success)
    {
        ::memcpy(values, *source, static_cast<size_t>(nrOfBytes));
        (*source) += nrOfBytes;
        bytesLeft -= nrOfBytes;
    }

    return success;
}
