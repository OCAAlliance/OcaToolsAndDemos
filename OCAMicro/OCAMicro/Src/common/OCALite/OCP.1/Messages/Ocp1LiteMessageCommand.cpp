/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : Ocp1LiteMessageCommand
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "Ocp1LiteMessageCommand.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

Ocp1LiteMessageCommand::Ocp1LiteMessageCommand()
    : ::OcaLiteMessageCommand(::OcaLiteHeader::OCA_MSG_CMD)
{
}

Ocp1LiteMessageCommand::~Ocp1LiteMessageCommand()
{
}

void Ocp1LiteMessageCommand::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
#ifndef OCA_LITE_CONTROLLER
    assert(false); // Don't need to marshal commands
#else
	const ::OcaLiteMethodID& methodId(GetMethodID());
	writer.Write(static_cast<UINT32>(GetSize(writer)), destination);//CommandSize
	writer.Write(GetHandle(), destination);
	MarshalValue< ::OcaONo>(GetTargetONo(), destination, writer);
	methodId.Marshal(destination, writer);
	writer.Write(GetParameters(), GetParametersSize(), destination);
#endif
}

bool Ocp1LiteMessageCommand::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool success((GetMessageType() == OcaLiteHeader::OCA_MSG_CMD) || (GetMessageType() == OcaLiteHeader::OCA_MSG_CMD_RRQ));

    ::OcaUint32 originalBytesLeft(bytesLeft);

    ::OcaUint32 commandSize(static_cast< ::OcaUint32>(0));
    success = success && reader.Read(bytesLeft, source, commandSize);

    ::OcaUint32 handle(static_cast< ::OcaUint32>(0));
    success = success && reader.Read(bytesLeft, source, handle);

    ::OcaONo targetONo(OCA_INVALID_ONO);
    success = success && UnmarshalValue< ::OcaONo>(targetONo, bytesLeft, source, reader);

    ::OcaLiteMethodID methodID;
    success = success && methodID.Unmarshal(bytesLeft, source, reader);

    ::OcaUint32 parametersSize(commandSize - (originalBytesLeft - bytesLeft));
    ::OcaUint32 parameterBytesLeft(parametersSize);
    if (success)
    {
        reader.Read(bytesLeft, source, parametersSize, GetParameters());
    }
    bytesLeft -= (parametersSize - parameterBytesLeft);
    
    WriteParameters(handle, targetONo, methodID, parametersSize);

    return success;
}

::OcaUint32 Ocp1LiteMessageCommand::GetSize(const ::IOcaLiteWriter& writer) const
{
    const ::OcaLiteMethodID& methodId(GetMethodID());
    return writer.GetSize(static_cast< ::OcaUint32>(0)) + // Message size
           writer.GetSize(GetHandle()) +
           writer.GetSize(GetTargetONo()) +
           methodId.GetSize(writer) +
           ((GetParametersSize() > 0) ? GetParametersSize() : 1);
}
