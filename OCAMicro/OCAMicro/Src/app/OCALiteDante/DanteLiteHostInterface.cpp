/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */


#ifdef _WIN32
#pragma comment(lib, "AdvAPI32.lib")
#pragma comment(lib, "user32.lib") 
#pragma comment(lib, "Ole32.lib") 
#pragma comment(lib, "Shell32.lib") 
#endif

#include <audinate/dante_api.h>

#include "DanteLiteHostInterface.h"
#include "DanteLiteConMon.h"
#include <OCC/ControlClasses/Workers/BlocksAndMatrices/OcaLiteBlock.h>
#include "OcaLiteDanteMediaClock.h"

static aud_env_t* gs_audEnv(NULL);
static dr_devices_t* gs_devices(NULL);
static dr_device_t* gs_device(NULL);
static conmon_client_t* gs_conmon(NULL);
static dante_sockets_t gs_sockets;

static aud_bool_t gs_bConmonConnected(AUD_FALSE);

static dante_request_id_t gs_requestId;
static aud_bool_t gs_bDrDeviceComm(AUD_FALSE);

typedef struct sSystemInterface
{
	::OcaLiteString ipaddr;
	::OcaLiteString gatewayaddr;
	::OcaLiteString dnsaddr;
	::OcaLiteString maskaddr;
	::OcaBoolean linkup;
	::OcaUint64 linkspeed;
	::OcaUint8 macaddr[6];
};

struct sSystemInterface interfaces[2];

static ::OcaONo OcaLiteStreamNetworkDanteONo(0);

static DanteDeviceClocking gs_DeviceClocking;
static DanteClockStatus gs_DeviceClockStatus;
static conmon_info DanteCMCtrlInfo;
static const conmon_networks_t * DanteCMNetworkInfo;
static uint32_t DanteCMMediaClockOjectNumber(0);
static aud_bool_t gs_runningLocal = AUD_TRUE;

extern uint32_t DanteCMhadSampleRateStat;
extern uint32_t DanteCMhadClockStat;
extern uint32_t DanteCMctrlInitialized;

static void UpdateStreamNetworkDanteRxChannels();

aud_bool_t deviceRoutingDump = AUD_FALSE; // AUD_FALSE;

extern "C" {
	void *GetDeviceStatusMessageCB(void);
}

extern "C" {
	void *GetDeviceMeteringMessageCB(void);
}

static void cbDeviceRouterDeviceChanged(dr_device_t *device, dr_device_change_flags_t change_flags);
static void cbDeviceRouterSocketsChanged(const dr_devices_t * devices);

static void conmonCbConnect(conmon_client_t* client, conmon_client_request_id_t requestId, aud_error_t result)
{
	if (AUD_SUCCESS == result)
	{
		gs_bConmonConnected = true;
	}
}

static void handleDrDeviceResponse(dr_device_t* client, dante_request_id_t requestId, aud_error_t result)
{
	if (gs_requestId == requestId)
	{
		gs_bDrDeviceComm = AUD_FALSE;
	}
}

bool DanteLiteHostInterfaceInitializeConMon(char *devName)
{
	const char localDevice[] = "127.0.0.1";
	// clear our globals
	memset(&gs_DeviceClocking, sizeof(DanteDeviceClocking), 0);
	memset(&gs_DeviceClockStatus, sizeof(DanteClockStatus), 0);
	// If devName == NULL then we're controlling the local device
	if(!devName)
		devName = (char *)localDevice;
	// setup the DanteCM
	return DanteCMInitialize(&DanteCMCtrlInfo, devName, GetDeviceStatusMessageCB(), GetDeviceMeteringMessageCB()) == AUD_SUCCESS;
}

// Wait for a remote device to become "resolved", 10 second timeout
aud_error_t DanteLiteWaitforRemoteHostResolved(void)
{
	aud_error_t result;
	const aud_utime_t comms_timeout = {10, 0};
	aud_utime_t now, then;
	aud_utime_get(&now);
	then = now;
	aud_utime_add(&then, &comms_timeout);
	while(aud_utime_compare(&now, &then) < 0)
	{
		// update our sockets
		dante_sockets_clear(&gs_sockets);
		dr_devices_get_sockets(gs_devices, &gs_sockets);
		timeval timeout = {0,1000};
		int select_result = select(gs_sockets.n, &gs_sockets.read_fds, NULL, NULL, &timeout);
		if (select_result < 0)
		{
			result = aud_error_get_last();
		
#ifdef _WIN32
			int errCode  = WSAGetLastError();

			if(!errCode)
			{
				aud_utime_get(&now);
				continue;
			}
#endif
			return result;
		}
		else if (select_result == 0)
		{
			aud_utime_get(&now);
			continue;
		}
		result = dr_devices_process(gs_devices, &gs_sockets);
		if (dr_device_get_state(gs_device) == DR_DEVICE_STATE_RESOLVED)
		{
			return AUD_SUCCESS;
		}
		aud_utime_get(&now);
	}

	return AUD_ERR_DONE;
}

::OcaLiteString DanteLiteHostInterfaceGetIPAddress(OcaUint32 interfaceid)
{
	return interfaces[interfaceid].ipaddr;
}

::OcaLiteString DanteLiteHostInterfaceGetNetMaskAddress(OcaUint32 interfaceid)
{
	return interfaces[interfaceid].maskaddr;
}

::OcaLiteString DanteLiteHostInterfaceGetDNSAddress(OcaUint32 interfaceid)
{
	return interfaces[interfaceid].dnsaddr;
}

::OcaLiteString DanteLiteHostInterfaceGetGatewayAddress(OcaUint32 interfaceid)
{
	return interfaces[interfaceid].gatewayaddr;
}

::OcaUint64 DanteLiteHostInterfaceGetLinkSpeed(OcaUint32 interfaceid)
{
	return interfaces[interfaceid].linkspeed;
}

::OcaUint8 * DanteLiteHostInterfaceGetMACAddress(OcaUint32 interfaceid)
{
	return interfaces[interfaceid].macaddr;
}

::OcaBoolean DanteLiteHostInterfaceGetLinkState(OcaUint32 interfaceid)
{
	return interfaces[interfaceid].linkup;
}

void DanteLiteHostInterfacePopulateSystemInterface(const conmon_networks_t * networks, int interfaceid)
{
			struct in_addr a;
			char in_buf[22], mask_buf[22], gw_buf[22], dns_buf[22];

			const conmon_network_t * n = networks->networks + interfaceid;

			// Convert addresses to strings
			a.s_addr = n->ip_address;
			strcpy ((char *)&in_buf, inet_ntoa (a));
			interfaces[interfaceid].ipaddr = static_cast< ::OcaLiteString>(in_buf);

			a.s_addr = n->netmask;
			strcpy (mask_buf, inet_ntoa (a));
			interfaces[interfaceid].maskaddr = static_cast< ::OcaLiteString>(mask_buf);

			a.s_addr = n->gateway;
			strcpy (gw_buf, inet_ntoa (a));
			interfaces[interfaceid].gatewayaddr = static_cast< ::OcaLiteString>(gw_buf);

			a.s_addr = n->dns_server;
			strcpy (dns_buf, inet_ntoa (a));
			interfaces[interfaceid].dnsaddr = static_cast< ::OcaLiteString>(dns_buf);

			for (uint8_t i=0; i<6; i++)
			{
				interfaces[interfaceid].macaddr[i] = n->mac_address[i];
			}

			interfaces[interfaceid].linkup = n->is_up ? true : false;

			interfaces[interfaceid].linkspeed = n->link_speed;
}

bool DanteLiteHostInterfaceInitialize(char *devName)
{
	::aud_error_t audResult(::aud_env_setup(&gs_audEnv));
	if (AUD_SUCCESS == audResult)
	{
		audResult = ::dr_devices_new(gs_audEnv, &gs_devices);
		if (AUD_SUCCESS == audResult)
		{
			// if no device name is given open locally
			if(!devName)
			{	// open local device
				audResult = ::dr_device_open_local(gs_devices, &gs_device);
				gs_runningLocal = AUD_TRUE;
			}
			else
			{	// open remote device
				dr_devices_set_sockets_changed_callback(gs_devices, cbDeviceRouterSocketsChanged);
				audResult = ::dr_device_open_remote(gs_devices, devName, &gs_device);
				gs_runningLocal = AUD_FALSE;
			}
			dr_device_set_changed_callback(gs_device, cbDeviceRouterDeviceChanged);
			// If we're connecting to a remote device we need to wait until it's "Resolved"
			if(devName)
			{
				if(AUD_SUCCESS != DanteLiteWaitforRemoteHostResolved())
				{
					printf("Failed to connect to remote device %s\n", devName);
					return false;
				}
			}
			if (AUD_SUCCESS == audResult)
			{
				audResult = conmon_client_new(gs_audEnv, &gs_conmon, devName ? "remote" : "local");
			}

			if (AUD_SUCCESS == audResult)
			{
				audResult = conmon_client_connect(gs_conmon, conmonCbConnect, NULL);
				while (!gs_bConmonConnected)
				{
					conmon_client_process(gs_conmon);
				}
			}

			if (AUD_SUCCESS == audResult)
			{
				DanteCMNetworkInfo = conmon_client_get_networks(gs_conmon);

				DanteLiteHostInterfacePopulateSystemInterface(DanteCMNetworkInfo, 0);
				DanteLiteHostInterfacePopulateSystemInterface(DanteCMNetworkInfo, 1);
			}

			if (AUD_SUCCESS == audResult)
			{
				gs_bDrDeviceComm = AUD_TRUE;
				audResult = dr_device_query_capabilities(gs_device, handleDrDeviceResponse, &gs_requestId);

				while (gs_bDrDeviceComm)
				{
					dante_sockets_t sockets;
					dante_sockets_clear(&sockets);
					dr_devices_get_sockets(gs_devices, &sockets);
					dr_devices_process(gs_devices, &sockets);
				}

				if (AUD_SUCCESS == audResult)
				{
					// Mark everything stale
					for (UINT8 componentCounter(0); componentCounter < DR_DEVICE_COMPONENT_COUNT; componentCounter++)
					{
						::dr_device_mark_component_stale(gs_device, static_cast< ::dr_device_component_t>(componentCounter));
					}
				}
			}
		}
	}


	return (AUD_SUCCESS == audResult);
}

static void UpdateComponent(dr_device_component_t component)
{
	if(gs_device) {
		if (dr_device_is_component_stale(gs_device, component))
		{
			deviceRoutingDump && printf("Component %d stale\n", component);
			gs_bDrDeviceComm = AUD_TRUE;
			dr_device_update_component(gs_device, handleDrDeviceResponse, &gs_requestId, component);

			while (gs_bDrDeviceComm)
			{
				dante_sockets_t sockets;
				dante_sockets_clear(&sockets);
				dr_devices_get_sockets(gs_devices, &sockets);
				dr_devices_process(gs_devices, &sockets);
			}
			switch(component) {
			case DR_DEVICE_COMPONENT_RXCHANNELS:
				UpdateStreamNetworkDanteRxChannels();
				break;
			case DR_DEVICE_COMPONENT_TXCHANNELS:
				break;
			case DR_DEVICE_COMPONENT_TXLABELS:
				break;
			case DR_DEVICE_COMPONENT_PROPERTIES:
				break;
			case DR_DEVICE_COMPONENT_TXFLOWS:
				break;
			case DR_DEVICE_COMPONENT_RXFLOWS:
				break;
			default:
				printf("UpdateComponent: Unknown dr_device_component_t (%d)\n", component);
				break;
			}
		}
	}
}

int DanteLiteHostInterfaceGetSocket()
{
	int socketFd = -1;
	if(DanteCMCtrlInfo.running) 
	{
		socketFd = conmon_client_get_socket (DanteCMCtrlInfo.client);
	}
	return socketFd;
}

void DanteLiteHostInterfaceRunWithFdSet(fd_set* readSet)
{
	DanteCMProcessWithFdSet(&DanteCMCtrlInfo, readSet);
	for (int c = 0; c < DR_DEVICE_COMPONENT_COUNT; c++) {
		UpdateComponent((dr_device_component_t)c);
	}
}

void DanteLiteHostInterfaceRun()
{
	DanteCMProcess(&DanteCMCtrlInfo);
	for (int c = 0; c < DR_DEVICE_COMPONENT_COUNT; c++) {
		UpdateComponent((dr_device_component_t)c);
	}
}

void DanteLiteHostInterfaceStop()
{
	DanteCMShutdownConmon(&DanteCMCtrlInfo);
}

bool DanteLiteHostInterfaceSetDanteChannelSubscription(UINT16 channel, const char* subscription)
{
	UpdateComponent(DR_DEVICE_COMPONENT_RXCHANNELS);

	aud_error_t audResult(AUD_ERR_FAULT);

	dr_rxchannel_t** rxChannels;
	uint16_t numChannels;
	if (AUD_SUCCESS == dr_device_get_rxchannels(gs_device, &numChannels, &rxChannels))
	{
		if (numChannels > channel)
		{
			if (NULL == subscription)
			{
				gs_bDrDeviceComm = AUD_TRUE;
				audResult = dr_rxchannel_subscribe(rxChannels[channel], handleDrDeviceResponse, &gs_requestId, NULL, NULL);
			}
			else
			{
				std::string sSubscription(subscription);
				std::string::const_iterator pos(std::find(sSubscription.begin(), sSubscription.end(), '@'));

				if (pos != sSubscription.end())
				{
					std::string portName(sSubscription.substr(0, sSubscription.find("@")));
					std::string nodeName(sSubscription.substr(sSubscription.find("@")+1, sSubscription.size() - 3));
			
					gs_bDrDeviceComm = AUD_TRUE;
					audResult = dr_rxchannel_subscribe(rxChannels[channel], handleDrDeviceResponse, &gs_requestId, nodeName.c_str(), portName.c_str());
				}
				else
				{
					audResult = AUD_ERR_FAULT;
				}
			}

			if (audResult == AUD_SUCCESS)
			{
				while (gs_bDrDeviceComm)
				{
					dante_sockets_t sockets;
					dante_sockets_clear(&sockets);
					dr_devices_get_sockets(gs_devices, &sockets);
					dr_devices_process(gs_devices, &sockets);
				}
			}
			else
			{
				gs_bDrDeviceComm = AUD_TRUE;
			}
		}
	}

	dr_device_mark_component_stale(gs_device, DR_DEVICE_COMPONENT_RXCHANNELS);

	return (AUD_SUCCESS == audResult);
}

const char* DanteLiteHostInterfaceGetDanteChannelSubscription(UINT16 channel)
{
	UpdateComponent(DR_DEVICE_COMPONENT_RXCHANNELS);

	dr_rxchannel_t** rxChannels;
	uint16_t numChannels;
	if (AUD_SUCCESS == dr_device_get_rxchannels(gs_device, &numChannels, &rxChannels))
	{
		return dr_rxchannel_get_subscription(rxChannels[channel]);
	}
	return NULL;
}

UINT32 DanteLiteHostInterfaceGetEncodings(UINT16 channel, bool bRxChannel)
{
	UINT32 result(0);

	//OCAENCODING_PCM16                       = 1,
	//OCAENCODING_PCM24                       = 2,
	//OCAENCODING_PCM32                       = 3,

	if (bRxChannel)
	{
		UpdateComponent(DR_DEVICE_COMPONENT_RXCHANNELS);

		dr_rxchannel_t** rxChannels;
		uint16_t numChannels;
		if (AUD_SUCCESS == dr_device_get_rxchannels(gs_device, &numChannels, &rxChannels))
		{
			for (UINT16 encodingCounter(0); encodingCounter < dr_rxchannel_num_encodings(rxChannels[channel]); encodingCounter++)
			{
				switch (dr_rxchannel_encoding_at_index(rxChannels[channel], encodingCounter))
				{
				case 16:
					result |= 0x1;
					break;
				case 24:
					result |= 02;
					break;
				case 32:
					result |= 0x4;
					break;
				default:
					break;
				}
			}
		}

	}
	else
	{
		UpdateComponent(DR_DEVICE_COMPONENT_TXCHANNELS);

		dr_txchannel_t** txChannels;
		uint16_t numChannels;
		if (AUD_SUCCESS == dr_device_get_txchannels(gs_device, &numChannels, &txChannels))
		{
			for (UINT16 encodingCounter(0); encodingCounter < dr_txchannel_num_encodings(txChannels[channel]); encodingCounter++)
			{
				switch (dr_txchannel_encoding_at_index(txChannels[channel], encodingCounter))
				{
				case 16:
					result |= 0x1;
					break;
				case 24:
					result |= 02;
					break;
				case 32:
					result |= 0x4;
					break;
				default:
					break;
				}
			}
		}
	}

	return result;
}

UINT32 DanteLiteHostInterfaceGetSampleRate(UINT16 channel, bool bRxChannel)
{
	if (bRxChannel)
	{
		UpdateComponent(DR_DEVICE_COMPONENT_RXCHANNELS);

		dr_rxchannel_t** rxChannels;
		uint16_t numChannels;
		if (AUD_SUCCESS == dr_device_get_rxchannels(gs_device, &numChannels, &rxChannels))
		{
			return dr_rxchannel_get_sample_rate(rxChannels[channel]);
		}

	}
	else
	{
		UpdateComponent(DR_DEVICE_COMPONENT_TXCHANNELS);

		dr_txchannel_t** txChannels;
		uint16_t numChannels;
		if (AUD_SUCCESS == dr_device_get_txchannels(gs_device, &numChannels, &txChannels))
		{
			return dr_txchannel_get_sample_rate(txChannels[channel]);
		}
	}

	return 0;
}

const char* DanteLiteHostInterfaceGetDanteChannelName(UINT16 channel, bool bRxChannel)
{
	if (bRxChannel)
	{
		UpdateComponent(DR_DEVICE_COMPONENT_RXCHANNELS);

		dr_rxchannel_t** rxChannels;
		uint16_t numChannels;
		if (AUD_SUCCESS == dr_device_get_rxchannels(gs_device, &numChannels, &rxChannels))
		{
			return dr_rxchannel_get_name(rxChannels[channel]);
		}
	}
	else
	{
		UpdateComponent(DR_DEVICE_COMPONENT_TXCHANNELS);

		dr_txchannel_t** txChannels;
		uint16_t numChannels;
		if (AUD_SUCCESS == dr_device_get_txchannels(gs_device, &numChannels, &txChannels))
		{
			return dr_txchannel_get_canonical_name(txChannels[channel]);
		}
	}

	return "";
}

const char* DanteLiteHostInterfaceGetDanteChannelLabel(UINT16 channel, bool bRxChannel)
{
	if (bRxChannel)
	{
		UpdateComponent(DR_DEVICE_COMPONENT_RXCHANNELS);

		dr_rxchannel_t** rxChannels;
		uint16_t numChannels;
		if (AUD_SUCCESS == dr_device_get_rxchannels(gs_device, &numChannels, &rxChannels))
		{
			return dr_rxchannel_get_name(rxChannels[channel]);
		}
	}
	else
	{
		UpdateComponent(DR_DEVICE_COMPONENT_TXCHANNELS);

		dr_txchannel_t** txChannels;
		uint16_t numChannels;
		if (AUD_SUCCESS == dr_device_get_txchannels(gs_device, &numChannels, &txChannels))
		{
			uint16_t len = 1;
			dr_txlabel_t labels[1];
			if ((AUD_SUCCESS == dr_txchannel_get_txlabels(txChannels[channel], &len, labels)) &&
				(len > 0))
			{
				return labels[0].name;
			}
			else
			{
				return dr_txchannel_get_canonical_name(txChannels[channel]);
			}
		}
	}

	return "";
}


const char* DanteLiteHostInterfaceGetDeviceName(void)
{
	UpdateComponent(DR_DEVICE_COMPONENT_PROPERTIES);
	if(gs_device)
		return dr_device_get_name(gs_device);
	else
		return OcfLiteConfigureGetDeviceName().c_str();
}

bool DanteLiteHostInterfaceGetNumberOfChannels(UINT16& nrRxChannels, UINT16& nrTxChannels)
{
	UpdateComponent(DR_DEVICE_COMPONENT_TXCHANNELS);
	UpdateComponent(DR_DEVICE_COMPONENT_RXCHANNELS);
	nrRxChannels = 0;
	nrTxChannels = 0;
	if(gs_device) {
		nrRxChannels = dr_device_num_rxchannels(gs_device);
		nrTxChannels = dr_device_num_txchannels(gs_device);
	}
	return true;
}

static void cbDeviceRouterDeviceChanged(dr_device_t *device, dr_device_change_flags_t change_flags)
{

	dr_device_change_index_t i;

	(void) device;
	
	deviceRoutingDump && printf("cbDeviceRouterDeviceChanged with changes");
	for (i = 0; i < DR_DEVICE_CHANGE_INDEX_COUNT; i++)
	{
		if (change_flags & (1 << i))
		{
			deviceRoutingDump && printf(" %s", dr_device_change_index_to_string(i));
			if (i == DR_DEVICE_CHANGE_INDEX_STATE)
			{
				dr_device_state_t state = dr_device_get_state(device);
				deviceRoutingDump && printf(" (%s", dr_device_state_to_string(state));
				if (state == DR_DEVICE_STATE_RESOLVED)
				{
					dante_ipv4_address_t addr;
					uint8_t * host = (uint8_t *) &(addr.host);
					dr_device_get_address(device, &addr);
					deviceRoutingDump && printf(" %u.%u.%u.%u:%u", host[0], host[1], host[2], host[3], addr.port);
				}
				deviceRoutingDump && printf(")");
			}
		}
	}
	deviceRoutingDump && printf("\n");
}

void cbDeviceRouterSocketsChanged(const dr_devices_t * devices)
{
	dante_sockets_clear(&gs_sockets);
	aud_error_t result = dr_devices_get_sockets(devices, &gs_sockets);
}

void DanteLiteHandleClockStatus(const conmon_message_body_t *aud_msg)
{
	conmon_audinate_servo_state_t	servo_state   = conmon_audinate_clocking_status_get_servo_state(aud_msg);
	conmon_audinate_clock_source_t	clock_source  = conmon_audinate_clocking_status_get_clock_source(aud_msg);
	conmon_audinate_clock_state_t	clock_state   = conmon_audinate_clocking_status_get_clock_state(aud_msg);
	//const char *domainName = conmon_audinate_clocking_status_get_subdomain_name(aud_msg);
	conmon_audinate_ext_wc_state_t  ext_wc_state  = conmon_audinate_clocking_status_get_ext_wc_state(aud_msg);
	uint16_t mute_state = conmon_audinate_clocking_status_get_mute_flags(aud_msg);

	// we'll assume there's only one clock
	const conmon_audinate_port_status_t * port_0_status = conmon_audinate_clocking_status_port_at_index(aud_msg, 0);
	// we'll assume there's only one clock if not bkn-2 with redundant NICs
	const conmon_audinate_port_status_t * port_1_status = conmon_audinate_clocking_status_port_at_index(aud_msg, 1);

	if (port_0_status) {
		gs_DeviceClockStatus.ClockPortState[0] = conmon_audinate_port_status_get_port_state(port_0_status, aud_msg);
		if(gs_DeviceClockStatus.ClockPortState[0] == CONMON_AUDINATE_PORT_STATE_MASTER) {
			gs_DeviceClockStatus.ClockMaster = 1;
		} else {
			gs_DeviceClockStatus.ClockMaster = 0;
		}
	} else {
		gs_DeviceClockStatus.ClockMaster = 0;
	}

	if (!gs_DeviceClockStatus.ClockMaster && port_1_status) {
		conmon_audinate_port_state_t state1 = conmon_audinate_port_status_get_port_state(port_1_status, aud_msg);
		if(state1 == CONMON_AUDINATE_PORT_STATE_MASTER) {
			gs_DeviceClockStatus.ClockMaster = 1;
			deviceRoutingDump && printf("handleClockStatus: setting Master state from Secondary Port\n");
		}
	}

	if ((mute_state != gs_DeviceClockStatus.ClockMuteState ) ||
		(clock_source != gs_DeviceClockStatus.ClockSource) ||
		(clock_state != gs_DeviceClockStatus.ClockState	) ||
		(ext_wc_state != gs_DeviceClockStatus.ClockExternalWordClockState))
	{
		deviceRoutingDump && printf("handleClockStatus: Servo (%d) = %s (%s), src = %d, Status = 0x%04x, Mute = 0x%04x ExtWc = %d\n", 
			servo_state, gs_DeviceClockStatus.ClockLocked ? "Locked" : "Unlocked", 
			gs_DeviceClockStatus.ClockMaster ? "Master" : "Slave", 
			clock_source, clock_state, mute_state, ext_wc_state );
	}
	gs_DeviceClockStatus.ClockSource				 = clock_source;
	gs_DeviceClockStatus.ClockState					 = clock_state;
	gs_DeviceClockStatus.ClockMuteState				 = mute_state;
	gs_DeviceClockStatus.ClockExternalWordClockState = ext_wc_state;

	if( ((servo_state == CONMON_AUDINATE_SERVO_STATE_SYNC) || (servo_state == CONMON_AUDINATE_SERVO_STATE_SYNCING)) &&
		(mute_state == 0) ) {
		gs_DeviceClockStatus.ClockLocked = 1;
	} else if(mute_state != 0) {
		gs_DeviceClockStatus.ClockLocked = 0;
	} else {
		gs_DeviceClockStatus.ClockLocked = 1;
	}
	DanteCMhadClockStat = 1;
	// If we're running on Windows with DVS we'll only get this clock call-back
	// On a Brooklyn module we'll get this and DanteLiteHandleSampleRateStatus()
#ifdef WIN32
	// only need to do this if we're not remotely connected to a Brooklyn module
	if(gs_runningLocal == AUD_TRUE)
		DanteCMhadSampleRateStat = 1;
#endif
	if(DanteCMhadSampleRateStat & !DanteCMctrlInitialized) {
		DanteCMctrlInitialized = 1;
	}
	// TODO: Need to update OCA media clock with changes
}

void DanteLiteHandleSampleRateStatus(const conmon_message_body_t *aud_msg)
{
	uint32_t value;
	value = conmon_audinate_srate_get_current(aud_msg);
	if(gs_DeviceClocking.CurrentSampleRate != value) {
		gs_DeviceClocking.CurrentSampleRate = value;
		deviceRoutingDump && printf("Sample Rate = %u\n", value);
		// TODO: Need to update OCA media clock with change
		if(DanteCMMediaClockOjectNumber) {
			OcaLiteRoot *mediaClock = ::OcaLiteBlock::GetRootBlock().GetObject(DanteCMMediaClockOjectNumber);
			reinterpret_cast<DanteLiteOcaMediaClock *>(mediaClock)->setNewDanteSampleRate(value);
		}
	}
	value = conmon_audinate_srate_get_available_count(aud_msg);
	if(gs_DeviceClocking.NumberOfSupportedRates != value) {
		deviceRoutingDump && printf("Number of Supported Sample Rates = %u\n", value);
		gs_DeviceClocking.NumberOfSupportedRates = value;
		for(uint32_t i = 0; i < value; i++) {
			gs_DeviceClocking.SupportedSampleRates[i] = conmon_audinate_srate_get_available(aud_msg, i);
			deviceRoutingDump && printf("Rate[%d]: %u\n", i, gs_DeviceClocking.SupportedSampleRates[i]);
		}
		// TODO: Need to update OCA media clock with change
	}
	DanteCMhadSampleRateStat = 1;
	if(DanteCMhadClockStat & !DanteCMctrlInitialized) {
		DanteCMctrlInitialized = 1;
	}
}

pDanteClockStatus DanteLiteHostInterfaceGetDanteDanteDanteClockStatus()
{
	return &gs_DeviceClockStatus;
}

pDanteDeviceClocking DanteLiteHostInterfaceGetDanteDanteDeviceClocking()
{
	return &gs_DeviceClocking;
}

void DanteLiteHostInterfaceSetMediaClockObjectNumber(uint32_t objNumber)
{
	DanteCMMediaClockOjectNumber = objNumber;
}

bool DanteLiteHostInterfaceSetSampleRate(uint32_t srate)
{
	uint16_t msg_size;
	uint32_t mode;
	aud_error_t rc;
	conmon_message_body_t aud_msg;
	// check if the sample rate is different
	if(srate == gs_DeviceClocking.CurrentSampleRate)
		return true;
	conmon_audinate_init_srate_control (&aud_msg, /*CongestionDelay_uS*/0);
	conmon_audinate_srate_control_set_rate(&aud_msg, srate);
	msg_size = conmon_audinate_srate_control_get_size(&aud_msg);
	mode = (uint32_t)conmon_audinate_srate_get_mode(&aud_msg);
	rc = DanteCMSendAudinateMessage(&aud_msg, msg_size);
	return (rc == AUD_SUCCESS);
}

void DanteLiteHostInterfaceRouterMarkComponentStale(dr_device_component_t devComponent)
{
	if(gs_device && (devComponent < DR_DEVICE_COMPONENT_COUNT))
		::dr_device_mark_component_stale(gs_device, devComponent);
}

void DanteLiteHostInterfaceSetStreamNetworkObject(::OcaONo snONo)
{
	OcaLiteStreamNetworkDanteONo = snONo;
}

void UpdateStreamNetworkDanteTxChannels()
{
}

// Update the OcaLiteNetworkSignalChannelDante sink with the new subscription
void UpdateStreamNetworkDanteRxChannels()
{
	unsigned int n = dr_device_num_rxchannels(gs_device);
	OcaLiteStreamNetworkDante *pSNDante = reinterpret_cast<OcaLiteStreamNetworkDante *>(::OcaLiteBlock::GetRootBlock().GetObject(OcaLiteStreamNetworkDanteONo));

	if(!pSNDante || !DanteCMctrlInitialized)
		return;
	for(unsigned int chan = 0; chan < n; chan++) {
		// get each channels subscription and pass to the OcaLiteStreamNetworkDante object to sort out
		dr_rxchannel_t * rxc = dr_device_rxchannel_at_index(gs_device, chan);
		const char * sub = dr_rxchannel_get_subscription(rxc);
		if(pSNDante->CheckforNewSubscription(chan, sub)) {
			deviceRoutingDump && printf("Rx Subscription Updated for channel %d to %s\n", chan, sub);
		}
	}

}
