/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
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

template <>
void MarshalValue< ::OcaLiteNetworkStatus>(const ::OcaLiteNetworkStatus& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
void MarshalValue< ::OcaLiteStreamStatus>(const ::OcaLiteStreamStatus& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
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
void MarshalValue< ::OcaLiteMediaConnectorState>(const ::OcaLiteMediaConnectorState& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}


template <>
void MarshalValue< ::OcaLiteMediaStreamCastMode>(const ::OcaLiteMediaStreamCastMode& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
void MarshalValue< ::OcaLiteMediaConnectorCommand>(const ::OcaLiteMediaConnectorCommand& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
void MarshalValue< ::OcaLiteMediaConnectorElement>(const ::OcaLiteMediaConnectorElement& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint16>(static_cast< ::OcaUint16>(value), destination, writer);
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
bool UnmarshalValue< ::OcaLiteStreamStatus>(::OcaLiteStreamStatus& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 networkStatus(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, networkStatus));
    if (result)
    {
        if (networkStatus < static_cast< ::OcaUint8>(OCANETWORKSTATUS_MAXIMUM))
        {
            value = static_cast< ::OcaLiteStreamStatus>(networkStatus);
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
        value = static_cast< ::OcaLiteNetworkMediaProtocol>(networkMediaProtocol);
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
bool UnmarshalValue< ::OcaLiteMediaConnectorState>(::OcaLiteMediaConnectorState& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 connectorState(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, connectorState));
    if (result)
    {
        if (connectorState < static_cast< ::OcaUint8>(OCAMEDIACONNECTORSTATE_MAXIMUM))
        {
            value = static_cast< ::OcaLiteMediaConnectorState>(connectorState);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
bool UnmarshalValue< ::OcaLiteMediaStreamCastMode>(::OcaLiteMediaStreamCastMode& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 streamCastMode(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, streamCastMode));
    if (result)
    {
        if (streamCastMode < static_cast< ::OcaUint8>(OCAMEDIASTREAMCASTMODE_MAXIMUM))
        {
            value = static_cast< ::OcaLiteMediaStreamCastMode>(streamCastMode);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
bool UnmarshalValue< ::OcaLiteMediaConnectorCommand>(::OcaLiteMediaConnectorCommand& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 connectorCommand(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, connectorCommand));
    if (result)
    {
        if (connectorCommand < static_cast< ::OcaUint8>(OCAMEDIACONNECTORCOMMAND_MAXIMUM))
        {
            value = static_cast< ::OcaLiteMediaConnectorCommand>(connectorCommand);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
bool UnmarshalValue< ::OcaLiteMediaConnectorElement>(::OcaLiteMediaConnectorElement& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint16 element(static_cast< ::OcaUint16>(0));
    bool result(reader.Read(bytesLeft, source, element));
    if (result)
    {
        value = static_cast< ::OcaLiteMediaConnectorElement>(element);
    }

    return result;
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteNetworkStatus>(const ::OcaLiteNetworkStatus& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteStreamStatus>(const ::OcaLiteStreamStatus& value, const ::IOcaLiteWriter& writer)
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

template <>
::OcaUint32 GetSizeValue< ::OcaLiteMediaConnectorState>(const ::OcaLiteMediaConnectorState& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteMediaStreamCastMode>(const ::OcaLiteMediaStreamCastMode& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteMediaConnectorCommand>(const ::OcaLiteMediaConnectorCommand& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteMediaConnectorElement>(const ::OcaLiteMediaConnectorElement& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint16>(static_cast< ::OcaUint16>(value), writer);
}
