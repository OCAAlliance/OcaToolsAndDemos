/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteMediaClock
 *
 */
// ---- Include system wide include files ----
#include <assert.h>
#include <OCC/ControlClasses/Managers/OcaLiteMediaClockManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteDeviceManager.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- Include local include files ----
#include "OcaLiteMediaClock.h"

// ---- Helper types and constants ----
static const ::OcaUint16        classID[]   = {OCA_MEDIACLOCK_CLASSID};
const ::OcaLiteClassID          OcaLiteMediaClock::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMediaClock::OcaLiteMediaClock(::OcaONo objectNumber,
                             ::OcaBoolean lockable,
                             const ::OcaLiteString& role,
                             ::OcaLiteMediaClockType type,
                             ::OcaUint16 domainID,
                             const ::OcaLiteList< ::OcaLiteMediaClockRate>& ratesSupported)
    : ::OcaLiteAgent(objectNumber, lockable, role),
      m_clockType(type),
      m_domainID(domainID),
      m_ratesSupported(ratesSupported),
      m_typesSupported()
{
    // Add all clock types if none provided
    m_typesSupported.Add(OCAMEDIACLOCKTYPE_INTERNAL);
    m_typesSupported.Add(OCAMEDIACLOCKTYPE_EXTERNAL);
    m_typesSupported.Add(OCAMEDIACLOCKTYPE_NETWORK);
    m_typesSupported.Add(OCAMEDIACLOCKTYPE_NONE);

    ::OcaBoolean result(::OcaLiteMediaClockManager::GetInstance().AddMediaClock(*this));
    if (!result)
    {
        OCA_LOG_ERROR_PARAMS("Unable to add media clock %u to media clock manager", objectNumber);
    }
}

OcaLiteMediaClock::OcaLiteMediaClock(::OcaONo objectNumber,
                             ::OcaBoolean lockable,
                             const ::OcaLiteString& role,
                             ::OcaLiteMediaClockType type,
                             const ::OcaLiteList< ::OcaLiteMediaClockType> typesSupported,
                             ::OcaUint16 domainID,
                             const ::OcaLiteList< ::OcaLiteMediaClockRate>& ratesSupported)
    : ::OcaLiteAgent(objectNumber, lockable, role),
      m_clockType(type),
      m_domainID(domainID),
      m_ratesSupported(ratesSupported),
      m_typesSupported(typesSupported)
{
    ::OcaBoolean result(::OcaLiteMediaClockManager::GetInstance().AddMediaClock(*this));
    if (!result)
    {
        OCA_LOG_ERROR_PARAMS("Unable to add media clock %u to media clock manager", objectNumber);
    }
}

OcaLiteMediaClock::~OcaLiteMediaClock()
{
    ::OcaLiteMediaClockManager::GetInstance().RemoveMediaClock(*this);
}

::OcaLiteStatus OcaLiteMediaClock::GetType(::OcaLiteMediaClockType& type) const
{
    return GetTypeValue(type);
}

::OcaLiteStatus OcaLiteMediaClock::SetType(::OcaLiteMediaClockType type)
{
    ::OcaLiteMediaClockType oldType;
    ::OcaLiteStatus rc(GetTypeValue(oldType));
    if ((OCASTATUS_OK == rc) && (oldType != type))
    {
        rc = SetTypeValue(type);
        if (OCASTATUS_OK == rc)
        {
            m_clockType = type;

            ClockTypeChanged(m_clockType);
        }
    }
    return rc;
}

::OcaLiteStatus OcaLiteMediaClock::GetDomainID(::OcaUint16& id) const
{
    return GetDomainIDValue(id);
}

::OcaLiteStatus OcaLiteMediaClock::SetDomainID(::OcaUint16 id)
{
    ::OcaUint16 oldID;
    ::OcaLiteStatus rc(GetDomainIDValue(oldID));
    if ((OCASTATUS_OK == rc) && (oldID != id))
    {
        rc = SetDomainIDValue(id);
        if (OCASTATUS_OK == rc)
        {
            m_domainID = id;

            ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_DOMAIN_ID));
            ::OcaLitePropertyChangedEventData< ::OcaUint16> eventData(GetObjectNumber(),
                                                                  propertyID,
                                                                  m_domainID,
                                                                  OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
            PropertyChanged(eventData, propertyID);
        }
    }

    return rc;
}

::OcaLiteStatus OcaLiteMediaClock::GetTypesSupported(::OcaLiteList< ::OcaLiteMediaClockType>& types) const
{
    types = m_typesSupported;
    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteMediaClock::GetRatesSupported(::OcaLiteList< ::OcaLiteMediaClockRate>& rates) const
{
    rates = m_ratesSupported;
    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteMediaClock::GetRate(::OcaLiteMediaClockRate& rate) const
{
    return GetRateValue(rate);
}

::OcaLiteStatus OcaLiteMediaClock::SetRate(const ::OcaLiteMediaClockRate& rate)
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);
    if (m_ratesSupported.Contains(rate))
    {
        ::OcaLiteMediaClockRate oldRate;
        rc = GetRateValue(oldRate);
        if ((OCASTATUS_OK == rc) && (oldRate != rate))
        {
            rc = SetRateValue(rate);
            if (OCASTATUS_OK == rc)
            {
                ::OcaLiteMediaClockRate actualRate;
                rc = GetRateValue(actualRate);
                if (OCASTATUS_OK == rc)
                {
                    ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_CURRENT_RATE));
                    ::OcaLitePropertyChangedEventData< ::OcaLiteMediaClockRate> eventData(GetObjectNumber(),
                                                                                    propertyID,
                                                                                    actualRate,
                                                                                    OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
                    PropertyChanged(eventData, propertyID);
                }
                else
                {
                    OCA_LOG_WARNING_PARAMS("Unable to retrieve new value after setting (rc = %u)", rc);
                }
            }
        }
    }
    return rc;
}

::OcaLiteStatus OcaLiteMediaClock::GetLockState(::OcaLiteMediaClockLockState& state) const
{
    return GetLockStateValue(state);
}

::OcaLiteStatus OcaLiteMediaClock::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_TYPE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteMediaClockType type;
                        rc = GetType(type);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     ::GetSizeValue< ::OcaLiteMediaClockType>(type, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaLiteMediaClockType>(type, &pResponse, writer);

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
            case SET_TYPE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaUint8 type;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        reader.Read(bytesLeft, &pCmdParameters, type))
                    {
                        rc = SetType((OcaLiteMediaClockType)type);
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
            case GET_DOMAIN_ID:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint16 domainID;
                        rc = GetDomainID(domainID);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     ::GetSizeValue< ::OcaUint16>(domainID, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaUint16>(domainID, &pResponse, writer);

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
            case SET_DOMAIN_ID:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaUint16 domainID;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        reader.Read(bytesLeft, &pCmdParameters, domainID))
                    {
                        rc = SetDomainID(domainID);
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
            case GET_RATES_SUPPORTED:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaLiteMediaClockRate> supportedRates;
                        rc = GetRatesSupported(supportedRates);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + supportedRates.GetSize(writer));
                            responseBuffer  = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                supportedRates.Marshal(&pResponse, writer);

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
            case GET_RATE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteMediaClockRate rate;
                        rc = GetRate(rate);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     ::GetSizeValue< ::OcaLiteMediaClockRate>(rate, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaLiteMediaClockRate>(rate, &pResponse, writer);

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
            case SET_RATE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaLiteMediaClockRate rate;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        rate.Unmarshal(bytesLeft, &pCmdParameters, reader))
                    {
                        rc = SetRate(rate);
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
            case GET_LOCK_STATE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteMediaClockLockState state;
                        rc = GetLockState(state);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     ::GetSizeValue< ::OcaLiteMediaClockLockState>(state, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaLiteMediaClockLockState>(state, &pResponse, writer);

                                *response = responseBuffer;
                            }
                            else
                            {
                                rc = OCASTATUS_BUFFER_OVERFLOW;
                            }
                        }
                    }
                }
            case GET_TYPES_SUPPORTED:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaLiteMediaClockType> supportedTypes;
                        rc = GetTypesSupported(supportedTypes);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + supportedTypes.GetSize(writer));
                            responseBuffer  = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                supportedTypes.Marshal(&pResponse, writer);

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
            rc = OcaLiteAgent::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }
    return rc;
}

::OcaClassVersionNumber OcaLiteMediaClock::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteAgent::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

void OcaLiteMediaClock::LockStateChanged(::OcaLiteMediaClockLockState state)
{
    ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_LOCK_STATE));
    ::OcaLitePropertyChangedEventData< ::OcaLiteMediaClockLockState> eventData(GetObjectNumber(),
                                                                       propertyID,
                                                                       state,
                                                                       OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
    PropertyChanged(eventData, propertyID);
}

void OcaLiteMediaClock::ClockTypeChanged(::OcaLiteMediaClockType type)
{
    m_clockType = type;

    ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_TYPE));
    ::OcaLitePropertyChangedEventData< ::OcaLiteMediaClockType> eventData(GetObjectNumber(),
                                                                  propertyID,
                                                                  type,
                                                                  OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
    PropertyChanged(eventData, propertyID);
}

::OcaLiteStatus OcaLiteMediaClock::GetTypeValue(::OcaLiteMediaClockType& type) const
{
    type = m_clockType;
    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteMediaClock::GetDomainIDValue(::OcaUint16& id) const
{
    id = m_domainID;
    return OCASTATUS_OK;
}

// ---- Function Implementation ----
template <>
void MarshalValue< ::OcaLiteMediaClockLockState>(const ::OcaLiteMediaClockLockState& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
void MarshalValue< ::OcaLiteMediaClockType>(const ::OcaLiteMediaClockType& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
bool UnmarshalValue< ::OcaLiteMediaClockLockState>(::OcaLiteMediaClockLockState& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 mediaClockLockState(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, mediaClockLockState));
    if (result)
    {
        if (mediaClockLockState < static_cast< ::OcaUint8>(OCAMEDIACLOCKLOCKSTATE_MAXIMUM))
        {
            value = static_cast< ::OcaLiteMediaClockLockState>(mediaClockLockState);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
bool UnmarshalValue< ::OcaLiteMediaClockType>(::OcaLiteMediaClockType& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 mediaClockType(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, mediaClockType));
    if (result)
    {
        if (mediaClockType < static_cast< ::OcaUint8>(OCAMEDIACLOCKTYPE_MAXIMUM))
        {
            value = static_cast< ::OcaLiteMediaClockType>(mediaClockType);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteMediaClockLockState>(const ::OcaLiteMediaClockLockState& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteMediaClockType>(const ::OcaLiteMediaClockType& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}
