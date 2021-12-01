/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : The OcaLiteSubscriptionManager.
 *
 */

// ---- Include system wide include files ----
#include <OCC/ControlClasses/Managers/OcaLiteDeviceManager.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlDataTypes/OcaLiteEvent.h>
#include <OCC/ControlDataTypes/OcaLiteEventData.h>
#include <OCC/ControlDataTypes/OcaLiteEventID.h>
#include <OCC/ControlDataTypes/OcaLiteMethod.h>
#include <OCC/ControlDataTypes/OcaLiteNetworkAddress.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- Include local include files ----
#include "OcaLiteSubscriptionManager.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_SUBSCRIPTION_MANAGER_CLASSID};
const ::OcaLiteClassID          OcaLiteSubscriptionManager::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

/** The singleton reference to the server. */
::OcaLiteSubscriptionManager* OcaLiteSubscriptionManager::m_pSingleton(NULL);
const ::OcaONo OcaLiteSubscriptionManager::OBJECT_NUMBER(static_cast< ::OcaONo>(4));

OcaLiteSubscriptionManager::OcaLiteSubscriptionManager()
    : ::OcaLiteManager(OBJECT_NUMBER, ::OcaLiteString("SubscriptionManager"), ::OcaLiteString("SubscriptionManager")),
      m_nrEvents(static_cast< ::OcaUint16>(0)),
      m_eventHandlers()
{
}

OcaLiteSubscriptionManager::~OcaLiteSubscriptionManager()
{
    assert(m_eventHandlers.empty());
}

::OcaLiteSubscriptionManager& OcaLiteSubscriptionManager::GetInstance()
{
    if (NULL == OcaLiteSubscriptionManager::m_pSingleton)
    {
        OcaLiteSubscriptionManager::m_pSingleton = new ::OcaLiteSubscriptionManager;
    }

    return *OcaLiteSubscriptionManager::m_pSingleton;
}

void OcaLiteSubscriptionManager::FreeInstance()
{
    if (OcaLiteSubscriptionManager::m_pSingleton != NULL)
    {
        delete m_pSingleton;
        m_pSingleton = NULL;
    }
}

::OcaBoolean OcaLiteSubscriptionManager::Initialize()
{
    return OcaLiteManager::Initialize();
}

::OcaBoolean OcaLiteSubscriptionManager::Shutdown()
{
    // Clear all event handlers
    OcaEventHandlerMap::iterator eh_iter(m_eventHandlers.begin());
    while (eh_iter != m_eventHandlers.end())
    {
        delete eh_iter->second;
        ++eh_iter;
    }
    m_eventHandlers.clear();

    return OcaLiteManager::Shutdown();
}

::OcaLiteStatus OcaLiteSubscriptionManager::AddSubscription(::OcaSessionID sessionID,
                                                            const ::OcaLiteEvent& ocaEvent,
                                                            const ::OcaLiteMethod& subscriber,
                                                            const ::OcaLiteBlob& context,
                                                            ::OcaLiteNotificationDeliveryMode deliveryMode,
                                                            const ::OcaLiteNetworkAddress& destInfo)
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    OcaEventHandlerMap::iterator em_iter(m_eventHandlers.find(ocaEvent));
    if (em_iter == m_eventHandlers.end())
    {
        OcaEventController* pEventController(new OcaEventController(*this));
        rc = pEventController->AddSubscription(sessionID, ocaEvent, subscriber, context, deliveryMode, destInfo);
        if (OCASTATUS_OK == rc)
        {
            m_eventHandlers[ocaEvent] = pEventController;
        }
        else
        {
            delete pEventController;
        }
    }
    else
    {
        rc = em_iter->second->AddSubscription(sessionID, ocaEvent, subscriber, context, deliveryMode, destInfo);
    }

    return rc;
}

::OcaLiteStatus OcaLiteSubscriptionManager::RemoveSubscription(::OcaSessionID sessionID,
                                                               const ::OcaLiteEvent& ocaEvent,
                                                               const ::OcaLiteMethod& subscriber)
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    OcaEventHandlerMap::iterator em_iter(m_eventHandlers.find(ocaEvent));
    if (em_iter != m_eventHandlers.end())
    {
        OcaEventController* pEventController(em_iter->second);
        assert(NULL != pEventController);

        rc = pEventController->RemoveSubscription(sessionID, ocaEvent, subscriber);

        if (OCASTATUS_OK == rc)
        {
            em_iter = m_eventHandlers.find(ocaEvent);
            if (em_iter != m_eventHandlers.end())
            {
                pEventController = em_iter->second;
                assert(NULL != pEventController);
                if (pEventController->CanBeDeleted())
                {
                    static_cast<void>(m_eventHandlers.erase(em_iter));

                    delete pEventController;
                }
            }
        }
    }
    else
    {
        rc = OCASTATUS_PARAMETER_ERROR;
    }

    return rc;
}

::OcaLiteStatus OcaLiteSubscriptionManager::AddPropertyChangeSubscription(::OcaSessionID sessionID,
                                                                          ::OcaONo emitter,
                                                                          const ::OcaLitePropertyID& property,
                                                                          const ::OcaLiteMethod& subscriber,
                                                                          const ::OcaLiteBlob& context,
                                                                          ::OcaLiteNotificationDeliveryMode deliveryMode,
                                                                          const ::OcaLiteNetworkAddress& destInfo) 
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    ::OcaLiteEvent ocaEvent(emitter, ::OcaLiteEventID(static_cast< ::OcaUint16>(1)/*OcaRoot is always level 1*/, 
                                                      static_cast< ::OcaUint16>(::OcaLiteRoot::OCA_EVENT_PROPERTY_CHANGED)));
    OcaEventHandlerMap::iterator em_iter(m_eventHandlers.find(ocaEvent));
    if (em_iter == m_eventHandlers.end())
    {
        OcaEventController* pEventController(new OcaEventController(*this));
        rc = pEventController->AddPropertyChangeSubscription(sessionID, emitter, property, subscriber, context, deliveryMode, destInfo);
        if (OCASTATUS_OK == rc)
        {
            m_eventHandlers[ocaEvent] = pEventController;
        }
        else
        {
            delete pEventController;
        }
    }
    else
    {
        rc = em_iter->second->AddPropertyChangeSubscription(sessionID, emitter, property, subscriber, context, deliveryMode, destInfo);
    }

    return rc;
}

::OcaLiteStatus OcaLiteSubscriptionManager::RemovePropertyChangeSubscription(::OcaSessionID sessionID,
                                                                             ::OcaONo emitter,
                                                                             const ::OcaLitePropertyID& property,
                                                                             const ::OcaLiteMethod& subscriber)
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    ::OcaLiteEvent ocaEvent(emitter, ::OcaLiteEventID(static_cast< ::OcaUint16>(1)/*OcaRoot is always level 1*/, 
                                                      static_cast< ::OcaUint16>(::OcaLiteRoot::OCA_EVENT_PROPERTY_CHANGED)));

    OcaEventHandlerMap::iterator em_iter(m_eventHandlers.find(ocaEvent));
    if (em_iter != m_eventHandlers.end())
    {
        OcaEventController* pEventController(em_iter->second);
        assert(NULL != pEventController);

        rc = pEventController->RemovePropertyChangeSubscription(sessionID, emitter, property, subscriber);

        if (OCASTATUS_OK == rc)
        {
            em_iter = m_eventHandlers.find(ocaEvent);
            if (em_iter != m_eventHandlers.end())
            {
                pEventController = em_iter->second;
                assert(NULL != pEventController);
                if (pEventController->CanBeDeleted())
                {
                    static_cast<void>(m_eventHandlers.erase(em_iter));

                    delete pEventController;
                }
            }
        }
    }
    else
    {
        rc = OCASTATUS_PARAMETER_ERROR;
    }

    return rc;
}

::OcaLiteStatus OcaLiteSubscriptionManager::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case ADD_SUBSCRIPTION:
                {
                    ::OcaLiteEvent ocaEvent;
                    ::OcaLiteMethod subscriber;
                    ::OcaLiteBlob context;
                    ::OcaLiteNotificationDeliveryMode deliveryMode;
                    ::OcaLiteNetworkAddress destInfo;

                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (5 == numberOfParameters) &&
                        (ocaEvent.Unmarshal(bytesLeft, &pCmdParameters, reader)) &&
                        (subscriber.Unmarshal(bytesLeft, &pCmdParameters, reader)) &&
                        (context.Unmarshal(bytesLeft, &pCmdParameters, reader)) &&
                        (::UnmarshalValue< ::OcaLiteNotificationDeliveryMode>(deliveryMode, bytesLeft, &pCmdParameters, reader)) &&
                        (destInfo.Unmarshal(bytesLeft, &pCmdParameters, reader)))
                    {
                        rc = AddSubscription(sessionID, ocaEvent, subscriber, context, deliveryMode, destInfo);

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
            case REMOVE_SUBSCRIPTION:
                {
                    ::OcaLiteEvent ocaEvent;
                    ::OcaLiteMethod subscriber;
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (2 == numberOfParameters) &&
                        (ocaEvent.Unmarshal(bytesLeft, &pCmdParameters, reader)) &&
                        (subscriber.Unmarshal(bytesLeft, &pCmdParameters, reader)))
                    {
                        rc = RemoveSubscription(sessionID, ocaEvent, subscriber);

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
            case GET_MAXIMUM_SUBSCRIBER_CONTEXT_LENGTH:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(0), writer) + ::GetSizeValue< ::OcaUint16>(static_cast< ::OcaUint16>(0), writer));
                        responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                        if (NULL != responseBuffer)
                        {
                            rc = OCASTATUS_OK;

                            ::OcaUint8* pResponse(responseBuffer);
                            writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                            writer.Write(static_cast< ::OcaUint16>(0/*ContextSize*/), &pResponse);

                            *response = responseBuffer;
                        }
                        else
                        {
                            rc = OCASTATUS_BUFFER_OVERFLOW;
                        }
                    }
                }
                break;
            case ADD_PROPERTY_CHANGE_SUBSCRIPTION:
                {
                    ::OcaONo emitter;
                    ::OcaLitePropertyID property;
                    ::OcaLiteMethod subscriber;
                    ::OcaLiteBlob context;
                    ::OcaLiteNotificationDeliveryMode deliveryMode; 
                    ::OcaLiteNetworkAddress destInfo;
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (6 == numberOfParameters) &&
                        (UnmarshalValue< ::OcaONo>(emitter, bytesLeft, &pCmdParameters, reader)) &&
                        (property.Unmarshal(bytesLeft, &pCmdParameters, reader)) &&
                        (subscriber.Unmarshal(bytesLeft, &pCmdParameters, reader)) &&
                        (context.Unmarshal(bytesLeft, &pCmdParameters, reader)) &&
                        (::UnmarshalValue< ::OcaLiteNotificationDeliveryMode>(deliveryMode, bytesLeft, &pCmdParameters, reader)) &&
                        (destInfo.Unmarshal(bytesLeft, &pCmdParameters, reader)))
                    {
                        rc = AddPropertyChangeSubscription(sessionID, emitter, property, subscriber, context, deliveryMode, destInfo);
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
            case REMOVE_PROPERTY_CHANGE_SUBSCRIPTION:
                {
                    ::OcaONo emitter;
                    ::OcaLitePropertyID property;
                    ::OcaLiteMethod subscriber;
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (3 == numberOfParameters) &&
                        (UnmarshalValue< ::OcaONo>(emitter, bytesLeft, &pCmdParameters, reader)) &&
                        (property.Unmarshal(bytesLeft, &pCmdParameters, reader)) &&
                        (subscriber.Unmarshal(bytesLeft, &pCmdParameters, reader)))
                    {
                        rc = RemovePropertyChangeSubscription(sessionID, emitter, property, subscriber);
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
            case DISABLE_NOTIFICATIONS:
            case RE_ENABLE_NOTIFICATIONS:
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

::OcaBoolean OcaLiteSubscriptionManager::AddEventSubscription(const ::OcaLiteEventID& eventID,
                                                                IEventDelegate& eventDelegate)
{
    bool bSuccess(false);

    if (eventID.GetDefLevel() == CLASS_ID.GetFieldCount())
    {
        switch (eventID.GetEventIndex())
        {
        case OCA_EVENT_EVENTS_DISABLED:
            // DisableEvents method is not implemented, nothing to do.
            bSuccess = true;
            break;
        case OCA_EVENT_SYNCHRONIZE_STATE:
            // ReEnableNotifications method is not implemented, nothing to do.
            bSuccess = true;
            break;
        default:
            break;
        }   
    }
    else
    {
        bSuccess = static_cast<bool>(OcaLiteManager::AddEventSubscription(eventID, eventDelegate));
    }

    return static_cast< ::OcaBoolean>(bSuccess);
}

void OcaLiteSubscriptionManager::RemoveEventSubscription(const ::OcaLiteEventID& eventID)
{
    if (eventID.GetDefLevel() == CLASS_ID.GetFieldCount())
    {
        switch (eventID.GetEventIndex())
        {
        case OCA_EVENT_EVENTS_DISABLED:
            // DisableEvents method is not implemented, nothing to do.
            break;
        case OCA_EVENT_SYNCHRONIZE_STATE:
            // ReEnableNotifications method is not implemented, nothing to do.
            break;
        default:
            OCA_LOG_WARNING("Invalid event id");
            break;
        }
    }
    else
    {
        OcaLiteManager::RemoveEventSubscription(eventID);
    }
}


void OcaLiteSubscriptionManager::SessionLost(::OcaSessionID sessionID)
{
    OcaEventHandlerMap::iterator em_iter(m_eventHandlers.begin());
    while (em_iter != m_eventHandlers.end())
    {
        ::OcaLiteEvent controlledEvent(em_iter->first);
        OcaEventController* pEventController(em_iter->second);
        assert(NULL != pEventController);

        pEventController->SessionLost(sessionID);

        em_iter = m_eventHandlers.find(controlledEvent);
        if (em_iter->second->CanBeDeleted())
        {
            delete em_iter->second;
            OcaEventHandlerMap::iterator iterCopy(em_iter);    // Effective STL
            ++em_iter;
            // Only iterCopy will be invalidated by the erase, em_iter remains valid
            static_cast<void>(m_eventHandlers.erase(iterCopy));
        }
        else
        {
            ++em_iter;
        }
    }

    OcaLiteManager::SessionLost(sessionID);
}

::OcaClassVersionNumber OcaLiteSubscriptionManager::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteManager::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

::OcaBoolean OcaLiteSubscriptionManager::SetNrEvents(::OcaUint16 nrEvents)
{
    m_nrEvents = nrEvents;

    return static_cast< ::OcaBoolean>(true);
}

// ---- event_comparator Class Implementation ----

bool event_comparator::operator()(const ::OcaLiteEvent& lhs, const ::OcaLiteEvent& rhs) const
{
    bool bLess(lhs.GetEmitterONo() < rhs.GetEmitterONo());
    if (!bLess &&
        (lhs.GetEmitterONo() == rhs.GetEmitterONo()))
    {
        bLess = (lhs.GetEventID().GetDefLevel() < rhs.GetEventID().GetDefLevel());
        if (!bLess &&
            (lhs.GetEventID().GetDefLevel() == rhs.GetEventID().GetDefLevel()))
        {
            bLess = (lhs.GetEventID().GetEventIndex() < rhs.GetEventID().GetEventIndex());
        }
    }

    return bLess;
}

// ---- OcaEventController Class Implementation ----

OcaLiteSubscriptionManager::OcaEventController::OcaEventController(::OcaLiteSubscriptionManager& subscriptionManager)
    : m_event(),
      m_eventDestinations(),
      m_subscriptionManager(subscriptionManager)
{
}

OcaLiteSubscriptionManager::OcaEventController::~OcaEventController()
{
    m_eventDestinations.clear();

    static_cast<void>(RemoveEventSubscription());
}

::OcaLiteStatus OcaLiteSubscriptionManager::OcaEventController::AddSubscription(::OcaSessionID sessionID,
                                                                                const ::OcaLiteEvent& ocaEvent,
                                                                                const ::OcaLiteMethod& subscriber,
                                                                                const ::OcaLiteBlob&,
                                                                                ::OcaLiteNotificationDeliveryMode deliveryMode,
                                                                                const ::OcaLiteNetworkAddress& destInfo)
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    if (m_eventDestinations.empty())
    {
        m_event = ocaEvent;

        ::OcaLiteRoot* pObject(GetOcaRootObject(m_event.GetEmitterONo()));

        if (NULL != pObject)
        {
            if (!pObject->AddEventSubscription(m_event.GetEventID(), *this))
            {
                rc = OCASTATUS_PROCESSING_FAILED;
            }
        }
        else
        {
            rc = OCASTATUS_BAD_ONO;
        }
    }
    else
    {
        // Check for duplicates
        OcaEventDestinationList::iterator ed_iter(m_eventDestinations.begin());
        while ((ed_iter != m_eventDestinations.end()) &&
               (OCASTATUS_OK == rc))
        {
            if ((ed_iter->GetSubscriber() == subscriber) &&
                (ed_iter->GetSessionID() == sessionID) &&
                (ed_iter->GetPropertyIDFilter() == ::OcaLitePropertyID()))
            {
                rc = OCASTATUS_INVALID_REQUEST;
            }
            else
            {
                ++ed_iter;
            }
        }
    }

    if (OCASTATUS_OK == rc)
    {
        OcaEventDestination eventDestination(subscriber, sessionID, deliveryMode, destInfo);
        m_eventDestinations.push_back(eventDestination);
    }
    return rc;
}

OcaLiteStatus OcaLiteSubscriptionManager::OcaEventController::RemoveSubscription(::OcaSessionID sessionID,
                                                                                 const ::OcaLiteEvent& ocaEvent,
                                                                                 const ::OcaLiteMethod& subscriber)
{
    OcaEventDestinationList::iterator ed_iter(m_eventDestinations.begin());
    while (ed_iter != m_eventDestinations.end())
    {
        if ((ed_iter->GetSubscriber() == subscriber) &&
            (ed_iter->GetSessionID() == sessionID) &&
            (ed_iter->GetPropertyIDFilter() == ::OcaLitePropertyID()))
        {
            static_cast<void>(m_eventDestinations.erase(ed_iter));
            break;
        }
        else
        {
            ++ed_iter;
        }
    }
    return RemoveEventSubscription();
}

::OcaLiteStatus OcaLiteSubscriptionManager::OcaEventController::AddPropertyChangeSubscription(::OcaSessionID sessionID,
                                                                                              ::OcaONo emitter,
                                                                                              const ::OcaLitePropertyID& property,
                                                                                              const ::OcaLiteMethod& subscriber,
                                                                                              const ::OcaLiteBlob& context,
                                                                                              ::OcaLiteNotificationDeliveryMode deliveryMode,
                                                                                              const ::OcaLiteNetworkAddress& destInfo)
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    if (m_eventDestinations.empty())
    {
        m_event = ::OcaLiteEvent(emitter, ::OcaLiteEventID(static_cast< ::OcaUint16>(1)/*OcaRoot is always level 1*/, 
                                                           static_cast< ::OcaUint16>(::OcaLiteRoot::OCA_EVENT_PROPERTY_CHANGED)));
;

        ::OcaLiteRoot* pObject(GetOcaRootObject(m_event.GetEmitterONo()));

        if (NULL != pObject)
        {
            if (!pObject->AddEventSubscription(m_event.GetEventID(), *this))
            {
                rc = OCASTATUS_PROCESSING_FAILED;
            }
        }
        else
        {
            rc = OCASTATUS_BAD_ONO;
        }
    }
    else
    {
        // Check for duplicates
        OcaEventDestinationList::iterator ed_iter(m_eventDestinations.begin());
        while ((ed_iter != m_eventDestinations.end()) &&
               (OCASTATUS_OK == rc))
        {
            if ((ed_iter->GetSubscriber() == subscriber) &&
                (ed_iter->GetSessionID() == sessionID) &&
                (ed_iter->GetPropertyIDFilter() == property))
            {
                rc = OCASTATUS_INVALID_REQUEST;
            }
            else
            {
                ++ed_iter;
            }
        }
    }

    if (OCASTATUS_OK == rc)
    {
        OcaEventDestination eventDestination(subscriber, sessionID, deliveryMode, destInfo, property);
        m_eventDestinations.push_back(eventDestination);
    }
    return rc;
}

OcaLiteStatus OcaLiteSubscriptionManager::OcaEventController::RemovePropertyChangeSubscription(::OcaSessionID sessionID,
                                                                                               ::OcaONo emitter,
                                                                                               const ::OcaLitePropertyID& property,
                                                                                               const ::OcaLiteMethod& subscriber)
{
    OcaEventDestinationList::iterator ed_iter(m_eventDestinations.begin());
    while (ed_iter != m_eventDestinations.end())
    {
        if ((ed_iter->GetSubscriber() == subscriber) &&
            (ed_iter->GetSessionID() == sessionID) &&
            (ed_iter->GetPropertyIDFilter() == property))
        {
            static_cast<void>(m_eventDestinations.erase(ed_iter));
            break;
        }
        else
        {
            ++ed_iter;
        }
    }
    return RemoveEventSubscription();
}

void OcaLiteSubscriptionManager::OcaEventController::OnEvent(const ::OcaLiteEventData& eventData)
{
    OcaEventDestinationList::iterator ed_iter(m_eventDestinations.begin());
    while (ed_iter != m_eventDestinations.end())
    {
        ::OcaSessionID sessionID(ed_iter->GetSessionID());
        
            
        ::OcaLiteNotificationData OcaLiteNotificationData(ed_iter->GetSubscriber().GetONo(),
                                                        ed_iter->GetSubscriber().GetMethodID(),
                                                        ::OcaLiteBlob(), // Always provide an empty context since we don't support context
                                                        &eventData);

        // Send the notification directly
        if (::OcaLiteCommandHandler::GetInstance().SendNotification(ed_iter->GetDeliveryMode(),
                                                                    sessionID,
                                                                    OcaLiteNotificationData,
                                                                    ed_iter->GetDestinationInfo()))
        {
            ++ed_iter;
        }
        else
        {
            OCA_LOG_WARNING("SendNotification failed.");

            // Sending failed, remove subscription.
            ed_iter = m_eventDestinations.erase(ed_iter);
        }
    }

    static_cast<void>(RemoveEventSubscription());
}

void OcaLiteSubscriptionManager::OcaEventController::OnEvent(const ::OcaLiteEventData& eventData, const ::OcaLitePropertyID& propertyID)
{
    OcaEventDestinationList::iterator ed_iter(m_eventDestinations.begin());
    while (ed_iter != m_eventDestinations.end())
    {
        if ((ed_iter->GetPropertyIDFilter() == ::OcaLitePropertyID()) || // No filter
            (ed_iter->GetPropertyIDFilter() == propertyID)) // Current property
        {
            ::OcaSessionID sessionID(ed_iter->GetSessionID());
            
                
            ::OcaLiteNotificationData OcaLiteNotificationData(ed_iter->GetSubscriber().GetONo(),
                                                            ed_iter->GetSubscriber().GetMethodID(),
                                                            ::OcaLiteBlob(), // Always provide an empty context since we don't support context
                                                            &eventData);

            // Send the notification directly
            if (::OcaLiteCommandHandler::GetInstance().SendNotification(ed_iter->GetDeliveryMode(),
                                                                        sessionID,
                                                                        OcaLiteNotificationData,
                                                                        ed_iter->GetDestinationInfo()))
            {
                ++ed_iter;
            }
            else
            {
                OCA_LOG_WARNING("SendNotification failed.");

                // Sending failed, remove subscription.
                ed_iter = m_eventDestinations.erase(ed_iter);
            }
        }
        else
        {
            ++ed_iter;
        }
        
    }

    static_cast<void>(RemoveEventSubscription());
}

void OcaLiteSubscriptionManager::OcaEventController::SessionLost(::OcaSessionID sessionID)
{
    OcaEventDestinationList::iterator ed_iter(m_eventDestinations.begin());
    while (ed_iter != m_eventDestinations.end())
    {
        if (ed_iter->GetSessionID() == sessionID)
        {
            ed_iter = m_eventDestinations.erase(ed_iter);
        }
        else
        {
            ++ed_iter;
        }
    }

    static_cast<void>(RemoveEventSubscription());
}

::OcaLiteStatus OcaLiteSubscriptionManager::OcaEventController::RemoveEventSubscription()
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    if (m_eventDestinations.empty() &&
        (OCA_INVALID_ONO != m_event.GetEmitterONo()))
    {
        ::OcaLiteRoot* pObject(GetOcaRootObject(m_event.GetEmitterONo()));
        // No more destinations for this event.
        if (NULL != pObject)
        {
            pObject->RemoveEventSubscription(m_event.GetEventID());
        }
        else
        {
            rc = OCASTATUS_BAD_ONO;
        }

        // Clear the event
        m_event = ::OcaLiteEvent();
    }

    return rc;
}

::OcaLiteRoot* OcaLiteSubscriptionManager::OcaEventController::GetOcaRootObject(::OcaONo oNo) const
{
    ::OcaLiteRoot* pObject(NULL);

    if ((oNo == OCA_ROOT_BLOCK_ONO) ||
        (oNo >= OCA_MINIMUM_DEVICE_OBJECT_ONO))
    {
        pObject = OcaLiteBlock::GetRootBlock().GetObject(oNo);
    }
    else
    {
        pObject = ::OcaLiteDeviceManager::GetInstance().GetManager(oNo);
    }

    return pObject;
}
