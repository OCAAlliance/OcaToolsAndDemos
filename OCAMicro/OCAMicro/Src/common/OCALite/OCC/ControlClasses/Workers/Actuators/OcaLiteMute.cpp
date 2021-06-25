/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteMuteActuator
 *
 */
// ---- Include system wide include files ----
#include <assert.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteMute.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_MUTE_CLASSID};
const ::OcaLiteClassID              OcaLiteMute::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteMute::OcaLiteMute(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports)
    : ::OcaLiteActuator(objectNumber, lockable, role, ports),
      m_state(OCAMUTESTATE_MUTED)
{
}

::OcaLiteStatus OcaLiteMute::GetState(::OcaLiteMuteState& state) const
{
    ::OcaLiteStatus rc(GetStateValue(state));

    return rc;
}

::OcaLiteStatus OcaLiteMute::SetState(::OcaLiteMuteState state)
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);
    if(state < ::OcaLiteMuteState::OCAMUTESTATE_MAXIMUM)
    {
        ::OcaLiteMuteState oldState;
        rc = GetStateValue(oldState);
        if ((OCASTATUS_OK == rc) && !CompareValue< ::OcaLiteMuteState>(oldState, state))
        {
            rc = SetStateValue(state);
            if (OCASTATUS_OK == rc)
            {
                m_state = state;
                                    
                ::OcaLiteMuteState actualState;
                rc = GetStateValue(actualState);
                if (OCASTATUS_OK == rc)
                {
                    ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_STATE));
                    ::OcaLitePropertyChangedEventData< ::OcaUint8> eventData(GetObjectNumber(),                            // ::OcaLiteMuteState cast to ::OcaUint8 according to OcaLiteWorkerDataTypes.h
                                                                             propertyID,
                                                                             actualState,
                                                                             OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
                    PropertyChanged(eventData, propertyID);
                }
            }
        }
    }

    return rc;
}

::OcaLiteStatus OcaLiteMute::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_STATE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteMuteState state;
                        rc = GetState(state);
                        if (OCASTATUS_OK == rc) 
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                    ::GetSizeValue< ::OcaUint8>(state, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);

                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaUint8>(state, &pResponse, writer);

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
            case SET_STATE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaUint8 state;                                                        // ::OcaLiteMuteState cast to ::OcaUint8 according to OcaLiteWorkerDataTypes.h
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        reader.Read(bytesLeft, &pCmdParameters, state))
                    {
                        rc = SetState((::OcaLiteMuteState)state);
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

::OcaClassVersionNumber OcaLiteMute::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteActuator::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

::OcaLiteStatus OcaLiteMute::GetStateValue(::OcaLiteMuteState& state) const
{
    state = m_state;
    return OCASTATUS_OK;
}
