/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

// DanteLiteConMon.cpp : Dante Connection Manager for local and remote subscriptions
//

#ifdef WIN32
#define SNPRINTF _snprintf
#define STRCASECMP _stricmp
#else
#include <libgen.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SNPRINTF snprintf
#define STRCASECMP strcasecmp
#endif

#include <stdio.h>
#include "DanteLiteConMon.h"
#include "DanteLiteHostInterface.h"


// external [routing] functions
extern void DanteLiteHandleClockStatus(const conmon_message_body_t *aud_msg);
extern void DanteLiteHandleSampleRateStatus(const conmon_message_body_t *aud_msg);
extern void DanteLiteHostInterfaceRouterMarkComponentStale(dr_device_component_t devComponent);

static const conmon_vendor_id_t AUDINATE_VENDOR_ID = {{'A', 'u', 'd', 'i', 'n', 'a', 't', 'e'}};
const char myself[] = {"127.0.0.1"};

// Inet address buffer
#ifndef INET_ADDRSTRLEN
    #define INET_ADDRSTRLEN 16
#endif

typedef conmon_target conmon_target_t;
typedef conmon_info conmon_info_t;

// local data
typedef char inet_buf_t [INET_ADDRSTRLEN];
uint32_t DanteCMmMessageID;
static uint32_t DanteCMctrlConnectRetries = 0;
static uint32_t DanteCMctrlConnected      = 0;
uint32_t        DanteCMctrlInitialized    = 0;
uint32_t        DanteCMhadSampleRateStat  = 0;
uint32_t        DanteCMhadClockStat          = 0;

static conmon_info *pDanteCMCtrlInfo = NULL;
static dante_sockets_t DanteCMClientSockets;


#define kDanteCMctrlConnectRetries        (20)
static char DanteCMControlledDeviceName[128];


static void DanteCMTimestampError(void);
static void DanteCMTimestampEvent(void);
static void DanteCMPrintNetworks(const conmon_networks_t * networks, const char * prefix);

static aud_error_t DanteCMSetupConmon (conmon_info * cm);
static aud_error_t DanteCMRetryConmon(conmon_info * cm);

static aud_error_t register_for_events(conmon_info * cm);
static aud_error_t DanteCMSubscribeToRemoteClients(conmon_info * cm);

static conmon_client_response_fn                            DanteCMCbRegConnect;
static conmon_client_response_fn                            DanteCMCbRegMonitoring;
static conmon_client_response_fn                            DanteCMCbRegSub, DanteCMCbRegSubAll;
static conmon_client_handle_networks_changed_fn                DanteCMCbNetwork;
static conmon_client_handle_dante_device_name_changed_fn    DanteCMCbDanteDeviceName;
static conmon_client_handle_dns_domain_name_changed_fn        DanteCMCbDnsDomainName;
static conmon_client_handle_monitoring_message_fn            DanteCMCbMonitoring;
static conmon_client_handle_subscriptions_changed_fn        DanteCMCbCMSubChanged;
static conmon_client_handle_monitoring_message_fn            *pDanteCMCbMonitoringVendor(NULL);
static conmon_client_handle_monitoring_message_fn            DanteCMCbMetering;
static conmon_client_handle_monitoring_message_fn            *pDanteCMCbMeteringVendor(NULL);

aud_bool_t deviceConMonDump = AUD_FALSE; // AUD_FALSE;

#ifdef DANTE_CM_METERING_SUBSCRIPTIONS

static void DanteCMCbMetering(conmon_client_t * client, 
                                conmon_channel_type_t type, 
                                conmon_channel_direction_t channel_direction, 
                                const conmon_message_head_t * head, 
                                const conmon_message_body_t * body)
{
    if(pDanteCMCbMeteringVendor)
        pDanteCMCbMeteringVendor(client, type, channel_direction, head, body);
}

#endif // DANTE_CM_METERING_SUBSCRIPTIONS

static void DanteLiteStatusMessageCallback(conmon_client_t * client, conmon_channel_type_t channel_type, 
                                            conmon_channel_direction_t channel_direction, 
                                            const conmon_message_head_t * head, 
                                            const conmon_message_body_t * body);

AUD_INLINE const char *DanteCMPName(void)
{
    return "DanteLiteConMon";
}

static void DanteCMTimestampError()
{
    aud_ctime_buf_t buf;
    printf("#ERROR %s: ", aud_utime_ctime_no_newline (NULL, buf));
}

static void DanteCMTimestampEvent()
{
    aud_ctime_buf_t buf;
    printf("#EVENT %s: ", aud_utime_ctime_no_newline (NULL, buf));
}

static void DanteCMCbDnsDomainName (conmon_client_t * client)
{
    DanteCMTimestampEvent();
    fprintf (stdout,
        "DNS domain name changed to '%s'\n"
        , conmon_client_get_dns_domain_name (client)
    );
}

static conmon_target *DanteCMTargetForSub(conmon_info * cm, const conmon_client_subscription_t * sub)
{
    unsigned int i;
    conmon_target * target;

    for (i = 0; i < cm->n_targets; i++)
    {
        target = (conmon_target *)(&cm->targets[i]);
        if (target->name && (target->sub == sub))
        {
            return target;
        }
    }
    
    return NULL;
}

// A helper function to print an instance id to a string buffer
AUD_INLINE char *DanteCMInstanceIDtoString(const conmon_instance_id_t * id, char * buf, size_t len)
{
    if (id)
    {
        SNPRINTF(buf, len,
            "%02x%02x%02x%02x%02x%02x%02x%02x/%04x",
            id->device_id.data[0], id->device_id.data[1],
            id->device_id.data[2], id->device_id.data[3],
            id->device_id.data[4], id->device_id.data[5],
            id->device_id.data[6], id->device_id.data[7],
            id->process_id);
    }
    else
    {
        SNPRINTF(buf, len, "[null]");
    }
    return buf;
}

AUD_INLINE const char *DanteCMRxStatusToString(conmon_rxstatus_t rxstatus)
{
    switch(rxstatus)
    {
    case CONMON_RXSTATUS_NONE:          return "none";
    case CONMON_RXSTATUS_PREPARING:     return "preparing";
    case CONMON_RXSTATUS_RESOLVED:      return "resolved";
    case CONMON_RXSTATUS_UNRESOLVED:    return "unresolved";
    case CONMON_RXSTATUS_UNICAST:       return "unicast";
    case CONMON_RXSTATUS_MULTICAST:     return "multicast";
    case CONMON_RXSTATUS_NO_CONNECTION: return "no connection";
    case CONMON_RXSTATUS_COMMS_ERROR:   return "comms error";
    case CONMON_RXSTATUS_INVALID_REPLY: return "invalid reply";
    default:                            return "unknown";
    }
}

static void DanteCMCbCMSubChanged (conmon_client_t * client, unsigned int num_changes, const conmon_client_subscription_t * const * changes)
{
    unsigned int i;
    conmon_info * cm = (conmon_info *)conmon_client_context (client);
    
    for (i = 0; i < num_changes; i++)
    {
        const conmon_client_subscription_t * sub = changes [i];
        
        conmon_target *target = DanteCMTargetForSub(cm, sub);
        if (target)
        {
            conmon_rxstatus_t rxstatus =
                conmon_client_subscription_get_rxstatus (sub);

            if (target->old_status == rxstatus)
            {
                continue;
            }
            target->old_status = rxstatus;

            switch (rxstatus)
            {
            case CONMON_RXSTATUS_UNICAST:
            case CONMON_RXSTATUS_MULTICAST:
                if (!target->found)
                {
                    target->found = YES;
                    target->conmon_id = conmon_client_subscription_get_instance_id (sub);
                    DanteCMInstanceIDtoString(
                        target->conmon_id,
                        target->id_buf,
                        sizeof(target->id_buf)
                    );
                    DanteCMTimestampEvent();
                    printf ("Subscription to '%s' active, id=%s\n"
                        , target->name
                        , target->id_buf
                    );
                }
                break;

            case CONMON_RXSTATUS_UNRESOLVED:
                target->found = NO;
                DanteCMTimestampEvent();
                printf ("Subscription to '%s' is now UNRESOLVED\n", target->name);
                break;

            // transient states, don't print anything
            case CONMON_RXSTATUS_PREPARING:
            case CONMON_RXSTATUS_RESOLVED:
                target->found = NO;
                target->id_buf [0] = 0;
                target->conmon_id = NULL;

                DanteCMTimestampEvent();
                printf ("Subscription to '%s' has entered transient state 0x%04x (%s)\n", 
                    target->name, rxstatus, DanteCMRxStatusToString(rxstatus)
                );
                break;

            default:
                target->found = NO;
                target->id_buf [0] = 0;
                target->conmon_id = NULL;

                DanteCMTimestampEvent();
                printf ("Subscription to '%s' has entered error state 0x%04x (%s)\n", 
                    target->name, rxstatus, DanteCMRxStatusToString(rxstatus)
                );
            }
        }
        // else we ignore because we're not interested in this subscription
    }
}

aud_error_t DanteCMShutdownConmon(conmon_info * cm)
{
    if (cm->client) {
        conmon_client_delete(cm->client);
        cm->client = NULL;
    }
    
    if (cm->env) {
        aud_env_release(cm->env);
        cm->env = NULL;
    }
    return AUD_SUCCESS;
}

static aud_error_t DanteCMSetupConmon(conmon_info * cm)
{
    conmon_client_config_t * config = NULL;
    aud_error_t result;
    aud_errbuf_t ebuf;

    cm->env = NULL;
    cm->client = NULL;
    cm->running = NO;
    cm->result = 0;

    result = aud_env_setup (& cm->env);
    if (result != AUD_SUCCESS)
    {
        DanteCMTimestampError();
        printf("%s: conmon failed to initialise environment: %s (%d)\n", DanteCMPName(), aud_error_message(result, ebuf), result);
        return result;
    }

    config = conmon_client_config_new("DanteOCAListener");
    if (!config) {
        DanteCMTimestampError();
        printf("%s: conmon failed to initialise client: NO_MEMORY\n", DanteCMPName());
        goto l__error;
    }
    conmon_client_config_set_metering_channel_enabled(config, AUD_TRUE);
    if (cm->server_port)
    {
        conmon_client_config_set_server_port(config, cm->server_port);
    }
    result = conmon_client_new_config (cm->env, config, &cm->client);
    conmon_client_config_delete(config);
    config = NULL;

    if (result != AUD_SUCCESS)
    {
        DanteCMTimestampError();
        printf("%s: conmon failed to initialise client: %s (%d)\n", DanteCMPName(), aud_error_message(result, ebuf), result);
        goto l__error;
    }

    conmon_client_set_context (cm->client, cm);

    result = conmon_client_connect (cm->client, &DanteCMCbRegConnect, NULL);
    if (result != AUD_SUCCESS) {
        DanteCMTimestampError();
        printf("%s: conmon failed to initiate connection: %s (%d)\n", DanteCMPName(), aud_error_message(result, ebuf), result);
        goto l__error;
    }
    
    return AUD_SUCCESS;

l__error:
    DanteCMShutdownConmon(cm);
    
    return result;
}

// Callbacks
static void DanteCMCbRegConnect(conmon_client_t * client, conmon_client_request_id_t request_id, aud_error_t result)
{
    conmon_info * info = (conmon_info *)conmon_client_context(client);

    (void) request_id;

    if (result == AUD_SUCCESS) {
        const conmon_networks_t * networks;

        DanteCMTimestampEvent();
        printf("RNListener: Conmon connection successful (%d)\n", DanteCMctrlConnectRetries);
        DanteCMctrlConnected = 1;
        
        if (info) {
            register_for_events(info);
            DanteCMSubscribeToRemoteClients(info);
            conmon_client_set_subscriptions_changed_callback (client, DanteCMCbCMSubChanged);
        } else {
            DanteCMTimestampError ();
            fprintf (stderr,
                "%s: no context!\n"
                , DanteCMPName()
            );
        }
        printf("Networks:");
        networks = conmon_client_get_networks(client);
        DanteCMPrintNetworks(networks, " ");
        printf("Dante device name '%s'\n", conmon_client_get_dante_device_name (client));
        printf("DNS domain name '%s'\n", conmon_client_get_dns_domain_name (client));
    }
    else
    {
        aud_errbuf_t ebuf;
        DanteCMTimestampError ();
        printf("RNListener: %s: failed to connect to conmon: %s (%d)\n", DanteCMPName(), aud_error_message (result, ebuf), result);
        DanteCMctrlConnectRetries++;
        if(DanteCMctrlConnectRetries < kDanteCMctrlConnectRetries) {
            aud_utime_t currentTime;
            aud_utime_t connectWait = {0,500000};
            // Retry connect
            aud_utime_get(&currentTime);
            // add timeout
            aud_utime_add(&connectWait, &currentTime);
            printf("RNListener: Retrying connect...\n");
            while(aud_utime_compare(&currentTime, &connectWait) < 0) {
                aud_utime_get(&currentTime);
            }
            conmon_client_disconnect(client);
            DanteCMRetryConmon(info);
        } else {
            printf("RNListener: Connect Retries exceeded, giving up...\n");
        }
    }
}

void DanteCMPrintNetworks(const conmon_networks_t * networks, const char * prefix)
{
    int i = 0;

    if (networks && networks->num_networks)
    {
        if (prefix)
        {
            fputs (prefix, stdout);
        }
        else
        {
            fputs ("[ {", stdout);
        }
        
        for (i = 0; i < networks->num_networks; i++)
        {
            struct in_addr a;
            inet_buf_t in_buf, mask_buf, gw_buf, dns_buf;

            const conmon_network_t * n = networks->networks + i;

            // Convert addresses to strings
            a.s_addr = n->ip_address;
            strcpy (in_buf, inet_ntoa (a));

            a.s_addr = n->netmask;
            strcpy (mask_buf, inet_ntoa (a));

            a.s_addr = n->gateway;
            strcpy (gw_buf, inet_ntoa (a));

            a.s_addr = n->dns_server;
            strcpy (dns_buf, inet_ntoa (a));
            
            // Prefix, if any
            if (i)
            {
                if (prefix)
                {
                    putchar ('\n');
                    fputs (prefix, stdout);
                }
                else
                {
                    fputs ("}, {", stdout);
                }
            }
            
            printf ("%d: %s 0x%08x %d %02x:%02x:%02x:%02x:%02x:%02x addr:%s mask:%s gw:%s dns:%s",
                n->interface_index,
                n->is_up ? "up" : "down", 
                n->flags,
                n->link_speed,
                n->mac_address[0], n->mac_address[1], n->mac_address[2],
                n->mac_address[3], n->mac_address[4], n->mac_address[5],
                in_buf, mask_buf, gw_buf, dns_buf
            );
        }

        if (prefix)
        {
            putchar ('\n');
        }
        else
        {
            fputs ("} ]", stdout);
        }
    }
    else if (! prefix)
    {
        fputs ("[]", stdout);
    }
}

static aud_error_t DanteCMRetryConmon(conmon_info * cm)
{
    aud_error_t result;
    aud_errbuf_t ebuf;

    result = conmon_client_connect(cm->client, &DanteCMCbRegConnect, NULL);
    if (result != AUD_SUCCESS)
    {
        DanteCMTimestampError();
        fprintf (stderr,
            "%s: conmon failed to initiate connection: %s (%d)\n"
            , DanteCMPName()
            , aud_error_message(result, ebuf), result
        );
        goto l__error;
    }
    
    return AUD_SUCCESS;

l__error:
    DanteCMShutdownConmon(cm);
    
    return result;
}

static aud_error_t register_for_events(conmon_info * cm)
{
    aud_error_t result;
    aud_bool_t has_success = AUD_FALSE;

    conmon_client_set_networks_changed_callback (cm->client, &DanteCMCbNetwork);
    
    conmon_client_set_dante_device_name_changed_callback(cm->client, &DanteCMCbDanteDeviceName);
    conmon_client_set_dns_domain_name_changed_callback(cm->client, &DanteCMCbDnsDomainName);

    // Register for outgoing (TX) status messages
    result = conmon_client_register_monitoring_messages (cm->client, &DanteCMCbRegMonitoring, 
                                                            NULL,
                                                            CONMON_CHANNEL_TYPE_STATUS, 
                                                            CONMON_CHANNEL_DIRECTION_TX,
                                                            &DanteCMCbMonitoring);
    if(result == AUD_SUCCESS) {
        has_success = AUD_TRUE;
    } else {
        aud_errbuf_t ebuf;
        DanteCMTimestampError();
        printf("%s: failed to register monitoring channel (status): %s (%d)\n", DanteCMPName(), aud_error_message (result, ebuf), result);
    }

    // Register for outgoing (TX) broadcast messages
    result = conmon_client_register_monitoring_messages(cm->client, &DanteCMCbRegMonitoring, 
                                                        NULL,
                                                        CONMON_CHANNEL_TYPE_BROADCAST, 
                                                        CONMON_CHANNEL_DIRECTION_TX,
                                                        &DanteCMCbMonitoring);
    if(result == AUD_SUCCESS) {
        has_success = AUD_TRUE;
    } else {
        aud_errbuf_t ebuf;
        
        DanteCMTimestampError();
        printf("%s: failed to register monitoring channel (broadcast): %s (%d)\n", DanteCMPName(), aud_error_message (result, ebuf), result);
    }

    // Register for "outgoing" (TX) local messages
    result = conmon_client_register_monitoring_messages(cm->client, &DanteCMCbRegMonitoring, 
                                                        NULL,
                                                        CONMON_CHANNEL_TYPE_LOCAL, 
                                                        CONMON_CHANNEL_DIRECTION_TX, 
                                                        &DanteCMCbMonitoring);
    if(result == AUD_SUCCESS) {
        has_success = AUD_TRUE;
    } else {
        aud_errbuf_t ebuf;
        
        DanteCMTimestampError();
        printf("%s: failed to register monitoring channel (local): %s (%d)\n", DanteCMPName(), aud_error_message (result, ebuf), result);
    }

    // register for incoming (RX) status messages
    result = conmon_client_register_monitoring_messages (cm->client, &DanteCMCbRegMonitoring, 
                                                            NULL,
                                                            CONMON_CHANNEL_TYPE_STATUS, 
                                                            CONMON_CHANNEL_DIRECTION_RX,
                                                            &DanteCMCbMonitoring);
    if(result == AUD_SUCCESS) {
        has_success = AUD_TRUE;
    } else {
        aud_errbuf_t ebuf;
        
        DanteCMTimestampError();
        printf("%s: failed to register external monitoring channel (status): %s (%d)\n", DanteCMPName(), aud_error_message (result, ebuf), result);
    }

    // register for incoming (RX) broadcast messages
    result = conmon_client_register_monitoring_messages(cm->client, &DanteCMCbRegMonitoring, 
                                                        NULL,
                                                        CONMON_CHANNEL_TYPE_BROADCAST, 
                                                        CONMON_CHANNEL_DIRECTION_RX,
                                                        &DanteCMCbMonitoring);
    if(result == AUD_SUCCESS) {
        has_success = AUD_TRUE;
    } else {
        aud_errbuf_t ebuf;
        
        DanteCMTimestampError();
        printf("%s: failed to register external monitoring channel (rx broadcast): %s (%d)\n", DanteCMPName(), aud_error_message (result, ebuf), result);
    }

#ifdef DANTE_CM_METERING_SUBSCRIPTIONS
    // now register for meter messages
    if (!conmon_client_is_metering_channel_active(cm->client)) {
        printf("Metering channel configuration failed\n");
    }
    result = conmon_client_register_monitoring_messages(cm->client, NULL, NULL, CONMON_CHANNEL_TYPE_METERING, CONMON_CHANNEL_DIRECTION_RX, DanteCMCbMetering);
    if (result == AUD_SUCCESS) {
        has_success = AUD_TRUE;
        printf("Registering for RX metering messages\n");
    } else {
        aud_errbuf_t ebuf;
        printf("Error registering for RX metering messages(request): %s\n", aud_error_message(result, ebuf));
    }
    result = conmon_client_register_monitoring_messages(cm->client, NULL, NULL, CONMON_CHANNEL_TYPE_METERING, CONMON_CHANNEL_DIRECTION_TX, DanteCMCbMetering);
    if (result == AUD_SUCCESS) {
        has_success = AUD_TRUE;
        printf("Registering for TX metering messages\n");
    } else {
        aud_errbuf_t ebuf;
        printf("Error registering for TX metering messages(request): %s\n", aud_error_message(result, ebuf));
    }
    result = conmon_client_subscribe(cm->client, NULL, NULL, CONMON_CHANNEL_TYPE_METERING, cm->targets[0].name);
    if (result == AUD_SUCCESS) {
        has_success = AUD_TRUE;
        printf("Subscribing to %s\n", cm->targets[0].name);
    }
#endif // DANTE_CM_METERING_SUBSCRIPTIONS
    if(has_success)
        return AUD_SUCCESS;
    return result;
}

static void DanteCMCbDanteDeviceName(conmon_client_t * client)
{
    DanteCMTimestampError();
    printf("Dante device name changed to '%s'\n", conmon_client_get_dante_device_name (client));
}

static void DanteCMCbNetwork(conmon_client_t * client)
{
    //conmon_info_t * info = conmon_client_context (client);
    const conmon_networks_t * networks;

    DanteCMTimestampError();
    puts ("Addresses changed");

    puts ("Networks:");
    networks = conmon_client_get_networks (client);
    DanteCMPrintNetworks(networks, " ");
}

static void DanteCMCbRegMonitoring(conmon_client_t * client, conmon_client_request_id_t request_id, aud_error_t result)
{
    (void) client;
    (void) request_id;
    
    if (result == AUD_SUCCESS)
    {
        DanteCMTimestampEvent();
        printf("Conmon status registration successful\n");
    }
    else
    {
        aud_errbuf_t ebuf;
        
        DanteCMTimestampError();
        fprintf (stderr,
            "%s: failed to subscribe to conmon status channel: %s (%d)\n"
            , DanteCMPName()
            , aud_error_message (result, ebuf), result
        );
    }
}

static void DanteLiteStatusMessageCallback(conmon_client_t * client, conmon_channel_type_t channel_type, 
                           conmon_channel_direction_t channel_direction, 
                           const conmon_message_head_t * head, 
                           const conmon_message_body_t * body)
{
    conmon_instance_id_t id;
    conmon_message_head_get_instance_id(head, &id);
    {
        uint16_t aud_type = conmon_audinate_message_get_type(body);
        const char *device_name = conmon_client_device_name_for_instance_id(client, &id);
        // check the message is a status message for us
        if(0 != strcmp(DanteCMControlledDeviceName, device_name)) {
            return;
        }
        switch(aud_type) {
            case CONMON_AUDINATE_MESSAGE_TYPE_SRATE_STATUS:
                DanteLiteHandleSampleRateStatus(body);
                break;
            case CONMON_AUDINATE_MESSAGE_TYPE_CLOCKING_STATUS:
                DanteLiteHandleClockStatus(body);
                break;
            case CONMON_AUDINATE_MESSAGE_TYPE_TX_CHANNEL_CHANGE:
                DanteLiteHostInterfaceRouterMarkComponentStale(DR_DEVICE_COMPONENT_TXCHANNELS);
                break;
            case CONMON_AUDINATE_MESSAGE_TYPE_RX_CHANNEL_CHANGE:
                DanteLiteHostInterfaceRouterMarkComponentStale(DR_DEVICE_COMPONENT_RXCHANNELS);
                break;
            case CONMON_AUDINATE_MESSAGE_TYPE_TX_LABEL_CHANGE:
                DanteLiteHostInterfaceRouterMarkComponentStale(DR_DEVICE_COMPONENT_TXLABELS);
                break;
            case CONMON_AUDINATE_MESSAGE_TYPE_TX_FLOW_CHANGE:
                DanteLiteHostInterfaceRouterMarkComponentStale(DR_DEVICE_COMPONENT_TXFLOWS);
                break;
            case CONMON_AUDINATE_MESSAGE_TYPE_RX_FLOW_CHANGE:
                DanteLiteHostInterfaceRouterMarkComponentStale(DR_DEVICE_COMPONENT_RXFLOWS);
                break;
            default:
                deviceConMonDump && printf("DanteLiteStatusMessageCallback: Audinate msg (local): %d\n", aud_type);
                break;
        }
    }
    fflush(stdout);
}

void DanteCMCbMonitoring(conmon_client_t * client, 
                          conmon_channel_type_t channel_type, 
                          conmon_channel_direction_t channel_direction, 
                          const conmon_message_head_t * head, 
                          const conmon_message_body_t * body)
{
    char vid[CONMON_VENDOR_ID_LENGTH+1];
    conmon_vendor_id_t *p;

    // get the vendor ID string
    p = (conmon_vendor_id_t *)conmon_message_head_get_vendor_id(head);
    strncpy(vid, (char *)p, CONMON_VENDOR_ID_LENGTH);
    vid[CONMON_VENDOR_ID_LENGTH] = '\0';
    if (conmon_vendor_id_equals(p, &AUDINATE_VENDOR_ID)) {
        // call the standard status message handler for OCA
        DanteLiteStatusMessageCallback(client, channel_type, channel_direction, head, body);
    }
    // Vendor specific messages, do we have a callback?
    if(pDanteCMCbMonitoringVendor)
        pDanteCMCbMonitoringVendor(client, channel_type, channel_direction, head, body);
}

static aud_error_t DanteCMSubscribeToRemoteClients(conmon_info * cm)
{
    aud_error_t result;
    unsigned int i;
    
    if (cm->all)
    {
        result = conmon_client_subscribe_global (cm->client,
                                                    DanteCMCbRegSubAll,
                                                    & (cm->all_req_id),
                                                    CONMON_CHANNEL_TYPE_STATUS);
        if (result != AUD_SUCCESS)
        {
            return result;
        }
    }
    
    for (i = 0; i < cm->n_targets; i++)
    {
        if (cm->targets[i].name == NULL)
        {
            continue;
        }
        // don't try and subscribe to ourself
        if (!strcmp(conmon_client_get_dante_device_name(cm->client), cm->targets[i].name))
        {
            continue;
        }
        result = conmon_client_subscribe(cm->client,
                                            DanteCMCbRegSub,
                                            & (cm->targets [i].req_id),
                                            CONMON_CHANNEL_TYPE_STATUS,
                                            cm->targets [i].name);
        if (result != AUD_SUCCESS)
        {
            return result;
        }
    }
    
    return AUD_SUCCESS;
}

static void DanteCMCbRegSub(conmon_client_t * client, conmon_client_request_id_t request_id, aud_error_t result)
{
    conmon_target_t * target;
    
    conmon_info_t * info = (conmon_info_t *)conmon_client_context(client);

    aud_errbuf_t ebuf;

    if (request_id != CONMON_CLIENT_NULL_REQ_ID)
    {
        const unsigned int n = info->n_targets;
        const char * p_name;
        unsigned int i;
        
        // find the remote target we were trying to subscribe
        for (i = 0; i < n; i++)
        {
            if (request_id == info->targets [i].req_id)
            {
                target = info->targets + i;
                goto l__found_target;
            }
        }
        
        DanteCMTimestampError();
        fprintf (stderr,
            "%s: invalid subscription request ID: 0x%p\n"
            , (p_name = DanteCMPName()), request_id
        );
        if (result != AUD_SUCCESS)
        {
            fprintf (stderr,
                "%s: error %s (%d)\n"
                , p_name
                , aud_error_message (result, ebuf), result
            );
        }
    }
    else
    {
        fprintf (stderr,
            "%s: unexpected subscription response\n"
            , DanteCMPName()
        );
    }
    
    return;
    
l__found_target:
    if (result == AUD_SUCCESS)
    {
        target->sub =
            conmon_client_get_subscription (
                info->client,
                CONMON_CHANNEL_TYPE_STATUS,
                target->name
            );
        if (! target->sub)
        {
            DanteCMTimestampError();
            fprintf (stderr,
                "Conmon subscription registration to %s - server error\n"
                , target->name
            );
        }
    }
    else
    {
        DanteCMTimestampError();
        fprintf (stderr,
            "%s: failed to subscribe to remote conmon device %s: %s (%d)\n"
            , DanteCMPName()
            , target->name
            , aud_error_message (result, ebuf), result
        );
    }
    
    target->req_id = CONMON_CLIENT_NULL_REQ_ID;
}

static void DanteCMCbRegSubAll (conmon_client_t * client, conmon_client_request_id_t request_id, aud_error_t result)
{
    conmon_info_t * info = (conmon_info_t *)conmon_client_context (client);

    aud_errbuf_t ebuf;
        
    if (request_id != CONMON_CLIENT_NULL_REQ_ID)
    {
        // find the remote target we were trying to subscribe
        if (request_id == info->all_req_id)
        {
            if (result != AUD_SUCCESS)
            {
                DanteCMTimestampError();
                fprintf (stderr,
                    "%s: failed to subscribe to all devices: %s (%d)\n"
                    , DanteCMPName()
                    , aud_error_message (result, ebuf), result
                );
            }

            info->all_req_id = CONMON_CLIENT_NULL_REQ_ID;
            return;
        }
    }

    fprintf (stderr,
        "%s: unexpected subscription response\n"
        , DanteCMPName()
    );

    return;
}

int DanteCMInitialize(conmon_info * cm, char *target, void *vendorStatusCallback, void *vendorMeteringCallback)
{
    aud_error_t result;
    aud_utime_t connectTimeout, currentTime;
    aud_utime_t connectWait = {20,0};
    int i, retries = 0;

    DanteCMctrlConnectRetries = 0;
    DanteCMctrlConnected = 0;
    pDanteCMCtrlInfo = cm;

    if(vendorStatusCallback)
        pDanteCMCbMonitoringVendor = (conmon_client_handle_monitoring_message_fn *)vendorStatusCallback;
    else
        pDanteCMCbMonitoringVendor = NULL;

    if(vendorMeteringCallback)
        pDanteCMCbMeteringVendor = (conmon_client_handle_monitoring_message_fn *)vendorMeteringCallback;
    else
        pDanteCMCbMeteringVendor = NULL;

    // put the target address into the structure
    if(!strlen(target)) {
        target = (char *)myself;
    }
    sprintf(DanteCMControlledDeviceName, "%s", target);
    cm->targets [0].name = target;
    cm->targets [0].req_id = CONMON_CLIENT_NULL_REQ_ID;
    cm->targets [0].found = NO;
    cm->n_targets = 1;
    cm->n_filters = 0;

    result = DanteCMSetupConmon(cm);


    if(result == AUD_SUCCESS) {
        // we can wait until the connection has actually happened, or timed out
        aud_utime_get(&currentTime);
        aud_utime_get(&connectTimeout);
        // add timeout
        aud_utime_add(&connectTimeout, &connectWait);
        cm->running = AUD_TRUE;
        // run the message pump until we time out
        while(aud_utime_compare(&currentTime, &connectTimeout) < 0) {
            DanteCMProcess(cm);
            if(DanteCMctrlConnected)
                break;
            aud_utime_get(&currentTime);
        }
        cm->running = AUD_FALSE;
        if(DanteCMctrlConnected) {
            // run the message pump for a few times to get everyting initialized
            cm->running = AUD_TRUE;
            for(i = 0; i < 10; i++) {
                DanteCMProcess(cm);
            }
            cm->running = AUD_TRUE;
retry:
            // send out requests
            DanteCMGetClockInformation();
            DanteCMProcess(cm);
            // now wait to be completely configured
            // add timeout
            connectWait.tv_usec = 500000;
            connectWait.tv_sec  = 0;
            aud_utime_get(&currentTime);
            aud_utime_get(&connectTimeout);
            aud_utime_add(&connectTimeout, &connectWait);
            // run the message pump until we time out
            while(aud_utime_compare(&currentTime, &connectTimeout) < 0) {
                DanteCMProcess(cm);
                if(DanteCMctrlInitialized)
                    break;
                aud_utime_get(&currentTime);
            }
            if(DanteCMctrlInitialized) {
                dante_sockets_clear(&DanteCMClientSockets);
                conmon_client_get_sockets(cm->client, &DanteCMClientSockets);
                return AUD_SUCCESS;
            } else {
                retries++;
                if(retries < 40)
                    goto retry;
            }
        }
        result = AUD_ERR_TIMEDOUT;
    }

    return result;
}

int DanteCMProcessWithFdSet(conmon_info * cm, fd_set* readSet)
{
    aud_errbuf_t ebuf;
    aud_error_t result = AUD_SUCCESS;

    if(cm->running) {
        aud_socket_t fd = conmon_client_get_socket (cm->client);
        if (FD_ISSET(fd, readSet))
        {
            result = conmon_client_process (cm->client);
            if (result != AUD_SUCCESS)
            {
                DanteCMTimestampError();
                fprintf (stderr,
                    "%s: failed processing fd %d: %s (%d)\n"
                    , DanteCMPName(), fd
                    , aud_error_message (result, ebuf), result
                );
            }
        }
        // process meters etc
        conmon_client_process_sockets(cm->client, &DanteCMClientSockets, NULL);
    }

    return result;
}

int DanteCMProcess(conmon_info * cm)
{
    aud_error_t result = AUD_SUCCESS;
    const aud_utime_t timeout = {0, 1000};        // 1ms

    if(cm->running) {
        int nfds;
        fd_set fdr;
        int count;

        aud_socket_t fd = conmon_client_get_socket (cm->client);
        
        FD_ZERO (& fdr);
        FD_SET (fd, & fdr);

#ifdef WIN32
        nfds = 1;
        count = select (nfds, & fdr, NULL, NULL, &timeout);
#else
        nfds = fd + 1;
        count = select (nfds, & fdr, NULL, NULL, (timeval*)&timeout);
#endif
        if (count > 0)
        {
            result = DanteCMProcessWithFdSet(cm, &fdr);
        }
        else if (count != 0)
        {
            DanteCMTimestampError();
            fprintf (stderr,
                "%s: select failed: %s (%d)\n"
                , DanteCMPName()
                , strerror (errno), errno
            );
        }
    }

    return result;
}

void DanteCMGetClockInformation(void)
{
    conmon_message_body_t aud_msg;
    uint16_t body_size;
    // Sample Rate
    conmon_audinate_init_srate_control(&aud_msg, 100);
    body_size = conmon_audinate_srate_control_get_size(&aud_msg);
    DanteCMSendAudinateMessage(&aud_msg, body_size);
    // Clock State
    conmon_audinate_init_clocking_control(&aud_msg, 100);
    body_size = conmon_audinate_clocking_control_get_size(&aud_msg);
    DanteCMSendAudinateMessage(&aud_msg, body_size);
    return;
}

int DanteCMSendAudinateMessage(conmon_message_body_t *msg, uint32_t size)
{
    const aud_utime_t comms_timeout = {2, 0};
    aud_error_t result(AUD_SUCCESS);

    result = conmon_client_send_control_message(pDanteCMCtrlInfo->client, 
                                                NULL, NULL, pDanteCMCtrlInfo->targets[0].name, 
                                                CONMON_MESSAGE_CLASS_VENDOR_SPECIFIC, &AUDINATE_VENDOR_ID, 
                                                msg, size, &comms_timeout);

    return result;
}

int DanteCMSendVendorMessage(conmon_message_body_t *msg, uint32_t size, const conmon_vendor_id_t * vendor_id)
{
    const aud_utime_t comms_timeout = {2, 0};
    aud_error_t result(AUD_SUCCESS);
    if(DanteCMctrlInitialized) {
        result = conmon_client_send_control_message(pDanteCMCtrlInfo->client, 
                                                    NULL, NULL, pDanteCMCtrlInfo->targets[0].name, 
                                                    CONMON_MESSAGE_CLASS_VENDOR_SPECIFIC, vendor_id, 
                                                    msg, size, &comms_timeout);
    }
    return result;
}

// Force a couple of runs through the Dante sockets
void DanteCMForceUpdate(void)
{
    aud_utime_t connectWait, currentTime;
    int loops = 2;
    while(loops) {
        DanteLiteHostInterfaceRun();
        aud_utime_get(&currentTime);
        connectWait.tv_sec = 0; connectWait.tv_usec = 100000;
        // add timeout
        aud_utime_add(&connectWait, &currentTime);
        while(aud_utime_compare(&currentTime, &connectWait) < 0) {
            aud_utime_get(&currentTime);
        }
        --loops;
    }
}
