/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : The OcaLiteDeviceManager.
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <HostInterfaceLite/OCA/OCF/Configuration/IOcfLiteConfigure.h>
#include <OCALite/OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlClasses/Managers/OcaLiteNetworkManager.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLiteModelGUID.h>
#include <OCC/ControlDataTypes/OcaLiteModelDescription.h>
#include <OCC/ControlDataTypes/OcaLiteManagerDescriptor.h>
#include <OCC/ControlDataTypes/OcaLitePropertyID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- Include local include files ----
#include "OcaLiteDeviceManager.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_DEVICEMANAGER_CLASSID};
const ::OcaLiteClassID          OcaLiteDeviceManager::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     static_cast< ::OcaClassVersionNumber>(0)

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

::OcaLiteDeviceManager* OcaLiteDeviceManager::m_pSingleton(NULL);
const ::OcaONo OcaLiteDeviceManager::OBJECT_NUMBER(static_cast< ::OcaONo>(1));

OcaLiteDeviceManager::OcaLiteDeviceManager()
  : ::OcaLiteManager(OBJECT_NUMBER, ::OcaLiteString("DeviceManager"), ::OcaLiteString("DeviceManager")),
    m_managerObjects(),
    m_bErrorState(static_cast< ::OcaBoolean>(false)),
    m_operationalState(OCA_OPSTATE_INITIALIZING)
{
}

OcaLiteDeviceManager::~OcaLiteDeviceManager()
{
}

::OcaLiteDeviceManager& OcaLiteDeviceManager::GetInstance()
{
    if (NULL == OcaLiteDeviceManager::m_pSingleton)
    {
        OcaLiteDeviceManager::m_pSingleton = new ::OcaLiteDeviceManager;
    }

    return *OcaLiteDeviceManager::m_pSingleton;
}

void OcaLiteDeviceManager::FreeInstance()
{
    if (OcaLiteDeviceManager::m_pSingleton != NULL)
    {
        delete m_pSingleton;
        m_pSingleton = NULL;
    }
}

::OcaLiteStatus OcaLiteDeviceManager::GetOcaVersion(::OcaUint16& ocaVersion) const
{
    ocaVersion = static_cast< ::OcaUint16>(1/*OCA_PROTOCOL_VERSION*/);
    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteDeviceManager::GetModelGUID(::OcaLiteModelGUID& modelGUID) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    UINT32 mnfrCode(0);
    UINT32 modelCode(0);
    if (OcfLiteConfigureGetModelGUID(mnfrCode, modelCode))
    {
        ::OcaUint64 guid(static_cast< ::OcaUint64>(mnfrCode) << (OCA_MODEL_GUID_MODEL_CODE_LENGTH * static_cast< ::OcaUint16>(8)));
        guid += static_cast< ::OcaUint64>(modelCode);
        modelGUID = ::OcaLiteModelGUID(guid);
    }
    else
    {
        rc = OCASTATUS_DEVICE_ERROR;
    }
    
    return rc;
}

::OcaLiteStatus OcaLiteDeviceManager::GetSerialNumber(::OcaLiteString& serialNumber) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    serialNumber = ::OcaLiteString(OcfLiteConfigureGetSerialNumber().c_str());
    
    return rc;
}

::OcaLiteStatus OcaLiteDeviceManager::GetDeviceName(::OcaLiteString& deviceName) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);
   
    deviceName = ::OcaLiteString(OcfLiteConfigureGetDeviceName().c_str());
   
    return rc;
}

::OcaLiteStatus OcaLiteDeviceManager::GetModelDescription(::OcaLiteModelDescription& modelDescription) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    std::string manufacturer;
    std::string name;
    std::string version;
    if (OcfLiteConfigureGetModelDescription(manufacturer, name, version))
    {
        modelDescription = ::OcaLiteModelDescription(::OcaLiteString(manufacturer.c_str()),
                                                     ::OcaLiteString(name.c_str()),
                                                     ::OcaLiteString(version.c_str()));
    }
    else
    {
        rc = OCASTATUS_DEVICE_ERROR;
    }

    return rc;
}

::OcaLiteStatus OcaLiteDeviceManager::SetEnabled(::OcaBoolean enabled)
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    bool bEnabled(static_cast<bool>(enabled));
    bool bCurrentEnabled = OcfLiteConfigureGetEnabled();
    if ((bCurrentEnabled && !enabled) ||
        (!bCurrentEnabled && enabled))
    {
        if (::OcfLiteConfigureSetEnabled(bEnabled))
        {
            ::OcaLitePropertyID propertyId(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_ENABLED));
            ::OcaLitePropertyChangedEventData< ::OcaBoolean> eventData(OBJECT_NUMBER,
                                                                       propertyId,
                                                                       static_cast< ::OcaBoolean>(::OcfLiteConfigureGetEnabled()),
                                                                       OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
            PropertyChanged(eventData);

            ::OcaLiteDeviceState state;
            if (OCASTATUS_OK == GetState(state))
            {
                OcaLitePropertyID propertyIdState(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_STATE));
                ::OcaLitePropertyChangedEventData< ::OcaLiteDeviceState> eventDataState(OBJECT_NUMBER,
                                                                                        propertyIdState,
                                                                                        state,
                                                                                        OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
                PropertyChanged(eventDataState);
            }
        }
        else
        {
            rc = OCASTATUS_PROCESSING_FAILED;
        }
    }
    return rc;
}

::OcaBoolean OcaLiteDeviceManager::AddManager(::OcaLiteManager& rObject)
{
    ::OcaBoolean bResult(static_cast< ::OcaBoolean>(false));

    OcaManagerList::const_iterator iter(m_managerObjects.find(rObject.GetObjectNumber()));
    if (m_managerObjects.end() == iter)
    {
        static_cast<void>(m_managerObjects.insert(OcaManagerList::value_type(rObject.GetObjectNumber(), &rObject)));
        bResult = static_cast< ::OcaBoolean>(true);

        // We are OCALite, don't expect dynamic objects in this implementation.
        assert(::OcaLiteDeviceManager::GetInstance().GetOperationalState() == ::OcaLiteDeviceManager::OCA_OPSTATE_INITIALIZING);
    }
    return bResult;
}

void OcaLiteDeviceManager::RemoveManager(const ::OcaLiteManager& rObject)
{
    ::OcaONo oNo(rObject.GetObjectNumber());
    OcaManagerList::const_iterator iter(m_managerObjects.find(oNo));
    if (m_managerObjects.end() != iter)
    {
        static_cast<void>(m_managerObjects.erase(oNo));  //lint !e792 Void cast not needed for certain STL implementations

        // We are OCALite, don't expect dynamic objects in this implementation.
        assert(::OcaLiteDeviceManager::GetInstance().GetOperationalState() == ::OcaLiteDeviceManager::OCA_OPSTATE_SHUTTING_DOWN);
    }
}

::OcaLiteManager* OcaLiteDeviceManager::GetManager(::OcaONo oNo)
{
    ::OcaLiteManager* pManager(NULL);

    OcaManagerList::const_iterator iter(m_managerObjects.find(oNo));
    if (m_managerObjects.end() != iter)
    {
        assert(NULL != iter->second);
        pManager = iter->second;
    }

    return pManager;
}

::OcaBoolean OcaLiteDeviceManager::GetErrorState() const
{
    return m_bErrorState;
}

void OcaLiteDeviceManager::SetErrorState(::OcaBoolean error)
{
    if ((!m_bErrorState && error) ||
        (m_bErrorState && !error))
    {
        m_bErrorState = error;

        ::OcaLiteDeviceState state;
        if (OCASTATUS_OK == GetState(state))
        {
            ::OcaLitePropertyID propertyIdState(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_STATE));
            ::OcaLitePropertyChangedEventData< ::OcaLiteDeviceState> eventDataState(OBJECT_NUMBER,
                                                                                     propertyIdState,
                                                                                     state,
                                                                                     OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
            PropertyChanged(eventDataState);
        }
    }
}

::OcaLiteDeviceManager::OperationalState OcaLiteDeviceManager::GetOperationalState() const
{
    return m_operationalState;
}

void OcaLiteDeviceManager::SetOperationalState(OperationalState state)
{
    if (m_operationalState != state)
    {
        m_operationalState = state;

        ::OcaLiteDeviceState getState;
        if (OCASTATUS_OK == GetState(getState))
        {
            ::OcaLitePropertyID propertyIdState(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_STATE));
            ::OcaLitePropertyChangedEventData< ::OcaLiteDeviceState> eventDataState(OBJECT_NUMBER,
                                                                                     propertyIdState,
                                                                                     getState,
                                                                                     OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
            PropertyChanged(eventDataState);
        }
    }
}

void OcaLiteDeviceManager::SetErrorAndOperationalState(::OcaBoolean error, OperationalState state)
{
    if ((!m_bErrorState && error) ||
        (m_bErrorState && !error) ||
        (m_operationalState != state))
    {
        m_bErrorState = error;
        m_operationalState = state;

        ::OcaLiteDeviceState getState;
        if (OCASTATUS_OK == GetState(getState))
        {
            ::OcaLitePropertyID propertyIdState(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_STATE));
            ::OcaLitePropertyChangedEventData< ::OcaLiteDeviceState> eventDataState(OBJECT_NUMBER,
                                                                                     propertyIdState,
                                                                                     getState,
                                                                                     OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
            PropertyChanged(eventDataState);
        }
    }
}

::OcaLiteStatus OcaLiteDeviceManager::Lock(::OcaSessionID sessionID)
{
    ::OcaLiteStatus rc(OCASTATUS_INVALID_REQUEST);

    // Check if there is an object that is locked by another session.
    // If there is, the device manager may not be locked

    bool objectLocked(false);

    // Check the managers
    for (OcaManagerList::iterator iter(m_managerObjects.begin()); m_managerObjects.end() != iter; ++iter)
    {
        assert(NULL != iter->second);

        if (OBJECT_NUMBER != iter->second->GetObjectNumber())
        {
            objectLocked = objectLocked && iter->second->IsLocked(sessionID);
        }
    }

    // Check the other objects
    ::OcaLiteList< ::OcaLiteObjectIdentification> members;
    ::OcaLiteStatus getResult(::OcaLiteBlock::GetRootBlock().GetMembers(members));
    if (OCASTATUS_OK == getResult)
    {
        for (::OcaUint16 m(static_cast< ::OcaUint16>(0)); m < members.GetCount(); m++)
        {
            const ::OcaLiteObjectIdentification& member(members.GetItem(m));
            ::OcaLiteRoot* pMember(::OcaLiteBlock::GetRootBlock().GetObject(member.GetONo()));
            if (NULL != pMember)
            {
                objectLocked = objectLocked && pMember->IsLocked(sessionID);
            }
        }
    }

    if (!objectLocked)
    {
        rc = OcaLiteManager::Lock(sessionID);
    }

    return rc;
}

void OcaLiteDeviceManager::SessionLost(::OcaSessionID sessionID)
{
    for (OcaManagerList::iterator iter(m_managerObjects.begin()); m_managerObjects.end() != iter; ++iter)
    {
        assert(NULL != iter->second);

        if (OBJECT_NUMBER != iter->second->GetObjectNumber())
        {
            iter->second->SessionLost(sessionID);
        }
    }

    OcaLiteManager::SessionLost(sessionID);
}

::OcaLiteStatus OcaLiteDeviceManager::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_OCA_VERSION:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint16 version;
                        rc = GetOcaVersion(version);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     ::GetSizeValue< ::OcaUint16>(version, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaUint16>(version, &pResponse, writer);

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
            case GET_MODEL_GUID:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteModelGUID modelGUID;
                        rc = GetModelGUID(modelGUID);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     modelGUID.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                modelGUID.Marshal(&pResponse, writer);

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
            case GET_SERIAL_NUMBER:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteString serialNumber;
                        rc = GetSerialNumber(serialNumber);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     serialNumber.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                serialNumber.Marshal(&pResponse, writer);

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
            case GET_DEVICE_NAME:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteString deviceName;
                        rc = GetDeviceName(deviceName);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     deviceName.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                deviceName.Marshal(&pResponse, writer);

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
            case GET_MODEL_DESCRIPTION:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteModelDescription description;
                        rc = GetModelDescription(description);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     description.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                description.Marshal(&pResponse, writer);

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
            case GET_ENABLED:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaBoolean enabled;
                        rc = GetEnabled(enabled);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     ::GetSizeValue< ::OcaBoolean>(enabled, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaBoolean>(enabled, &pResponse, writer);

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
            case SET_ENABLED:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaBoolean enabled;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        reader.Read(bytesLeft, &pCmdParameters, enabled))
                    {
                        rc = SetEnabled(enabled);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(0), writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(0/*NrParameters*/), &pResponse);

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
            case GET_STATE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteDeviceState state;
                        rc = GetState(state);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     ::GetSizeValue< ::OcaLiteDeviceState>(state, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaLiteDeviceState>(state, &pResponse, writer);
                                
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
            case GET_MANAGERS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaLiteManagerDescriptor> managers;
                        rc = GetManagers(managers);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     managers.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                managers.Marshal(&pResponse, writer);

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
            case SET_DEVICE_NAME:
            case GET_DEVICE_ROLE:
            case SET_DEVICE_ROLE:
            case GET_USER_INVENTORY_CODE:
            case SET_USER_INVENTORY_CODE:
            case SET_RESET_KEY:
            case GET_RESET_CAUSE:
            case CLEAR_RESET_CAUSE:
            case GET_MESSAGE:
            case SET_MESSAGE:
                rc = OCASTATUS_NOT_IMPLEMENTED;
                break;
            default:
                rc = OCASTATUS_BAD_METHOD;
                break;
            }
        }
        else
        {
            rc = OcaLiteManager::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

//lint -e{835} A zero has been given as right argument to operator '+'
::OcaClassVersionNumber OcaLiteDeviceManager::GetClassVersion() const
{
    return (OcaLiteManager::GetClassVersion() + CLASS_VERSION_INCREMENT);
}

::OcaLiteStatus OcaLiteDeviceManager::GetEnabled(::OcaBoolean& enabled) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    enabled = static_cast< ::OcaBoolean>(OcfLiteConfigureGetEnabled());
   
    return rc;
}

::OcaLiteStatus OcaLiteDeviceManager::GetState(::OcaLiteDeviceState& state) const
{
    ::OcaBoolean currentEnabled;
    ::OcaLiteStatus rc(GetEnabled(currentEnabled));

    if (OCASTATUS_OK == rc)
    {
        switch (m_operationalState)
        {
        case OCA_OPSTATE_OPERATIONAL:
            state = OCADEVICESTATE_OPERATIONAL;
            break;
        case OCA_OPSTATE_INITIALIZING:
            state = OCADEVICESTATE_INITIALIZING;
            break;
        case OCA_OPSTATE_UPDATING:
            state = OCADEVICESTATE_UPDATING;
            break;
        case OCA_OPSTATE_SHUTTING_DOWN:
            state = OCADEVICESTATE_OPERATIONAL;
            currentEnabled = static_cast< ::OcaBoolean>(false);
            break;
        default:
            rc = OCASTATUS_DEVICE_ERROR;
            break;
        }
    }

    if (OCASTATUS_OK == rc)
    {
        if (!currentEnabled)
        {
            state = static_cast< ::OcaLiteDeviceState>(static_cast< ::OcaUint32>(state) | static_cast< ::OcaUint32>(OCADEVICESTATE_DISABLED));
        }
        if (m_bErrorState)
        {
            state = static_cast< ::OcaLiteDeviceState>(static_cast< ::OcaUint32>(state) | static_cast< ::OcaUint32>(OCADEVICESTATE_ERROR));
        }
    }

    return rc;
}

::OcaLiteStatus OcaLiteDeviceManager::GetManagers(::OcaLiteList< ::OcaLiteManagerDescriptor>& managers) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    managers.Clear();

    OcaManagerList::const_iterator iter(m_managerObjects.begin());
    while (m_managerObjects.end() != iter)
    {
        ::OcaLiteClassIdentification classIdentification;
        assert(NULL != iter->second);
            
        if (OCASTATUS_OK == iter->second->GetClassIdentification(classIdentification))
        {
            assert(iter->first == iter->second->GetObjectNumber());

            managers.Add(::OcaLiteManagerDescriptor(iter->second->GetObjectNumber(),
                                                    iter->second->GetName(),
                                                    classIdentification.GetClassID(),
                                                    classIdentification.GetClassVersion()));
        }
        else
        {
            rc = OCASTATUS_DEVICE_ERROR;
            break;
        }
        ++iter;
    }

    return rc;
}

// ---- Function Implementation ----

//lint -save -e1576 Explicit specialization does not occur in the same file as corresponding function template

template <>
void MarshalValue< ::OcaLiteDeviceState>(const ::OcaLiteDeviceState& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint16>(static_cast< ::OcaUint16>(value), destination, writer);
}

template <>
bool UnmarshalValue< ::OcaLiteDeviceState>(::OcaLiteDeviceState& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint16 deviceState(static_cast< ::OcaUint16>(0));
    bool result(reader.Read(bytesLeft, source, deviceState));
    if (result)
    {
        value = static_cast< ::OcaLiteDeviceState>(deviceState);
    }

    return result;
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteDeviceState>(const ::OcaLiteDeviceState& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint16>(static_cast< ::OcaUint16>(value), writer);
}

//lint -restore
