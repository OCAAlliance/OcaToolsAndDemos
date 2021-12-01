/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteMethod
 *
 */

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteMethod.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMethod::OcaLiteMethod()
    : ::IOcaLiteMarshal(),
      m_oNo(OCA_INVALID_ONO),
      m_methodID()
{
}

OcaLiteMethod::OcaLiteMethod(::OcaONo oNo, const ::OcaLiteMethodID& methodID)
    : ::IOcaLiteMarshal(),
      m_oNo(oNo),
      m_methodID(methodID)
{
}

OcaLiteMethod::OcaLiteMethod(const ::OcaLiteMethod& source)
    : ::IOcaLiteMarshal(source),
      m_oNo(source.m_oNo),
      m_methodID(source.m_methodID)
{
}

OcaLiteMethod::~OcaLiteMethod()
{
}

::OcaLiteMethod& OcaLiteMethod::operator=(const ::OcaLiteMethod& source)
{
    if (this != &source)
    {
        m_oNo = source.m_oNo;
        m_methodID = source.m_methodID;
    }

    return *this;
}

bool OcaLiteMethod::operator==(const ::OcaLiteMethod& rhs) const
{
    bool result(m_oNo == rhs.m_oNo);
    result = result && (m_methodID == rhs.m_methodID);

    return result;
}

bool OcaLiteMethod::operator!=(const ::OcaLiteMethod& rhs) const
{
    return !(operator==(rhs));
}

void OcaLiteMethod::Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
{
    writer.Write(m_oNo, destination);
    m_methodID.Marshal(destination, writer);
}

bool OcaLiteMethod::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool result(reader.Read(bytesLeft, source, m_oNo));
    result = result && m_methodID.Unmarshal(bytesLeft, source, reader);

    if (!result)
    {
        m_oNo = OCA_INVALID_ONO;
        m_methodID = ::OcaLiteMethodID();
    }

    return result;
}

::OcaUint32 OcaLiteMethod::GetSize(const ::IOcaLiteWriter& writer) const
{
    ::OcaUint32 length(writer.GetSize(m_oNo));
    length += m_methodID.GetSize(writer);

    return length;
}
