/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteApplicationNetwork
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
#include "OcaLiteApplicationNetwork.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_APPLICATION_NETWORK_CLASSID};
const ::OcaLiteClassID          OcaLiteApplicationNetwork::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT    -1

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteApplicationNetwork::OcaLiteApplicationNetwork(::OcaONo objectNumber,
                                                     ::OcaBoolean lockable,
                                                     const ::OcaLiteString& role)
    : ::OcaLiteRoot(objectNumber, lockable, role)
{
}

OcaLiteApplicationNetwork::~OcaLiteApplicationNetwork()
{
}

::OcaLiteStatus OcaLiteApplicationNetwork::GetServiceID(::OcaLiteApplicationNetworkServiceID& name) const
{
    return GetServiceIDValue(name);
}

::OcaLiteStatus OcaLiteApplicationNetwork::GetSystemInterfaces(::OcaLiteList< ::OcaLiteNetworkSystemInterfaceDescriptor>& interfaces) const
{
    return GetSystemInterfacesValue(interfaces);
}

::OcaLiteStatus OcaLiteApplicationNetwork::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_SERVICE_ID:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteApplicationNetworkServiceID serviceId;
                        rc = GetServiceID(serviceId);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + serviceId.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                serviceId.Marshal(&pResponse, writer);

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
                        ::OcaLiteList< ::OcaLiteNetworkSystemInterfaceDescriptor> systemInterfaceDescriptor;
                        rc = GetSystemInterfacesValue(systemInterfaceDescriptor);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + systemInterfaceDescriptor.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                systemInterfaceDescriptor.Marshal(&pResponse, writer);

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
            case GET_STATE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint8 state(static_cast< ::OcaUint8>(OCAAPPLICATIONNETWORKSTATE_READY)); // We are always ready for this.
                        rc = OCASTATUS_OK;

                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + ::GetSizeValue< ::OcaUint8>(state, writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            writer.Write(state, &pResponse);

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
            case SET_SERVICE_ID:
            case GET_ERROR_CODE:
            case GET_LABEL:
            case SET_LABEL:
            case GET_OWNER:
            case CONTROL:
            case GET_PATH:
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
            rc = OcaLiteRoot::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }
    
    return rc;
}

::OcaClassVersionNumber OcaLiteApplicationNetwork::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteRoot::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

::OcaLiteStatus OcaLiteApplicationNetwork::GetSystemInterfacesValue(::OcaLiteList< ::OcaLiteNetworkSystemInterfaceDescriptor>& interfaces) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);
    interfaces.Clear();
    if (::OcaLiteNetworkSystemInterfaceDescriptor() != m_interface)
    {
        interfaces.Add(m_interface);
    }
    return rc;
}

::OcaLiteStatus OcaLiteApplicationNetwork::GetServiceIDValue(::OcaLiteApplicationNetworkServiceID& serviceId) const
{
    serviceId = m_serviceId;
    return OCASTATUS_OK;
}

void OcaLiteApplicationNetwork::SystemInterfaceIDsChanged(const ::OcaLiteList< ::OcaLiteNetworkSystemInterfaceDescriptor>& interfaces)
{
    // Store the new value for queries
    if (interfaces.GetCount() == static_cast< ::OcaUint16>(1))
    {
        m_interface = interfaces.GetItem(static_cast< ::OcaUint16>(1));
    }
    else
    {
        m_interface = ::OcaLiteNetworkSystemInterfaceDescriptor();
    }

    ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_SYSTEM_INTERFACES));
    ::OcaLitePropertyChangedEventData< ::OcaLiteList< ::OcaLiteNetworkSystemInterfaceDescriptor> > eventData(GetObjectNumber(),
                                                                                                             propertyID,
                                                                                                             interfaces,
                                                                                                             OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
    PropertyChanged(eventData, propertyID);
}

void OcaLiteApplicationNetwork::ServiceIDChanged(const ::OcaLiteApplicationNetworkServiceID& serviceId)
{
    // Store the new value for queries
    m_serviceId = serviceId;

    ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_SERVICE_ID));
    ::OcaLitePropertyChangedEventData< ::OcaLiteApplicationNetworkServiceID> eventData(GetObjectNumber(),
                                                                                       propertyID,
                                                                                       m_serviceId,
                                                                                       OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
    PropertyChanged(eventData, propertyID);
}
