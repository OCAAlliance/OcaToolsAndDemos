/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The logging interface of the Host Interface.
 *
 */

#ifndef IOCFLITELOG_H
#define IOCFLITELOG_H

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----
/**
 * No logging.
 */
#define OCA_LOG_LVL_NONE    0
/**
 * Level of software errors log messages.
 */
#define OCA_LOG_LVL_ERROR   10
/**
 * Level of warning log messages.
 */
#define OCA_LOG_LVL_WARNING 20
/**
 * Level of info log messages.
 */
#define OCA_LOG_LVL_INFO    30
/**
 * Level of trace log messages.
 */
#define OCA_LOG_LVL_TRACE   40

/**
 * The identification string of an error message.
 */
#define OCA_ERROR_MESSAGE_HEADER    "Error:"
/**
 * The identification string of a warning message.
 */
#define OCA_WARNING_MESSAGE_HEADER  "Warning:"
/**
 * The identification string of a info message.
 */
#define OCA_INFO_MESSAGE_HEADER     "Info:"
/**
 * The identification string of a trace message.
 */
#define OCA_TRACE_MESSAGE_HEADER    "Trace:"

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Log a message
 *
 * @param[in]   logLevel    The log level of the message.
 * @param[in]   type        The type string of the message.
 * @param[in]   file        The file name of the file in which the message is logged.
 * @param[in]   lineNumber  Line number where the log is called.
 * @param[in]   message     The message to be logged.
 * @param[in]   ...         Optional arguments in the message
 */
void OcfLiteLogLogMessage(UINT8 logLevel, const char* type, const char* file, UINT32 lineNumber, const char* message, ...);

/**
 * Sets the log level of the logger. Messages with a log level that is
 * higher than the configured log level do not get logged.
 *
 * @param[in]  logLevel  The log level of the logger
 */
void OcfLiteLogSetLogLevel(UINT8 logLevel);

#endif // IOCFLITELOG_H
