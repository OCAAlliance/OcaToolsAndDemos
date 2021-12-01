/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : The PISwitchLedBitStringActuator class.
 *
 */

// ---- Include system wide include files ----

// ---- Include local include files ----
#include <app/Stm32CortexM3/OCALitePrototype/PISwitchLedBitStringActuator.h>
#include "main.h"
//#include "PILEDBitStringActuator.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

PISwitchLedBitStringActuator::PISwitchLedBitStringActuator(::OcaONo objectNumber, ::OcaUint16 length)
    : ::OcaLiteBitstringActuator(objectNumber,
                             static_cast< ::OcaBoolean>(true),
                             ::OcaLiteString("GPIO LED BitString Actuator"),
                             ::OcaLiteList< ::OcaLitePort>(),
                             length),
      m_bitstring(length)
{
    assert(static_cast< ::OcaUint16>(0) < length);
}

::OcaLiteStatus PISwitchLedBitStringActuator::SetBitstringValue(const ::OcaLiteBitstring& bitString)
{
    OcaUint8 i;
    OcaUint16 numBits;
    ::OcaLiteStatus rc(OCASTATUS_OK);

    m_bitstring = bitString;

    numBits = m_bitstring.GetNrBits();
    for (i = 0; i < numBits; i++)
    {
        if (m_bitstring.GetBit(i))
        {
            setSwitchLed(i+1, Led_On);
        }
        else
        {
            setSwitchLed(i+1, Led_Off);
        }
    }


    return rc;
}

