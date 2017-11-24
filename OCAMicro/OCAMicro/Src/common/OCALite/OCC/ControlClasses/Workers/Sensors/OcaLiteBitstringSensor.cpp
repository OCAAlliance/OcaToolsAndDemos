/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteBitstringSensor
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
#include "OcaLiteBitstringSensor.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_BITSTRING_SENSOR_CLASSID};
const ::OcaLiteClassID          OcaLiteBitstringSensor::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     static_cast< ::OcaClassVersionNumber>(0)

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteBitstringSensor::OcaLiteBitstringSensor(::OcaONo objectNumber,
                                           ::OcaBoolean lockable,
                                           const ::OcaLiteString& role,
                                           const ::OcaLiteList< ::OcaLitePort>& ports,
                                           ::OcaUint16 nrBits)
    : ::OcaLiteBasicSensor(objectNumber, lockable, role, ports),
      m_nrBits(nrBits)
{
}

::OcaLiteStatus OcaLiteBitstringSensor::GetNrBits(::OcaUint16& nrBits) const
{
    nrBits = m_nrBits;

    return OCASTATUS_OK;
}

::OcaLiteStatus OcaLiteBitstringSensor::GetBit(::OcaUint16 bitNr, ::OcaBoolean& value) const
{
    ::OcaLiteBitstring bitString;
    ::OcaLiteStatus rc(GetBitstringValue(bitString));
    assert(bitString.GetNrBits() == m_nrBits);
    if (OCASTATUS_OK == rc)
    {
        if (bitNr < bitString.GetNrBits())
        {
            value = static_cast< ::OcaBoolean>(bitString.GetBit(bitNr));
        }
        else
        {
            rc = OCASTATUS_PARAMETER_OUT_OF_RANGE;
        }
    }

    return rc;
}

::OcaLiteStatus OcaLiteBitstringSensor::GetBitstring(::OcaLiteBitstring& bitString) const
{
    ::OcaLiteStatus rc(GetBitstringValue(bitString));
    assert(bitString.GetNrBits() == m_nrBits);

    return rc;
}

::OcaLiteStatus OcaLiteBitstringSensor::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            case GET_NRBITS:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaUint16 nrBits;
                        rc = GetNrBits(nrBits);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     ::GetSizeValue< ::OcaUint16>(nrBits, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaUint16>(nrBits, &pResponse, writer);

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
            case GET_BIT:
                {
                    ::OcaUint8 numberOfParameters(0);
                    ::OcaUint16 bitNr;
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (1 == numberOfParameters) &&
                        reader.Read(bytesLeft, &pCmdParameters, bitNr))
                    {
                        ::OcaBoolean value;
                        rc = GetBit(bitNr, value);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + 
                                                     ::GetSizeValue< ::OcaBoolean>(value, writer));
                            responseBuffer = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                       ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                ::MarshalValue< ::OcaBoolean>(value, &pResponse, writer);

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
            case GET_BITSTRING:
                {
                    ::OcaUint8 numberOfParameters(0);
                    if (reader.Read(bytesLeft, &pCmdParameters, numberOfParameters) &&
                        (0 == numberOfParameters))
                    {
                        ::OcaLiteBitstring bitString;
                        rc = GetBitstring(bitString);
                        if (OCASTATUS_OK == rc)
                        {
                            ::OcaUint32 responseSize(::GetSizeValue< ::OcaUint8>(static_cast< ::OcaUint8>(1), writer) + bitString.GetSize(writer));
                            responseBuffer  = ::OcaLiteCommandHandler::GetInstance().GetResponseBuffer(responseSize);
                            if (NULL != responseBuffer)
                            {
                                ::OcaUint8* pResponse(responseBuffer);
                                writer.Write(static_cast< ::OcaUint8>(1/*NrParameters*/), &pResponse);
                                bitString.Marshal(&pResponse, writer);

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
            rc = OcaLiteBasicSensor::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

//lint -e{835} A zero has been given as right argument to operator '+'
::OcaClassVersionNumber OcaLiteBitstringSensor::GetClassVersion() const
{
    return (OcaLiteBasicSensor::GetClassVersion() + CLASS_VERSION_INCREMENT);
}

void OcaLiteBitstringSensor::BitstringChanged(const ::OcaLiteBitstring& bitString)
{
    assert(bitString.GetNrBits() == m_nrBits);

    ::OcaLitePropertyID propertyID(CLASS_ID.GetFieldCount(), static_cast< ::OcaUint16>(OCA_PROP_BITSTRING));
    ::OcaLitePropertyChangedEventData< ::OcaLiteBitstring> eventData(GetObjectNumber(),
                                                                     propertyID,
                                                                     bitString,
                                                                     OCAPROPERTYCHANGETYPE_CURRENT_CHANGED);
    PropertyChanged(eventData);
}
