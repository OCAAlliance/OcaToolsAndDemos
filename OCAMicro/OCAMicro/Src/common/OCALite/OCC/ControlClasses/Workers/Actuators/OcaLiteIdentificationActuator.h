/*
*  By downloading or using this file, the user agrees to be bound by the terms of the license
*  agreement located at http://ocaalliance.com/EULA as an original contracting party.
*
*  Description         : OcaLiteIdentificationActuator
*
*/
#ifndef OCALITEIDENTIFICATIONACTUATOR_H
#define OCALITEIDENTIFICATIONACTUATOR_H

// ---- Include system wide include files ----
//#include <StlDllExportMacros.h>

// ---- Include local include files ----
#include "OcaLiteActuator.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 * @ingroup ActuatorWorkers
 */
#define OCA_IDENTIFICATION_ACTUATOR_CLASSID     OCA_ACTUATOR_CLASSID,static_cast< ::OcaUint16>(21)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Represents a function that turns on some kind of human-detectable indicator for purposes of device
 * identification during network setup. Physical form of indicator is not defined by OCA, so it could be
 * anything, e.g.
 * - LED
 * - Foghorn
 * - Smoke emitter
 * - Little waving robot hand wearing white glove
 * - Jack-in-the-box popout
 * - et cetera
 * @ingroup ActuatorWorkers
 */
class OcaLiteIdentificationActuator : public ::OcaLiteActuator
{
public:
    /** Method indexes for the supported methods. */
    //lint -e(578) Hides inherited symbol
    enum MethodIndex
    {
        /** GetActive() */
        GET_ACTIVE          = 1,
        /** SetActive() */
        SET_ACTIVE          = 2
    };

    /** Property indexes for the supported properties. */
    //lint -e(578) Hides inherited symbol
    enum PropertyIndex
    {
        /** True iff indicator is active */
        OCA_PROP_ACTIVE     = 1
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteIdentificationActuator() {}

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    //lint -e(1516) Hides inherited member
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods  ----
    /**
     * Gets the current indicator activity state.
     *
     * @param[out] active       Output parameter that holds active state
     * @return Indicates whether the data was successfully retrieved
     */
    ::OcaLiteStatus GetActive(::OcaBoolean& active) const;

    /**
     * Sets the Active state
     *
     * @param[in] active        The new value of the active property
     * @return Indicates whether the property was successfully set
     */
    ::OcaLiteStatus SetActive(::OcaBoolean active);

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

    /**
     * Retrieves the property descriptor for the Active property of OcaLiteIdentificationActuator.
     *
     * @return The property descriptor for the Active property of OcaLiteIdentificationActuator.
     */
   // static const ::OcaPropertyDescriptor& GetActivePropertyDescriptor();

protected:
    /**
     * Constructor
     *
     * @param[in]   objectNumber        Object number of this instance.
     * @param[in]   lockable            Indicates whether or not the object is lockable.
     * @param[in]   role                The role of this instance.
     * @param[in]   ports               The OCA input and output ports.
     */
    OcaLiteIdentificationActuator(::OcaONo objectNumber,
                              ::OcaBoolean lockable,
                              const ::OcaLiteString& role,
                              const ::OcaLiteList< ::OcaLitePort>& ports);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    /**
     * Gets the value of the active property. Can be overridden by derived classes
     * that actually implement this class.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[out] active       The value of the active property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetActiveValue(::OcaBoolean& active) const;

    /**
     * Set the value of the active property. Must be implemented by derived classes
     * that actually implement this class.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[in]  active       Input parameter that holds the value of the active property.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetActiveValue(::OcaBoolean active) = 0;

private:
    /** The active value */
    ::OcaBoolean                            m_active;

    /** private copy constructor, no copying of object allowed */
    OcaLiteIdentificationActuator(const ::OcaLiteIdentificationActuator&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteIdentificationActuator& operator=(const ::OcaLiteIdentificationActuator&);
};
#endif // OCALITEIDENTIFICATIONACTUATOR_H
