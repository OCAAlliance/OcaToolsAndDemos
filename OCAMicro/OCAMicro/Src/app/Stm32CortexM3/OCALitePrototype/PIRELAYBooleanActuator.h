/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The PIRelayBooleanActuator class.
 *
 *
 */

#ifndef PIRELAYBOOLEANACTUATOR_H
#define PIRELAYBOOLEANACTUATOR_H

// ---- Include system wide include files ----
#include <OCC/ControlClasses/Workers/Actuators/OcaLiteBooleanActuator.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ---
/**
 * Handles Relays connected to GPIO pins.
 * @ingroup STM32Classes
 */
class PIRELAYBooleanActuator : public ::OcaLiteBooleanActuator
{
public:
    /**
     * Constructor
     *
     * @param[in] objectNumber  The objectNumber.
     * @param[in] relay        The relay being controlled
     */
    PIRELAYBooleanActuator(::OcaONo objectNumber, ::OcaUint16 relay);

    /**
     * Destructor
     */
    virtual ~PIRELAYBooleanActuator() {}

protected:

    virtual ::OcaLiteStatus SetSettingValue(::OcaBoolean setting);

    virtual ::OcaLiteStatus GetSetting(::OcaBoolean& setting) const
    {
        setting = m_setting;

        return OCASTATUS_OK;
    }
    

private:

    /** Private copy constructor to prevent copying. */
    PIRELAYBooleanActuator(const ::PIRELAYBooleanActuator&);
    /** Private assignment operator to prevent assignment. */
    ::PIRELAYBooleanActuator& operator=(const ::PIRELAYBooleanActuator& source);

    /** The boolean value */
    ::OcaBoolean m_setting;
    // the channel    
    ::OcaUint16 m_relay;
};

#endif //PIRELAYBOOLEANACTUATOR_H
