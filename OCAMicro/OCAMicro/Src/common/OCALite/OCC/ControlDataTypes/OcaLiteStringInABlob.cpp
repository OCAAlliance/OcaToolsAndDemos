/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteStringInABlob
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>
#include <OCP.1/Ocp1LiteReader.h>
#include <OCP.1/Ocp1LiteWriter.h>
// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteStringInABlob.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteStringInABlob::OcaLiteStringInABlob(const ::OcaLiteString& danteID)
    : ::OcaLiteBlobDataType(),
      m_stringValue(danteID)
{
    UpdateBlob();
}

OcaLiteStringInABlob::OcaLiteStringInABlob(const ::OcaLiteStringInABlob& source)
    : ::OcaLiteBlobDataType(source),
      m_stringValue(source.m_stringValue)
{
    UpdateBlob();
}

OcaLiteStringInABlob::~OcaLiteStringInABlob()
{
}

const ::OcaLiteString& OcaLiteStringInABlob::GetStringValue() const
{
    return m_stringValue;
}

::OcaLiteStringInABlob& OcaLiteStringInABlob::operator=(const ::OcaLiteStringInABlob& source)
{
    if (this != &source)
    {
        OcaLiteBlobDataType::operator=(source);
        m_stringValue = source.m_stringValue;
    }

    return *this;
}

::OcaLiteStringInABlob* OcaLiteStringInABlob::CreateFromBase(const ::OcaLiteBlobDataType& baseClass)
{
    ::OcaLiteStringInABlob* result(NULL);
    ::OcaLiteString danteID;

    if (GetValuesFromBase(baseClass, danteID))
    {
        result = new ::OcaLiteStringInABlob(danteID);
    }
    else
    {
        OCA_LOG_WARNING("not an OcaLiteStringInABlob");
    }

    return result;
}

bool OcaLiteStringInABlob::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool success(OcaLiteBlobDataType::Unmarshal(bytesLeft, source, reader));
    success = success && GetValuesFromBase(*this, m_stringValue);
    return success;
}

bool OcaLiteStringInABlob::GetValuesFromBase(const ::OcaLiteBlobDataType& baseClass, ::OcaLiteString& danteId)
{
    ::OcaUint32 bytesLeft(static_cast< ::OcaUint32>(baseClass.GetValue().GetDataSize()));

    ::Ocp1LiteReader reader;
    const ::OcaUint8* buffer(baseClass.GetValue().GetData());
    bool success(danteId.Unmarshal(bytesLeft, &buffer, reader));
    success = success && (static_cast< ::OcaUint32>(0) == bytesLeft);

    return success;
}

void OcaLiteStringInABlob::UpdateBlob()
{
    ::Ocp1LiteWriter writer;
    ::OcaUint32 size(m_stringValue.GetSize(writer));
    ::OcaUint8* buffer(new ::OcaUint8[static_cast<size_t>(size)]);
    ::OcaUint8* pBuffer(buffer);

    m_stringValue.Marshal(&pBuffer, writer);

    ::OcaLiteBlob ocaBlob(static_cast< ::OcaUint16>(size), buffer);
    OcaLiteBlobDataType::SetValue(ocaBlob);

    delete[] buffer;
}
