/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 */

// OCALite.cpp : Defines the entry point for the console application.
//


#include <HostInterfaceLite/OCA/OCF/OcfLiteHostInterface.h>
#include <HostInterfaceLite/OCA/OCP.1/Ocp1LiteHostInterface.h>
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include <OCC/ControlClasses/Managers/OcaLiteDeviceManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteNetworkManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteSubscriptionManager.h>
#include <OCC/ControlClasses/Managers/OcaLiteFirmwareManager.h>
#include <OCF/OcaLiteCommandHandler.h>
#include <OCP.1/Ocp1LiteNetwork.h>
#include <OCP.1/Ocp1LiteNetworkSystemInterfaceID.h>
#include <StandardLib/StandardLib.h>

#ifdef OCA_RUN
extern void Ocp1LiteServiceRun();
#else
extern void Ocp1LiteServiceRunWithFdSet(fd_set* readSet);
extern int Ocp1LiteServiceGetSocket();
#endif

int main(int argc, const char* argv[])
{
    // Initialize the host interfaces
    bool bSuccess = ::OcfLiteHostInterfaceInitialize();
    bSuccess = bSuccess && ::Ocp1LiteHostInterfaceInitialize();

    // Initialize Oca Device
    static_cast<void>(::OcaLiteBlock::GetRootBlock());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteNetworkManager::GetInstance().Initialize());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteSubscriptionManager::GetInstance().SetNrEvents(1/*OCA_NR_EVENTS*/));
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteSubscriptionManager::GetInstance().Initialize());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteDeviceManager::GetInstance().Initialize());
    bSuccess = bSuccess && static_cast<bool>(::OcaLiteFirmwareManager::GetInstance().Initialize());

    if (bSuccess)
    {
        Ocp1LiteNetworkSystemInterfaceID interfaceId = ::Ocp1LiteNetworkSystemInterfaceID(static_cast< ::OcaUint32>(0));
        std::vector<std::string> txtRecords;
        txtRecords.push_back("modelGUID=DEADBEEFEATERS");
        ::OcaLiteString nodeId = ::OcaLiteString("OCALite@" + OcfLiteConfigureGetDeviceName());
        ::Ocp1LiteNetwork* ocp1Network = new ::Ocp1LiteNetwork(static_cast< ::OcaONo>(9000), static_cast< ::OcaBoolean>(true), 
                                                               ::OcaLiteString("Ocp1LiteNetwork"), ::Ocp1LiteNetworkNodeID(nodeId),
                                                               interfaceId, txtRecords, ::OcaLiteString("local"), OcaInt16(65000U));
        if (ocp1Network->Initialize())
        {
            if (::OcaLiteBlock::GetRootBlock().AddObject(*ocp1Network))
            {
                OcaLiteStatus rc(ocp1Network->Startup());
                if (OCASTATUS_OK == rc)
                {
                    bSuccess = bSuccess && ::OcaLiteCommandHandler::GetInstance().Initialize();
                    ::OcaLiteDeviceManager::GetInstance().SetErrorAndOperationalState(static_cast< ::OcaBoolean>(!bSuccess), ::OcaLiteDeviceManager::OCA_OPSTATE_OPERATIONAL);
                    ::OcaLiteDeviceManager::GetInstance().SetEnabled(static_cast< ::OcaBoolean>(bSuccess));

                    while (bSuccess)
                    {
#ifdef OCA_RUN
                        ::OcaLiteCommandHandler::GetInstance().RunWithTimeout(1000);
                        Ocp1LiteServiceRun();
#else
                        int highestFd(-1);
                        fd_set readset;
                        fd_set writeset;
                        fd_set exceptset;
                        FD_ZERO(&readset);
                        FD_ZERO(&writeset);
                        FD_ZERO(&exceptset);

                        bool bShortSelect(::OcaLiteCommandHandler::GetInstance().AddSelectables(highestFd, readset, writeset, exceptset));

                        int serviceFd(Ocp1LiteServiceGetSocket());
                        if (-1 != serviceFd)
                        {
                            FD_SET(serviceFd, &readset);
                            if (highestFd < serviceFd)
                            {
                                highestFd = serviceFd;
                            }
                        }

                        timeval timeout = { bShortSelect ? 1 : 0, 0} ;
                        select(highestFd + 1, &readset, &writeset, &exceptset, &timeout);

                        ::OcaLiteCommandHandler::GetInstance().RunWithSelectSet(readset, writeset, exceptset);
                        if (FD_ISSET(serviceFd, &readset))
                        {
                            Ocp1LiteServiceRunWithFdSet(&readset);
                        }
#endif
                    }
                }
            }
        }
    }

    

    return 0;
}
