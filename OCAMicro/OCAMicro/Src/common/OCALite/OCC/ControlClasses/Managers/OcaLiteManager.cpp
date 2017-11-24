/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLiteManager
 *
 */

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteDeviceManager.h"
#include "OcaLiteManager.h"

// ---- Referenced classes and types ---

// ---- Helper types and constants ----

static const ::OcaUint16        classID[]   = {OCA_MANAGER_CLASSID};
const ::OcaLiteClassID          OcaLiteManager::CLASS_ID(static_cast< ::OcaUint16>(sizeof(classID) / sizeof(classID[0])), classID);

/** Defines the version increment of this class compared to its base class. */
#define CLASS_VERSION_INCREMENT     static_cast< ::OcaClassVersionNumber>(0)

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

OcaLiteManager::OcaLiteManager(::OcaONo objectNumber, const ::OcaLiteString& role, const ::OcaLiteString& name)
    : ::OcaLiteRoot(objectNumber, static_cast< ::OcaBoolean>(true), role),  // All managers are lockable
      m_name(name)
{
    assert((objectNumber != OCA_ROOT_BLOCK_ONO) && (objectNumber < OCA_MINIMUM_DEVICE_OBJECT_ONO));
}


const ::OcaLiteString& OcaLiteManager::GetName() const
{
    return m_name;
}

::OcaBoolean OcaLiteManager::Initialize()
{
    ::OcaBoolean bSuccess(::OcaLiteDeviceManager::GetInstance().AddManager(*this));

    if (!bSuccess)
    {
        OCA_LOG_ERROR_PARAMS("Failed to add the manager (ono %u) to the managers list.", GetObjectNumber());
    }

    return bSuccess;
}

::OcaBoolean OcaLiteManager::Shutdown()
{
    ::OcaLiteDeviceManager::GetInstance().RemoveManager(*this);

    return static_cast< ::OcaBoolean>(true);
}

//lint -e{835} A zero has been given as right argument to operator '+'
::OcaClassVersionNumber OcaLiteManager::GetClassVersion() const
{
    return (OcaLiteRoot::GetClassVersion() + CLASS_VERSION_INCREMENT);
}
