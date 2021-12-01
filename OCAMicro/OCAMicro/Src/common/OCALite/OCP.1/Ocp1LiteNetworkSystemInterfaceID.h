/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : Ocp1LiteNetworkSystemInterfaceID
 *
 */

#ifndef OCP1LITENETWORKSYSTEMINTERFACEID_H
#define OCP1LITENETWORKSYSTEMINTERFACEID_H

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCP.1/Ocp1LiteHostInterfaceConstants.h>
#include <OCC/ControlDataTypes/OcaLiteBlobFixedLen.h>
#include <OCC/ControlDataTypes/OcaLiteNetworkSystemInterfaceID.h>
#include <OCC/ControlDataTypes/OcaLiteTemplateHelpers.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

#define MAC_ADDRESS_LENGTH  static_cast< ::OcaUint16>(MAC_ADDRESS_SIZE)

// ---- Helper types and constants ----
/**
 * The source of the network information
 */
enum Ocp1LiteIPParametersType   /* maps onto OcaUint8 */
{
    /** The network information type is
        unknown */
    OCP1IPPARAMETERS_TYPE_UNKNOWN = 0,
    /** The network information is assigned
        by the link-local algorithm */
    OCP1IPPARAMETERS_TYPE_LINK_LOCAL = 1,
    /** The network information is assigned
        by DHCP */
    OCP1IPPARAMETERS_TYPE_DHCP = 2,
    /** The network information is static
        assigned */
    OCP1IPPARAMETERS_TYPE_STATIC = 3,
    /** Needed for boundary checking. */
    OCP1IPPARAMETERS_TYPE_MAXIMUM
};

// ---- Helper functions ----

// ---- Class Definition ----
/**
 * Representation of OCP.1 Network System Interface ID.
 */
class Ocp1LiteNetworkSystemInterfaceID : public ::OcaLiteNetworkSystemInterfaceID
{
public:
    /** Default constructor. */
    Ocp1LiteNetworkSystemInterfaceID();

    /**
     * Constructor.
     *
     * @param[in] interfaceIndex    The interface index belonging to this interface information.
     * @param[in] ipAddress         The IP Address.
     * @param[in] subnetMaskLength  The length of the subnet mask.
     * @param[in] defaultGateway    The default gateway address.
     * @param[in] dnsServer         The address of the DNS Server.
     * @param[in] dnsDomainName     The assigned DNS domain name.
     * @param[in] linkUp            The linkUp boolean.
     * @param[in] adapterSpeed      The adapter speed.
     * @param[in] type              The source of the IP Address information.
     * @param[in] macAddress        The MAC (Hardware) address of this information.
     */
    Ocp1LiteNetworkSystemInterfaceID(::OcaUint32 interfaceIndex,
                                     const ::OcaLiteString& ipAddress,
                                     ::OcaUint8 subnetMaskLength,
                                     const ::OcaLiteString& defaultGateway,
                                     const ::OcaLiteString& dnsServer,
                                     const ::OcaLiteString& dnsDomainName,
                                     ::OcaBoolean linkUp,
                                     ::OcaUint64 adapterSpeed,
                                     ::Ocp1LiteIPParametersType type,
                                     const ::OcaLiteBlobFixedLen<MAC_ADDRESS_LENGTH>& macAddress);

    /**
     * Constructor.
     *
     * @param[in] interfaceIndex    The interface index belonging to this interface information.
     */
    explicit Ocp1LiteNetworkSystemInterfaceID(::OcaUint32 interfaceIndex);

    /**
     * Copy constructor.
     *
     * @param[in]   source      The source to copy the object from.
     */
    Ocp1LiteNetworkSystemInterfaceID(const ::Ocp1LiteNetworkSystemInterfaceID&);

    /** Destructor */
    virtual ~Ocp1LiteNetworkSystemInterfaceID();

    /**
     * Getter for the interface index.
     *
     * @return The interface index.
     */
    ::OcaUint32 GetInterfaceIndex() const;

    /**
     * Getter for the IP Address.
     *
     * @return The IP Address.
     */
    const ::OcaLiteString& GetIPAddress() const;

    /**
     * Getter for the subnet mask length.
     *
     * @return The subnet mask length.
     */
    ::OcaUint8 GetSubnetMaskLength() const;

    /**
     * Getter for the default gateway.
     *
     * @return The default gateway.
     */
    const ::OcaLiteString& GetDefaultGateway() const;

    /**
     * Getter for the DNS Server.
     *
     * @return The DNS Server.
     */
    const ::OcaLiteString& GetDnsServer() const;

    /**
     * Getter for the DNS Domain Name.
     *
     * @return The DNS Domain Name.
     */
    const ::OcaLiteString& GetDnsDomainName() const;

    /**
     * Getter for the linkup boolean.
     *
     * @return The linkup state.
     */
    ::OcaBoolean GetLinkUp() const;

    /**
     * Getter for the adapter speed.
     *
     * @return The adapter speed.
     */
    ::OcaUint64 GetAdapterSpeed() const;

    /**
     * Getter for the IP Parameters type.
     *
     * @return The IP Parameters type.
     */
    ::Ocp1LiteIPParametersType GetIPParametersType() const;

    /**
     * Getter for the MAC address.
     *
     * @return The MAC address.
     */
    const ::OcaLiteBlobFixedLen<MAC_ADDRESS_LENGTH>& GetMacAddress() const;

    /**
     * Assignment operator.
     *
     * @param[in]   source      The source to assign the object from.
     * @return A reference to this object.
     */
    ::Ocp1LiteNetworkSystemInterfaceID& operator=(const Ocp1LiteNetworkSystemInterfaceID& source);

    /**
     * Creates an Ocp1LiteNetworkSystemInterfaceID from the base class.
     *
     * @param[in]   baseClass   The base class to create the instance from.
     * @return The newly created instance, or NULL if parsing failed.
     */
    static ::Ocp1LiteNetworkSystemInterfaceID* CreateFromBase(const ::OcaLiteNetworkSystemInterfaceID& baseClass);

    virtual bool Unmarshal(::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

private:

    /**
     * Unmarshal the base and fill the members
     *
     * @param[in]  baseClass        The base class the read the values from.
     * @param[out] interfaceIndex   The interface index.
     * @param[out] ipAddress        The IP address.
     * @param[out] subnetMaskLength The subnet mask length.
     * @param[out] defaultGateway   The default gateway.
     * @param[out] dnsServer        The dns server.
     * @param[out] dnsDomainName    The dns domain name.
     * @param[out] bLinkUp          The linkup boolean
     * @param[out] adapterSpeed     The adapterspeed.
     * @param[out] ipParametersType The ip parameters type.
     * @param[out] macAddress       The mac address.
     *
     * @return True if succeeded, false if not. 
     */
    static bool GetValuesFromBase(const ::OcaLiteNetworkSystemInterfaceID& baseClass, ::OcaUint32& interfaceIndex, ::OcaLiteString& ipAddress,
                                  ::OcaUint8& subnetMaskLength, ::OcaLiteString& defaultGateway, ::OcaLiteString& dnsServer,
                                  ::OcaLiteString& dnsDomainName, ::OcaBoolean& bLinkUp, ::OcaUint64& adapterSpeed,
                                  ::Ocp1LiteIPParametersType& ipParametersType, ::OcaLiteBlobFixedLen<MAC_ADDRESS_LENGTH>& macAddress);

    /** Update the base blob with the private members */
    void UpdateBlob();

    /** The interface index */
    ::OcaUint32 m_interfaceIndex;

    /** The IP address */
    ::OcaLiteString m_ipAddress;

    /** The length of the subnet mask */
    ::OcaUint8 m_subnetMaskLength;

    /** The Default Gateway address */
    ::OcaLiteString m_defaultGateway;

    /** The DNS Server address */
    ::OcaLiteString m_dnsServer;

    /** The DNS Domain Name */
    ::OcaLiteString m_dnsDomainName;

    /** Is the link up */
    ::OcaBoolean m_linkUp;

    /** The current adpater speed */
    ::OcaUint64 m_adapterSpeed;

    /** The source of the network information */
    ::Ocp1LiteIPParametersType m_ipParametersType;

    /** The MAC address of the device. */
    ::OcaLiteBlobFixedLen<MAC_ADDRESS_LENGTH> m_macAddress;
};

// ---- Specialized Template Function Definition ----

template <>
void MarshalValue< ::Ocp1LiteIPParametersType>(const ::Ocp1LiteIPParametersType& value, ::OcaUint8** destination, const ::IOcaLiteWriter& writer);

template <>
bool UnmarshalValue< ::Ocp1LiteIPParametersType>(::Ocp1LiteIPParametersType& value, ::OcaUint32& bytesLeft, const ::OcaUint8** source, const ::IOcaLiteReader& reader);

template <>
::OcaUint32 GetSizeValue< ::Ocp1LiteIPParametersType>(const ::Ocp1LiteIPParametersType& value, const ::IOcaLiteWriter& writer);

#endif // OCP1LITENETWORKSYSTEMINTERFACEID_H
