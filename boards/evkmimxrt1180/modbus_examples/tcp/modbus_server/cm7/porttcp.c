/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "stdint.h"
#include "app.h"

#include "lwip/api.h"
#include "lwip/tcp.h"

#include "mb.h"
#include "mbport.h"

/* ----------------------- Static variables ---------------------------------*/
typedef struct {
    struct tcp_pcb *server_pcb;
    struct tcp_pcb *active_client;
    uint8_t buffer[MAX_BUFFER_SIZE];
    uint16_t buffer_index;
} network_context_t;

static network_context_t net_ctx = {0};

/* ----------------------- Static functions ---------------------------------*/
static err_t prvxMBTCPPortReceive(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
static void prvvMBTCPPortError(void *arg, err_t err);
static err_t accept_connection(void *arg, struct tcp_pcb *new_pcb, err_t err);

/* ----------------------- Begin implementation -----------------------------*/
int8_t xMBTCPPortInit(uint16_t port) {
    struct tcp_pcb *temp_pcb;
    uint16_t listen_port = port ? port : TCP_PORT_DEFAULT;
    
    temp_pcb = tcp_new();
    if (!temp_pcb) return 0;
 
    if (tcp_bind(temp_pcb, IP_ADDR_ANY, listen_port) != ERR_OK) {
        tcp_close(temp_pcb);
        return 0;
    }
 
    net_ctx.server_pcb = tcp_listen(temp_pcb);
    if (!net_ctx.server_pcb) {
        tcp_close(temp_pcb);
        return 0;
    }
 
    tcp_accept(net_ctx.server_pcb, accept_connection);
    return 1;
}

void prvvMBPortReleaseClient(struct tcp_pcb *pcb) {
    if (!pcb) return;
    
    if (tcp_close(pcb) != ERR_OK) {
        tcp_abort(pcb);
    }
    
    if (pcb == net_ctx.active_client) {
        net_ctx.active_client = NULL;
    }
    if (pcb == net_ctx.server_pcb) {
        net_ctx.server_pcb = NULL;
    }
}

void vMBTCPPortDisable( void )
{
    prvvMBPortReleaseClient( net_ctx.active_client );
}

err_t accept_connection(void *arg, struct tcp_pcb *new_pcb, err_t err) {
    if (err != ERR_OK) return err;
    
    if (!net_ctx.active_client) {
        net_ctx.active_client = new_pcb;
        net_ctx.buffer_index = 0;
        
        tcp_recv(new_pcb, prvxMBTCPPortReceive);
        tcp_err(new_pcb, prvvMBTCPPortError);
        tcp_arg(new_pcb, new_pcb);
        
        return ERR_OK;
    }
    
    prvvMBPortReleaseClient(new_pcb);
    return ERR_OK;
}

err_t prvxMBTCPPortReceive(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err) {
    uint16_t msg_length;
    
    if (err != ERR_OK) return err;
    if (!p) {
        prvvMBPortReleaseClient(pcb);
        return ERR_OK;
    }
 
    tcp_recved(pcb, p->len);
 
    if ((net_ctx.buffer_index + p->len) >= MAX_BUFFER_SIZE) {
        prvvMBPortReleaseClient(pcb);
        pbuf_free(p);
        return ERR_OK;
    }
 
    memcpy(&net_ctx.buffer[net_ctx.buffer_index], p->payload, p->len);
    net_ctx.buffer_index += p->len;
 
    if (net_ctx.buffer_index >= HEADER_FUNC_POS) {
        msg_length = (net_ctx.buffer[HEADER_LEN_POS] << 8) | net_ctx.buffer[HEADER_LEN_POS + 1];
        
        if (net_ctx.buffer_index == (HEADER_UID_POS + msg_length)) {
            xMBPortEventPost( EV_FRAME_RECEIVED );
        } else if (net_ctx.buffer_index > (HEADER_UID_POS + msg_length)) {
            prvvMBPortReleaseClient(pcb);
        }
    }
 
    pbuf_free(p);
    return ERR_OK;
}

int8_t xMBTCPPortGetRequest( uint8_t ** ppucMBTCPFrame, uint16_t * usTCPLength )
{
    *ppucMBTCPFrame = &net_ctx.buffer[0];
    *usTCPLength = net_ctx.buffer_index;

    /* Reset the buffer. */
    net_ctx.buffer_index = 0;
    return 1;
}

void prvvMBTCPPortError(void *arg, err_t err) {
    struct tcp_pcb *pcb = arg;
    if (pcb) prvvMBPortReleaseClient(pcb);
}
 
int8_t xMBTCPPortSendResponse(const uint8_t *data, uint16_t length) {
    if (!net_ctx.active_client) return 0;
    
    if (tcp_sndbuf(net_ctx.active_client) < length) return 0;

    /* Lock tcp core */
    sys_lock_tcpip_core();
    
    if (tcp_write(net_ctx.active_client, data, length, NETCONN_COPY) != ERR_OK) {
        prvvMBPortReleaseClient(net_ctx.active_client);
        return 0;
    }
    
    tcp_output(net_ctx.active_client);

    /* Unlock tcp core */
    sys_unlock_tcpip_core();

    return 1;
}

void vMBTCPPortClose(void) {
    prvvMBPortReleaseClient(net_ctx.active_client);
    prvvMBPortReleaseClient(net_ctx.server_pcb);
}

