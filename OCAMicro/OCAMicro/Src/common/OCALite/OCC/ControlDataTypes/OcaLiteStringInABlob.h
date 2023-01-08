/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteStringInABlob
 *
 */

#ifndef OCALITESTRINGINABLOB_H
#define OCALITESTRINGINABLOB_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteBlobDataType.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/** Aes67ServiceID is a string in a blob */
#define OcaLiteMediaTransportNetworkAes67ServiceID      OcaLiteStringInABlob
/** DanteNodeID is a string in a blob */
#define OcaLiteNetworkDanteNodeID                       OcaLiteStringInABlob
/** Stream Network ID advertised is a string in a blob */
#define OcaLiteStreamNetworkIDAdvertised                OcaLiteStringInABlob
/** OCP.1 Network Node ID is a string in a blob */
#define Ocp1LiteNetworkNodeID                           OcaLiteStringInABlob
/** Network Signal Channel Dante ID is a string in a blob */
#define OcaLiteNetworkSignalChannelDanteID              OcaLiteStringInABlob

/**
 * To save some code size. A lot of names are marshalled in a blob. These are defined above and all use this data type.
 */
class OcaLiteStringInABlob : public ::OcaLiteBlobDataType
{
public:
    /**
     * Constructor
     *
     * @param[in]   danteId      The dante ID
     */
    explicit OcaLiteStringInABlob(const ::OcaLiteString& danteId);

   /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteStringInABlob(const ::OcaLiteStringInABlob&);

    /** Destructor */
    virtual ~OcaLiteStringInABlob();

    /**
     * Getter for the string value
     *
     * @return m_stringValue
     */
    const ::OcaLiteString& GetStringValue() const;

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteStringInABlob& operator=(const ::OcaLiteStringInABlob& source);

    /**
     * Creates an OcaLiteStringInABlob from the base class.
     *
     * @param[in]   baseClass   The base class to create the instance from.
     * @return The newly created instance, or NULL if parsing failed.
     */
    static ::OcaLiteStringInABlob* CreateFromBase(const ::OcaLiteBlobDataType& baseClass);

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);


    /**
     * Get the string value from the base value
     *
     * @param[in]   baseClass   The base class to get the value from
     * @param[out]  stringValue The parsed value, only valid if the return value is true.
     * @return The result of the operation.
     */
    ::OcaBoolean GetStringValueFromBase(const ::OcaLiteBlobDataType& baseClass, OcaLiteString &stringValue)
    {
        return GetValuesFromBase(baseClass, stringValue);
    }


private:

    /**
     * Unmarshal the base and fill the members
     *
     * @param[in]  baseClass The base class the read the values from.
     * @param[out] danteId   The dante Id.
     *
     * @return True if succeeded, false if not. 
     */
    static bool GetValuesFromBase(const ::OcaLiteBlobDataType& baseClass, ::OcaLiteString& danteId);

    /** Update the base blob with the private members */
    void UpdateBlob();

    /** The dante ID. */
    ::OcaLiteString                 m_stringValue;
};

#endif //OCALITESTRINGINABLOB_H
