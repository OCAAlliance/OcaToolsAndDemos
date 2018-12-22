/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteMediaClock3
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
#include "OcaLiteMediaClock3.h"

// ---- Helper types and constants ----
static const ::OcaUint16        classID[]   = {OCA_MEDIACLOCK3_CLASSID};
const ::OcaLiteClassID          OcaLiteMediaClock3::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     -1
// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMediaClock3::OcaLiteMediaClock3(::OcaONo objectNumber,
                                       ::OcaBoolean lockable,
                                       const ::OcaLiteString& role,
                                       ::OcaLiteMediaClockAvailability availablility,
                                       ::OcaLiteMediaClockRate& rate,
                                       ::OcaONo timeSourceONo,
                                       ::OcaLiteTimeOffset offset,
                                       const ::OcaLiteList< ::OcaLiteMediaClockRate>& ratesSupported)
    : ::OcaLiteAgent(objectNumber, lockable, role),
    m_availablility(availablility),
    m_rate(rate),
    m_timeSourceONo(timeSourceONo),
    m_offset(offset),
    m_ratesSupported(ratesSupported)
{
    ::OcaBoolean result(::OcaLiteMediaClockManager::GetInstance().AddMediaClock3(*this));
    if (!result)
    {
        OCA_LOG_ERROR_PARAMS("Unable to add media clock %u to media clock manager", objectNumber);
    }
}

OcaLiteMediaClock3::~OcaLiteMediaClock3()
{
    ::OcaLiteMediaClockManager::GetInstance().RemoveMediaClock3(*this);
}

::OcaLiteStatus OcaLiteMediaClock3::GetAvailability(::OcaLiteMediaClockAvailability& availablility) const
{
    availablility = m_availablility;

    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteMediaClock3::SetAvailability(::OcaLiteMediaClockAvailability availablility)
{
    return OCASTATUS_NOT_IMPLEMENTED;
}

::OcaLiteStatus OcaLiteMediaClock3::GetCurrentRate(::OcaLiteMediaClockRate& rate, ::OcaONo& timeSourceONo) const
{
    rate = m_rate;
    timeSourceONo = m_timeSourceONo;

    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteMediaClock3::SetCurrentRate(::OcaLiteMediaClockRate rate, ::OcaONo timeSourceONo)
{
    return OCASTATUS_NOT_IMPLEMENTED;
}
::OcaLiteStatus OcaLiteMediaClock3::GetOffset(::OcaLiteTimeOffset& offset)
{
    m_offset = offset;

    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteMediaClock3::SetOffset(::OcaLiteTimeOffset offset)
{
    return OCASTATUS_NOT_IMPLEMENTED;
}

::OcaLiteStatus OcaLiteMediaClock3::GetSupportedRates(::OcaLiteList< ::OcaLiteMediaClockRate> ratesSupported)
{
    ratesSupported = m_ratesSupported;

    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteMediaClock3::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_AVAILABILITY:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteMediaClockAvailability availability;
                        rc = GetAvailability(availability);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     ::GetSizeValue< ::OcaLiteMediaClockAvailability>(availability, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaLiteMediaClockAvailability>(availability, &pResponse, writer);

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
            case SET_AVAILABILITY:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaLiteMediaClockAvailability availability;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        ::UnmarshalValue< ::OcaLiteMediaClockAvailability>(availability, bytesLeft, &pCmdParameters, reader))
                    {
                        rc = SetAvailability(availability);
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
            case GET_CURRENT_RATE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteMediaClockRate rate;
                        ::OcaONo ono;
                        rc = GetCurrentRate(rate, ono);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(2), writer) + 
                                                     rate.GetSize(writer) +
                                                     ::GetSizeValue< ::OcaONo>(ono, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(2/*NrParameters*/), &pResponse);
                                rate.Marshal(&pResponse, writer);
                                ::MarshalValue< ::OcaONo>(ono, &pResponse, writer);

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
            case SET_CURRENT_RATE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaLiteMediaClockRate rate;
                    ::OcaONo ono;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (2 == numberOfParameters) &&
                        rate.Unmarshal(bytesLeft, &pCmdParameters, reader) &&
                        ::UnmarshalValue< ::OcaONo>(ono, bytesLeft, &pCmdParameters, reader))
                    {
                        rc = SetCurrentRate(rate, ono);
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
            case GET_OFFSET:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteTimeOffset timeOffset;
                        rc = GetOffset(timeOffset);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     ::GetSizeValue< ::OcaLiteTimeOffset>(timeOffset, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaLiteTimeOffset>(timeOffset, &pResponse, writer);

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
            case SET_OFFSET:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaLiteTimeOffset offset;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        ::UnmarshalValue< ::OcaLiteTimeOffset>(offset, bytesLeft, &pCmdParameters, reader))
                    {
                        rc = SetOffset(offset);
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
            case GET_SUPPORTED_RATES:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaLiteMediaClockRate> supportedRates;
                        rc = GetSupportedRates(supportedRates);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + supportedRates.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
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

::OcaClassVersionNumber OcaLiteMediaClock3::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteAgent::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

// ---- Function Implementation ----
template <>
void MarshalValue< ::OcaLiteMediaClockAvailability>(const ::OcaLiteMediaClockAvailability& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
bool UnmarshalValue< ::OcaLiteMediaClockAvailability>(::OcaLiteMediaClockAvailability& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 availability(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, availability));
    if (result)
    {
        if (availability < static_cast< ::OcaUint8>(OCAMEDIACLOCKAVAILABILITY_MAXIMUM))
        {
            value = static_cast< ::OcaLiteMediaClockAvailability>(availability);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteMediaClockAvailability>(const ::OcaLiteMediaClockAvailability& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}
