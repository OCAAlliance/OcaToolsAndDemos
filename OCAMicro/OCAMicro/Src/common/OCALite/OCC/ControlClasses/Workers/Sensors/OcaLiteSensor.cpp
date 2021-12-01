/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteSensor
 *
 */
// ---- Include system wide include files ----
#include <assert.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCF/OcaLiteCommandHandler.h>
#include "OCC/ControlDataTypes/OcaLitePropertyID.h"
#include "OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h"

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteSensor.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_SENSOR_CLASSID};
const ::OcaLiteClassID          OcaLiteSensor::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteSensor::OcaLiteSensor(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports)
    : ::OcaLiteWorker(objectNumber, lockable, role, ports)
{
}

::OcaLiteStatus OcaLiteSensor::GetReadingState(::OcaLiteSensorReadingState& state) const
{
    ::OcaLiteStatus rc(GetReadingStateValue(state));

    return rc;
}

::OcaLiteStatus OcaLiteSensor::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_READING_STATE:
                {
                      ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                              ::OcaLiteSensorReadingState state;
                              rc = GetReadingState(state);
                        if (OCASTATUS_OK == rc)
                              {
                                    ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     ::GetSizeValue< ::OcaLiteSensorReadingState>(state, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                       ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaLiteSensorReadingState>(state, &pResponse, writer);

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
            rc = OcaLiteWorker::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

::OcaClassVersionNumber OcaLiteSensor::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteWorker::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

void OcaLiteSensor::ReadingStateChanged(::OcaLiteSensorReadingState state)
{
    ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_READING_STATE));
    ::OcaLitePropertyChangedEventData< ::OcaLiteSensorReadingState> eventData(GetObjectNumber(),
                                                                      propertyID,
                                                                      state,
                                                                      OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
    PropertyChanged(eventData, propertyID);
}

// ---- Function Implementation ----

template <>
void MarshalValue< ::OcaLiteSensorReadingState>(const ::OcaLiteSensorReadingState& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
bool UnmarshalValue< ::OcaLiteSensorReadingState>(::OcaLiteSensorReadingState& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 status(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, status));
    if (result)
    {
        if (status < static_cast< ::OcaUint8>(OCALITESENSORREADINGSTATE_MAXIMUM))
        {
            value = static_cast< ::OcaLiteSensorReadingState>(status);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteSensorReadingState>(const ::OcaLiteSensorReadingState& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}
