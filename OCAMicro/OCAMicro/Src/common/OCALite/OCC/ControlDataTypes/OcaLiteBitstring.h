/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteBitstring
 *
 */

#ifndef OCALITEBITSTRING_H
#define OCALITEBITSTRING_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteBaseDataTypes.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
#ifndef MAX_NR_OF_BYTES_IN_BITSTRING
#define MAX_NR_OF_BYTES_IN_BITSTRING 2
#endif
// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Representation of a bitmask that is used on the network to send bitmask data.
 * @ingroup DataTypes
 */
class OcaLiteBitstring : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteBitstring();

    /**
     * Constructor.
     *
     * @param[in]   nrBits      The size of the bitmask in bits.
     */
    explicit OcaLiteBitstring(::OcaUint16 nrBits);

    /**
     * Constructor.
     *
     * @param[in]   nrBits      The size of the bitmask in bits.
     * @param[in]   bitstring   The bitstring data as an array of bytes with the most significant
     *                          bit of the first byte being bit number 0.
     */
    OcaLiteBitstring(::OcaUint16 nrBits, const ::OcaUint8* bitstring);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteBitstring(const ::OcaLiteBitstring& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteBitstring();

    /**
     * Getter for NrBits.
     *
     * @return The size of the bitmask in bits.
     */
    ::OcaUint16 GetNrBits() const
    {
        return m_nrBits;
    }

    /**
     * Getter for the number of bytes in the bitmask.
     *
     * @return The size of the bitmask in bytes.
     */
    ::OcaUint16 GetNrBytes() const;

    /**
     * Gets the bit value of the given bit.
     *
     * @param[in]   bitNr   The bit to get the value of.
     * @return  The bit value of the given bit.
     */
    bool GetBit(::OcaUint16 bitNr) const;

    /**
     * Sets the bit value of the given bit.
     *
     * @param[in]   bitNr   The bit to set the value of.
     * @param[in]   value   The new value of the bit
     * @return  Indicates if successfully set
     */
    bool SetBit(::OcaUint16 bitNr, bool value);

    /**
     * Getter for bitstring as an array of bytes with the most significant bit
     * of the first byte being bit number 0. This will be NULL in case the NrBits is 0.
     *
     * @return The bitstring as an array of bytes.
     */
    const ::OcaUint8* GetBitstring() const
    {
        return m_bitstring;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteBitstring& operator=(const ::OcaLiteBitstring& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteBitstring& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteBitstring& rhs) const;

    // ---- IOcaMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The size of the bitmask in bits. */
    ::OcaUint16     m_nrBits;

    /** The bitstring data. */
    ::OcaUint8      m_bitstring[MAX_NR_OF_BYTES_IN_BITSTRING];
};

#endif // OCALITEBITSTRING_H
