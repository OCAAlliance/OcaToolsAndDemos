/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : OcaLiteClassID
 *
 */

#ifndef OCALITECLASSID_H
#define OCALITECLASSID_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteFrameworkDataTypes.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

#ifndef OCA_CLASS_ID_SIZE
/** The maximum depth of the class ID */
#define OCA_CLASS_ID_SIZE 8
#endif //OCA_CLASS_ID_SIZE

/** Class ID field mask to identify a proprietary class. */
#define OCA_CLASS_ID_PROPRIETARY_CLASS_FIELD_MASK       static_cast< ::OcaUint16>(0x8000)

/** Class ID field mask to identify a proprietary test class. */
#define OCA_CLASS_ID_PROPRIETARY_TEST_CLASS_FIELD_MASK  static_cast< ::OcaUint16>(0xff00)

/** Class ID field to identify a proprietary class (as of OCA 1.4). */
#define OCA_CLASS_ID_PROPRIETARY_CLASS_FIELD            static_cast< ::OcaUint16>(0xffff)

/** The CID (issued by the IEEE Registration Authority) for proprietary classes defined by the OCA Alliance. */
#define OCA_ALLIANCE_COMPANY_ID                         0xFA2EE9

/** The manufacturer ID in a class ID array to indicate a proprietary class. */
#define OCA_CLASS_ID_PROPRIETARY_MANUFACTURER_ID(oui)   OCA_CLASS_ID_PROPRIETARY_CLASS_FIELD,static_cast< ::OcaUint16>((oui >> 16) & 0x00ff),static_cast< ::OcaUint16>(oui & 0xffff)

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Class identifier : structured variable of the form a.b.c...
 */
class OcaLiteClassID : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteClassID();

    /**
     * Constructor.
     *
     * @param[in]   fieldCount  Identifies the number of fields of the ClassID, i.e. the level of the class
     *                          tree the class is in. For example if this field count is 3, the ClassID is of
     *                          the form 'n.m.p'. The minimum field count is 1 (OcaRoot class). A field count
     *                          of zero is invalid.
     * @param[in]   fields      Array of actual fields of the ClassID. The array has 'fieldCount' entries. If
     *                          for example the field count is 3 and the Fields array has value {1, 2, 3} the
     *                          ClassID is '1.2.3'.
     */
    OcaLiteClassID(::OcaUint16 fieldCount, const ::OcaUint16* fields);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteClassID(const ::OcaLiteClassID& source);

#ifdef _WIN32
    /**
     * Constructor, create a class id from a string
     *
     * @param[in] source    The string to create the classId from.
     */
    OcaLiteClassID(const ::std::string& source);
#endif

    /**
     * Destructor.
     */
    virtual ~OcaLiteClassID();

    /**
     * Retrieves the class ID of the parent class.
     *
     * @param[out]  parent      The class ID of the parent class.
     * @return True if there is a parent, false otherwise.
     */
    bool GetParent(::OcaLiteClassID& parent) const;

    /**
     * Getter for the number of 16-bit fields of the ClassID.
     *
     * @return The number of 16-bit fields of the ClassID.
     */
    ::OcaUint16 GetFieldCount() const
    {
        return m_fieldCount;
    }

    /**
     * Getter for the Fields.
     *
     * @return The Fields.
     */
    const ::OcaUint16* GetFields() const
    {
        return m_fields;
    }

    /**
     * Getter for the definition level of the class identified by this ClassID.
     *
     * @return The definition level of the class identified by this ClassID.
     */
    ::OcaUint16 GetDefLevel() const
    {
        return m_defLevel;
    }

    /**
     * Indicates if this ClassID is valid (complete).
     *
     * @return True if the ClassID is valid, false otherwise.
     */
    ::OcaBoolean IsValid() const;

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteClassID& operator=(const ::OcaLiteClassID& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteClassID& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteClassID& rhs) const;

    /**
     * The less-than operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if this object is less than rhs.
     */
    bool operator<(const ::OcaLiteClassID& rhs) const;

    /**
     * The less-than-or-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if this object is less than or equal to rhs.
     */
    bool operator<=(const ::OcaLiteClassID& rhs) const;

    /**
     * The greater-than operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if this object is greater than rhs.
     */
    bool operator>(const ::OcaLiteClassID& rhs) const;

    /**
     * The greater-than-or-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if this object is greater than or equal to rhs.
     */
    bool operator>=(const ::OcaLiteClassID& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /**
     * Indicates if the given fields will result in a correct class ID.
     *
     * @param[in]   fieldCount      Identifies the number of fields of the ClassID.
     * @param[in]   fields          Array of actual fields of the ClassID. The array has 'fieldCount' entries.
     * @return True if the given fields will result in a correct class ID, false otherwise.
     */
    static bool AreFieldsValid(::OcaUint16 fieldCount, const ::OcaUint16* fields);

    /**
     * Determines the definition level of the class identified by this ClassID.
     *
     * @param[in]   fieldCount      Identifies the number of fields of the ClassID.
     * @param[in]   fields          Array of actual fields of the ClassID. The array has 'fieldCount' entries.
     * @return The definition level of the class identified by this ClassID.
     */
    static ::OcaUint16 DetermineDefLevel(::OcaUint16 fieldCount, const ::OcaUint16* fields);

    /** The number of fields of the ClassID. */
    ::OcaUint16     m_fieldCount;

    /** The fields. */
    ::OcaUint16     m_fields[OCA_CLASS_ID_SIZE];

    /** The definition level of the class identified by this ClassID. */
    ::OcaUint16     m_defLevel;
};

#endif // OCALITECLASSID_H
