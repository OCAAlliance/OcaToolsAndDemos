/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : The PIencoderInt8Sensor class.
 *
 */

// ---- Include system wide include files ----

// ---- Include local include files ----
#include <app/Stm32CortexM3/OCALitePrototype/PIencoderInt8Sensor.h>
#include "main.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

PIencoderInt8Sensor::PIencoderInt8Sensor(::OcaONo objectNumber, ::OcaInt8 minReading, ::OcaInt8 maxReading, ::OcaUint16 encoder)
    : ::OcaLiteInt8Sensor(objectNumber,
                             static_cast< ::OcaBoolean>(true),
                             ::OcaLiteString("GPIO Encoder Sensor"),
                             ::OcaLiteList< ::OcaLitePort>(),
                             minReading, maxReading),
      m_encoder(encoder),
      m_reading(0),
      m_bChanged(false)

{
    m_state = OCALITESENSORREADINGSTATE_UNKNOWN;
    assert(((encoder >= 1) && (encoder <= 2)));
}

::OcaLiteStatus PIencoderInt8Sensor::GetReadingValue(::OcaInt8& reading) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    reading = m_reading;
    
    return rc;
}


void PIencoderInt8Sensor::Run()
{
    if (m_bChanged)
    {
        m_bChanged = false;

        ReadingChanged(m_reading);
    }
}


::OcaLiteStatus PIencoderInt8Sensor::GetReadingStateValue(::OcaLiteSensorReadingState& state) const
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    state = m_state;
    
    return rc;
}

::OcaLiteStatus PIencoderInt8Sensor::SetEncoderValue(::OcaInt8 encoderValue)
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    m_state = OCALITESENSORREADINGSTATE_VALID;
    
    m_reading = encoderValue;

    m_bChanged = true;

    return rc;
}

::OcaLiteStatus PIencoderInt8Sensor::GetEncoderLimits(::OcaInt8& minReading, ::OcaInt8& maxReading) const
{
    return ::OcaLiteStatus(GetLimits(minReading, maxReading));
}
