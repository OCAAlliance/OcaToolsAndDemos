/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteUInt32Sensor
 *
 */
#ifndef OCALITEUINT32SENSOR_H
#define OCALITEUINT32SENSOR_H

// ---- Include system wide include files ----

// ---- Include local include files ----

#include "OcaLiteBasicSensor.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCA_UINT32_SENSOR_CLASSID      OCA_BASIC_SENSOR_CLASSID,static_cast< ::OcaUint16>(8)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Basic Uint32 sensor.
 */
class OcaLiteUint32Sensor : public ::OcaLiteBasicSensor
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetReading() */
        GET_READING         = 1
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Int8 reading */
        OCA_PROP_READING    = 1
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteUint32Sensor() {}

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
    ::OcaLiteStatus GetReading(::OcaUint32& reading, ::OcaUint32& minReading, ::OcaUint32& maxReading) const;

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
    OcaLiteUint32Sensor(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports, ::OcaUint32 minReading, ::OcaUint32 maxReading);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    /**
     * Gets the value of the reading property. Must be implemented by derived classes
     * that actually implement the OcaInt8Sensor.
     *
     * @param[out] reading      The value of the reading property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetReadingValue(::OcaUint32& reading) const = 0;


    /**
     * Gets the value of the limit properties.
     *
     * @param[out]   minReading          Lower limit of the sensor
     * @param[out]   maxReading          Upper limit of the sensor
     * @return Indicates whether the operation succeeded.
     */
    ::OcaLiteStatus GetLimits(::OcaUint32& minReading, ::OcaUint32& maxReading) const;

    /**
     * Event that is emitted when the reading has changed.
     *
     * @param[in]   reading     The value of the reading property
     */
    void ReadingChanged(::OcaUint32 reading);

private:
    /** Lower limit of the reading property */
    ::OcaUint32                               m_minReading;
    /** Upper limit of the reading property */
    ::OcaUint32                               m_maxReading;

    /** private copy constructor, no copying of object allowed */
    OcaLiteUint32Sensor(const ::OcaLiteUint32Sensor&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteUint32Sensor& operator=(const ::OcaLiteUint32Sensor&);
};

#endif // OCALITEUINT32SENSOR_H
