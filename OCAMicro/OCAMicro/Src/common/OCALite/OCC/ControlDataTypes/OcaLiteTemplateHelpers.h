/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Helper template functions for template data types
 *
 */

#ifndef OCALITETEMPLATEHELPERS_H
#define OCALITETEMPLATEHELPERS_H

// ---- Include system wide include files ----
#include <OCF/IOcaLiteReader.h>
#include <OCF/IOcaLiteWriter.h>

// ---- Include local include files ----
#include "OcaLiteString.h"

// ---- Referenced classes and types ----
class IOcaLiteReader;
class IOcaLiteWriter;

// ---- Helper types and constants ----

// ---- Helper functions ----

/**
 * Checks if two values are identical.
 *
 * @param[in]   lhs         The left hand part of the comparison.
 * @param[in]   rhs         The right hand part of the comparison.
 * @tparam      DataType    The data type of the elements to compare.
 * @return True if both values are identical, False otherwise.
 */
template <class DataType>
bool CompareValue(const DataType& lhs, const DataType& rhs)
{
    return (lhs == rhs);
}

/**
 * Returns the string representation of the given value.
 *
 * @param[in]   value       The value to get the string representation of.
 * @tparam      DataType    The data type to get the string representation of.
 * @return The string representation of the given value.
 */

template <class DataType>
::OcaLiteString ValueToString(const DataType& value)
{
    return value.ToString();
}

/**
 * Marshals the value data. Serializes the data of the value into the
 * passed byte array using the passed writer.
 *
 * @param[in]  value        The value that must be marshaled.
 * @param[out] destination  Pointer to destination byte array.
 * @param[in]  writer       The writer that helps in serializing the value.
 * @tparam     DataType     The data type to marshal.
 */

template <class DataType>
void MarshalValue(const DataType& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    value.Marshal(destination, writer);
}

/**
 * Unmarshals the value data. Deserializes the data from the passed byte
 * array into the value using the passed binary reader.
 *
 * @param[out]    value     The unmarshaled value.
 * @param[in,out] bytesLeft Number of bytes left in the array from which the
 *                          value is deserialized. Decremented with the number of
 *                          bytes read.
 * @param[in,out] source    Pointer to the input byte array from which the value
 *                          is deserialized.
 * @param[in]     reader    The binary reader that helps in deserializing the value.
 * @tparam     DataType     The data type to unmarshal.
 * @return true when the unmarshal was successful, false otherwise.
 */

template <class DataType>
bool UnmarshalValue(DataType& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    return value.Unmarshal(bytesLeft, source, reader);
}

/**
 * Returns the size (in bytes!) of the Marshalled value.
 * @note This size is the amount of bytes of the data of the Marshalled
 *       value, i.e. the amount of bytes needed for marshalling and
 *       unmarshalling using the given writer.
 *
 * @param[in]  value        The value to serialize.
 * @param[in]  writer       The writer that will be used to serialize the object.
 * @tparam     DataType     The data type to get the size of.
 * @return  Size of the value
 */

template <class DataType>
::OcaUint32 GetSizeValue(const DataType& value, const ::IOcaLiteWriter& writer)
{
    return value.GetSize(writer);
}

// ---- Class Definition ----

// ---- Specialized Template Function Definition ----

template <>
bool CompareValue< ::OcaFloat32>(const ::OcaFloat32& lhs, const ::OcaFloat32& rhs);

template <>
void MarshalValue< ::OcaBoolean>(const ::OcaBoolean& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaInt8>(const ::OcaInt8& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaInt16>(const ::OcaInt16& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaInt32>(const ::OcaInt32& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaInt64>(const ::OcaInt64& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaUint8>(const ::OcaUint8& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaUint16>(const ::OcaUint16& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaUint32>(const ::OcaUint32& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaUint64>(const ::OcaUint64& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
void MarshalValue< ::OcaFloat32>(const ::OcaFloat32& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::OcaBoolean>(::OcaBoolean& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaInt8>(::OcaInt8& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaInt16>(::OcaInt16& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaInt32>(::OcaInt32& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaInt64>(::OcaInt64& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaUint8>(::OcaUint8& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaUint16>(::OcaUint16& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaUint32>(::OcaUint32& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaUint64>(::OcaUint64& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
bool UnmarshalValue< ::OcaFloat32>(::OcaFloat32& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
::OcaUint32 GetSizeValue< ::OcaBoolean>(const ::OcaBoolean& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaInt8>(const ::OcaInt8& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaInt16>(const ::OcaInt16& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaInt32>(const ::OcaInt32& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaInt64>(const ::OcaInt64& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaUint8>(const ::OcaUint8& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaUint16>(const ::OcaUint16& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaUint32>(const ::OcaUint32& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaUint64>(const ::OcaUint64& value, const ::IOcaLiteWriter& writer);

template <>
::OcaUint32 GetSizeValue< ::OcaFloat32>(const ::OcaFloat32& value, const ::IOcaLiteWriter& writer);

template <>
::OcaLiteString ValueToString< ::OcaBoolean>(const ::OcaBoolean& value);

template <>
::OcaLiteString ValueToString< ::OcaInt8>(const ::OcaInt8& value);

template <>
::OcaLiteString ValueToString< ::OcaInt16>(const ::OcaInt16& value);

template <>
::OcaLiteString ValueToString< ::OcaInt32>(const ::OcaInt32& value);

template <>
::OcaLiteString ValueToString< ::OcaInt64>(const ::OcaInt64& value);

template <>
::OcaLiteString ValueToString< ::OcaUint8>(const ::OcaUint8& value);

template <>
::OcaLiteString ValueToString< ::OcaUint16>(const ::OcaUint16& value);

template <>
::OcaLiteString ValueToString< ::OcaUint32>(const ::OcaUint32& value);

template <>
::OcaLiteString ValueToString< ::OcaUint64>(const ::OcaUint64& value);

template <>
::OcaLiteString ValueToString< ::OcaFloat32>(const ::OcaFloat32& value);

template <>
::OcaLiteString ValueToString< ::OcaLiteString>(const ::OcaLiteString& value);

#endif // OCALITETEMPLATEHELPERS_H
