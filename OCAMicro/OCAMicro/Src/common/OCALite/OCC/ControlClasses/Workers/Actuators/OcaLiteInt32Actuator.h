/*
*  By downloading or using this file, the user agrees to be bound by the terms of the license
*  agreement located at http://ocaalliance.com/EULA as an original contracting party.
*
*  Description         : OcaLiteInt32Actuator
*
*/
#ifndef OCALITEINT32ACTUATOR_H
#define OCALITEINT32ACTUATOR_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "OcaLiteBasicActuator.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 * @ingroup ActuatorWorkers
 */
#define OCA_INT32_ACTUATOR_CLASSID   OCA_BASIC_ACTUATOR_CLASSID,static_cast< ::OcaUint16>(4)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Basic Int32 actuator
 */
class OcaLiteInt32Actuator : public ::OcaLiteBasicActuator
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
    virtual ~OcaLiteInt32Actuator() {}

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    //lint -e(1516) Hides inherited member
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods  ----
    /**
     * Gets the Setting property
     *
     * @param[out] setting      The value of the setting property
     * @param[out] minSetting   Minimum setting value
     * @param[out] maxSetting   Maximum setting value
     * @return Indicates whether the data was successfully retrieved
     */
    ::OcaLiteStatus GetSetting(::OcaInt32& setting, ::OcaInt32& minSetting, ::OcaInt32& maxSetting) const;

    /**
     * Sets the Setting property
     *
     * @param[in] setting   The new value of the setting property
     * @return Indicates whether the property was successfully set
     */
    ::OcaLiteStatus SetSetting(::OcaInt32 setting);

    virtual ::OcaLiteStatus Execute(const ::IOcaLiteReader& reader, const ::IOcaLiteWriter& writer, ::OcaSessionID sessionID, const ::OcaLiteMethodID& methodID,
                                    ::OcaUint32 parametersSize, const ::OcaUint8* parameters, ::OcaUint8** response);

protected:
    /**
     * Constructor
     *
     * @param[in]   objectNumber        Object number of this instance.
     * @param[in]   lockable            Indicates whether or not the object is lockable.
     * @param[in]   role                The role of this instance.
     * @param[in]   ports               The OCA input and output ports.
     * @param[in]   minSetting          Lower limit of the setting
     * @param[in]   maxSetting          Upper limit of the setting
     */
    OcaLiteInt32Actuator(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports, ::OcaInt32 minSetting, ::OcaInt32 maxSetting);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    /**
     * Gets the value of the setting property. Can be overridden by derived classes
     * that actually implement this class.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[out] setting  The value of the setting property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetSettingValue(::OcaInt32& setting) const;

    /**
     * Set the value of the setting property. Must be implemented by derived classes
     * that actually implement this class.
     * @note This method should not take the object's mutex itself.
     *       The mutex is already taken before this method is called.
     *
     * @param[in]  setting  Input parameter that holds the value of the setting property.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetSettingValue(::OcaInt32 setting) = 0;

private:
    /** The setting value */
    ::OcaInt32                              m_setting;
    /** Lower limit of the setting property */
    ::OcaInt32                              m_minSetting;
    /** Upper limit of the setting property */
    ::OcaInt32                              m_maxSetting;

    /** private copy constructor, no copying of object allowed */
    OcaLiteInt32Actuator(const ::OcaLiteInt32Actuator&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteInt32Actuator& operator=(const ::OcaLiteInt32Actuator&);
};
#endif // OCALITEINT32ACTUATOR_H
