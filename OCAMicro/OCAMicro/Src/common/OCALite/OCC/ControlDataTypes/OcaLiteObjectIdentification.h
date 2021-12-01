/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteObjectIdentification
 *
 */

#ifndef OCALITEOBJECTIDENTIFICATION_H
#define OCALITEOBJECTIDENTIFICATION_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteClassIdentification.h"
#include "OcaLiteFrameworkDataTypes.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Object identification. Composite of object number and object's class.
 */
class OcaLiteObjectIdentification : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteObjectIdentification();

    /**
     * Constructor.
     *
     * @param[in]   oNo                 The object number of the referenced object.
     * @param[in]   classIdentification The class identification of the referenced object.
     */
    OcaLiteObjectIdentification(::OcaONo oNo, const ::OcaLiteClassIdentification& classIdentification);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteObjectIdentification(const ::OcaLiteObjectIdentification& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteObjectIdentification();

    /**
     * Getter for the ONo.
     *
     * @return The object number of the referenced object.
     */
    ::OcaONo GetONo() const
    {
        return m_oNo;
    }

    /**
     * Getter for ClassIdentification.
     *
     * @return The class identification of the referenced object.
     */
    const ::OcaLiteClassIdentification& GetClassIdentification() const
    {
        return m_classIdentification;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteObjectIdentification& operator=(const ::OcaLiteObjectIdentification& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteObjectIdentification& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteObjectIdentification& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The object number of the referenced object. */
    ::OcaONo                    m_oNo;

    /** The class identification of the refernced object. */
    ::OcaLiteClassIdentification    m_classIdentification;
};

#endif // OCALITEOBJECTIDENTIFICATION_H
