/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMessageResponse.
 *
 */

#ifndef OCALITEMESSAGERESPONSE_H
#define OCALITEMESSAGERESPONSE_H

// ---- Include system wide include files ----
#include <assert.h>

// ---- Include local include files ----
#include "OcaLiteMessageGeneral.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Representation of an Oca Response message.
 * @ingroup Messages
 */
class  OcaLiteMessageResponse : public ::OcaLiteMessageGeneral
{
public:
    /** Constructor */
    explicit OcaLiteMessageResponse();

    /** Destructor */
    virtual ~OcaLiteMessageResponse();

    /**
     * Getter for handle.
     *
     * @return handle.
     */
    ::OcaUint32 GetHandle() const
    {
        return m_handle;
    }

    /**
     * Getter for statusCode.
     *
     * @return statusCode.
     */
    ::OcaLiteStatus GetStatus() const
    {
        return m_statusCode;
    }

    /**
     * Getter for parameters.
     *
     * @return parameters.
     */
    const ::OcaUint8* GetParameters() const
    {
        return m_pParameters;
    }

    ::OcaUint32 GetParametersSize() const
    {
        return m_parametersSize;
    }

    /**
     * Writes the response parameters to the message
     *
     * @param[in]   handle              The message handle.
     * @param[in]   status              The result status code.
     */
    void WriteParameters(::OcaUint32 handle,
                         ::OcaUint8* pResponse,
                         ::OcaUint32 responseSize,
                         ::OcaLiteStatus status);

protected:

private:

    /** Parameters */
    ::OcaUint8*                     m_pParameters;
    ::OcaUint32                     m_parametersSize;

    /** Handle */
    ::OcaUint32                     m_handle;

    /** ::OcaLiteStatus code */
    ::OcaLiteStatus                 m_statusCode;

    /** Private copy constructor; no copying of object allowed. */
    OcaLiteMessageResponse(const ::OcaLiteMessageResponse&);
    /** Private assignment operator; no assignment of object allowed. */
    ::OcaLiteMessageResponse& operator=(const ::OcaLiteMessageResponse&);
};

#endif /* OCALITEMESSAGERESPONSE_H */
