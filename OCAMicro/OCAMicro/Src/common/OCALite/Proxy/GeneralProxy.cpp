/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLite GeneralProxy implementation.
 *
 */

// ---- Include system wide include files ----
#include <OCALite/OCC/ControlClasses/Managers/OcaLiteDeviceManager.h>
#include <OCALite/OCC/ControlClasses/Managers/OcaLiteNetworkManager.h>
#include <OCALite/OCC/ControlClasses/Managers/OcaLiteSubscriptionManager.h>
#include <OCALite/OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCALite/OCC/ControlDataTypes/OcaLiteBlockMember.h>
#include <OCALite/OCC/ControlDataTypes/OcaLiteEvent.h>
#include <OCALite/OCC/ControlDataTypes/OcaLiteManagerDescriptor.h>
#include <OCALite/OCC/ControlDataTypes/OcaLiteMethod.h>
#include <OCALite/OCC/ControlDataTypes/OcaLiteNetworkAddress.h>
#include <OCALite/OCF/OcaLiteCommandHandlerController.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "GeneralProxy.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----
GeneralProxy::GeneralProxy(::OcaSessionID sessionId, ::OcaONo networkObjectNumber)
    : m_sessionId(sessionId),
    m_networkObjectNumber(networkObjectNumber)
{
    memset(m_buffer, 0, sizeof(m_buffer));
}

::OcaLiteStatus GeneralProxy::OcaRoot_GetClassIdentification(::OcaONo remoteObjectNumber, ::OcaLiteClassIdentification& classIdentification)
{
    ::OcaUint8 noParams(0);
    ::OcaUint32 responseSize;
    ::OcaUint8* pResponse;
    ::OcaLiteStatus rc(::OcaLiteCommandHandlerController::GetInstance().SendCommandWithResponse(m_sessionId,
                                                                                                m_networkObjectNumber,
                                                                                                remoteObjectNumber,
                                                                                                ::OcaLiteMethodID(::OcaLiteRoot::CLASS_ID.GetFieldCount(),
                                                                                                                  ::OcaLiteRoot::GET_CLASS_IDENTIFICATION),
                                                                                                1,
                                                                                                &noParams,
                                                                                                responseSize,
                                                                                                &pResponse));

    if ((OCASTATUS_OK == rc) &&
        (responseSize > 0))
    {
        const ::IOcaLiteReader& reader(::OcaLiteNetworkManager::GetInstance().GetNetwork(m_networkObjectNumber)->GetReader());
        ::OcaUint8 nrParameters(0);
        const ::OcaUint8* source(pResponse);
        reader.Read(responseSize, &source, nrParameters);
        if (nrParameters == 1)
        {
            if (!classIdentification.Unmarshal(responseSize, &source, reader))
            {
                rc = OCASTATUS_PARAMETER_ERROR;
            }
        }
        else
        {
            rc = OCASTATUS_PARAMETER_ERROR;
        }
    }

    return rc;
}


::OcaLiteStatus GeneralProxy::OcaBlock_GetMembersRecursive(::OcaONo remoteObjectNumber, ::OcaLiteList< ::OcaLiteBlockMember>& members)
{
    members.Clear();

    ::OcaUint8 noParams(0);
    ::OcaUint32 responseSize;
    ::OcaUint8* pResponse;
    ::OcaLiteStatus rc(::OcaLiteCommandHandlerController::GetInstance().SendCommandWithResponse(m_sessionId,
                                                                                                m_networkObjectNumber,
                                                                                                remoteObjectNumber,
                                                                                                ::OcaLiteMethodID(::OcaLiteBlock::CLASS_ID.GetFieldCount(),
                                                                                                                  ::OcaLiteBlock::GET_MEMBERS_RECURSIVE),
                                                                                                1,
                                                                                                &noParams,
                                                                                                responseSize,
                                                                                                &pResponse));

    if ((OCASTATUS_OK == rc) &&
        (responseSize > 0))
    {
        const ::IOcaLiteReader& reader(::OcaLiteNetworkManager::GetInstance().GetNetwork(m_networkObjectNumber)->GetReader());
        ::OcaUint8 nrParameters(0);
        const ::OcaUint8* source(pResponse);
        reader.Read(responseSize, &source, nrParameters);
        if (nrParameters == 1)
        {
            if (!members.Unmarshal(responseSize, &source, reader))
            {
                rc = OCASTATUS_PARAMETER_ERROR;
            }
        }
        else
        {
            rc = OCASTATUS_PARAMETER_ERROR;
        }
    }

    return rc;
}
::OcaLiteStatus GeneralProxy::OcaBlock_GetMembers(::OcaONo remoteObjectNumber, ::OcaLiteList< ::OcaLiteObjectIdentification>& members)
{
     members.Clear();

    ::OcaUint8 noParams(0);
    ::OcaUint32 responseSize;
    ::OcaUint8* pResponse;
    ::OcaLiteStatus rc(::OcaLiteCommandHandlerController::GetInstance().SendCommandWithResponse(m_sessionId,
                                                                                                m_networkObjectNumber,
                                                                                                remoteObjectNumber,
                                                                                                ::OcaLiteMethodID(::OcaLiteBlock::CLASS_ID.GetFieldCount(),
                                                                                                                  ::OcaLiteBlock::GET_MEMBERS),
                                                                                                1,
                                                                                                &noParams,
                                                                                                responseSize,
                                                                                                &pResponse));

    if ((OCASTATUS_OK == rc) &&
        (responseSize > 0))
    {
        const ::IOcaLiteReader& reader(::OcaLiteNetworkManager::GetInstance().GetNetwork(m_networkObjectNumber)->GetReader());
        ::OcaUint8 nrParameters(0);
        const ::OcaUint8* source(pResponse);
        reader.Read(responseSize, &source, nrParameters);
        if (nrParameters == 1)
        {
            if (!members.Unmarshal(responseSize, &source, reader))
            {
                rc = OCASTATUS_PARAMETER_ERROR;
            }
        }
        else
        {
            rc = OCASTATUS_PARAMETER_ERROR;
        }
    }

    return rc;
}

::OcaLiteStatus GeneralProxy::OcaDeviceManager_GetManagers(::OcaLiteList< ::OcaLiteManagerDescriptor>& managers)
{
    managers.Clear();

    ::OcaUint8 noParams(0);
    ::OcaUint32 responseSize;
    ::OcaUint8* pResponse;
    ::OcaLiteStatus rc(::OcaLiteCommandHandlerController::GetInstance().SendCommandWithResponse(m_sessionId,
                                                                                                m_networkObjectNumber,
                                                                                                ::OcaLiteDeviceManager::OBJECT_NUMBER,
                                                                                                ::OcaLiteMethodID(::OcaLiteDeviceManager::CLASS_ID.GetFieldCount(),
                                                                                                    ::OcaLiteDeviceManager::GET_MANAGERS),
                                                                                                1,
                                                                                                &noParams,
                                                                                                responseSize,
                                                                                                &pResponse));

    if ((OCASTATUS_OK == rc) &&
        (responseSize > 0))
    {
        const ::IOcaLiteReader& reader(::OcaLiteNetworkManager::GetInstance().GetNetwork(m_networkObjectNumber)->GetReader());
        ::OcaUint8 nrParameters(0);
        const ::OcaUint8* source(pResponse);
        reader.Read(responseSize, &source, nrParameters);
        if (nrParameters == 1)
        {
            if (!managers.Unmarshal(responseSize, &source, reader))
            {
                rc = OCASTATUS_PARAMETER_ERROR;
            }
        }
        else
        {
            rc = OCASTATUS_PARAMETER_ERROR;
        }
    }

    return rc;
}

::OcaLiteStatus GeneralProxy::OcaDeviceManager_SetResetKey(const ::OcaLiteBlobFixedLen<16>& resetKey, ::OcaLiteNetworkAddress networkAddress)
{
    const ::IOcaLiteWriter& writer(::OcaLiteNetworkManager::GetInstance().GetNetwork(m_networkObjectNumber)->GetWriter());
    ::OcaUint8* pParams(m_buffer);

    writer.Write(static_cast<UINT8>(2), &pParams); // Nr params
    resetKey.Marshal(&pParams, writer);
    networkAddress.Marshal(&pParams, writer);

    ::OcaUint32 responseSize;
    ::OcaUint8* pResponse;
    ::OcaLiteStatus rc(::OcaLiteCommandHandlerController::GetInstance().SendCommandWithResponse(m_sessionId,
                                                                                                m_networkObjectNumber,
                                                                                                ::OcaLiteDeviceManager::OBJECT_NUMBER,
                                                                                                ::OcaLiteMethodID(::OcaLiteDeviceManager::CLASS_ID.GetFieldCount(),
                                                                                                                  ::OcaLiteDeviceManager::SET_RESET_KEY),
                                                                                                static_cast< ::OcaUint32>(pParams - m_buffer),
                                                                                                m_buffer,
                                                                                                responseSize,
                                                                                                &pResponse));

    if ((responseSize != 1) && 
        (OCASTATUS_OK == rc))
    {
        rc = OCASTATUS_PARAMETER_ERROR;
    }

    return rc;
}

::OcaLiteStatus GeneralProxy::OcaDeviceManager_ClearResetCause()
{
    ::OcaUint8 noParams(0);
    ::OcaUint32 responseSize;
    ::OcaUint8* pResponse;
    ::OcaLiteStatus rc(::OcaLiteCommandHandlerController::GetInstance().SendCommandWithResponse(m_sessionId,
                                                                                                m_networkObjectNumber,
                                                                                                ::OcaLiteDeviceManager::OBJECT_NUMBER,
                                                                                                ::OcaLiteMethodID(::OcaLiteDeviceManager::CLASS_ID.GetFieldCount(),
                                                                                                    ::OcaLiteDeviceManager::CLEAR_RESET_CAUSE),
                                                                                                1,
                                                                                                &noParams,
                                                                                                responseSize,
                                                                                                &pResponse));

    if ((responseSize != 1) && 
        (OCASTATUS_OK == rc))
    {
        rc = OCASTATUS_PARAMETER_ERROR;
    }

    return rc;
}

::OcaLiteStatus GeneralProxy::OcaDeviceManager_GetResetCause(::OcaLiteResetCause& resetCause)
{
    ::OcaUint8 noParams(0);
    ::OcaUint32 responseSize;
    ::OcaUint8* pResponse;
    ::OcaLiteStatus rc(::OcaLiteCommandHandlerController::GetInstance().SendCommandWithResponse(m_sessionId,
                                                                                                m_networkObjectNumber,
                                                                                                ::OcaLiteDeviceManager::OBJECT_NUMBER,
                                                                                                ::OcaLiteMethodID(::OcaLiteDeviceManager::CLASS_ID.GetFieldCount(),
                                                                                                    ::OcaLiteDeviceManager::GET_RESET_CAUSE),
                                                                                                1,
                                                                                                &noParams,
                                                                                                responseSize,
                                                                                                &pResponse));

    if ((OCASTATUS_OK == rc) &&
        (responseSize > 0))
    {
        const ::IOcaLiteReader& reader(::OcaLiteNetworkManager::GetInstance().GetNetwork(m_networkObjectNumber)->GetReader());
        ::OcaUint8 nrParameters(0);
        const ::OcaUint8* source(pResponse);
        reader.Read(responseSize, &source, nrParameters);
        if (nrParameters == 1)
        {
            if (!::UnmarshalValue< ::OcaLiteResetCause>(resetCause, responseSize, &source, reader))
            {
                rc = OCASTATUS_PARAMETER_ERROR;
            }
        }
        else
        {
            rc = OCASTATUS_PARAMETER_ERROR;
        }
    }

    return rc;
}

::OcaLiteStatus GeneralProxy::OcaSubscriptionManager_AddSubscription(const OcaLiteEvent& ocaEvent,
                                                                     const OcaLiteMethod& subscriber,
                                                                     const OcaLiteBlob& context,
                                                                     ::OcaLiteNotificationDeliveryMode deliveryMode,
                                                                     const ::OcaLiteNetworkAddress& destInfo)
{
    const ::IOcaLiteWriter& writer(::OcaLiteNetworkManager::GetInstance().GetNetwork(m_networkObjectNumber)->GetWriter());
    ::OcaUint8* pParams(m_buffer);

    writer.Write(static_cast<UINT8>(5), &pParams); // Nr params
    ocaEvent.Marshal(&pParams, writer);
    subscriber.Marshal(&pParams, writer);
    context.Marshal(&pParams, writer);
    MarshalValue< ::OcaLiteNotificationDeliveryMode>(deliveryMode, &pParams, writer);
    destInfo.Marshal(&pParams, writer);

    ::OcaUint32 responseSize;
    ::OcaUint8* pResponse;
    ::OcaLiteStatus rc(::OcaLiteCommandHandlerController::GetInstance().SendCommandWithResponse(m_sessionId,
                                                                                                m_networkObjectNumber,
                                                                                                ::OcaLiteSubscriptionManager::OBJECT_NUMBER,
                                                                                                ::OcaLiteMethodID(::OcaLiteSubscriptionManager::CLASS_ID.GetFieldCount(),
                                                                                                                  ::OcaLiteSubscriptionManager::ADD_SUBSCRIPTION),
                                                                                                static_cast< ::OcaUint32>(pParams - m_buffer),
                                                                                                m_buffer,
                                                                                                responseSize,
                                                                                                &pResponse));

    if ((responseSize != 1) && 
        (OCASTATUS_OK == rc))
    {
        rc = OCASTATUS_PARAMETER_ERROR;
    }

    return rc;
}

::OcaLiteStatus GeneralProxy::OcaSubscriptionManager_RemoveSubscription(const OcaLiteEvent& ocaEvent,
                                                                        const OcaLiteMethod& subscriber)
{
    const ::IOcaLiteWriter& writer(::OcaLiteNetworkManager::GetInstance().GetNetwork(m_networkObjectNumber)->GetWriter());
    ::OcaUint8* pParams(m_buffer);

    writer.Write(static_cast<UINT8>(2), &pParams); // Nr params
    ocaEvent.Marshal(&pParams, writer);
    subscriber.Marshal(&pParams, writer);

    ::OcaUint32 responseSize;
    ::OcaUint8* pResponse;
    ::OcaLiteStatus rc(::OcaLiteCommandHandlerController::GetInstance().SendCommandWithResponse(m_sessionId,
                                                                                                m_networkObjectNumber,
                                                                                                ::OcaLiteSubscriptionManager::OBJECT_NUMBER,
                                                                                                ::OcaLiteMethodID(::OcaLiteSubscriptionManager::CLASS_ID.GetFieldCount(),
                                                                                                                  ::OcaLiteSubscriptionManager::REMOVE_SUBSCRIPTION),
                                                                                                static_cast< ::OcaUint32>(pParams - m_buffer),
                                                                                                m_buffer,
                                                                                                responseSize,
                                                                                                &pResponse));

    if ((responseSize != 1) && 
        (OCASTATUS_OK == rc))
    {
        rc = OCASTATUS_PARAMETER_ERROR;
    }

    return rc;
}

::OcaLiteStatus GeneralProxy::OcaCompliancyTestTool_InvalidCall(::OcaONo objectNumber, ::OcaLiteMethodID& methodId)
{
    ::OcaUint8 noParams(100);
    ::OcaUint32 responseSize;
    ::OcaUint8* pResponse;
    return ::OcaLiteCommandHandlerController::GetInstance().SendCommandWithResponse(m_sessionId,
                                                                                    m_networkObjectNumber,
                                                                                    objectNumber,
                                                                                    methodId,
                                                                                    1,
                                                                                    &noParams,
                                                                                    responseSize,
                                                                                    &pResponse);
}
