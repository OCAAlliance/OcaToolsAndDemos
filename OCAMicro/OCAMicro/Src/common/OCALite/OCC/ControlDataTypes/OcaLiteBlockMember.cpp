/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteBlockMember
 *
 */

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteBlockMember.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteBlockMember::OcaLiteBlockMember()
    : ::IOcaLiteMarshal(),
      m_memberObjectIdentification(),
      m_containerObjectNumber(OCA_INVALID_ONO)
{
}

OcaLiteBlockMember::OcaLiteBlockMember(const ::OcaLiteObjectIdentification& memberObjectIdentification, ::OcaONo containerObjectNumber)
    : ::IOcaLiteMarshal(),
      m_memberObjectIdentification(memberObjectIdentification),
      m_containerObjectNumber(containerObjectNumber)
{
}

OcaLiteBlockMember::OcaLiteBlockMember(const ::OcaLiteBlockMember& source)
    : ::IOcaLiteMarshal(source),
      m_memberObjectIdentification(source.m_memberObjectIdentification),
      m_containerObjectNumber(source.m_containerObjectNumber)
{
}

OcaLiteBlockMember::~OcaLiteBlockMember()
{
}

::OcaLiteBlockMember& OcaLiteBlockMember::operator=(const ::OcaLiteBlockMember& source)
{
    if (this != &source)
    {
        m_memberObjectIdentification = source.m_memberObjectIdentification;
        m_containerObjectNumber = source.m_containerObjectNumber;
    }

    return *this;
}

bool OcaLiteBlockMember::operator==(const ::OcaLiteBlockMember& rhs) const
{
    bool result(m_memberObjectIdentification == rhs.m_memberObjectIdentification);
    result = result && (m_containerObjectNumber == rhs.m_containerObjectNumber);

    return result;
}

bool OcaLiteBlockMember::operator!=(const ::OcaLiteBlockMember& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteBlockMember::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    m_memberObjectIdentification.Marshal(destination, writer);
    writer.Write(m_containerObjectNumber, destination);
}

bool OcaLiteBlockMember::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(m_memberObjectIdentification.Unmarshal(bytesLeft, source, reader));
    result = result && reader.Read(bytesLeft, source, m_containerObjectNumber);

    if (!result)
    {
        m_memberObjectIdentification = ::OcaLiteObjectIdentification();
        m_containerObjectNumber = OCA_INVALID_ONO;
    }

    return result;
}

::OcaUint32 OcaLiteBlockMember::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(m_memberObjectIdentification.GetSize(writer));
    length += writer.GetSize(m_containerObjectNumber);

    return length;
}
