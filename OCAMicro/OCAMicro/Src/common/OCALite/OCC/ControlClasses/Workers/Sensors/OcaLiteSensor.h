/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteSensor
 *
 */
#ifndef OCALITESENSOR_H
#define OCALITESENSOR_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "../OcaLiteWorker.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

/**
 * Enumeration that describes whether an OcaSensor's current reading value can be trusted, and if not, why not.
 */
enum OcaLiteSensorReadingState    /* maps onto OcaUint8 */
{
    /** Cannot determine validity of reading, or this feature is not implemented. */
    OCALITESENSORREADINGSTATE_UNKNOWN       = 0,
    /** Sensor reading is valid. */
    OCALITESENSORREADINGSTATE_VALID         = 1,
    /** Sensor is underrange, reading is not valid. */
    OCALITESENSORREADINGSTATE_UNDERRANGE    = 2,
    /** Sensor is overrange, reading is not valid. */
    OCALITESENSORREADINGSTATE_OVERRANGE     = 3,
    /** Sensor is in an error state, reading is not valid. */
    OCALITESENSORREADINGSTATE_ERROR         = 4,
    /** Maximum value used for range checking */
    OCALITESENSORREADINGSTATE_MAXIMUM
};
/**
 * The classID used for initialization.
 */
#define OCA_SENSOR_CLASSID      OCA_WORKER_CLASSID,static_cast< ::OcaUint16>(2)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Abstract base class for sensor classes.
 */
class OcaLiteSensor : public ::OcaLiteWorker
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetReadingState() */
        GET_READING_STATE       = 1
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Describes whether reading value is valid */
        OCA_PROP_READING_STATE  = 1
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteSensor() {}

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods  ----
    /**
     * Gets the current reading state of the sensor.
     *
     * @param[out] state    The current reading state of the sensor
     * @return Indicates whether state was successfully retrieved.
     */
    ::OcaLiteStatus GetReadingState(::OcaLiteSensorReadingState& state) const;

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

protected:
    /**
     * Constructor
     *
     * @param[in]   objectNumber        Object number of this instance.
     * @param[in]   lockable            Indicates whether or not the object
     *                                  is lockable.
     * @param[in]   role                Read-only text property that describes object's role
     *                                  in the device. Particularly useful for workers, e.g. "Input 1 Gain".
     * @param[in]   ports               The list of (input and output) OCA ports the worker object
     *                                  has. Note that a worker object can have no ports, in which case
     *                                  the list is empty.
     */
    OcaLiteSensor(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;
    
    /**
     * Gets the current reading state of the sensor. Must be implemented by derived classes
     * that actually implement the worker.
     *
     * @param[out] state    The current reading state of the sensor
     * @return Indicates whether state was successfully retrieved.
     */
    virtual ::OcaLiteStatus GetReadingStateValue(::OcaLiteSensorReadingState& state) const = 0;

    /**
     * Event that is emitted when the reading state has changed.
     *
     * @param[in]  state    The current reading state of the sensor
     */
    void ReadingStateChanged(::OcaLiteSensorReadingState state);

private:
    /** private copy constructor, no copying of object allowed */
    OcaLiteSensor(const ::OcaLiteSensor&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteSensor& operator=(const ::OcaLiteSensor&);
};

template <>
void MarshalValue< ::OcaLiteSensorReadingState>(const ::OcaLiteSensorReadingState& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::OcaLiteSensorReadingState>(::OcaLiteSensorReadingState& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
::OcaUint32 GetSizeValue< ::OcaLiteSensorReadingState>(const ::OcaLiteSensorReadingState& value, const ::IOcaLiteWriter& writer);

#endif // OCALITESENSOR_H
