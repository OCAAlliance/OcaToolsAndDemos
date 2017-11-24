/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : Ocp1LiteNetworkNodeID
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "Ocp1LiteReader.h"
#include "Ocp1LiteWriter.h"
#include "Ocp1LiteNetworkNodeID.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

Ocp1LiteNetworkNodeID::Ocp1LiteNetworkNodeID(const ::OcaLiteString& nodeID)
    : ::OcaLiteNetworkNodeID(),
      m_nodeID(nodeID)
{
    UpdateBlob();
}

Ocp1LiteNetworkNodeID::Ocp1LiteNetworkNodeID(const ::Ocp1LiteNetworkNodeID& source)
    : ::OcaLiteNetworkNodeID(source),
      m_nodeID(source.m_nodeID)
{
    UpdateBlob();
}

Ocp1LiteNetworkNodeID::~Ocp1LiteNetworkNodeID()
{
}

const ::OcaLiteString& Ocp1LiteNetworkNodeID::GetNodeID() const
{
    return m_nodeID;
}

::Ocp1LiteNetworkNodeID& Ocp1LiteNetworkNodeID::operator=(const ::Ocp1LiteNetworkNodeID& source)
{
    if (this != &source)
    {
        OcaLiteNetworkNodeID::operator=(source);
        m_nodeID = source.m_nodeID;
    }

    return *this;
}

::Ocp1LiteNetworkNodeID* Ocp1LiteNetworkNodeID::CreateFromBase(const ::OcaLiteNetworkNodeID& baseClass)
{
    ::Ocp1LiteNetworkNodeID* result(NULL);
    ::OcaLiteString nodeID;

    if (GetValuesFromBase(baseClass, nodeID))
    {
        result = new ::Ocp1LiteNetworkNodeID(nodeID);
    }
    else
    {
        OCA_LOG_WARNING("Network node ID not an Ocp1LiteNetworkNodeID");
    }

    return result;
}

bool Ocp1LiteNetworkNodeID::Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    bool success(OcaLiteNetworkNodeID::Unmarshal(bytesLeft, source, reader));
    success = success && GetValuesFromBase(*this, m_nodeID);
    return success;
}

bool Ocp1LiteNetworkNodeID::GetValuesFromBase(const ::OcaLiteNetworkNodeID& baseClass, ::OcaLiteString& nodeId)
{
    ::OcaUint32 bytesLeft(static_cast< ::OcaUint32>(baseClass.GetValue().GetDataSize()));

    ::Ocp1LiteReader reader;
    const ::OcaUint8* buffer(baseClass.GetValue().GetData());
    bool success(nodeId.Unmarshal(bytesLeft, &buffer, reader));
    success = success && (static_cast< ::OcaUint32>(0) == bytesLeft);

    return success;
}

void Ocp1LiteNetworkNodeID::UpdateBlob()
{
    ::Ocp1LiteWriter writer;
    ::OcaUint32 size(m_nodeID.GetSize(writer));
    ::OcaUint8* buffer(new ::OcaUint8[static_cast<size_t>(size)]);
    ::OcaUint8* pBuffer(buffer);

    m_nodeID.Marshal(&pBuffer, writer);

    ::OcaLiteBlob ocaBlob(static_cast< ::OcaUint16>(size), buffer);
    OcaLiteNetworkNodeID::SetValue(ocaBlob);

    delete[] buffer;
}
