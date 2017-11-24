/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteMessageCommand
 *
 */

// ---- Include system wide include files ----

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteMessageCommand.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMessageCommand::OcaLiteMessageCommand(::OcaLiteHeader::OcaLiteMessageType messageType)
    : ::OcaLiteMessageGeneral(messageType),
      m_parametersSize(0),
      m_pParameters(new ::OcaUint8[OCA_BUFFER_SIZE]),
      m_handle(static_cast< ::OcaUint32>(0)),
      m_targetONo(OCA_INVALID_ONO),
      m_methodID()
{
    assert((OcaLiteHeader::OCA_MSG_CMD == messageType) || (OcaLiteHeader::OCA_MSG_CMD_RRQ == messageType));
}

OcaLiteMessageCommand::~OcaLiteMessageCommand()
{
    if (NULL != m_pParameters)
    {
        delete m_pParameters;
        m_pParameters = NULL;
    }
}

void OcaLiteMessageCommand::WriteParameters(::OcaUint32 handle,
                                            ::OcaONo targetONo,
                                            const ::OcaLiteMethodID& methodID,
                                            ::OcaUint32 parametersSize)
{
    assert(OCA_INVALID_ONO != targetONo);

    m_handle = handle;
    m_targetONo = targetONo;
    m_methodID = methodID;
    m_parametersSize = parametersSize;
}
