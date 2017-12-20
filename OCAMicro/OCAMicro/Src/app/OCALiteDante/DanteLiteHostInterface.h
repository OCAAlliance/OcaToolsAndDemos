/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 *
 *  Description         : The dante configuration interface of the Host Interface.
 *
 */

#ifndef DANTELITEHOSTINTERFACE_H
#define DANTELITEHOSTINTERFACE_H 

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>
#include "audinate/dante_api.h"

// ---- Include local include files ----

// ---- Referenced classes and types ----
#include "OcaLiteStreamNetworkDante.h"

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

typedef struct _DanteDeviceClocking
{
    uint32_t CurrentSampleRate;
    uint32_t NumberOfSupportedRates;
    uint32_t SupportedSampleRates[10];
} DanteDeviceClocking, *pDanteDeviceClocking;

typedef struct _DanteClockStatus
{
    conmon_audinate_servo_state_t     ServoState;
    conmon_audinate_clock_source_t    ClockSource;
    conmon_audinate_clock_state_t     ClockState;
    conmon_audinate_ext_wc_state_t    ClockExternalWordClockState;
    char *                            DomainName;
    conmon_audinate_port_state_t      ClockPortState[2];
    uint32_t                          ClockMaster;
    uint16_t                          ClockMuteState;
    uint32_t                          ClockLocked;
} DanteClockStatus, *pDanteClockStatus;

/**
 * Initialize the Dante hostinterface object by creating
 * a connection to the dante daemons.
 *
 * @return Indicates whether successfully a connection has been setup 
 *         to the dante daemons
 */
bool DanteLiteHostInterfaceInitialize(char *devName);
bool DanteLiteHostInterfaceInitializeConMon(char *devName);

/**
 * Get the number of receive and transmit channels
 * available in this device 
 *
 * @param[out] nrRxChannels The number of receive channels
 * @param[out] nrTxChannels The number of transmit channels
 *
 * @return The result of the operation
 */
bool DanteLiteHostInterfaceGetNumberOfChannels(UINT16& nrRxChannels, UINT16& nrTxChannels);

/**
 * Set the dante channel subscription
 *
 * @param[in] channel       The receive channel number for which the subscription should be updated
 * @param[in] subscription  The subscription in the form <channel>@<node>. NULL if the subscription should be deleted.
 *
 * @return The result of the operation
 */
bool DanteLiteHostInterfaceSetDanteChannelSubscription(UINT16 channel, const char* subscription);

/**
 * Get the dante channel name
 *
 * @param[in] channel       The channel number
 * @param[in] bRxChannel    True for a receive channel, false for a transmit channel.
 *
 * @return The channel name
 */
const char* DanteLiteHostInterfaceGetDanteChannelName(UINT16 channel, bool bRxChannel);

/**
 * Get the dante channel label
 *
 * @param[in] channel       The channel number
 * @param[in] bRxChannel    True for a receive channel, false for a transmit channel.
 *
 * @return The channel label
 */
const char* DanteLiteHostInterfaceGetDanteChannelLabel(UINT16 channel, bool bRxChannel);

/**
 * Get the current dante channel subscription
 *
 * @param[in] channel       The channel number
 *
 * @return The channel subscription
 */
const char* DanteLiteHostInterfaceGetDanteChannelSubscription(UINT16 channel);

/**
 * Get the current dante device name
 *
 * @return The device name
 */
const char* DanteLiteHostInterfaceGetDeviceName(void);

/**
 * Get the sampe rate
 *
 * @param[in] channel       The channel number
 * @param[in] bRxChannel    True for a receive channel, false for a transmit channel.
 *
 * @return The sample rate
 */
UINT32 DanteLiteHostInterfaceGetSampleRate(UINT16 channel, bool bRxChannel);

/**
 * Get the encodings
 *
 * @param[in] channel       The channel number
 * @param[in] bRxChannel    True for a receive channel, false for a transmit channel.
 *
 * @return The encodings, 0x1 for PCM16, 0x2 for PCM24, 0x4 for PCM32.
 */
UINT32 DanteLiteHostInterfaceGetEncodings(UINT16 channel, bool bRxChannel);

/**
 * Set the object number of ourSetStreamNetworkObject
 *
 * @return
 */
void DanteLiteHostInterfaceSetStreamNetworkObject(::OcaONo snONo);

/**
 * Get the dante clocking
 *
 * @return The clock object
 */
pDanteDeviceClocking DanteLiteHostInterfaceGetDanteDanteDeviceClocking();

/**
 * Get the dante clocking status
 *
 * @return The clock status object
 */
pDanteClockStatus DanteLiteHostInterfaceGetDanteDanteDanteClockStatus();

/**
 * Set the object number of our Media Clock (call at .Initialize())
 *
 * @return
 */
void DanteLiteHostInterfaceSetMediaClockObjectNumber(uint32_t objNumber);

/**
 * Set Units Physical sample rate from the OCA Media Clock
 *
 * @param[in] channel       The new Sample Rate
 *
 * @return true on success
 */
bool DanteLiteHostInterfaceSetSampleRate(uint32_t srate);

/**
 * Perform a run.
 */
void DanteLiteHostInterfaceRun();

/**
 * Stop.
 */
void DanteLiteHostInterfaceStop();

/** 
 * Get the socket to process to the dante layer.
 *
 * @return -1 if no socket exists, else the socket fd.
 */
int DanteLiteHostInterfaceGetSocket();

/**
 * Run on bases of the passed fd set.
 */ 
void DanteLiteHostInterfaceRunWithFdSet(fd_set* readSet);
void DanteCMForceUpdate(void);


/**
 * Get the ip address for a system interface
 *
 * @param[in] interfaceid       The interface channel index
 *
 * @return IP address as a string.
 */
::OcaLiteString DanteLiteHostInterfaceGetIPAddress(OcaUint32 interfaceid);

/**
 * Get the mask address for a system interface
 *
 * @param[in] interfaceid       The interface channel index
 *
 * @return Mask address as a string.
 */
::OcaLiteString DanteLiteHostInterfaceGetNetMaskAddress(OcaUint32 interfaceid);

/**
 * Get the dns address for a system interface
 *
 * @param[in] interfaceid       The interface channel index
 *
 * @return DNS address as a string.
 */
::OcaLiteString DanteLiteHostInterfaceGetDNSAddress(OcaUint32 interfaceid);

/**
 * Get the gateway address for a system interface
 *
 * @param[in] interfaceid       The interface channel index
 *
 * @return Gateway address as a string.
 */
::OcaLiteString DanteLiteHostInterfaceGetGatewayAddress(OcaUint32 interfaceid);

/**
 * Get the link speed for a system interface
 *
 * @param[in] interfaceid       The interface channel index
 *
 * @return Link speed.
 */
::OcaUint64 DanteLiteHostInterfaceGetLinkSpeed(OcaUint32 interfaceid);

/**
 * Get the mac address for a system interface
 *
 * @param[in] interfaceid       The interface channel index
 *
 * @return Pointer to mac address array.
 */
::OcaUint8 * DanteLiteHostInterfaceGetMACAddress(OcaUint32 interfaceid);

/**
 * Get the link state for a system interface
 *
 * @param[in] interfaceid       The interface channel index
 *
 * @return Link state as a bool.
 */
::OcaBoolean DanteLiteHostInterfaceGetLinkState(OcaUint32 interfaceid);

#endif // DANTELITEHOSTINTERFACE_H
