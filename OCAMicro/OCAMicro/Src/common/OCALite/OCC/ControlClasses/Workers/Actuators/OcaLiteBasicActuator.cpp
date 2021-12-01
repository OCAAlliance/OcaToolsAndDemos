/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteBasicActuator
 *
 */
// ---- Include system wide include files ----
#include <assert.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteBasicActuator.h"

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_BASIC_ACTUATOR_CLASSID};
const ::OcaLiteClassID          OcaLiteBasicActuator::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     0

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteBasicActuator::OcaLiteBasicActuator(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports)
    : ::OcaLiteActuator(objectNumber, lockable, role, ports)
{
}

::OcaLiteStatus OcaLiteBasicActuator::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
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
            rc = OcaLiteActuator::Execute(reader, writer, sessionID, methodID, parametersSize, parameters, response);
        }
    }
    else
    {
        rc = OCASTATUS_LOCKED;
    }

    return rc;
}

::OcaClassVersionNumber OcaLiteBasicActuator::GetClassVersion() const
{
    return static_cast< ::OcaClassVersionNumber>(static_cast<int>(OcaLiteActuator::GetClassVersion()) + CLASS_VERSION_INCREMENT);
}

OcaLiteBasicActuator::~OcaLiteBasicActuator()
{
    ::OcaLiteBlock::GetRootBlock().RemoveObject(GetObjectNumber());
}
