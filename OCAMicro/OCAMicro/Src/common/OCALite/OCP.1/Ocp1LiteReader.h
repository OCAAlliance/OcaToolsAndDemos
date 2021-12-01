/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Ocp1LiteReader
 *
 */

#ifndef OCP1LITEREADER_H
#define OCP1LITEREADER_H

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Specific reader to read data sent over an OCP.1 network.
 */
class Ocp1LiteReader : public ::IOcaLiteReader
{
public:
    /**
     * Constructor
     */
    Ocp1LiteReader() : ::IOcaLiteReader() {}

    /**
     * Destructor.
     */
    virtual ~Ocp1LiteReader() {}

    // ---- Interface methods ----

    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaBoolean& value) const;

    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaInt8& value) const;

    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaInt16& value) const;

    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaInt32& value) const;

    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaInt64& value) const;

    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaUint8& value) const;

    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaUint16& value) const;

    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaUint32& value) const;

    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaUint64& value) const;

    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaFloat32& value) const;

    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaUint32 nrOfBytes, ::OcaUint8* values) const;

private:
    /**
     * Copy Constructor
     */
    Ocp1LiteReader(const ::Ocp1LiteReader&);

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::Ocp1LiteReader& operator=(const ::Ocp1LiteReader& source);
};

#endif /* OCP1LITEREADER_H */
