/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : The OcaLiteFirmwareManager.
 *
 */

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteBlob.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLiteNetworkAddress.h>
#include <OCC/ControlDataTypes/OcaLiteVersion.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- Include local include files ----
#include "OcaLiteFirmwareManager.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_FIRMWAREMANAGER_CLASSID};
const ::OcaLiteClassID          OcaLiteFirmwareManager::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

/** The main version */
#ifndef MAINVERSION
#define MAINVERSION 0
#endif
/** The subversion */
#ifndef SUBVERSION
#define SUBVERSION 1
#endif
/* The build number */
#ifndef BUILDNUMBER
#define BUILDNUMBER 0
#endif

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----
::OcaLiteFirmwareManager* OcaLiteFirmwareManager::m_pSingleton(NULL);
const ::OcaONo OcaLiteFirmwareManager::OBJECT_NUMBER(static_cast< ::OcaONo>(3));

OcaLiteFirmwareManager::OcaLiteFirmwareManager()
  : ::OcaLiteManager(OBJECT_NUMBER, ::OcaLiteString("FirmwareManager"), ::OcaLiteString("FirmwareManager")),
  m_updatingSession(OCA_INVALID_SESSIONID)
{
}

OcaLiteFirmwareManager::~OcaLiteFirmwareManager()
{
}

::OcaLiteFirmwareManager& OcaLiteFirmwareManager::GetInstance()
{
    if (NULL == OcaLiteFirmwareManager::m_pSingleton)
    {
        OcaLiteFirmwareManager::m_pSingleton = new ::OcaLiteFirmwareManager;
    }

    return *OcaLiteFirmwareManager::m_pSingleton;
}

void OcaLiteFirmwareManager::FreeInstance()
{
    if (OcaLiteFirmwareManager::m_pSingleton != NULL)
    {
        delete m_pSingleton;
        m_pSingleton = NULL;
    }
}

::OcaLiteStatus OcaLiteFirmwareManager::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_COMPONENT_VERSIONS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaLiteVersion> componentVersions;
                        rc = GetComponentVersions(componentVersions);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     componentVersions.GetSize(writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                componentVersions.Marshal(&pResponse, writer);

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
            case START_UPDATE_PROCESS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        if (OCA_INVALID_SESSIONID == m_updatingSession)
                        {
                            // Store this session ID to be the one to accept updates from
                            m_updatingSession = sessionID;
                            rc = OCASTATUS_OK;

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
                        else
                        {
                            rc = OCASTATUS_INVALID_REQUEST;
                        }
                    }
                }
                break;
            case BEGIN_ACTIVE_IMAGE_UPDATE:
                // TODO
                break;
            case ADD_IMAGE_DATA:
                {
                    ::OcaUint32 id;
                    ::OcaLiteBlob data;
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (2 == numberOfParameters) &&
                        reader.Read(bytesLeft, &pCmdParameters, id) &&
                        data.Unmarshal(bytesLeft, &pCmdParameters, reader))
                    {
                        if (sessionID == m_updatingSession)
                        {
                            // TODO Check the sequence ID. If the next one push the data to the next processor

                            rc = OCASTATUS_OK;

                            // All fine
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
                        else
                        {
                            rc = OCASTATUS_INVALID_REQUEST;
                        }
                    }
                }
                break;
            case VERIFY_IMAGE:
                {
                    ::OcaLiteBlob data;
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        data.Unmarshal(bytesLeft, &pCmdParameters, reader))
                    {
                        if (sessionID == m_updatingSession)
                        {
                            // Verify the calculated hash with the one in the data
                            OcaUint8 calculatedHash[4] = { 0 };

                            // TODO fill calculatedHash

                            if ((data.GetDataSize() == static_cast< ::OcaUint16>(4)) &&
                                (calculatedHash[0] == data.GetData()[0]) &&
                                (calculatedHash[1] == data.GetData()[1]) &&
                                (calculatedHash[2] == data.GetData()[2]) &&
                                (calculatedHash[3] == data.GetData()[3]))
                            {
                                rc = OCASTATUS_OK;

                                // All fine
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
                            else
                            {
                                rc = OCASTATUS_DEVICE_ERROR;
                            }
                        }
                        else
                        {
                            rc = OCASTATUS_INVALID_REQUEST;
                        }
                    }
                }
                break;
            case END_ACTIVE_IMAGE_UPDATE:
                //TODO
                break;
            case BEGIN_PASSIVE_COMPONENT_UPDATE:
                //TODO
                break;
            case END_UPDATE_PROCESS:
                //TODO
                break;
            default:
                rc = OCASTATUS_BAD_METHOD;
                break;
            }
        }
        else
        {
            rc = OcaLiteManager::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

::OcaLiteStatus OcaLiteFirmwareManager::GetComponentVersions(::OcaLiteList< ::OcaLiteVersion>& componentVersions) const
{
    componentVersions.Clear();
    componentVersions.Add(::OcaLiteVersion(static_cast< ::OcaUint32>(MAINVERSION), 
                                           static_cast< ::OcaUint32>(SUBVERSION), 
                                           static_cast< ::OcaUint32>(BUILDNUMBER), 
                                           static_cast< ::OcaLiteComponent>(0)));

    return OCASTATUS_OK;
}

::OcaClassVersionNumber OcaLiteFirmwareManager::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteManager::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}
