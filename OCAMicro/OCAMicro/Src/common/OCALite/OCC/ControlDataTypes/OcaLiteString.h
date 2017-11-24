/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteString
 *
 */

#ifndef OCALITESTRING_H
#define OCALITESTRING_H

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteBaseDataTypes.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * General character string, UTF-8 encoded.
 * @ingroup DataTypes
 */
class OcaLiteString : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteString();

    /**
     * Constructor.
     * This constructor verifies if the passed string is actually
     * UTF-8 encoded. Invalid characters in the string will be replaced
     * by 0x7
     *
     * @param[in]   s           The string as a null-terminated array of UTF-8 encoded bytes.
     */
    explicit OcaLiteString(const ::OcaUint8* s);

    /**
     * Constructor.
     * This constructor verifies if the passed string is actually
     * UTF-8 encoded. Invalid characters in the string will be replaced
     * by 0x7
     *
     * @param[in]   s           The string as a null-terminated array of UTF-8 encoded bytes.
     */
    explicit OcaLiteString(const char* s);

    /**
     * Constructor.
     * This constructor verifies if the passed string is actually
     * UTF-8 encoded. Invalid characters in the string will be replaced
     * by 0x7
     *
     * @param[in]   s           The string as an STL string.
     */
    explicit OcaLiteString(const std::string& s);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteString(const ::OcaLiteString& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteString();

    /**
     * Getter for Length.
     *
     * @return The length of the string.
     */
    ::OcaUint16 GetLength() const;

    /**
     * Getter for string as a null-terminated array of UTF-8 encoded bytes.
     *
     * @return The string as a null-terminated array of UTF-8 encoded bytes.
     */
    const ::OcaUint8* GetValue() const;

    /**
     * Getter for string as an STL string.
     *
     * @return The string as an STL string.
     */
    const std::string& GetString() const
    {
        return m_string;
    }

    /**
     * Getter for Valid.
     *
     * @return Indicates if the passed string was valid when initialized (before any replacements).
     */
    ::OcaBoolean IsValid() const
    {
        return m_bValid;
    }

    /**
     * Concatenation operator.
     *
     * @param[in]   s           The string to concatenate.
     */
    ::OcaLiteString operator+(const ::OcaLiteString& s);

    /**
     * Concatenation operator.
     *
     * @param[in]   s           The string to concatenate.
     */
    ::OcaLiteString& operator+=(const ::OcaLiteString& s);

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteString& operator=(const ::OcaLiteString& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteString& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteString& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /**
     * Verifies the string contents and corrects the string if necessary.
     */
    void VerifyString();

    /** The UTF-8 encoded string as a null-terminated array of bytes. */
    std::string                 m_string;

    /** Indicates if the initialized string was valid (before the invalid characters were replaced). */
    ::OcaBoolean                m_bValid;
};

#endif // OCALITESTRING_H
