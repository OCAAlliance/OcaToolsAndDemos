/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The Service interface of the Host Interface.
 *
 */

#ifndef IOCP1LITESERVICE_H
#define IOCP1LITESERVICE_H

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>

// ---- Include local include files ----
#include "../Ocp1LiteHostInterfaceDataTypes.h"

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

/**
 * Register a service with the mDNSResponder daemon. The service
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
 * @return ZC_SC_NO_ERROR indicates registration was successfully initiated
 *         (the callback can still indicate asynchronous errors that
 *         occurred though). The other possible return values indicate
 *         which error occurred.
 */
bool Ocp1LiteServiceRegister(const std::string& name, const std::string& registrationType,
                             UINT16 port, const std::vector<std::string>& txtRecordList, const std::string& domain);

 /**
 * Dispose of the service. This will terminate its connection
 * with the mDNSResponder daemon.
 */
void Ocp1LiteServiceDispose(void);


#endif // IOCP1LITESERVICE_H
