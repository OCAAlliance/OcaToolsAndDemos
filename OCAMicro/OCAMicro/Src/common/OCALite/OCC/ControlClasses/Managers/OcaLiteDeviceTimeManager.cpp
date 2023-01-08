/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : The OcaLiteDeviceTimeManager.
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/Timer/IOcfLiteTimer.h>
#include <OCC/ControlClasses/Agents/OcaLiteTimeSource.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- Include local include files ----
#include "OcaLiteDeviceTimeManager.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_DEVICE_TIME_MANAGER_CLASSID};
const ::OcaLiteClassID          OcaLiteDeviceTimeManager::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

#ifndef DEVICE_REVISION_ID
#ifdef _DEBUG
#define DEVICE_REVISION_ID "Compiled @ " __TIME__" " __DATE__
#else
#define DEVICE_REVISION_ID "Unknown revision"
#endif
#endif
// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

const ::OcaONo OcaLiteDeviceTimeManager::OBJECT_NUMBER(static_cast< ::OcaONo>(10));

OcaLiteDeviceTimeManager::OcaLiteDeviceTimeManager()
  : ::OcaLiteManager(OBJECT_NUMBER, ::OcaLiteString("DeviceTimeManager"), ::OcaLiteString("DeviceTimeManager")),
    m_timeSourceList(),
    m_currentDeviceTimeSource(OCA_INVALID_ONO),
    m_bInitialized(static_cast< ::OcaBoolean>(false))
{
}

OcaLiteDeviceTimeManager::~OcaLiteDeviceTimeManager()
{
}

::OcaBoolean OcaLiteDeviceTimeManager::Initialize()
{
    ::OcaBoolean bResult(OcaLiteManager::Initialize());

    if (bResult)
    {
        m_bInitialized = true;
    }

    return bResult;
}

::OcaBoolean OcaLiteDeviceTimeManager::Shutdown()
{
    m_bInitialized = false;

    return OcaLiteManager::Shutdown();
}

::OcaLiteStatus OcaLiteDeviceTimeManager::GetDeviceTimeNTP(::OcaLiteTimeNTP& deviceTime) const
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);

    if (m_bInitialized)
    {
        ::OcaUint64 seconds;
        ::OcaUint32 nanoSeconds;
        rc = GetDeviceTimeValue(seconds, nanoSeconds);
        if (OCASTATUS_OK == rc)
        {
            deviceTime = ::CreateTimeNTP(static_cast< ::OcaUint32>(seconds), nanoSeconds);
        }
    }

    return rc;

}

::OcaLiteStatus OcaLiteDeviceTimeManager::SetDeviceTimeNTP(::OcaLiteTimeNTP deviceTime)
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);

    if (m_bInitialized)
    {
        ::OcaUint64 oldSeconds;
        ::OcaUint32 oldNanoSeconds;
        rc = GetDeviceTimeValue(oldSeconds, oldNanoSeconds);
        ::OcaLiteTimeNTP oldDeviceTime(::CreateTimeNTP(static_cast< ::OcaUint32>(oldSeconds), oldNanoSeconds));

        if ((OCASTATUS_OK == rc) &&
            !CompareValue< ::OcaLiteTimeNTP>(deviceTime, oldDeviceTime))
        {
            ::OcaUint32 newSeconds;
            ::OcaUint32 newNanoSeconds;
            if (::ParseTimeNTP(deviceTime, newSeconds, newNanoSeconds))
            {
                rc = SetDeviceTimeValue(static_cast< ::OcaUint64>(newSeconds), newNanoSeconds);
            }
            else
            {
                OCA_LOG_ERROR("Unable to parse new device time");
                rc = OCASTATUS_PARAMETER_ERROR;
            }
        }
    }

    return rc;
}

::OcaLiteStatus OcaLiteDeviceTimeManager::GetTimeSources(::OcaLiteList< ::OcaONo>& timeSources) const
{
    ::OcaLiteStatus rc(InternalGetTimeSources(timeSources));

    return rc;
}

::OcaLiteStatus OcaLiteDeviceTimeManager::GetCurrentDeviceTimeSource(::OcaONo& timeSource) const
{
    ::OcaLiteStatus rc(GetCurrentDeviceTimeSourceValue(timeSource));

    return rc;
}

::OcaLiteStatus OcaLiteDeviceTimeManager::SetCurrentDeviceTimeSource(::OcaONo timeSource)
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);
    if ((OCA_INVALID_ONO == timeSource) ||
        (m_timeSourceList.end() != std::find(m_timeSourceList.begin(), m_timeSourceList.end(), timeSource)))
    {
        ::OcaONo oldTimeSource;
        rc = GetCurrentDeviceTimeSourceValue(oldTimeSource);
        if ((OCASTATUS_OK == rc) && (oldTimeSource != timeSource))
        {
            rc = SetCurrentDeviceTimeSourceValue(timeSource);
            if (OCASTATUS_OK == rc)
            {
                m_currentDeviceTimeSource = timeSource;

                ::OcaLitePropertyID propertyId(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_CURRENT_DEVICE_TIME_SOURCE));
                ::OcaLitePropertyChangedEventData< ::OcaONo> eventData(OBJECT_NUMBER,
                                                                   propertyId,
                                                                   timeSource,
                                                                   OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);

                PropertyChanged(eventData, propertyId);
            }
        }
    }

    return rc;
}

::OcaLiteStatus OcaLiteDeviceTimeManager::GetDeviceTimePTP(::OcaLiteTimePTP& deviceTime) const
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);

    if (m_bInitialized)
    {
        ::OcaUint64 seconds;
        ::OcaUint32 nanoSeconds;
        rc = GetDeviceTimeValue(seconds, nanoSeconds);
        if (OCASTATUS_OK == rc)
        {
            deviceTime = ::OcaLiteTimePTP(static_cast< ::OcaBoolean>(false), ::OcaLitePTPSeconds(seconds), nanoSeconds);
        }
    }

    return rc;

}

::OcaLiteStatus OcaLiteDeviceTimeManager::SetDeviceTimePTP(const ::OcaLiteTimePTP& deviceTime)
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);

    if (m_bInitialized)
    {
        ::OcaUint64 oldSeconds;
        ::OcaUint32 oldNanoSeconds;
        rc = GetDeviceTimeValue(oldSeconds, oldNanoSeconds);
        ::OcaLiteTimePTP oldDeviceTime(static_cast< ::OcaBoolean>(false), ::OcaLitePTPSeconds(oldSeconds), oldNanoSeconds);

        if ((OCASTATUS_OK == rc) &&
            (deviceTime != oldDeviceTime))
        {
            rc = SetDeviceTimeValue(deviceTime.GetSeconds().GetSeconds(), deviceTime.GetNanoSeconds());
        }
    }

    return rc;
}

::OcaLiteStatus OcaLiteDeviceTimeManager::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_DEVICE_TIME_NTP:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteTimeNTP deviceTime;
                        rc = GetDeviceTimeNTP(deviceTime);
                        if ((OCASTATUS_OK == rc))
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast<::OcaUint8>(1), writer) +
                                ::GetSizeValue< ::OcaUint64>(deviceTime, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast<::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaUint64>(deviceTime, &pResponse, writer);

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
            case SET_DEVICE_TIME_NTP:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaLiteTimeNTP deviceTime;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        reader.Read(bytesLeft, &pCmdParameters, deviceTime))
                    {
                        rc = SetDeviceTimeNTP(deviceTime);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast<::OcaUint8>(0), writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast<::OcaUint8>(0/*NrParameters*/), &pResponse);
                    
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
            case GET_TIME_SOURCES:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaONo> timeSources;
                        rc = GetTimeSources(timeSources);
                        if ((OCASTATUS_OK == rc))
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast<::OcaUint8>(1), writer) +
                                timeSources.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast<::OcaUint8>(1/*NrParameters*/), &pResponse);
                                timeSources.Marshal(&pResponse, writer);

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
            case GET_CURRENT_DEVICE_TIME_SOURCE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaONo timeSource;
                        rc = GetCurrentDeviceTimeSource(timeSource);
                        if ((OCASTATUS_OK == rc))
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast<::OcaUint8>(1), writer) +
                                ::GetSizeValue< ::OcaONo>(timeSource, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast<::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaONo>(timeSource, &pResponse, writer);

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
            case SET_CURRENT_DEVICE_TIME_SOURCE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaONo timeSource;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        reader.Read(bytesLeft, &pCmdParameters, timeSource))
                    {
                        rc = SetCurrentDeviceTimeSource(timeSource);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast<::OcaUint8>(0), writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast<::OcaUint8>(0/*NrParameters*/), &pResponse);

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
            case GET_DEVICE_TIME_PTP:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteTimePTP deviceTime;
                        rc = GetDeviceTimePTP(deviceTime);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast<::OcaUint8>(1), writer) +
                                                     deviceTime.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast<::OcaUint8>(1/*NrParameters*/), &pResponse);
                                deviceTime.Marshal(&pResponse, writer);

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
            case SET_DEVICE_TIME_PTP:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaLiteTimePTP deviceTime;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        deviceTime.Unmarshal(bytesLeft, &pCmdParameters, reader))
                    {
                        rc = SetDeviceTimePTP(deviceTime);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast<::OcaUint8>(0), writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast<::OcaUint8>(0/*NrParameters*/), &pResponse);

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
            default:
                rc = OCASTATUS_BAD_METHOD;
                break;
            }
        }
        else
        {
            // Should be executed on higher level
            rc = OcaLiteManager::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

::OcaClassVersionNumber OcaLiteDeviceTimeManager::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteManager::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

#if !defined(OCA_DISABLE_ROOT_ROLE) && defined(OCA_USE_VIRTUAL_ROOT_ROLE)
::OcaLiteStatus OcaLiteDeviceTimeManager::GetRoleValue(::OcaLiteString& role) const
{
    role = MANAGER_NAME;
    return OCASTATUS_OK;
}
#endif // !defined(OCA_DISABLE_ROOT_ROLE) && defined(OCA_USE_VIRTUAL_ROOT_ROLE)

::OcaLiteStatus OcaLiteDeviceTimeManager::GetDeviceTimeValue(::OcaUint64& seconds, ::OcaUint32& nanoSeconds) const
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);

    UINT64 hiSeconds(0);
    UINT32 hiNanoSeconds(0);
    if (::OcfLiteTimerGetTimeNow(hiSeconds, hiNanoSeconds))
    {
        seconds = static_cast< ::OcaUint64>(hiSeconds);
        nanoSeconds = static_cast< ::OcaUint32>(hiNanoSeconds);
        rc = OCASTATUS_OK;
    }

    return rc;
}

::OcaBoolean OcaLiteDeviceTimeManager::AddTimeSource(const ::OcaLiteTimeSource& timeSource)
{
    ::OcaBoolean result(static_cast< ::OcaBoolean>(false));

    if (m_timeSourceList.end() == std::find(m_timeSourceList.begin(), m_timeSourceList.end(), timeSource.GetObjectNumber()))
    {
        m_timeSourceList.push_back(timeSource.GetObjectNumber());

        ::OcaLiteList< ::OcaONo> timeSourceList;
        if (OCASTATUS_OK == InternalGetTimeSources(timeSourceList))
        {
            ::OcaLitePropertyID propertyId(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_TIME_SOURCES));
            ::OcaLitePropertyChangedEventData< ::OcaLiteList< ::OcaONo> > eventDataState(OBJECT_NUMBER,
                                                                                 propertyId,
                                                                                 timeSourceList,
                                                                                 OCAPROPERTYCHANGETYPE_ITEM_ADDED);

            PropertyChanged(eventDataState, propertyId);

            result = static_cast< ::OcaBoolean>(true);
        }
        else
        {
            OCA_LOG_WARNING("Failed to retrieve time source list");
        }
    }

    return result;
}

void OcaLiteDeviceTimeManager::RemoveTimeSource(const ::OcaLiteTimeSource& timeSource)
{
    TimeSourceONoList::iterator iter(std::find(m_timeSourceList.begin(), m_timeSourceList.end(), timeSource.GetObjectNumber()));
    if (m_timeSourceList.end() != iter)
    {
        static_cast<void>(m_timeSourceList.erase(iter));

        // Check if the time source is the currende device time source and reset the current device time source
        if (m_currentDeviceTimeSource == timeSource.GetObjectNumber())
        {
            static_cast<void>(SetCurrentDeviceTimeSourceValue(OCA_INVALID_ONO));
            m_currentDeviceTimeSource = OCA_INVALID_ONO;

            ::OcaLitePropertyID propertyId(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_CURRENT_DEVICE_TIME_SOURCE));
            ::OcaLitePropertyChangedEventData< ::OcaONo> eventData(OBJECT_NUMBER,
                                                               propertyId,
                                                               OCA_INVALID_ONO,
                                                               OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);

            PropertyChanged(eventData, propertyId);
        }

        ::OcaLiteList< ::OcaONo> timeSourceList;
        if (OCASTATUS_OK == InternalGetTimeSources(timeSourceList))
        {
            ::OcaLitePropertyID propertyId(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_TIME_SOURCES));
            ::OcaLitePropertyChangedEventData< ::OcaLiteList< ::OcaONo> > eventDataState(OBJECT_NUMBER,
                                                                                 propertyId,
                                                                                 timeSourceList,
                                                                                 OCAPROPERTYCHANGETYPE_ITEM_DELETED);

            PropertyChanged(eventDataState, propertyId);
        }
        else
        {
            OCA_LOG_WARNING("Failed to retrieve time source list");
        }
    }
}

::OcaLiteStatus OcaLiteDeviceTimeManager::InternalGetTimeSources(::OcaLiteList< ::OcaONo>& timeSources) const
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);

    if (m_bInitialized)
    {
        timeSources.Clear();
        timeSources.Reserve(static_cast< ::OcaUint16>(m_timeSourceList.size()));
        TimeSourceONoList::const_iterator iter(m_timeSourceList.begin());
        while (m_timeSourceList.end() != iter)
        {
            timeSources.Add(*iter);
            ++iter;
        }

        rc = OCASTATUS_OK;
    }

    return rc;
}

::OcaLiteStatus OcaLiteDeviceTimeManager::GetCurrentDeviceTimeSourceValue(::OcaONo& timeSource) const
{
    timeSource = m_currentDeviceTimeSource;
    return OCASTATUS_OK;
}
