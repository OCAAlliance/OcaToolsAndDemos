/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMessageCommand
 *
 */

#ifndef OCALITEMESSAGECOMMAND_H
#define OCALITEMESSAGECOMMAND_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteMethodID.h>

// ---- Include local include files ----
#include "OcaLiteMessageGeneral.h"

// ---- Referenced classes and types ----
class OcaLiteMethodID;

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Representation of an OCA Command message.
 */
class  OcaLiteMessageCommand : public ::OcaLiteMessageGeneral
{
public:
    /**
     * Getter for Handle.
     *
     * @return m_Handle
     */
    ::OcaUint32 GetHandle() const
    {
        return m_handle;
    }
    
    /**
     * Getter for TargetONo.
     *
     * @return m_TargetONo
     */
    ::OcaONo GetTargetONo() const
    {
        return m_targetONo;
    }

    /**
     * Getter for MethodID.
     *
     * @return m_MethodID
     */
    const ::OcaLiteMethodID& GetMethodID() const
    {
        return m_methodID;
    }

    /**
     * Getter for Parameters.
     *
     * @return m_Parameters
     */
    ::OcaUint8* GetParameters() const
    {
        return m_pParameters;
    }

    /**
     * Getter for parameterSize. I.e. the size of the data.
     *
     * @return parameterSize.
     */
    ::OcaUint32 GetParametersSize() const
    {
        return m_parametersSize;
    }

    /**
     * Writes the command parameters to the message
     *
     * @param[in]   handle              The command handle.
     * @param[in]   targetONo           The target object number.
     * @param[in]   methodID            The method ID.
     * @param[in]   parametersSize      The size of the parameters.
     */
    void WriteParameters(::OcaUint32 handle,
                         ::OcaONo targetONo,
                         const ::OcaLiteMethodID& methodID,
                         ::OcaUint32 parametersSize);  

protected:
    /**
     * Constructor
     *
     * @param[in]   messageType  Type of message
     */
    explicit OcaLiteMessageCommand(::OcaLiteHeader::OcaLiteMessageType messageType);

    /** Destructor */
    virtual ~OcaLiteMessageCommand();

private:
    
    /** Parameters */
    ::OcaUint32 m_parametersSize;
    ::OcaUint8* m_pParameters;

    /** Handle */
    ::OcaUint32                     m_handle;

    /** TargetONo */
    ::OcaONo                        m_targetONo;

    /** MethodID */
    ::OcaLiteMethodID               m_methodID;

    /** Private copy constructor; no copying of object allowed. */
    OcaLiteMessageCommand(const ::OcaLiteMessageCommand&);
    /** Private assignment operator; no assignment of object allowed. */
    ::OcaLiteMessageCommand& operator=(const ::OcaLiteMessageCommand&);
};

#endif // OCALITEMESSAGECOMMAND_H
