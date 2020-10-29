/*  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteStringSensor
 *
 */
// ---- Include system wide include files ----
#include <assert.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
//#include <OCF/OcaLiteMessageParameters.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteStringSensor.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_STRING_SENSOR_CLASSID};
const ::OcaLiteClassID          OcaLiteStringSensor::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteStringSensor::OcaLiteStringSensor(::OcaONo objectNumber,
                                           ::OcaBoolean lockable,
                                           const ::OcaLiteString& role,
                                           const ::OcaLiteList< ::OcaLitePort>& ports)
    : ::OcaLiteBasicSensor(objectNumber, lockable, role, ports),
      m_maxLen(static_cast< ::OcaUint16>(0xFFFF))
{
}

::OcaLiteStatus OcaLiteStringSensor::GetString(::OcaLiteString& stringData) const
{
    ::OcaLiteString tempString;
    ::OcaLiteStatus rc(GetStringValue(tempString));

    //Trim the string in case it exceeds the limit
    if(OCASTATUS_OK == rc && tempString.GetLength() > m_maxLen)
    {
        std::string adjustedStringData(tempString.GetString().substr(0, static_cast<UINT16>(m_maxLen)));
        stringData = ::OcaLiteString(adjustedStringData.c_str());
    }
    else
    {
        stringData = tempString;
    }

    return rc;
}
::OcaLiteStatus OcaLiteStringSensor::GetMaxLen(::OcaUint16& maxLength) const
{
    maxLength = m_maxLen;
    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteStringSensor::SetMaxLen(::OcaUint16 maxLength)
{
    ::OcaUint16 oldMaxLen;
    ::OcaLiteStatus rc(GetMaxLengthValue(oldMaxLen));
    if((OCASTATUS_OK == rc) && (oldMaxLen != maxLength))
    {
        rc = SetMaxLengthValue(maxLength);
        if(OCASTATUS_OK == rc)
        {
            m_maxLen = maxLength;
            ::OcaUint16 actualLength;
            rc = GetMaxLengthValue(actualLength);
            if(OCASTATUS_OK == rc)
            {
                ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_MAX_LEN));
                ::OcaLitePropertyChangedEventData< ::OcaUint16> eventData(GetObjectNumber(),
                                                                            propertyID,
                                                                            actualLength,
                                                                            OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
                PropertyChanged(eventData, propertyID);
            }
        }
    }
    return OCASTATUS_OK;
}


::OcaLiteStatus OcaLiteStringSensor::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_STRING:
                {
                    //ToDo: kl�ren, wie das mit den parametersIn.ReadParameters(tInput, reader)) aus der Boschimplementierung aussieht
                    ::OcaLiteString stringData;
                    rc = GetString(stringData);
                    if ((OCASTATUS_OK == rc) && (NULL != response))
                    {
                           ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                                             ::GetSizeValue< ::OcaLiteString>(stringData, writer));
                                                    responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                                                    if (NULL != responseBuffer)
                                                    {
                                                        ::OcaUint8* pResponse(responseBuffer);
                                                        writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                                        ::MarshalValue< ::OcaLiteString>(stringData, &pResponse, writer);
                                                        *response = responseBuffer;
                                                    }
                                                    else
                                                    {
                                                        rc = OCASTATUS_BUFFER_OVERFLOW;
                                                    }



                    }
                    break;
                }
            case GET_MAX_LEN:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint16 maxLen;
                        rc = GetMaxLen(maxLen);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     ::GetSizeValue< ::OcaUint16>(maxLen, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaUint16>(maxLen, &pResponse, writer);

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
            case SET_MAX_LEN:
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
            rc = OcaLiteBasicSensor::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

::OcaLiteStatus OcaLiteStringSensor::GetMaxLengthValue(::OcaUint16& maxLength) const
{
    maxLength = m_maxLen;
    return OCASTATUS_OK;
}

::OcaClassVersionNumber OcaLiteStringSensor::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteBasicSensor::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}


void OcaLiteStringSensor::StringChanged(::OcaLiteString string)
{
    ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_STRING));
    ::OcaLitePropertyChangedEventData< ::OcaLiteString> eventData(GetObjectNumber(),
                                                          propertyID,
                                                          string,
                                                          OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
    PropertyChanged(eventData, propertyID);
}

