/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteBasicActuator
 *
 */
#ifndef OCALITEBASICACTUATOR_H
#define OCALITEBASICACTUATOR_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "OCC/ControlClasses/Workers/Actuators/OcaLiteActuator.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 * @ingroup Workers
 */
#define OCA_BASIC_ACTUATOR_CLASSID      OCA_ACTUATOR_CLASSID,static_cast< ::OcaUint16>(1)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Abstract base class for weakly typed actuators.
 * @ingroup ActuatorWorkers
 */
class OcaLiteBasicActuator : public ::OcaLiteActuator
{
public:
    /**
     * Destructor.
     */
    virtual ~OcaLiteBasicActuator();

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    //lint -e(1516) Hides inherited member
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
    OcaLiteBasicActuator(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

private:
    /** private copy constructor, no copying of object allowed */
    OcaLiteBasicActuator(const ::OcaLiteBasicActuator&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteBasicActuator& operator=(const ::OcaLiteBasicActuator&);
};

#endif // OCALITEBASICACTUATOR_H
