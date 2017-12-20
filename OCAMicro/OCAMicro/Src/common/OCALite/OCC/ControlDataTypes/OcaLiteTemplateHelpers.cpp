/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : Helper template functions for template data types
 *
 */

// ---- Include system wide include files ----
#include <math.h>
#include <string>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteTemplateHelpers.h"
#include "OcaLiteWorkerDataTypes.h"

// ---- Helper types and constants ----
// Make the Visual Studio compiler understand isnan and isinf
#ifdef _MSC_VER
#define isnan(x) _isnan(x)
#define isinf(x) (0 == _finite(x) && !_isnan(x))
#endif

// ---- Helper functions ----
extern "C" int CompareFloats(FLOAT firstValue, FLOAT secondValue, FLOAT delta)
{
    return ((isnan(firstValue) && isnan(secondValue)) ||
            (isinf(firstValue) && isinf(secondValue)) ||
            (fabs(firstValue - secondValue) < delta));
}

extern "C" int CompareDoubles(DOUBLE firstValue, DOUBLE secondValue, DOUBLE delta)
{
    return ((isnan(firstValue) && isnan(secondValue)) ||
            (isinf(firstValue) && isinf(secondValue)) ||
            (fabs(firstValue - secondValue) < delta));
}

// ---- Local data ----

// ---- Function Implementation ----

template <>
bool CompareValue< ::OcaFloat32>(const ::OcaFloat32& lhs, const ::OcaFloat32& rhs)
{
    return (0 != CompareFloats(static_cast<FLOAT>(lhs), static_cast<FLOAT>(rhs), FLOAT_COMPARE_ACCURACY));
}

template <>
void MarshalValue< ::OcaBoolean>(const ::OcaBoolean& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    writer.Write(value, destination);
}

template <>
void MarshalValue< ::OcaInt8>(const ::OcaInt8& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    writer.Write(value, destination);
}

template <>
void MarshalValue< ::OcaInt16>(const ::OcaInt16& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    writer.Write(value, destination);
}

template <>
void MarshalValue< ::OcaInt32>(const ::OcaInt32& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    writer.Write(value, destination);
}

template <>
void MarshalValue< ::OcaInt64>(const ::OcaInt64& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    writer.Write(value, destination);
}

template <>
void MarshalValue< ::OcaUint8>(const ::OcaUint8& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    writer.Write(value, destination);
}

template <>
void MarshalValue< ::OcaUint16>(const ::OcaUint16& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    writer.Write(value, destination);
}

template <>
void MarshalValue< ::OcaUint32>(const ::OcaUint32& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    writer.Write(value, destination);
}

template <>
void MarshalValue< ::OcaUint64>(const ::OcaUint64& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    writer.Write(value, destination);
}

template <>
void MarshalValue< ::OcaFloat32>(const ::OcaFloat32& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer)
{
    writer.Write(value, destination);
}

template <>
bool UnmarshalValue< ::OcaBoolean>(::OcaBoolean& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    return reader.Read(bytesLeft, source, value);
}

template <>
bool UnmarshalValue< ::OcaInt8>(::OcaInt8& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    return reader.Read(bytesLeft, source, value);
}

template <>
bool UnmarshalValue< ::OcaInt16>(::OcaInt16& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    return reader.Read(bytesLeft, source, value);
}

template <>
bool UnmarshalValue< ::OcaInt32>(::OcaInt32& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    return reader.Read(bytesLeft, source, value);
}

template <>
bool UnmarshalValue< ::OcaInt64>(::OcaInt64& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    return reader.Read(bytesLeft, source, value);
}

template <>
bool UnmarshalValue< ::OcaUint8>(::OcaUint8& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    return reader.Read(bytesLeft, source, value);
}

template <>
bool UnmarshalValue< ::OcaUint16>(::OcaUint16& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    return reader.Read(bytesLeft, source, value);
}

template <>
bool UnmarshalValue< ::OcaUint32>(::OcaUint32& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    return reader.Read(bytesLeft, source, value);
}

template <>
bool UnmarshalValue< ::OcaUint64>(::OcaUint64& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    return reader.Read(bytesLeft, source, value);
}

template <>
bool UnmarshalValue< ::OcaFloat32>(::OcaFloat32& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader)
{
    return reader.Read(bytesLeft, source, value);
}

template <>
::OcaUint32 GetSizeValue< ::OcaBoolean>(const ::OcaBoolean& value, const ::IOcaLiteWriter& writer)
{
    return writer.GetSize(value);
}

template <>
::OcaUint32 GetSizeValue< ::OcaInt8>(const ::OcaInt8& value, const ::IOcaLiteWriter& writer)
{
    return writer.GetSize(value);
}

template <>
::OcaUint32 GetSizeValue< ::OcaInt16>(const ::OcaInt16& value, const ::IOcaLiteWriter& writer)
{
    return writer.GetSize(value);
}

template <>
::OcaUint32 GetSizeValue< ::OcaInt32>(const ::OcaInt32& value, const ::IOcaLiteWriter& writer)
{
    return writer.GetSize(value);
}

template <>
::OcaUint32 GetSizeValue< ::OcaInt64>(const ::OcaInt64& value, const ::IOcaLiteWriter& writer)
{
    return writer.GetSize(value);
}

template <>
::OcaUint32 GetSizeValue< ::OcaUint8>(const ::OcaUint8& value, const ::IOcaLiteWriter& writer)
{
    return writer.GetSize(value);
}

template <>
::OcaUint32 GetSizeValue< ::OcaUint16>(const ::OcaUint16& value, const ::IOcaLiteWriter& writer)
{
    return writer.GetSize(value);
}

template <>
::OcaUint32 GetSizeValue< ::OcaUint32>(const ::OcaUint32& value, const ::IOcaLiteWriter& writer)
{
    return writer.GetSize(value);
}

template <>
::OcaUint32 GetSizeValue< ::OcaUint64>(const ::OcaUint64& value, const ::IOcaLiteWriter& writer)
{
    return writer.GetSize(value);
}

template <>
::OcaUint32 GetSizeValue< ::OcaFloat32>(const ::OcaFloat32& value, const ::IOcaLiteWriter& writer)
{
    return writer.GetSize(value);
}

template <>
::OcaLiteString ValueToString< ::OcaBoolean>(const ::OcaBoolean& value)
{
    if (value)
    {
        return ::OcaLiteString("true");
    }
    else
    {
        return ::OcaLiteString("false");
    }
}

template <>
::OcaLiteString ValueToString< ::OcaInt8>(const ::OcaInt8& value)
{
    char numberString[5];
    static_cast<void>(::sprintf(numberString, "%d", value));
    return ::OcaLiteString(numberString);
}

template <>
::OcaLiteString ValueToString< ::OcaInt16>(const ::OcaInt16& value)
{
    char numberString[7];
    static_cast<void>(::sprintf(numberString, "%d", value));
    return ::OcaLiteString(numberString);
}

template <>
::OcaLiteString ValueToString< ::OcaInt32>(const ::OcaInt32& value)
{
    char numberString[12];
    static_cast<void>(::sprintf(numberString, "%d", value));
    return ::OcaLiteString(numberString);
}

template <>
::OcaLiteString ValueToString< ::OcaInt64>(const ::OcaInt64& value)
{
    char numberString[21];
    static_cast<void>(::sprintf(numberString, "%lld", value));
    return ::OcaLiteString(numberString);
}

template <>
::OcaLiteString ValueToString< ::OcaUint8>(const ::OcaUint8& value)
{
    char numberString[4];
    static_cast<void>(::sprintf(numberString, "%u", value));
    return ::OcaLiteString(numberString);
}

template <>
::OcaLiteString ValueToString< ::OcaUint16>(const ::OcaUint16& value)
{
    char numberString[6];
    static_cast<void>(::sprintf(numberString, "%u", value));
    return ::OcaLiteString(numberString);
}

template <>
::OcaLiteString ValueToString< ::OcaUint32>(const ::OcaUint32& value)
{
    char numberString[11];
    static_cast<void>(::sprintf(numberString, "%u", value));
    return ::OcaLiteString(numberString);
}

template <>
::OcaLiteString ValueToString< ::OcaUint64>(const ::OcaUint64& value)
{
    char numberString[21];
    static_cast<void>(::sprintf(numberString, "%llu", value));
    return ::OcaLiteString(numberString);
}

template <>
::OcaLiteString ValueToString< ::OcaFloat32>(const ::OcaFloat32& value)
{
    char numberString[100];
    static_cast<void>(::sprintf(numberString, "%f", value));
    return ::OcaLiteString(numberString);
}

template <>
::OcaLiteString ValueToString< ::OcaLiteBaseDataType>(const ::OcaLiteBaseDataType& value)
{
    ::OcaLiteString result;

    switch (value)
    {
    case OCABASEDATATYPE_NONE:
        result = ::OcaLiteString("None");
        break;
    case OCABASEDATATYPE_BOOLEAN:
        result = ::OcaLiteString("Boolean");
        break;
    case OCABASEDATATYPE_INT8:
        result = ::OcaLiteString("Int8");
        break;
    case OCABASEDATATYPE_INT16:
        result = ::OcaLiteString("Int16");
        break;
    case OCABASEDATATYPE_INT32:
        result = ::OcaLiteString("Int32");
        break;
    case OCABASEDATATYPE_INT64:
        result = ::OcaLiteString("Int64");
        break;
    case OCABASEDATATYPE_UINT8:
        result = ::OcaLiteString("Uint8");
        break;
    case OCABASEDATATYPE_UINT16:
        result = ::OcaLiteString("Uint16");
        break;
    case OCABASEDATATYPE_UINT32:
        result = ::OcaLiteString("Uint32");
        break;
    case OCABASEDATATYPE_UINT64:
        result = ::OcaLiteString("Uint64");
        break;
    case OCABASEDATATYPE_FLOAT32:
        result = ::OcaLiteString("Float32");
        break;
    case OCABASEDATATYPE_FLOAT64:
        result = ::OcaLiteString("Float64");
        break;
    case OCABASEDATATYPE_STRING:
        result = ::OcaLiteString("String");
        break;
    case OCABASEDATATYPE_BIT_STRING:
        result = ::OcaLiteString("BitString");
        break;
    case OCABASEDATATYPE_BLOB:
        result = ::OcaLiteString("Blob");
        break;
    case OCABASEDATATYPE_BLOB_FIXED_LEN:
        result = ::OcaLiteString("BlobFixedLen");
        break;
    default:
        result = ValueToString< ::OcaUint8>(static_cast< ::OcaUint8>(value));
        break;
    }

    return result;
}

template <>
::OcaLiteString ValueToString< ::OcaLiteString>(const ::OcaLiteString& value)
{
    return value;
}
