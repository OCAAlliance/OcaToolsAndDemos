/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : OcaLiteMessageKeepAlive
 *
 */

#ifndef OCALITEMESSAGEKEEPALIVE_H
#define OCALITEMESSAGEKEEPALIVE_H

// ---- Include system wide include files ----

// ---- Include local include files ----
#include "OcaLiteMessageGeneral.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/** 
 * Representation of an Oca KeepAlive message. 
 * @ingroup Messages
 */
class  OcaLiteMessageKeepAlive : public ::OcaLiteMessageGeneral
{
public:
    /**
     * Getter for HeartBeatTime.
     *
     * @return m_HeartBeatTime
     */
    ::OcaUint16 GetHeartBeatTime() const
    {
        return m_heartBeatTime;
    }

    /**
     * Getter for HeartBeatTime.
     *
     * @return m_HeartBeatTimeInMIlliseconds
     */
    ::OcaUint32 GetHeartBeatTimeInMilliseconds() const
    {
        return m_heartBeatTimeInMilliseconds;
    }

    /**
     * Writes the keep alive parameters to the message
     *
     * @param[in]   heartBeatTime       The heart beat time in seconds.
     */
    void WriteParameters(::OcaUint16 heartBeatTime);

    /**
     * Writes the keep alive parameters to the message
     *
     * @param[in]   heartBeatTimeInMilliseconds       The heart beat time in milliseconds.
     */
    void WriteParameters(::OcaUint32 heartBeatTimeInMilliseconds);
protected:
    /**
     * Constructor
     */
    OcaLiteMessageKeepAlive();

    /** Destructor */
    virtual ~OcaLiteMessageKeepAlive();

private:
    /** HeartBeatTime in seconds */
    ::OcaUint16          m_heartBeatTime;

    /** HeartBeatTime in milliseconds */
    ::OcaUint32          m_heartBeatTimeInMilliseconds;
    /** Private copy constructor; no copying of object allowed. */
    OcaLiteMessageKeepAlive(const ::OcaLiteMessageKeepAlive&);
    /** Private assignment operator; no assignment of object allowed. */
    ::OcaLiteMessageKeepAlive& operator=(const ::OcaLiteMessageKeepAlive&);
};

#endif // OCALITEMESSAGEKEEPALIVE_H
