/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteNetwork
 *
 */
// ---- Include system wide include files ----
#include <assert.h>
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <OCC/ControlClasses/Managers/OcaLiteNetworkManager.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLiteNetworkStatistics.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- Include local include files ----
#include "OcaLiteNetwork.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_NETWORK_CLASSID};
const ::OcaLiteClassID          OcaLiteNetwork::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteNetwork::OcaLiteNetwork(::OcaONo objectNumber,
                               ::OcaBoolean lockable,
                               const ::OcaLiteString& role,
                               ::OcaLiteNetworkLinkType linkType,
                               const ::OcaLiteNetworkNodeID& idAdvertised,
                               ::OcaLiteNetworkControlProtocol controlProtocol,
                               const ::OcaLiteNetworkSystemInterfaceID& systemInterface)
    : ::OcaLiteAgent(objectNumber, lockable, role),
      m_networkLinkType(linkType),
      m_idAdvertised(idAdvertised),
      m_networkControlProtocol(controlProtocol),
      m_interface(systemInterface)
{
    assert(linkType < OCANETWORKLINKTYPE_MAXIMUM);
    assert(controlProtocol < OCANETWORKCONTROLPROTOCOL_MAXIMUM);
}

OcaLiteNetwork::~OcaLiteNetwork()
{
}

::OcaBoolean OcaLiteNetwork::Initialize()
{
    return ::OcaLiteNetworkManager::GetInstance().AddNetwork(*this);
}

void OcaLiteNetwork::Teardown()
{
    ::OcaLiteNetworkManager::GetInstance().RemoveNetwork(*this);
}

::OcaLiteStatus OcaLiteNetwork::GetSystemInterfaces(::OcaLiteList< ::OcaLiteNetworkSystemInterfaceID>& interfaces) const
{
    ::OcaLiteStatus rc(GetSystemInterfacesValue(interfaces));
    return rc;
}

::OcaLiteStatus OcaLiteNetwork::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
                        ::OcaLiteNetworkNodeID nodeId;
                        rc = GetIdAdvertisedValue(nodeId);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + nodeId.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                nodeId.Marshal(&pResponse, writer);

                                *response = responseBuffer;

                                rc = OCASTATUS_OK;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
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
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + ::GetSizeValue< ::OcaLiteNetworkMediaProtocol>(OCANETWORKMEDIAPROTOCOL_NONE, writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            ::MarshalValue< ::OcaLiteNetworkMediaProtocol>(OCANETWORKMEDIAPROTOCOL_NONE, &pResponse, writer);

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
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteNetworkStatus status;
                        rc = GetStatus(status);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + ::GetSizeValue< ::OcaLiteNetworkStatus>(status, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaLiteNetworkStatus>(status, &pResponse, writer);

                                *response = responseBuffer;

                                rc = OCASTATUS_OK;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
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
                        ::OcaLiteList< ::OcaLiteNetworkSystemInterfaceID> interfaces;
                        rc = GetSystemInterfaces(interfaces);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + interfaces.GetSize(writer));
                            responseBuffer  = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                interfaces.Marshal(&pResponse, writer);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
                break;
            case STARTUP:
            case SHUTDOWN:
                rc = OCASTATUS_INVALID_REQUEST;
                break;
            case GET_MEDIA_PORTS:
            case SET_ID_ADVERTISED:
            case SET_SYSTEM_INTERFACES:
            case GET_STATISTICS:
            case RESET_STATISTICS:
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

::OcaClassVersionNumber OcaLiteNetwork::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteAgent::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

::OcaLiteStatus OcaLiteNetwork::GetSystemInterfacesValue(::OcaLiteList< ::OcaLiteNetworkSystemInterfaceID>& interfaces) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);
    interfaces.Clear();
    interfaces.Add(m_interface);
    return rc;
}

::OcaLiteStatus OcaLiteNetwork::GetIdAdvertisedValue(::OcaLiteNetworkNodeID& nodeId) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);
    nodeId = m_idAdvertised;
    return rc;
}

void OcaLiteNetwork::SystemInterfaceIDsChanged(const ::OcaLiteList< ::OcaLiteNetworkSystemInterfaceID>& interfaces)
{
    ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_SYSTEM_INTERFACES));
    ::OcaLitePropertyChangedEventData< ::OcaLiteList< ::OcaLiteNetworkSystemInterfaceID> > eventData(GetObjectNumber(),
                                                                                                     propertyID,
                                                                                                     interfaces,
                                                                                                     OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
    PropertyChanged(eventData, propertyID);
}
