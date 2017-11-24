/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteGain
 *
 */
// ---- Include system wide include files ----
#include <assert.h>


// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteGain.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_GAIN_CLASSID};
const ::OcaLiteClassID          OcaLiteGain::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     static_cast< ::OcaClassVersionNumber>(0)

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteGain::OcaLiteGain(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports, ::OcaDB minGain, ::OcaDB maxGain)
    : ::OcaLiteActuator(objectNumber, lockable, role, ports),
      m_gain(minGain),
      m_minGain(minGain),
      m_maxGain(maxGain)
{
    assert(minGain <= maxGain);
}

::OcaLiteStatus OcaLiteGain::GetGain(::OcaDB& gain, ::OcaDB& minGain, ::OcaDB& maxGain) const
{
    TakeMutex();

    ::OcaLiteStatus rc(GetGainValue(gain));
    minGain = m_minGain;
    maxGain = m_maxGain;

    ReleaseMutex();

    return rc;
}

::OcaLiteStatus OcaLiteGain::SetGain(::OcaDB gain)
{
    TakeMutex();

    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_OUT_OF_RANGE);

    if ((gain >= m_minGain) && (gain <= m_maxGain))
    {
        ::OcaDB oldGain;
        rc = GetGainValue(oldGain);
        if (OCASTATUS_OK == rc)
        {
            if (!CompareValue< ::OcaDB>(oldGain, gain))
            {
                rc = SetGainValue(gain);
                if (OCASTATUS_OK == rc)
                {
                    m_gain = gain;

                    ::OcaDB actualGain;
                    rc = GetGainValue(actualGain);
                    if (OCASTATUS_OK == rc)
                    {
                        ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_GAIN));
                        ::OcaLitePropertyChangedEventData< ::OcaDB> eventData(GetObjectNumber(),
                                                                          propertyID,
                                                                          actualGain,
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

::OcaLiteStatus OcaLiteGain::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_GAIN:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaDB gain;
                        ::OcaDB minGain;
                        ::OcaDB maxGain;
                        rc = GetGain(gain, minGain, maxGain);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(3), writer) + 
                                                     ::GetSizeValue< ::OcaDB>(gain, writer) + 
													 ::GetSizeValue< ::OcaDB>(minGain, writer) + 
													 ::GetSizeValue< ::OcaDB>(maxGain, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
							    ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(3/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaDB>(gain, &pResponse, writer);
                                ::MarshalValue< ::OcaDB>(minGain, &pResponse, writer);
                                ::MarshalValue< ::OcaDB>(maxGain, &pResponse, writer);

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
            case SET_GAIN:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaDB gain;

                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
						reader.Read(bytesLeft, &pCmdParameters, gain))
                    {
                        rc = SetGain(gain);
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
::OcaClassVersionNumber OcaLiteGain::GetClassVersion() const
{
    return (OcaLiteActuator::GetClassVersion() + CLASS_VERSION_INCREMENT);
}

::OcaLiteStatus OcaLiteGain::GetGainValue(::OcaDB& gain) const
{
    gain = m_gain;
    return OCASTATUS_OK;
}
