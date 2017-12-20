/*
*  By downloading or using this file, the user agrees to be bound by the terms of the license
*  agreement located at http://ocaalliance.com/EULA as an original contracting party.
*
*  Description         : OcaLiteMediaCoding
*
*/

#ifndef OCALITEMEDIACODING_H
#define OCALITEMEDIACODING_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "IOcaLiteMarshal.h"
#include "OcaLiteFrameworkDataTypes.h"
#include "OcaLiteMediaStreamParameters.h"
#include "OcaLiteNetworkDataTypes.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Codec ID + Coding parameters
 */
class OcaLiteMediaCoding : public ::IOcaLiteMarshal
{
public:
    /** Default constructor */
    OcaLiteMediaCoding();
    
    /*
     * Constructor 
     * @param[in] codingSchemeID    ID of coding scheme to use.
     * @param[in] codecParameters   Coding parameters.  Content is coding-scheme-dependent.
     * @param[in] clockONo          Object number of OcaMediaClock3 object to use for this coding scheme. May be zero if no OcaMediaClock3 object is used. 
     */
    OcaLiteMediaCoding(::OcaLiteMediaCodingSchemeID codingSchemeID, const ::OcaLiteString& codecParameters, ::OcaONo clockONo);

    /** 
     * Copy constructor
     */
    OcaLiteMediaCoding(const ::OcaLiteMediaCoding& source);

    /**
     * Destructor.
     */
    virtual ~OcaLiteMediaCoding();


    /** 
     * Get the ID of coding scheme to use.   
     *
     * @return the coding scheme ID.
     */
    ::OcaLiteMediaCodingSchemeID GetCodingSchemeID() const
    {
        return m_codingSchemeID;
    }

    /** 
     * Get the coding parameters.  Content is coding-scheme-dependent. 
     *
     * @return the coding parameters.
     */
    ::OcaLiteString GetCodecParameters() const
    {
        return m_codecParameters;
    }

    /** 
     * Get the Object number of OcaMediaClock3 object to use for this coding scheme. 
     *
     * @return The object number.
     */
    ::OcaONo GetClockONo() const
    {
        return m_clockONo;
    }

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::OcaLiteMediaCoding& operator=(const ::OcaLiteMediaCoding& source);

    /**
     * The is-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are equal.
     */
    virtual bool operator==(const ::OcaLiteMediaCoding& rhs) const;

    /**
     * The is-not-equal operator.
     *
     * @param[in]   rhs         The object to compare with.
     * @return True if the classes are not equal.
     */
    bool operator!=(const ::OcaLiteMediaCoding& rhs) const;

    // ---- IOcaLiteMarshal methods ----

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** ID of coding scheme to use.   */
    ::OcaLiteMediaCodingSchemeID    m_codingSchemeID;
    /** Coding parameters.  Content is coding-scheme-dependent. */
    ::OcaLiteString                 m_codecParameters;
    /** Object number of OcaMediaClock3 object to use for this coding scheme.  May be zero if no OcaMediaClock3 object is used. */
    ::OcaONo                        m_clockONo;
};

#endif //OCALITEMEDIACODING_H
