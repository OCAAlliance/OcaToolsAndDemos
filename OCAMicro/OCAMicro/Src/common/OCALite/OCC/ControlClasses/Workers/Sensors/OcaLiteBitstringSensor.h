/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteBitstringSensor
 *
 */
#ifndef OCALITEBITSTRINGSENSOR_H
#define OCALITEBITSTRINGSENSOR_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteBitstring.h>

// ---- Include local include files ----
#include "OcaLiteBasicSensor.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/**
 * The classID used for initialization.
 */
#define OCA_BITSTRING_SENSOR_CLASSID      OCA_BASIC_SENSOR_CLASSID,static_cast< ::OcaUint16>(13)

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Bitstring sensor.
 */
class OcaLiteBitstringSensor : public ::OcaLiteBasicSensor
{
public:
    /** Method indexes for the supported methods. */
    enum MethodIndex
    {
        /** GetNrBits() */
        GET_NRBITS          = 1,
        /** GetBit() */
        GET_BIT             = 2,
        /** GetBitstring() */
        GET_BITSTRING       = 3
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
    virtual ~OcaLiteBitstringSensor() {}

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
     * Gets the entire bitstring
     *
     * @param[out] bitString    The entire bitstring
     * @return Indicates whether the property was successfully gathered.
     */
    ::OcaLiteStatus GetBitstring(::OcaLiteBitstring& bitString) const;

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
     * @param[in]   nrBits              Length of bitstring, in bits.
     */
    OcaLiteBitstringSensor(::OcaONo objectNumber, ::OcaBoolean lockable, const ::OcaLiteString& role, const ::OcaLiteList< ::OcaLitePort>& ports, ::OcaUint16 nrBits);

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
    virtual ::OcaLiteStatus GetBitstringValue(::OcaLiteBitstring& bitString) const = 0;

    /**
     * Event that is emitted when the bitstring has changed.
     *
     * @param[in]   bitString   The new bitstring
     */
    void BitstringChanged(const ::OcaLiteBitstring& bitString);

private:
    /** Length of bitstring, in bits */
    ::OcaUint16                             m_nrBits;

    /** private copy constructor, no copying of object allowed */
    OcaLiteBitstringSensor(const ::OcaLiteBitstringSensor&);
    /** private assignment operator, no assignment of object allowed */
    ::OcaLiteBitstringSensor& operator=(const ::OcaLiteBitstringSensor&);
};

#endif // OCALITEBITSTRINGSENSOR_H
