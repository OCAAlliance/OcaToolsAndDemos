/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteNetworkAddress
 *
 */

#ifndef OCALITENETWORKADDRESS_H
#define OCALITENETWORKADDRESS_H

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
 * Identifier that specifies the network address.
 */
class OcaLiteNetworkAddress : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteNetworkAddress();

    /**
     * Constructor.
     *
     * @param[in]   value       The value.
     */
    explicit OcaLiteNetworkAddress(const ::OcaLiteBlob& value);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteNetworkAddress(const ::OcaLiteNetworkAddress& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteNetworkAddress();

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
    ::OcaLiteNetworkAddress& operator=(const ::OcaLiteNetworkAddress& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteNetworkAddress& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteNetworkAddress& rhs) const;

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

#endif // OCALITENETWORKADDRESS_H
