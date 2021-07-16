/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"

#include <porting.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/atomic.h>
#include <sys/byteorder.h>
#include <sys/util.h>
#include <sys/slist.h>
#include <sys/work_queue.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hfp_ag.h>
#include <bluetooth/sdp.h>
#include "BT_common.h"
#include "BT_hci_api.h"
#include "BT_sm_api.h"
#include "BT_sdp_api.h"

#include "BT_config.h"
#include "app_handsfree_ag.h"
#include "app_connect.h"
#include "app_shell.h"

struct bt_conn *default_conn;
static uint8_t s_hfp_in_calling_status = 0xff;
typedef struct app_hfp_ag_
{
    struct bt_hfp_ag *hfp_agHandle;
    struct bt_conn *conn;
    uint8_t peerKeyMissed;
    uint8_t appl_acl_initiated;
    uint8_t peer_bd_addr[6];
} app_hfp_ag_t;
static app_hfp_ag_t g_HfpAg;
static TimerHandle_t s_xTimers = 0;
static struct bt_work s_ataRespWork;

static void ag_connected(struct bt_hfp_ag *hfp_ag)
{
    printf("HFP AG Connected!\n");
    s_hfp_in_calling_status = 1;
}
static void ag_disconnected(struct bt_hfp_ag *hfp_ag)
{
    printf("HFP AG Disconnected!\n");
    bt_hfp_ag_disconnect(hfp_ag); 
}

hfp_ag_get_config hfp_ag_config = {
    .bt_hfp_ag_vgs             = 15,
    .bt_hfp_ag_vgm             = 15,
    .bt_hfp_ag_codec           = 1,
    .bt_hfp_ag_nrec            = 1,
    .bt_hfp_ag_inband          = 0,
    .bt_hfp_ag_codec_negotiate = 0,
    .bt_hfp_ag_dial            = 0,
};
static void bt_work_ata_response(struct bt_work *work)
{
    printf("HFP HP have accepted the call\n");
    s_hfp_in_calling_status = 3;
    bt_hfp_ag_send_callsetup_indicator(g_HfpAg.hfp_agHandle, 0);
    bt_hfp_ag_send_call_indicator(g_HfpAg.hfp_agHandle, 1);
    if (s_xTimers != 0)
    {
        xTimerStop(s_xTimers, 0);
        xTimerDelete(s_xTimers, 0);
        s_xTimers = 0;
    }
    bt_hfp_ag_call_status_pl(g_HfpAg.hfp_agHandle, hfp_ag_call_call_incoming);
}

void ata_response(struct bt_hfp_ag *hfp_ag)
{
    bt_work_submit(&s_ataRespWork);
}

void chup_response(struct bt_hfp_ag *hfp_ag)
{
    printf("HFP HP have ended the call\n");
    s_hfp_in_calling_status = 1;
    bt_hfp_ag_call_status_pl(g_HfpAg.hfp_agHandle, hfp_ag_call_call_end);
    bt_hfp_ag_send_call_indicator(g_HfpAg.hfp_agHandle, 0);
    if (s_xTimers != 0)
    {
        xTimerStop(s_xTimers, 0);
        xTimerDelete(s_xTimers, 0);
        s_xTimers = 0;
    }
}

static struct bt_hfp_ag_cb ag_cb = {
    .connected     = ag_connected,
    .disconnected  = ag_disconnected,
    .ata_response  = ata_response,
    .chup_response = chup_response,
};

int app_hfp_ag_discover(struct bt_conn *conn, uint8_t channel)
{
    int status = 0;
    hfp_ag_config.server_channel = channel;
    if (default_conn == conn)
    {
        status = bt_hfp_ag_connect(default_conn, &hfp_ag_config, &ag_cb, &g_HfpAg.hfp_agHandle);
        if (0 != status)
        {
            PRINTF("fail to connect hfp_hf (err: %d)\r\n", status );
        }
    }
    return status;
}
int app_hfp_ag_disconnect()
{
  return bt_hfp_ag_disconnect(g_HfpAg.hfp_agHandle);
}

static void bt_ready(int err)
{
    if (err)
    {
        PRINTF("Bluetooth init failed (err %d)\n", err);
        return;
    }

    PRINTF("Bluetooth initialized\n");

    app_connect_init();
    bt_hfp_ag_init();
    app_shell_init();
    bt_work_init(&s_ataRespWork, bt_work_ata_response);
}

static void vTimerRingCallback(TimerHandle_t xTimer)
{
    bt_hfp_ag_send_callring(g_HfpAg.hfp_agHandle);
}
int app_hfp_ag_start_incoming_call()
{
    if (s_hfp_in_calling_status == 1)
    {
        PRINTF("Simulate a incoming call an incoming calling!!\r\n");
        bt_hfp_ag_send_callsetup_indicator(g_HfpAg.hfp_agHandle, 1);
        s_xTimers = xTimerCreate("RingTimer", (2000) + 10, pdTRUE, 0, vTimerRingCallback);
        xTimerStart(s_xTimers, 0);
        bt_hfp_ag_send_callring(g_HfpAg.hfp_agHandle);
        s_hfp_in_calling_status = 2;
        return 0;
    }
    return -1;
}

int app_hfp_ag_accept_incoming_call()
{
    if (s_hfp_in_calling_status == 2)
    {
        printf("HFP AG have accepted the incoming call\n");
        s_hfp_in_calling_status = 3;
        bt_hfp_ag_send_callsetup_indicator(g_HfpAg.hfp_agHandle, 0);
        bt_hfp_ag_send_call_indicator(g_HfpAg.hfp_agHandle, 1);
        if (s_xTimers != 0)
        {
            xTimerStop(s_xTimers, 0);
            xTimerDelete(s_xTimers, 0);
            s_xTimers = 0;
        }
        bt_hfp_ag_call_status_pl(g_HfpAg.hfp_agHandle, hfp_ag_call_call_incoming);
        return 0;
    }
    return -1;
}
int app_hfp_ag_stop_incoming_call()
{
    if (s_hfp_in_calling_status >= 2)
    {
        bt_hfp_ag_call_status_pl(g_HfpAg.hfp_agHandle, hfp_ag_call_call_end);
        if (s_xTimers != 0)
        {
            xTimerStop(s_xTimers, 0);
            xTimerDelete(s_xTimers, 0);
            s_xTimers = 0;
        }
        bt_hfp_ag_send_call_indicator(g_HfpAg.hfp_agHandle, 0);
        printf("HFP AG have ended the call\n");
        s_hfp_in_calling_status = 1;
        return 0;
    }
    return -1;
}
void peripheral_hfp_ag_task(void *pvParameters)
{
    int err = 0;

    /* Initializate BT Host stack */
    err = bt_enable(bt_ready);
    if (err)
    {
        PRINTF("Bluetooth init failed (err %d)\n", err);
        return;
    }
    vTaskDelete(NULL);
}
