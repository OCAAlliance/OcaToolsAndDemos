/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteStreamNetwork
 *
 */
// ---- Include system wide include files ----
#include <assert.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlClasses/Managers/OcaLiteNetworkManager.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteStreamNetwork.h"
#include "OcaLiteNetworkSignalChannel.h"

// ---- Referenced classes and types ---

// ---- Helper types and constants ----
static const ::OcaUint16        classID[]   = {OCA_STREAMNETWORK_CLASSID};
const ::OcaLiteClassID          OcaLiteStreamNetwork::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     static_cast< ::OcaClassVersionNumber>(0)

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteStreamNetwork::OcaLiteStreamNetwork(::OcaONo objectNumber, 
                                           ::OcaBoolean lockable,
                                           const ::OcaLiteString& role, 
                                           ::OcaLiteNetworkLinkType linkType,
                                           const ::OcaLiteNetworkNodeID& idAdvertised, 
                                           ::OcaLiteNetworkControlProtocol controlProtocol,
                                           ::OcaLiteNetworkMediaProtocol mediaProtocol)
    : ::OcaLiteAgent(objectNumber, lockable, role),
    m_networkLinkType(linkType),
    m_idAdvertised(idAdvertised),
    m_networkControlProtocol(controlProtocol),
    m_networkMediaProtocol(mediaProtocol),
    m_signalSource(),
    m_signalSink()

{
    assert(linkType < OCANETWORKLINKTYPE_MAXIMUM);
    assert(controlProtocol < OCANETWORKCONTROLPROTOCOL_MAXIMUM);
    assert(mediaProtocol < OCANETWORKMEDIAPROTOCOL_MAXIMUM);
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
    m_signalSource.Clear();
    m_signalSink.Clear();
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
			case GET_SYSTEM_INTERFACES:
				{
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + m_interfaces.GetSize(writer));
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
            case GET_STATUS:
                // TODO, return status
                rc = OCASTATUS_NOT_IMPLEMENTED;
                break;
            case GET_STATISTICS:
            case RESET_STATISTICS:
            case SET_SYSTEM_INTERFACES:
            case GET_STREAM_CONNECTORS_SOURCE:
            case SET_STREAM_CONNECTORS_SOURCE:
            case GET_STREAM_CONNECTORS_SINK:
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

//lint -e{835} A zero has been given as right argument to operator '+'
::OcaClassVersionNumber OcaLiteStreamNetwork::GetClassVersion() const
{
    return (OcaLiteAgent::GetClassVersion() + CLASS_VERSION_INCREMENT);
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

