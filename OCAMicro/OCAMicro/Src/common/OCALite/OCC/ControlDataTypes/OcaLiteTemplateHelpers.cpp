/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Helper template functions for template data types
 */

// ---- Include system wide include files ----
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
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

