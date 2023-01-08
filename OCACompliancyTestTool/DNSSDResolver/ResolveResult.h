/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : The ResolveResult class.
 */
#ifndef RESOLVE_RESULT_H
#define RESOLVE_RESULT_H

 // ---- Include system wide include files ----
#include <PlatformDataTypes.h>

 // ---- Include local include files ----

 // ---- Referenced classes and types ----

 // ---- Helper types and constants ----

 // ---- Helper functions ----

 // ---- Class Definition ----
/**
 * Storage helper for the resolve result.
 */
class ResolveResult
{
public:
    /** Constructor */
    ResolveResult();

    /** 
     * Store the resolve result.
     *
     * @param[in] interfaceindex    The interface index.
     * @param[in] fullName          The fullname.
     * @param[in] hosttarget        The host name.
     * @param[in] port              The port number
     * @param[in] txtLength         The text record length.
     * @param[in] txtRecord         The text record in standard txt record format.
     */
    void SetResolveResult(UINT32 interfaceIndex, const char* fullName, const char* hosttarget,
                          UINT16 port, UINT16 txtLength, const unsigned char* txtRecord);

    /**
     * Get the interface index.
     *
     * @return m_interfaceIndex
     */
    UINT32 GetInterfaceIndex() const;

    /**
     * Get the fullname.
     *
     * @return m_fullName.
     */
    const std::string& GetFullName() const;

    /**
     * Get the hosttarget / hostname.
     *
     * @return m_hosttarget.
     */
    const std::string& GetHostTarget() const;

    /**
     * Get the port.
     *
     * @return m_port.
     */
    UINT16 GetPort() const; 

    /**
     * Get txt records.
     *
     * @return m_txtRecords
     */
    const std::vector<std::string>& GetTxtRecords() const; 

protected:
private:
    /**
     * Parse the txt records from a standard txt record format into the m_txtRecrods member.
     *
     * @param[in] txtLength     The length of the txt record
     * @param[in] txtRecord     Pointer to the array containing the txt record.
     */
    void ParseTxtRecords(UINT16 txtLength, const unsigned char* txtRecord);

    /** The interface index */
    UINT32                      m_interfaceIndex;
    /** The fullname */
    std::string                 m_fullName;
    /** The host target */
    std::string                 m_hostTarget;
    /** The port */
    UINT16                      m_port;
    /** Vector with txt records */
    std::vector<std::string>    m_txtRecords;
};

#endif //RESOLVE_RESULT_H
