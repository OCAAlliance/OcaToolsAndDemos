/*  By downloading or using this file, the user agrees to be bound by the terms of the license
*  agreement located in the LICENSE file in the root of this project
*  as an original contracting party.
*/

/*
*  Description         : OcaLiteInt32Actuator
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
#include "OcaLiteInt32Actuator.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_INT32_ACTUATOR_CLASSID};
const ::OcaLiteClassID              OcaLiteInt32Actuator::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteInt32Actuator::OcaLiteInt32Actuator(::OcaONo objectNumber, 
                                        ::OcaBoolean lockable, 
                                        const ::OcaLiteString& role, 
                                        const ::OcaLiteList< ::OcaLitePort>& ports, 
                                        ::OcaInt32 minSetting, 
                                        ::OcaInt32 maxSetting)
    : ::OcaLiteBasicActuator(objectNumber, lockable, role, ports),
      m_setting(minSetting),
      m_minSetting(minSetting),
      m_maxSetting(maxSetting)
{
    assert(minSetting <= maxSetting);
}

::OcaLiteStatus OcaLiteInt32Actuator::GetSetting(::OcaInt32& setting, ::OcaInt32& minSetting, ::OcaInt32& maxSetting) const
{


    ::OcaLiteStatus rc(GetSettingValue(setting));
    minSetting = m_minSetting;
    maxSetting = m_maxSetting;

    return rc;
}

::OcaLiteStatus OcaLiteInt32Actuator::SetSetting(::OcaInt32 setting)
{

    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);
    if ((setting >= m_minSetting) && (setting <= m_maxSetting))
    {
        ::OcaInt32 oldSetting;
        rc = GetSettingValue(oldSetting);
        if ((OCASTATUS_OK == rc) && (oldSetting != setting))
        {
            rc = SetSettingValue(setting);
            if (OCASTATUS_OK == rc)
            {
                m_setting = setting;

                ::OcaInt32 actualSetting;
                rc = GetSettingValue(actualSetting);
                if (OCASTATUS_OK == rc)
                {
                    ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_SETTING));
                    ::OcaLitePropertyChangedEventData< ::OcaInt32> eventData(GetObjectNumber(),
                                                                         propertyID,
                                                                         actualSetting,
                                                                         OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
                    PropertyChanged(eventData, propertyID);
                }
            }
        }
    }

    return rc;
}

::OcaLiteStatus OcaLiteInt32Actuator::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_SETTING:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaInt32 setting;
                        ::OcaInt32 minSetting;
                        ::OcaInt32 maxSetting;
                        rc = GetSetting(setting, minSetting, maxSetting);
                        if (OCASTATUS_OK == rc) 
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(3), writer) +
                                                                     ::GetSizeValue< ::OcaInt32>(setting, writer) +
                                                                     ::GetSizeValue< ::OcaInt32>(minSetting, writer) +
                                                                     ::GetSizeValue< ::OcaInt32>(maxSetting, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(3/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaInt32>(setting, &pResponse, writer);
                                ::MarshalValue< ::OcaInt32>(minSetting, &pResponse, writer);
                                ::MarshalValue< ::OcaInt32>(maxSetting, &pResponse, writer);

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
            case SET_SETTING:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaInt32 setting;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        reader.Read(bytesLeft, &pCmdParameters, setting))
                    {
                        rc = SetSetting(setting);
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
            default:
                rc = OCASTATUS_BAD_METHOD;
                break;
            }
        }
        else
        {
            // Should be executed on higher level
            rc = OcaLiteBasicActuator::Execute(reader, writer, sessionID, methodID,  parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}


::OcaClassVersionNumber OcaLiteInt32Actuator::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteBasicActuator::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

::OcaLiteStatus OcaLiteInt32Actuator::GetSettingValue(::OcaInt32& setting) const
{
    setting = m_setting;
    return OCASTATUS_OK;
}
