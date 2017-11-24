/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : Ocp1LiteMessageNotification
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "Ocp1LiteMessageNotification.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

Ocp1LiteMessageNotification::Ocp1LiteMessageNotification()
    : ::OcaLiteMessageNotification()
{
}

Ocp1LiteMessageNotification::~Ocp1LiteMessageNotification()
{
}

void Ocp1LiteMessageNotification::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(GetSize(writer), destination);
    MarshalValue< ::OcaONo>(GetTargetONo(), destination, writer);

    const ::OcaLiteMethodID& methodId(GetMethodID());
    methodId.Marshal(destination, writer);

    if (NULL != GetParameters())
    {
        writer.Write(GetParameters(), GetParametersSize(), destination);
    }
    else
    {
        writer.Write(static_cast< ::OcaUint8>(0), destination);
    }
}

bool Ocp1LiteMessageNotification::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    return false; // Don't need to unmarshal events
}

::OcaUint32 Ocp1LiteMessageNotification::GetSize(const ::IOcaLiteWriter& writer) const
{
    const ::OcaLiteMethodID& methodId(GetMethodID());
    return writer.GetSize(static_cast< ::OcaUint32>(0)) +       // Message size
           writer.GetSize(GetTargetONo()) +
           methodId.GetSize(writer) +
           ((GetParametersSize() > 0) ? GetParametersSize() : 1);
}
