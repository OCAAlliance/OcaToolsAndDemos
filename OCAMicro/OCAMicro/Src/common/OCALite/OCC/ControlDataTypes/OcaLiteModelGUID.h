/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteModelGUID
 *
 */

#ifndef OCALITEMODELGUID_H
#define OCALITEMODELGUID_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteBlobFixedLen.h"
#include "OcaLiteManagementDataTypes.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
#define OCA_MODEL_GUID_RESERVED_LENGTH      static_cast< ::OcaUint16>(1)
#define OCA_MODEL_GUID_MFR_CODE_LENGTH      static_cast< ::OcaUint16>(3)
#define OCA_MODEL_GUID_MODEL_CODE_LENGTH    static_cast< ::OcaUint16>(4)

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * 64-bit device type GUID.
 */
class OcaLiteModelGUID : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteModelGUID();

    /**
     * Constructor.
     *
     * @param[in]   mfrCode         The manufacturer code. Unique worldwide.
     * @param[in]   modelCode       The model code. Unique within the given manufacturer's products.
     */
    OcaLiteModelGUID(const ::OcaLiteBlobFixedLen<OCA_MODEL_GUID_MFR_CODE_LENGTH>& mfrCode,
                 const ::OcaLiteBlobFixedLen<OCA_MODEL_GUID_MODEL_CODE_LENGTH>& modelCode);

    /**
     * Constructor.
     *
     * @param[in]   guid            The complete identifier as an unsigned 64-bit integer.
     */
    explicit OcaLiteModelGUID(::OcaUint64 guid);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteModelGUID(const ::OcaLiteModelGUID& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteModelGUID();

    /**
     * Getter for MfrCode.
     *
     * @return The manufacturer code.
     */
    const ::OcaLiteBlobFixedLen<OCA_MODEL_GUID_MFR_CODE_LENGTH>& GetMfrCode() const
    {
        return m_mfrCode;
    }

    /**
     * Getter for ModelCode.
     *
     * @return The model code.
     */
    const ::OcaLiteBlobFixedLen<OCA_MODEL_GUID_MODEL_CODE_LENGTH>& GetModelCode() const
    {
        return m_modelCode;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteModelGUID& operator=(const ::OcaLiteModelGUID& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteModelGUID& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteModelGUID& rhs) const;

    /**
     * Returns the unique identifier as a 64-bit integer.
     *
     * @return The unique identifier as a 64-bit integer.
     */
    ::OcaUint64 ToUint64() const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The reserved bits. */
    ::OcaLiteBlobFixedLen<OCA_MODEL_GUID_RESERVED_LENGTH>   m_reserved;

    /** The manufacturer code. */
    ::OcaLiteBlobFixedLen<OCA_MODEL_GUID_MFR_CODE_LENGTH>   m_mfrCode;

    /** The model code. */
    ::OcaLiteBlobFixedLen<OCA_MODEL_GUID_MODEL_CODE_LENGTH> m_modelCode;
};

#endif // OCALITEMODELGUID_H
