/* a2dp.c - Bluetooth L2CAP Tester */

/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if defined(CONFIG_BT_A2DP) && (CONFIG_BT_A2DP > 0)

#include <porting.h>

#include "bt_pal_conn_internal.h"

#include <bluetooth/l2cap.h>
#include <bluetooth/a2dp.h>
#include <bluetooth/a2dp_codec_sbc.h>
#include <bluetooth/sdp.h>

#include "board.h"

#include "fsl_adapter_audio.h"
#include "fsl_codec_common.h"

#include "a2dp_pl_media_48KHz.h"
#include "btp/btp.h"

#include "fsl_component_log_config.h"
#define LOG_MODULE_NAME bttester_a2dp
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

struct bt_a2dp *default_a2dp;
struct bt_a2dp_endpoint *default_a2dp_endpoint;

/* a2dp endpoint capability buffer */
static uint8_t bt_a2dp_endpoint_cap_buffer[BT_A2DP_SBC_IE_LENGTH + 1] =
{BT_A2DP_SBC_IE_LENGTH, A2DP_SBC_SAMP_FREQ_16000 | A2DP_SBC_SAMP_FREQ_32000 | A2DP_SBC_SAMP_FREQ_44100 | A2DP_SBC_SAMP_FREQ_48000 | \
A2DP_SBC_CH_MODE_MONO | A2DP_SBC_CH_MODE_DUAL | A2DP_SBC_CH_MODE_STREO | A2DP_SBC_CH_MODE_JOINT, A2DP_SBC_BLK_LEN_4 | \
A2DP_SBC_BLK_LEN_8 | A2DP_SBC_BLK_LEN_12 | A2DP_SBC_BLK_LEN_16 | A2DP_SBC_SUBBAND_4 | A2DP_SBC_SUBBAND_8 | A2DP_SBC_ALLOC_MTHD_SNR | \
A2DP_SBC_ALLOC_MTHD_LOUDNESS, 2U, 250U};
/* a2dp endpoint configure buffer */
static uint8_t bt_a2dp_endpoint_conf_buffer[BT_A2DP_SBC_IE_LENGTH + 1] =
{BT_A2DP_SBC_IE_LENGTH, A2DP_SBC_SAMP_FREQ_48000 | A2DP_SBC_CH_MODE_JOINT, A2DP_SBC_BLK_LEN_16 |\
A2DP_SBC_SUBBAND_8 | A2DP_SBC_ALLOC_MTHD_LOUDNESS, 18U, 35U};
/* a2dp endpoint codec none cahceable buffer */
AT_NONCACHEABLE_SECTION(static uint32_t bt_a2dp_endpoint_codec_buffer_nocached[(BT_A2DP_SINK_SBC_CODEC_BUFFER_NOCACHED_SIZE + 3) / 4]);

/* a2dp endpoint codec buffer */
#if defined(CONFIG_BT_A2DP_SINK) && (CONFIG_BT_A2DP_SINK > 0)
#define BT_A2DP_SBC_CODEC_BUFFER_SIZE BT_A2DP_SINK_SBC_CODEC_BUFFER_SIZE
#endif
#if defined(CONFIG_BT_A2DP_SOURCE) && (CONFIG_BT_A2DP_SOURCE > 0)
#if defined(CONFIG_BT_A2DP_SINK) && (CONFIG_BT_A2DP_SINK > 0)
#undef BT_A2DP_SBC_CODEC_BUFFER_SIZE
#endif
#define BT_A2DP_SBC_CODEC_BUFFER_SIZE BT_A2DP_SOURCE_SBC_CODEC_BUFFER_SIZE
#endif
static uint32_t bt_a2dp_endpoint_codec_buffer[(BT_A2DP_SBC_CODEC_BUFFER_SIZE + 3) / 4];

#if defined(CONFIG_BT_A2DP_SINK) && (CONFIG_BT_A2DP_SINK > 0)
#define A2DP_CLASS_OF_DEVICE (0x200404U)
#define APP_A2DP_STREAMER_SYNC_TASK_PRIORITY (5U)
/*If codec is cs42448, it doesn't support DAC CHANNAL, the macro A2DP_CODEC_DAC_VOLUME is meaningless.*/
#define A2DP_CODEC_DAC_VOLUME (100U) /* Range: 0 ~ 100 */
#define A2DP_CODEC_HP_VOLUME  (100U)  /* Range: 0 ~ 100 */

static uint32_t audio_start;
static QueueHandle_t audio_sem;

/* a2dp sink endpoint */
static struct bt_a2dp_endpoint sbcEndpointSink;

extern hal_audio_config_t audioTxConfig;
extern codec_config_t boardCodecConfig;
AT_NONCACHEABLE_SECTION_ALIGN(static HAL_AUDIO_HANDLE_DEFINE(audio_tx_handle), 4);
static codec_handle_t codec_handle;

extern uint32_t BOARD_SwitchAudioFreq(uint32_t sampleRate);
#endif /* CONFIG_BT_A2DP_SINK > 0 */

/* varabiles for a2dp source */
#if defined(CONFIG_BT_A2DP_SOURCE) && (CONFIG_BT_A2DP_SOURCE > 0)
#define SDP_CLIENT_USER_BUF_LEN		512U
NET_BUF_POOL_FIXED_DEFINE(app_sdp_client_pool, CONFIG_BT_MAX_CONN,
			  SDP_CLIENT_USER_BUF_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

#define APPL_A2DP_MTU   (672U)
#define A2DP_SRC_PERIOD_MS    10

static uint32_t a2dp_src_sf;
static uint16_t a2dp_src_num_samples;

static int32_t a2dp_src_sent_ms;
static uint32_t a2dp_src_missed_count;
static volatile uint8_t a2dp_src_playback;
static TimerHandle_t a2dp_src_timer;
static int tone_index;
uint8_t a2dp_src_nc;

static struct bt_a2dp_endpoint sbcEndpointSource;
#endif /* CONFIG_BT_A2DP_SOURCE > 0 */

#if defined(CONFIG_BT_A2DP_SOURCE) && (CONFIG_BT_A2DP_SOURCE > 0)
static void a2dp_pl_produce_media(void)
{
    uint8_t * media;
    uint16_t  medialen;
    uint8_t malloc = 0;

    /* Music Audio is Stereo */
    medialen = (a2dp_src_num_samples << a2dp_src_nc);

    /* For mono or dual configuration, skip alternative samples */
    if (1 == a2dp_src_nc)
    {
        uint16_t index;

        /* Allocate Memory */
        malloc = 1;
        media = (uint8_t *)pvPortMalloc(medialen);

        if (NULL == media)
        {
            return;
        }

        for (index = 0; index < a2dp_src_num_samples; index++)
        {
            media[(2 * index)] = *((uint8_t *)beethoven + tone_index);
            media[(2 * index) + 1] = *((uint8_t *)beethoven + tone_index + 1);
            /* Update the tone index */
            tone_index += 4u;
            if (tone_index >= sizeof(beethoven))
            {
                tone_index = 0U;
            }
        }
    }
    else
    {
        if ((tone_index + (a2dp_src_num_samples << 2)) > sizeof(beethoven))
        {
            malloc = 1;
            media = (uint8_t *)pvPortMalloc(medialen);
            if (NULL == media)
            {
                PRINTF("Memory Allocation failed in Produce Media\n");
                return;
            }
            memcpy(media, ((uint8_t *)beethoven + tone_index), sizeof(beethoven) - tone_index);
            memcpy(&media[sizeof(beethoven) - tone_index],
                   ((uint8_t *)beethoven),
                   ((a2dp_src_num_samples << 2) - (sizeof(beethoven) - tone_index)));
            /* Update the tone index */
            tone_index = ((a2dp_src_num_samples << 2) - (sizeof(beethoven) - tone_index));
        }
        else
        {
            media = ((uint8_t *)beethoven + tone_index);
            /* Update the tone index */
            tone_index += (a2dp_src_num_samples << 2);
            if (tone_index >= sizeof(beethoven))
            {
                tone_index = 0U;
            }
        }
    }

    /* Give data to callback */
    bt_a2dp_src_media_write(default_a2dp_endpoint, media, medialen);

    if (malloc == 1)
    {
        vPortFree(media);
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
#endif /* CONFIG_BT_A2DP_SOURCE > 0 */

#if 0
static uint8_t a2dp_restart(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
    a2dp_src_timer = xTimerCreate("a2dp play", (A2DP_SRC_PERIOD_MS / portTICK_PERIOD_MS),
                                  pdTRUE, NULL, a2dp_pl_playback_timeout_handler);
    xTimerStart(a2dp_src_timer, 0);

    return BTP_STATUS_SUCCESS;
}
#endif

#if defined(CONFIG_BT_A2DP_SINK) && (CONFIG_BT_A2DP_SINK > 0)
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

        bt_a2dp_snk_media_sync(default_a2dp_endpoint, NULL, 0U);
    }
}

static void tx_callback(hal_audio_handle_t handle, hal_audio_status_t completionStatus, void *callbackParam)
{
    app_audio_streamer_task_signal();
}

void sbc_configured_sink(struct bt_a2dp_endpoint_configure_result *configResult)
{
    if (configResult->err == 0)
    {
        audioTxConfig.sampleRate_Hz  = bt_a2dp_sbc_get_sampling_frequency((struct bt_a2dp_codec_sbc_params *)&configResult->config.media_config->codec_ie[0]);
        audioTxConfig.lineChannels = (hal_audio_channel_t)bt_a2dp_sbc_get_channel_num((struct bt_a2dp_codec_sbc_params *)&configResult->config.media_config->codec_ie[0]);
        audioTxConfig.srcClock_Hz = BOARD_SwitchAudioFreq(audioTxConfig.sampleRate_Hz);

        PRINTF("a2dp configure sample rate %dHz\r\n", audioTxConfig.sampleRate_Hz);

        HAL_AudioTxInit((hal_audio_handle_t)&audio_tx_handle[0], &audioTxConfig);
        HAL_AudioTxInstallCallback((hal_audio_handle_t)&audio_tx_handle[0], tx_callback, NULL);

        if (CODEC_Init(&codec_handle, &boardCodecConfig) != kStatus_Success)
        {
            PRINTF("codec init failed!\r\n");
        }
        CODEC_SetMute(&codec_handle, kCODEC_PlayChannelHeadphoneRight | kCODEC_PlayChannelHeadphoneLeft, true);
        CODEC_SetFormat(&codec_handle, audioTxConfig.srcClock_Hz, audioTxConfig.sampleRate_Hz, audioTxConfig.bitWidth);
        CODEC_SetVolume(&codec_handle, kCODEC_VolumeDAC, A2DP_CODEC_DAC_VOLUME);
        CODEC_SetVolume(&codec_handle, kCODEC_VolumeHeadphoneLeft | kCODEC_VolumeHeadphoneRight, A2DP_CODEC_HP_VOLUME);
        CODEC_SetMute(&codec_handle, kCODEC_PlayChannelHeadphoneRight | kCODEC_PlayChannelHeadphoneLeft, false);
    }
}

void sbc_deconfigured_sink(int err)
{
    if (err == 0)
    {
        audio_start = 0;
        /* Stop Audio Player */
        PRINTF("a2dp deconfigure\r\n");
        CODEC_SetMute(&codec_handle, kCODEC_PlayChannelHeadphoneRight | kCODEC_PlayChannelHeadphoneLeft, true);
        HAL_AudioTxDeinit((hal_audio_handle_t)&audio_tx_handle[0]);
        (void)BOARD_SwitchAudioFreq(0U);
    }
}

void sbc_start_play_sink(int err)
{
    if (err == 0)
    {
        audio_start = 1;
        /* Start Audio Player */
        PRINTF("a2dp start playing\r\n");
    }
}

void sbc_stop_play_sink(int err)
{
    if (err == 0)
    {
        audio_start = 0;
        /* Stop Audio Player */
        PRINTF("a2dp stop playing\r\n");
        HAL_AudioTransferAbortSend((hal_audio_handle_t)&audio_tx_handle[0]);
    }
}

void sbc_streamer_data_sink(uint8_t *data, uint32_t length)
{
    if ((data != NULL) && (length != 0U))
    {
        hal_audio_transfer_t xfer;

        if(0 == audio_start)
        {
            /*return;*/
        }

        xfer.dataSize       = length;
        xfer.data           = data;

        if (kStatus_HAL_AudioSuccess != HAL_AudioTransferSendNonBlocking((hal_audio_handle_t)&audio_tx_handle[0], &xfer))
        {
            PRINTF("prime fail\r\n");
        }
    }
}
#endif /* CONFIG_BT_A2DP_SINK > 0 */

#if defined(CONFIG_BT_A2DP_SOURCE) && (CONFIG_BT_A2DP_SOURCE > 0)
static struct bt_sdp_attribute a2dp_source_attrs[] = {
    BT_SDP_NEW_SERVICE,
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3),
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16),
            BT_SDP_ARRAY_16(BT_SDP_AUDIO_SOURCE_SVCLASS)
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROTO_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 16),
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16),
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP)
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16),
                BT_SDP_ARRAY_16(BT_UUID_AVDTP_VAL)
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16),
                BT_SDP_ARRAY_16(BT_UUID_AVDTP_VAL)
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16),
                BT_SDP_ARRAY_16(0X0100u)
            },
            )
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROFILE_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8),
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16),
                BT_SDP_ARRAY_16(BT_SDP_ADVANCED_AUDIO_SVCLASS)
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16),
                BT_SDP_ARRAY_16(0x0103U)
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("A2DPSource"),
    BT_SDP_SUPPORTED_FEATURES(0x0001U),
};
static struct bt_sdp_record a2dp_source_rec = BT_SDP_RECORD(a2dp_source_attrs);

static void sbc_start_play_source(int err)
{
    /* Start Audio Source */
    a2dp_src_playback = 1U;

    a2dp_src_timer = xTimerCreate("a2dp play", (A2DP_SRC_PERIOD_MS / portTICK_PERIOD_MS),
                                  pdTRUE, NULL, a2dp_pl_playback_timeout_handler);
    xTimerStart(a2dp_src_timer, 0);
}

void sbc_configured_source(struct bt_a2dp_endpoint_configure_result *result)
{
    if (result->err == 0) {
        a2dp_src_sf = bt_a2dp_sbc_get_sampling_frequency((struct bt_a2dp_codec_sbc_params *)&result->config.media_config->codec_ie[0]);
        a2dp_src_nc = bt_a2dp_sbc_get_channel_num((struct bt_a2dp_codec_sbc_params *)&result->config.media_config->codec_ie[0]);
        PRINTF("a2dp start playing\r\n");
    }
}
#endif /* CONFIG_BT_A2DP_SOURCE > 0 */

void a2dp_connected(struct bt_a2dp *a2dp, int err)
{
#if 0
	struct btp_a2dp_connected_ev ev;

	(void)memset(&ev, 0, sizeof(ev));

	ev.status = err;
#endif

    if (!err) {
        default_a2dp = a2dp;
    }
    else {
        if (default_a2dp != NULL) {
            default_a2dp = NULL;
        }
    }

	//tester_event(BTP_SERVICE_ID_A2DP, BTP_A2DP_EV_CONNECTED, &ev, sizeof(ev));
}

void a2dp_disconnected(struct bt_a2dp *a2dp)
{
	struct btp_a2dp_disconnected_ev event;

    if (default_a2dp != NULL)
    {
        default_a2dp = NULL;
    }

#if defined(CONFIG_BT_A2DP_SINK) && (CONFIG_BT_A2DP_SINK > 0)
    audio_start = 0;
	event.role = 0;		//a2dp sink
#endif /* CONFIG_BT_A2DP_SINK > 0 */

#if defined(CONFIG_BT_A2DP_SOURCE) && (CONFIG_BT_A2DP_SOURCE > 0)
    a2dp_src_playback = 0U;
    if (a2dp_src_timer != NULL)
    {
        /* Stop Audio Source */
        xTimerStop(a2dp_src_timer, 0);
        xTimerDelete(a2dp_src_timer, 0);
        a2dp_src_timer = NULL;
    }
	event.role = 1; 	//a2dp source
    //PRINTF("a2dp disconnected\r\n");
#endif /* CONFIG_BT_A2DP_SOURCE > 0 */

	//tester_event(BTP_SERVICE_ID_A2DP, BTP_A2DP_EV_DISCONNECTED, &event, sizeof(event));
}

static uint8_t a2dp_init_sep(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
	const struct btp_a2dp_init_sep_cmd *cp = cmd;

	if (BT_A2DP_SINK == cp->role) {
#if defined(CONFIG_BT_A2DP_SINK) && (CONFIG_BT_A2DP_SINK > 0)
		/* a2dp sink */
		sbcEndpointSink = (struct bt_a2dp_endpoint)BT_A2DP_ENDPOINT_INIT(BT_A2DP_SINK, BT_A2DP_SBC, &bt_a2dp_endpoint_cap_buffer[0], &bt_a2dp_endpoint_conf_buffer[0], &bt_a2dp_endpoint_codec_buffer[0], &bt_a2dp_endpoint_codec_buffer_nocached[0]);

		/* enable delay report service */
#if defined(CONFIG_BT_A2DP_DR_SERVICE) && (CONFIG_BT_A2DP_DR_SERVICE > 0)
		sbcEndpointSink.delay_reporting_service_enable = 1U;
#endif

		/* register a2dp sink endpoint */
		sbcEndpointSink.control_cbs.configured = sbc_configured_sink;
		sbcEndpointSink.control_cbs.deconfigured = sbc_deconfigured_sink;
		sbcEndpointSink.control_cbs.start_play = sbc_start_play_sink;
		sbcEndpointSink.control_cbs.stop_play = sbc_stop_play_sink;
		sbcEndpointSink.control_cbs.sink_streamer_data = sbc_streamer_data_sink;
		bt_a2dp_register_endpoint(&sbcEndpointSink, BT_A2DP_AUDIO, BT_A2DP_SINK);

		default_a2dp_endpoint = &sbcEndpointSink;
#endif /* CONFIG_BT_A2DP_SINK > 0 */
	} else {
#if defined(CONFIG_BT_A2DP_SOURCE) && (CONFIG_BT_A2DP_SOURCE > 0)
		/* a2dp source */
		sbcEndpointSource = (struct bt_a2dp_endpoint)BT_A2DP_SOURCE_ENDPOINT_INIT(BT_A2DP_SBC, &bt_a2dp_endpoint_cap_buffer[0], &bt_a2dp_endpoint_conf_buffer[0], &bt_a2dp_endpoint_codec_buffer[0], NULL);

		/* enable delay report service */
#if defined(CONFIG_BT_A2DP_DR_SERVICE) && (CONFIG_BT_A2DP_DR_SERVICE > 0)
		sbcEndpointSource.delay_reporting_service_enable = 1U;
#endif

		sbcEndpointSource.control_cbs.start_play = sbc_start_play_source;
		sbcEndpointSource.control_cbs.configured = sbc_configured_source;
		bt_a2dp_register_endpoint(&sbcEndpointSource, BT_A2DP_AUDIO, BT_A2DP_SOURCE);

		default_a2dp_endpoint = &sbcEndpointSource;
#endif /* CONFIG_BT_A2DP_SOURCE > 0 */
	}

	return BTP_STATUS_SUCCESS;
}

static uint8_t a2dp_configure(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
	if (default_a2dp != NULL) {
		bt_a2dp_configure(default_a2dp, NULL);

		return BTP_STATUS_SUCCESS;
	} else {
		/* a2dp connection isn't created */
		return BTP_STATUS_FAILED;
	}
}

static uint8_t a2dp_start(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
	if (default_a2dp != NULL) {
		bt_a2dp_start(default_a2dp_endpoint);

		return BTP_STATUS_SUCCESS;
	} else {
		return BTP_STATUS_FAILED;
	}
}

static uint8_t a2dp_close(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
	if (default_a2dp != NULL) {
		bt_a2dp_disconnect(default_a2dp);

		return BTP_STATUS_SUCCESS;
	} else {
		return BTP_STATUS_FAILED;
	}
}

static uint8_t a2dp_suspend(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
	if (default_a2dp != NULL) {
		bt_a2dp_stop(default_a2dp_endpoint);

		return BTP_STATUS_SUCCESS;
	} else {
		return BTP_STATUS_FAILED;
	}
}

static uint8_t a2dp_connect(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
	const struct btp_a2dp_connect_cmd *cp = cmd;
	struct bt_conn *conn;
	struct channel *chan = NULL;

	conn = bt_conn_lookup_addr_br(&cp->address.a);

	if (!conn) {
		return BTP_STATUS_FAILED;
	} else {
		bt_a2dp_connect(conn);

		return BTP_STATUS_SUCCESS;
	}
}

#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
static uint8_t a2dp_init_delay_report(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
	int16_t delay;

	delay = 2200;

	if (default_a2dp != NULL) {
		bt_a2dp_set_initial_delay_report(default_a2dp_endpoint, delay);

		return BTP_STATUS_SUCCESS;
	} else {
		return BTP_STATUS_FAILED;
	}
}

static uint8_t a2dp_send_delay_report(const void *cmd, uint16_t cmd_len,
		       void *rsp, uint16_t *rsp_len)
{
	int16_t delay;

	delay = 2200;

	if (default_a2dp != NULL) {
		bt_a2dp_send_delay_report(default_a2dp_endpoint, delay);

		return BTP_STATUS_SUCCESS;
	} else {
		return BTP_STATUS_FAILED;
	}
}
#endif

static const struct btp_handler handlers[] = {
 	{
		.opcode = BTP_A2DP_INIT_SEP,
		.expect_len = sizeof(struct btp_a2dp_init_sep_cmd),
		.func = a2dp_init_sep,
	},
	{
		.opcode = BTP_A2DP_CONFIGURE,
		.expect_len = 0,
		.func = a2dp_configure,
	},
	{
		.opcode = BTP_A2DP_START,
		.expect_len = 0,
		.func = a2dp_start,
	},
	{
		.opcode = BTP_A2DP_CLOSE,
		.expect_len = 0,
		.func = a2dp_close,
	},
	{
		.opcode = BTP_A2DP_SUSPEND,
		.expect_len = 0,
		.func = a2dp_suspend,
	},
	{
		.opcode = BTP_A2DP_CONNECT,
		.expect_len = sizeof(struct btp_a2dp_connect_cmd),
		.func = a2dp_connect,
	},
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
	{
		.opcode = BTP_A2DP_INIT_DR,
		.expect_len = 0,
		.func = a2dp_init_delay_report,
	},
	{
		.opcode = BTP_A2DP_SEND_DR,
		.expect_len = 0,
		.func = a2dp_send_delay_report,
	},
#endif
#if 0
	{
		.opcode = BTP_A2DP_RESTART,
		.expect_len = 0,
		.func = a2dp_restart,
	},
#endif
};

uint8_t tester_init_a2dp(void)
{
#if defined(CONFIG_BT_A2DP_SINK) && (CONFIG_BT_A2DP_SINK > 0)
	struct net_buf *buf = NULL;
	struct bt_hci_cp_write_class_of_device *cp;

	/* send hci write device class cmd */
	buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, sizeof(*cp));
    if (buf != NULL) {
        cp = net_buf_add(buf, sizeof(*cp));
        sys_put_le24(A2DP_CLASS_OF_DEVICE, &cp->class_of_device[0]);
        (void)bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_CLASS_OF_DEVICE, buf, NULL);
    }
    else {
        return BTP_STATUS_FAILED;
    }

	/* reigster A2DP SINK SDP records */
	bt_sdp_register_service(&a2dp_sink_rec);

	/* create AudioTask */
    BaseType_t result = 0;
    result = xTaskCreate(AudioTask, "audio", 2048U, NULL, APP_A2DP_STREAMER_SYNC_TASK_PRIORITY, NULL);
    assert(pdPASS == result);
    (void)result;
#endif /* CONFIG_BT_A2DP_SINK > 0 */

#if defined(CONFIG_BT_A2DP_SOURCE) && (CONFIG_BT_A2DP_SOURCE > 0)
	/* reigster A2DP SOURCE SDP records */
	bt_sdp_register_service(&a2dp_source_rec);
#endif /* CONFIG_BT_A2DP_SOURCE > 0 */

	/* register a2dp connct/disconnect callback */
    struct bt_a2dp_connect_cb connectCb;
    connectCb.connected = a2dp_connected;
    connectCb.disconnected = a2dp_disconnected;
    bt_a2dp_register_connect_callback(&connectCb);

	tester_register_command_handlers(BTP_SERVICE_ID_A2DP, handlers,
					 ARRAY_SIZE(handlers));

	return BTP_STATUS_SUCCESS;
}

uint8_t tester_unregister_a2dp(void)
{
	return BTP_STATUS_SUCCESS;
}
#endif /* CONFIG_BT_A2DP > 0 */