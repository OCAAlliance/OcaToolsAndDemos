/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : OcaLiteStreamConnectorID
 *
 */

#ifndef OCALITESTREAMCONNECTORID_H
#define OCALITESTREAMCONNECTORID_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteBlob.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * ID (name or GUID) of network node. There may be one or more nodes per host. Nodes offer media
 * network services, and are what is discovered by discovery processes. Each instance of OcaNetwork (or
 * one of its subclasses) is a node.
 */
class OcaLiteStreamConnectorID : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteStreamConnectorID();

    /**
     * Constructor.
     *
     * @param[in]   value       The value.
     */
    explicit OcaLiteStreamConnectorID(const ::OcaLiteBlob& value);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteStreamConnectorID(const ::OcaLiteStreamConnectorID& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteStreamConnectorID();

    /**
     * Getter for the value
     *
     * @return The value
     */
    const ::OcaLiteBlob& GetValue() const
    {
        return m_value;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteStreamConnectorID& operator=(const ::OcaLiteStreamConnectorID& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteStreamConnectorID& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteStreamConnectorID& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

protected:
    /** Set value */
    void SetValue(const ::OcaLiteBlob& value)
    {
        m_value = value;
    }

private:
    /** The value. */
    ::OcaLiteBlob    m_value;
};

#endif // OCANETWORKNODEID_H
