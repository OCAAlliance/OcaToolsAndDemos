/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 */

/*
 *  Description         : The logging implementation of the Host Interface for
 *                        a OcaLite enabled platform.
 *
 */

// ---- Include system wide include files ----
#include <stdarg.h>
#include <algorithm>
#include <HostInterfaceLite/OCA/OCF/Logging/IOcfLiteLog.h>

// ---- Include local include files ----

// ---- Helper types and constants ----
/** Maximum amount of characters in one log message */
#define MAX_MESSAGE_SIZE             1024
/** Additional size needed for log message header */
#define MAX_OCA_LOG_MESSAGE_HEADER_SIZE  256
/** Maximum full log message length */
#define MAX_FULL_MESSAGE_LENGTH (MAX_MESSAGE_SIZE + MAX_OCA_LOG_MESSAGE_HEADER_SIZE - 1)


// ---- Helper functions ----

// ---- Local data ----
static UINT8 m_logLevel(OCA_LOG_LVL_ERROR);
static char m_fullLogMessage[MAX_MESSAGE_SIZE + MAX_OCA_LOG_MESSAGE_HEADER_SIZE];
static UINT8 m_indent;

// ---- Class Implementation ----

void OcfLiteLogLogMessage(UINT8 logLevel, const char* type, const char* file, UINT32 lineNumber,
                          const char* message, ...)
{
    if (logLevel <= m_logLevel)
    {
        // Format and print the message
        va_list argList;
        va_start(argList, message);

        // Check if the message fits into the message buffer, add 1 for terminating '\0'
        UINT32 messageLength(static_cast<UINT32>(::vsnprintf(NULL, 0, message, argList) + 1));
        if (messageLength <= MAX_FULL_MESSAGE_LENGTH)
        {
            // For Apple this seems required to reset the argList after above vsnprintf function
            va_start(argList, message);

#ifdef _WIN32
            INT32 result(::vsprintf_s(m_fullLogMessage, message, argList));
#else
            INT32 result(::vsprintf(m_fullLogMessage, message, argList));
#endif
            bool bAddCarrageReturn = false;

            if (static_cast<UINT32>(result) <= messageLength)
            {
                // Decrease message indent
                if (m_fullLogMessage[0] == '}')
                {
                    if (m_indent >= 2)
                    {
                        m_indent -= 2;
                    }
                    else
                    {
                        m_indent = 0;
                    }
                }
                if ((logLevel == OCA_LOG_LVL_ERROR) &&
                    (m_fullLogMessage[messageLength - 2] != '\r') &&
                    (m_fullLogMessage[messageLength - 2] != '\n'))
                {
                    bAddCarrageReturn = true;
                }

                if (m_fullLogMessage[0] == '.')
                {
                    printf("%s%s", &m_fullLogMessage[1], bAddCarrageReturn ? "\r\n" : "");
                }
                else
                {
                    printf("%*s%s%s", m_indent, "", m_fullLogMessage, bAddCarrageReturn ? "\r\n" : "");
                }

                // Increase message indent
                if (m_fullLogMessage[0] == '{')
                {
                    m_indent += 2;
                }
            }
            else 
            {
                printf("Buffer overflow in processing log message\r\n");
            }
        }
        else 
        {
            printf("Buffer overflow in log message\r\n");
        }
        va_end(argList);
    }
}

void OcfLiteLogSetLogLevel(UINT8 logLevel)
{
    m_logLevel = logLevel;
}
