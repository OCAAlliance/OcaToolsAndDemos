/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OCC Worker data types.
 *
 */

#include <assert.h>
#include <time.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----
#include "OcaLiteWorkerDataTypes.h"

// ---- Helper types and constants ----

/** The divisor to convert from decimal seconds to the fraction. */
#define FRACTION_DIVISOR                static_cast<UINT64>(0x100000000ULL) // 2^32

/** The number of nano seconds per second. */
#define NANO_SECONDS_PER_SECOND         static_cast<UINT32>(1000000000)     // 1,000,000,000

/** The number of seconds between 1/1/1900 and 1/1/1970 */
#define SECONDS_BETWEEN_1900_AND_1970   static_cast<UINT32>(2208988800U)

// ---- Helper functions ----

// ---- Local data ----

/** The number of seconds between 1/1/1900 and 1/1/1970 */
#define SECONDS_BETWEEN_1900_AND_1970   static_cast<UINT32>(2208988800U)

// ---- Function Implementation ----

::OcaLiteTimeNTP CreateTimeNTP(::OcaUint16 year, ::OcaUint8 month, ::OcaUint8 day, ::OcaUint8 hour, ::OcaUint8 minute, ::OcaUint8 second, ::OcaUint32 nanoSeconds)
{
    assert(year >= static_cast<::OcaUint16>(1970));

    struct tm dateTime;
    ::memset(&dateTime, 0, sizeof(dateTime));
    dateTime.tm_year = static_cast<int>(year - static_cast<::OcaUint16>(1900));
    dateTime.tm_mon = static_cast<int>(month - static_cast<::OcaUint8>(1));
    dateTime.tm_mday = static_cast<int>(day);
    dateTime.tm_hour = static_cast<int>(hour);
    dateTime.tm_min = static_cast<int>(minute);
    dateTime.tm_sec = static_cast<int>(second);
    dateTime.tm_isdst = -1; // -1 computes whether standard time or daylight saving time is in effect
    ::time_t timeValue(::mktime(&dateTime));

    UINT32 timeSeconds(static_cast<UINT32>(timeValue));

    return ::CreateTimeNTP(static_cast<::OcaUint32>(timeSeconds), nanoSeconds);
}

::OcaLiteTimeNTP CreateTimeNTP(::OcaUint32 seconds, ::OcaUint32 nanoSeconds)
{
    UINT32 fraction(static_cast<UINT32>((static_cast<DOUBLE>(nanoSeconds) / NANO_SECONDS_PER_SECOND) * FRACTION_DIVISOR));

    return static_cast<::OcaLiteTimeNTP>(((static_cast<UINT64>(seconds + SECONDS_BETWEEN_1900_AND_1970) << 32) & 0xFFFFFFFF00000000LL) + static_cast<UINT64>(fraction));
}

bool ParseTimeNTP(::OcaLiteTimeNTP timeNTP, ::OcaUint16& year, ::OcaUint8& month, ::OcaUint8& day, ::OcaUint8& hour, ::OcaUint8& minute, ::OcaUint8& second, ::OcaUint32& nanoSeconds)
{
    bool bSuccess(false);

    ::OcaUint32 totalSeconds;
    static_cast<void>(::ParseTimeNTP(timeNTP, totalSeconds, nanoSeconds));
    ::time_t timeValue(static_cast<::time_t>(static_cast<UINT32>(totalSeconds)));

    struct tm* dateTime(::localtime(&timeValue));
    if (NULL != dateTime)
    {
        year = static_cast<::OcaUint16>(dateTime->tm_year + 1900);
        month = static_cast<::OcaUint8>(dateTime->tm_mon + 1);
        day = static_cast<::OcaUint8>(dateTime->tm_mday);
        hour = static_cast<::OcaUint8>(dateTime->tm_hour);
        minute = static_cast<::OcaUint8>(dateTime->tm_min);
        second = static_cast<::OcaUint8>(dateTime->tm_sec);
        bSuccess = true;
    }

    return bSuccess;
}

bool ParseTimeNTP(::OcaLiteTimeNTP timeNTP, ::OcaUint32& seconds, ::OcaUint32& nanoSeconds)
{
    seconds = static_cast<::OcaUint32>((static_cast<UINT64>(timeNTP) >> 32) & 0x00000000FFFFFFFFLL) - SECONDS_BETWEEN_1900_AND_1970;

    UINT32 fraction(static_cast<::OcaUint32>(static_cast<UINT64>(timeNTP) & 0x00000000FFFFFFFFLL));
    nanoSeconds = static_cast<::OcaUint32>(((static_cast<DOUBLE>(fraction) / FRACTION_DIVISOR) * NANO_SECONDS_PER_SECOND) + 0.5);

    return true;
}
