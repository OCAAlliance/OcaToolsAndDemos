/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : The PILEDBitstringActuator class.
 *
 */

// ---- Include system wide include files ----

// ---- Include local include files ----
#include <app/Stm32CortexM3/OCALitePrototype/PILEDBitStringActuator.h>
#include "main.h"
//#include "PILEDBitStringActuator.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

PILEDBitStringActuator::PILEDBitStringActuator(::OcaONo objectNumber, ::OcaUint16 length, ::OcaUint16 channel)
    : ::OcaLiteBitstringActuator(objectNumber,
                             static_cast< ::OcaBoolean>(true),
                             ::OcaLiteString("GPIO LED BitString Actuator"),
                             ::OcaLiteList< ::OcaLitePort>(),
                             length),
      m_bitstring(length),
      m_channel(channel)
{
    assert(static_cast< ::OcaUint16>(0) < length);
    assert(((channel >= 1) && (channel <= 8)));
}

::OcaLiteStatus PILEDBitStringActuator::SetBitstringValue(const ::OcaLiteBitstring& bitString)
{
    OcaUint8 i;
    OcaUint8  temp = 0;
    OcaUint8  mask = 1;
    OcaUint16 numBits;
//    ::OcaLiteStatus rc(OCASTATUS_INVALID_REQUEST);
    ::OcaLiteStatus rc(OCASTATUS_OK);

    m_bitstring = bitString;

    numBits = m_bitstring.GetNrBits();
    for (i = 0; i < numBits; i++)
    {
        if (m_bitstring.GetBit(i))
        {
            temp |= mask;
        }
        mask <<= 1;
    }
    setDbRawLED(this->m_channel, temp);

#if 0    
    meterDbSetting_t meterSetting = Meter_Off;
    m_bitstring = bitString;
    
    if (bitString.GetBit(0))
    {
        meterSetting = Meter_0db;
    }
    else if (bitString.GetBit(1))
    {
        meterSetting = Meter_n3db;
    }
    else if (bitString.GetBit(2))
    {
        meterSetting = Meter_n6db;
    }
    else if (bitString.GetBit(3))
    {
        meterSetting = Meter_n12db;
    }
    else if (bitString.GetBit(4))
    {
        meterSetting = Meter_n18db;
    }
    else if (bitString.GetBit(5))
    {
        meterSetting = Meter_n42db;
    }
    
    setDbMeter(this->m_channel, meterSetting);
#endif
	// Implement the LED handling
// OCASTATUS_PARAMETER_OUT_OF_RANGE
    return rc;
}

