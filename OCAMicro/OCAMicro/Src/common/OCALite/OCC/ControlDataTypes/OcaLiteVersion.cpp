/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteVersion
 *
 */

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteTemplateHelpers.h"
#include "OcaLiteVersion.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteVersion::OcaLiteVersion()
    : ::IOcaLiteMarshal(),
      m_major(static_cast< ::OcaUint32>(0)),
      m_minor(static_cast< ::OcaUint32>(0)),
      m_build(static_cast< ::OcaUint32>(0)),
      m_component(static_cast< ::OcaLiteComponent>(0))
{
}

OcaLiteVersion::OcaLiteVersion(::OcaUint32 major, ::OcaUint32 minor, ::OcaUint32 build, ::OcaLiteComponent component)
    : ::IOcaLiteMarshal(),
      m_major(major),
      m_minor(minor),
      m_build(build),
      m_component(component)
{
}

OcaLiteVersion::OcaLiteVersion(const ::OcaLiteVersion& source)
    : ::IOcaLiteMarshal(source),
      m_major(source.m_major),
      m_minor(source.m_minor),
      m_build(source.m_build),
      m_component(source.m_component)
{
}

OcaLiteVersion::~OcaLiteVersion()
{
}

::OcaLiteVersion& OcaLiteVersion::operator=(const ::OcaLiteVersion& source)
{
    if (this != &source)
    {
        m_major = source.m_major;
        m_minor = source.m_minor;
        m_build = source.m_build;
        m_component = source.m_component;
    }

    return *this;
}

bool OcaLiteVersion::operator==(const ::OcaLiteVersion& rhs) const
{
    bool result(m_major == rhs.m_major);
    result = result && (m_minor == rhs.m_minor);
    result = result && (m_build == rhs.m_build);
    result = result && (m_component == rhs.m_component);

    return result;
}

bool OcaLiteVersion::operator!=(const ::OcaLiteVersion& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteVersion::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(m_major, destination);
    writer.Write(m_minor, destination);
    writer.Write(m_build, destination);
    MarshalValue< ::OcaLiteComponent>(m_component, destination, writer);
}

bool OcaLiteVersion::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(reader.Read(bytesLeft, source, m_major));
    result = result && reader.Read(bytesLeft, source, m_minor);
    result = result && reader.Read(bytesLeft, source, m_build);
    result = result && UnmarshalValue< ::OcaLiteComponent>(m_component, bytesLeft, source, reader);

    if (!result)
    {
        m_major = static_cast< ::OcaUint32>(0);
        m_minor = static_cast< ::OcaUint32>(0);
        m_build = static_cast< ::OcaUint32>(0);
        m_component = static_cast< ::OcaLiteComponent>(0);
    }

    return result;
}

::OcaUint32 OcaLiteVersion::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(writer.GetSize(m_major));
    length += writer.GetSize(m_minor);
    length += writer.GetSize(m_build);
    length += GetSizeValue< ::OcaLiteComponent>(m_component, writer);

    return length;
}
