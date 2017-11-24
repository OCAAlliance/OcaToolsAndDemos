/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteSwitch
 *
 */
// ---- Include system wide include files ----
#include <assert.h>


// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteSwitch.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_SWITCH_CLASSID};
const ::OcaLiteClassID          OcaLiteSwitch::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     static_cast< ::OcaClassVersionNumber>(0)

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteSwitch::OcaLiteSwitch(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports, ::OcaUint16 minPosition, ::OcaUint16 maxPosition)
    : ::OcaLiteActuator(objectNumber, lockable, role, ports),
      m_position(minPosition),
      m_minPosition(minPosition),
      m_maxPosition(maxPosition),
      m_positionNames(),
      m_positionEnable()
{
    assert(minPosition <= maxPosition);
    // Add dummy enabled empty positions 
    for (::OcaUint16 position(static_cast< ::OcaUint16>(0)); position < ((maxPosition - minPosition) + static_cast< ::OcaUint16>(1))/*Total number of available positions*/; position++)
    {
        m_positionNames.Add( ::OcaLiteString());
        m_positionEnable.Add(static_cast< ::OcaBoolean>(true));
    }
}

OcaLiteSwitch::OcaLiteSwitch(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports, ::OcaUint16 minPosition,
                     ::OcaUint16 maxPosition, const ::OcaLiteList< ::OcaLiteString>& positionNames, const ::OcaLiteList< ::OcaBoolean>& positionEnable)
    : ::OcaLiteActuator(objectNumber, lockable, role, ports),
      m_position(minPosition),
      m_minPosition(minPosition),
      m_maxPosition(maxPosition),
      m_positionNames(positionNames),
      m_positionEnable(positionEnable)
{
    assert(minPosition <= maxPosition);
    assert(positionEnable.GetCount() == positionNames.GetCount());
    assert(positionEnable.GetCount() == ((maxPosition - minPosition) + static_cast< ::OcaUint16>(1))/*Total number of available positions*/);
}

::OcaLiteStatus OcaLiteSwitch::GetPosition(::OcaUint16& position, ::OcaUint16& minPosition, ::OcaUint16& maxPosition) const
{
    TakeMutex();

    ::OcaLiteStatus rc(GetPositionValue(position));
    minPosition = m_minPosition;
    maxPosition = m_maxPosition;

    ReleaseMutex();

    return rc;
}

::OcaLiteStatus OcaLiteSwitch::SetPosition(::OcaUint16 position)
{
    TakeMutex();

    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);
    if ((position >= m_minPosition) && (position <= m_maxPosition))
    {
        ::OcaUint16 oldPosition;
        rc = GetPositionValue(oldPosition);
        if (oldPosition != position)
        {
            rc = SetPositionValue(position);
            if (OCASTATUS_OK == rc)
            {
                m_position = position;

                ::OcaUint16 actualPosition;
                rc = GetPositionValue(actualPosition);
                if (OCASTATUS_OK == rc)
                {
                    ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_POSITION));
                    ::OcaLitePropertyChangedEventData< ::OcaUint16> eventData(GetObjectNumber(),
                                                                          propertyID,
                                                                          actualPosition,
                                                                          OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
                    ReleaseMutex();
                    PropertyChanged(eventData);
                    TakeMutex();
                }
            }

        }
    }

    ReleaseMutex();

    return rc;
}

::OcaLiteStatus OcaLiteSwitch::GetPositionName(::OcaUint16 index, ::OcaLiteString& name) const
{
    TakeMutex();

    ::OcaLiteStatus rc(GetPositionNameValue(index, name));

    ReleaseMutex();

    return rc;
}

::OcaLiteStatus OcaLiteSwitch::SetPositionName(::OcaUint16 index, const ::OcaLiteString& name)
{
    TakeMutex();

    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);
    if ((index >= m_minPosition) && (index <= m_maxPosition))
    {
        ::OcaLiteString oldName;
        rc = GetPositionNameValue(index, oldName);
        if (OCASTATUS_OK == rc)
        {
            if (name != oldName)
            {
                rc = SetPositionNameValue(index, name);
                if (OCASTATUS_OK == rc)
                {
                    m_positionNames.RemovePosition(index - m_minPosition);
                    m_positionNames.Insert(index - m_minPosition, name);

                    ::OcaLiteList< ::OcaLiteString> actualNames;
                    rc = GetPositionNamesValue(actualNames);
                    if (OCASTATUS_OK == rc)
                    {
                        ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_POSITION_NAMES));
                        ::OcaLitePropertyChangedEventData< ::OcaLiteList< ::OcaLiteString> > eventData(GetObjectNumber(),
                                                                                           propertyID,
                                                                                           actualNames,
                                                                                           OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
                        ReleaseMutex();
                        PropertyChanged(eventData);
                        TakeMutex();
                    }
                }
            }
        }
    }

    ReleaseMutex();

    return rc;
}

::OcaLiteStatus OcaLiteSwitch::GetPositionNames(::OcaLiteList< ::OcaLiteString>& names) const
{
    TakeMutex();

    ::OcaLiteStatus rc(GetPositionNamesValue(names));

    ReleaseMutex();

    return rc;
}

::OcaLiteStatus OcaLiteSwitch::SetPositionNames(const ::OcaLiteList< ::OcaLiteString>& names)
{
    TakeMutex();

    ::OcaLiteList< ::OcaLiteString> oldNames;
    ::OcaLiteStatus rc(GetPositionNamesValue(oldNames));
    if (OCASTATUS_OK == rc)
    {
        // Number of items in the new list must be equal
        if (names.GetCount() == oldNames.GetCount())
        {
            if (names != oldNames)
            {
                rc = SetPositionNamesValue(names);
                if (OCASTATUS_OK == rc)
                {
                    m_positionNames = names;

                    ::OcaLiteList< ::OcaLiteString> actualNames;
                    rc = GetPositionNamesValue(actualNames);
                    if (OCASTATUS_OK == rc)
                    {
                        ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_POSITION_NAMES));
                        ::OcaLitePropertyChangedEventData< ::OcaLiteList< ::OcaLiteString> > eventData(GetObjectNumber(),
                                                                                           propertyID,
                                                                                           actualNames,
                                                                                           OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
                        ReleaseMutex();
                        PropertyChanged(eventData);
                        TakeMutex();
                    }
                }
            }
        }
        else
        {
            rc = OCASTATUS_PARAMETER_ERROR;
        }
    }

    ReleaseMutex();

    return rc;
}

::OcaLiteStatus OcaLiteSwitch::GetPositionEnabled(::OcaUint16 index, ::OcaBoolean& enabled) const
{
    TakeMutex();

    ::OcaLiteStatus rc(GetPositionEnabledValue(index, enabled));

    ReleaseMutex();

    return rc;
}

::OcaLiteStatus OcaLiteSwitch::SetPositionEnabled(::OcaUint16 index, ::OcaBoolean enabled)
{
    TakeMutex();

    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);
    if ((index >= m_minPosition) && (index <= m_maxPosition))
    {
        ::OcaBoolean oldEnabled;
        rc = GetPositionEnabledValue(index, oldEnabled);
        if (OCASTATUS_OK == rc)
        {
            if ( ((oldEnabled && !enabled) || (!oldEnabled && enabled)))
            {
                rc = SetPositionEnabledValue(index, enabled);
                if (OCASTATUS_OK == rc)
                {
                    m_positionEnable.RemovePosition(index - m_minPosition);
                    m_positionEnable.Insert(index - m_minPosition, enabled);

                    ::OcaLiteList< ::OcaBoolean> actualEnableds;
                    rc = GetPositionEnabledsValue(actualEnableds);
                    if (OCASTATUS_OK == rc)
                    {
                        ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_POSITION_ENABLE));
                        ::OcaLitePropertyChangedEventData< ::OcaLiteList< ::OcaBoolean> > eventData(GetObjectNumber(),
                                                                                            propertyID,
                                                                                            actualEnableds,
                                                                                            OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
                        ReleaseMutex();
                        PropertyChanged(eventData);
                        TakeMutex();
                    }
                }
            }
        }
    }

    ReleaseMutex();

    return rc;
}

::OcaLiteStatus OcaLiteSwitch::GetPositionEnableds(::OcaLiteList< ::OcaBoolean>& enableds) const
{
    TakeMutex();

    ::OcaLiteStatus rc(GetPositionEnabledsValue(enableds));

    ReleaseMutex();

    return rc;
}

::OcaLiteStatus OcaLiteSwitch::SetPositionEnableds(const ::OcaLiteList< ::OcaBoolean>& enableds)
{
    TakeMutex();

    ::OcaLiteList< ::OcaBoolean> oldEnableds;
    ::OcaLiteStatus rc(GetPositionEnabledsValue(oldEnableds));
    if (OCASTATUS_OK == rc)
    {
        // Number of items in the new list must be equal
        if (enableds.GetCount() == oldEnableds.GetCount())
        {
            if (enableds != oldEnableds)
            {
                rc = SetPositionEnabledsValue(enableds);
                if (OCASTATUS_OK == rc)
                {
                    m_positionEnable = enableds;

                    ::OcaLiteList< ::OcaBoolean> actualEnableds;
                    rc = GetPositionEnabledsValue(actualEnableds);
                    if (OCASTATUS_OK == rc)
                    {
                        ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_POSITION_ENABLE));
                        ::OcaLitePropertyChangedEventData< ::OcaLiteList< ::OcaBoolean> > eventData(GetObjectNumber(),
                                                                                            propertyID,
                                                                                            actualEnableds,
                                                                                            OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
                        ReleaseMutex();
                        PropertyChanged(eventData);
                        TakeMutex();
                    }
                }
            }
        }
        else
        {
            rc = OCASTATUS_PARAMETER_ERROR;
        }
    }

    ReleaseMutex();

    return rc;
}

::OcaLiteStatus OcaLiteSwitch::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_POSITION:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint16 position;
                        ::OcaUint16 minPosition;
                        ::OcaUint16 maxPosition;
                        rc = GetPosition(position, minPosition, maxPosition);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(3), writer) + 
                                                     ::GetSizeValue< ::OcaUint16>(position, writer) + 
													 ::GetSizeValue< ::OcaUint16>(minPosition, writer) + 
													 ::GetSizeValue< ::OcaUint16>(maxPosition, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
							    ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(3/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaUint16>(position, &pResponse, writer);
                                ::MarshalValue< ::OcaUint16>(minPosition, &pResponse, writer);
                                ::MarshalValue< ::OcaUint16>(maxPosition, &pResponse, writer);

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
            case SET_POSITION:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaUint16 position;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
						reader.Read(bytesLeft, &pCmdParameters, position))
                    {
                        rc = SetPosition(position);
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
            case GET_POSITION_NAME:
                {
                    ::OcaUint8 numberOfParameters(0);
					::OcaUint16 index;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
						reader.Read(bytesLeft, &pCmdParameters, index))
                    {
                        ::OcaLiteString name;
                        rc = GetPositionName(index, name);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + name.GetSize(writer));
                            responseBuffer  = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                name.Marshal(&pResponse, writer);

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
            case SET_POSITION_NAME:
                {
                    ::OcaUint8 numberOfParameters(0);
					::OcaUint16 index;
					::OcaLiteString name;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (2 == numberOfParameters) &&
						reader.Read(bytesLeft, &pCmdParameters, index) &&
                        name.Unmarshal(bytesLeft, &pCmdParameters, reader))
                    {
                        rc = SetPositionName(index, name); //lint !e864 Expression does not depend on order of evaluation
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
            case GET_POSITION_NAMES:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaLiteString> names;
                        rc = GetPositionNames(names);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + names.GetSize(writer));
                            responseBuffer  = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                names.Marshal(&pResponse, writer);

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
            case SET_POSITION_NAMES:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaLiteList< ::OcaLiteString> names;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        names.Unmarshal(bytesLeft, &pCmdParameters, reader))
                    {
                        rc = SetPositionNames(names);
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
            case GET_POSITION_ENABLED:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaUint16 index;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
						reader.Read(bytesLeft, &pCmdParameters, index))
                    {
                        ::OcaBoolean enabled;
                        rc = GetPositionEnabled(index, enabled);
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
            case SET_POSITION_ENABLED:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaUint16 index;
                    ::OcaBoolean enabled;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (2 == numberOfParameters) &&
						reader.Read(bytesLeft, &pCmdParameters, index) &&
						reader.Read(bytesLeft, &pCmdParameters, enabled))
                    {
                        rc = SetPositionEnabled(index, enabled); //lint !e864 Expression does not depend on order of evaluation
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
            case GET_POSITION_ENABLEDS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteList< ::OcaBoolean> enableds;
                        rc = GetPositionEnableds(enableds);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + enableds.GetSize(writer));
                            responseBuffer  = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                enableds.Marshal(&pResponse, writer);

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
            case SET_POSITION_ENABLEDS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaLiteList< ::OcaBoolean> enableds;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        enableds.Unmarshal(bytesLeft, &pCmdParameters, reader))
                    {
                        rc = SetPositionEnableds(enableds);
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
            rc = OcaLiteActuator::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

//lint -e{835} A zero has been given as right argument to operator '+'
::OcaClassVersionNumber OcaLiteSwitch::GetClassVersion() const
{
    return (OcaLiteActuator::GetClassVersion() + CLASS_VERSION_INCREMENT);
}

::OcaLiteStatus OcaLiteSwitch::GetPositionValue(::OcaUint16& position) const
{
    position = m_position;
    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteSwitch::GetPositionNameValue(::OcaUint16 index, ::OcaLiteString& name) const
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);
    if ((index >= m_minPosition) && (index <= m_maxPosition))
    {
        name = m_positionNames.GetItem(index - m_minPosition);
        rc = OCASTATUS_OK;
    }
    return rc;
}

::OcaLiteStatus OcaLiteSwitch::GetPositionNamesValue(::OcaLiteList< ::OcaLiteString>& names) const
{
    names = m_positionNames;
    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteSwitch::GetPositionEnabledValue(::OcaUint16 index, ::OcaBoolean& enabled) const
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);
    if ((index >= m_minPosition) && (index <= m_maxPosition))
    {
        enabled = m_positionEnable.GetItem(index - m_minPosition);
        rc = OCASTATUS_OK;
    }
    return rc;
}

::OcaLiteStatus OcaLiteSwitch::GetPositionEnabledsValue(::OcaLiteList< ::OcaBoolean>& enableds) const
{
    enableds = m_positionEnable;
    return OCASTATUS_OK;
}
