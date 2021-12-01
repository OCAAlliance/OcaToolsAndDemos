/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : The IOcaLiteWriter interface.
 *
 */

#ifndef IOCALITEWRITER_H
#define IOCALITEWRITER_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteBaseDataTypes.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----
class OcaLiteString;

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Interface that defines a writer for primitive types to an OcaUint8 array.
 */
class IOcaLiteWriter
{
public:
    /**
     * Destructor.
     */
    virtual ~IOcaLiteWriter() {}

    // ---- Interface methods ----

    /**
     * Writes an OcaBoolean to the passed OcaUint8 array.
     *
     * @param[in]     value         Value to write to the array.
     * @param[in,out] destination   Pointer to array to write the value in.
     *                              Pointer is incremented with the number
     *                              of bytes written.
     */
    virtual void Write(::OcaBoolean value, ::OcaUint8** destination) const = 0;

    /**
     * Writes an OcaInt8 to the passed OcaUint8 array.
     *
     * @param[in]     value         Value to write to the array.
     * @param[in,out] destination   Pointer to array to write the value in.
     *                              Pointer is incremented with the number
     *                              of bytes written.
     */
    virtual void Write(::OcaInt8 value, ::OcaUint8** destination) const = 0;

    /**
     * Writes an OcaInt16 to the passed OcaUint8 array.
     *
     * @param[in]     value         Value to write to the array.
     * @param[in,out] destination   Pointer to array to write the value in.
     *                              Pointer is incremented with the number
     *                              of bytes written.
     */
    virtual void Write(::OcaInt16 value, ::OcaUint8** destination) const = 0;

    /**
     * Writes an OcaInt32 to the passed OcaUint8 array.
     *
     * @param[in]     value         Value to write to the array.
     * @param[in,out] destination   Pointer to array to write the value in.
     *                              Pointer is incremented with the number
     *                              of bytes written.
     */
    virtual void Write(::OcaInt32 value, ::OcaUint8** destination) const = 0;

    /**
     * Writes an OcaInt64 to the passed OcaUint8 array.
     *
     * @param[in]     value         Value to write to the array.
     * @param[in,out] destination   Pointer to array to write the value in.
     *                              Pointer is incremented with the number
     *                              of bytes written.
     */
    virtual void Write(::OcaInt64 value, ::OcaUint8** destination) const = 0;

    /**
     * Writes an OcaUint8 to the passed OcaUint8 array.
     *
     * @param[in]     value         Value to write to the array.
     * @param[in,out] destination   Pointer to array to write the value in.
     *                              Pointer is incremented with the number
     *                              of bytes written.
     */
    virtual void Write(::OcaUint8 value, ::OcaUint8** destination) const = 0;

    /**
     * Writes an OcaUint16 to the passed OcaUint8 array.
     *
     * @param[in]     value         Value to write to the array.
     * @param[in,out] destination   Pointer to array to write the value in.
     *                              Pointer is incremented with the number
     *                              of bytes written.
     */
    virtual void Write(::OcaUint16 value, ::OcaUint8** destination) const = 0;

    /**
     * Writes an OcaUint32 to the passed OcaUint8 array.
     *
     * @param[in]     value         Value to write to the array.
     * @param[in,out] destination   Pointer to array to write the value in.
     *                              Pointer is incremented with the number
     *                              of bytes written.
     */
    virtual void Write(::OcaUint32 value, ::OcaUint8** destination) const = 0;

    /**
     * Writes an OcaUint64 to the passed OcaUint8 array.
     *
     * @param[in]     value         Value to write to the array.
     * @param[in,out] destination   Pointer to array to write the value in.
     *                              Pointer is incremented with the number
     *                              of bytes written.
     */
    virtual void Write(::OcaUint64 value, ::OcaUint8** destination) const = 0;

    /**
     * Writes an OcaFloat32 to the passed OcaUint8 array.
     *
     * @param[in]     value         Value to write to the array.
     * @param[in,out] destination   Pointer to array to write the value in.
     *                              Pointer is incremented with the number
     *                              of bytes written.
     */
    virtual void Write(::OcaFloat32 value, ::OcaUint8** destination) const = 0;

    /**
     * Writes an array of OcaUint8 to the passed OcaUint8 array.
     *
     * @param[in]     values        Pointer to an OcaUint8 buffer to write to the array.
     * @param[in]     nrOfBytes     Number of OcaUint8 in the buffer.
     * @param[in,out] destination   Pointer to array to write the value in.
     *                              Pointer is incremented with the number
     *                              of bytes written.
     */
    virtual void Write(const ::OcaUint8* values, ::OcaUint32 nrOfBytes, ::OcaUint8** destination) const = 0;

    /**
     * Gets the size of an OcaBoolean when marshalled with this writer.
     *
     * @param[in]     value         The value to get the size of.
     */
    virtual ::OcaUint32 GetSize(::OcaBoolean value) const = 0;

    /**
     * Gets the size of an OcaInt8 when marshalled with this writer.
     *
     * @param[in]     value         The value to get the size of.
     */
    virtual ::OcaUint32 GetSize(::OcaInt8 value) const = 0;

    /**
     * Gets the size of an OcaInt16 when marshalled with this writer.
     *
     * @param[in]     value         The value to get the size of.
     */
    virtual ::OcaUint32 GetSize(::OcaInt16 value) const = 0;

    /**
     * Gets the size of an OcaInt32 when marshalled with this writer.
     *
     * @param[in]     value         The value to get the size of.
     */
    virtual ::OcaUint32 GetSize(::OcaInt32 value) const = 0;

    /**
     * Gets the size of an OcaInt64 when marshalled with this writer.
     *
     * @param[in]     value         The value to get the size of.
     */
    virtual ::OcaUint32 GetSize(::OcaInt64 value) const = 0;

    /**
     * Gets the size of an OcaUint8 when marshalled with this writer.
     *
     * @param[in]     value         The value to get the size of.
     */
    virtual ::OcaUint32 GetSize(::OcaUint8 value) const = 0;

    /**
     * Gets the size of an OcaUint16 when marshalled with this writer.
     *
     * @param[in]     value         The value to get the size of.
     */
    virtual ::OcaUint32 GetSize(::OcaUint16 value) const = 0;

    /**
     * Gets the size of an OcaUint32 when marshalled with this writer.
     *
     * @param[in]     value         The value to get the size of.
     */
    virtual ::OcaUint32 GetSize(::OcaUint32 value) const = 0;

    /**
     * Gets the size of an OcaUint64 when marshalled with this writer.
     *
     * @param[in]     value         The value to get the size of.
     */
    virtual ::OcaUint32 GetSize(::OcaUint64 value) const = 0;

    /**
     * Gets the size of an OcaFloat64 when marshalled with this writer.
     *
     * @param[in]     value         The value to get the size of.
     */
    virtual ::OcaUint32 GetSize(::OcaFloat32 value) const = 0;

    /**
     * Gets the size of an array of OcaUint8 when marshalled with this writer.
     *
     * @param[in]     values        Pointer to an OcaUint8 buffer to get the marshalled size of.
     * @param[in]     nrOfBytes     Number of OcaUint8 in the buffer.
     */
    virtual ::OcaUint32 GetSize(const ::OcaUint8* values, ::OcaUint32 nrOfBytes) const = 0;

protected:
    /**
     * Constructor
     */
    IOcaLiteWriter() {}

private:
    /**
     * Copy Constructor
     */
    IOcaLiteWriter(const ::IOcaLiteWriter&);

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::IOcaLiteWriter& operator=(const ::IOcaLiteWriter& source);
};

#endif // IOCALITEWRITER_H
