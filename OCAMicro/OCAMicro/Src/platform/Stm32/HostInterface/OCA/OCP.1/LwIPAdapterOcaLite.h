/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : LwIPAdapterOcaLite
 *
 */

#ifndef LwIPAdapterOcaLite_H
#define LwIPAdapterOcaLite_H

// ---- Include system wide include files ----
#include <HostInterfaceLite/OCA/OCP.1/Ocp1LiteHostInterfaceConstants.h>
#include <Stm32/LogicalDevices/IMember/IMember.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

/**
 * @addtogroup OcaLiteOCFHostInterface
 * @{
 */

// ---- Helper types and constants ----

// ---- Class Definition ----

#if (defined __cplusplus) || (defined DOXYGEN)

/**
 * This class is a GoF adapter for the LwIP interface.
 */
class LwIPAdapterOcaLite: IMember
{
public:

    /**
     * EventID's as known within OcaLite
     */
    enum EventID
    {
        /** Updrade status is changed */
        ON_UPGRADE_STATUS_CHANGED_EVENT_ID      = 0x1, 
        /** Interface status is changed */
        ON_INTERFACE_STATUS_CHANGED_EVENT_ID    = 0x2,
    };

    /** Member type */
    enum MemberType
    {
        // Triggered on any occasion
        MEMBER_ALWAYS                       = 0,
        // Trigger in case of link status changes
        MEMBER_LINK_STATUS                  = 1,
        // MAXIMUM
        MEMBER_MAXIMUM
    };

    /** Switch query state */
    enum SwitchQueryState
    {
        // New status available
        STATUS_REFRESHED                   = 0,
        // Status known
        STATUS_KNOWN                       = 1,
        // Call switch to get the status
        QUERY_STATUS                       = 2,
        // Status unknown (at start up)
        STATUS_UNKNOWN                     = 3
    };

    /** DHCP state */
    enum DhcpState
    {
        DHCP_INACTIVE                      = 0,
        DHCP_START                         = 1,
        DHCP_WAIT_ADDRESS                  = 2,
        DHCP_ADDRESS_ASSIGNED              = 3,
        DHCP_TIMEOUT                       = 4,
        DHCP_LINK_DOWN                     = 5
    };

    /**
     * Getter for the SpiAdapter singleton.
     *
     * @return  Reference to the SpiAdapter.
     */
    static LwIPAdapterOcaLite& GetInstance();

    /**
     * Cleans up the singleton instance.
     */
    static void FreeInstance();

    /**
     * Initialize the SpiAdapter
     *
     * @return True if succeeded, false if not.
     */
    bool Initialize();

    /**
     * Teardown the SpiAdapter
     */
    void Teardown() const;
    
    /**
     * Add Member for LwIP worker thread
     *
     * @param[in] memberType   Member type
     * @param[in] pMember      Member to address for packet delivery
     *
     */
    void AddMember(MemberType memberType, ::IMember* pMember);

    /**
     * Remove Member for LwIP worker thread
     *
     * @param[in] memberType   Member type
     * @param[in] pMember      Member to remove
     */
    void RemoveMember(MemberType memberType, ::IMember* pMember);

    /**
     * GetMacAddress
     *
     * @return MAC address.
     **/
    UINT8* GetMacAddress();

    /**
     * GetIpAddress
     *
     * @return IP address.
     **/
    UINT32 GetIpAddress();

    /**
     * GetNetMask
     *
     * @return Net mask.
     **/
    UINT32 GetNetMask();

    /**
     * GetDefaultGateway
     *
     * @return Default gateway address.
     **/
    UINT32 GetDefaultGateway();

    /**
     * IsLinkUpAndAddressAssigned
     *
     * @return True if link state is up and Ip address defined, else false.
     **/
    bool IsLinkUpAndAddressAssigned();

    /**
     * Set DHCP mode
     *
     * @param[in] active      True to set active
     */
    void SetDhcpMode(bool active);

    /**
     * Run
     *
     * @return Delay value in msecs.
     **/
    UINT32 Run();

private:
    /** Singleton instance of the server */
    static ::LwIPAdapterOcaLite* m_pSingleton;

    /** Whether the adapter is initialized */
    bool m_bInitialized;

    /** Map of registered adapter members */
    ::IMember::MemberMap m_members;
    
    /** Latest queried link status */
    bool m_latestLinkUpState;

    /** Time of lastest link state query  */
    UINT32 m_linkStateRefreshTime;
    
    /** State of the link status query action */
    SwitchQueryState m_SwitchQueryState;
    
    /** State of the DHCP server */
    DhcpState m_DhcpState;
    
    /** Stop DHCP service */
    bool m_StopDhcpService;
    
    /**
     * Process link status change
     **/
    void ProcessLinkStatusChange();

    /**
     * Inform members as requested
     **/
    void InformMembers(IMember::MemberMap::const_iterator &mapIter);

    /**
     * Callback as switch member.
     */
    bool MemberCallback(UINT32 data);

    /**
     * Default constructor. Initializes the member data.
     */
    LwIPAdapterOcaLite();

    /**
     * Destructor.
     */
    virtual ~LwIPAdapterOcaLite();

    /**
     * Private copy constructor, no copying of object allowed
     */
    LwIPAdapterOcaLite(const ::LwIPAdapterOcaLite&);

    /**
     * Private assignment operator, no assignment of object allowed
     */
    ::LwIPAdapterOcaLite& operator=(const ::LwIPAdapterOcaLite&);

};

#endif // (__cplusplus || DOXYGEN)

/** @} */ // end addtogroup OcaLiteOCFHostInterface

#endif // LwIPAdapterOcaLite_H
