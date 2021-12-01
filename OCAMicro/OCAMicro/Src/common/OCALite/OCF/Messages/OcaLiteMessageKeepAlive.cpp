/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteMessageKeepAlive
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteMessageKeepAlive.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMessageKeepAlive::OcaLiteMessageKeepAlive()
    : ::OcaLiteMessageGeneral(::OcaLiteHeader::OCA_MSG_KEEP_ALIVE),
      m_heartBeatTime(static_cast< ::OcaUint16>(0)),
      m_heartBeatTimeInMilliseconds(static_cast< ::OcaUint32>(0))
{
}

OcaLiteMessageKeepAlive::~OcaLiteMessageKeepAlive()
{
}

void OcaLiteMessageKeepAlive::WriteParameters(::OcaUint16 heartBeatTime)
{
    m_heartBeatTime = heartBeatTime;
    m_heartBeatTimeInMilliseconds = static_cast< ::OcaUint32>(0);
}

void OcaLiteMessageKeepAlive::WriteParameters(::OcaUint32 heartBeatTimeInMilliseconds)
{
    m_heartBeatTimeInMilliseconds = heartBeatTimeInMilliseconds;
    m_heartBeatTime = static_cast< ::OcaUint16>(0);
}

