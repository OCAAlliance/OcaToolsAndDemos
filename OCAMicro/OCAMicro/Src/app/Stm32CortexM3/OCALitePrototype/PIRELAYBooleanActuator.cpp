/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : The PIRelayBooleanActuator class.
 *
 */

// ---- Include system wide include files ----

// ---- Include local include files ----
#include <app/Stm32CortexM3/OCALitePrototype/PIRELAYBooleanActuator.h>
#include "main.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

PIRELAYBooleanActuator::PIRELAYBooleanActuator(::OcaONo objectNumber, ::OcaUint16 relay)
    : ::OcaLiteBooleanActuator(objectNumber,
                             static_cast< ::OcaBoolean>(true),
                             ::OcaLiteString("GPIO Relay Boolean Actuator"),
                             ::OcaLiteList< ::OcaLitePort>()),
                  m_relay(relay)
{
    assert(((relay >= 1) && (relay <= 2)));
}

::OcaLiteStatus PIRELAYBooleanActuator::SetSettingValue(::OcaBoolean setting)
{
    ::OcaLiteStatus rc(OCASTATUS_OK);

    m_setting = setting;
    
    if (1 == this->m_relay)
    {
        if (m_setting == true)
        {
            RELAY1_on();
        }
        else
        {
            RELAY1_off();            
        }
    }
    else if (2 == this->m_relay)
    {
        if (m_setting == true)
        {
            RELAY2_on();
        }
        else
        {
            RELAY2_off();            
        }
    }

    return rc;
}

