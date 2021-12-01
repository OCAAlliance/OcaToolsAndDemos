/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : Ocp1LiteMessageKeepAlive
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "Ocp1LiteMessageKeepAlive.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

Ocp1LiteMessageKeepAlive::Ocp1LiteMessageKeepAlive()
    : ::OcaLiteMessageKeepAlive()
{
}

Ocp1LiteMessageKeepAlive::~Ocp1LiteMessageKeepAlive()
{
}

void Ocp1LiteMessageKeepAlive::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(GetHeartBeatTime(), destination);
}

bool Ocp1LiteMessageKeepAlive::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool success(GetMessageType() == OcaLiteHeader::OCA_MSG_KEEP_ALIVE);

    ::OcaUint16 heartBeatTime(static_cast< ::OcaUint16>(0));
    success = success && reader.Read(bytesLeft, source, heartBeatTime);

    WriteParameters(heartBeatTime);

    return success;
}

::OcaUint32 Ocp1LiteMessageKeepAlive::GetSize(const ::IOcaLiteWriter& writer) const
{
    return writer.GetSize(GetHeartBeatTime());
}
