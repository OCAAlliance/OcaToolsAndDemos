/*
 * tinysvcmdns - a tiny MDNS implementation for publishing services
 * Copyright (C) 2011 Darell Tan
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "FreeRTOS.h"
#include "task.h"
#include "lwip/mem.h"
#include "lwip/sockets.h"
#include "semphr.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

/*
 * Define a proper IP socket level if not already done.
 * Required to compile on OS X
 */
#ifndef SOL_IP
#define SOL_IP IPPROTO_IP
#endif

#include "mdns.h"
#include "mdnsd.h"

#define MDNS_ADDR "224.0.0.251"
#define MDNS_PORT 5353

//#define PACKET_SIZE 65536
#define PACKET_SIZE 1024

#define SERVICES_DNS_SD_NLABEL \
		((uint8_t *) "\x09_services\x07_dns-sd\x04_udp\x05local")

#define LOG_ERR         (0)
#define SOCKET          int
#define INVALID_SOCKET  (-1)
#define SOCKET_ERROR    (-1)

#define DEBUGGING   0

#if DEBUGGING
#define DEBUG_PRINTF(fmt, args...) printf(fmt, ## args)
#else
#define DEBUG_PRINTF(fmt, args...)
#endif

const signed char mdnsd_taskName[] = "mdnsd_task";

xSemaphoreHandle mDNS_started;

struct mdnsd {
    xSemaphoreHandle data_lock;
	int sockfd;
#ifdef USE_NOTIFY
	int notify_pipe[2];
#endif
	int stop_flag;

	struct rr_group *group;
	struct rr_list *announce;
	struct rr_list *services;
	uint8_t *hostname;
};

struct mdns_service {
	struct rr_list *entries;
};


struct mdnsd *server;

/////////////////////////////////


static void log_message(int loglevel, char *fmt_str, ...) {
#if DEBUGGING
	va_list ap;
	char buf[2048];

	va_start(ap, fmt_str);
	vsnprintf(buf, 2047, fmt_str, ap);
	va_end(ap);
	buf[2047] = 0;

        DEBUG_PRINTF("%s\r\n", buf);
#endif
}

static int create_recv_sock() {
	int sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
		log_message(LOG_ERR, "recv socket(): %m");
		return sd;
	}

	int r = -1;

	int on = 0;

	if ((r = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on))) < 0) {
		log_message(LOG_ERR, "recv setsockopt(SO_REUSEADDR): %m");
		return r;
	}

	/* bind to an address */
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(MDNS_PORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);	/* receive multicast */
	if ((r = bind(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0) {
		log_message(LOG_ERR, "recv bind(): %m");
	}

	// add membership to receiving socket
	struct ip_mreq mreq;
	memset(&mreq, 0, sizeof(struct ip_mreq));
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	mreq.imr_multiaddr.s_addr = inet_addr(MDNS_ADDR);
	if ((r = setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *) &mreq, sizeof(mreq))) < 0) {
		log_message(LOG_ERR, "recv setsockopt(IP_ADD_MEMBERSHIP): %m");
		return r;
	}

#ifdef ENABLE_LOOPBACK
	// enable loopback in case someone else needs the data
	if ((r = setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *) &on, sizeof(on))) < 0) {
		log_message(LOG_ERR, "recv setsockopt(IP_MULTICAST_LOOP): %m");
		return r;
	}
#endif


#ifdef IP_PKTINFO
	if ((r = setsockopt(sd, SOL_IP, IP_PKTINFO, (char *) &on, sizeof(on))) < 0) {
		log_message(LOG_ERR, "recv setsockopt(IP_PKTINFO): %m");
		return r;
	}
#endif

	return sd;
}

static size_t send_packet(int fd, const void *data, size_t len) {
	static struct sockaddr_in toaddr;
	if (toaddr.sin_family != AF_INET) {
		memset(&toaddr, 0, sizeof(struct sockaddr_in));
		toaddr.sin_family = AF_INET;
		toaddr.sin_port = htons(MDNS_PORT);
		toaddr.sin_addr.s_addr = inet_addr(MDNS_ADDR);
	}

	return sendto(fd, data, len, 0, (struct sockaddr *) &toaddr, sizeof(struct sockaddr_in));
}


// populate the specified list which matches the RR name and type
// type can be RR_ANY, which populates all entries EXCEPT RR_NSEC
static int populate_answers(struct mdnsd *svr, struct rr_list **rr_head, uint8_t *name, enum rr_type type) {
	int num_ans = 0;

	// check if we have the records
        xSemaphoreTake(svr->data_lock, portMAX_DELAY);
	struct rr_group *ans_grp = rr_group_find(svr->group, name);
	if (ans_grp == NULL) {
                xSemaphoreGive(svr->data_lock);
		return num_ans;
	}

	// decide which records should go into answers
	struct rr_list *n = ans_grp->rr;
	for (; n; n = n->next) {
		// exclude NSEC for RR_ANY
		if (type == RR_ANY && n->e->type == RR_NSEC)
			continue;

		if ((type == n->e->type || type == RR_ANY) && cmp_nlabel(name, n->e->name) == 0) {
			num_ans += rr_list_append(rr_head, n->e);
		}
	}

        xSemaphoreGive(svr->data_lock);

	return num_ans;
}

// given a list of RRs, look up related records and add them
static void add_related_rr(struct mdnsd *svr, struct rr_list *list, struct mdns_pkt *reply) {
	for (; list; list = list->next) {
		struct rr_entry *ans = list->e;

		switch (ans->type) {
			case RR_PTR:
				// target host A, AAAA records
				reply->num_add_rr += populate_answers(svr, &reply->rr_add, 
										MDNS_RR_GET_PTR_NAME(ans), RR_ANY);
				break;

			case RR_SRV:
				// target host A, AAAA records
				reply->num_add_rr += populate_answers(svr, &reply->rr_add, 
										ans->data.SRV.target, RR_ANY);

				// perhaps TXT records of the same name?
				// if we use RR_ANY, we risk pulling in the same RR_SRV
				reply->num_add_rr += populate_answers(svr, &reply->rr_add, 
										ans->name, RR_TXT);
				break;

			case RR_A:
			case RR_AAAA:
				reply->num_add_rr += populate_answers(svr, &reply->rr_add, 
										ans->name, RR_NSEC);
				break;

			default:
				// nothing to add
				break;
		}
	}
}

// creates an announce packet given the type name PTR 
static void announce_srv(struct mdnsd *svr, struct mdns_pkt *reply, uint8_t *name) {
	mdns_init_reply(reply, 0);

	reply->num_ans_rr += populate_answers(svr, &reply->rr_ans, name, RR_PTR);

	// remember to add the services dns-sd PTR too
	reply->num_ans_rr += populate_answers(svr, &reply->rr_ans, 
								SERVICES_DNS_SD_NLABEL, RR_PTR);

	// see if we can match additional records for answers
	add_related_rr(svr, reply->rr_ans, reply);

	// additional records for additional records
	add_related_rr(svr, reply->rr_add, reply);
}

// processes the incoming MDNS packet
// returns >0 if processed, 0 otherwise
static int process_mdns_pkt(struct mdnsd *svr, struct mdns_pkt *pkt, struct mdns_pkt *reply) {
	int i;

	// is it standard query?
	if ((pkt->flags & MDNS_FLAG_RESP) == 0 && 
			MDNS_FLAG_GET_OPCODE(pkt->flags) == 0) {
		mdns_init_reply(reply, pkt->id);

#if DEBUGGING
		DEBUG_PRINTF("flags = %04x, qn = %d, ans = %d, add = %d\n", 
						pkt->flags,
						pkt->num_qn,
						pkt->num_ans_rr,
						pkt->num_add_rr);
#endif

		// loop through questions
		struct rr_list *qnl = pkt->rr_qn;
		for (i = 0; i < pkt->num_qn; i++, qnl = qnl->next) {
			struct rr_entry *qn = qnl->e;
			int num_ans_added = 0;

#if DEBUGGING
			char *namestr = nlabel_to_str(qn->name);
			DEBUG_PRINTF("qn #%d: type %s (%02x) %s - ", i, rr_get_type_name(qn->type), qn->type, namestr);
			mem_free(namestr);
#endif
			// check if it's a unicast query - we ignore those
			if (qn->unicast_query) {
				DEBUG_PRINTF("skipping unicast query\n");
				continue;
			}

			num_ans_added = populate_answers(svr, &reply->rr_ans, qn->name, qn->type);
			reply->num_ans_rr += num_ans_added;

			DEBUG_PRINTF("added %d answers\n", num_ans_added);
			vTaskDelay(1);
		}

		// remove our replies if they were already in their answers
		struct rr_list *ans = NULL, *prev_ans = NULL;
		for (ans = reply->rr_ans; ans; ) {
			struct rr_list *next_ans = ans->next;
			struct rr_entry *known_ans = rr_entry_match(pkt->rr_ans, ans->e);

			// discard answers that have at least half of the actual TTL
			if (known_ans != NULL && known_ans->ttl >= ans->e->ttl / 2) {
#if DEBUGGING
				char *namestr = nlabel_to_str(ans->e->name);
				DEBUG_PRINTF("removing answer for %s\n", namestr);
				mem_free(namestr);
#endif
				// check if list item is head
				if (prev_ans == NULL)
					reply->rr_ans = ans->next;
				else
					prev_ans->next = ans->next;
				mem_free(ans);

				ans = prev_ans;

				// adjust answer count
				reply->num_ans_rr--;
			}

			prev_ans = ans;
			ans = next_ans;
			vTaskDelay(1);
		}


		// see if we can match additional records for answers
		add_related_rr(svr, reply->rr_ans, reply);

		// additional records for additional records
		add_related_rr(svr, reply->rr_add, reply);

		DEBUG_PRINTF("\n");

		return reply->num_ans_rr;
	}

	return 0;
}

#ifdef USE_NOTIFY
int create_pipe(int handles[2]) {
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
        log_message(LOG_ERR, "socket failed");
		return -1;
	}
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(0);
	serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if (bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
		closesocket(sock);
        log_message(LOG_ERR, "bind failed");
		return -1;
	}
	if (listen(sock, 1) == SOCKET_ERROR) {
        log_message(LOG_ERR, "listen failed");
		closesocket(sock);
		return -1;
	}
	int len = sizeof(serv_addr);
	if (getsockname(sock, (struct sockaddr*)&serv_addr, (socklen_t*)&len) == SOCKET_ERROR) {
        log_message(LOG_ERR, "getsockname failed");
		closesocket(sock);
		return -1;
	}
	if ((handles[1] = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        log_message(LOG_ERR, "socket2 failed");
		closesocket(sock);
		return -1;
	}

	if (connect(handles[1], (struct sockaddr*)&serv_addr, len) == SOCKET_ERROR) {
        log_message(LOG_ERR, "connect failed");
		closesocket(sock);
		return -1;
	}
	if ((handles[0] = accept(sock, (struct sockaddr*)&serv_addr, (socklen_t*)&len)) == INVALID_SOCKET) {
		closesocket((SOCKET)handles[1]);
        log_message(LOG_ERR, "accept failed");
		handles[1] = INVALID_SOCKET;
		closesocket(sock);
		return -1;
	}
	closesocket(sock);
	return 0;
}

int read_pipe(int s, char* buf, int len) {
	return read(s, buf, len);
}

int write_pipe(int s, char* buf, int len) {
	return write(s, buf, len);
}

int close_pipe(int s) {
	return close(s);
}
#endif

//MTW
// main loop to receive, process and send out MDNS replies
// also handles MDNS service announces
//static void main_loop(struct mdnsd *svr) {
void mdnsd_task(void * pvParameters) {
        server = mem_malloc(sizeof(struct mdnsd));
	memset(server, 0, sizeof(struct mdnsd));

#ifdef USE_NOTIFY
	if (create_pipe(server->notify_pipe) != 0) {
		log_message(LOG_ERR, "Failed to create pipe: MSDND task not running\r\n");
		mem_free(server);
                goto pipe_err;
	}
#endif

	server->sockfd = create_recv_sock();
	if (server->sockfd < 0) {
		log_message(LOG_ERR, "unable to create recv socket:  MSDND task not running\r\n");
		mem_free(server);
                goto sockfd_err;
	}

    server->data_lock = xSemaphoreCreateMutex();


	fd_set sockfd_set;
	int max_fd = server->sockfd;
#ifdef USE_NOTIFY
	char notify_buf[2];	// buffer for reading of notify_pipe
#endif

	void *pkt_buffer = mem_malloc(PACKET_SIZE);

#ifdef USE_NOTIFY
	if (server->notify_pipe[0] > max_fd)
		max_fd = server->notify_pipe[0];
#endif

	struct mdns_pkt *mdns_reply = mem_malloc(sizeof(struct mdns_pkt));
	memset(mdns_reply, 0, sizeof(struct mdns_pkt));

    //Giveup the mDNS semaphore
    xSemaphoreGive(mDNS_started);

	while (! server->stop_flag) {
		FD_ZERO(&sockfd_set);
		FD_SET(server->sockfd, &sockfd_set);
#ifdef USE_NOTIFY
		FD_SET(server->notify_pipe[0], &sockfd_set);
#endif
#ifdef USE_NOTIFY
		select(max_fd + 1, &sockfd_set, NULL, NULL, NULL);
#else
        struct timeval timeout = { 1, 0 };
        select(max_fd + 1, &sockfd_set, NULL, NULL, &timeout);
#endif

#ifdef USE_NOTIFY
		if (FD_ISSET(server->notify_pipe[0], &sockfd_set)) 
		{
			// flush the notify_pipe
			read_pipe(server->notify_pipe[0], (char*)&notify_buf, 1);
		} 
        else 
#endif
            if (FD_ISSET(server->sockfd, &sockfd_set)) 
	        {
			struct sockaddr_in fromaddr;
			socklen_t sockaddr_size = sizeof(struct sockaddr_in);

			size_t recvsize = recvfrom(server->sockfd, pkt_buffer, PACKET_SIZE, 0, 
				(struct sockaddr *) &fromaddr, &sockaddr_size);
			if (recvsize < 0) {
				log_message(LOG_ERR, "recv(): %m");
			}

			DEBUG_PRINTF("data from=%s size=%ld\n", inet_ntoa(fromaddr.sin_addr), (long) recvsize);
			struct mdns_pkt *mdns = mdns_parse_pkt(pkt_buffer, recvsize);
			if (mdns != NULL) {
				if (process_mdns_pkt(server, mdns, mdns_reply)) {
					size_t replylen = mdns_encode_pkt(mdns_reply, pkt_buffer, PACKET_SIZE);
					send_packet(server->sockfd, pkt_buffer, replylen);
				} else if (mdns->num_qn == 0) {
					DEBUG_PRINTF("(no questions in packet)\n\n");
				}

				mdns_pkt_destroy(mdns);
			}
		}

		// send out announces
		while (1) {
			struct rr_entry *ann_e = NULL;

			// extract from head of list
                        xSemaphoreTake(server->data_lock, portMAX_DELAY);
			if (server->announce) 
				ann_e = rr_list_remove(&server->announce, server->announce->e);
                        xSemaphoreGive(server->data_lock);

			if (! ann_e)
				break;

#if DEBUGGING
			char *namestr = nlabel_to_str(ann_e->name);
			DEBUG_PRINTF("sending announce for %s\n", namestr);
			mem_free(namestr);
#endif
			announce_srv(server, mdns_reply, ann_e->name);

			if (mdns_reply->num_ans_rr > 0) {
				size_t replylen = mdns_encode_pkt(mdns_reply, pkt_buffer, PACKET_SIZE);
				send_packet(server->sockfd, pkt_buffer, replylen);
			}
			vTaskDelay(1);
		}
	}

	// main thread terminating. send out "goodbye packets" for services
	mdns_init_reply(mdns_reply, 0);

        xSemaphoreTake(server->data_lock, portMAX_DELAY);
	struct rr_list *svc_le = server->services;
	for (; svc_le; svc_le = svc_le->next) {
		// set TTL to zero
		svc_le->e->ttl = 0;
		mdns_reply->num_ans_rr += rr_list_append(&mdns_reply->rr_ans, svc_le->e);
	}
        xSemaphoreGive(server->data_lock);

	// send out packet
	if (mdns_reply->num_ans_rr > 0) {
		size_t replylen = mdns_encode_pkt(mdns_reply, pkt_buffer, PACKET_SIZE);
		send_packet(server->sockfd, pkt_buffer, replylen);
	}

	// destroy packet
	mdns_init_reply(mdns_reply, 0);
	mem_free(mdns_reply);

	mem_free(pkt_buffer);

sockfd_err:
#ifdef USE_NOTIFY
	close_pipe(server->sockfd);
#endif

	server->stop_flag = 2;

#ifdef USE_NOTIFY
pipe_err:
#endif
    vTaskDelete( NULL );
}

/////////////////////////////////////////////////////


void mdnsd_set_hostname(struct mdnsd *svr, const char *hostname, uint32_t ip) {
	struct rr_entry *a_e = NULL,
					*nsec_e = NULL;

	// currently can't be called twice
	// dont ask me what happens if the IP changes

	a_e = rr_create_a(create_nlabel(hostname), ip);

	nsec_e = rr_create(create_nlabel(hostname), RR_NSEC);
	rr_set_nsec(nsec_e, RR_A);

        xSemaphoreTake(svr->data_lock, portMAX_DELAY);
	svr->hostname = create_nlabel(hostname);
	rr_group_add(&svr->group, a_e);
	rr_group_add(&svr->group, nsec_e);

        xSemaphoreGive(svr->data_lock);
}

void mdnsd_add_rr(struct mdnsd *svr, struct rr_entry *rr) {
        xSemaphoreTake(svr->data_lock, portMAX_DELAY);
	rr_group_add(&svr->group, rr);
        xSemaphoreGive(svr->data_lock);
}

struct mdns_service *mdnsd_register_svc(struct mdnsd *svr, const char *instance_name, 
		const char *type, uint16_t port, const char *hostname, const char *txt[]) {
	struct rr_entry *txt_e = NULL, 
					*srv_e = NULL, 
					*ptr_e = NULL,
					*bptr_e = NULL;
	uint8_t *target;
	uint8_t *inst_nlabel, *type_nlabel, *nlabel;
	struct mdns_service *service = mem_malloc(sizeof(struct mdns_service));
	memset(service, 0, sizeof(struct mdns_service));

	// combine service name
	type_nlabel = create_nlabel(type);
	inst_nlabel = create_nlabel(instance_name);
	nlabel = join_nlabel(inst_nlabel, type_nlabel);

	// create TXT record
	if (txt && *txt) {
		txt_e = rr_create(dup_nlabel(nlabel), RR_TXT);
		rr_list_append(&service->entries, txt_e);

		// add TXTs
		for (; *txt; txt++) 
			rr_add_txt(txt_e, *txt);
	}

	// create SRV record
	target = hostname ? 
				create_nlabel(hostname) : 
				dup_nlabel(svr->hostname);

	srv_e = rr_create_srv(dup_nlabel(nlabel), port, target);
	rr_list_append(&service->entries, srv_e);

	// create PTR record for type
	ptr_e = rr_create_ptr(type_nlabel, srv_e);

	// create services PTR record for type
	// this enables the type to show up as a "service"
	bptr_e = rr_create_ptr(dup_nlabel(SERVICES_DNS_SD_NLABEL), ptr_e);

	// modify lists here
        xSemaphoreTake(svr->data_lock, portMAX_DELAY);

	if (txt_e)
		rr_group_add(&svr->group, txt_e);
	rr_group_add(&svr->group, srv_e);
	rr_group_add(&svr->group, ptr_e);
	rr_group_add(&svr->group, bptr_e);

	// append PTR entry to announce list
	rr_list_append(&svr->announce, ptr_e);
	rr_list_append(&svr->services, ptr_e);

        xSemaphoreGive(svr->data_lock);

	// don't free type_nlabel - it's with the PTR record
	mem_free(nlabel);
	mem_free(inst_nlabel);
#ifdef USE_NOTIFY
	// notify server
	write_pipe(svr->notify_pipe[1], ".", 1);
#endif

	return service;
}

void mdns_service_destroy(struct mdns_service *srv) {
	rr_list_destroy(srv->entries, 0);
	mem_free(srv);
}

void mdnsd_start() {
    //Create and take the mDNS semaphore.
    mDNS_started = xSemaphoreCreateMutex();
    xSemaphoreTake(mDNS_started, portMAX_DELAY);

    //Create the task
    xTaskCreate(mdnsd_task,mdnsd_taskName,
            (configMINIMAL_STACK_SIZE * 10), NULL, (tskIDLE_PRIORITY + 3), NULL);

}
