/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The IOcaLiteReader interface.
 *
 */

#ifndef IOCALITEREADER_H
#define IOCALITEREADER_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteBaseDataTypes.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----
class OcaLiteString;

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Interface that defines a reader for primitive types from an OcaUint8 array.
 */
class IOcaLiteReader
{
public:
    /**
     * Destructor.
     */
    virtual ~IOcaLiteReader() {}

    // ---- Interface methods ----

    /**
     * Reads an OcaBoolean from the passed OcaUint8 array.
     *
     * @param[in,out] bytesLeft Number of bytes left in the array from which the
     *                          value is read. Decremented with the number of
     *                          bytes read.
     * @param[in,out] source    Pointer to input array from which the value is
     *                          read. Pointer is incremented with the number
     *                          of bytes read.
     * @param[out]    value     The value of the OcaBoolean.
     * @return true for a successful read, false otherwise.
     */
    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaBoolean& value) const = 0;

    /**
     * Reads an OcaInt8 from the passed OcaUint8 array.
     *
     * @param[in,out] bytesLeft Number of bytes left in the array from which the
     *                          value is read. Decremented with the number of
     *                          bytes read.
     * @param[in,out] source    Pointer to input array from which the value is
     *                          read. Pointer is incremented with the number
     *                          of bytes read.
     * @param[out]    value     The value of the OcaInt8.
     * @return true for a successful read, false otherwise.
     */
    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaInt8& value) const = 0;

    /**
     * Reads an OcaInt16 from the passed OcaUint8 array.
     *
     * @param[in,out] bytesLeft Number of bytes left in the array from which the
     *                          value is read. Decremented with the number of
     *                          bytes read.
     * @param[in,out] source    Pointer to input array from which the value is
     *                          read. Pointer is incremented with the number
     *                          of bytes read.
     * @param[out]    value     The value of the OcaInt16.
     * @return true for a successful read, false otherwise.
     */
    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaInt16& value) const = 0;

    /**
     * Reads an OcaInt32 from the passed OcaUint8 array.
     *
     * @param[in,out] bytesLeft Number of bytes left in the array from which the
     *                          value is read. Decremented with the number of
     *                          bytes read.
     * @param[in,out] source    Pointer to input array from which the value is
     *                          read. Pointer is incremented with the number
     *                          of bytes read.
     * @param[out]    value     The value of the OcaInt32.
     * @return true for a successful read, false otherwise.
     */
    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaInt32& value) const = 0;

    /**
     * Reads an OcaInt64 from the passed OcaUint8 array.
     *
     * @param[in,out] bytesLeft Number of bytes left in the array from which the
     *                          value is read. Decremented with the number of
     *                          bytes read.
     * @param[in,out] source    Pointer to input array from which the value is
     *                          read. Pointer is incremented with the number
     *                          of bytes read.
     * @param[out]    value     The value of the OcaInt64.
     * @return true for a successful read, false otherwise.
     */
    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaInt64& value) const = 0;

    /**
     * Reads an OcaUint8 from the passed OcaUint8 array.
     *
     * @param[in,out] bytesLeft Number of bytes left in the array from which the
     *                          value is read. Decremented with the number of
     *                          bytes read.
     * @param[in,out] source    Pointer to input array from which the value is
     *                          read. Pointer is incremented with the number
     *                          of bytes read.
     * @param[out]    value     The value of the byte.
     * @return true for a successful read, false otherwise.
     */
    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaUint8& value) const = 0;

    /**
     * Reads an OcaUint16 from the passed OcaUint8 array.
     *
     * @param[in,out] bytesLeft Number of bytes left in the array from which the
     *                          value is read. Decremented with the number of
     *                          bytes read.
     * @param[in,out] source    Pointer to input array from which the value is
     *                          read. Pointer is incremented with the number
     *                          of bytes read.
     * @param[out]    value     The value of the OcaUint16.
     * @return true for a successful read, false otherwise.
     */
    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaUint16& value) const = 0;

    /**
     * Reads an OcaUint32 from the passed OcaUint8 array.
     *
     * @param[in,out] bytesLeft Number of bytes left in the array from which the
     *                          value is read. Decremented with the number of
     *                          bytes read.
     * @param[in,out] source    Pointer to input array from which the value is
     *                          read. Pointer is incremented with the number
     *                          of bytes read.
     * @param[out]    value     The value of the OcaUint32.
     * @return true for a successful read, false otherwise.
     */
    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaUint32& value) const = 0;

    /**
     * Reads an OcaUint64 from the passed OcaUint8 array.
     *
     * @param[in,out] bytesLeft Number of bytes left in the array from which the
     *                          value is read. Decremented with the number of
     *                          bytes read.
     * @param[in,out] source    Pointer to input array from which the value is
     *                          read. Pointer is incremented with the number
     *                          of bytes read.
     * @param[out]    value     The value of the OcaUint64.
     * @return true for a successful read, false otherwise.
     */
    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaUint64& value) const = 0;

    /**
     * Reads an OcaFloat32 from the passed OcaUint8 array.
     *
     * @param[in,out] bytesLeft Number of bytes left in the array from which the
     *                          value is read. Decremented with the number of
     *                          bytes read.
     * @param[in,out] source    Pointer to input array from which the value is
     *                          read. Pointer is incremented with the number
     *                          of bytes read.
     * @param[out]    value     The value of the OcaUint64.
     * @return true for a successful read, false otherwise.
     */
    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaFloat32& value) const = 0;

    /**
     * Reads an array of OcaUint8 from the passed OcaUint8 array.
     *
     * @param[in,out] bytesLeft Number of bytes left in the array from which the
     *                          value is read. Decremented with the number of
     *                          bytes read.
     * @param[in,out] source    Pointer to input array from which the value is
     *                          read. Pointer is incremented with the number
     *                          of bytes read.
     * @param[in]     nrOfBytes Number of bytes to read.
     * @param[out]    values    Pointer to buffer where to store the read data.
     * @return true for a successful read, false otherwise.
     */
    virtual bool Read(::OcaUint32& bytesLeft, const ::OcaUint8** source, ::OcaUint32 nrOfBytes, ::OcaUint8* values) const = 0;

protected:
    /**
     * Constructor
     */
    IOcaLiteReader() {}

private:
    /**
     * Private Copy Constructor to prevent copying
     */
    IOcaLiteReader(const ::IOcaLiteReader&);

    /**
     * Private Assignment operator to prevent assignment.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::IOcaLiteReader& operator=(const ::IOcaLiteReader& source);
};

#endif // IOCALITEREADER_H
