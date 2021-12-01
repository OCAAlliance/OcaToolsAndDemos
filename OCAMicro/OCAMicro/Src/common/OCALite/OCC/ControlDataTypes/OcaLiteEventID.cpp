/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteEventID
 *
 */

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteEventID.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteEventID::OcaLiteEventID()
    : ::IOcaLiteMarshal(),
      m_defLevel(static_cast< ::OcaUint16>(0)),
      m_eventIndex(static_cast< ::OcaUint16>(0))
{
}

OcaLiteEventID::OcaLiteEventID(::OcaUint16 defLevel, ::OcaUint16 eventIndex)
    : ::IOcaLiteMarshal(),
      m_defLevel(defLevel),
      m_eventIndex(eventIndex)
{
}

OcaLiteEventID::OcaLiteEventID(const ::OcaLiteEventID& source)
    : ::IOcaLiteMarshal(source),
      m_defLevel(source.m_defLevel),
      m_eventIndex(source.m_eventIndex)
{
}

OcaLiteEventID::~OcaLiteEventID()
{
}

::OcaLiteEventID& OcaLiteEventID::operator=(const ::OcaLiteEventID& source)
{
    if (this != &source)
    {
        m_defLevel = source.m_defLevel;
        m_eventIndex = source.m_eventIndex;
    }

    return *this;
}

bool OcaLiteEventID::operator==(const ::OcaLiteEventID& rhs) const
{
    bool result(m_defLevel == rhs.m_defLevel);
    result = result && (m_eventIndex == rhs.m_eventIndex);

    return result;
}

bool OcaLiteEventID::operator!=(const ::OcaLiteEventID& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteEventID::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(m_defLevel, destination);
    writer.Write(m_eventIndex, destination);
}

bool OcaLiteEventID::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(reader.Read(bytesLeft, source, m_defLevel));
    result = result && reader.Read(bytesLeft, source, m_eventIndex);
    
    if (!result)
    {
        m_defLevel = static_cast< ::OcaUint16>(0);
        m_eventIndex = static_cast< ::OcaUint16>(0);
    }

    return result;
}

::OcaUint32 OcaLiteEventID::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(writer.GetSize(m_defLevel));
    length += writer.GetSize(m_eventIndex);

    return length;
}
