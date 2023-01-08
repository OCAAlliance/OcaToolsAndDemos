/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : DanteLiteOcaMediaClock
 *
 */

#include "OcaLiteDanteMediaClock.h"
#include "OCC/ControlClasses/Managers/OcaLiteMediaClockManager.h"
#include "OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h"
#include "OCC/ControlClasses/OcaLiteRoot.h"
#include "DanteLiteHostInterface.h"

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

DanteLiteOcaMediaClock::DanteLiteOcaMediaClock(::OcaONo objectNumber,
                                                 ::OcaBoolean lockable,
                                                 const ::OcaLiteString& role,
                                                 ::OcaLiteMediaClockType type,
                                                 ::OcaUint16 domainID,
                                                 const ::OcaLiteList< ::OcaLiteMediaClockRate>& ratesSupported,
                                                 const OcaLiteMediaClockRate& currentRate)
    : ::OcaLiteMediaClock(objectNumber, lockable, role, type, domainID, ratesSupported)
    , m_Rate(currentRate)
    , m_ClockState(OCAMEDIACLOCKLOCKSTATE_LOCKED)
    , m_DomainID(domainID)
    , m_ClockType(type)
{
}

DanteLiteOcaMediaClock::~DanteLiteOcaMediaClock()
{
}

::OcaLiteStatus DanteLiteOcaMediaClock::Initialize()
{
    OcaLiteStatus rc(OCASTATUS_OK);
    if (!OcaLiteBlock::GetRootBlock().AddObject(*this)) {            
        OCA_LOG_ERROR("Adding DanteLiteOcaMediaClock object to the root block failed");            
        rc = OCASTATUS_PROCESSING_FAILED;        
    }
    // Set our object number for the Dante Interface
    DanteLiteHostInterfaceSetMediaClockObjectNumber((uint32_t (GetObjectNumber())));
    return rc;
}

::OcaLiteStatus DanteLiteOcaMediaClock::GetRateValue(::OcaLiteMediaClockRate& rate) const
{
    OcaLiteStatus rc(OCASTATUS_OK);
    rate = m_Rate;
    return rc;
}

::OcaLiteStatus DanteLiteOcaMediaClock::SetRateValue(const ::OcaLiteMediaClockRate& rate)
{
    OcaLiteStatus rc(OCASTATUS_OK);
    m_Rate = rate;
    // set the rate on the actual device
    OcaFloat32 nominal = rate.GetNominalRate();
    OcaUint32 danteRate = static_cast< ::OcaUint32>(nominal);
    // this function will only update the device if it's a new rate (i.e. the change has come from the OCA device)
    DanteLiteHostInterfaceSetSampleRate(danteRate);
    return rc;
}

::OcaLiteStatus DanteLiteOcaMediaClock::GetLockStateValue(::OcaLiteMediaClockLockState& state) const
{
    OcaLiteStatus rc(OCASTATUS_OK);
    state = m_ClockState;
    return rc;
}

::OcaLiteStatus DanteLiteOcaMediaClock::SetDomainIDValue(::OcaUint16 id)
{
    OcaLiteStatus rc(OCASTATUS_OK);
    m_DomainID = id;
    return rc;
}

::OcaLiteStatus DanteLiteOcaMediaClock::GetDomainIDValue(::OcaUint16& id) const
{
    OcaLiteStatus rc(OCASTATUS_OK);
    id = m_DomainID;
    return rc;
}

::OcaLiteStatus DanteLiteOcaMediaClock::SetTypeValue(::OcaLiteMediaClockType type)
{
    OcaLiteStatus rc(OCASTATUS_OK);
    m_ClockType = type;
    return rc;
}

// update the clock object from a dante network change
bool DanteLiteOcaMediaClock::setNewDanteSampleRate(OcaUint32 newRate)
{
    ::OcaLiteList< ::OcaLiteMediaClockRate> ratesSupported;
    OcaFloat32 danteRate = static_cast< ::OcaFloat32>(newRate);
    // get our supported rates
    GetRatesSupported(ratesSupported);
    int numberofRates = ratesSupported.GetCount();
    for(int i = 0; i < ratesSupported.GetCount(); i++) {
        OcaLiteMediaClockRate rate = ratesSupported.GetItem(i);
        OcaFloat32 nominal = rate.GetNominalRate();
        if((danteRate == nominal) && (m_Rate != rate)) {
            SetRate(rate);
            continue;
        }
    }
    return false;
}
