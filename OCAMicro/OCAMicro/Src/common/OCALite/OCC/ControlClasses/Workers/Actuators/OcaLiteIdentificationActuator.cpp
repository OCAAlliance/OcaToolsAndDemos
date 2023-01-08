/*  By downloading or using this file, the user agrees to be bound by the terms of the license
*  agreement located in the LICENSE file in the root of this project
*  as an original contracting party.
*/

/*
*  Description         : OcaLiteIdentificationActuator
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
#include "OcaLiteIdentificationActuator.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_IDENTIFICATION_ACTUATOR_CLASSID};
const ::OcaLiteClassID              OcaLiteIdentificationActuator::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteIdentificationActuator::OcaLiteIdentificationActuator(::OcaONo objectNumber,
                                                     ::OcaBoolean lockable,
                                                     const ::OcaLiteString& role,
                                                     const ::OcaLiteList< ::OcaLitePort>& ports)
    : ::OcaLiteActuator(objectNumber, lockable, role, ports),
      m_active(static_cast< ::OcaBoolean>(false))
{
}

::OcaLiteStatus OcaLiteIdentificationActuator::GetActive(::OcaBoolean& active) const
{

    ::OcaLiteStatus rc(GetActiveValue(active));

    return rc;
}

::OcaLiteStatus OcaLiteIdentificationActuator::SetActive(::OcaBoolean active)
{

    ::OcaBoolean oldActive;
    ::OcaLiteStatus rc(GetActiveValue(oldActive));
    if ((OCASTATUS_OK == rc) &&
        ((oldActive && !active) || (!oldActive && active)))
    {
        rc = SetActiveValue(active);
        if (OCASTATUS_OK == rc)
        {
            m_active = active;

            ::OcaBoolean actualActive;
            rc = GetActiveValue(actualActive);
            if (OCASTATUS_OK == rc)
            {
                ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_ACTIVE));
                ::OcaLitePropertyChangedEventData< ::OcaBoolean> eventData(GetObjectNumber(),
                                                                       propertyID,
                                                                       actualActive,
                                                                       OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
                PropertyChanged(eventData, propertyID);
            }
        }
    }

    return rc;
}

::OcaLiteStatus OcaLiteIdentificationActuator::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_ACTIVE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaBoolean active;
                        rc = GetActive(active);
                        if (OCASTATUS_OK == rc) 
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     ::GetSizeValue< ::OcaBoolean>(active, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaBoolean>(active, &pResponse, writer);

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
            case SET_ACTIVE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaBoolean active;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        reader.Read(bytesLeft, &pCmdParameters, active))
                    {
                        rc = SetActive(active);
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
            rc = OcaLiteActuator::Execute(reader, writer, sessionID, methodID,  parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

/*const ::OcaLitePropertyDescriptor& OcaLiteIdentificationActuator::GetActivePropertyDescriptor()
{
    static const ::OcaLitePropertyDescriptor descriptor(::OcaLitePropertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_ACTIVE)),
                                                    OCABASEDATATYPE_BOOLEAN,
                                                    ::OcaMethodID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(GET_ACTIVE)),
                                                    ::OcaMethodID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(SET_ACTIVE)));
    return descriptor;
}*/

::OcaClassVersionNumber OcaLiteIdentificationActuator::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteActuator::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

::OcaLiteStatus OcaLiteIdentificationActuator::GetActiveValue(::OcaBoolean& active) const
{
    active = m_active;
    return OCASTATUS_OK;
}
