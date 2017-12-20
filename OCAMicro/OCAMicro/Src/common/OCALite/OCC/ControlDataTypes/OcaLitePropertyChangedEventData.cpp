/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLitePropertyChangedEventData
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLitePropertyChangedEventData.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

// ---- Function Implementation ----

template <>
void MarshalValue< ::OcaLitePropertyChangeType>(const ::OcaLitePropertyChangeType& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
bool UnmarshalValue< ::OcaLitePropertyChangeType>(::OcaLitePropertyChangeType& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 changeType(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, changeType));
    if (result)
    {
        if ((changeType > static_cast< ::OcaUint8>(0)) &&
            (changeType < static_cast< ::OcaUint8>(OCAPROPERTYCHANGETYPE_MAXIMUM)))
        {
            value = static_cast< ::OcaLitePropertyChangeType>(changeType);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
::OcaUint32 GetSizeValue< ::OcaLitePropertyChangeType>(const ::OcaLitePropertyChangeType& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}
