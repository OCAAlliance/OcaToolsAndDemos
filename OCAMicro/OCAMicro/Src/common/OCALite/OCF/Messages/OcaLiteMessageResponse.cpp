/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteMessageResponse.
 *
 */

// ---- Include system wide include files ----
#include <OCF/OcaLiteCommandHandler.h>

// ---- Include local include files ----
#include "OcaLiteMessageResponse.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMessageResponse::OcaLiteMessageResponse()
    : ::OcaLiteMessageGeneral(::OcaLiteHeader::OCA_MSG_RSP),
      m_pParameters(NULL),
      m_parametersSize(0),
      m_handle(static_cast< ::OcaUint32>(0)),
      m_statusCode(OCASTATUS_OK)
{
}

OcaLiteMessageResponse::~OcaLiteMessageResponse()
{
    if (NULL != m_pParameters)
    {
        m_pParameters = NULL;
    }
}

void OcaLiteMessageResponse::WriteParameters(::OcaUint32 handle,
                                             ::OcaUint8* pResponse,
                                             ::OcaUint32 responseSize,
                                             ::OcaLiteStatus status)
{
    m_handle = handle;
    m_pParameters = pResponse;
    m_parametersSize = responseSize;
    m_statusCode = status;
}
