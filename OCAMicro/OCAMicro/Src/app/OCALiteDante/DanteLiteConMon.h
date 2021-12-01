/*
 *  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located in the LICENSE file in the root of this project
 *  as an original contracting party.
 *
 *  Description         : The Dante ConMon Implementation Interface
 *
 */

#ifndef DANTELITECONMON_H
#define DANTELITECONMON_H 

// ---- Include system wide include files ----
#include <PlatformDataTypes.h>
#include "audinate/dante_api.h"

// ---- Include local include files ----

// ---- Referenced classes and types ----

// ---- Helper types and constants ----

// ---- Helper functions ----

// ---- Class Definition ----

#ifdef __cplusplus
extern "C" {
#endif

enum
{
	LISTEN_MAX_TARGETS = 2,
	FILTER_MAX_FILTERS = 2
};

typedef enum message_filter_mode
{
	MESSAGE_FILTER_MODE_PASS = 0,
	MESSAGE_FILTER_MODE_FAIL
} message_filter_mode_t;

typedef enum print_mode_raw
{
	PRINT_MODE_RAW_NONE = 0,
		// Don't print raw messages
	PRINT_MODE_RAW_UNKNOWN,
		// Print raw messages if printing fails
	PRINT_MODE_RAW_ALWAYS
		// Always print raw messages
} print_mode_raw_t;

typedef struct _conmon_target
{
	const char * name;
	const conmon_client_subscription_t * sub;
	conmon_rxstatus_t old_status;
	aud_bool_t found;

	conmon_client_request_id_t req_id;

	const conmon_instance_id_t * conmon_id;
	char id_buf [64];
} conmon_target, *pconmon_target;

typedef struct _conmon_info
{
	// conmon connectivity
	aud_env_t * env;
	uint16_t server_port;
	conmon_client_t * client;
	
	// runtime
	aud_bool_t running;
	int result;
	
	// Target devices
	aud_bool_t all;
	conmon_client_request_id_t all_req_id;

	// options
	aud_bool_t quiet;
	
	unsigned int n_targets;
	conmon_target targets [LISTEN_MAX_TARGETS];

	unsigned n_filters;
	message_filter_mode_t filter_mode;
	struct message_filter
	{
		conmon_audinate_message_type_t mtype;
	} filter [FILTER_MAX_FILTERS];

	struct conmon_info_raw
	{
		print_mode_raw_t mode;
		aud_bool_t offsets;
		unsigned n_filters;
		struct message_filter filter[FILTER_MAX_FILTERS];
	} raw;
} conmon_info, *pconmon_info;

int DanteCMInitialize(conmon_info * cm, char *target, void *vendorStatusCallback, void *vendorMeteringCallback);
int DanteCMProcess(conmon_info * cm);
int DanteCMProcessWithFdSet(conmon_info * cm, fd_set* readSet);
aud_error_t DanteCMShutdownConmon(conmon_info * cm);
void DanteCMGetClockInformation(void);
int DanteCMSendAudinateMessage(conmon_message_body_t *msg, uint32_t size);
int DanteCMSendVendorMessage(conmon_message_body_t *msg, uint32_t size, const conmon_vendor_id_t * vendor_id);

#ifdef __cplusplus
}
#endif

#endif // DANTELITECONMON_H
