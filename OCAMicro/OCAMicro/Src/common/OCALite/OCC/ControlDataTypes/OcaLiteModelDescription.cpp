/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteModelDescription
 *
 */

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteModelDescription.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteModelDescription::OcaLiteModelDescription()
    : ::IOcaLiteMarshal(),
      m_manufacturer(),
      m_name(),
      m_version()
{
}

OcaLiteModelDescription::OcaLiteModelDescription(const ::OcaLiteString& manufacturer,
                                         const ::OcaLiteString& name,
                                         const ::OcaLiteString& version)
    : ::IOcaLiteMarshal(),
      m_manufacturer(manufacturer),
      m_name(name),
      m_version(version)
{
}

OcaLiteModelDescription::OcaLiteModelDescription(const ::OcaLiteModelDescription& source)
    : ::IOcaLiteMarshal(source),
      m_manufacturer(source.m_manufacturer),
      m_name(source.m_name),
      m_version(source.m_version)
{
}

OcaLiteModelDescription::~OcaLiteModelDescription()
{
}

::OcaLiteModelDescription& OcaLiteModelDescription::operator=(const ::OcaLiteModelDescription& source)
{
    if (this != &source)
    {
        m_manufacturer = source.m_manufacturer;
        m_name = source.m_name;
        m_version = source.m_version;
    }

    return *this;
}

bool OcaLiteModelDescription::operator==(const ::OcaLiteModelDescription& rhs) const
{
    bool result(m_manufacturer == rhs.m_manufacturer);
    result = result && (m_name == rhs.m_name);
    result = result && (m_version == rhs.m_version);

    return result;
}

bool OcaLiteModelDescription::operator!=(const ::OcaLiteModelDescription& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteModelDescription::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    m_manufacturer.Marshal(destination, writer);
    m_name.Marshal(destination, writer);
    m_version.Marshal(destination, writer);
}

bool OcaLiteModelDescription::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(m_manufacturer.Unmarshal(bytesLeft, source, reader));
    result = result && m_name.Unmarshal(bytesLeft, source, reader);
    result = result && m_version.Unmarshal(bytesLeft, source, reader);

    if (!result)
    {
        m_manufacturer = ::OcaLiteString();
        m_name = ::OcaLiteString();
        m_version = ::OcaLiteString();
    }

    return result;
}

::OcaUint32 OcaLiteModelDescription::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(m_manufacturer.GetSize(writer));
    length += m_name.GetSize(writer);
    length += m_version.GetSize(writer);

    return length;
}
