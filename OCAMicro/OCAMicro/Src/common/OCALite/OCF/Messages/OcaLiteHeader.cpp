/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteHeader
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteHeader.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteHeader::OcaLiteHeader()
    : ::IOcaLiteMarshal(),
      m_messageType(OCA_MSG_CMD_RRQ),
      m_messageCount(static_cast< ::OcaUint16>(0))
{
}

OcaLiteHeader::~OcaLiteHeader()
{
}

void OcaLiteHeader::WriteParameters(::OcaLiteHeader::OcaLiteMessageType messageType, ::OcaUint16 messageCount)
{
    assert(messageType < OCA_MSG_MAXIMUM);
    m_messageType = messageType;
    m_messageCount = messageCount;
}

// ---- Function Implementation ----

//lint -save -e1576 Explicit specialization does not occur in the same file as corresponding function template

template <>
void MarshalValue< ::OcaLiteHeader::OcaLiteMessageType>(const ::OcaLiteHeader::OcaLiteMessageType& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
bool UnmarshalValue< ::OcaLiteHeader::OcaLiteMessageType>(::OcaLiteHeader::OcaLiteMessageType& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 messageType(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, messageType));
    if (result)
    {
        if (messageType < static_cast< ::OcaUint8>(::OcaLiteHeader::OCA_MSG_MAXIMUM))
        {
            value = static_cast< ::OcaLiteHeader::OcaLiteMessageType>(messageType);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteHeader::OcaLiteMessageType>(const ::OcaLiteHeader::OcaLiteMessageType& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

//lint -restore
