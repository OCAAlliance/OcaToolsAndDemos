/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLitePort
 *
 */

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLitePort.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLitePort::OcaLitePort()
    : ::IOcaLiteMarshal(),
      m_owner(OCA_INVALID_ONO),
      m_id(),
      m_name()
{
}

OcaLitePort::OcaLitePort(::OcaONo owner, const ::OcaLitePortID& id, const ::OcaLiteString& name)
    : ::IOcaLiteMarshal(),
      m_owner(owner),
      m_id(id),
      m_name(name)
{
}

OcaLitePort::OcaLitePort(const ::OcaLitePort& source)
    : ::IOcaLiteMarshal(source),
      m_owner(source.m_owner),
      m_id(source.m_id),
      m_name(source.m_name)
{
}

OcaLitePort::~OcaLitePort()
{
}

::OcaLitePort& OcaLitePort::operator=(const ::OcaLitePort& source)
{
    if (this != &source)
    {
        m_owner = source.m_owner;
        m_id = source.m_id;
        m_name = source.m_name;
    }

    return *this;
}

bool OcaLitePort::operator==(const ::OcaLitePort& rhs) const
{
    bool result(m_owner == rhs.m_owner);
    result = result && (m_id == rhs.m_id);
    result = result && (m_name == rhs.m_name);

    return result;
}

bool OcaLitePort::operator!=(const ::OcaLitePort& rhs) const
{
    return !(operator==(rhs));
}

void OcaLitePort::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(m_owner, destination);
    m_id.Marshal(destination, writer);
    m_name.Marshal(destination, writer);
}

bool OcaLitePort::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(reader.Read(bytesLeft, source, m_owner));
    result = result && m_id.Unmarshal(bytesLeft, source, reader);
    result = result && m_name.Unmarshal(bytesLeft, source, reader);

    if (!result)
    {
        m_owner = OCA_INVALID_ONO;
        m_id = ::OcaLitePortID();
        m_name = ::OcaLiteString();
    }

    return result;
}

::OcaUint32 OcaLitePort::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(writer.GetSize(m_owner));
    length += m_id.GetSize(writer);
    length += m_name.GetSize(writer);

    return length;
}
