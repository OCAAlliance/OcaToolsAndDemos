/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLitePropertyID
 *
 */

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLitePropertyID.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLitePropertyID::OcaLitePropertyID()
    : ::IOcaLiteMarshal(),
      m_defLevel(static_cast< ::OcaUint16>(0)),
      m_propertyIndex(static_cast< ::OcaUint16>(0))
{
}

OcaLitePropertyID::OcaLitePropertyID(::OcaUint16 defLevel, ::OcaUint16 eventIndex)
    : ::IOcaLiteMarshal(),
      m_defLevel(defLevel),
      m_propertyIndex(eventIndex)
{
}

OcaLitePropertyID::OcaLitePropertyID(const ::OcaLitePropertyID& source)
    : ::IOcaLiteMarshal(source),
      m_defLevel(source.m_defLevel),
      m_propertyIndex(source.m_propertyIndex)
{
}

OcaLitePropertyID::~OcaLitePropertyID()
{
}

::OcaLitePropertyID& OcaLitePropertyID::operator=(const ::OcaLitePropertyID& source)
{
    if (this != &source)
    {
        m_defLevel = source.m_defLevel;
        m_propertyIndex = source.m_propertyIndex;
    }

    return *this;
}

bool OcaLitePropertyID::operator==(const ::OcaLitePropertyID& rhs) const
{
    bool result(m_defLevel == rhs.m_defLevel);
    result = result && (m_propertyIndex == rhs.m_propertyIndex);

    return result;
}

bool OcaLitePropertyID::operator!=(const ::OcaLitePropertyID& rhs) const
{
    return !(operator==(rhs));
}

void OcaLitePropertyID::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(m_defLevel, destination);
    writer.Write(m_propertyIndex, destination);
}

bool OcaLitePropertyID::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(reader.Read(bytesLeft, source, m_defLevel));
    result = result && reader.Read(bytesLeft, source, m_propertyIndex);

    if (!result)
    {
        m_defLevel = static_cast< ::OcaUint16>(0);
        m_propertyIndex = static_cast< ::OcaUint16>(0);
    }

    return result;
}

::OcaUint32 OcaLitePropertyID::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(writer.GetSize(m_defLevel));
    length += writer.GetSize(m_propertyIndex);

    return length;
}
