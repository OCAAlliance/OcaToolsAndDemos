/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteWorker
 *
 */
// ---- Include system wide include files ----
#include <assert.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteWorker.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_WORKER_CLASSID};
const ::OcaLiteClassID          OcaLiteWorker::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     static_cast< ::OcaClassVersionNumber>(0)

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteWorker::OcaLiteWorker(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports)
    : ::OcaLiteRoot(objectNumber, lockable, role),
      m_bEnabled(static_cast< ::OcaBoolean>(true)),
      m_ocaPorts(ports)
{
    assert((objectNumber == OCA_ROOT_BLOCK_ONO) || (objectNumber >= OCA_MINIMUM_DEVICE_OBJECT_ONO));
}

OcaLiteWorker::~OcaLiteWorker()
{
    if (GetObjectNumber() != OCA_ROOT_BLOCK_ONO)
    {
        ::OcaLiteBlock::GetRootBlock().RemoveObject(GetObjectNumber());
    }
}

::OcaLiteStatus OcaLiteWorker::GetEnabled(::OcaBoolean& enabled) const
{
    ::OcaLiteStatus rc(GetEnabledValue(enabled));
    return rc;
}

::OcaLiteStatus OcaLiteWorker::SetEnabled(::OcaBoolean bEnabled)
{
    ::OcaBoolean oldEnabled;
    ::OcaLiteStatus rc(GetEnabledValue(oldEnabled));
    if ((OCASTATUS_OK == rc) &&
        ((m_bEnabled && !bEnabled) || (!m_bEnabled && bEnabled)))
    {
        rc = SetEnabledValue(bEnabled);
        if (OCASTATUS_OK == rc)
        {
            m_bEnabled = bEnabled;

            ::OcaBoolean actualEnabled;
            rc = GetEnabledValue(actualEnabled);
            if (OCASTATUS_OK == rc)
            {
                ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_ENABLED));
                ::OcaLitePropertyChangedEventData< ::OcaBoolean> eventData(GetObjectNumber(),
                                                                       propertyID,
                                                                       actualEnabled,
                                                                       OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
                PropertyChanged(eventData);
            }
            else
            {
                OCA_LOG_WARNING_PARAMS("Unable to retrieve new value after setting (rc = %u)", rc);
            }
        }
    }
    return rc;
}

::OcaLiteStatus OcaLiteWorker::GetPorts(::OcaLiteList< ::OcaLitePort>& ports) const
{
    ports = m_ocaPorts;
    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteWorker::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_ENABLED:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaBoolean enabled;
                        rc = GetEnabled(enabled);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     ::GetSizeValue< ::OcaBoolean>(enabled, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaBoolean>(enabled, &pResponse, writer);

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
            case SET_ENABLED:
                {
                    ::OcaBoolean enabled;
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        ::UnmarshalValue< ::OcaBoolean>(enabled, bytesLeft, &pCmdParameters, reader))
                    {
                        rc = SetEnabled(enabled);
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
            case GET_PORTS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaLitePort> ports;
                        rc = GetPorts(ports);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     ports.GetSize(writer)); 
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ports.Marshal(&pResponse, writer);

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
            case GET_PORT_NAME:
            case SET_PORT_NAME:
            case GET_LABEL:
            case SET_LABEL:
            case GET_OWNER:
            case GET_LATENCY:
            case SET_LATENCY:
            case GET_PATH:
            case ADD_PORT:
            case DELETE_PORT:
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
            rc = OcaLiteRoot::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

//lint -e{835} A zero has been given as right argument to operator '+'
::OcaClassVersionNumber OcaLiteWorker::GetClassVersion() const
{
    return (OcaLiteRoot::GetClassVersion() + CLASS_VERSION_INCREMENT);
}

::OcaLiteStatus OcaLiteWorker::GetEnabledValue(::OcaBoolean& enabled) const
{
    enabled = m_bEnabled;
    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteWorker::SetEnabledValue(::OcaBoolean bEnabled)
{
    return OCASTATUS_NOT_IMPLEMENTED;
}
