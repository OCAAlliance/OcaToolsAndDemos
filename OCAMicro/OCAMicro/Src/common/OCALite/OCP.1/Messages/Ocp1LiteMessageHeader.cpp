/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : Ocp1LiteMessageHeader
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "Ocp1LiteMessageHeader.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

::OcaUint16 Ocp1LiteMessageHeader::m_latestProtocolVersion(static_cast< ::OcaUint16>(1));

Ocp1LiteMessageHeader::Ocp1LiteMessageHeader()
    : ::OcaLiteHeader(),
      m_protocolVersion(m_latestProtocolVersion),
      m_messageSize(static_cast< ::OcaUint32>(0))
{
}

Ocp1LiteMessageHeader::~Ocp1LiteMessageHeader()
{
}

void Ocp1LiteMessageHeader::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(m_protocolVersion, destination);
    writer.Write(m_messageSize, destination);
    MarshalValue< ::OcaLiteHeader::OcaLiteMessageType>(GetMessageType(), destination, writer);
    writer.Write(GetMessageCount(), destination);
}

bool Ocp1LiteMessageHeader::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool success(reader.Read(bytesLeft, source, m_protocolVersion));
    success = success && reader.Read(bytesLeft, source, m_messageSize);
    ::OcaLiteHeader::OcaLiteMessageType messageType(static_cast< ::OcaLiteHeader::OcaLiteMessageType>(0));
    success = success && UnmarshalValue< ::OcaLiteHeader::OcaLiteMessageType>(messageType, bytesLeft, source, reader);
    success = success && (GetMessageType() < OCA_MSG_MAXIMUM);
    ::OcaUint16 messageCount(static_cast< ::OcaUint16>(0));
    success = success && reader.Read(bytesLeft, source, messageCount);
    OcaLiteHeader::WriteParameters(messageType, messageCount);

    return success;
}

::OcaUint32 Ocp1LiteMessageHeader::GetSize(const ::IOcaLiteWriter& writer) const
{
    return writer.GetSize(m_protocolVersion) +
           writer.GetSize(m_messageSize) +
           GetSizeValue< ::OcaLiteHeader::OcaLiteMessageType>(GetMessageType(), writer) +
           writer.GetSize(GetMessageCount());
}

void Ocp1LiteMessageHeader::WriteOcp1Parameters(::OcaLiteHeader::OcaLiteMessageType messageType,
                                            ::OcaUint16 messageCount,
                                            ::OcaUint32 messageSize)
{
    WriteParameters(messageType, messageCount);

    m_messageSize = messageSize;
}
