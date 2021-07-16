/*
 * Copyright 2020 - 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <porting.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/atomic.h>
#include <sys/byteorder.h>
#include <sys/util.h>
#include <sys/slist.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/a2dp.h>
#include <bluetooth/a2dp-codec.h>
#include <bluetooth/sdp.h>
#include "clock_config.h"
#include "board.h"
#include "fsl_sai.h"
#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "fsl_codec_common.h"
#include "fsl_wm8960.h"
#include "fsl_codec_adapter.h"
#include "fsl_debug_console.h"
#include "app_connect.h"

#define A2DP_CLASS_OF_DEVICE (0x200404U)
#define APP_A2DP_STREAMER_SYNC_TASK_PRIORITY (5U)

extern void BOARD_CodecStop(void);
extern void BOARD_CodecStart(void);
extern void BOARD_CodecSet(uint32_t sample_rate, uint8_t channels);
extern void BOARD_CodecStreamerPlay(uint8_t *data, uint32_t length);

struct bt_a2dp *default_a2dp;
static uint32_t audio_start;
static QueueHandle_t audio_sem;

static struct bt_sdp_attribute a2dp_sink_attrs[] = {
    BT_SDP_NEW_SERVICE,
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_AUDIO_SINK_SVCLASS) //11 0B
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROTO_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 16),//35 10
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),// 35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) // 01 00
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(BT_UUID_AVDTP_VAL) // 00 19
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),// 35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_UUID_AVDTP_VAL) // 00 19
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0X0100u) //AVDTP version: 01 00
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
                BT_SDP_ARRAY_16(BT_SDP_ADVANCED_AUDIO_SVCLASS) //11 0d
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0x0103U) //01 03
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("A2DPSink"),
    BT_SDP_SUPPORTED_FEATURES(0x0001U),
};

static struct bt_sdp_record a2dp_sink_rec = BT_SDP_RECORD(a2dp_sink_attrs);

static uint32_t app_a2dp_sbc_get_sample_rate(struct bt_a2dp_preset *config)
{
    if (config == NULL)
    {
        return 0U;
    }

    if (config->preset[0] & A2DP_SBC_SAMP_FREQ_16000)
    {
        return 16000U;
    }
    else if (config->preset[0] & A2DP_SBC_SAMP_FREQ_32000)
    {
        return 32000U;
    }
    else if (config->preset[0] & A2DP_SBC_SAMP_FREQ_44100)
    {
        return 44100U;
    }
    else if (config->preset[0] & A2DP_SBC_SAMP_FREQ_48000)
    {
        return 48000U;
    }
    else
    {
        return 0U;
    }
}

static uint32_t app_a2dp_sbc_get_channel_number(struct bt_a2dp_preset *config)
{
    if (config == NULL)
    {
        return 0U;
    }

    /* Decode Support for Channel Mode */
    if (config->preset[0] & A2DP_SBC_CH_MODE_MONO)
    {
        return 1U;
    }
    else if (config->preset[0] & A2DP_SBC_CH_MODE_DUAL)
    {
        return 2U;
    }
    else if (config->preset[0] & A2DP_SBC_CH_MODE_STREO)
    {
        return 2U;
    }
    else if (config->preset[0] & A2DP_SBC_CH_MODE_JOINT)
    {
        return 2U;
    }
    else
    {
        return 0U;
    }
}

void app_audio_streamer_task_signal(void)
{    
    if (0U != __get_IPSR())
    {
        portBASE_TYPE task_to_wake = pdFALSE;

        if (pdTRUE == xSemaphoreGiveFromISR(audio_sem, &task_to_wake))
        {
            portYIELD_FROM_ISR((task_to_wake));
        }
    }
    else
    {
        xSemaphoreGive(audio_sem);
    }
}

void AudioTask(void *handle)
{
    audio_sem = xSemaphoreCreateCounting(0xFF, 0U);
    if (NULL == audio_sem)
    {
        vTaskDelete(NULL);
    }

    while (1U)
    {
        if (pdFALSE == xSemaphoreTake(audio_sem, portMAX_DELAY))
        {
            continue;
        }

        bt_a2dp_snk_media_sync(default_a2dp, NULL, 0U);
    }
}

void app_configured(struct bt_a2dp *a2dp, struct a2dp_configure_result *configResult)
{
    if (configResult->err == 0)
    {
        BOARD_CodecSet(app_a2dp_sbc_get_sample_rate(configResult->config), app_a2dp_sbc_get_channel_number(configResult->config));
    }
}

void app_start_play(struct bt_a2dp *a2dp, int err)
{
    if (err == 0)
    {
        audio_start = 1;
        /* Start Audio Player */
        BOARD_CodecStart();
        PRINTF("a2dp start playing\r\n");
    }
}

void app_suspend_play(struct bt_a2dp *a2dp, int err)
{
    if (err == 0)
    {
        audio_start = 0;
        /* Stop Audio Player */
        BOARD_CodecStop();
        PRINTF("a2dp stop playing\r\n");
    }
}

void app_streamer_data(struct bt_a2dp *a2dp, uint8_t *data, uint32_t length)
{
    if ((data != NULL) && (length != 0U))
    {
        if(0 == audio_start)
        {
            /*return;*/
        }
        BOARD_CodecStreamerPlay(data, length);
    }
}

void app_connected(struct bt_a2dp *a2dp, int err)
{
    struct bt_a2dp_control_cb controlCb;

    if (!err)
    {
        default_a2dp = a2dp;
        controlCb.configured = app_configured;
        controlCb.sink_start_play = app_start_play;
        controlCb.sink_suspend_play = app_suspend_play;
        controlCb.sink_streamer_data = app_streamer_data;
        bt_a2dp_register_control_callback(a2dp, &controlCb);
        PRINTF("a2dp connected success\r\n");
    }
    else
    {
        PRINTF("a2dp connected fail\r\n");
    }
}

void app_disconnected(struct bt_a2dp *a2dp)
{
    audio_start = 0;
    /* Stop Audio Player */
    BOARD_CodecStop();
}

BT_A2DP_SBC_SINK_ENDPOINT(sbcEndpoint);

static void app_edgefast_a2dp_init(void)
{
    BaseType_t result = 0;
    result =
        xTaskCreate(AudioTask, "audio", 2048U, NULL, APP_A2DP_STREAMER_SYNC_TASK_PRIORITY, NULL);
    assert(pdPASS == result);
    (void)result;

    struct bt_a2dp_connect_cb connectCb;
    connectCb.connected = app_connected;
    connectCb.disconnected = app_disconnected;

    bt_a2dp_register_endpoint(&sbcEndpoint, BT_A2DP_AUDIO, BT_A2DP_SINK);

    bt_a2dp_register_connect_callback(&connectCb);
}

static void bt_ready(int err)
{
    struct net_buf *buf = NULL;
    struct bt_hci_cp_write_class_of_device *cp;

    if (err) {
        PRINTF("Bluetooth init failed (err %d)\n", err);
        return;
    }

    PRINTF("Bluetooth initialized\n");

    buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, sizeof(*cp));
    if (buf != NULL)
    {
        cp = net_buf_add(buf, sizeof(*cp));
        sys_put_le24(A2DP_CLASS_OF_DEVICE, &cp->class_of_device[0]);
        err = bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, buf, NULL);
    }
    else
    {
        err = -ENOBUFS;
    }

    if (err)
    {
        PRINTF("setting class of device failed\n");
    }
    
    app_connect_init();

    err = bt_br_set_connectable(true);
    if (err) {
        PRINTF("BR/EDR set/rest connectable failed (err %d)\n", err);
        return;
    }
    err = bt_br_set_discoverable(true);
    if (err) {
        PRINTF("BR/EDR set discoverable failed (err %d)\n", err);
        return;
    }
    PRINTF("BR/EDR set connectable and discoverable done\n");

    bt_sdp_register_service(&a2dp_sink_rec);
    app_edgefast_a2dp_init();
}

void app_a2dp_sink_task(void *pvParameters)
{
    int err = 0;
    (void)err;

    /* Initializate BT Host stack */
    err = bt_enable(bt_ready);
    if (err) {
        PRINTF("Bluetooth init failed (err %d)\n", err);
        return;
    }

    vTaskDelete(NULL);
}
