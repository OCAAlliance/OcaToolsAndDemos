/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : OcaLite Ocp1Service implementation.
 *
 */

// ---- Include system wide include files ----
#include <common/OcaLite/tinymdns/tinymDNSWrapper.h>
#include <HostInterfaceLite/OCA/OCP.1/ZeroConf/IOcp1LiteService.h>

// ---- FileInfo Macro ----

// ---- Include local include files ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Local data ----

// ---- Class Implementation ----

bool Ocp1LiteServiceRegister(const std::string& name, const std::string& registrationType,
                             UINT16 port, const std::vector<std::string>& txtRecordList, const std::string& domain)
{
    return ::tinymDNSWrapper::GetInstance().Register(name, registrationType, port, txtRecordList, domain);
}

void Ocp1LiteServiceDispose(void)
{
    ::tinymDNSWrapper::GetInstance().Unregister();
}
