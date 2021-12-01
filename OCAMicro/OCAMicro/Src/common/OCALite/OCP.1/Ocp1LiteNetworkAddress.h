/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Ocp1LiteNetworkAddress
 *
 */

#ifndef OCP1LITENETWORKADDRESS_H
#define OCP1LITENETWORKADDRESS_H

// ---- Include system wide include files ----
#include <OCC/ControlDataTypes/OcaLiteNetworkAddress.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Representation of OCP.1 network address.
 */
class Ocp1LiteNetworkAddress : public ::OcaLiteNetworkAddress
{
public:
    /** Default constructor. */
    Ocp1LiteNetworkAddress();

    /**
     * Constructor
     *
     * @param[in]   destHostOrIPAddress  The hostname or IP address of the destination device.
     * @param[in]   port                 The port of the destination device
     */
    Ocp1LiteNetworkAddress(const ::OcaLiteString& destHostOrIPAddress,
                       ::OcaUint16 port);

   /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    Ocp1LiteNetworkAddress(const ::Ocp1LiteNetworkAddress&);

    /** Destructor */
    virtual ~Ocp1LiteNetworkAddress();

    /**
     * Getter for the hostname or IP address of the destination device.
     *
     * @return m_destHostOrIPAddress
     */
    const ::OcaLiteString& GetDestHostOrIPAddress() const;

    /**
     * Getter for the port of the destination device
     *
     * @return m_port
     */
    ::OcaUint16 GetPort() const;

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    Ocp1LiteNetworkAddress& operator=(const ::Ocp1LiteNetworkAddress& source);

    /**
     * Creates an Ocp1LiteNetworkAddress from the base class.
     *
     * @param[in]   baseClass   The base class to create the instance from.
     * @return The newly created instance, or NULL if parsing failed.
     */
    static ::Ocp1LiteNetworkAddress* CreateFromBase(const ::OcaLiteNetworkAddress& baseClass);

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

private:
    /**
     * Unmarshal the base and fill the members
     *
     * @param[in]  baseClass    The base class the read the values from.
     * @param[out] destHostOrIp The destination host or ip address.
     * @param[out] port         The port.
     *
     * @return True if succeeded, false if not. 
     */
    static bool GetValuesFromBase(const ::OcaLiteNetworkAddress& baseClass, ::OcaLiteString& destHostOrIp, ::OcaUint16& port);

    /** Update the base blob with the private members */
    void UpdateBlob();

    /** The hostname or IP address of the destination device. */
    ::OcaLiteString             m_destHostOrIPAddress;

    /** The port of the destination device */
    ::OcaUint16                 m_port;
};

#endif /*  OCP1LITENETWORKADDRESS_H */
