/*
 * Copyright 2021, 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"

#include <porting.h>
#include <string.h>
#include <errno/errno.h>
#include <stdbool.h>
#include <sys/atomic.h>
#include <sys/byteorder.h>
#include <sys/util.h>
#include <sys/slist.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hfp_ag.h>
#include <bluetooth/sdp.h>
#include <bluetooth/rfcomm.h>
#include "BT_common.h"
#include "BT_hci_api.h"
#include "BT_sm_api.h"
#include "BT_sdp_api.h"

#include "BT_config.h"
#include "app_handsfree_ag.h"
#include "app_connect.h"
#include "app_shell.h"

/* User may need to change it for real production */
#define APP_CLASS_OF_DEVICE (0x200000U)
app_hfp_ag_t g_HfpAgs[CONFIG_BT_HFP_AG_MAX_CONN];
static app_hfp_ag_t *g_HfpAg;
static struct bt_sdp_attribute hfp_ag_attrs[] = {
    BT_SDP_NEW_SERVICE,
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_HANDSFREE_AGW_SVCLASS) //11 1F
        },
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_GENERIC_AUDIO_SVCLASS) //12 03
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROTO_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 12),//35 10
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3),// 35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) // 01 00
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 5),// 35 05
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_RFCOMM) // 00 19
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT8), //08
                BT_SDP_ARRAY_16(BT_RFCOMM_CHAN_HFP_AG) //channel number
            },
            )
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROFILE_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8), //35 08
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_HANDSFREE_SVCLASS) //11 1E
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0x0108U) //01 08
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("Handsfree_ag"),
    BT_SDP_SUPPORTED_FEATURES(0x2100),
};
static struct bt_sdp_record hfp_ag_rec = BT_SDP_RECORD(hfp_ag_attrs);

/* app_ag_save_instance and app_ag_remove_instance are only called in the same hfp callback one by one,
 * so don't need mutext to protect allocating/freeing g_HfpAgs.
 */
static app_hfp_ag_t *app_ag_save_instance(struct bt_hfp_ag *hfp_ag)
{
    for (uint8_t index = 0; index < APP_MAX_HFP_AG_CONN; index++)
    {
        if (g_HfpAgs[index].hfp_agHandle == hfp_ag)
        {
            return &g_HfpAgs[index];
        }
    }

    for (uint8_t index = 0; index < APP_MAX_HFP_AG_CONN; index++)
    {
        if (g_HfpAgs[index].hfp_agHandle == NULL)
        {
            g_HfpAgs[index].hfp_agHandle = hfp_ag;
            return &g_HfpAgs[index];
        }
    }

    return NULL;
}

static void app_ag_remove_instance(struct bt_hfp_ag *hfp_ag)
{
    for (uint8_t index = 0; index < APP_MAX_HFP_AG_CONN; index++)
    {
        if (g_HfpAgs[index].hfp_agHandle == hfp_ag)
        {
            g_HfpAgs[index].hfp_agHandle = NULL;
            break;
        }
    }
}

static app_hfp_ag_t *app_ag_get_instance(struct bt_hfp_ag *hfp_ag)
{
    for (uint8_t index = 0; index < APP_MAX_HFP_AG_CONN; index++)
    {
        if (g_HfpAgs[index].hfp_agHandle == hfp_ag)
        {
            return &g_HfpAgs[index];
        }
    }

    return NULL;
}

static uint8_t app_ag_instance_index(struct bt_hfp_ag *hfp_ag)
{
    for (uint8_t index = 0; index < APP_MAX_HFP_AG_CONN; index++)
    {
        if (g_HfpAgs[index].hfp_agHandle == hfp_ag)
        {
            return index;
        }
    }

    return 0;
}

static bool app_ag_check_idle_instance(void)
{
    for (uint8_t index = 0; index < APP_MAX_HFP_AG_CONN; index++)
    {
        if (g_HfpAgs[index].hfp_agHandle == NULL)
        {
            return true;
        }
    }

    return false;
}

static void ag_connected(struct bt_hfp_ag *hfp_ag, int err)
{
    app_hfp_ag_t *app_hfp_ag;

    if (err)
    {
        return;
    }

    app_hfp_ag = app_ag_save_instance(hfp_ag);
    if (app_hfp_ag == NULL)
    {
        PRINTF("Too many connections\n");
        return;
    }
    printf("HFP AG (index:%d) Connected:%d!\n", app_ag_instance_index(hfp_ag), err);

    app_hfp_ag->hfp_agHandle = hfp_ag;
    app_hfp_ag->hfp_in_calling_status = 1;
    app_hfp_ag->selectCodec = 1;
}

static void ag_disconnected(struct bt_hfp_ag *hfp_ag)
{
    printf("HFP AG (index:%d) Disconnected!\n", app_ag_instance_index(hfp_ag));
    app_ag_remove_instance(hfp_ag);
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
    .hf_indicators_slc_enable  = HF_INDICATOR_BATTERY_LEVEL/* | HF_INDICATOR_ENHANCED_DRIVER_SAFETY*/,
};

static void bt_work_ata_response(struct k_work *work)
{
    app_hfp_ag_t *app_hfp_ag = CONTAINER_OF(work, app_hfp_ag_t, ataRespWork);

    printf("HFP HP have accepted the call\n");
    app_hfp_ag->hfp_in_calling_status = 3;
    bt_hfp_ag_send_callsetup_indicator(app_hfp_ag->hfp_agHandle, 0);
    bt_hfp_ag_send_call_indicator(app_hfp_ag->hfp_agHandle, 1);
    if (app_hfp_ag->xTimers != 0)
    {
        xTimerStop(app_hfp_ag->xTimers, 0);
        xTimerDelete(app_hfp_ag->xTimers, 0);
        app_hfp_ag->xTimers = 0;
    }
    bt_hfp_ag_call_status_pl(app_hfp_ag->hfp_agHandle, hfp_ag_call_call_incoming);
}

void dial(struct bt_hfp_ag *hfp_ag, char *number)
{
    printf("HFP HP have a in coming call :%s\n", number);
    if (g_HfpAg->hfp_in_calling_status == 1)
    {
        PRINTF("Simulate a outcoming calling!!\r\n");
        bt_hfp_ag_send_callsetup_indicator(hfp_ag, 1);
        g_HfpAg->hfp_in_calling_status = 2;
    }
}

void ata_response(struct bt_hfp_ag *hfp_ag)
{
    app_hfp_ag_t *app_hfp_ag = app_ag_get_instance(hfp_ag);

    if (app_hfp_ag != NULL)
    {
        k_work_submit(&app_hfp_ag->ataRespWork);
    }
}

void chup_response(struct bt_hfp_ag *hfp_ag)
{
    app_hfp_ag_t *app_hfp_ag = app_ag_get_instance(hfp_ag);
    
    if (app_hfp_ag == NULL)
    {
        return;
    }

    printf("HFP HP have ended the call\n");
    app_hfp_ag->hfp_in_calling_status = 1;
    bt_hfp_ag_call_status_pl(hfp_ag, hfp_ag_call_call_end);
    bt_hfp_ag_send_call_indicator(hfp_ag, 0);
    if (app_hfp_ag->xTimers != 0)
    {
        xTimerStop(app_hfp_ag->xTimers, 0);
        xTimerDelete(app_hfp_ag->xTimers, 0);
        app_hfp_ag->xTimers = 0;
    }
}

static void brva(struct bt_hfp_ag *hfp_ag, uint32_t value)
{
    printf("HFP voice recognition :%d\n", value);
}
static void codec_negotiate(struct bt_hfp_ag *hfp_ag, uint32_t value)
{
    printf("HFP codec negotiate :%d\n", value);
}

static void chld(struct bt_hfp_ag *hfp_ag, uint8_t option, uint8_t index)
{
    app_hfp_ag_t *app_hfp_ag = app_ag_get_instance(hfp_ag);

    if (app_hfp_ag == NULL)
    {
        return;
    }

    printf("AT_CHLD mutlipcall option  index :%d %d\n", option, index);
    if (option == 0)
    {
        printf(
            " Release all Held Calls and set UUDB tone "
            "(Reject new incoming waiting call)\n");
    }
    else if (option == 1)
    {
        printf("  Release Active Calls and accept held/waiting call\n");
    }
    else if (option == 2)
    {
        printf(
            "  Hold Active Call and accept already "
            "held/new waiting call\n");
    }
    else if (option == 3)
    {
        printf(" bt multipcall 3. Conference all calls\n");
    }
    else if (option == 4)
    {
        printf(" bt multipcall 4. Connect other calls and disconnect self from TWC\n");
    }
    if (app_hfp_ag->xTwcTimers != 0)
    {
        xTimerStop(app_hfp_ag->xTwcTimers, 0);
        xTimerDelete(app_hfp_ag->xTwcTimers, 0);
        app_hfp_ag->xTwcTimers = 0;
    }
}

void get_config(struct bt_hfp_ag *hfp_ag, hfp_ag_get_config **config)
{
    *config = &hfp_ag_config;
}

void hf_indicator(struct bt_hfp_ag *hfp_ag, uint16_t indicator, uint32_t value)
{
    if (indicator == HF_INDICATOR_BATTERY_LEVEL)
    {
        PRINTF("battery level: %d\r\n", value);
    }
    else if (indicator == HF_INDICATOR_ENHANCED_DRIVER_SAFETY)
    {
        PRINTF("enhanced driver safety: %d\r\n", value);
    }
    else
    {
    }
}

static struct bt_hfp_ag_cb ag_cb = {
    .connected       = ag_connected,
    .disconnected    = ag_disconnected,
    .ata_response    = ata_response,
    .chup_response   = chup_response,
    .dial            = dial,
    .brva            = brva,
    .chld            = chld,
    .codec_negotiate = codec_negotiate,
    .get_config      = get_config,
    .hf_indicator    = hf_indicator,
};

int app_hfp_ag_discover(struct bt_conn *conn, uint8_t channel)
{
    struct bt_hfp_ag *hfp_agHandle;
    int status = 0;

    if (!app_ag_check_idle_instance())
    {
        PRINTF("no idle instance\r\n");
        return 0;
    }

    hfp_ag_config.server_channel = channel;
    status = bt_hfp_ag_connect(conn, &hfp_ag_config, &ag_cb, &hfp_agHandle);
    if (0 != status)
    {
        PRINTF("fail to connect hfp_hf (err: %d)\r\n", status);
    }
    return status;
}

static void bt_ready(int err)
{
    struct net_buf *buf = NULL;
    struct bt_hci_cp_write_class_of_device *cp;

    if (err)
    {
        PRINTF("Bluetooth init failed (err %d)\n", err);
        return;
    }

#if (defined(CONFIG_BT_SETTINGS) && (CONFIG_BT_SETTINGS > 0))
    settings_load();
#endif /* CONFIG_BT_SETTINGS */

    PRINTF("Bluetooth initialized\n");

    buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));
        sys_put_le24(APP_CLASS_OF_DEVICE, &cp->class_of_device[0]);
        err = bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, buf, NULL);
    }
    else
    {
        err = -ENOBUFS;
    }

    app_connect_init();

    err = bt_br_set_connectable(true);
    if (err)
    {
        PRINTF("BR/EDR set/rest connectable failed (err %d)\n", err);
        return;
    }
    err = bt_br_set_discoverable(true);
    if (err)
    {
        PRINTF("BR/EDR set discoverable failed (err %d)\n", err);
        return;
    }
    PRINTF("BR/EDR set connectable and discoverable done\n");
    bt_sdp_register_service(&hfp_ag_rec);

    bt_hfp_ag_init();
    bt_hfp_ag_register_cb(&ag_cb);
    app_hfp_ag_select_conn(0);
    app_shell_init();
    for (uint8_t index = 0; index < CONFIG_BT_HFP_AG_MAX_CONN; index++)
    {
        k_work_init(&g_HfpAgs[index].ataRespWork, bt_work_ata_response);
        g_HfpAgs[index].hfp_in_calling_status = 0xFF;
    }
}

static void vTimerRingCallback(TimerHandle_t xTimer)
{
    bt_hfp_ag_send_callring(g_HfpAg->hfp_agHandle);
}

static void vTimerTwcRingCallback(TimerHandle_t xTimer)
{
    bt_hfp_ag_send_callring(g_HfpAg->hfp_agHandle);
}
int app_hfp_ag_start_incoming_call()
{
    if (g_HfpAg->hfp_in_calling_status == 1)
    {
        PRINTF("Simulate a incoming call an incoming calling!!\r\n");
        bt_hfp_ag_send_callsetup_indicator(g_HfpAg->hfp_agHandle, 1);
        g_HfpAg->xTimers = xTimerCreate("RingTimer", (2000) + 10, pdTRUE, 0, vTimerRingCallback);
        xTimerStart(g_HfpAg->xTimers, 0);
        bt_hfp_ag_send_callring(g_HfpAg->hfp_agHandle);
        g_HfpAg->hfp_in_calling_status = 2;
        return 0;
    }
    return -1;
}
int app_hfp_ag_start_twc_incoming_call(void)
{
    if (g_HfpAg->hfp_in_calling_status == 3)
    {
        PRINTF("Simulate a mutiple call incoming call!!\r\n");
        bt_hfp_ag_send_callsetup_indicator(g_HfpAg->hfp_agHandle, 1);
        bt_hfp_ag_send_ccwa_indicator(g_HfpAg->hfp_agHandle, "1234567");
        g_HfpAg->xTwcTimers = xTimerCreate("TwcRingTimer", (2000) + 10, pdTRUE, 0, vTimerTwcRingCallback);
        xTimerStart(g_HfpAg->xTwcTimers, 0);
        bt_hfp_ag_send_callring(g_HfpAg->hfp_agHandle);
        g_HfpAg->hfp_in_calling_status = 4;
        return 0;
    }
    return -1;
}

void app_hfp_ag_open_audio()
{
    bt_hfp_ag_open_audio(g_HfpAg->hfp_agHandle, g_HfpAg->selectCodec - 1);
}
void app_hfp_ag_close_audio()
{
    bt_hfp_ag_close_audio(g_HfpAg->hfp_agHandle);
}
int app_hfp_ag_accept_incoming_call()
{
    if (g_HfpAg->hfp_in_calling_status == 2)
    {
        printf("HFP AG have accepted the incoming call\n");
        g_HfpAg->hfp_in_calling_status = 3;
        bt_hfp_ag_send_callsetup_indicator(g_HfpAg->hfp_agHandle, 0);
        bt_hfp_ag_send_call_indicator(g_HfpAg->hfp_agHandle, 1);
        if (g_HfpAg->xTimers != 0)
        {
            xTimerStop(g_HfpAg->xTimers, 0);
            xTimerDelete(g_HfpAg->xTimers, 0);
            g_HfpAg->xTimers = 0;
        }
        bt_hfp_ag_call_status_pl(g_HfpAg->hfp_agHandle, hfp_ag_call_call_incoming);
        return 0;
    }
    return -1;
}
int app_hfp_ag_stop_incoming_call()
{
    if (g_HfpAg->hfp_in_calling_status >= 2)
    {
        bt_hfp_ag_call_status_pl(g_HfpAg->hfp_agHandle, hfp_ag_call_call_end);
        if (g_HfpAg->xTimers != 0)
        {
            xTimerStop(g_HfpAg->xTimers, 0);
            xTimerDelete(g_HfpAg->xTimers, 0);
            g_HfpAg->xTimers = 0;
        }
        bt_hfp_ag_send_call_indicator(g_HfpAg->hfp_agHandle, 0);
        printf("HFP AG have ended the call\n");
        g_HfpAg->hfp_in_calling_status = 1;
        return 0;
    }
    return -1;
}
int app_hfp_ag_codec_select(uint8_t codec)
{
    g_HfpAg->selectCodec = codec;
    return bt_hfp_ag_codec_selector(g_HfpAg->hfp_agHandle, codec);
}
void app_hfp_ag_set_phnum_tag(char *name)
{
    bt_hfp_ag_set_phnum_tag(g_HfpAg->hfp_agHandle, name);
}
void app_hfp_ag_volume_update(hf_ag_volume_type_t type, int volume)
{
    bt_hfp_ag_set_volume_control(g_HfpAg->hfp_agHandle, type, volume);
}

int app_hfp_ag_set_hf_indicator(uint8_t indicator, uint8_t control)
{
    return bt_hfp_ag_set_hf_indicator(g_HfpAg->hfp_agHandle, indicator, control);
}

void app_hfp_ag_select_conn(uint8_t index)
{
    struct bt_conn *acl_conn;

    acl_conn = bt_hfp_ag_get_conn(g_HfpAgs[index].hfp_agHandle);

    if (acl_conn == NULL)
    {
        PRINTF("connection %d is invalid",index);
    }
    else
    {
        g_HfpAg = &g_HfpAgs[index];
    }
}

void peripheral_hfp_ag_task(void *pvParameters)
{
    int err = 0;

    PRINTF("Bluetooth Handsfree AG demo start...\n");

    /* Initializate BT Host stack */
    err = bt_enable(bt_ready);
    if (err)
    {
        PRINTF("Bluetooth init failed (err %d)\n", err);
        return;
    }
    vTaskDelete(NULL);
}
