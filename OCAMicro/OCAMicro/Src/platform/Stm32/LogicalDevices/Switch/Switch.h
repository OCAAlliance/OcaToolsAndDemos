/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : Proxy of switch device of application 
 *
 */

#ifndef SWITCH_H
#define SWITCH_H

// ---- Include system wide include files ----
#include "../IMember/IMember.h"

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

/**
 * The switch driver class,
 * Proxy for the device driver running on the Application Processor
 */
class Switch
{
public:
    /**
     * Getter for the OcaDeviceManager singleton.
     *
     * @return Reference to the OcaDeviceManager.
     */
    static ::Switch& GetInstance();

    /**
     * Cleans up the singleton instance.
     */
    static void FreeInstance();

    /**
     * Initialize the Switch driver
     *
     * @param[in] bReset       Whether to perform a hardware reset
     * @param[out] bSuccess    True if successful, false if not.
     */
    void Initialize(bool bReset, bool& bSuccess);

    /**
     * Terminate the Switch driver
     */
    void Terminate();
	
    /**
     * Whether any physical link is up
     * @return True if any external port is up, false if not.
     */
    bool GetLinkUp();

    /**
     * Whether link is in full duplex mode
     * @return True if full duplex, false if not.
     */
    bool GetFullDuplex();

    /**
     * Get maximum link speed
     * @return The maximum link speed of all external connected links.
     */
    UINT32 GetLinkSpeed();

    /*
     * Get RX errors
     * @return The number of receive errors
     */
    UINT32 GetRxErrors();

    /*
     * Reset RX errors
     */
    void ResetRxErrors();

protected:

private:

    /** Singleton instance of the server */
    static ::Switch* m_pSingleton;


    /** Constructor */
    Switch();

    /** Destructor */
    virtual ~Switch();

    /** private copy constructor, no copying of object allowed */
    Switch(const ::Switch&);

    /** private assignment operator, no assignment of object allowed */
    ::Switch& operator=(const ::Switch&);
};

#endif //~SWITCH_H
