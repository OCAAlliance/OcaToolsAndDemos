/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The PISwitchLedBitStringActuator class.
 *
 *
 */

#ifndef PISWITCHLEDBITSTRINGACTUATOR_H
#define PISWITCHLEDBITSTRINGACTUATOR_H

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
class PISwitchLedBitStringActuator : public ::OcaLiteBitstringActuator
{
public:
    /**
     * Constructor
     *
     * @param[in] objectNumber  The objectNumber.
     * @param[in] length        The number of LEDs.
     */
    PISwitchLedBitStringActuator(::OcaONo objectNumber, ::OcaUint16 length);

    /**
     * Destructor
     */
    virtual ~PISwitchLedBitStringActuator() {}

protected:

    virtual ::OcaLiteStatus SetBitstringValue(const ::OcaLiteBitstring& bitString);

    virtual ::OcaLiteStatus GetBitstringValue(::OcaLiteBitstring& bitString) const
    {
        bitString = m_bitstring;

        return OCASTATUS_OK;
    }

private:

    /** Private copy constructor to prevent copying. */
    PISwitchLedBitStringActuator(const ::PISwitchLedBitStringActuator&);
    /** Private assignment operator to prevent assignment. */
    ::PISwitchLedBitStringActuator& operator=(const ::PISwitchLedBitStringActuator& source);

    /** The bitstring */
    ::OcaLiteBitstring m_bitstring;
};

#endif //PISWITCHLEDBITSTRINGACTUATOR_H
