/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : Contains basic routines to prevent need for stdlib calls.
 *
 */

// ---- Include system wide include files ----
#include <StandardLib/StandardLib.h>

// ---- Include local include files ----

//----- Definitions ----

//----- Type declaration ----

//----- Global variables ----

//----- Local variables  ---
static const char s_byteMap[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
static const int s_byteMapLen = sizeof(s_byteMap);

//----- Local Function declarations ----

static int FindEndOfNumber(const char *pBuffer);
static char NibbleToChar(UINT8 nibble);

//****************************************************************************

/**
 * The __verbose_terminate_handler stub function.
 */
namespace __gnu_cxx
{
    void __verbose_terminate_handler()
    {
        for (;;)
            ;
    }
}

/**
 * The ScanIpAddress function.
 */
extern "C" int ScanIpAddress(const char *buffer, int numbers[])
{
    int i, j, idx;
    const char *pNumber(buffer);

    idx = -1;
    for (i = 0; i < 4; i++)
    {
        numbers[i] = 0;
        pNumber = &pNumber[idx + 1];
        idx = FindEndOfNumber(pNumber);
        if ((idx > 0) && (idx <= 3))
        {
            for (j = 0; j < idx; j++)
            {
                UINT8 value(static_cast<UINT8>(pNumber[j] - '0'));
                if (value <= 9)
                {
                    numbers[i] = static_cast<int>((numbers[i] * 10) + static_cast<int>(value));
                }
                else
                {
                    break; // j loop
                }
            }

            if (j != idx)
            {
                break; // i loop
            }
        }
        else
        {
            break; // i loop
        }
    }

    return static_cast<int>(4);
}

/**
 * The PrintIpAddress function.
 */
extern "C" void PrintIpAddress(char *buffer, const UINT8 numbers[])
{
    unsigned char writeIndex = 0;
    unsigned char i;
    for (i = 0; i < 4; i++) // Loop through our 4 IP digits
    {
        unsigned char valueCopy = numbers[i];
        if (valueCopy > 0)
        {
            unsigned char decimalDivider;
            bool bFoundSomeRealValue = false;
            for (decimalDivider = 100; decimalDivider > 0; decimalDivider /= 10)
            {
                unsigned char valueToWrite = valueCopy / decimalDivider;
                if (valueToWrite != 0)
                {
                    bFoundSomeRealValue =true;
                }
            
                if (bFoundSomeRealValue || (valueToWrite != 0))
                {
                    buffer[writeIndex++] = static_cast<char>('0' + valueToWrite);
                }
                valueCopy %= decimalDivider;
            }
        }
        else
        {
            buffer[writeIndex++] = '0';
        }
        buffer[writeIndex++] = '.';
    }
    buffer[writeIndex - 1] = '\0';
}

/**
 * The PrintMacAsSerialNumber function.
 */
extern "C" void PrintMacAsSerialNumber( char *buffer, const UINT8 numbers[])
{
    int i;
    
    for (i = 0; i < 6; i++)
    {
        buffer[i * 2] = NibbleToChar(numbers[i] >> 4);
        buffer[i * 2 + 1] = NibbleToChar(numbers[i] & 0x0f);
    }
    buffer[i * 2] = '\0';
}

/**
 * Configures the different system clocks.
 */
static int FindEndOfNumber(const char *pBuffer)
{
    int length(static_cast<int>(strlen(pBuffer)));
    int i;

    for (i = 0; i <= length; i++)
    {
        if (pBuffer[i] == '.')
        {
            break;
        }
    }
    return i;
}

/**
 * Utility function to convert nibbles (4 bit values) into a hex character representation
 */
static char NibbleToChar(UINT8 nibble)
{
    if (nibble < s_byteMapLen)
    {
        return s_byteMap[nibble];
    }
    else
    {
       return '*';
    }
}
