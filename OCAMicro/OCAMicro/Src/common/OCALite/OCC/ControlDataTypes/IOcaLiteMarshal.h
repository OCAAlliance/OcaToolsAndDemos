/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The IOcaLiteMarshal interface.
 *
 */

#ifndef IOCALITEMARSHAL_H
#define IOCALITEMARSHAL_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "OcaLiteBaseDataTypes.h"

// ---- Referenced classes and types ----
class IOcaLiteWriter;
class IOcaLiteReader;

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Interface that defines the generic interface for all OCA data
 * types that need to be serialized (i.e. communicated between OCA
 * devices).
 */
class IOcaLiteMarshal
{
public:
    /**
     * Destructor
     */
    virtual ~IOcaLiteMarshal() {}

    // ---- Interface methods  ----
    /**
     * Marshals the object data. Serializes the data of the object into the
     * passed byte array using the passed writer.
     *
     * @param[in,out]   destination  Pointer to destination byte array.
     * @param[in]       writer       The writer that helps in serializing the object.
     */
    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const = 0;

    /**
     * Unmarshals the object data. Deserializes the data from the passed byte
     * array into the object using the passed reader.
     *
     * @param[in,out] bytesLeft Number of bytes left in the array from which the
     *                          object is deserialized. Decremented with the number of
     *                          bytes read.
     * @param[in,out] source    Pointer to the input byte array from which the object
     *                          is deserialized.
     * @param[in] reader        The reader that helps in deserializing the object.
     * @return true when the unmarshal was successful, false otherwise.
     */
    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader) = 0;

    /**
     * Returns the size (in bytes!) of the Marshalled object.
     * @note This size is the amount of bytes of the data of the Marshalled
     *       object, i.e. the amount of bytes needed for marshalling and
     *       unmarshalling using the given writer.
     *
     * @param[in]  writer       The writer that will be used to serialize the object.
     * @return  Size of the object
     */
    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const = 0;

protected:
    /**
     * Constructor
     */
    IOcaLiteMarshal() {}

    /**
     * Copy Constructor
     */
    IOcaLiteMarshal(const ::IOcaLiteMarshal&) {}
};

#endif // IOCALITEMARSHAL_H
