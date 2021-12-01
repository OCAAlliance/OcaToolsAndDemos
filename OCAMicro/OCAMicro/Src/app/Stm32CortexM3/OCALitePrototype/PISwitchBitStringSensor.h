/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The PISwitchBitStringSensor class.
 *
 *
 */

#ifndef PISWITCHBITSTRINGSENSOR_H
#define PISWITCHBITSTRINGSENSOR_H

// ---- Include system wide include files ----
#include <OCC/ControlClasses/Workers/Sensors/OcaLiteBitstringSensor.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ---
/**
 * Handles LEDs connected to GPIO pins.
 * @ingroup STM32Classes
 */
class PISwitchBitStringSensor : public ::OcaLiteBitstringSensor
{
public:
    /**
     * Constructor
     *
     * @param[in] objectNumber  The objectNumber.
     * @param[in] length        The number of Switches.
     */
    PISwitchBitStringSensor(::OcaONo objectNumber, ::OcaUint16 length);
        
    ::OcaLiteStatus SetSwitchValue(::OcaUint16 switchNr, bool value);

    void Run();

    /**
     * Destructor
     */
    virtual ~PISwitchBitStringSensor() {}

protected:

    virtual ::OcaLiteStatus GetBitstringValue(::OcaLiteBitstring& bitString) const;

    virtual ::OcaLiteStatus GetReadingStateValue(::OcaLiteSensorReadingState& state) const;

private:

    /** Private copy constructor to prevent copying. */
    PISwitchBitStringSensor(const ::PISwitchBitStringSensor&);
    /** Private assignment operator to prevent assignment. */
    ::PISwitchBitStringSensor& operator=(const ::PISwitchBitStringSensor& source);

    /** The bitstring */
    ::OcaLiteBitstring m_bitstring;
    bool m_bChanged;
};

#endif //PISWITCHBITSTRINGSENSOR_H
