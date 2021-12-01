/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteStreamNetwork
 *
 */
// ---- Include system wide include files ----
#include <assert.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlClasses/Workers/Networking/OcaLiteNetworkSignalChannel.h>
#include <OCC/ControlClasses/Managers/OcaLiteNetworkManager.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteStreamNetwork.h"
#include "OcaLiteStreamConnector.h"

// ---- Referenced classes and types ---

// ---- Helper types and constants ----
static const ::OcaUint16        classID[]   = {OCA_STREAMNETWORK_CLASSID};
const ::OcaLiteClassID          OcaLiteStreamNetwork::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteStreamNetwork::OcaLiteStreamNetwork(::OcaONo objectNumber, 
                                           ::OcaBoolean lockable,
                                           const ::OcaLiteString& role, 
                                           ::OcaLiteNetworkLinkType linkType,
                                           const ::OcaLiteBlobDataType& idAdvertised, 
                                           ::OcaLiteNetworkControlProtocol controlProtocol,
                                           ::OcaLiteNetworkMediaProtocol mediaProtocol)
    : ::OcaLiteAgent(objectNumber, lockable, role),
    m_networkLinkType(linkType),
    m_idAdvertised(idAdvertised),
    m_networkControlProtocol(controlProtocol),
    m_networkMediaProtocol(mediaProtocol),
    m_signalSource(),
    m_signalSink(),
    m_connectorsSource(),
    m_connectorsSink()

{
    assert(linkType < OCANETWORKLINKTYPE_MAXIMUM);
    assert(controlProtocol < OCANETWORKCONTROLPROTOCOL_MAXIMUM);
    assert((mediaProtocol < OCANETWORKMEDIAPROTOCOL_MAXIMUM) || (mediaProtocol >= OCANETWORKMEDIAPROTOCOL_EXTENSION_POINT));
}

OcaLiteStreamNetwork::~OcaLiteStreamNetwork()
{
}

::OcaBoolean OcaLiteStreamNetwork::Initialize()
{
    return ::OcaLiteNetworkManager::GetInstance().AddStreamNetwork(*this);
}

void OcaLiteStreamNetwork::Teardown()
{
    for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < m_signalSource.GetCount(); i++)
    {
        ::OcaLiteRoot* pObject(::OcaLiteBlock::GetRootBlock().GetOCAObject(m_signalSource.GetItem(i)));
        ::OcaLiteBlock::GetRootBlock().RemoveObject(m_signalSource.GetItem(i));
        delete pObject;
    }
    m_signalSource.Clear();
    for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < m_signalSink.GetCount(); i++)
    {
        ::OcaLiteRoot* pObject(::OcaLiteBlock::GetRootBlock().GetOCAObject(m_signalSink.GetItem(i)));
        ::OcaLiteBlock::GetRootBlock().RemoveObject(m_signalSink.GetItem(i));
        delete pObject;
    }
    m_signalSink.Clear();
    for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < m_connectorsSource.GetCount(); i++)
    {
        ::OcaLiteRoot* pObject(::OcaLiteBlock::GetRootBlock().GetOCAObject(m_connectorsSource.GetItem(i)));
        ::OcaLiteBlock::GetRootBlock().RemoveObject(m_connectorsSource.GetItem(i));
        delete pObject;
    }
    m_connectorsSource.Clear();
    for (::OcaUint16 i(static_cast< ::OcaUint16>(0)); i < m_connectorsSink.GetCount(); i++)
    {
        ::OcaLiteRoot* pObject(::OcaLiteBlock::GetRootBlock().GetOCAObject(m_connectorsSink.GetItem(i)));
        ::OcaLiteBlock::GetRootBlock().RemoveObject(m_connectorsSink.GetItem(i));
        delete pObject;
    }
    m_connectorsSink.Clear();
    ::OcaLiteNetworkManager::GetInstance().RemoveStreamNetwork(*this);
}

::OcaLiteStatus OcaLiteStreamNetwork::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                              ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response)
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_ERROR);
    if (!IsLocked(sessionID))
    {
        if (methodID.GetDefLevel() == CLASS_ID.GetFieldCount())
        {
            ::OcaUint8* responseBuffer(NULL);
            const ::OcaUint8* pCmdParameters(parameters);
            ::OcaUint32 bytesLeft(parametersSize);

            switch (methodID.GetMethodIndex())
            {
            case GET_LINK_TYPE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + ::GetSizeValue< ::OcaLiteNetworkLinkType>(m_networkLinkType, writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            ::MarshalValue< ::OcaLiteNetworkLinkType>(m_networkLinkType, &pResponse, writer);

                            *response = responseBuffer;

                            rc = OCASTATUS_OK;
                        }
                        else
                        {
                            rc = OCASTATUS_BUFFER_OVERFLOW;
                        }
                    }
                }
                break;
            case GET_ID_ADVERTISED:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + m_idAdvertised.GetSize(writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            m_idAdvertised.Marshal(&pResponse, writer);

                            *response = responseBuffer;

                            rc = OCASTATUS_OK;
                        }
                        else
                        {
                            rc = OCASTATUS_BUFFER_OVERFLOW;
                        }
                    }
                }
                break;
            case SET_ID_ADVERTISED:
                rc = OCASTATUS_NOT_IMPLEMENTED;
                break;
            case GET_CONTROL_PROTOCOL:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + ::GetSizeValue< ::OcaLiteNetworkControlProtocol>(m_networkControlProtocol, writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            ::MarshalValue< ::OcaLiteNetworkControlProtocol>(m_networkControlProtocol, &pResponse, writer);

                            *response = responseBuffer;

                            rc = OCASTATUS_OK;
                        }
                        else
                        {
                            rc = OCASTATUS_BUFFER_OVERFLOW;
                        }
                    }
                }
                break;
            case GET_MEDIA_PROTOCOL:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + ::GetSizeValue< ::OcaLiteNetworkMediaProtocol>(m_networkMediaProtocol, writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            ::MarshalValue< ::OcaLiteNetworkMediaProtocol>(m_networkMediaProtocol, &pResponse, writer);

                            *response = responseBuffer;

                            rc = OCASTATUS_OK;
                        }
                        else
                        {
                            rc = OCASTATUS_BUFFER_OVERFLOW;
                        }
                    }
                }
                break;
            case GET_SIGNAL_CHANNELS_SOURCE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + m_signalSource.GetSize(writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            m_signalSource.Marshal(&pResponse, writer);

                            *response = responseBuffer;

                            rc = OCASTATUS_OK;
                        }
                        else
                        {
                            rc = OCASTATUS_BUFFER_OVERFLOW;
                        }
                    }
                }
                break;
            case GET_SIGNAL_CHANNELS_SINK:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + m_signalSink.GetSize(writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            m_signalSink.Marshal(&pResponse, writer);

                            *response = responseBuffer;

                            rc = OCASTATUS_OK;
                        }
                        else
                        {
                            rc = OCASTATUS_BUFFER_OVERFLOW;
                        }
                    }
                }
                break;
            case GET_STREAM_CONNECTORS_SOURCE:
            {
                ::OcaUint8 numberOfParameters(0);
                if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                    (0 == numberOfParameters))
                {
                    ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + m_connectorsSource.GetSize(writer));
                    responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                    if (NULL != responseBuffer)
                    {
                        ::OcaUint8* pResponse(responseBuffer);
                        writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                        m_connectorsSource.Marshal(&pResponse, writer);

                        *response = responseBuffer;

                        rc = OCASTATUS_OK;
                    }
                    else
                    {
                        rc = OCASTATUS_BUFFER_OVERFLOW;
                    }
                }
            }
            break;
            case GET_STREAM_CONNECTORS_SINK:
            {
                ::OcaUint8 numberOfParameters(0);
                if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                    (0 == numberOfParameters))
                {
                    ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + m_connectorsSink.GetSize(writer));
                    responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                    if (NULL != responseBuffer)
                    {
                        ::OcaUint8* pResponse(responseBuffer);
                        writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                        m_connectorsSink.Marshal(&pResponse, writer);

                        *response = responseBuffer;

                        rc = OCASTATUS_OK;
                    }
                    else
                    {
                        rc = OCASTATUS_BUFFER_OVERFLOW;
                    }
                }
            }
            break;
            case GET_STATUS:
                // TODO, return status
                rc = OCASTATUS_NOT_IMPLEMENTED;
                break;
            case GET_STATISTICS:
            case RESET_STATISTICS:
            case GET_SYSTEM_INTERFACES:
            case SET_SYSTEM_INTERFACES:
            case SET_STREAM_CONNECTORS_SOURCE:
            case SET_STREAM_CONNECTORS_SINK:
            case SET_SIGNAL_CHANNELS_SOURCE:
            case SET_SIGNAL_CHANNELS_SINK:
                rc = OCASTATUS_NOT_IMPLEMENTED;
                break;
            default:
                rc = OCASTATUS_BAD_METHOD;
                break;
            }
        }
        else
        {
            // Should be executed on higher level
            rc = OcaLiteAgent::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

::OcaClassVersionNumber OcaLiteStreamNetwork::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteAgent::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

::OcaBoolean OcaLiteStreamNetwork::AddSignalSourceSink(::OcaLiteNetworkSignalChannel& signalChannel)
{
    bool bResult(false);
    if (signalChannel.GetSourceOrSink() == OCANETWORKMEDIASOURCEORSINK_SOURCE)
    {
        m_signalSource.Add(signalChannel.GetObjectNumber());
        bResult = true;
    }
    else if (signalChannel.GetSourceOrSink() == OCANETWORKMEDIASOURCEORSINK_SINK)
    {
        m_signalSink.Add(signalChannel.GetObjectNumber());
        bResult = true;
    }
    return bResult;
}

::OcaBoolean OcaLiteStreamNetwork::AddConnectorSourceSink(::OcaLiteStreamConnector& connector)
{
    bool bResult(false);
    ::OcaLiteNetworkMediaSourceOrSink sourceOrSink;
    if (OCASTATUS_OK == connector.GetSourceOrSink(sourceOrSink))
    {
        if (sourceOrSink == OCANETWORKMEDIASOURCEORSINK_SOURCE)
        {
            m_connectorsSource.Add(connector.GetObjectNumber());
            bResult = true;

            ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_STREAM_CONNECTORS_SOURCE));
            ::OcaLitePropertyChangedEventData< ::OcaLiteList< ::OcaONo> > eventData(GetObjectNumber(),
                                                                                   propertyID,
                                                                                   m_connectorsSource,
                                                                                   OCAPROPERTYCHANGETYPE_ITEM_ADDED);
            PropertyChanged(eventData, propertyID);
        }
        else if (sourceOrSink == OCANETWORKMEDIASOURCEORSINK_SINK)
        {
            m_connectorsSink.Add(connector.GetObjectNumber());
            bResult = true;

            ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_STREAM_CONNECTORS_SINK));
            ::OcaLitePropertyChangedEventData< ::OcaLiteList< ::OcaONo> > eventData(GetObjectNumber(),
                                                                                   propertyID,
                                                                                   m_connectorsSink,
                                                                                   OCAPROPERTYCHANGETYPE_ITEM_ADDED);
            PropertyChanged(eventData, propertyID);
        }
    }
    return bResult;
}

::OcaBoolean OcaLiteStreamNetwork::RemoveConnectorSourceSink(::OcaLiteStreamConnector& connector)
{
    bool bResult(false);
    ::OcaLiteNetworkMediaSourceOrSink sourceOrSink;
    if (OCASTATUS_OK == connector.GetSourceOrSink(sourceOrSink))
    {
        if (sourceOrSink == OCANETWORKMEDIASOURCEORSINK_SOURCE)
        {
            m_connectorsSource.RemoveElement(connector.GetObjectNumber());
            bResult = true;

            ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_STREAM_CONNECTORS_SOURCE));
            ::OcaLitePropertyChangedEventData< ::OcaLiteList< ::OcaONo> > eventData(GetObjectNumber(),
                propertyID,
                m_connectorsSource,
                OCAPROPERTYCHANGETYPE_ITEM_ADDED);
            PropertyChanged(eventData, propertyID);
        }
        else if (sourceOrSink == OCANETWORKMEDIASOURCEORSINK_SINK)
        {
            m_connectorsSink.RemoveElement(connector.GetObjectNumber());
            bResult = true;

            ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_STREAM_CONNECTORS_SINK));
            ::OcaLitePropertyChangedEventData< ::OcaLiteList< ::OcaONo> > eventData(GetObjectNumber(),
                propertyID,
                m_connectorsSink,
                OCAPROPERTYCHANGETYPE_ITEM_ADDED);
            PropertyChanged(eventData, propertyID);
        }
    }
    return bResult;
}



