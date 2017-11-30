/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteAgent
 *
 */
// ---- Include system wide include files ----
#include <assert.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteAgent.h"

// ---- Referenced classes and types ---

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_AGENT_CLASSID};
const ::OcaLiteClassID          OcaLiteAgent::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     static_cast< ::OcaClassVersionNumber>(0)

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteAgent::OcaLiteAgent(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role)
    : ::OcaLiteRoot(objectNumber, lockable, role)
{
    assert(objectNumber >= OCA_MINIMUM_DEVICE_OBJECT_ONO);
}

OcaLiteAgent::~OcaLiteAgent()
{
    ::OcaLiteBlock::GetRootBlock().RemoveObject(GetObjectNumber());
}

::OcaLiteStatus OcaLiteAgent::Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                      ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response)
{
    ::OcaLiteStatus rc(OCASTATUS_PARAMETER_ERROR);
    if (!IsLocked(sessionID))
    {
        if (methodID.GetDefLevel() == CLASS_ID.GetFieldCount())
        {
            switch (methodID.GetMethodIndex())
            {
            case GET_LABEL:
            case SET_LABEL:
            case GET_OWNER:
            case GET_PATH:
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
::OcaClassVersionNumber OcaLiteAgent::GetClassVersion() const
{
    return (OcaLiteRoot::GetClassVersion() + CLASS_VERSION_INCREMENT);
}
