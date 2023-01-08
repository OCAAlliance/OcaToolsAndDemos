/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteObjectIdentification
 *
 */

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteObjectIdentification.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteObjectIdentification::OcaLiteObjectIdentification()
    : ::IOcaLiteMarshal(),
      m_oNo(OCA_INVALID_ONO),
      m_classIdentification()
{
}

OcaLiteObjectIdentification::OcaLiteObjectIdentification(::OcaONo oNo, const ::OcaLiteClassIdentification& classIdentification)
    : ::IOcaLiteMarshal(),
      m_oNo(oNo),
      m_classIdentification(classIdentification)
{
}

OcaLiteObjectIdentification::OcaLiteObjectIdentification(const ::OcaLiteObjectIdentification& source)
    : ::IOcaLiteMarshal(source),
      m_oNo(source.m_oNo),
      m_classIdentification(source.m_classIdentification)
{
}

OcaLiteObjectIdentification::~OcaLiteObjectIdentification()
{
}

::OcaLiteObjectIdentification& OcaLiteObjectIdentification::operator=(const ::OcaLiteObjectIdentification& source)
{
    if (this != &source)
    {
        m_oNo = source.m_oNo;
        m_classIdentification = source.m_classIdentification;
    }

    return *this;
}

bool OcaLiteObjectIdentification::operator==(const ::OcaLiteObjectIdentification& rhs) const
{
    bool result(m_oNo == rhs.m_oNo);
    result = result && (m_classIdentification == rhs.m_classIdentification);

    return result;
}

bool OcaLiteObjectIdentification::operator!=(const ::OcaLiteObjectIdentification& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteObjectIdentification::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(m_oNo, destination);
    m_classIdentification.Marshal(destination, writer);
}

bool OcaLiteObjectIdentification::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(reader.Read(bytesLeft, source, m_oNo));
    result = result && m_classIdentification.Unmarshal(bytesLeft, source, reader);

    if (!result)
    {
        m_oNo = OCA_INVALID_ONO;
        m_classIdentification = ::OcaLiteClassIdentification();
    }

    return result;
}

::OcaUint32 OcaLiteObjectIdentification::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(writer.GetSize(m_oNo));
    length += m_classIdentification.GetSize(writer);

    return length;
}
