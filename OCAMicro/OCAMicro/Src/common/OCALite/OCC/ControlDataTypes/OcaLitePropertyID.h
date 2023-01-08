/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLitePropertyID
 *
 */

#ifndef OCALITEPROPERTYID_H
#define OCALITEPROPERTYID_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteFrameworkDataTypes.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Representation of an OCA property ID.
 */
class OcaLitePropertyID : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLitePropertyID();

    /**
     * Constructor.
     *
     * @param[in]   defLevel        Level in the tree of classes which defines this property (1=root).
     * @param[in]   propertyIndex   Index of the property (in the class description).
     */
    OcaLitePropertyID(::OcaUint16 defLevel, ::OcaUint16 propertyIndex);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLitePropertyID(const ::OcaLitePropertyID& source);

    /**
     * Destructor.
     */
    virtual ~OcaLitePropertyID();

    /**
     * Getter for DefLevel.
     *
     * @return The level in the tree of classes which defines this property (1=root).
     */
    ::OcaUint16 GetDefLevel() const
    {
        return m_defLevel;
    }

    /**
     * Getter for PropertyIndex.
     *
     * @return The index of the property (in the class description).
     */
    ::OcaUint16 GetPropertyIndex() const
    {
        return m_propertyIndex;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLitePropertyID& operator=(const ::OcaLitePropertyID& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLitePropertyID& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLitePropertyID& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The level in the tree of classes which defines this property (1=root). */
    ::OcaUint16     m_defLevel;

    /** The index of the property (in the class description). */
    ::OcaUint16     m_propertyIndex;
};

#endif // OCALITEPROPERTYID_H
