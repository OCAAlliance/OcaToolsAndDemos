/*  By downloading or using this file, the user agrees to be bound by the terms of the license
*  agreement located at http://ocaalliance.com/EULA as an original contracting party.
*/

/*
*  Description         : OcaLiteStringSensor
*
*/
#ifndef OCALITESTRINGSENSOR_H
#define OCALITESTRINGSENSOR_H

// ---- Include system wide include files ----

//#include <OCC/ControlDataTypes/OcaBitstring.h>

// ---- Include local include files ----
#include "OcaLiteBasicSensor.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

/**
 * The classID used for initialization.
 * @ingroup SensorWorkers
 */
#define OCA_STRING_SENSOR_CLASSID      OCA_BASIC_SENSOR_CLASSID,static_cast< ::OcaUint16>(12)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Bit string sensor.
 * @ingroup SensorWorkers
 */
class OcaLiteStringSensor : public ::OcaLiteBasicSensor
{
public:
    /** Method indexes for the supported methods. */
    //lint -e(578) Hides inherited symbol
    enum MethodIndex
    {
        /** GetString() */
        GET_STRING              = 1,
        /** GetMaxLen() */
        GET_MAX_LEN             = 2,
        /** SetMaxLen() */
        SET_MAX_LEN             = 3
    };

    /** Property indexes for the supported properties. */
    //lint -e(578) Hides inherited symbol
    enum PropertyIndex
    {
        /** The string data */
        OCA_PROP_STRING  = 1,
        /** Maximum string length */
        OCA_PROP_MAX_LEN  = 2
    };

    /**
     * Destructor.
     */
    virtual ~OcaLiteStringSensor() {}

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    //lint -e(1516) Hides inherited member
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods  ----
    /**
     * Gets the string
     *
     * @param[out] string    The entire string
     * @return Indicates whether the property was successfully gathered.
     */
    ::OcaLiteStatus GetString(::OcaLiteString& string) const;

    /**
     * Gets the maximum length of the string
     *
     * @param[out] maxLength   Maximum length of the string
     * @return Indicates whether the property was successfully gathered.
     */
    ::OcaLiteStatus GetMaxLen(::OcaUint16& maxLength) const;

    /**
     * Sets the maximum length of the string
     *
     * @param[in] maxLength   Maximum length of the string
     * @return Indicates whether the property was successfully gathered.
     */
    ::OcaLiteStatus SetMaxLen(::OcaUint16 maxLength);

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
    OcaLiteStringSensor(::OcaONo objectNumber,
                    ::OcaBoolean lockable,
                    const ::OcaLiteString& role,
                    const ::OcaLiteList< ::OcaLitePort>& ports);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;

    /**
     * Gets the value of the String property. Must be implemented by derived classes
     * that actually implement the OcaStringSensor.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[out] stringData    The value of the string property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetStringValue(::OcaLiteString& stringData) const = 0;

    /**
     * Gets the value of the MaxLength property. Can be overridden by derived classes
     * that actually implement the worker OcaStringSensor.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[out] maxLength    The value of the MaxLength property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetMaxLengthValue(::OcaUint16& maxLength) const;

    /**
     * Sets the value of the MaxLength property. Must be implemented by derived classes
     * that actually implement the OcaStringSensor.
     * @note This method does not take the object's mutex itself.
     *       The mutex should be taken before this method is called.
     *
     * @param[in] maxLength    The value of the MaxLength property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetMaxLengthValue(const ::OcaUint16 maxLength) = 0;

    /**
     * Event that is emitted when the string has changed.
     * @note This method takes the object's mutex itself.
     *       The mutex should not be taken before this method is called.
     *
     * @param[in]   string   The new string
     */

    void StringChanged(const ::OcaLiteString string);


private:   
    /** The maximum length that this object can accept */
    ::OcaUint16                             m_maxLen;

    /** private copy constructor, no copying of object allowed */
    OcaLiteStringSensor(const ::OcaLiteStringSensor&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteStringSensor& operator=(const ::OcaLiteStringSensor&);
};

#endif // OCALITESTRINGSENSOR_H
