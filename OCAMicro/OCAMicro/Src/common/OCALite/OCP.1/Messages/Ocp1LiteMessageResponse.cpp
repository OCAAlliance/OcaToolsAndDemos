/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : Ocp1MessageResponse
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "Ocp1LiteMessageResponse.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

Ocp1MessageResponse::Ocp1MessageResponse()
    : ::OcaLiteMessageResponse()
{
}

Ocp1MessageResponse::~Ocp1MessageResponse()
{
}

void Ocp1MessageResponse::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(GetSize(writer), destination);
    writer.Write(static_cast< ::OcaUint32>(GetHandle()), destination);
    ::MarshalValue< ::OcaLiteStatus>(GetStatus(), destination, writer);
    if (NULL != GetParameters())
    {
        writer.Write(GetParameters(), GetParametersSize(), destination);
    }
    else
    {
        writer.Write(static_cast< ::OcaUint8>(0), destination);
    }
}

bool Ocp1MessageResponse::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
#ifndef OCA_LITE_CONTROLLER
    assert(false);
    return false;
#else
    bool success(GetMessageType() == OcaLiteHeader::OCA_MSG_RSP);

    ::OcaUint32 originalBytesLeft(bytesLeft);

    ::OcaUint32 commandSize(static_cast< ::OcaUint32>(0));
    success = success && reader.Read(bytesLeft, source, commandSize);

    ::OcaUint32 handle(static_cast< ::OcaUint32>(0));
    success = success && reader.Read(bytesLeft, source, handle);

    ::OcaLiteStatus status(OCASTATUS_PROCESSING_FAILED);
    success = success && UnmarshalValue< ::OcaLiteStatus>(status, bytesLeft, source, reader);

    ::OcaUint32 parametersSize(commandSize - (originalBytesLeft - bytesLeft));
    ::OcaUint32 parameterBytesLeft(parametersSize);
    if (success)
    {
        WriteParameters(handle, const_cast< ::OcaUint8*>(*source), parametersSize, status);
        source += parametersSize;
    }
    bytesLeft -= parametersSize;

    return success;
#endif
}

::OcaUint32 Ocp1MessageResponse::GetSize(const ::IOcaLiteWriter& writer) const
{
    return writer.GetSize(static_cast< ::OcaUint32>(0)) + // Message size
           writer.GetSize(GetHandle()) +
           writer.GetSize(static_cast< ::OcaUint8>(GetStatus())) +
           ((GetParametersSize() > 0) ? GetParametersSize() : 1);
}
