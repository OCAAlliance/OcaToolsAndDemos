/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteModelDescription
 *
 */

#ifndef OCALITEMODELDESCRIPTION_H
#define OCALITEMODELDESCRIPTION_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteManagementDataTypes.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Friendly description of this device model.
 */
class OcaLiteModelDescription : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteModelDescription();

    /**
     * Constructor.
     *
     * @param[in]   manufacturer    The name of the manufacturer.
     * @param[in]   name            The name of this model (whatever the manufacturer wants to call it).
     * @param[in]   version         The text name for the version of this model, e.g. "1.2.1a".
     */
    OcaLiteModelDescription(const ::OcaLiteString& manufacturer,
                        const ::OcaLiteString& name,
                        const ::OcaLiteString& version);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteModelDescription(const ::OcaLiteModelDescription& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteModelDescription();

    /**
     * Getter for Manufacturer.
     *
     * @return The name of the manufacturer.
     */
    const ::OcaLiteString& GetManufacturer() const
    {
        return m_manufacturer;
    }

    /**
     * Getter for Name.
     *
     * @return The name of this model.
     */
    const ::OcaLiteString& GetName() const
    {
        return m_name;
    }

    /**
     * Getter for Version.
     *
     * @return The text name for the version of this model.
     */
    const ::OcaLiteString& GetVersion() const
    {
        return m_version;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteModelDescription& operator=(const ::OcaLiteModelDescription& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteModelDescription& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteModelDescription& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The name of the manufacturer. */
    ::OcaLiteString             m_manufacturer;

    /** The name of this model. */
    ::OcaLiteString             m_name;

    /** The text name for the version of this model. */
    ::OcaLiteString             m_version;
};

#endif // OCALITEMODELDESCRIPTION_H
