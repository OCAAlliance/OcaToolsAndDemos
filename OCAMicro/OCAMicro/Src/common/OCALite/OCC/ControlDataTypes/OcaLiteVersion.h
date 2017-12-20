/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteVersion
 *
 */

#ifndef OCALITEVERSION_H
#define OCALITEVERSION_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteFrameworkDataTypes.h"
#include "OcaLiteManagementDataTypes.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Representation of a version number of a (hardware/software) component of a device in the form
 * of Major.Minor.Build (e.g. 1.0.123).
 */
class OcaLiteVersion : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteVersion();

    /**
     * Constructor.
     *
     * @param[in]   major           The major version number.
     * @param[in]   minor           The minor version number.
     * @param[in]   build           The build number. May be 0 if it is not used (e.g. for a hardware component)
     * @param[in]   component       The component.
     */
    OcaLiteVersion(::OcaUint32 major, ::OcaUint32 minor, ::OcaUint32 build, ::OcaLiteComponent component);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteVersion(const ::OcaLiteVersion& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteVersion();

    /**
     * Getter for Major.
     *
     * @return The major version number.
     */
    ::OcaUint32 GetMajor() const
    {
        return m_major;
    }

    /**
     * Getter for Minor.
     *
     * @return The minor version number.
     */
    ::OcaUint32 GetMinor() const
    {
        return m_minor;
    }

    /**
     * Getter for Build.
     *
     * @return The build number.
     */
    ::OcaUint32 GetBuild() const
    {
        return m_build;
    }

    /**
     * Getter for Component.
     *
     * @return The component.
     */
    ::OcaLiteComponent GetComponent() const
    {
        return m_component;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteVersion& operator=(const ::OcaLiteVersion& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteVersion& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteVersion& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The major version number. */
    ::OcaUint32             m_major;

    /** The minor version number. */
    ::OcaUint32             m_minor;

    /** The build number. */
    ::OcaUint32             m_build;

    /** The component. */
    ::OcaLiteComponent          m_component;
};

#endif // OCALITEVERSION_H
