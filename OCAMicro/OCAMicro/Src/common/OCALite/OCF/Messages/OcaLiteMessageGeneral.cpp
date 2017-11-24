/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteMessageGeneral
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteMessageGeneral.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMessageGeneral::OcaLiteMessageGeneral(::OcaLiteHeader::OcaLiteMessageType messageType)
    : ::IOcaLiteMarshal(),
      m_messageType(messageType)
{
    assert(messageType < ::OcaLiteHeader::OCA_MSG_MAXIMUM);
}

OcaLiteMessageGeneral::~OcaLiteMessageGeneral()
{
}
