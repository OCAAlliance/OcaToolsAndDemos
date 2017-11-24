/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteGain
 *
 */
#ifndef OCALITEGAIN_H
#define OCALITEGAIN_H

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
#define OCA_GAIN_CLASSID      OCA_ACTUATOR_CLASSID,static_cast< ::OcaUint16>(5)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Gain (or attenuation) element.
 * @ingroup ActuatorWorkers
 */
class OcaLiteGain : public ::OcaLiteActuator
{
public:
    /** Method indexes for the supported methods. */
    //lint -e(578) Hides inherited symbol
    enum MethodIndex
    {
        /** GetGain() */
        GET_GAIN        = 1,
        /** SetGain() */
        SET_GAIN        = 2
    };

    /** Property indexes for the supported properties. */
    //lint -e(578) Hides inherited symbol
    enum PropertyIndex
    {
        /** Gain in dB */
        OCA_PROP_GAIN  = 1
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteGain() {}

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    //lint -e(1516) Hides inherited member
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods  ----
    /**
     * Gets the value and limits of the Gain property.
     *
     * @param[out] gain     The value and limits of the Gain property
     * @param[out] minGain  Lower limit of the gain
     * @param[out] maxGain  Upper limit of the gain
     * @return Indicates whether the data was successfully retrieved.
     */
    ::OcaLiteStatus GetGain(::OcaDB& gain, ::OcaDB& minGain, ::OcaDB& maxGain) const;

    /**
     * Sets the value of the Level property.
     *
     * @param[in]  gain   The value of the Level property
     * @return Indicates whether the property was successfully set.
     */
    ::OcaLiteStatus SetGain(::OcaDB gain);

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
     * @param[in]  minGain      Lower limit of the gain
     * @param[in]  maxGain      Upper limit of the gain
     */
    OcaLiteGain(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports, ::OcaDB minGain, ::OcaDB maxGain);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    /**
     * Gets the value of the Gain property. Can be overridden by derived classes
     * that actually implement the worker.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[out]  gain    The value of the Gain property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetGainValue(::OcaDB& gain) const;

    /**
     * Set the value of the Gain property. Must be implemented by derived classes
     * that actually implement the worker.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[in]  gain     Input parameter that holds the value of the Gain property.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetGainValue(::OcaDB gain) = 0;

private:
    /** The gain in dB */
    ::OcaDB                                 m_gain;
    /** Lower limit of the gain */
    ::OcaDB                                 m_minGain;
    /** Upper limit of the gain */
    ::OcaDB                                 m_maxGain;

    /** private copy constructor, no copying of object allowed */
    OcaLiteGain(const ::OcaLiteGain&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteGain& operator=(const ::OcaLiteGain&);
};

#endif // OCALITEGAIN_H
