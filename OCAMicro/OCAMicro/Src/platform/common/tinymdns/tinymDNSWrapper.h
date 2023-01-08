/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : mDNS on LwIP for OCALite
 *
 */

#ifndef TINYMDNSWRAPPER_H
#define TINYMDNSWRAPPER_H

// ---- Include system wide include files ----
#include <Stm32/LogicalDevices/IMember/IMember.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Class Definition ----

/**
 * This class is a GoF adapter for the tiny mdns wrapper interface.
 */
class tinymDNSWrapper: IMember
{
public:

    /**
     * Server states
     */
    enum mDNSdStates
    {
        Uninitialized = 0,
        Idle,
        NewAndUp,
        NewAndDown,
        RunAndUp,
        RunAndDown,
        StopAndDown
    };

    /**
     * Server events
     */
    enum mDNSdEvent
    {
        RegisterEvt = 0,
        UnregisterEvt,
        LinkUpEvt,
        LinkDownEvt,
        PublishEvt
    };

    /**
     * Getter for the SpiAdapter singleton.
     *
     * @return  Reference to the SpiAdapter.
     */
    static tinymDNSWrapper& GetInstance();

    /**
     * Cleans up the singleton instance.
     */
    static void FreeInstance();

    /**
     * Initialize the mdns wrapper
     *
     * @return True if succeeded, false if not.
     */
    bool Initialize();

    /**
     * Teardown the mdns wrapper
     */
    void Teardown();
    
    /**
     * Register a service with the mDNSd daemon. The service
     * is registered on all available network interfaces in the passed
     * domain. If service registration succeeds the service
     * is announced to the network.
     *
     * @param[in]  name             Name of the service that is registered
     * @param[in]  registrationType The registration type (service type and transport protocol, separated by a dot,
     *                              e.g. "_ftp._tcp"). The service type must be an underscore, followed
     *                              by 1-15 characters, which may be letters, digits, or hyphens.
     *                              The transport protocol must be "_tcp" or "_udp".
     * @param[in]  port             Port on which the service accepts connections
     * @param[in]  txtRecordList    The TXT Record data as a string list. Pass an empty list if
     *                              the service does not have a TXT record.
     * @param[in]  domain           The domain on which this service should be announced.
     *
     * @return True if succeeded, false if not.
     */
    bool Register(const std::string& name, const std::string& registrationType,
                  UINT16 port, const std::vector<std::string>& txtRecordList, const std::string& domain);

    /**
     * Unregister the current service with the mDNSd daemon.
     *
     * @return True if succeeded, false if not.
     */
    bool Unregister();

    /**
     * Execute deamon job
     **/
    UINT32 Run();

private:
    /** Singleton instance of the server */
    static ::tinymDNSWrapper* m_pSingleton;

    /* State of the mDNSd deamon */
    mDNSdStates m_myState;

    /** The registered name */
    std::string m_name;
    /** The registration type */
    std::string m_registrationType;
    /** The txt record list */
    std::vector<std::string> m_txtRecordList;
    /** The domain */
    std::string m_domain;
    /** The port */
    UINT16 m_port;
    /** The registered IP address */
    UINT32 m_ipAddress;
    /** The service structure */
    struct mdns_service* m_svc;

    /**
     * Handle the events in the different States
     *
     * @param[in]   event Event to handle
     *
     * @return True indicates that the event was handled, otherwise an error occurred
     **/
    bool HandleEvent(mDNSdEvent event);
    bool HandleIdleState(mDNSdEvent event);
    bool HandleNewAndUpState(mDNSdEvent event);
    bool HandleNewAndDownState(mDNSdEvent event);
    bool HandleRunAndUpState(mDNSdEvent event);
    bool HandleRunAndDownState(mDNSdEvent event);
    bool HandleStopAndDownState(mDNSdEvent event);

    /**
     * Create new service
     **/
    void CreateService();

    /**
     * Delete current service
     **/
    void DeleteService();

    /**
     * Check if current service needs to be shutdown
     *
     * @return True if shutdown needed, otherwise false
     **/
    bool CheckShutdownNeed();

    /**
     * Callback as LwIPAdapterOcaLite member.
     */
    bool MemberCallback(UINT32 data);

    /**
     * Default constructor. Initializes the member data.
     */
    tinymDNSWrapper();

    /**
     * Destructor.
     */
    virtual ~tinymDNSWrapper();

    /**
     * Private copy constructor, no copying of object allowed
     */
    tinymDNSWrapper(const ::tinymDNSWrapper&);

    /**
     * Private assignment operator, no assignment of object allowed
     */
    ::tinymDNSWrapper& operator=(const ::tinymDNSWrapper&);

};

#endif // TINYMDNSWRAPPER_H
