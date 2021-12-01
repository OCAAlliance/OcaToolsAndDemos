/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteMethodID
 *
 */

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteMethodID.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMethodID::OcaLiteMethodID()
    : ::IOcaLiteMarshal(),
      m_defLevel(static_cast< ::OcaUint16>(0)),
      m_methodIndex(static_cast< ::OcaUint16>(0))
{
}

OcaLiteMethodID::OcaLiteMethodID(::OcaUint16 defLevel, ::OcaUint16 methodIndex)
    : ::IOcaLiteMarshal(),
      m_defLevel(defLevel),
      m_methodIndex(methodIndex)
{
}

OcaLiteMethodID::OcaLiteMethodID(const ::OcaLiteMethodID& source)
    : ::IOcaLiteMarshal(source),
      m_defLevel(source.m_defLevel),
      m_methodIndex(source.m_methodIndex)
{
}

OcaLiteMethodID::~OcaLiteMethodID()
{
}

::OcaLiteMethodID& OcaLiteMethodID::operator=(const ::OcaLiteMethodID& source)
{
    if (this != &source)
    {
        m_defLevel = source.m_defLevel;
        m_methodIndex = source.m_methodIndex;
    }

    return *this;
}

bool OcaLiteMethodID::operator==(const ::OcaLiteMethodID& rhs) const
{
    bool result(m_defLevel == rhs.m_defLevel);
    result = result && (m_methodIndex == rhs.m_methodIndex);

    return result;
}

bool OcaLiteMethodID::operator!=(const ::OcaLiteMethodID& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteMethodID::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(m_defLevel, destination);
    writer.Write(m_methodIndex, destination);
}

bool OcaLiteMethodID::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(reader.Read(bytesLeft, source, m_defLevel));
    result = result && reader.Read(bytesLeft, source, m_methodIndex);

    if (!result)
    {
        m_defLevel = static_cast< ::OcaUint16>(0);
        m_methodIndex = static_cast< ::OcaUint16>(0);
    }

    return result;
}

::OcaUint32 OcaLiteMethodID::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(writer.GetSize(m_defLevel));
    length += writer.GetSize(m_methodIndex);

    return length;
}
