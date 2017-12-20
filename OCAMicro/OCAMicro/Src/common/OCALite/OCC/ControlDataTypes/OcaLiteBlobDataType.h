/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteBlobDataType
 *
 */

#ifndef OcaLiteBlobDataType_H
#define OcaLiteBlobDataType_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteBlob.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/** ApplicationNetworkServiceID is defined as a blob */
#define OcaLiteApplicationNetworkServiceID  OcaLiteBlobDataType

/** NetworkSignalChannelID is defined as a blob */
#define OcaLiteNetworkSignalChannelID       OcaLiteBlobDataType

/** Network NodeID is defined as a blob */
#define OcaLiteNetworkNodeID                OcaLiteBlobDataType

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Datatype which only embeds a blob. Mostlikely this class will be deribed from which includes actual data in the blob
 */
class OcaLiteBlobDataType : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteBlobDataType();

    /**
     * Constructor.
     *
     * @param[in]   value       The value.
     */
    explicit OcaLiteBlobDataType(const ::OcaLiteBlob& value);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteBlobDataType(const ::OcaLiteBlobDataType& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteBlobDataType();

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
    ::OcaLiteBlobDataType& operator=(const ::OcaLiteBlobDataType& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteBlobDataType& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteBlobDataType& rhs) const;

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

#endif // OcaLiteBlobDataType_H
