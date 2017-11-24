/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteLevelSensor
 *
 */
#ifndef OCALITELEVELSENSOR_H
#define OCALITELEVELSENSOR_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include <OCC/ControlDataTypes/OcaLiteWorkerDataTypes.h>
#include "OcaLiteSensor.h"

// ---- Referenced classes and types ----
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 * @ingroup ActuatorWorkers
 */
#define OCA_LEVEL_SENSOR_CLASSID      OCA_SENSOR_CLASSID,static_cast< ::OcaUint16>(2)

// ---- Helper functions ----

// ---- Class Definition ----
class OcaLiteLevelSensor : public ::OcaLiteSensor
{
public:
    /** Method indexes for the supported methods. */
    //lint -e(578) Hides inherited symbol
    enum MethodIndex
    {
        /** GetReading() */
        GET_READING       = 1
    };

    /** Property indexes for the supported properties. */
    //lint -e(578) Hides inherited symbol
    enum PropertyIndex
    {
        /** dB reading */
        OCA_PROP_READING  = 1
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteLevelSensor() {}

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    //lint -e(1516) Hides inherited member
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
    ::OcaLiteStatus GetReading(::OcaDB& reading, ::OcaDB& minReading, ::OcaDB& maxReading) const;

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
     * @param[in]   minRefLevel         Minimum reference level.
     * @param[in]   maxRefLevel         Maximum reference level.
     */
    OcaLiteLevelSensor(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports, ::OcaDB minRefLevel, ::OcaDB maxRefLevel);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    /**
     * Gets the value of the reading property. Must be implemented by derived classes
     * that actually implement the OcaLiteLevelSensor.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[out] reading      The value of the reading property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetReadingValue(::OcaDB& reading) const = 0;

    /**
     * Event that is emitted when the reading has changed.
     * @note This method takes the object's mutex itself.
     *       The mutex should not be taken before this method is called.
     *
     * @param[in]   reading     The value of the reading property
     */
    void ReadingChanged(::OcaDB reading);

private:
    /** Lower limit of the reading property */
    ::OcaDB                                 m_minReading;
    /** Upper limit of the reading property */
    ::OcaDB                                 m_maxReading;

    /** private copy constructor, no copying of object allowed */
    OcaLiteLevelSensor(const ::OcaLiteLevelSensor&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteLevelSensor& operator=(const ::OcaLiteLevelSensor&);
};

#endif // OCALITELEVELSENSOR_H
