/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Ocp1LiteMessageKeepAlive
 *
 */

#ifndef OCP1LITEMESSAGEKEEPALIVE_H
#define OCP1LITEMESSAGEKEEPALIVE_H

// ---- Include system wide include files ----
#include <OCF/Messages/OcaLiteMessageKeepAlive.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

class Ocp1LiteMessageKeepAlive;

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Representation of an OCP.1 Message Keep Alive
 */
class Ocp1LiteMessageKeepAlive : public ::OcaLiteMessageKeepAlive
{
public:

    /** Constructor */
    Ocp1LiteMessageKeepAlive();

    /** Destructor */
    virtual ~Ocp1LiteMessageKeepAlive();

    virtual void Marshal(::OcaUint8** destination, const ::IOcaLiteWriter& writer) const;

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

    virtual ::OcaUint32 GetSize(const ::IOcaLiteWriter& writer) const;

protected:

private:

    /** Private copy constructor; no copying of object allowed. */
    Ocp1LiteMessageKeepAlive(const ::Ocp1LiteMessageKeepAlive&);
    /** Private assignment operator; no assignment of object allowed. */
    ::Ocp1LiteMessageKeepAlive& operator=(const ::Ocp1LiteMessageKeepAlive&);
};

#endif // OCP1LITEMESSAGEKEEPALIVE_H
