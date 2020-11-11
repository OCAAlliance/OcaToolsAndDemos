/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteTimeSource
 *
 */

// ---- Include system wide include files ----
#include <assert.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/OcaLiteCommandHandler.h>

// ---- Include local include files ----
#include "OcaLiteTimeSource.h"

// ---- Helper types and constants ----
static const ::OcaUint16        classID[]   = {OCA_TIMESOURCE_CLASSID};
const ::OcaLiteClassID              OcaLiteTimeSource::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     -1

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteTimeSource::OcaLiteTimeSource(::OcaONo objectNumber,
                                     ::OcaBoolean lockable,
                                     const ::OcaLiteString& role,
                                     ::OcaLiteTimeSourceAvailability availability,
                                     ::OcaLiteTimeProtocol protocol,
                                     const ::OcaLiteString& parameters,
                                     ::OcaLiteTimeReferenceType referenceType,
                                     const ::OcaLiteString& referenceID,
                                     ::OcaLiteTimeSourceSyncStatus syncStatus)
    : ::OcaLiteAgent(objectNumber, lockable, role),
      m_availability(availability),
      m_protocol(protocol),
      m_parameters(parameters),
      m_referenceType(referenceType),
      m_referenceID(referenceID),
      m_syncStatus(syncStatus)
{
}

OcaLiteTimeSource::~OcaLiteTimeSource()
{
}

::OcaLiteStatus OcaLiteTimeSource::GetAvailability(::OcaLiteTimeSourceAvailability& availability) const
{
    availability = m_availability;

    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteTimeSource::GetProtocol(::OcaLiteTimeProtocol& protocol) const
{
    protocol = m_protocol;

    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteTimeSource::SetProtocol(::OcaLiteTimeProtocol protocol)
{
    return OCASTATUS_NOT_IMPLEMENTED;
}

::OcaLiteStatus OcaLiteTimeSource::GetParameters(::OcaLiteString& parameters) const
{
    parameters = m_parameters;

    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteTimeSource::SetParameters(const ::OcaLiteString& parameters)
{
    return OCASTATUS_NOT_IMPLEMENTED;
}

::OcaLiteStatus OcaLiteTimeSource::GetReferenceType(::OcaLiteTimeReferenceType& referenceType) const
{
    referenceType = m_referenceType;

    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteTimeSource::SetReferenceType(::OcaLiteTimeReferenceType referenceType)
{
    return OCASTATUS_NOT_IMPLEMENTED;
}

::OcaLiteStatus OcaLiteTimeSource::GetReferenceID(::OcaLiteString& id) const
{
    id = m_referenceID;

    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteTimeSource::SetReferenceID(const ::OcaLiteString& id)
{
    ::OcaLiteStatus rc(OCASTATUS_OK);
    if (m_referenceID != id)
    {
        rc = SetReferenceIDValue(id);
        if (OCASTATUS_OK == rc)
        {
            ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_REFERENCE_ID));
            ::OcaLitePropertyChangedEventData< ::OcaLiteString> eventData(GetObjectNumber(),
                                                                          propertyID,
                                                                          id,
                                                                          OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
            PropertyChanged(eventData, propertyID);
        }
    }

    return rc;
}

::OcaLiteStatus OcaLiteTimeSource::GetSyncStatus(::OcaLiteTimeSourceSyncStatus& syncStatus) const
{
    syncStatus = m_syncStatus;

    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteTimeSource::Reset()
{
    return OCASTATUS_NOT_IMPLEMENTED;
}

::OcaLiteStatus OcaLiteTimeSource::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
                        ::OcaLiteTimeSourceAvailability availability;
                        rc = GetAvailability(availability);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     ::GetSizeValue< ::OcaLiteTimeSourceAvailability>(availability, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaLiteTimeSourceAvailability>(availability, &pResponse, writer);

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
            case GET_PROTOCOL:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteTimeProtocol protocol;
                        rc = GetProtocol(protocol);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     ::GetSizeValue< ::OcaLiteTimeProtocol>(protocol, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaLiteTimeProtocol>(protocol, &pResponse, writer);

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
            case SET_PROTOCOL:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaLiteTimeProtocol protocol;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        ::UnmarshalValue< ::OcaLiteTimeProtocol>(protocol, bytesLeft, &pCmdParameters, reader))
                    {
                        rc = SetProtocol(protocol);
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
            case GET_PARAMETERS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteString parameters;
                        rc = GetParameters(parameters);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     parameters.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                parameters.Marshal(&pResponse, writer);

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
            case SET_PARAMETERS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaLiteString parameters;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        ::UnmarshalValue< ::OcaLiteString>(parameters, bytesLeft, &pCmdParameters, reader))
                    {
                            rc = SetParameters(parameters);
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
            case GET_REFERENCE_TYPE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteTimeReferenceType referenceType;
                        rc = GetReferenceType(referenceType);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     ::GetSizeValue< ::OcaLiteTimeReferenceType>(referenceType, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaLiteTimeReferenceType>(referenceType, &pResponse, writer);

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
            case SET_REFERENCE_TYPE:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaLiteTimeReferenceType referenceType;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        ::UnmarshalValue< ::OcaLiteTimeReferenceType>(referenceType, bytesLeft, &pCmdParameters, reader))
                    {
                        rc = SetReferenceType(referenceType);
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
            case GET_REFERENCE_ID:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteString referenceID;
                        rc = GetReferenceID(referenceID);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     ::GetSizeValue< ::OcaLiteString>(referenceID, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaLiteString>(referenceID, &pResponse, writer);

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
            case SET_REFERENCE_ID:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaLiteString referenceId;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        ::UnmarshalValue< ::OcaLiteString>(referenceId, bytesLeft, &pCmdParameters, reader))
                    {
                        rc = SetReferenceID(referenceId);
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
            case GET_SYNC_STATUS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteTimeSourceSyncStatus syncStatus;
                        rc = GetSyncStatus(syncStatus);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) +
                                                     ::GetSizeValue< ::OcaLiteTimeSourceSyncStatus>(syncStatus, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaLiteTimeSourceSyncStatus>(syncStatus, &pResponse, writer);

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
            case RESET:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        rc = Reset();
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
            rc = OcaLiteAgent::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }
    return rc;
}

const ::OcaLiteClassID& OcaLiteTimeSource::GetClassID() const
{
    return CLASS_ID;
}

::OcaClassVersionNumber OcaLiteTimeSource::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteAgent::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}


::OcaLiteStatus OcaLiteTimeSource::SetReferenceIDValue(const ::OcaLiteString&)
{
    return OCASTATUS_NOT_IMPLEMENTED;
}

void OcaLiteTimeSource::SetSyncStatusValue(::OcaLiteTimeSourceSyncStatus syncStatus)
{
    if (m_syncStatus != syncStatus)
    {
        m_syncStatus = syncStatus;

        ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_SYNC_STATUS));
        ::OcaLitePropertyChangedEventData< ::OcaLiteTimeSourceSyncStatus> eventData(GetObjectNumber(),
                                                                                    propertyID,
                                                                                    m_syncStatus,
                                                                                    OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
        PropertyChanged(eventData, propertyID);
    }
}

// ---- Function Implementation ----

template <>
void MarshalValue< ::OcaLiteTimeProtocol>(const ::OcaLiteTimeProtocol& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
void MarshalValue< ::OcaLiteTimeReferenceType>(const ::OcaLiteTimeReferenceType& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
void MarshalValue< ::OcaLiteTimeSourceAvailability>(const ::OcaLiteTimeSourceAvailability& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
void MarshalValue< ::OcaLiteTimeSourceSyncStatus>(const ::OcaLiteTimeSourceSyncStatus& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    MarshalValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), destination, writer);
}

template <>
bool UnmarshalValue< ::OcaLiteTimeProtocol>(::OcaLiteTimeProtocol& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 availability(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, availability));
    if (result)
    {
        if (availability < static_cast< ::OcaUint8>(OCATIMEPROTOCOL_MAXIMUM))
        {
            value = static_cast< ::OcaLiteTimeProtocol>(availability);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
bool UnmarshalValue< ::OcaLiteTimeReferenceType>(::OcaLiteTimeReferenceType& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 availability(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, availability));
    if (result)
    {
        if (availability < static_cast< ::OcaUint8>(OCATIMEREFERENCETYPE_MAXIMUM))
        {
            value = static_cast< ::OcaLiteTimeReferenceType>(availability);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
bool UnmarshalValue< ::OcaLiteTimeSourceAvailability>(::OcaLiteTimeSourceAvailability& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 availability(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, availability));
    if (result)
    {
        if (availability < static_cast< ::OcaUint8>(OCATIMESOURCEAVAILABILITY_MAXIMUM))
        {
            value = static_cast< ::OcaLiteTimeSourceAvailability>(availability);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
bool UnmarshalValue< ::OcaLiteTimeSourceSyncStatus>(::OcaLiteTimeSourceSyncStatus& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    ::OcaUint8 availability(static_cast< ::OcaUint8>(0));
    bool result(reader.Read(bytesLeft, source, availability));
    if (result)
    {
        if (availability < static_cast< ::OcaUint8>(OCATIMESOURCESYNCSTATUS_MAXIMUM))
        {
            value = static_cast< ::OcaLiteTimeSourceSyncStatus>(availability);
        }
        else
        {
            result = false;
        }
    }

    return result;
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteTimeProtocol>(const ::OcaLiteTimeProtocol& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteTimeReferenceType>(const ::OcaLiteTimeReferenceType& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteTimeSourceAvailability>(const ::OcaLiteTimeSourceAvailability& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}

template <>
::OcaUint32 GetSizeValue< ::OcaLiteTimeSourceSyncStatus>(const ::OcaLiteTimeSourceSyncStatus& value, const ::IOcaLiteWriter& writer)
{
    return GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(value), writer);
}
