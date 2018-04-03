/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The entry point to the Host Interface.
 *
 */

#ifndef OCFLITEHOSTINTERFACE_H
#define OCFLITEHOSTINTERFACE_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "Logging/IOcfLiteLog.h"
#include "Selection/OcfLiteSelectableSet.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----
#ifndef OCA_DISABLE_LOG_MACROS

#ifndef OCA_DISABLE_OCA_LOG_ERROR
    /**
     * Log an error message that does not have any additional parameters.
     *
     * @param  msg  The error message that must be logged.
     */
    #define OCA_LOG_ERROR(msg) \
            ::OcfLiteLogLogMessage(OCA_LOG_LVL_ERROR, OCA_ERROR_MESSAGE_HEADER, __FILE__, __LINE__, "%s", msg)
#else
    /**
     * Log an error message that does not have any additional parameters.
     * This macro is compiled out.
     *
     * @param  msg  The error message that must be logged.
     */
    #define OCA_LOG_ERROR(msg)
#endif

#ifndef OCA_DISABLE_OCA_LOG_WARNING
    /**
     * Log a warning message that does not have any additional parameters.
     *
     * @param  msg  The warning message that must be logged.
     */
    #define OCA_LOG_WARNING(msg) \
            ::OcfLiteLogLogMessage(OCA_LOG_LVL_WARNING, OCA_WARNING_MESSAGE_HEADER, __FILE__, __LINE__, "%s", msg)
#else
    /**
     * Log a warning message that does not have any additional parameters.
     * This macro is compiled out.
     *
     * @param  msg  The warning message that must be logged.
     */
    #define OCA_LOG_WARNING(msg)
#endif

#ifndef OCA_DISABLE_OCA_LOG_INFO
    /**
     * Log an info message that does not have any additional parameters.
     *
     * @param  msg  The info message that must be logged.
     */
    #define OCA_LOG_INFO(msg) \
            ::OcfLiteLogLogMessage(OCA_LOG_LVL_INFO, OCA_INFO_MESSAGE_HEADER, __FILE__, __LINE__, "%s", msg)
#else
    /**
     * Log a info message that does not have any additional parameters.
     * This macro is compiled out.
     *
     * @param  msg  The info message that must be logged.
     */
    #define OCA_LOG_INFO(msg)
#endif

#ifdef _DEBUG
    /**
     * Log a trace message that does not have any additional parameters.
     *
     * @param  msg  The trace message that must be logged.
     */
    #define OCA_LOG_TRACE(msg) \
            ::OcfLiteLogLogMessage(OCA_LOG_LVL_TRACE, OCA_TRACE_MESSAGE_HEADER, __FILE__, __LINE__, "%s", msg)
#else
    /**
     * Log a trace message that does not have any additional parameters.
     * This macro is compiled out of a release build.
     *
     * @param  msg  The trace message that must be logged (discarded in a release build).
     */
    #define OCA_LOG_TRACE(msg)
#endif

#ifdef NO_VAR_ARGS
    #define OCA_LOG_ERROR_PARAMS false ||
    #define OCA_LOG_WARNING_PARAMS false ||
    #define OCA_LOG_INFO_PARAMS false ||
    #define OCA_LOG_TRACE_PARAMS false ||
#else

    #ifndef OCA_DISABLE_OCA_LOG_ERROR
        /**
         * Log an error message that has additional parameters and must be formatted.
         *
         * @param  msg  The error message that must be logged (containing format specifiers
         *              for the additional parameters).
         * @param  ...  Optional additional parameters of the error message.
         */
        #define OCA_LOG_ERROR_PARAMS(msg, ...) \
                ::OcfLiteLogLogMessage(OCA_LOG_LVL_ERROR, OCA_ERROR_MESSAGE_HEADER, __FILE__, __LINE__, msg, __VA_ARGS__)
    #else
        /**
         * Log an error message that has additional parameters and must be formatted.
         * This macro is compiled out.
         *
         * @param  msg  The error message that must be logged.
         * @param  ...  Optional additional parameters of the trace message.
         */
        #define OCA_LOG_ERROR_PARAMS(msg, ...)
    #endif

    #ifndef OCA_DISABLE_OCA_LOG_WARNING
        /**
         * Log a warning message that has additional parameters and must be formatted.
         *
         * @param  msg  The warning message that must be logged (containing format specifiers
         *              for the additional parameters).
         * @param  ...  Optional additional parameters of the warning message.
         */
        #define OCA_LOG_WARNING_PARAMS(msg, ...) \
                ::OcfLiteLogLogMessage(OCA_LOG_LVL_WARNING, OCA_WARNING_MESSAGE_HEADER, __FILE__, __LINE__, msg, __VA_ARGS__)
    #else
        /**
         * Log a warning message that has additional parameters and must be formatted.
         * This macro is compiled out.
         *
         * @param  msg  The warning message that must be logged.
         * @param  ...  Optional additional parameters of the trace message.
         */
        #define OCA_LOG_WARNING_PARAMS(msg, ...)
    #endif

    #ifndef OCA_DISABLE_OCA_LOG_INFO
        /**
         * Log an info message that has additional parameters and must be formatted.
         *
         * @param  msg  The info message that must be logged (containing format specifiers
         *              for the additional parameters).
         * @param  ...  Optional additional parameters of the info message.
         */
        #define OCA_LOG_INFO_PARAMS(msg, ...) \
                ::OcfLiteLogLogMessage(OCA_LOG_LVL_INFO, OCA_INFO_MESSAGE_HEADER, __FILE__, __LINE__, msg, __VA_ARGS__)
    #else
        /**
         * Log an info message that has additional parameters and must be formatted.
         * This macro is compiled out.
         *
         * @param  msg  The info message that must be logged.
         * @param  ...  Optional additional parameters of the trace message.
         */
        #define OCA_LOG_INFO_PARAMS(msg, ...)
    #endif

    #ifdef _DEBUG
        /**
         * Log a trace message that has additional parameters and must be formatted.
         *
         * @param  msg  The trace message that must be logged (containing format specifiers
         *              for the additional parameters).
         * @param  ...  Optional additional parameters of the trace message.
         */
        #define OCA_LOG_TRACE_PARAMS(msg, ...) \
                ::OcfLiteLogLogMessage(OCA_LOG_LVL_TRACE, OCA_TRACE_MESSAGE_HEADER, __FILE__, __LINE__, msg, __VA_ARGS__)
    #else
        /**
         * Log a trace message that has additional parameters and must be formatted.
         * This macro is compiled out of a release build.
         *
         * @param  msg  The trace message that must be logged (discarded in a release build).
         * @param  ...  Optional additional parameters of the trace message.
         */
        #define OCA_LOG_TRACE_PARAMS(msg, ...)
    #endif
#endif
#endif

// ---- Class Definition ----

/**
 * Initializes the OcfLiteHostInterface.
 *
 * @return Indicates whether the host interface was properly initialized.
 */
bool OcfLiteHostInterfaceInitialize(void);

/**
 * Perform a select call.
 *
 * @param[in] highest       The highest filedescriptor.
 * @param[in] readset       The read socket set.
 * @param[in] writeset      The write socket set.
 * @param[in] exceptset     The except socket set.
 * @param[in] timeout       The timeout in ms.
 */
INT32 OcfLiteHostInterfaceSelect(INT32 highest, OcfLiteSelectableSet& readset,  OcfLiteSelectableSet& writeset, OcfLiteSelectableSet& exceptset, INT32 timeout);

#endif // OCFLITEHOSTINTERFACE_H
