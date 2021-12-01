/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteClassIdentification
 *
 */

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteClassIdentification.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteClassIdentification::OcaLiteClassIdentification()
    : ::IOcaLiteMarshal(),
      m_classID(),
      m_classVersion(static_cast< ::OcaClassVersionNumber>(0))
{
}

OcaLiteClassIdentification::OcaLiteClassIdentification(const ::OcaLiteClassID& classID, ::OcaClassVersionNumber classVersion)
    : ::IOcaLiteMarshal(),
      m_classID(classID),
      m_classVersion(classVersion)
{
}

OcaLiteClassIdentification::OcaLiteClassIdentification(const ::OcaLiteClassIdentification& source)
    : ::IOcaLiteMarshal(source),
      m_classID(source.m_classID),
      m_classVersion(source.m_classVersion)
{
}

OcaLiteClassIdentification::~OcaLiteClassIdentification()
{
}

::OcaLiteClassIdentification& OcaLiteClassIdentification::operator=(const ::OcaLiteClassIdentification& source)
{
    if (this != &source)
    {
        m_classID = source.m_classID;
        m_classVersion = source.m_classVersion;
    }

    return *this;
}

bool OcaLiteClassIdentification::operator==(const ::OcaLiteClassIdentification& rhs) const
{
    bool result(m_classID == rhs.m_classID);
    result = result && (m_classVersion == rhs.m_classVersion);

    return result;
}

bool OcaLiteClassIdentification::operator!=(const ::OcaLiteClassIdentification& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteClassIdentification::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    m_classID.Marshal(destination, writer);
    writer.Write(m_classVersion, destination);
}

bool OcaLiteClassIdentification::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(m_classID.Unmarshal(bytesLeft, source, reader));
    result = result && reader.Read(bytesLeft, source, m_classVersion);

    if (!result)
    {
        m_classID = ::OcaLiteClassID();
        m_classVersion = static_cast< ::OcaClassVersionNumber>(0);
    }

    return result;
}

::OcaUint32 OcaLiteClassIdentification::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(m_classID.GetSize(writer));
    length += writer.GetSize(m_classVersion);

    return length;
}
