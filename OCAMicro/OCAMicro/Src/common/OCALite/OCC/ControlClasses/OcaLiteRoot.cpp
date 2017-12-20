/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteRoot
 *
 */
// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <OCC/ControlClasses/Managers/OcaLiteDeviceManager.h>
#include <OCC/ControlDataTypes/OcaLiteClassIdentification.h>
#include <OCC/ControlDataTypes/OcaLiteEventData.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteRoot.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_ROOT_CLASSID};
const ::OcaLiteClassID          OcaLiteRoot::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the class version of this class. */
#define CLASS_VERSION       static_cast< ::OcaClassVersionNumber>(2)

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteRoot::OcaLiteRoot(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role)
    : m_objectNumber(objectNumber),
      m_lockable(lockable),
      m_role(role),
      m_lockHolder(OCA_INVALID_SESSIONID),
      m_pPropertyChangedEventDelegate(NULL)
{
}

OcaLiteRoot::~OcaLiteRoot()
{
    m_pPropertyChangedEventDelegate = NULL;
}

::OcaLiteStatus OcaLiteRoot::GetClassIdentification(::OcaLiteClassIdentification& classIdentification) const
{
    classIdentification = ::OcaLiteClassIdentification(GetClassID(), GetClassVersion());
    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteRoot::GetLockable(::OcaBoolean& lockable) const
{
    lockable = m_lockable;
    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteRoot::Lock(::OcaSessionID sessionID)
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    if (m_lockable)
    {
        if (OCA_INVALID_SESSIONID != sessionID)
        {
            if (OCA_INVALID_SESSIONID == m_lockHolder)
            {
                m_lockHolder = sessionID;
            }
            else if (m_lockHolder != sessionID)
            {
                // Locked by another session.
                rc = OCASTATUS_LOCKED;
            }
        }
        else
        {
            rc = OCASTATUS_PARAMETER_ERROR;
        }
    }
    else
    {
        rc = OCASTATUS_INVALID_REQUEST;
    }

    return rc;
}

::OcaLiteStatus OcaLiteRoot::Unlock(::OcaSessionID sessionID)
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    if (m_lockable)
    {
        if (OCA_INVALID_SESSIONID != sessionID)
        {
            if (OCA_INVALID_SESSIONID == m_lockHolder)
            {
                // Not locked. This is not a real problem.
            }
            else if (m_lockHolder == sessionID)
            {
                m_lockHolder = OCA_INVALID_SESSIONID;
            }
            else if (m_lockHolder != sessionID)
            {
                // Locked by another session.
                rc = OCASTATUS_LOCKED;
            }
        }
        else
        {
            rc = OCASTATUS_PARAMETER_ERROR;
        }
    }
    else
    {
        rc = OCASTATUS_INVALID_REQUEST;
    }
    return rc;
}

::OcaLiteStatus OcaLiteRoot::GetRole(::OcaLiteString& role) const
{
    role = m_role;
    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteRoot::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_CLASS_IDENTIFICATION:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteClassIdentification classIdentification;
                        rc = GetClassIdentification(classIdentification);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     classIdentification.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                classIdentification.Marshal(&pResponse, writer);

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
            case GET_LOCKABLE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaBoolean lockable;
                        rc = GetLockable(lockable);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     ::GetSizeValue< ::OcaBoolean>(lockable, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaBoolean>(lockable, &pResponse, writer);

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
            case LOCK:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        rc = Lock(sessionID);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(0), writer));
                            responseBuffer  = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
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
            case UNLOCK:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        rc = Unlock(sessionID);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(0), writer));
                            responseBuffer  = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
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
            case GET_ROLE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteString role;
                        rc = GetRole(role);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     role.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                role.Marshal(&pResponse, writer);

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
            case LOCK_READONLY:
                rc = OCASTATUS_NOT_IMPLEMENTED;
                break;
            default:
                rc = OCASTATUS_BAD_METHOD;
                break;
            }
        }
        else
        {
            // There is nothing higher for root
            rc = OCASTATUS_DEVICE_ERROR;
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

void OcaLiteRoot::SessionLost(::OcaSessionID sessionID)
{
    if (m_lockHolder == sessionID)
    {
        m_lockHolder = OCA_INVALID_SESSIONID;
    }
}

::OcaBoolean OcaLiteRoot::IsLocked(::OcaSessionID sessionID) const
{
    // Check if the object itself is locked
    bool result((OCA_INVALID_SESSIONID != m_lockHolder) &&
                (m_lockHolder != sessionID));

    // Check if the device manager is locked. If it is, it must also be handled
    // like this object is locked.
    if (GetObjectNumber() != ::OcaLiteDeviceManager::OBJECT_NUMBER)
    {
        result = result || static_cast<bool>(::OcaLiteDeviceManager::GetInstance().IsLocked(sessionID));
    }

    return static_cast< ::OcaBoolean>(result);
}

::OcaBoolean OcaLiteRoot::HasLock() const
{
    // Check if the object itself has a lock
    bool result(OCA_INVALID_SESSIONID != m_lockHolder);

    // Check if the device manager has a lock. If it has, it must also be handled
    // like this object has a lock.
    if (GetObjectNumber() != ::OcaLiteDeviceManager::OBJECT_NUMBER)
    {
        result = result || static_cast<bool>(::OcaLiteDeviceManager::GetInstance().HasLock());
    }

    return static_cast< ::OcaBoolean>(result);
}

::OcaBoolean OcaLiteRoot::AddEventSubscription(const ::OcaLiteEventID& eventID,
                                           IEventDelegate& eventDelegate)
{
    bool bSuccess(false);

    if (eventID.GetDefLevel() == CLASS_ID.GetFieldCount())
    {
        switch (eventID.GetEventIndex())
        {
        case OCA_EVENT_PROPERTY_CHANGED:
            if (NULL == m_pPropertyChangedEventDelegate)
            {
                m_pPropertyChangedEventDelegate = &eventDelegate;
                bSuccess = true;
            }
            else
            {
                OCA_LOG_WARNING("Trying to subscribe event delegate when already subscribed");
            }
            break;
        default:
            OCA_LOG_WARNING("Invalid event id");
            break;
        }
    }

    return static_cast< ::OcaBoolean>(bSuccess);
}

void OcaLiteRoot::RemoveEventSubscription(const ::OcaLiteEventID& eventID)
{
    if (eventID.GetDefLevel() == CLASS_ID.GetFieldCount())
    {
        switch (eventID.GetEventIndex())
        {
        case OCA_EVENT_PROPERTY_CHANGED:
            m_pPropertyChangedEventDelegate = NULL;
            break;
        default:
            OCA_LOG_WARNING("Invalid event id");
            break;
        }
    }
}

void OcaLiteRoot::PropertyChanged(const ::OcaLiteEventData& eventData)
{
    if (NULL != m_pPropertyChangedEventDelegate)
    {
        m_pPropertyChangedEventDelegate->OnEvent(eventData);
    }
}

::OcaClassVersionNumber OcaLiteRoot::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(CLASS_VERSION);
}
