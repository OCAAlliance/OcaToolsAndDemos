/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteSwitch
 *
 */
#ifndef OCALITESWITCH_H
#define OCALITESWITCH_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "OcaLiteActuator.h"

// ---- Referenced classes and types ----
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>
#include <OCC/ControlDataTypes/OcaLitePropertyChangedEventData.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCA_SWITCH_CLASSID      OCA_ACTUATOR_CLASSID,static_cast< ::OcaUint16>(4)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * (n)-position single-pole switch
 */
class OcaLiteSwitch : public ::OcaLiteActuator
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetPosition() */
        GET_POSITION            = 1,
        /** SetPosition() */
        SET_POSITION            = 2,
        /** GetPositionName() */
        GET_POSITION_NAME       = 3,
        /** SetPositionName() */
        SET_POSITION_NAME       = 4,
        /** GetPositionNames() */
        GET_POSITION_NAMES      = 5,
        /** SetPositionNames() */
        SET_POSITION_NAMES      = 6,
        /** GetPositionEnabled() */
        GET_POSITION_ENABLED    = 7,
        /** SetPositionEnabled() */
        SET_POSITION_ENABLED    = 8,
        /** GetPositionEnableds() */
        GET_POSITION_ENABLEDS   = 9,
        /** SetPositionEnableds() */
        SET_POSITION_ENABLEDS   = 10
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** Current position of the switch */
        OCA_PROP_POSITION           = 1,
        /** Vector of switch position names */
        OCA_PROP_POSITION_NAMES     = 2,
        /** Vector of booleans which enable or disable corresponding switch positions */
        OCA_PROP_POSITION_ENABLE    = 3
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteSwitch() {}

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods  ----
    /**
     * Gets the value of the Position property.
     *
     * @param[out] position     Output parameter that holds current switch position
     * @param[out] minPosition  Output parameter that holds minimum value for the switch position
     * @param[out] maxPosition  Output parameter that holds maximum value for the switch position
     * @return Indicates whether the data was successfully retrieved.
     */
    ::OcaLiteStatus GetPosition(::OcaUint16& position, ::OcaUint16& minPosition, ::OcaUint16& maxPosition) const;

    /**
     * Sets the value of the Position property
     *
     * @param[in] position      The value to which the Position property must be set
     * @return Indicates whether the property was successfully set.
     */
    ::OcaLiteStatus SetPosition(::OcaUint16 position);

    /**
     * Gets the name assigned to a given switch position
     *
     * @param[in]  index    Index of name to get.
     * @param[out] name     Output parameter that holds the value of the name
     * @return Indicates whether the data was successfully retrieved.
     */
    ::OcaLiteStatus GetPositionName(::OcaUint16 index, ::OcaLiteString& name) const;

    /**
     * Assigns a name to a given switch position
     *
     * @param[in] index     Index of name to set.
     * @param[in] name      New name for this Position
     * @return Indicates whether the property was successfully set.
     */
    ::OcaLiteStatus SetPositionName(::OcaUint16 index, const ::OcaLiteString& name);

    /**
     * Gets list of names assigned to the switch's positions
     *
     * @param[out] names    Output parameter that holds the list of switch position names
     * @return Indicates whether the data was successfully retrieved.
     */
    ::OcaLiteStatus GetPositionNames(::OcaLiteList< ::OcaLiteString>& names) const;

    /**
     * Assigns names to the switch's positions
     *
     * @param[in]  names    List of switch position names
     * @return Indicates whether the property was successfully set.
     */
    ::OcaLiteStatus SetPositionNames(const ::OcaLiteList< ::OcaLiteString>& names);

    /**
     * Gets the enabled flag assigned to a given switch position
     *
     * @param[in]  index    Index of enabled flag to get.
     * @param[out] enabled  Output parameter that holds the value of the enabled flag
     * @return Indicates whether the data was successfully retrieved.
     */
    ::OcaLiteStatus GetPositionEnabled(::OcaUint16 index, ::OcaBoolean& enabled) const;

    /**
     * Sets the enabled flag assigned to a given switch position
     *
     * @param[in] index     Index of enabled flag to set.
     * @param[in] enabled   New value for the enabled flag for this Position
     * @return Indicates whether the property was successfully set.
     */
    ::OcaLiteStatus SetPositionEnabled(::OcaUint16 index, ::OcaBoolean enabled);

    /**
     * Gets list of enabled flags assigned to the switch's positions
     *
     * @param[out] enableds Output parameter that holds the list of enabled flags
     * @return Indicates whether the data was successfully retrieved.
     */
    ::OcaLiteStatus GetPositionEnableds(::OcaLiteList< ::OcaBoolean>& enableds) const;

    /**
     * Sets list of enabled flags for the switch's positions
     *
     * @param[in]  enableds List of enabled flags
     * @return Indicates whether the property was successfully set.
     */
    ::OcaLiteStatus SetPositionEnableds(const ::OcaLiteList< ::OcaBoolean>& enableds);

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
     * @param[in]  minPosition      The minimum value for the switch position
     * @param[in]  maxPosition      The maximum value for the switch position
     */
    OcaLiteSwitch(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports, ::OcaUint16 minPosition, ::OcaUint16 maxPosition);

    /**
     * Constructor
     *
     * @param[in]  objectNumber     Object number of this instance.
     * @param[in]  lockable         Indicates whether or not the object is lockable.
     * @param[in]  role             The role of this instance.
     * @param[in]  ports            The OCA input and output ports.
     * @param[in]  minPosition      The minimum value for the switch position
     * @param[in]  maxPosition      The maximum value for the switch position
     * @param[in]  positionNames    Switch position names.
     * @param[in]  positionEnable   Switch position enable/disable.
     */
    OcaLiteSwitch(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports, ::OcaUint16 minPosition,
              ::OcaUint16 maxPosition, const ::OcaLiteList< ::OcaLiteString>& positionNames, const ::OcaLiteList< ::OcaBoolean>& positionEnable);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    /**
     * Gets the value of the Position property. Can be overridden by derived classes
     * that actually implement this class.
     *
     * @param[out] position  The value of the Position property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetPositionValue(::OcaUint16& position) const;

    /**
     * Set the value of the position property. Must be implemented by derived classes
     * that actually implement the worker.
     *
     * @param[in]  position    Input parameter that holds the value of the position property.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetPositionValue(::OcaUint16 position) = 0;

    /**
     * Gets the value of the PositionName property. Can be overridden by derived classes
     * that actually implement this class.
     *
     * @param[in]  index     Index of name to set.
     * @param[out] name      New name for this Position
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetPositionNameValue(::OcaUint16 index, ::OcaLiteString& name) const;

    /**
     * Assigns a name to a given switch position. Must be implemented by derived classes
     * that actually implement the worker.
     *
     * @param[in] index     Index of name to set.
     * @param[in] name      New name for this Position
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetPositionNameValue(::OcaUint16 index, const ::OcaLiteString& name) = 0;

    /**
     * Gets the value of the PositionNames property. Can be overridden by derived classes
     * that actually implement this class.
     *
     * @param[out] names  The value of the PositionNames property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetPositionNamesValue(::OcaLiteList< ::OcaLiteString>& names) const;

    /**
     * Assigns names to the switch's positions. Must be implemented by derived classes
     * that actually implement the worker.
     *
     * @param[in]  names    List of switch position names
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetPositionNamesValue(const ::OcaLiteList< ::OcaLiteString>& names) = 0;

    /**
     * Gets the value of the PositionEnabled property. Can be overridden by derived classes
     * that actually implement this class.
     *
     * @param[in]  index     Index of enabled to set.
     * @param[out] enabled   he enabled for this Position
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetPositionEnabledValue(::OcaUint16 index, ::OcaBoolean& enabled) const;

    /**
     * Sets the enabled flag assigned to a given switch position. Must be implemented by derived classes
     * that actually implement the worker.
     *
     * @param[in] index     Index of enabled flag to set.
     * @param[in] enabled   New value for the enabled flag for this Position
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetPositionEnabledValue(::OcaUint16 index, ::OcaBoolean enabled) = 0;

    /**
     * Gets the value of the PositionEnableds property. Can be overridden by derived classes
     * that actually implement this class.
     *
     * @param[out] enableds  The value of the PositionEnableds property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetPositionEnabledsValue(::OcaLiteList< ::OcaBoolean>& enableds) const;

    /**
     * Sets list of enabled flags for the switch's positions. Must be implemented by derived classes
     * that actually implement the worker.
     *
     * @param[in]  enableds List of enabled flags
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetPositionEnabledsValue(const ::OcaLiteList< ::OcaBoolean>& enableds) = 0;

private:
    /** The current position of the switch */
    ::OcaUint16                             m_position;
    /** The minimum position of the switch */
    ::OcaUint16                             m_minPosition;
    /** The maximum position of the switch */
    ::OcaUint16                             m_maxPosition;

    /** Vector of switch position names. Supplied by controller. */
    ::OcaLiteList< ::OcaLiteString>             m_positionNames;

    /** Vector of booleans which enable or disable corresponding switch positions */
    ::OcaLiteList< ::OcaBoolean>                m_positionEnable;

    /** private copy constructor, no copying of object allowed */
    OcaLiteSwitch(const ::OcaLiteSwitch&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteSwitch& operator=(const ::OcaLiteSwitch&);
};

#endif // OCALITESWITCH_H
