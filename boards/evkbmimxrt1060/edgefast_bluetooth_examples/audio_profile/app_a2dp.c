/*
 * Copyright 2020 - 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

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
#include <bluetooth/a2dp.h>
#include <bluetooth/a2dp_codec_sbc.h>
#include "app_common.h"
#include "app_a2dp.h"
#include "app_music_control.h"

#define APPL_A2DP_MTU           (672U)
#define A2DP_SRC_PERIOD_MS      (10U)
#define A2DP_SOURCE_BUFFER_SIZE (2048U + 512U)

static uint32_t a2dp_src_sf;
static uint16_t a2dp_src_num_samples;
static int32_t a2dp_src_sent_ms;
static uint32_t a2dp_src_missed_count;
static uint8_t a2dp_src_playback;
static TimerHandle_t a2dp_src_timer;
static uint8_t s_A2dpSourceSendBuffer[A2DP_SOURCE_BUFFER_SIZE];
app_a2dp_t app_a2dp;
BT_A2DP_SBC_SOURCE_ENDPOINT(sbcEndpoint, A2DP_SBC_SAMP_FREQ_44100);

static void a2dp_pl_start_playback_timer(void);
int streamer_pcm_get_data(uint8_t *dest, uint32_t length);

static void a2dp_pl_produce_media(void)
{
    uint32_t count = a2dp_src_num_samples << 2;
    if (count > A2DP_SOURCE_BUFFER_SIZE)
    {
        count = A2DP_SOURCE_BUFFER_SIZE;
    }
    if (streamer_pcm_get_data(&s_A2dpSourceSendBuffer[0], count) != 0)
    {
        /* send data */
        bt_a2dp_src_media_write(app_a2dp.a2dp_ep, &s_A2dpSourceSendBuffer[0], count);
    }
}

static void a2dp_pl_playback_timeout_handler(TimerHandle_t timer_id)
{
    int32_t now_ms, period_ms;
    TickType_t ticks;

    /* If stopped then return */
    if (0U == a2dp_src_playback)
    {
        return;
    }

    /* Get the current time */
    if (0U != __get_IPSR())
    {
        ticks = xTaskGetTickCountFromISR();
    }
    else
    {
        ticks = xTaskGetTickCount();
    }

    now_ms = ((uint32_t)((uint64_t)(ticks)*1000uL / (uint64_t)configTICK_RATE_HZ));
    period_ms = A2DP_SRC_PERIOD_MS;

    /* Adjust the period */
    if (a2dp_src_sent_ms > 0)
    {
        period_ms = (now_ms > a2dp_src_sent_ms)?
            (now_ms - a2dp_src_sent_ms): A2DP_SRC_PERIOD_MS;
    }

    /* Get the number of samples */
    a2dp_src_num_samples = (period_ms * a2dp_src_sf) / 1000;
    a2dp_src_missed_count += (period_ms * a2dp_src_sf) % 1000;

    /* Raw adjust for the drift */
    while (a2dp_src_missed_count >= 1000)
    {
        a2dp_src_num_samples++;
        a2dp_src_missed_count -= 1000;
    }

    /* Update the sent timestamp */
    a2dp_src_sent_ms = now_ms;

    a2dp_pl_produce_media();
}

static void a2dp_pl_start_playback_timer(void)
{
    a2dp_src_timer = NULL;
    a2dp_src_timer = xTimerCreate("a2dp play", (A2DP_SRC_PERIOD_MS / portTICK_PERIOD_MS),
                                  pdTRUE, NULL, a2dp_pl_playback_timeout_handler);
    xTimerStart(a2dp_src_timer, 0);
}

void app_endpoint_configured(struct bt_a2dp_endpoint_configure_result *result)
{
    if (result->err == 0)
    {
        app_a2dp.a2dp_ep = &sbcEndpoint;
        music_player_ready();

        a2dp_src_sf = bt_a2dp_sbc_get_sampling_frequency((struct bt_a2dp_codec_sbc_params *)&result->config.media_config->codec_ie[0]);
    }
}

void app_connected(struct bt_a2dp *a2dp, int err)
{
    if (!err)
    {
        app_a2dp.a2dp_handle = a2dp;
        bt_a2dp_configure(a2dp, NULL);
    }
    else
    {
        if (app_a2dp.a2dp_handle != NULL)
        {
            app_a2dp.a2dp_handle = NULL;
        }
    }
}

void app_disconnected(struct bt_a2dp *a2dp)
{
    if (app_a2dp.a2dp_handle != NULL)
    {
        app_a2dp.a2dp_handle = NULL;
    }
    a2dp_src_playback = 0U;
    music_player_lost();
    if (a2dp_src_timer != NULL)
    {
        /* Stop Audio Source */
        xTimerStop(a2dp_src_timer, 0);
        xTimerDelete(a2dp_src_timer, 0);
        a2dp_src_timer = NULL;
    }
}

static void music_control_a2dp_start_callback(int err)
{
    /* Start Audio Source */
    a2dp_src_playback = 1U;
    a2dp_pl_start_playback_timer();
}

static void music_control_a2dp_suspend_callback(int err)
{
    a2dp_src_playback = 0U;
    if (a2dp_src_timer != NULL)
    {
        /* Stop Audio Source */
        xTimerStop(a2dp_src_timer, 0);
        xTimerDelete(a2dp_src_timer, 0);
        a2dp_src_timer = NULL;
    }
}

void app_edgefast_a2dp_init(void)
{
    struct bt_a2dp_connect_cb connectCb;
    connectCb.connected = app_connected;
    connectCb.disconnected = app_disconnected;

    sbcEndpoint.control_cbs.start_play = music_control_a2dp_start_callback;
    sbcEndpoint.control_cbs.stop_play = music_control_a2dp_suspend_callback;
    sbcEndpoint.control_cbs.configured = app_endpoint_configured;
    bt_a2dp_register_endpoint(&sbcEndpoint, BT_A2DP_AUDIO, BT_A2DP_SOURCE);

    bt_a2dp_register_connect_callback(&connectCb);
}

void app_edgefast_a2dp_suspend(void)
{
    bt_a2dp_stop(app_a2dp.a2dp_ep);
}

void app_edgefast_a2dp_start(void)
{
    bt_a2dp_start(app_a2dp.a2dp_ep);
}
