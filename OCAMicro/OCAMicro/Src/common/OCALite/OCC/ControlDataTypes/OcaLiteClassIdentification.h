/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteClassIdentification
 *
 */

#ifndef OCALITECLASSIDENTIFICATION_H
#define OCALITECLASSIDENTIFICATION_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteClassID.h"
#include "OcaLiteFrameworkDataTypes.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Identification of an OCA class.
 */
class OcaLiteClassIdentification : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteClassIdentification();

    /**
     * Constructor.
     *
     * @param[in]   classID         The class ID of the class.
     * @param[in]   classVersion    The version number of the class.
     */
    OcaLiteClassIdentification(const ::OcaLiteClassID& classID, ::OcaClassVersionNumber classVersion);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteClassIdentification(const ::OcaLiteClassIdentification& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteClassIdentification();

    /**
     * Getter for ClassID.
     *
     * @return The class ID of the class.
     */
    const ::OcaLiteClassID& GetClassID() const
    {
        return m_classID;
    }

    /**
     * Getter for the ClassVersion.
     *
     * @return The class version.
     */
    ::OcaClassVersionNumber GetClassVersion() const
    {
        return m_classVersion;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteClassIdentification& operator=(const ::OcaLiteClassIdentification& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteClassIdentification& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteClassIdentification& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The class ID of the class. */
    ::OcaLiteClassID            m_classID;

    /** The class version. */
    ::OcaClassVersionNumber m_classVersion;
};

#endif // OCALITECLASSIDENTIFICATION_H
