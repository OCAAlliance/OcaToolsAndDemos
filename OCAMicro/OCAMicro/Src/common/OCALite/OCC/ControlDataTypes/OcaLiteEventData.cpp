/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteEventData
 *
 */

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteEventData.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteEventData::OcaLiteEventData()
    : ::IOcaLiteMarshal(),
      m_event()
{
}

OcaLiteEventData::OcaLiteEventData(const ::OcaLiteEvent& event)
    : ::IOcaLiteMarshal(),
      m_event(event)
{
}

OcaLiteEventData::OcaLiteEventData(const ::OcaLiteEventData& source)
    : ::IOcaLiteMarshal(source),
      m_event(source.m_event)
{
}

OcaLiteEventData::~OcaLiteEventData()
{
}

::OcaLiteEventData& OcaLiteEventData::operator=(const ::OcaLiteEventData& source)
{
    if (this != &source)
    {
        m_event = source.m_event;
    }

    return *this;
}

bool OcaLiteEventData::operator==(const ::OcaLiteEventData& rhs) const
{
    bool result(m_event == rhs.m_event);

    return result;
}

bool OcaLiteEventData::operator!=(const ::OcaLiteEventData& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteEventData::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    m_event.Marshal(destination, writer);
}

bool OcaLiteEventData::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(m_event.Unmarshal(bytesLeft, source, reader));

    return result;
}

::OcaUint32 OcaLiteEventData::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(m_event.GetSize(writer));

    return length;
}
