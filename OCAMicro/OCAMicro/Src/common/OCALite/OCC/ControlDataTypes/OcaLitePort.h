/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLitePort
 *
 */

#ifndef OCALITEPORT_H
#define OCALITEPORT_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteFrameworkDataTypes.h"
#include "OcaLitePortID.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Representation of an OCA (input or output) port that is used in the signal path representation
 * of an OCA device.
 */
class OcaLitePort : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLitePort();

    /**
     * Constructor.
     *
     * @param[in]   owner   The object number of the object that owns the port.
     * @param[in]   id      The ID of the port.
     * @param[in]   name    The name of the port.
     */
    OcaLitePort(::OcaONo owner, const ::OcaLitePortID& id, const ::OcaLiteString& name);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLitePort(const ::OcaLitePort& source);

    /**
     * Destructor.
     */
    virtual ~OcaLitePort();

    /**
     * Getter for Owner.
     *
     * @return The object number of the object that owns the port.
     */
    ::OcaONo GetOwner() const
    {
        return m_owner;
    }

    /**
     * Getter for ID.
     *
     * @return The ID of the port.
     */
    const ::OcaLitePortID& GetID() const
    {
        return m_id;
    }

    /**
     * Getter for Name.
     *
     * @return The name of the port.
     */
    const ::OcaLiteString& GetName() const
    {
        return m_name;
    }

    /**
     * Setter for Name.
     * @note This method should only be used by the OcaWorker class, else no network
     *       notifications will be sent when the name is changed.
     *
     * @param[in]   name    The new name of the port.
     */
    void SetName(const ::OcaLiteString& name)
    {
        m_name = name;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLitePort& operator=(const ::OcaLitePort& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    virtual bool operator==(const ::OcaLitePort& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLitePort& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The object number of the object that owns the port. */
    ::OcaONo                        m_owner;

    /** The ID of the port. */
    ::OcaLitePortID                     m_id;

    /** The name of the port. */
    ::OcaLiteString                     m_name;
};

#endif // OCALITEPORT_H
