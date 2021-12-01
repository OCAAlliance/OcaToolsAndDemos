/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMethodID
 *
 */

#ifndef OCALITEMETHODID_H
#define OCALITEMETHODID_H

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
 * Representation of an OCA method ID.
 */
class OcaLiteMethodID : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteMethodID();

    /**
     * Constructor.
     *
     * @param[in]   defLevel    Level in the tree of classes which defines this method (1=root).
     * @param[in]   methodIndex Index of the method (in the class description).
     */
    OcaLiteMethodID(::OcaUint16 defLevel, ::OcaUint16 methodIndex);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteMethodID(const ::OcaLiteMethodID& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteMethodID();

    /**
     * Getter for DefLevel.
     *
     * @return The level in the tree of classes which defines this method (1=root).
     */
    ::OcaUint16 GetDefLevel() const
    {
        return m_defLevel;
    }

    /**
     * Getter for MethodIndex.
     *
     * @return The index of the method (in the class description).
     */
    ::OcaUint16 GetMethodIndex() const
    {
        return m_methodIndex;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteMethodID& operator=(const ::OcaLiteMethodID& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteMethodID& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteMethodID& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The level in the tree of classes which defines this method (1=root). */
    ::OcaUint16     m_defLevel;

    /** The index of the method (in the class description). */
    ::OcaUint16     m_methodIndex;
};

#endif // OCALITEMETHODID_H
