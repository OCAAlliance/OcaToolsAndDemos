/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteNetworkDanteNodeID
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
#include "OcaLiteNetworkDanteNodeID.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteNetworkDanteNodeID::OcaLiteNetworkDanteNodeID(const ::OcaLiteString& danteID)
    : ::OcaLiteNetworkNodeID(),
      m_danteId(danteID)
{
    UpdateBlob();
}

OcaLiteNetworkDanteNodeID::OcaLiteNetworkDanteNodeID(const ::OcaLiteNetworkDanteNodeID& source)
    : ::OcaLiteNetworkNodeID(source),
      m_danteId(source.m_danteId)
{
    UpdateBlob();
}

OcaLiteNetworkDanteNodeID::~OcaLiteNetworkDanteNodeID()
{
}

const ::OcaLiteString& OcaLiteNetworkDanteNodeID::GetdanteId() const
{
    return m_danteId;
}

::OcaLiteNetworkDanteNodeID& OcaLiteNetworkDanteNodeID::operator=(const ::OcaLiteNetworkDanteNodeID& source)
{
    if (this != &source)
    {
        OcaLiteNetworkNodeID::operator=(source);
        m_danteId = source.m_danteId;
    }

    return *this;
}

::OcaLiteNetworkDanteNodeID* OcaLiteNetworkDanteNodeID::CreateFromBase(const ::OcaLiteNetworkNodeID& baseClass)
{
    ::OcaLiteNetworkDanteNodeID* result(NULL);
    ::OcaLiteString danteID;

    if (GetValuesFromBase(baseClass, danteID))
    {
        result = new ::OcaLiteNetworkDanteNodeID(danteID);
    }
    else
    {
        OCA_LOG_WARNING("Channel ID not an OcaLiteNetworkDanteNodeID");
    }

    return result;
}

bool OcaLiteNetworkDanteNodeID::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool success(OcaLiteNetworkNodeID::Unmarshal(bytesLeft, source, reader));
    success = success && GetValuesFromBase(*this, m_danteId);
    return success;
}

bool OcaLiteNetworkDanteNodeID::GetValuesFromBase(const ::OcaLiteNetworkNodeID& baseClass, ::OcaLiteString& danteId)
{
    ::OcaUint32 bytesLeft(static_cast< ::OcaUint32>(baseClass.GetValue().GetDataSize()));

    ::Ocp1LiteReader reader;
    const ::OcaUint8* buffer(baseClass.GetValue().GetData());
    bool success(danteId.Unmarshal(bytesLeft, &buffer, reader));
    success = success && (static_cast< ::OcaUint32>(0) == bytesLeft);

    return success;
}

void OcaLiteNetworkDanteNodeID::UpdateBlob()
{
    ::Ocp1LiteWriter writer;
    ::OcaUint32 size(m_danteId.GetSize(writer));
    ::OcaUint8* buffer(new ::OcaUint8[static_cast<size_t>(size)]);
    ::OcaUint8* pBuffer(buffer);

    m_danteId.Marshal(&pBuffer, writer);

    ::OcaLiteBlob ocaBlob(static_cast< ::OcaUint16>(size), buffer);
    OcaLiteNetworkNodeID::SetValue(ocaBlob);

    delete[] buffer;
}
