/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteBlockMember
 *
 */

#ifndef OCALITEBLOCKMEMBER_H
#define OCALITEBLOCKMEMBER_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteObjectIdentification.h"
#include "OcaLiteString.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Descibes an object that is a member of a block.
 */
class OcaLiteBlockMember : public ::IOcaLiteMarshal
{
public:
    /** Default constructor. */
    OcaLiteBlockMember();

    /**
     * Constructor.
     *
     * @param[in]   memberObjectIdentification  Object identification of the block member.
     * @param[in]   containerObjectNumber       Object number of the block that contains the member.
     */
    OcaLiteBlockMember(const ::OcaLiteObjectIdentification& memberObjectIdentification, ::OcaONo containerObjectNumber);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    OcaLiteBlockMember(const ::OcaLiteBlockMember& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteBlockMember();

    /**
     * Getter for MemberObjectIdentification.
     *
     * @return The object identification of the block member.
     */
    const ::OcaLiteObjectIdentification& GetMemberObjectIdentification() const
    {
        return m_memberObjectIdentification;
    }

    /**
     * Getter for ContainerObjectNumber.
     *
     * @return The object number of the block that contains the member.
     */
    ::OcaONo GetContainerObjectNumber() const
    {
        return m_containerObjectNumber;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     */
    ::OcaLiteBlockMember& operator=(const ::OcaLiteBlockMember& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    bool operator==(const ::OcaLiteBlockMember& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteBlockMember& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** The object identification of the block member. */
    ::OcaLiteObjectIdentification       m_memberObjectIdentification;

    /** The object number of the block that contains the member. */
    ::OcaONo                        m_containerObjectNumber;
};

#endif // OCALITEBLOCKMEMBER_H
