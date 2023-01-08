/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OCC application network data types.
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteApplicationNetworkDataTypes.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Function Implementation ----

template <>
void MarshalValue< ::OcaLiteApplicationNetworkState>(const ::OcaLiteApplicationNetworkState& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
void MarshalValue< ::OcaLiteApplicationNetworkCommand>(const ::OcaLiteApplicationNetworkCommand& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
void MarshalValue< ::OcaLiteDiscoverySystemAES67>(const ::OcaLiteDiscoverySystemAES67& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
bool UnmarshalValue< ::OcaLiteApplicationNetworkState>(::OcaLiteApplicationNetworkState& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 applicationNetworkState(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, applicationNetworkState));
    if (result)
    {
        if (applicationNetworkState < static_cast< ::OcaUint8>(OCAAPPLICATIONNETWORKSTATE_MAXIMUM))
        {
            value = static_cast< ::OcaLiteApplicationNetworkState>(applicationNetworkState);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
bool UnmarshalValue< ::OcaLiteApplicationNetworkCommand>(::OcaLiteApplicationNetworkCommand& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 applicationNetworkCommand(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, applicationNetworkCommand));
    if (result)
    {
        if (applicationNetworkCommand < static_cast< ::OcaUint8>(OCAAPPLICATIONNETWORKCOMMAND_MAXIMUM))
        {
            value = static_cast< ::OcaLiteApplicationNetworkCommand>(applicationNetworkCommand);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
bool UnmarshalValue< ::OcaLiteDiscoverySystemAES67>(::OcaLiteDiscoverySystemAES67& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 discoverySystem(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, discoverySystem));
    if (result)
    {
        if (discoverySystem < static_cast< ::OcaUint8>(OCADISCOVERYSYSTEMAES67_MAXIMUM))
        {
            value = static_cast< ::OcaLiteDiscoverySystemAES67>(discoverySystem);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteApplicationNetworkState>(const ::OcaLiteApplicationNetworkState& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteApplicationNetworkCommand>(const ::OcaLiteApplicationNetworkCommand& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteDiscoverySystemAES67>(const ::OcaLiteDiscoverySystemAES67& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}
