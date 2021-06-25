/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteDelay
 *
 */
#ifndef OCALITEDELAY_H
#define OCALITEDELAY_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include <OCC/ControlDataTypes/OcaLiteWorkerDataTypes.h>
#include "OcaLiteActuator.h"

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
#define OCA_DELAY_CLASSID      OCA_ACTUATOR_CLASSID,static_cast< ::OcaUint16>(7)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Signal delay - basic version
 * @ingroup ActuatorWorkers
 */
class OcaLiteDelay : public ::OcaLiteActuator
{
public:
    /** Method indexes for the supported methods. */
    //lint -e(578) Hides inherited symbol
    enum MethodIndex
    {
        /** GetDelayTime() */
        GET_DELAY_TIME  = 1,
        /** SetDelayTime() */
        SET_DELAY_TIME  = 2
    };

    /** Property indexes for the supported properties. */
    //lint -e(578) Hides inherited symbol
    enum PropertyIndex
    {
        /** Delay in seconds */
        OCA_PROP_DELAY_TIME = 1
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteDelay() {}

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    //lint -e(1516) Hides inherited member
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods  ----
    /**
     * Gets the value of the DelayTime property.
     *
     * @param[out] time     Output parameter that holds the DelayTime property
     * @param[out] minTime  Output parameter that holds the minimum DelayTime property
     * @param[out] maxTime  Output parameter that holds the minimum DelayTime property
     * @return Indicates whether the data was successfully retrieved.
     */
    ::OcaLiteStatus GetDelayTime(::OcaTimeInterval& time, ::OcaTimeInterval& minTime, ::OcaTimeInterval& maxTime) const;

    /**
     * Sets the value of the DelayTime property.
     *
     * @param[in]  time    The value to which the DelayTime property must be set.
     * @return Indicates whether the property was successfully set.
     */
    ::OcaLiteStatus SetDelayTime(::OcaTimeInterval time);

    
    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
        ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

    
protected:
    /**
     * Constructor
     *
     * @param[in]  objectNumber     Object number of this instance.
     * @param[in]  lockable         Indicates whether or not the object is lockable.
     * @param[in]  role             The role of this instance.
     * @param[in]  ports            The OCA input and output ports.
     * @param[in]  minDelayTime     Minimum delay time in seconds
     * @param[in]  maxDelayTime     Maximum delay time in seconds
     */
    OcaLiteDelay(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports, ::OcaTimeInterval minDelayTime, ::OcaTimeInterval maxDelayTime);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    /**
     * Gets the value of the DelayTime property. Can be overridden by derived classes
     * that actually implement this class.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[out] time  The value of the DelayTime property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetDelayTimeValue(::OcaTimeInterval& time) const;

    /**
     * Set the value of the DelayTime property. Must be implemented by derived classes
     * that actually implement the worker.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[in]  time    The value to which the DelayTime property must be set
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetDelayTimeValue(::OcaTimeInterval time) = 0;

private:
    /** Delay in seconds */
    ::OcaTimeInterval                       m_delayTime;

    /** Minimum delay in seconds */
    ::OcaTimeInterval                       m_minDelayTime;

    /** Maximum delay in seconds */
    ::OcaTimeInterval                       m_maxDelayTime;

    /** private copy constructor, no copying of object allowed */
    OcaLiteDelay(const ::OcaLiteDelay&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteDelay& operator=(const ::OcaLiteDelay&);
};

#endif // OCALITEDELAY_H
