/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Ocp1LiteWriter
 *
 */

#ifndef OCP1LITEWRITER_H
#define OCP1LITEWRITER_H

// ---- Include system wide include files ----
#include <OCF/IOcaLiteWriter.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Specific writer to write data to be sent over an OCP.1 network.
 */
class Ocp1LiteWriter : public ::IOcaLiteWriter
{
public:
    /**
     * Constructor
     */
    Ocp1LiteWriter() : ::IOcaLiteWriter() {}
    /**
     * Destructor.
     */
    virtual ~Ocp1LiteWriter() {}

    // ---- Interface methods ----
    virtual void Write(::OcaBoolean value, ::OcaUint8** destination) const;

    virtual void Write(::OcaInt8 value, ::OcaUint8** destination) const;

    virtual void Write(::OcaInt16 value, ::OcaUint8** destination) const;

    virtual void Write(::OcaInt32 value, ::OcaUint8** destination) const;

    virtual void Write(::OcaInt64 value, ::OcaUint8** destination) const;

    virtual void Write(::OcaUint8 value, ::OcaUint8** destination) const;

    virtual void Write(::OcaUint16 value, ::OcaUint8** destination) const;

    virtual void Write(::OcaUint32 value, ::OcaUint8** destination) const;

    virtual void Write(::OcaUint64 value, ::OcaUint8** destination) const;

    virtual void Write(::OcaFloat32 value, ::OcaUint8** destination) const;

    virtual void Write(const ::OcaUint8* values, ::OcaUint32 nrOfBytes, ::OcaUint8** destination) const;

    virtual ::OcaUint32 GetSize(::OcaBoolean value) const;

    virtual ::OcaUint32 GetSize(::OcaInt8 value) const;

    virtual ::OcaUint32 GetSize(::OcaInt16 value) const;

    virtual ::OcaUint32 GetSize(::OcaInt32 value) const;

    virtual ::OcaUint32 GetSize(::OcaInt64 value) const;

    virtual ::OcaUint32 GetSize(::OcaUint8 value) const;

    virtual ::OcaUint32 GetSize(::OcaUint16 value) const;

    virtual ::OcaUint32 GetSize(::OcaUint32 value) const;

    virtual ::OcaUint32 GetSize(::OcaUint64 value) const;

    virtual ::OcaUint32 GetSize(::OcaFloat32 value) const;

    virtual ::OcaUint32 GetSize(const ::OcaUint8* values, ::OcaUint32 nrOfBytes) const;

private:
    /**
     * Copy Constructor
     */
    Ocp1LiteWriter(const ::Ocp1LiteWriter&);

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::Ocp1LiteWriter& operator=(const ::Ocp1LiteWriter& source);
};

#endif /* OCP1LITEWRITER_H */
