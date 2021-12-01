/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteBlobFixedLen
 *
 */

#ifndef OCALITEBLOBFIXEDLEN_H
#define OCALITEBLOBFIXEDLEN_H

// ---- Include system wide include files ----
#include <assert.h>

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteBaseDataTypes.h"
#include "OcaLiteString.h"
#include "OcaLiteTemplateHelpers.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Representation of a binary large object with a fixed-length.
 * @tparam  Len         The length of the BLOB in bytes.
 */
template < ::OcaUint16 Len>
class OcaLiteBlobFixedLen : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteBlobFixedLen()
        : ::IOcaLiteMarshal()
    {
        ::memset(m_value, 0, static_cast<size_t>(Len));
    }

    /**
     * Constructor.
     *
     * @param[in]   value       The BLOB data.
     */
    explicit OcaLiteBlobFixedLen(const ::OcaUint8 value[Len])
        : ::IOcaLiteMarshal()
    {
        assert(NULL != value);
        ::memcpy(m_value, value, static_cast<size_t>(Len));
    }

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteBlobFixedLen(const ::OcaLiteBlobFixedLen<Len>& source)
        : ::IOcaLiteMarshal(source)
    {
        ::memcpy(m_value, source.m_value, static_cast<size_t>(Len));
    }

    /**
     * Destructor.
     */
    virtual ~OcaLiteBlobFixedLen()
    {
    }

    /**
     * Getter for the BLOB data.
     *
     * @return The BLOB data.
     */
    const ::OcaUint8* GetValue() const
    {
        return m_value;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteBlobFixedLen<Len>& operator=(const ::OcaLiteBlobFixedLen<Len>& source)
    {
        if (this != &source)
        {
            ::memcpy(m_value, source.m_value, static_cast<size_t>(Len));
        }

        return *this;
    }

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteBlobFixedLen<Len>& rhs) const
    {
        bool result(0 == ::memcmp(m_value, rhs.m_value, static_cast<size_t>(Len)));

        return result;
    }

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteBlobFixedLen<Len>& rhs) const
    {
        return !(operator==(rhs));
    }

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const
    {
        writer.Write(m_value, static_cast< ::OcaUint32>(Len), destination);
    }

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
    {
        bool result(reader.Read(bytesLeft, source, static_cast< ::OcaUint32>(Len), m_value));

        // Clear all unmarshalled data if unmarshalling failed
        if (!result)
        {
            ::memset(m_value, 0, static_cast<size_t>(Len));
        }

        return result;
    }

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const
    {
        ::OcaUint32 length(writer.GetSize(m_value, static_cast< ::OcaUint32>(Len)));

        return length;
    }

private:
    /** The BLOB data. */
    ::OcaUint8      m_value[Len];
};

#endif // OCALITEBLOBFIXEDLEN_H
