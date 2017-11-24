/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteBlob
 *
 */

#ifndef OCALITEBLOB_H
#define OCALITEBLOB_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteBaseDataTypes.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
#ifndef MAX_NR_OF_BYTES_IN_BLOB
#define MAX_NR_OF_BYTES_IN_BLOB 128
#endif

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Representation of a binary large object that is used on the network to send large chunks of binary data.
 * @ingroup DataTypes
 */
class OcaLiteBlob : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteBlob();

    /**
     * Constructor.
     *
     * @param[in]   dataSize    The size of the BLOB data in bytes.
     * @param[in]   data        The BLOB data.
     */
    OcaLiteBlob(::OcaUint16 dataSize, const ::OcaUint8* data);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteBlob(const ::OcaLiteBlob& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteBlob();

    /**
     * Getter for DataSize.
     *
     * @return The size of the BLOB data in bytes.
     */
    ::OcaUint16 GetDataSize() const
    {
        return m_dataSize;
    }

    /**
     * Getter for the BLOB data.
     *
     * @return The BLOB data.
     */
    const ::OcaUint8* GetData() const
    {
        return m_data;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteBlob& operator=(const ::OcaLiteBlob& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteBlob& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteBlob& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The size of the BLOB data in bytes. */
    ::OcaUint16     m_dataSize;

    /** The BLOB data. */
    ::OcaUint8      m_data[MAX_NR_OF_BYTES_IN_BLOB];
};

#endif // OCALITEBLOB_H
