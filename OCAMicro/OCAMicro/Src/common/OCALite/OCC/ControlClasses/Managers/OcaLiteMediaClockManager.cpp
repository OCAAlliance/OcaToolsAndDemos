/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : The OcaLiteMediaClockManager.
 *
 */

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- Include local include files ----
#include "OcaLiteMediaClockManager.h"

// ---- Helper types and constants ----
static const ::OcaUint16        classID[]   = {OCA_MEDIACLOCKMANAGER_CLASSID};
const ::OcaLiteClassID          OcaLiteMediaClockManager::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----
::OcaLiteMediaClockManager* OcaLiteMediaClockManager::m_pSingleton(NULL);
const ::OcaONo OcaLiteMediaClockManager::OBJECT_NUMBER(static_cast< ::OcaONo>(7));

OcaLiteMediaClockManager::OcaLiteMediaClockManager()
    : ::OcaLiteManager(OBJECT_NUMBER, ::OcaLiteString("MediaClockManager"), ::OcaLiteString("MediaClockManager")),
      m_mediaClockList(),
      m_bInitialized(false),
      m_mediaClockTypeList()
{
    // Add all known media clock types to the map of clock types and initialize the counter to 0
    for (int mcType(static_cast<int>(OCAMEDIACLOCKTYPE_NONE)); mcType < static_cast<int>(OCAMEDIACLOCKTYPE_MAXIMUM); mcType++)
    {
        ::OcaLiteMediaClockType clockType(static_cast< ::OcaLiteMediaClockType>(mcType));
        static_cast<void>(m_mediaClockTypeList.insert(MediaClockTypeList::value_type(clockType, 0)));
    }
}

OcaLiteMediaClockManager::~OcaLiteMediaClockManager()
{
    m_mediaClockList.clear();
    m_mediaClockTypeList.clear();
}

::OcaLiteMediaClockManager& OcaLiteMediaClockManager::GetInstance()
{
    if (NULL == OcaLiteMediaClockManager::m_pSingleton)
    {
        OcaLiteMediaClockManager::m_pSingleton = new ::OcaLiteMediaClockManager;
    }

    return *OcaLiteMediaClockManager::m_pSingleton;
}

void OcaLiteMediaClockManager::FreeInstance()
{
    if (OcaLiteMediaClockManager::m_pSingleton != NULL)
    {
        delete m_pSingleton;
        m_pSingleton = NULL;
    }
}

::OcaBoolean OcaLiteMediaClockManager::Initialize()
{
    ::OcaBoolean bResult(OcaLiteManager::Initialize());

    if (bResult)
    {
        m_bInitialized = true;
    }

    return bResult;
}

::OcaBoolean OcaLiteMediaClockManager::Shutdown()
{
    m_bInitialized = false;

    return OcaLiteManager::Shutdown();
}

::OcaLiteStatus OcaLiteMediaClockManager::GetClocks(::OcaLiteList< ::OcaONo>& clocks) const
{
    return InternalGetClocks(clocks);
}

::OcaLiteStatus OcaLiteMediaClockManager::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_CLOCKS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaONo> clockList;
                        rc = GetClocks(clockList);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + clockList.GetSize(writer));
                            responseBuffer  = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                clockList.Marshal(&pResponse, writer);

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
            case GET_MEDIACLOCKTYPESSUPPORTED:
                rc = OCASTATUS_NOT_IMPLEMENTED;
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

::OcaClassVersionNumber OcaLiteMediaClockManager::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteManager::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

::OcaBoolean OcaLiteMediaClockManager::AddMediaClock(OcaLiteMediaClock& clock)
{
    ::OcaBoolean result(static_cast< ::OcaBoolean>(false));

    if (m_mediaClockList.end() == m_mediaClockList.find(clock.GetObjectNumber()))
    {
        static_cast<void>(m_mediaClockList.insert(MediaClockList::value_type(clock.GetObjectNumber(), &clock)));

        bool clockTypesChanged(false);
        ::OcaLiteMediaClockType clockType;
        ::OcaLiteStatus rc(clock.GetType(clockType));
        if (OCASTATUS_OK == rc)
        {
            assert(clockType < OCAMEDIACLOCKTYPE_MAXIMUM);
            assert(m_mediaClockTypeList.end() != m_mediaClockTypeList.find(clockType));
            m_mediaClockTypeList[clockType]++;
            clockTypesChanged = (1 == m_mediaClockTypeList[clockType]);
        }

        if (OCASTATUS_OK == rc)
        {
            ::OcaLiteList< ::OcaONo> clockList;
            rc = InternalGetClocks(clockList);
            if (OCASTATUS_OK == rc)
            {
                ::OcaLitePropertyID propertyId(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_CLOCKS));
                ::OcaLitePropertyChangedEventData< ::OcaLiteList< ::OcaONo> > eventDataState(OBJECT_NUMBER,
                                                                                     propertyId,
                                                                                     clockList,
                                                                                     OCAPROPERTYCHANGETYPE_ITEM_ADDED);
                PropertyChanged(eventDataState);

                result = static_cast< ::OcaBoolean>(true);
            }
            else
            {
                OCA_LOG_WARNING("Failed to retrieve clock list");
            }
        }

        if (clockTypesChanged)
        {
            NotifySupportedClockTypesChanged(OCAPROPERTYCHANGETYPE_ITEM_ADDED);
        }
    }

    return result;
}

void OcaLiteMediaClockManager::RemoveMediaClock(const OcaLiteMediaClock& clock)
{
    MediaClockList::iterator iter(m_mediaClockList.find(clock.GetObjectNumber()));
    if (m_mediaClockList.end() != iter)
    {
        static_cast<void>(m_mediaClockList.erase(iter));

        bool clockTypesChanged(false);
        ::OcaLiteMediaClockType clockType;
        ::OcaLiteStatus rc(clock.GetType(clockType));
        if (OCASTATUS_OK == rc)
        {
            assert(clockType < OCAMEDIACLOCKTYPE_MAXIMUM);
            assert(m_mediaClockTypeList.end() != m_mediaClockTypeList.find(clockType));
            m_mediaClockTypeList[clockType]--;
            clockTypesChanged = (0 == m_mediaClockTypeList[clockType]);
        }

        ::OcaLiteList< ::OcaONo> clockList;
        if (OCASTATUS_OK == InternalGetClocks(clockList))
        {
            ::OcaLitePropertyID propertyId(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_CLOCKS));
            ::OcaLitePropertyChangedEventData< ::OcaLiteList< ::OcaONo> > eventDataState(OBJECT_NUMBER,
                                                                                 propertyId,
                                                                                 clockList,
                                                                                 OCAPROPERTYCHANGETYPE_ITEM_DELETED);
            PropertyChanged(eventDataState);
        }
        else
        {
            OCA_LOG_WARNING("Failed to retrieve clock list");
        }

        if (clockTypesChanged)
        {
            NotifySupportedClockTypesChanged(OCAPROPERTYCHANGETYPE_ITEM_DELETED);
        }
    }
}

::OcaLiteStatus OcaLiteMediaClockManager::InternalGetClocks(::OcaLiteList< ::OcaONo>& clocks) const
{
    ::OcaLiteStatus rc(OCASTATUS_DEVICE_ERROR);

    if (m_bInitialized)
    {
        clocks.Clear();
        MediaClockList::const_iterator iter(m_mediaClockList.begin());
        while (m_mediaClockList.end() != iter)
        {
            clocks.Add(iter->first);
            ++iter;
        }

        rc = OCASTATUS_OK;
    }

    return rc;
}

void OcaLiteMediaClockManager::NotifySupportedClockTypesChanged(::OcaLitePropertyChangeType changeType)
{
    ::OcaLiteList< ::OcaLiteMediaClockType> clockTypes;
    clockTypes.Reserve(static_cast< ::OcaUint16>(m_mediaClockTypeList.size()));

    for (MediaClockTypeList::iterator iter(m_mediaClockTypeList.begin());
         iter != m_mediaClockTypeList.end();
         ++iter)
    {
        if (iter->second > 0)
        {
            clockTypes.Add(iter->first);
        }
    }

    ::OcaLitePropertyID propertyId(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_CLOCK_SOURCE_TYPES_SUPPORTED));
    ::OcaLitePropertyChangedEventData< ::OcaLiteList< ::OcaLiteMediaClockType> > eventDataState(OBJECT_NUMBER,
                                                                                    propertyId,
                                                                                    clockTypes,
                                                                                    changeType);
    PropertyChanged(eventDataState);
}
