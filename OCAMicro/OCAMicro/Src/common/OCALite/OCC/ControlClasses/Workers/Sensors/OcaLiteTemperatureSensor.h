/*
*  By downloading or using this file, the user agrees to be bound by the terms of the license
*  agreement located in the LICENSE file in the root of this project
*  as an original contracting party.
*
*  Description         : OcaLiteTemperatureSensor
*
*/
#ifndef OCALITETEMPERATURESENSOR_H
#define OCALITETEMPERATURESENSOR_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteWorkerDataTypes.h>

// ---- Include local include files ----
#include "OcaLiteSensor.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

/**
 * The classID used for initialization.
 * @ingroup SensorWorkers
 */
#define OCA_TEMPERATURE_SENSOR_CLASSID      OCA_SENSOR_CLASSID,static_cast< ::OcaUint16>(5)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Basic temperature sensor.
 */
class OcaLiteTemperatureSensor : public ::OcaLiteSensor
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetReading() */
        GET_READING       = 1
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Temperature value (Celsius) */
        OCA_PROP_READING  = 1
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteTemperatureSensor() {}

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods  ----
    /**
     * Gets the Reading property
     *
     * @param[out] reading      The reading value
     * @param[out] minReading   Minimum reading value
     * @param[out] maxReading   Maximum reading value
     * @return Indicates whether the data was successfully retrieved.
     */
    ::OcaLiteStatus GetReading(::OcaTemperature& reading, ::OcaTemperature& minReading, ::OcaTemperature& maxReading) const;

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

protected:
    /**
     * Constructor
     *
     * @param[in]   objectNumber        Object number of this instance.
     * @param[in]   lockable            Indicates whether or not the object
     *                                  is lockable.
     * @param[in]   role                The role of this instance.
     * @param[in]   ports               The OCA input and output ports.
     * @param[in]   minReading          Lower limit of the reading
     * @param[in]   maxReading          Upper limit of the reading
     */
    OcaLiteTemperatureSensor(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports, ::OcaTemperature minReading, ::OcaTemperature maxReading);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    /**
     * Gets the value of the reading property. Must be implemented by derived classes
     * that actually implement the OcaTemperatureSensor.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[out] reading      The value of the reading property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetReadingValue(::OcaTemperature& reading) const = 0;

    /**
     * Event that is emitted when the reading has changed.
     * @note This method takes the object's mutex itself.
     *       The mutex should not be taken before this method is called.
     *
     * @param[in]   reading     The value of the reading property
     */
    void ReadingChanged(::OcaTemperature reading);

private:
    /** Lower limit of the reading property */
    ::OcaTemperature                        m_minReading;
    /** Upper limit of the reading property */
    ::OcaTemperature                        m_maxReading;

    /** private copy constructor, no copying of object allowed */
    OcaLiteTemperatureSensor(const ::OcaLiteTemperatureSensor&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteTemperatureSensor& operator=(const ::OcaLiteTemperatureSensor&);
};

#endif // OCALITETEMPERATURESENSOR_H
