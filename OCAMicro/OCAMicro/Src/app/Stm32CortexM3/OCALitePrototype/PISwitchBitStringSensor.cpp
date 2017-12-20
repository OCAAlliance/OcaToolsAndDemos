/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The PISwitchBitStringSensor class.
 *
 */

// ---- Include system wide include files ----

// ---- Include local include files ----
#include <app/Stm32CortexM3/OCALitePrototype/PISwitchBitStringSensor.h>
#include "main.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

PISwitchBitStringSensor::PISwitchBitStringSensor(::OcaONo objectNumber, ::OcaUint16 length)
    : ::OcaLiteBitstringSensor(objectNumber,
                             static_cast< ::OcaBoolean>(true),
                             ::OcaLiteString("GPIO Switch BitString Sensor"),
                             ::OcaLiteList< ::OcaLitePort>(),
                             length),
      m_bitstring(length),
      m_bChanged(false)
{
    assert(static_cast< ::OcaUint16>(0) < length);
}

::OcaLiteStatus PISwitchBitStringSensor::GetBitstringValue(::OcaLiteBitstring& bitString) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    bitString = m_bitstring;
    
    return rc;
}

void PISwitchBitStringSensor::Run()
{
    if (m_bChanged)
    {
        m_bChanged = false;

        BitstringChanged(m_bitstring);
    }
}
	
::OcaLiteStatus PISwitchBitStringSensor::GetReadingStateValue(::OcaLiteSensorReadingState& state) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    state = OCALITESENSORREADINGSTATE_VALID;
    
    return rc;
    
}    

::OcaLiteStatus PISwitchBitStringSensor::SetSwitchValue(::OcaUint16 switchNr, bool value)
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    m_bitstring.SetBit(switchNr, value);
    m_bChanged = true;

    return rc;
}
