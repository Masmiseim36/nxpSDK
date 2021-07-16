/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "iperf_api.h"
#include "serial_mwm.h"
#include "fsl_debug_console.h"

static mwm_sockaddr_t s_addr = {0};

static void wifi_reset_prov_mode()
{
    int ret;
    int c;

    PRINTF("\r\nDo you want to reset the Wi-Fi module to provisioning mode?\r\n");
    PRINTF("Press 'y' to reset Wi-Fi module.\r\n");
    c = GETCHAR();
    PRINTF("\r\n");
    if (c == 'y')
    {
        PRINTF("Resetting Wi-Fi module...\r\n\r\n");
        ret = mwm_wlan_prov();
        if (ret < 0)
        {
            PRINTF("Wi-Fi reset failed!\r\n");
            while (1)
                ;
        }
        vTaskDelay(pdMS_TO_TICKS(6000));
    }
}

void iperf_hw_init(struct iperf_ctx *ctx)
{
    int ret;

    ret = mwm_init();
    if (ret < 0)
    {
        PRINTF("Could not initialize Serial MWM\r\n");
        while (1)
            ;
    }

    assert(NULL != ctx);

    ctx->ctrl_buf = pvPortMalloc(IPERF_BUFFER_MAX);
    assert(NULL != ctx->ctrl_buf);
    memset(ctx->ctrl_buf, 0, IPERF_BUFFER_MAX);

    ctx->send_buf = pvPortMalloc(IPERF_BUFFER_MAX);
    assert(NULL != ctx->send_buf);
    memset(ctx->send_buf, 0, IPERF_BUFFER_MAX);

    ctx->recv_buf = pvPortMalloc(IPERF_BUFFER_MAX);
    assert(NULL != ctx->recv_buf);
    memset(ctx->recv_buf, 0, IPERF_BUFFER_MAX);

    memset(&s_addr, 0, sizeof(mwm_sockaddr_t));

    ret = sprintf(s_addr.host, "%hu.%hu.%hu.%hu", ctx->server_ip.ip[0], ctx->server_ip.ip[1], ctx->server_ip.ip[2],
                  ctx->server_ip.ip[3]);
    assert(ret > 0);
    s_addr.port = SENDER_PORT_NUM;

    ctx->addr     = &s_addr;
    ctx->addr_len = sizeof(s_addr);

    /* get wlan status */
    ret = mwm_wlan_status();
    if (ret < 0)
    {
        PRINTF("Failed to get WLAN status.\r\n");
        while (1)
            ;
    }

    if (ret == MWM_INITIALIZED)
    {
        /* start wlan */
        ret = mwm_wlan_start();
        if (ret < 0)
        {
            PRINTF("Could not start WLAN subsystem.\r\n");
            while (1)
                ;
        }
    }

    while (1)
    {
        /* get wlan status */
        ret = mwm_wlan_status();
        if (ret < 0)
        {
            PRINTF("Failed to get WLAN status.\r\n");
            while (1)
                ;
        }
        else if (ret == MWM_CONNECTED)
        {
            char ssid[33]    = {0};
            char ip_addr[16] = {0};
            ret              = mwm_wlan_info(ssid, ip_addr);
            PRINTF("Wi-Fi is connected to: %s, IP Address: %s\r\n", ssid, ip_addr);
            PRINTF(
                "\r\nTo reset Wi-Fi module to provisioning mode connect to WLAN: %s, open address: http://%s in web "
                "browser and select \"Reset to Provisioning\".\r\n",
                ssid, ip_addr);
            PRINTF("Then restart this application.\r\n\r\n");
            break;
        }
        else if (ret == MWM_CONNECTING)
        {
            PRINTF("Wi-Fi is connecting...\r\n");
            vTaskDelay(pdMS_TO_TICKS(4000));
        }
        else if (ret == MWM_AUTH_FAILED)
        {
            PRINTF("Connection failed: Wi-Fi authentication failed.\r\n");
            wifi_reset_prov_mode();
        }
        else if (ret == MWM_NETWORK_NOT_FOUND)
        {
            PRINTF("Connection failed: WLAN not found.\r\n");
            wifi_reset_prov_mode();
        }
        else
        {
            PRINTF("\r\nWi-Fi module is not configured for WLAN connection.\r\n");
            PRINTF(
                "Connect to Wi-Fi AP: Serial2Wifi, open address: http://192.168.10.1 in web browser and configure WLAN "
                "connection.\r\n");
            PRINTF("Press any key to continue.\r\n");
            GETCHAR();
            PRINTF("\r\n");
        }
    }
}

int iperf_send(int socket, void *buffer, size_t len, int flag)
{
    int sent_bytes;
    sent_bytes = mwm_send(socket, buffer, len);
    return sent_bytes;
}

int iperf_send_to(int socket, char *buffer, int len, int flags, void *to, int tolen)
{
    (void)flags;
    int sent_bytes;
    sent_bytes = mwm_sendto(socket, buffer, len, (mwm_sockaddr_t *)to, tolen);
    return sent_bytes;
}

int iperf_connect(int socket, void *addr, uint32_t addrlen)
{
    int conn;
    conn = mwm_connect(socket, (mwm_sockaddr_t *)addr, addrlen);
    return conn;
}

int iperf_socket(int protocol)
{
    int type = MWM_TCP;
    int socket;

    if (protocol == UDP)
    {
        type = MWM_UDP;
    }

    socket = mwm_socket(type);
    return socket;
}

int iperf_recv_from_blocked(int socket, void *buf, size_t len, int flags)
{
    int recv_bytes;
    recv_bytes = mwm_recvfrom_timeout(socket, buf, len, 0);
    return recv_bytes;
}

int iperf_recv_from_timeout(int socket, void *buf, size_t len, int flags, int timeout_ms)
{
    int recv_bytes;
    recv_bytes = mwm_recv_timeout(socket, buf, len, timeout_ms);
    return recv_bytes;
}

int iperf_recv_noblock(int socket, void *buf, size_t len, int flags)
{
    int recv_bytes;
    recv_bytes = mwm_recv_timeout(socket, buf, len, 1);
    return recv_bytes;
}

int iperf_recv_blocked(int socket, void *buf, size_t len, int flags)
{
    int recv_bytes;
    recv_bytes = mwm_recv_timeout(socket, buf, len, 0);
    return recv_bytes;
}

int iperf_recv_timeout(int socket, void *buf, size_t len, int flags, int timeout_ms)
{
    int recv_bytes;
    recv_bytes = mwm_recv_timeout(socket, buf, len, timeout_ms);
    return recv_bytes;
}

int iperf_socket_close(int socket)
{
    int ret;
    ret = mwm_close(socket);
    return ret;
}
