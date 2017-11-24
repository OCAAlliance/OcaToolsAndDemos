/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OCC network data types.
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteNetworkDataTypes.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Function Implementation ----

//lint -save -e1576 Explicit specialization does not occur in the same file as corresponding function template

template <>
void MarshalValue< ::OcaLiteNetworkStatus>(const ::OcaLiteNetworkStatus& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
void MarshalValue< ::OcaLiteNetworkConnectionStatus>(const ::OcaLiteNetworkConnectionStatus& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
void MarshalValue< ::OcaLiteNetworkLinkType>(const ::OcaLiteNetworkLinkType& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
void MarshalValue< ::OcaLiteEncoding>(const ::OcaLiteEncoding& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
void MarshalValue< ::OcaLiteNetworkControlProtocol>(const ::OcaLiteNetworkControlProtocol& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
void MarshalValue< ::OcaLiteNetworkMediaProtocol>(const ::OcaLiteNetworkMediaProtocol& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
void MarshalValue< ::OcaLiteNetworkMediaSourceOrSink>(const ::OcaLiteNetworkMediaSourceOrSink& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
bool UnmarshalValue< ::OcaLiteNetworkStatus>(::OcaLiteNetworkStatus& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 networkStatus(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, networkStatus));
    if (result)
    {
        if (networkStatus < static_cast< ::OcaUint8>(OCANETWORKSTATUS_MAXIMUM))
        {
            value = static_cast< ::OcaLiteNetworkStatus>(networkStatus);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
bool UnmarshalValue< ::OcaLiteNetworkConnectionStatus>(::OcaLiteNetworkConnectionStatus& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 networkConnectionStatus(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, networkConnectionStatus));
    if (result)
    {
        if (networkConnectionStatus < static_cast< ::OcaUint8>(OCANETWORKCONNECTIONSTATUS_MAXIMUM))
        {
            value = static_cast< ::OcaLiteNetworkConnectionStatus>(networkConnectionStatus);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
bool UnmarshalValue< ::OcaLiteNetworkLinkType>(::OcaLiteNetworkLinkType& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 networkLinkType(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, networkLinkType));
    if (result)
    {
        if (networkLinkType < static_cast< ::OcaUint8>(OCANETWORKLINKTYPE_MAXIMUM))
        {
            value = static_cast< ::OcaLiteNetworkLinkType>(networkLinkType);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
bool UnmarshalValue< ::OcaLiteEncoding>(::OcaLiteEncoding& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 encoding(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, encoding));
    if (result)
    {
        value = static_cast< ::OcaLiteEncoding>(encoding);
    }

    return result;
}

template <>
bool UnmarshalValue< ::OcaLiteNetworkControlProtocol>(::OcaLiteNetworkControlProtocol& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 networkControlProtocol(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, networkControlProtocol));
    if (result)
    {
        if (networkControlProtocol < static_cast< ::OcaUint8>(OCANETWORKCONTROLPROTOCOL_MAXIMUM))
        {
            value = static_cast< ::OcaLiteNetworkControlProtocol>(networkControlProtocol);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
bool UnmarshalValue< ::OcaLiteNetworkMediaProtocol>(::OcaLiteNetworkMediaProtocol& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 networkMediaProtocol(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, networkMediaProtocol));
    if (result)
    {
        if (networkMediaProtocol < static_cast< ::OcaUint8>(OCANETWORKMEDIAPROTOCOL_MAXIMUM))
        {
            value = static_cast< ::OcaLiteNetworkMediaProtocol>(networkMediaProtocol);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
bool UnmarshalValue< ::OcaLiteNetworkMediaSourceOrSink>(::OcaLiteNetworkMediaSourceOrSink& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 networkMediaSourceOrSink(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, networkMediaSourceOrSink));
    if (result)
    {
        if (networkMediaSourceOrSink < static_cast< ::OcaUint8>(OCANETWORKMEDIASOURCEORSINK_MAXIMUM))
        {
            value = static_cast< ::OcaLiteNetworkMediaSourceOrSink>(networkMediaSourceOrSink);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteNetworkStatus>(const ::OcaLiteNetworkStatus& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteNetworkConnectionStatus>(const ::OcaLiteNetworkConnectionStatus& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteNetworkLinkType>(const ::OcaLiteNetworkLinkType& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteEncoding>(const ::OcaLiteEncoding& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteNetworkControlProtocol>(const ::OcaLiteNetworkControlProtocol& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteNetworkMediaProtocol>(const ::OcaLiteNetworkMediaProtocol& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteNetworkMediaSourceOrSink>(const ::OcaLiteNetworkMediaSourceOrSink& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

//lint -restore
