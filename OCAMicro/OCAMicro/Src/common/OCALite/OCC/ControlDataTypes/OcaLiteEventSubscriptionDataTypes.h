/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OCC event and subscription data types.
 *
 */

#ifndef OCALITEEVENTSUBSCRIPTIONDATATYPES_H
#define OCALITEEVENTSUBSCRIPTIONDATATYPES_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "OcaLiteBaseDataTypes.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

/**
 * @addtogroup EventSubscriptionDataTypes
 * @{
 */

/**
 * Enumeration for subscriptions that specifies whether its notification messages are
 * to be delivered by reliable means (e.g. TCP) or fast means (e.g. UDP).
 */
//lint -strong(AJX, OcaLiteNotificationDeliveryMode)
enum OcaLiteNotificationDeliveryMode    /* maps onto OcaUint8 */
{
    /** Reliable delivery mode, e.g. TCP. */
    OCANOTIFICATIONDELIVERYMODE_RELIABLE        = 1,
    /** Fast delivery mode, e.g. UDP. */
    OCANOTIFICATIONDELIVERYMODE_FAST            = 2,
    /** Maximum value used for range checking */
    OCANOTIFICATIONDELIVERYMODE_MAXIMUM
};

/** @} */ /* end of adding to group OCA */

// ---- Helper functions ----

// ---- Specialized Template Function Definition ----

//lint -save -e1576 Explicit specialization does not occur in the same file as corresponding function template

template <>
void MarshalValue< ::OcaLiteNotificationDeliveryMode>(const ::OcaLiteNotificationDeliveryMode& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::OcaLiteNotificationDeliveryMode>(::OcaLiteNotificationDeliveryMode& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteNotificationDeliveryMode>(const ::OcaLiteNotificationDeliveryMode& value, const ::IOcaLiteWriter& writer);

//lint -restore

#endif /* OCALITEEVENTSUBSCRIPTIONDATATYPES_H */
