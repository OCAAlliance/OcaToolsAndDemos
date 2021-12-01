/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The PILEDBitStringActuator class.
 *
 */

#ifndef PILEDBITSTRINGACTUATOR_H
#define PILEDBITSTRINGACTUATOR_H

// ---- Include system wide include files ----
#include <OCC/ControlClasses/Workers/Actuators/OcaLiteBitstringActuator.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ---
/**
 * Handles LEDs connected to GPIO pins.
 * @ingroup STM32Classes
 */
class PILEDBitStringActuator : public ::OcaLiteBitstringActuator
{
public:
    /**
     * Constructor
     *
     * @param[in] objectNumber  The objectNumber.
     * @param[in] length        The number of LEDs.
     */
    PILEDBitStringActuator(::OcaONo objectNumber, ::OcaUint16 length, ::OcaUint16 channel);

    /**
     * Destructor
     */
    virtual ~PILEDBitStringActuator() {}

protected:

    virtual ::OcaLiteStatus SetBitstringValue(const ::OcaLiteBitstring& bitString);

    virtual ::OcaLiteStatus GetBitstringValue(::OcaLiteBitstring& bitString) const
    {
        bitString = m_bitstring;

        return OCASTATUS_OK;
    }

private:

    /** Private copy constructor to prevent copying. */
    PILEDBitStringActuator(const ::PILEDBitStringActuator&);
    /** Private assignment operator to prevent assignment. */
    ::PILEDBitStringActuator& operator=(const ::PILEDBitStringActuator& source);

    /** The bitstring */
    ::OcaLiteBitstring m_bitstring;
    // the channel    
    ::OcaUint16 m_channel;
};

#endif //OCADEMOLEDBITSTRINGACTUATOR_H
