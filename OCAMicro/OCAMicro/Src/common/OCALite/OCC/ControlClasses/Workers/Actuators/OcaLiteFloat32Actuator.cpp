/*  By downloading or using this file, the user agrees to be bound by the terms of the license
*  agreement located at http://ocaalliance.com/EULA as an original contracting party.
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
#include "OcaLiteFloat32Actuator.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_FLOAT32_ACTUATOR_CLASSID};
const ::OcaLiteClassID              OcaLiteFloat32Actuator::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteFloat32Actuator::OcaLiteFloat32Actuator(::OcaONo objectNumber, 
                                               ::OcaBoolean lockable, const 
                                               ::OcaLiteString& role, const 
                                               ::OcaLiteList< ::OcaLitePort>& ports, 
                                               ::OcaFloat32 minSetting, 
                                               ::OcaFloat32 maxSetting)
    : ::OcaLiteBasicActuator(objectNumber, lockable, role, ports),
      m_setting(minSetting),
      m_minSetting(minSetting),
      m_maxSetting(maxSetting)
{
    assert(minSetting <= maxSetting);
}

::OcaLiteStatus OcaLiteFloat32Actuator::GetSetting(::OcaFloat32& setting, ::OcaFloat32& minSetting, ::OcaFloat32& maxSetting) const
{
    ::OcaLiteStatus rc(GetSettingValue(setting));
    minSetting = m_minSetting;
    maxSetting = m_maxSetting;

    return rc;
}

::OcaLiteStatus OcaLiteFloat32Actuator::SetSetting(::OcaFloat32 setting)
{

    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);
    if ((setting >= m_minSetting) && (setting <= m_maxSetting))
    {
        ::OcaFloat32 oldSetting;
        rc = GetSettingValue(oldSetting);
        if ((OCASTATUS_OK == rc) && (!CompareValue< ::OcaFloat32>(oldSetting, setting)))
        {
            rc = SetSettingValue(setting);
            if (OCASTATUS_OK == rc)
            {
                m_setting = setting;

                ::OcaFloat32 actualSetting;
                rc = GetSettingValue(actualSetting);
                if (OCASTATUS_OK == rc)
                {
                    ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_SETTING));
                    ::OcaLitePropertyChangedEventData< ::OcaFloat32> eventData(GetObjectNumber(),
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

::OcaLiteStatus OcaLiteFloat32Actuator::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
                        ::OcaFloat32 setting;
                        ::OcaFloat32 minSetting;
                        ::OcaFloat32 maxSetting;
                        rc = GetSetting(setting, minSetting, maxSetting);
                        if (OCASTATUS_OK == rc) 
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(3), writer) +
                                                     ::GetSizeValue< ::OcaFloat32>(setting, writer) +
                                                     ::GetSizeValue< ::OcaFloat32>(minSetting, writer) +
                                                     ::GetSizeValue< ::OcaFloat32>(maxSetting, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(3/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaFloat32>(setting, &pResponse, writer);
                                ::MarshalValue< ::OcaFloat32>(minSetting, &pResponse, writer);
                                ::MarshalValue< ::OcaFloat32>(maxSetting, &pResponse, writer);

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
                    ::OcaFloat32 setting;
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


::OcaClassVersionNumber OcaLiteFloat32Actuator::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteBasicActuator::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

::OcaLiteStatus OcaLiteFloat32Actuator::GetSettingValue(::OcaFloat32& setting) const
{
    setting = m_setting;
    return OCASTATUS_OK;
}
