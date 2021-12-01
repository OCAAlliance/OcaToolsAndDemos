/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteBooleanActuator
 *
 */
#ifndef OCALITEBOOLEANACTUATOR_H
#define OCALITEBOOLEANACTUATOR_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "OcaLiteBasicActuator.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCA_BOOLEAN_ACTUATOR_CLASSID      OCA_BASIC_ACTUATOR_CLASSID,static_cast< ::OcaUint16>(1)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Basic boolean actuator.
 */
class OcaLiteBooleanActuator : public ::OcaLiteBasicActuator
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetSetting() */
        GET_SETTING         = 1,
        /** SetSetting() */
        SET_SETTING         = 2
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Reading of the setting */
        OCA_PROP_SETTING  = 1
    };
    
    /**
     * Destructor.
     */
    virtual ~OcaLiteBooleanActuator() {}

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods  ----
    /**
     * Gets the Setting property
     *
     * @param[out] setting  The value of the setting property
     * @return Indicates whether the data was successfully retrieved
     */
    ::OcaLiteStatus GetSetting(::OcaBoolean& setting) const;

    /**
     * Sets the Setting property
     *
     * @param[in] setting   The new value of the setting property
     * @return Indicates whether the property was successfully set
     */
    ::OcaLiteStatus SetSetting(::OcaBoolean setting);

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
     */
    OcaLiteBooleanActuator(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;
    
    /**
     * Gets the value of the setting property. Can be overridden by derived classes
     * that actually implement this class.
     *
     * @param[out] setting  The value of the setting property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetSettingValue(::OcaBoolean& setting) const;

    /**
     * Set the value of the setting property. Must be implemented by derived classes
     * that actually implement this class.
     *
     * @param[in]  setting  Input parameter that holds the value of the setting property.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetSettingValue(::OcaBoolean setting) = 0;

private:
    /** The setting value */
    ::OcaBoolean                            m_setting;

    /** private copy constructor, no copying of object allowed */
    OcaLiteBooleanActuator(const ::OcaLiteBooleanActuator&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteBooleanActuator& operator=(const ::OcaLiteBooleanActuator&);
};

#endif // OCALITEBOOLEANACTUATOR_H
