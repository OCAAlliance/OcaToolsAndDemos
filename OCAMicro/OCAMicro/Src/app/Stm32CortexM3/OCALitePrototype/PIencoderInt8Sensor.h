/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : The PIencoderInt8Sensor class.
 *
 */

#ifndef PIENCODERINT8SENSOR_H
#define PIENCODERINT8SENSOR_H

// ---- Include system wide include files ----
#include <OCC/ControlClasses/Workers/Sensors/OcaLiteInt8Sensor.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ---
/**
 * Handles LEDs connected to GPIO pins.
 * @ingroup STM32Classes
 */
class PIencoderInt8Sensor : public ::OcaLiteInt8Sensor
{
public:
    /**
     * Constructor
     *
     * @param[in] objectNumber  The objectNumber.
     * @param[in] encoder       The encoder number (1 or 2).
     */
    PIencoderInt8Sensor(::OcaONo objectNumber, ::OcaInt8 minReading, ::OcaInt8 maxReading, ::OcaUint16 encoder);
        
    ::OcaLiteStatus SetEncoderValue(::OcaInt8 encoderValue);

    ::OcaLiteStatus GetEncoderLimits(::OcaInt8& minReading, ::OcaInt8& maxReading) const;

    void Run();

    /**
     * Destructor
     */
    virtual ~PIencoderInt8Sensor() {}

protected:

    virtual ::OcaLiteStatus GetReadingValue(::OcaInt8& reading) const;

    virtual ::OcaLiteStatus GetReadingStateValue(::OcaLiteSensorReadingState& state) const;

private:

    /** Private copy constructor to prevent copying. */
    PIencoderInt8Sensor(const ::PIencoderInt8Sensor&);
    /** Private assignment operator to prevent assignment. */
    ::PIencoderInt8Sensor& operator=(const ::PIencoderInt8Sensor& source);

        
    ::OcaUint16 m_encoder;  

    /** The encoder reading */
    ::OcaInt8 m_reading;

    ::OcaInt8 mm_Max;
    ::OcaInt8 mm_Min;
    bool m_bChanged;
        
    ::OcaLiteSensorReadingState m_state;           
};

#endif //PIENCODERINT8SENSOR_H
