/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : Clock Generator device
 *
 */

#ifndef CLOCKGENERATOR_H
#define CLOCKGENERATOR_H

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

/**
 * FPGA driver class.
 */
class ClockGenerator
{
public:

    /**
     * Getter for the ClockGenerator singleton.
     *
     * @return  Reference to the ClockGenerator.
     */
    static ClockGenerator& GetInstance();

    /**
     * Cleans up the singleton instance.
     */
    static void FreeInstance();

    /**
     * Initialize the Clock Generator.
     */
    void Initialize();

    /**
     * Teardown the Clock Generator.
     */
    void Teardown();

    /**
     * Configure the Clock Generator accoording referred configuration
     *
     * @param[in] configuration    Refrence to predefined setings.
     *
     * @return <b>true</b> if succeeded, <b>false</b> if not.
     */
    bool Configure(const UINT8 configuration);

    /**
     * Return true if configured and initialized.
     */
    bool IsConfiguredAndInitialized() const
    {
        return m_bConfigured & m_bInitialized;
    }

protected:

private:

    /** Singleton instance of the FPGA driver */
    static ClockGenerator*  m_pSingleton;

    /** True if initialized */
    mutable bool m_bInitialized;

    /** True if configured */
    mutable bool m_bConfigured;

    /** Default constructor. Initializes the member data. */
    ClockGenerator();

    /** Destructor. */
    virtual ~ClockGenerator();

    /** Private copy constructor, no copying of object allowed */
    ClockGenerator(const ::ClockGenerator&);

    /** Private assignment operator, no assignment of object allowed */
    ::ClockGenerator& operator=(const ::ClockGenerator&);
};

#endif //~CLOCKGENERATOR_H
