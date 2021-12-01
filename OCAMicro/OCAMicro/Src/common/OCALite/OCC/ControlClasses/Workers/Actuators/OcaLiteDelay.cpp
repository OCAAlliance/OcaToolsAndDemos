/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

 /*
  *  Description         : OcaLiteDelay
  *
  */
// ---- Include system wide include files ----
#include <assert.h>


// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteDelay.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_DELAY_CLASSID};
const ::OcaLiteClassID          OcaLiteDelay::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteDelay::OcaLiteDelay(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports, ::OcaTimeInterval minDelayTime, ::OcaTimeInterval maxDelayTime)
    : ::OcaLiteActuator(objectNumber, lockable, role, ports),
      m_delayTime(minDelayTime),
      m_minDelayTime(minDelayTime),
      m_maxDelayTime(maxDelayTime)
{
    assert(minDelayTime <= maxDelayTime);
}

::OcaLiteStatus OcaLiteDelay::GetDelayTime(::OcaTimeInterval& time, ::OcaTimeInterval& minTime, ::OcaTimeInterval& maxTime) const
{
    minTime = m_minDelayTime;
    maxTime = m_maxDelayTime;
    return GetDelayTimeValue(time);
}

::OcaLiteStatus OcaLiteDelay::SetDelayTime(::OcaTimeInterval time)
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);


    if ((time >= m_minDelayTime) && (time <= m_maxDelayTime))
    {
        ::OcaTimeInterval oldTime;
        rc = GetDelayTimeValue(oldTime);

        if ((OCASTATUS_OK == rc) && !CompareValue< ::OcaTimeInterval>(time, oldTime))
        {
            rc = SetDelayTimeValue(time);
            if (OCASTATUS_OK == rc)
            {
                m_delayTime = time;

                ::OcaTimeInterval actualTime;
                rc = GetDelayTimeValue(actualTime);
                if (OCASTATUS_OK == rc)
                {
                    ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_DELAY_TIME));
                    ::OcaLitePropertyChangedEventData< ::OcaTimeInterval> eventData(GetObjectNumber(),
                                                                                propertyID,
                                                                                actualTime,
                                                                                OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
                    PropertyChanged(eventData, propertyID);
                }
            }
        }
    }
    return rc;
}



::OcaLiteStatus OcaLiteDelay::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_DELAY_TIME:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaTimeInterval delayTime;
                        ::OcaTimeInterval minDelayTime;
                        ::OcaTimeInterval maxDelayTime;
                        rc = GetDelayTime(delayTime, minDelayTime, maxDelayTime);
                        if (OCASTATUS_OK == rc) 
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast<::OcaUint8>(3), writer) +
                                ::GetSizeValue< ::OcaDB>(delayTime, writer) +
                                ::GetSizeValue< ::OcaDB>(minDelayTime, writer) +
                                ::GetSizeValue< ::OcaDB>(maxDelayTime, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast<::OcaUint8>(3/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaDB>(delayTime, &pResponse, writer);
                                ::MarshalValue< ::OcaDB>(minDelayTime, &pResponse, writer);
                                ::MarshalValue< ::OcaDB>(maxDelayTime, &pResponse, writer);

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
            case SET_DELAY_TIME:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaTimeInterval delayTime;

                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        reader.Read(bytesLeft, &pCmdParameters, delayTime))
                    {
                        rc = SetDelayTime(delayTime);
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
            rc = OcaLiteActuator::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}


//lint -e{835} A zero has been given as right argument to operator '+'
::OcaClassVersionNumber OcaLiteDelay::GetClassVersion() const
{
    return static_cast<::OcaClassVersionNumber>(static_cast<int> (OcaLiteActuator::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}


::OcaLiteStatus OcaLiteDelay::GetDelayTimeValue(::OcaTimeInterval& time) const
{
    time = m_delayTime;
    return OCASTATUS_OK;
}
