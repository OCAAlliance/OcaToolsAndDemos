/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteBasicSensor
 *
 */
// ---- Include system wide include files ----
#include <assert.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>


// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteBasicSensor.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_BASIC_SENSOR_CLASSID};
const ::OcaLiteClassID          OcaLiteBasicSensor::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     static_cast< ::OcaClassVersionNumber>(0)

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteBasicSensor::OcaLiteBasicSensor(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports)
    : ::OcaLiteSensor(objectNumber, lockable, role, ports)
{
}

::OcaLiteStatus OcaLiteBasicSensor::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                              ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response)
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_ERROR);
    if (!IsLocked(sessionID))
    {
        if (methodID.GetDefLevel() == CLASS_ID.GetFieldCount())
        {
            rc = OCASTATUS_BAD_METHOD;
        }
        else
        {
            // Should be executed on higher level
            rc = OcaLiteSensor::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

//lint -e{835} A zero has been given as right argument to operator '+'
::OcaClassVersionNumber OcaLiteBasicSensor::GetClassVersion() const
{
    return (OcaLiteSensor::GetClassVersion() + CLASS_VERSION_INCREMENT);
}
