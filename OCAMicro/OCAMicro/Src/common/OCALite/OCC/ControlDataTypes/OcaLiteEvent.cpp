/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteEvent
 *
 */

// ---- Include system wide include files ----
#include <assert.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteEvent.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteEvent::OcaLiteEvent()
    : ::IOcaLiteMarshal(),
      m_emitterONo(OCA_INVALID_ONO),
      m_eventID()
{
}

OcaLiteEvent::OcaLiteEvent(::OcaONo emitterONo, const ::OcaLiteEventID& eventID)
    : ::IOcaLiteMarshal(),
      m_emitterONo(emitterONo),
      m_eventID(eventID)
{
    assert(OCA_INVALID_ONO != emitterONo);
}

OcaLiteEvent::OcaLiteEvent(const ::OcaLiteEvent& source)
    : ::IOcaLiteMarshal(source),
      m_emitterONo(source.m_emitterONo),
      m_eventID(source.m_eventID)
{
}

OcaLiteEvent::~OcaLiteEvent()
{
}

::OcaLiteEvent& OcaLiteEvent::operator=(const ::OcaLiteEvent& source)
{
    if (this != &source)
    {
        m_emitterONo = source.m_emitterONo;
        m_eventID = source.m_eventID;
    }

    return *this;
}

bool OcaLiteEvent::operator==(const ::OcaLiteEvent& rhs) const
{
    bool result(m_emitterONo == rhs.m_emitterONo);
    result = result && (m_eventID == rhs.m_eventID);

    return result;
}

bool OcaLiteEvent::operator!=(const ::OcaLiteEvent& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteEvent::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(m_emitterONo, destination);
    m_eventID.Marshal(destination, writer);
}

bool OcaLiteEvent::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(reader.Read(bytesLeft, source, m_emitterONo));
    result = result && m_eventID.Unmarshal(bytesLeft, source, reader);

    if (!result)
    {
        m_emitterONo = OCA_INVALID_ONO;
        m_eventID = OcaLiteEventID();
    }

    return result;
}

::OcaUint32 OcaLiteEvent::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(writer.GetSize(m_emitterONo));
    length += m_eventID.GetSize(writer);

    return length;
}
