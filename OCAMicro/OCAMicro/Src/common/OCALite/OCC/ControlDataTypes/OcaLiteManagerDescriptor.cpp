/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteManagerDescriptor
 *
 */

// ---- Include system wide include files ----
#include <assert.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteManagerDescriptor.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteManagerDescriptor::OcaLiteManagerDescriptor()
    : ::IOcaLiteMarshal(),
      m_objectNumber(OCA_INVALID_ONO),
      m_name(),
      m_classID(),
      m_classVersion(static_cast< ::OcaClassVersionNumber>(0))
{
}

OcaLiteManagerDescriptor::OcaLiteManagerDescriptor(::OcaONo objectNumber,
                                           const ::OcaLiteString& name,
                                           const ::OcaLiteClassID& classID,
                                           ::OcaClassVersionNumber classVersion)
    : ::IOcaLiteMarshal(),
      m_objectNumber(objectNumber),
      m_name(name),
      m_classID(classID),
      m_classVersion(classVersion)
{
    assert(static_cast< ::OcaClassVersionNumber>(0) < m_classVersion);
}

OcaLiteManagerDescriptor::OcaLiteManagerDescriptor(const ::OcaLiteManagerDescriptor& source)
    : ::IOcaLiteMarshal(source),
      m_objectNumber(source.m_objectNumber),
      m_name(source.m_name),
      m_classID(source.m_classID),
      m_classVersion(source.m_classVersion)
{
}

OcaLiteManagerDescriptor::~OcaLiteManagerDescriptor()
{
}

::OcaLiteManagerDescriptor& OcaLiteManagerDescriptor::operator=(const ::OcaLiteManagerDescriptor& source)
{
    if (this != &source)
    {
        m_objectNumber = source.m_objectNumber;
        m_name = source.m_name;
        m_classID = source.m_classID;
        m_classVersion = source.m_classVersion;
    }

    return *this;
}

bool OcaLiteManagerDescriptor::operator==(const ::OcaLiteManagerDescriptor& rhs) const
{
    bool result(m_objectNumber == rhs.m_objectNumber);
    result = result && (m_name == rhs.m_name);
    result = result && (m_classID == rhs.m_classID);
    result = result && (m_classVersion == rhs.m_classVersion);

    return result;
}

bool OcaLiteManagerDescriptor::operator!=(const ::OcaLiteManagerDescriptor& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteManagerDescriptor::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(m_objectNumber, destination);
    m_name.Marshal(destination, writer);
    m_classID.Marshal(destination, writer);
    writer.Write(m_classVersion, destination);
}

bool OcaLiteManagerDescriptor::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(reader.Read(bytesLeft, source, m_objectNumber));
    result = result && m_name.Unmarshal(bytesLeft, source, reader);
    result = result && m_classID.Unmarshal(bytesLeft, source, reader);
    result = result && reader.Read(bytesLeft, source, m_classVersion);

    if (!result)
    {
        m_objectNumber = OCA_INVALID_ONO;
        m_name = ::OcaLiteString();
        m_classID = ::OcaLiteClassID();
        m_classVersion = static_cast< ::OcaClassVersionNumber>(0);
    }

    return result;
}

::OcaUint32 OcaLiteManagerDescriptor::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(writer.GetSize(m_objectNumber));
    length += m_name.GetSize(writer);
    length += m_classID.GetSize(writer);
    length += writer.GetSize(m_classVersion);

    return length;
}
