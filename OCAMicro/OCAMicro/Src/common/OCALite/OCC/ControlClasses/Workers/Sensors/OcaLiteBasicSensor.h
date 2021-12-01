/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteBasicSensor
 *
 */
#ifndef OCALITEBASICSENSOR_H
#define OCALITEBASICSENSOR_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "OcaLiteSensor.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCA_BASIC_SENSOR_CLASSID      OCA_SENSOR_CLASSID,static_cast< ::OcaUint16>(1)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Abstract base class for weakly typed sensors.
 */
class OcaLiteBasicSensor : public ::OcaLiteSensor
{
public:
    /**
     * Destructor.
     */
    virtual ~OcaLiteBasicSensor() {}

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods  ----
    

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
    OcaLiteBasicSensor(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

private:
    /** private copy constructor, no copying of object allowed */
    OcaLiteBasicSensor(const ::OcaLiteBasicSensor&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteBasicSensor& operator=(const ::OcaLiteBasicSensor&);
};

#endif // OCALITEBASICSENSOR_H
