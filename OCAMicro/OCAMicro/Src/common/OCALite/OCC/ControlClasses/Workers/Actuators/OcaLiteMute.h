/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteMute
 *
 */
#ifndef OCALITEMUTE_H
#define OCALITEMUTE_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include <OCC/ControlDataTypes/OcaLiteWorkerDataTypes.h>
#include "OcaLiteActuator.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCA_MUTE_CLASSID      OCA_ACTUATOR_CLASSID,static_cast< ::OcaUint16>(2)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Signal mute
 * @ingroup ActuatorWorkers
 */
class OcaLiteMute : public ::OcaLiteActuator
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetState() */
        GET_STATE   = 1,
        /** SetState() */
        SET_STATE   = 2
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Current state of the mute */
        OCA_PROP_STATE  = 1
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteMute() {}

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods  ----
    /**
     * Gets the current mute state.
     *
     * @param[out] state    Output parameter that holds mute state (i.e. the value of the State property)
     * @return Indicates whether the data was successfully retrieved.
     */
    ::OcaLiteStatus GetState(::OcaLiteMuteState& state) const;

    /**
     * Sets the mute state (i.e. value of the State property)
     *
     * @param[in]  state    The value to which the State property must be set.
     * @return Indicates whether the property was successfully set.
     */
    ::OcaLiteStatus SetState(::OcaLiteMuteState state);

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

protected:
    /**
     * Constructor
     *
     * @param[in]  objectNumber Object number of this instance.
     * @param[in]  lockable     Indicates whether or not the object is lockable.
     * @param[in]  role         The role of this instance.
     * @param[in]  ports        The OCA input and output ports.
     */
    OcaLiteMute(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    /**
     * Gets the value of the state property. Can be overridden by derived classes
     * that actually implement the worker.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[out]  state   The value of the state property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetStateValue(::OcaLiteMuteState& state) const;

    /**
     * Set the value of the state property. Must be implemented by derived classes
     * that actually implement the worker.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[in]  state    Input parameter that holds the value of the state property.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetStateValue(::OcaLiteMuteState state) = 0;

private:
    /** The mute state */
    ::OcaLiteMuteState                          m_state;

    /** private copy constructor, no copying of object allowed */
    OcaLiteMute(const ::OcaLiteMute&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteMute& operator=(const ::OcaLiteMute&);
};

#endif // OCALITEMUTE_H
