/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Ocp1MessageResponse
 *
 */

#ifndef OCP1LITEMESSAGERESPONSE_H
#define OCP1LITEMESSAGERESPONSE_H

// ---- Include system wide include files ----
#include <OCF/Messages/OcaLiteMessageResponse.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

class Ocp1MessageResponse;

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Representation of an OCP.1 Message Response
 * @ingroup OCP1Messages
 */
class Ocp1MessageResponse : public ::OcaLiteMessageResponse
{
public:
    /** Constructor */
    explicit Ocp1MessageResponse();

    /** Destructor */
    virtual ~Ocp1MessageResponse();

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

private:
    /** Private copy constructor; no copying of object allowed. */
    Ocp1MessageResponse(const ::Ocp1MessageResponse&);
    /** Private assignment operator; no assignment of object allowed. */
    ::Ocp1MessageResponse& operator=(const ::Ocp1MessageResponse&);
};

#endif // OCP1LITEMESSAGERESPONSE_H
