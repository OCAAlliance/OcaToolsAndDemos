/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OCC event and subscription data types.
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteEventSubscriptionDataTypes.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Function Implementation ----

template <>
void MarshalValue< ::OcaLiteNotificationDeliveryMode>(const ::OcaLiteNotificationDeliveryMode& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
bool UnmarshalValue< ::OcaLiteNotificationDeliveryMode>(::OcaLiteNotificationDeliveryMode& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 notificationDeliveryMode(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, notificationDeliveryMode));
    if (result)
    {
        if ((notificationDeliveryMode > static_cast< ::OcaUint8>(0)) &&
            (notificationDeliveryMode < static_cast< ::OcaUint8>(OCANOTIFICATIONDELIVERYMODE_MAXIMUM)))
        {
            value = static_cast< ::OcaLiteNotificationDeliveryMode>(notificationDeliveryMode);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteNotificationDeliveryMode>(const ::OcaLiteNotificationDeliveryMode& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}
