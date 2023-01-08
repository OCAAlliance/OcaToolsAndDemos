/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteBitstringActuator
 *
 */
#ifndef OCALITEBITSTRINGACTUATOR_H
#define OCALITEBITSTRINGACTUATOR_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteBitstring.h>

// ---- Include local include files ----
#include "OcaLiteBasicActuator.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCA_BITSTRING_ACTUATOR_CLASSID      OCA_BASIC_ACTUATOR_CLASSID,static_cast< ::OcaUint16>(13)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Bitstring actuator. Maximum bitstring length is 65,536 bits.
 */
class OcaLiteBitstringActuator : public ::OcaLiteBasicActuator
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetNrBits() */
        GET_NRBITS          = 1,
        /** GetBit() */
        GET_BIT             = 2,
        /** SetBit() */
        SET_BIT             = 3,
        /** GetBitstring() */
        GET_BITSTRING       = 4,
        /** SetBitstring() */
        SET_BITSTRING       = 5
    };

    /** Property indexes for the supported properties. */
    enum PropertyIndex
    {
        /** The bitstring data */
        OCA_PROP_BITSTRING  = 1
    };
    
    /**
     * Destructor.
     */
    virtual ~OcaLiteBitstringActuator() {}

    /**
     * Number that uniquely identifies the class. Note that this differs from the object number, which
     * identifies the instantiated object. This is a class property instead of an object property. This
     * property will be overridden by each descendant class, in order to specify that class's ClassID.
     */
    static const ::OcaLiteClassID CLASS_ID;

    // ---- Interface methods  ----
    /**
     * Gets the number of bits in the string
     *
     * @param[out] nrBits   The number of bits in the string
     * @return Indicates whether the property was successfully gathered.
     */
    ::OcaLiteStatus GetNrBits(::OcaUint16& nrBits) const;

    /**
     * Gets the bit value of the given bit.
     *
     * @param[in]  bitNr    The bitnr
     * @param[out] value    Value of the given bit
     * @return Indicates whether the property was successfully gathered.
     */
    ::OcaLiteStatus GetBit(::OcaUint16 bitNr, ::OcaBoolean& value) const;

    /**
     * Sets the bit value of the given bit.
     *
     * @param[in]  bitNr    The bitnr
     * @param[in]  value    Value of the given bit
     * @return Indicates whether the property was successfully set.
     */
    ::OcaLiteStatus SetBit(::OcaUint16 bitNr, ::OcaBoolean value);

    /**
     * Gets the entire bitstring
     *
     * @param[out] bitString    The entire bitstring
     * @return Indicates whether the property was successfully gathered.
     */
    ::OcaLiteStatus GetBitstring(::OcaLiteBitstring& bitString) const;

    /**
     * Sets the entire bitstring
     *
     * @param[in]  bitString    The entire bitstring
     * @return Indicates whether the property was successfully set.
     */
    ::OcaLiteStatus SetBitstring(const ::OcaLiteBitstring& bitString);

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
     * @param[in]   maxLength           The maximum length of bitstring, in bits.
     */
    OcaLiteBitstringActuator(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports, ::OcaUint16 maxLength);

    virtual const ::OcaLiteClassID& GetClassID() const
    {
        return CLASS_ID;
    }

    virtual ::OcaClassVersionNumber GetClassVersion() const;
    
    /**
     * Gets the value of the Bitstring property. 
     *
     * @param[out] bitString    The value of the Bitstring property
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus GetBitstringValue(::OcaLiteBitstring& bitString) const;

    /**
     * Set the value of the Bitstring property. 
     *
     * @param[in]  bitString    Input parameter that holds the value of the
     *                          Bitstring property.
     * @return Indicates whether the operation succeeded.
     */
    virtual ::OcaLiteStatus SetBitstringValue(const ::OcaLiteBitstring& bitString) = 0;

private:
    /** The bitstring data */
    ::OcaLiteBitstring                          m_bitString;

    /** The maximum length of bitstring, in bits. */
    ::OcaUint16                             m_maxLength;

    /** private copy constructor, no copying of object allowed */
    OcaLiteBitstringActuator(const ::OcaLiteBitstringActuator&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteBitstringActuator& operator=(const ::OcaLiteBitstringActuator&);
};

#endif // OCALITEBITSTRINGACTUATOR_H
