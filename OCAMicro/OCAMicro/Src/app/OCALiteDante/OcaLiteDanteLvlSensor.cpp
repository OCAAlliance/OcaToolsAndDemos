/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OCALiteDanteLvlSensor
 *
 */

#include "OcaLiteDanteLvlSensor.h"
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>

OcaLiteStatus OCALiteDanteLvlSensor::Initialize()
{        
    // Make sure the Gain is initialized to the default value, if succeeded add ourself to the root block        
    OcaLiteStatus status(OCASTATUS_OK);        
    if (!OcaLiteBlock::GetRootBlock().AddObject(*this)) {            
        OCA_LOG_ERROR("Adding OCALiteDanteLvlSensor object to the root block failed");            
        status = OCASTATUS_PROCESSING_FAILED;        
    }
    return status;    
}

void OCALiteDanteLvlSensor::Terminate()
{
    // De-register with root block
    OcaLiteBlock::GetRootBlock().RemoveObject(GetObjectNumber());
}

/**
* Gets the value of the reading property. Must be implemented by derived classes
* that actually implement the OcaLevelSensor.
* @note This method does not take the object's mutex itself.
*       The mutex should be taken before this method is called.
*
* @param[out] reading      The value of the reading property
* @return Indicates whether the operation succeeded.
*/
OcaLiteStatus OCALiteDanteLvlSensor::GetReadingValue(::OcaDB& reading) const
{
    OcaLiteStatus status(OCASTATUS_OK);
    reading = m_Level;
    return status;    
}

/**
* Gets the current reading state of the sensor. Must be implemented by derived classes
* that actually implement the worker.
* @note This method does not take the object's mutex itself.
*       The mutex should be taken before this method is called.
*
* @param[out] state    The current reading state of the sensor
* @return Indicates whether state was successfully retrieved.
*/
OcaLiteStatus OCALiteDanteLvlSensor::GetReadingStateValue(::OcaLiteSensorReadingState& state) const
{
    state = OCALITESENSORREADINGSTATE_VALID;
    return OCASTATUS_OK;
}

OcaLiteStatus OCALiteDanteLvlSensor::updateMeter(OcaDB value)
{
    OcaLiteStatus status(OCASTATUS_OK);

    // flag a change
    if (!CompareValue< ::OcaDB>(value, m_Level))
    {
        m_Level = value;
        m_bChanged = static_cast< ::OcaBoolean>(true);
    }
    return status;    
}

// update a meter, converting from Audinate conmon_metering_message_peak_t to OcaDB
OcaLiteStatus OCALiteDanteLvlSensor::updateMeter(OcaUint8 value)
{
    OcaLiteStatus status(OCASTATUS_OK);
    OcaDB min, max, current;

    GetReading(current, min, max);
    if(!value) {
        // clipping
        m_Level = max;
    } else if(value >= 0xfe) {
        m_Level = min;
    } else {
        // 1..253 -> 0..252 -> 0..126 -> 0..-126
        m_Level = 0 - ((value-1)/2.0F);
    }

    // flag a change
    if (!CompareValue< ::OcaDB>(current, m_Level))
    {
        m_bChanged = static_cast< ::OcaBoolean>(true);
    }
    return status;    
}

void OCALiteDanteLvlSensor::Run()
{
    if (m_bChanged)
    {
        ReadingChanged(m_Level);

        m_bChanged = static_cast< ::OcaBoolean>(false);
    }
}
