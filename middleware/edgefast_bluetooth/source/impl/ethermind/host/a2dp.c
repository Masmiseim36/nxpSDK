/*
 * Copyright 2020 - 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <porting.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/atomic.h>
#include <sys/util.h>
#include <sys/slist.h>
#include <sys/byteorder.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/a2dp.h>
#include <bluetooth/a2dp-codec.h>
#include "a2dp_internal.h"
#include "fsl_os_abstraction.h"
#include "BT_features.h"
#include "BT_common.h"
#include "BT_hci_api.h"
#include "BT_sdp_api.h"
#include "BT_avdtp_api.h"
#include "BT_a2dp_api.h"
#include "BT_config.h"
#include "BT_jpl_api.h"
#include "BT_fops.h"

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_A2DP)
#define LOG_MODULE_NAME bt_a2dp
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#define SBC_BLOCK_MAX               (512U)
#define BT_A2DP_CODEC_IE_LENGTH_MAX (128U)
#define A2DP_SRC_MAX_BUFFER_SIZE    (4096U)
#define A2DP_MAX_ENDPOINT_COUNT     (10U)
#define APP_A2DP_SRC_WR_TH_INIT     (0x01)
#define APP_A2DP_SRC_WR_TH_STOP     (0x02)
#define APP_A2DP_SRC_WR_TH_PLAYING  (0x03)
#define A2DP_TRANSFER_RETRY_COUNT   (5U)
#define A2DP_MSG_Q_COUNT            (8U)

enum bt_a2dp_internal_state
{
    INTERNAL_STATE_IDLE = 0,
    INTERNAL_STATE_AVDTP_CONNECTED,
    INTERNAL_STATE_DISCONNECT_CONNECT_AGAIN,
    INTERNAL_STATE_CONFIGURED_OPENED,
};

struct bt_a2dp_endpoint_internal
{
    struct bt_a2dp_endpoint *endpoint;
    uint8_t ethermind_a2dp_codec_index;
    struct bt_a2dp_preset *config_result;
};

struct bt_a2dp_preset_internal {
	/** Length of preset */
	uint8_t len;
	/** Preset */
	uint8_t preset[BT_A2DP_CODEC_IE_LENGTH_MAX];
};

/* A2DP Codec Instances */
struct bt_a2dp_internal
{
    AVDTP_HANDLE ethermind_avdtp_handle;
    AVDTP_SEP_CONF sep_conf;
    SBC_ENCODER_IO_STRUCT a2dp_sbc_encoder_io;
    struct bt_avdtp_seid_info current_seid_info;
    int32_t a2dp_src_buffer_rd_ptr;
    int32_t a2dp_src_buffer_wr_ptr;
    int32_t a2dp_src_buffer_size;
    /* Pointer to start of Discover Response Info Element */
    uint8_t * sep_inf_buf_start;
    JPL_PARAM jpl_param;
    uint8_t *a2dp_jpl_pof;
    uint16_t a2dp_jpl_poflen;
    uint16_t a2dp_pcm_datalen;
    /* Buffer for converting UCHAR pcm stream to UINT16 pcm samples */
    uint16_t a2dp_pcm_data_le [SBC_BLOCK_MAX >> 1];
    /* Actual response buffer length */
    uint16_t sep_inf_buflen;
    uint8_t a2dp_src_wr_th_state;
    uint8_t a2dp_src_buffer[A2DP_SRC_MAX_BUFFER_SIZE];
    /* Buffer used to hold encoded SBC data */
    uint8_t a2dp_sbc_data[SBC_BLOCK_MAX];
    uint8_t pcm_to_send[SBC_BLOCK_MAX];
    /* Buffer for AVDTP Discover Response */
    uint8_t sep_inf_buf[A2DP_MAX_DISCOVER_RESPONSE_LEN];
    /* Buffer for AVDTP Get Capability Response */
    uint8_t sep_cap_buf[A2DP_GET_CAPABILITIES_RESPONSE_LEN];
    uint8_t peer_addr[BT_BD_ADDR_SIZE];
    uint8_t retry_count;
    enum bt_a2dp_internal_state a2dp_state;
    uint8_t retry_command;
    uint8_t msg_command;
    uint8_t configured_from_peer : 1;
    uint8_t discover_done : 1;
    uint8_t connected_from_peer : 1;
    uint8_t auto_configure_enabled : 1;
};

struct bt_a2dp
{
    struct bt_delayed_work retry_work;
    uint8_t peer_endpoint_idle_seid[A2DP_MAX_ENDPOINT_COUNT];
    bt_a2dp_discover_peer_endpoint_cb_t discover_peer_endpoint_cb;
    bt_a2dp_deconfigure_cb_t de_configure_cb;
    bt_a2dp_configure_cb_t re_configure_cb;
    bt_a2dp_configure_cb_t configure_cb;
    bt_a2dp_configure_cb_t configure_ep_cb;
    struct bt_a2dp_control_cb control_cb;
    bt_a2dp_start_cb_t source_start_play;
    bt_a2dp_suspend_cb_t source_suspend_play;
    struct bt_a2dp_internal internal_a2dp;
    /* it is one of a2dp_endpoint_internals[A2DP_MAX_ENDPOINT_COUNT] */
    struct bt_a2dp_endpoint_internal *internal_selected_endpoint;
    struct bt_a2dp_endpoint select_peer_endpoint;
    struct bt_a2dp_preset_internal select_peer_endpoint_internal_cap;
    /* save the config result's content */
    struct bt_a2dp_preset_internal a2dp_config_internal;
    /* Flag - Whether allocated or free */
    uint8_t allocated;
};

enum a2dp_control_callback_event
{
    A2DP_CONTROL_CONFIGURED,
    A2DP_CONTROL_START_PLAY,
    A2DP_CONTROL_SUSPEND_PLAY,
    A2DP_CONTROL_SINK_STREAMER_DATA,
};

enum a2dp_configure_callback_event
{
    A2DP_CONFIG_DONE,
};

enum a2dp_connect_callback_event
{
    A2DP_CB_CONNECTED,
    A2DP_CB_DISCONNECTED,
};

typedef struct _a2dp_streamer_data
{
    uint8_t *data;
    uint32_t data_length;
} a2dp_streamer_data_t;

static struct bt_a2dp_endpoint_internal a2dp_endpoint_internals[A2DP_MAX_ENDPOINT_COUNT];

static struct bt_a2dp_connect_cb connect_cb;

AT_NONCACHEABLE_SECTION_INIT(static uint8_t a2dp_sbc_buffer[JPL_SBC_FRAME_SIZE * JPL_SBC_NUM_FRAMES]);
AT_NONCACHEABLE_SECTION_INIT(static uint8_t a2dp_pcm_buffer[JPL_PCM_BLOCK_SIZE * JPL_PCM_NUM_BLOCKS]);
AT_NONCACHEABLE_SECTION_INIT(static uint8_t a2dp_silence_buffer[JPL_PCM_BLOCK_SIZE]);

static OSA_TASK_HANDLE_DEFINE(a2dp_process_task_def);
static osa_task_handle_t a2dp_process_task_handle;
static void a2dp_process_task(osa_task_param_t arg);
static OSA_TASK_DEFINE(a2dp_process_task, CONFIG_BT_A2DP_TASK_PRIORITY, 1, CONFIG_BT_A2DP_TASK_STACK_SIZE, 0);

static OSA_MUTEX_HANDLE_DEFINE(a2dp_lock_mutex);
static osa_mutex_handle_t a2dp_lock;
static struct bt_a2dp a2dp_instances[CONFIG_BT_MAX_CONN];
static osa_msgq_handle_t a2dp_process_queue;
static OSA_MSGQ_HANDLE_DEFINE(a2dp_process_queue_handle, A2DP_MSG_Q_COUNT, sizeof(a2dp_task_msg_t));
static struct bt_a2dp *jpl_active_a2dp;

#define EDGEFAST_A2DP_LOCK OSA_MutexLock(a2dp_lock, osaWaitForever_c)
#define EDGEFAST_A2DP_UNLOCK OSA_MutexUnlock(a2dp_lock)

typedef struct _a2dp_task_msg_
{
    struct bt_a2dp *a2dp;
    uint8_t event;
} a2dp_task_msg_t;

typedef enum _a2dp_task_event_
{
    A2DP_EVENT_INVALID = 0,
    A2DP_EVENT_RETRY_DISCOVER,
    A2DP_EVENT_RETRY_GET_CAPS,
    A2DP_EVENT_RETRY_A2DP_CONNECT,
    A2DP_EVENT_SEND_SBC_PCM_DATA,
} a2dp_task_event_t;

static struct bt_a2dp * a2dp_GetInstance(void)
{
    EDGEFAST_A2DP_LOCK;
    for (uint8_t index = 0; index < CONFIG_BT_MAX_CONN; ++index)
    {
        if (a2dp_instances[index].allocated == 0U)
        {
            memset(&a2dp_instances[index].peer_endpoint_idle_seid[0], 0U, sizeof (a2dp_instances[index]) - sizeof (struct bt_delayed_work));
            a2dp_instances[index].allocated = 1U;
            EDGEFAST_A2DP_UNLOCK;
            return &a2dp_instances[index];
        }
    }

    EDGEFAST_A2DP_UNLOCK;
    return NULL;
}

static void a2dp_FreeInstance(struct bt_a2dp *a2dp)
{
    if (NULL != a2dp)
    {
        EDGEFAST_A2DP_LOCK;
        a2dp->allocated = 0U;
        EDGEFAST_A2DP_UNLOCK;
    }
}

static uint8_t a2dp_get_endpoints_count(void)
{
    uint8_t index;

    for (index = 0; index < A2DP_MAX_ENDPOINT_COUNT; ++index)
    {
        if (a2dp_endpoint_internals[index].endpoint == NULL)
        {
            break;
        }
    }
    return index;
}

static uint8_t bt_a2dp_get_ethermind_a2dp_index(struct bt_a2dp *a2dp)
{
    if (a2dp->internal_selected_endpoint == NULL)
    {
        return 0U;
    }

    return a2dp->internal_selected_endpoint->ethermind_a2dp_codec_index;
}

static uint8_t bt_a2dp_get_ethermind_a2dp_sep_type(struct bt_a2dp *a2dp)
{
    if (a2dp->internal_selected_endpoint == NULL)
    {
        return 0U;
    }

    return a2dp->internal_selected_endpoint->endpoint->info.sep.tsep;
}

static uint8_t bt_a2dp_get_ethermind_a2dp_codec_type(struct bt_a2dp *a2dp)
{
    if (a2dp->internal_selected_endpoint == NULL)
    {
        return 0U;
    }

    return a2dp->internal_selected_endpoint->endpoint->codec_id;
}

static void a2dp_control_callback_call(struct bt_a2dp *a2dp, uint8_t event, int result, void *parameter)
{
    if (a2dp != NULL)
    {
        switch (event)
        {
            case A2DP_CONTROL_CONFIGURED:
                if (a2dp->control_cb.configured != NULL)
                {
                    a2dp->control_cb.configured(a2dp, (struct a2dp_configure_result *)parameter);
                }
                break;

            case A2DP_CONTROL_START_PLAY:
                if (BT_A2DP_SOURCE == bt_a2dp_get_ethermind_a2dp_sep_type(a2dp))
                {
                    if (a2dp->source_start_play != NULL)
                    {
                        a2dp->source_start_play(a2dp, result);
                    }
                }
                else
                {
                    if (a2dp->control_cb.sink_start_play != NULL)
                    {
                        a2dp->control_cb.sink_start_play(a2dp, result);
                    }
                }
                break;
            case A2DP_CONTROL_SUSPEND_PLAY:
                if (BT_A2DP_SOURCE == bt_a2dp_get_ethermind_a2dp_sep_type(a2dp))
                {
                    if (a2dp->source_suspend_play != NULL)
                    {
                        a2dp->source_suspend_play(a2dp, result);
                    }
                }
                else
                {
                    if (a2dp->control_cb.sink_suspend_play != NULL)
                    {
                        a2dp->control_cb.sink_suspend_play(a2dp, result);
                    }
                }
                break;

            case A2DP_CONTROL_SINK_STREAMER_DATA:
                if (a2dp->control_cb.sink_streamer_data != NULL)
                {
                    a2dp->control_cb.sink_streamer_data(a2dp, ((a2dp_streamer_data_t *)parameter)->data, ((a2dp_streamer_data_t *)parameter)->data_length);
                }
                break;

            default:
                /* MISRA 16.4 : The switch statement does not have a non-empty default clause. */
                break;
        }
    }
}

static void a2dp_connectcallback_call(struct bt_a2dp *a2dp, uint8_t event, int result, void *parameter)
{
    if (event == A2DP_CB_CONNECTED)
    {
        if (connect_cb.connected != NULL)
        {
            connect_cb.connected(a2dp, result);
        }
    }
    else if (event == A2DP_CB_DISCONNECTED)
    {
        if (connect_cb.disconnected != NULL)
        {
            connect_cb.disconnected(a2dp);
        }
    }
    else
    {
    }
}

static void a2dp_configure_callback_call(struct bt_a2dp *a2dp, uint8_t autoConfig, int result, void *parameter)
{
    if (a2dp == NULL)
    {
        return;
    }

    if (autoConfig)
    {
        a2dp->internal_a2dp.auto_configure_enabled = 0U;
        if (a2dp->configure_cb != NULL)
        {
            if (a2dp->configure_cb != NULL)
            {
                if (result == 0)
                {
                    a2dp->configure_cb(a2dp, (struct a2dp_configure_result *)parameter);
                }
                else
                {
                    struct a2dp_configure_result parameter;
                    parameter.err = -1;
                    parameter.config = NULL;
                    parameter.endpoint = NULL;

                    a2dp->configure_cb(a2dp, &parameter);
                }
            }
        }
    }
    else
    {
        if (a2dp->configure_ep_cb != NULL)
        {
            if (a2dp->configure_ep_cb != NULL)
            {
                struct a2dp_configure_result configureResult;
                if (result == 0)
                {
                    configureResult.err = 0;
                    configureResult.config = a2dp->internal_selected_endpoint->config_result;
                    configureResult.endpoint = a2dp->internal_selected_endpoint->endpoint;
                    a2dp->configure_ep_cb(a2dp, &configureResult);
                }
                else
                {
                    configureResult.err = -1;
                    configureResult.config = NULL;
                    configureResult.endpoint = NULL;
                    a2dp->configure_ep_cb(a2dp, &configureResult);
                }
            }
        }
    }
}

static int a2dp_set_a2dp_source_codec_encoder(struct bt_a2dp *a2dp, uint8_t codecType, uint8_t *codec_ie)
{
    API_RESULT retval;
    /* Get SBC Encoder Codec Parameters */
    SBC_CODEC_PARAM * codec_param = &a2dp->internal_a2dp.a2dp_sbc_encoder_io.encoder_params;

    if (codecType != AVDTP_CODEC_AUDIO_SBC)
    {
        return 0;
    }

    /* Decode Sampling Frequency */
    switch (codec_ie[0] & 0xF0)
    {
    case A2DP_SBC_SAMPLING_FREQUENCY_16000:
        codec_param->sampling_frequency = 0;
        break;

    case A2DP_SBC_SAMPLING_FREQUENCY_32000:
        codec_param->sampling_frequency = 1;
        break;

    case A2DP_SBC_SAMPLING_FREQUENCY_44100:
        codec_param->sampling_frequency = 2;
        break;

    case A2DP_SBC_SAMPLING_FREQUENCY_48000:
        codec_param->sampling_frequency = 3;
        break;

    default:
        BT_DBG("*** SBC Config: Sampling Frequency = Unknown (0x%X)!!\r\n", (codec_ie[0] & 0xF0));
        return -EIO;
    }

    /* Decode Channel Mode */
    switch (codec_ie[0] & 0x0F)
    {
    case A2DP_SBC_CHANNEL_MONO:
        codec_param->channel_mode = SBC_CHANNEL_MONO;
        codec_param->nrof_channels = 1;
        break;

    case A2DP_SBC_CHANNEL_DUAL:
        codec_param->channel_mode = SBC_CHANNEL_DUAL;
        codec_param->nrof_channels = 2;
        break;

    case A2DP_SBC_CHANNEL_STEREO:
        codec_param->channel_mode = SBC_CHANNEL_STEREO;
        codec_param->nrof_channels = 2;
        break;

    case A2DP_SBC_CHANNEL_JOINT_STEREO:
        codec_param->channel_mode = SBC_CHANNEL_JOINT_STEREO;
        codec_param->nrof_channels = 2;
        break;

    default:
        BT_DBG("*** SBC Config: Channel Mode = Unknown (0x%X)!!\r\n",
        (codec_ie[0] & 0x0F));
        return -EIO;
    }

    /* Decode Block Length */
    switch (codec_ie[1] & 0xF0)
    {
    case A2DP_SBC_BLOCK_LENGTH_4:
        codec_param->nrof_blocks = SBC_BLOCK_SIZE_4;
        break;

    case A2DP_SBC_BLOCK_LENGTH_8:
        codec_param->nrof_blocks = SBC_BLOCK_SIZE_8;
        break;

    case A2DP_SBC_BLOCK_LENGTH_12:
        codec_param->nrof_blocks = SBC_BLOCK_SIZE_12;
        break;

    case A2DP_SBC_BLOCK_LENGTH_16:
        codec_param->nrof_blocks = SBC_BLOCK_SIZE_16;
        break;

    default:
        BT_DBG("*** SBC Config: Block Length = Unknown (0x%X)!!\r\n",
        (codec_ie[1] & 0xF0));
        return -EIO;
    }

    /* Decode Subbands */
    switch (codec_ie[1] & 0x0C)
    {
    case A2DP_SBC_SUBBANDS_4:
        codec_param->nrof_subbands = SBC_SUBBAND_4;
        break;

    case A2DP_SBC_SUBBANDS_8:
        codec_param->nrof_subbands = SBC_SUBBAND_8;
        break;

    default:
        BT_DBG("*** SBC Config: Subbands = Unknown (0x%X)!!\r\n",
        (codec_ie[1] & 0x0C));
        return -EIO;
    }

    /* Decode Allocation Method */
    switch (codec_ie[1] & 0x03)
    {
    case A2DP_SBC_ALLOCATION_METHOD_SNR:
        codec_param->allocation_method = SBC_SNR;
        break;

    case A2DP_SBC_ALLOCATION_METHOD_LOUDNESS:
        codec_param->allocation_method = SBC_LOUDNESS;
        break;

    default:
        BT_DBG("*** SBC Config: Allocation Method = Unknown (0x%X)!!\r\n",
        (codec_ie[1] & 0x03));
        return -EIO;
    }

    /* Set Bitpool */
    codec_param->bitpool = codec_ie[3];

    a2dp->internal_a2dp.a2dp_pcm_datalen = 2 *
                  codec_param->nrof_subbands *
                  codec_param->nrof_blocks *
                  codec_param->nrof_channels;
    /* Initialize SBC Decoder */
    retval = sbc_encoder_init (&a2dp->internal_a2dp.a2dp_sbc_encoder_io);
    if (API_SUCCESS != retval)
    {
        BT_DBG("*** Failed to Initialize SBC Encoder: 0x%X\r\n", retval);
        return -EIO;
    }
    return 0;
}

static void a2dp_retry_work_timeout(struct bt_work *work)
{
    struct bt_a2dp *a2dp = CONTAINER_OF(work, struct bt_a2dp, retry_work);
    a2dp_task_msg_t message;

    EDGEFAST_A2DP_LOCK;
    message.a2dp = a2dp;
    message.event = a2dp->internal_a2dp.retry_command;
    a2dp->internal_a2dp.msg_command = a2dp->internal_a2dp.retry_command;
    EDGEFAST_A2DP_UNLOCK;
    OSA_MsgQPut(a2dp_process_queue, &message);
}

static void a2dp_set_delay_work(struct bt_a2dp *a2dp, uint8_t event, uint32_t delayMs)
{
    a2dp->internal_a2dp.retry_command = event;
    if (delayMs == 0)
    {
        a2dp_retry_work_timeout((struct bt_work*)&a2dp->retry_work);
    }
    else
    {
        bt_delayed_work_submit(&a2dp->retry_work, BT_MSEC(delayMs));
    }
}

static void a2dp_callback_auto_configure(struct bt_a2dp *a2dp)
{
    uint8_t index;
    uint8_t count;
    API_RESULT retval;

    if (a2dp->internal_a2dp.a2dp_state != INTERNAL_STATE_AVDTP_CONNECTED)
    {
        return;
    }

    count = a2dp_get_endpoints_count();
    /* check all the endpoint capabilities, and select the highest priority endpoint */
    for (index = 0U; index < count; ++index)
    {
        if (a2dp->peer_endpoint_idle_seid[index])
        {
            break;
        }
    }

    if (index < count)
    {
        struct bt_a2dp_preset *config;
        struct bt_a2dp_endpoint *self_endpoint = a2dp_endpoint_internals[index].endpoint;

        if (a2dp_endpoint_internals[index].endpoint->preset == NULL)
        {
            a2dp_configure_callback_call(a2dp, 1U, -EIO, NULL);
            return;
        }
        else
        {
            config = self_endpoint->preset;
        }

        a2dp->a2dp_config_internal.len = config->len;
        memcpy
        (
            &a2dp->a2dp_config_internal.preset[0],
            &config->preset[0],
            config->len
        );

        /* load the BD addr */
        BT_COPY_BD_ADDR(a2dp->internal_a2dp.peer_addr, a2dp->internal_a2dp.ethermind_avdtp_handle.bd_addr);
        /* Initialize SEP Configuration Parameters */
        AVDTP_INIT_SEP_CONFIGURATION (a2dp->internal_a2dp.sep_conf);
        /* Enable Media Transport Service in SEP Configuration */
        AVDTP_SET_SEP_CONFIGURATION_MEDIA (a2dp->internal_a2dp.sep_conf);
        /* Set AVDTP Handle Remote SEID */
        AVDTP_SET_HANDLE_REMOTE_SEID (a2dp->internal_a2dp.ethermind_avdtp_handle, a2dp->peer_endpoint_idle_seid[index]);
        /* Enable Codec Capabilities in SEP Configuration */
        AVDTP_SET_SEP_CONFIGURATION_CODEC_CAP
        (
            a2dp->internal_a2dp.sep_conf,
            BT_A2DP_AUDIO,
            self_endpoint->codec_id,
            config->len,
            config->preset
        )

        a2dp->internal_a2dp.discover_done = 1U;
        /* A2DP Connect */
        if (a2dp->internal_a2dp.connected_from_peer)
        {
            BT_DBG("A2DP connect wait ...\r\n");
            a2dp->internal_selected_endpoint = &a2dp_endpoint_internals[index];
            /* the peer may establish the a2dp connection firstly */
            a2dp->internal_a2dp.retry_count = A2DP_TRANSFER_RETRY_COUNT;
            a2dp_set_delay_work(a2dp, A2DP_EVENT_RETRY_A2DP_CONNECT, 500U);
        }
        else
        {
            BT_DBG("A2DP connect ...\r\n");
            a2dp->internal_selected_endpoint = &a2dp_endpoint_internals[index];
            a2dp->internal_a2dp.retry_count = A2DP_TRANSFER_RETRY_COUNT;
            retval = BT_a2dp_connect
                    (
                        bt_a2dp_get_ethermind_a2dp_index(a2dp),
                        a2dp->internal_a2dp.peer_addr,
                        a2dp->internal_a2dp.ethermind_avdtp_handle.remote_seid,
                        &a2dp->internal_a2dp.sep_conf
                    );
            if (API_SUCCESS != retval)
            {
                BT_DBG("BT_a2dp_connect fail 0x%X\r\n", retval);
            }
            else
            {
                BT_DBG("BT_a2dp_connect success, wait for A2DP_CONNECT_CNF\r\n");
                //todo: only support sbc codec now.
                a2dp_set_a2dp_source_codec_encoder(a2dp, self_endpoint->codec_id, config->preset);
            }
        }
    }
    else
    {
        a2dp_configure_callback_call(a2dp, 1U, -EIO, NULL);
    }
}

static void a2dp_jpl_configure(struct bt_a2dp *a2dp, uint8_t* codec_ie)
{
    SBC_CODEC_PARAM * codec_param;

    codec_param = &a2dp->internal_a2dp.jpl_param.sbc_param;

    /* Get the sampling Frequency */
    switch (codec_ie[0] & 0xF0)
    {
    case A2DP_SBC_SAMPLING_FREQUENCY_16000:
        codec_param->sampling_frequency = 0;
        break;

    case A2DP_SBC_SAMPLING_FREQUENCY_32000:
        codec_param->sampling_frequency = 1;
        break;

    case A2DP_SBC_SAMPLING_FREQUENCY_44100:
        codec_param->sampling_frequency = 2;
        break;

    case A2DP_SBC_SAMPLING_FREQUENCY_48000:
        codec_param->sampling_frequency = 3;
        break;

    default:
        /* This should not be reached */
        BT_DBG("Invalid sampling frequency\r\n");
        return;
    }

    /* Get the Channel Mode */
    switch (codec_ie[0] & 0x0F)
    {
    case A2DP_SBC_CHANNEL_MONO:
        codec_param->channel_mode = SBC_CHANNEL_MONO;
        codec_param->nrof_channels = 1;
        break;

    case A2DP_SBC_CHANNEL_DUAL:
        codec_param->channel_mode = SBC_CHANNEL_DUAL;
        codec_param->nrof_channels = 2;
        break;

    case A2DP_SBC_CHANNEL_STEREO:
        codec_param->channel_mode = SBC_CHANNEL_STEREO;
        codec_param->nrof_channels = 2;
        break;

    case A2DP_SBC_CHANNEL_JOINT_STEREO:
        codec_param->channel_mode = SBC_CHANNEL_JOINT_STEREO;
        codec_param->nrof_channels = 2;
        break;

    default:
        /* This should not be reached */
        BT_DBG("Invalid Channel mode\r\n");
        return;
    }

    /* Get the Block Length */
    switch (codec_ie[1] & 0xF0)
    {
    case A2DP_SBC_BLOCK_LENGTH_4:
        codec_param->nrof_blocks = SBC_BLOCK_SIZE_4;
        break;

    case A2DP_SBC_BLOCK_LENGTH_8:
        codec_param->nrof_blocks = SBC_BLOCK_SIZE_8;
        break;

    case A2DP_SBC_BLOCK_LENGTH_12:
        codec_param->nrof_blocks = SBC_BLOCK_SIZE_12;
        break;

    case A2DP_SBC_BLOCK_LENGTH_16:
        codec_param->nrof_blocks = SBC_BLOCK_SIZE_16;
        break;

    default:
        /* This should not be reached */
        BT_DBG("Invalid Block Length\r\n");
        return;
    }

    /* Get the Subbands */
    switch (codec_ie[1] & 0x0C)
    {
    case A2DP_SBC_SUBBANDS_4:
        codec_param->nrof_subbands = SBC_SUBBAND_4;
        break;

    case A2DP_SBC_SUBBANDS_8:
        codec_param->nrof_subbands = SBC_SUBBAND_8;
        break;

    default:
        /* This should not be reached */
        BT_DBG("Invalid Subband\r\n");
        return;
    }

    /* Get the Allocation Method */
    switch (codec_ie[1] & 0x03)
    {
    case A2DP_SBC_ALLOCATION_METHOD_SNR:
        codec_param->allocation_method = SBC_SNR;
        break;

    case A2DP_SBC_ALLOCATION_METHOD_LOUDNESS:
        codec_param->allocation_method = SBC_LOUDNESS;
        break;

    default:
        /* This should not be reached */
        BT_DBG("Invalid Allocation method\r\n");
        return;
    }

    /* Set Bitpool */
    codec_param->bitpool = codec_ie[3];

    a2dp->internal_a2dp.jpl_param.pcm_num_frames =
        ((3 * (SBC_MAX_BLOCK_SIZE * SBC_MAX_SUBBAND)) /
        (codec_param->nrof_blocks * codec_param->nrof_subbands));

    a2dp->internal_a2dp.jpl_param.pod_frames =
        ((18 * (SBC_MAX_BLOCK_SIZE * SBC_MAX_SUBBAND)) /
        (codec_param->nrof_blocks * codec_param->nrof_subbands));

    BT_DBG("A2DP Sink Configured\r\n");

    return;
}

static API_RESULT jpl_callback_handle
       (
           /* IN */ UCHAR   event_type,
           /* IN */ UCHAR * event_data,
           /* IN */ UINT16  event_datalen
       )
{
    if ((JPL_UNDERFLOW_IND == event_type) ||
        (JPL_SILENCE_DATA_IND == event_type))
    {
        BT_DBG("U ");
    }

    if (jpl_active_a2dp != NULL)
    {
        a2dp_streamer_data_t streamerData;
        streamerData.data = jpl_active_a2dp->internal_a2dp.a2dp_jpl_pof = event_data;
        streamerData.data_length = jpl_active_a2dp->internal_a2dp.a2dp_jpl_poflen = event_datalen;

        a2dp_control_callback_call(jpl_active_a2dp, A2DP_CONTROL_SINK_STREAMER_DATA, 0, &streamerData);
    }

    return API_SUCCESS;
}

static int a2dp_handle_avdtp_discover_rsp (struct bt_a2dp *a2dp)
{
    API_RESULT retval;
    AVDTP_SEP_INFO sep_info;
    uint8_t sepIndex;

    for (sepIndex = 0U; sepIndex < a2dp->internal_a2dp.sep_inf_buflen; sepIndex += 2U)
    {
        retval = BT_avdtp_decode_sep_information
                 (
                     a2dp->internal_a2dp.sep_inf_buf_start,
                     (UINT16) a2dp->internal_a2dp.sep_inf_buflen,
                     &sep_info
                 );

        /* Set response buffer to next SEP info index */
        a2dp->internal_a2dp.sep_inf_buf_start += 2;
        a2dp->internal_a2dp.sep_inf_buflen -= 2;

        if (API_SUCCESS != retval)
        {
            BT_DBG("*** FAILED to Decode SEP Information (0x%X)\n", retval);
        }
        else
        {
#if (defined(CONFIG_BT_DEBUG_A2DP) && (CONFIG_BT_DEBUG_A2DP))
            BT_DBG("Remote SEP Information [%d]:\r\n", sepIndex);

            /* SEP In Use */
            BT_DBG("\t    In Use     = %s\r\n",
            ((0x1 == sep_info.in_use) ? "Yes" : "No"));

            /* SEP Type */
            BT_DBG("\t    SEP Type   = 0x%X -> ", sep_info.sep_type);
            switch (sep_info.sep_type)
            {
            case AVDTP_SEP_SOURCE:
                BT_DBG("Source\r\n");
                break;

            case AVDTP_SEP_SINK:
                BT_DBG("Sink\r\n");
                break;

            default:
                BT_DBG("????\r\n");
            }

            /* ACP SEID */
            BT_DBG("\t    ACP SEID   = 0x%X\r\n", sep_info.acp_seid);

            /* Media Type */
            BT_DBG("\t    Media Type = 0x%X -> ", sep_info.media_type);
            switch (sep_info.media_type)
            {
            case AVDTP_MEDIA_AUDIO:
                BT_DBG("Audio\r\n");
                break;

            case AVDTP_MEDIA_VIDEO:
                BT_DBG("Video\r\n");
                break;

            case AVDTP_MEDIA_MULTIMEDIA:
                BT_DBG("Multi\r\n");
                break;

            default:
                BT_DBG("????\r\n");
            }
#endif

            if ((AVDTP_MEDIA_AUDIO == sep_info.media_type) && (0x00 == sep_info.in_use))
            {
                /* Set AVDTP Handle Remote SEID */
                AVDTP_SET_HANDLE_REMOTE_SEID (a2dp->internal_a2dp.ethermind_avdtp_handle, sep_info.acp_seid);

                /* AVDTP Get Capabilities */
                BT_DBG("AVDTP Get Capabilities ... ");
                a2dp->internal_a2dp.current_seid_info.id = sep_info.acp_seid;
                a2dp->internal_a2dp.current_seid_info.inuse = sep_info.in_use;
                a2dp->internal_a2dp.current_seid_info.media_type = sep_info.media_type;
                a2dp->internal_a2dp.current_seid_info.tsep = sep_info.sep_type;

                a2dp->internal_a2dp.retry_count = A2DP_TRANSFER_RETRY_COUNT;
                retval = BT_avdtp_get_capabilities_req
                         (
                             &a2dp->internal_a2dp.ethermind_avdtp_handle,
                             a2dp->internal_a2dp.sep_cap_buf,
                             A2DP_GET_CAPABILITIES_RESPONSE_LEN
                         );

                if (API_SUCCESS != retval)
                {
                    BT_DBG("BT_avdtp_get_capabilities_req fail 0x%X\r\n", retval);
                    a2dp_set_delay_work(a2dp, A2DP_EVENT_RETRY_GET_CAPS, 500U);
                }
                else
                {
                    BT_DBG("BT_avdtp_get_capabilities_req success, wait for AVDTP_GET_CAPABILITIES_CNF\r\n");
                }

                return 0;
            }
        }
    }
    return -EIO;
}

static API_RESULT a2dp_encode_n_send
           (
               struct bt_a2dp *a2dp,
               uint8_t    *pcm_data,
               uint16_t   pcm_datalen
           )
{
    API_RESULT        retval;
    uint16_t            count;
    uint16_t            sbc_datalen;

    /* Convert UCHAR LE16 PCM Data to uint16_t LE16 */
    for (count = 0; count < (pcm_datalen >> 1); count ++)
    {
        a2dp->internal_a2dp.a2dp_pcm_data_le[count] =
            (uint16_t) ((pcm_data[2 * count + 1] << 8) | (pcm_data[2 * count]));
    }

    /* Initialize amount of buffer passed for SBC stream */
    sbc_datalen = a2dp->internal_a2dp.a2dp_pcm_datalen;

    /* Prepare for SBC Encoder */
    a2dp->internal_a2dp.a2dp_sbc_encoder_io.encoder_input_buflen = count;
    a2dp->internal_a2dp.a2dp_sbc_encoder_io.encoder_input_buffer = a2dp->internal_a2dp.a2dp_pcm_data_le;

    a2dp->internal_a2dp.a2dp_sbc_encoder_io.encoder_output_buffer = a2dp->internal_a2dp.a2dp_sbc_data;
    a2dp->internal_a2dp.a2dp_sbc_encoder_io.encoder_output_buflen = sbc_datalen;

    /* Do SBC Encoding */
    retval = sbc_encoder
             (
                 &a2dp->internal_a2dp.a2dp_sbc_encoder_io
             );

    if (API_SUCCESS != retval)
    {
        BT_DBG("SBC Encoder failed - 0x%X\r\n", retval);
        return retval;
    }

    /* Update SBC Frame Size */
    sbc_datalen = a2dp->internal_a2dp.a2dp_sbc_encoder_io.encoder_output_buflen;

    /* Write to A2DP */
    retval = BT_a2dp_media_write
             (
                 bt_a2dp_get_ethermind_a2dp_index(a2dp),
                 a2dp->internal_a2dp.a2dp_sbc_data,
                 sbc_datalen
             );
    if (API_SUCCESS != retval)
    {
        BT_DBG("A2DP media write failed - 0x%X\r\n", retval);
    }

    return retval;
}

static void edgefast_a2dp_src_write_task (struct bt_a2dp *a2dp)
{
    int32_t  rd_ptr, index, remaining;
    uint16_t bytes_to_send, buf_index, encode_len;

    for(;;)
    {
        if (APP_A2DP_SRC_WR_TH_INIT == a2dp->internal_a2dp.a2dp_src_wr_th_state)
        {
            /* Wait for data in buffer */
            return;
            #if 0
            BT_thread_mutex_lock (&a2dp_src_th_mutex);
            BT_thread_cond_wait (&a2dp_src_th_cond, &a2dp_src_th_mutex);
            BT_thread_mutex_unlock (&a2dp_src_th_mutex);
            #endif
        }
        else if (APP_A2DP_SRC_WR_TH_STOP == a2dp->internal_a2dp.a2dp_src_wr_th_state)
        {
            /* Exit from task */
            return;
        }

        //BT_thread_mutex_lock (&a2dp_src_th_mutex);
        EDGEFAST_A2DP_LOCK;
        if (a2dp->internal_a2dp.a2dp_src_buffer_wr_ptr >= a2dp->internal_a2dp.a2dp_src_buffer_rd_ptr)
        {
            /*
             *    ----------DDDDDDDDDDDDDDD--------------X
             *    ^         ^              ^             ^
             *    |         |              |             |
             *    0       rd_ptr         wr_ptr         max
             *
             *  Free Space => '-----------'
             */
            remaining = a2dp->internal_a2dp.a2dp_src_buffer_wr_ptr - a2dp->internal_a2dp.a2dp_src_buffer_rd_ptr;
        }
        else
        {
            /*
             *    DDDDDDDDDD---------------DDDDDDDDDDDDDDX
             *    ^         ^              ^             ^
             *    |         |              |             |
             *    0       wr_ptr         rd_ptr         max
             *
             *  Free Space => '-----------'
             */
            remaining = a2dp->internal_a2dp.a2dp_src_buffer_size -
                    (a2dp->internal_a2dp.a2dp_src_buffer_rd_ptr - a2dp->internal_a2dp.a2dp_src_buffer_wr_ptr);
        }

        /* Do we really have anything to read ? */
        if (remaining < a2dp->internal_a2dp.a2dp_pcm_datalen)
        {
            /* Wait for data in buffer */
            a2dp->internal_a2dp.a2dp_src_wr_th_state = APP_A2DP_SRC_WR_TH_INIT;
            //BT_thread_cond_wait (&a2dp_src_th_cond, &a2dp_src_th_mutex);
            //T_thread_mutex_unlock (&a2dp_src_th_mutex);
            EDGEFAST_A2DP_UNLOCK;
            continue;
        }
        else
        {
            //BT_thread_mutex_unlock (&a2dp_src_th_mutex);
            EDGEFAST_A2DP_UNLOCK;
        }

        rd_ptr = a2dp->internal_a2dp.a2dp_src_buffer_rd_ptr;

        bytes_to_send = (remaining > a2dp->internal_a2dp.a2dp_pcm_datalen) ?
            a2dp->internal_a2dp.a2dp_pcm_datalen : (uint16_t)remaining;

        for (index = 0; index < bytes_to_send; index++)
        {
            a2dp->internal_a2dp.pcm_to_send[index] = a2dp->internal_a2dp.a2dp_src_buffer[rd_ptr];

            rd_ptr = rd_ptr + 1;
            if (rd_ptr == a2dp->internal_a2dp.a2dp_src_buffer_size)
            {
                rd_ptr = 0;
            }
        }

        /* Update the read pointer */
        a2dp->internal_a2dp.a2dp_src_buffer_rd_ptr = rd_ptr;

        buf_index = 0;

        while (buf_index < bytes_to_send)
        {
            if ((bytes_to_send - buf_index) <= a2dp->internal_a2dp.a2dp_pcm_datalen)
            {
                encode_len = bytes_to_send - buf_index;
            }
            else
            {
                encode_len = a2dp->internal_a2dp.a2dp_pcm_datalen;
            }

            a2dp_encode_n_send
            (
                a2dp,
                a2dp->internal_a2dp.pcm_to_send + buf_index,
                encode_len
            );

            buf_index += encode_len;
        }
    }
}

static void a2dp_process_task(osa_task_param_t arg)
{
    a2dp_task_msg_t message;
    struct bt_a2dp *a2dp;
    API_RESULT retval;

    a2dp_process_queue = NULL;
    if (OSA_MsgQCreate((osa_msgq_handle_t)a2dp_process_queue_handle, A2DP_MSG_Q_COUNT, sizeof(a2dp_task_msg_t)) == KOSA_StatusSuccess)
    {
        a2dp_process_queue = (osa_msgq_handle_t)a2dp_process_queue_handle;
    }

    if (a2dp_process_queue == NULL)
    {
        BT_DBG("Failed to create a2dp_process_queue queue.\r\n");
        vTaskDelete(NULL);
    }

    for (;;)
    {
        if (OSA_MsgQGet(a2dp_process_queue, &message, (uint32_t)osaWaitForever_c) == KOSA_StatusSuccess)
        {
            a2dp = message.a2dp;
            if (NULL == a2dp)
            {
                continue;
            }

            EDGEFAST_A2DP_LOCK;
            if (message.event != A2DP_EVENT_SEND_SBC_PCM_DATA)
            {
                if (a2dp->internal_a2dp.msg_command == A2DP_EVENT_INVALID)
                {
                    continue;
                }
                else
                {
                    message.event = a2dp->internal_a2dp.msg_command;
                    a2dp->internal_a2dp.msg_command = A2DP_EVENT_INVALID;
                }
            }
            switch (message.event)
            {
                case A2DP_EVENT_RETRY_DISCOVER:
                    BT_DBG("retry discover\r\n");
                    if (a2dp->internal_a2dp.retry_count > 0U)
                    {
                        a2dp->internal_a2dp.retry_count--;
                        retval = BT_avdtp_discover_req(&a2dp->internal_a2dp.ethermind_avdtp_handle, a2dp->internal_a2dp.sep_inf_buf, A2DP_MAX_DISCOVER_RESPONSE_LEN);
                        if (API_SUCCESS != retval)
                        {
                            BT_DBG("a2dp discover request fail: %X\r\n", retval);
                            /*  retry */
                            a2dp_set_delay_work(a2dp, A2DP_EVENT_RETRY_DISCOVER, 500U);
                        }
                    }
                    else
                    {
                        if (a2dp->internal_a2dp.auto_configure_enabled)
                        {
                            a2dp_configure_callback_call(a2dp, 1U, -EIO, NULL);
                        }
                        else if (a2dp->discover_peer_endpoint_cb != NULL)
                        {
                            a2dp->discover_peer_endpoint_cb(a2dp, NULL, 0);
                            a2dp->discover_peer_endpoint_cb = NULL;
                        }
                        else
                        {
                            /* MISRA 15.7 : Empty else without comment. */
                        }
                    }
                    break;

                case A2DP_EVENT_RETRY_GET_CAPS:
                    BT_DBG("retry get caps\r\n");
                    if (a2dp->internal_a2dp.retry_count > 0U)
                    {
                        a2dp->internal_a2dp.retry_count--;
                        retval = BT_avdtp_get_capabilities_req
                         (
                             &a2dp->internal_a2dp.ethermind_avdtp_handle,
                             a2dp->internal_a2dp.sep_cap_buf,
                             A2DP_GET_CAPABILITIES_RESPONSE_LEN
                         );

                        if (API_SUCCESS != retval)
                        {
                            BT_DBG("BT_avdtp_get_capabilities_req fail 0x%X\r\n", retval);
                            a2dp_set_delay_work(a2dp, A2DP_EVENT_RETRY_GET_CAPS, 500U);
                        }
                        else
                        {
                            BT_DBG("BT_avdtp_get_capabilities_req success, wait for AVDTP_GET_CAPABILITIES_CNF\r\n");
                        }
                    }
                    else
                    {
                        /* Try to get capabilites of next Information element */
                        if (a2dp_handle_avdtp_discover_rsp(a2dp) != 0)
                        {
                            a2dp_callback_auto_configure(a2dp);
                        }
                    }
                    break;

                case A2DP_EVENT_RETRY_A2DP_CONNECT:
                    if (a2dp->internal_a2dp.retry_count > 0U)
                    {
                        if (a2dp->internal_a2dp.configured_from_peer)
                        {
                            /* don't need retry */
                            a2dp->internal_a2dp.retry_count = 0U;
                            BT_DBG("wait peer a2dp connection\r\n");
                        }
                        else
                        {
                            BT_DBG("retry a2dp connecting\r\n");
                            a2dp->internal_a2dp.retry_count--;
                            retval = BT_a2dp_connect
                                    (
                                        bt_a2dp_get_ethermind_a2dp_index(a2dp),
                                        a2dp->internal_a2dp.peer_addr,
                                        a2dp->internal_a2dp.ethermind_avdtp_handle.remote_seid,
                                        &a2dp->internal_a2dp.sep_conf
                                    );
                            if (API_SUCCESS != retval)
                            {
                                BT_DBG("BT_a2dp_connect fail 0x%X\r\n", retval);
                            }
                            else
                            {
                                BT_DBG("BT_a2dp_connect success, wait for A2DP_CONNECT_CNF\r\n");
                                if (a2dp->internal_selected_endpoint != NULL)
                                {
                                    a2dp_set_a2dp_source_codec_encoder(a2dp,
                                                                     bt_a2dp_get_ethermind_a2dp_codec_type(a2dp),
                                                                     &a2dp->a2dp_config_internal.preset[0]);
                                }
                            }
                        }
                    }
                    else
                    {
                        a2dp_configure_callback_call(a2dp, a2dp->internal_a2dp.auto_configure_enabled, -EIO, NULL);
                    }
                    break;

                case A2DP_EVENT_SEND_SBC_PCM_DATA:
                    edgefast_a2dp_src_write_task(a2dp);
                    break;

                default:
                     /* MISRA 16.4 : The switch statement does not have a non-empty default clause. */
                    break;
            }
            EDGEFAST_A2DP_UNLOCK;
        }
    }
}

static API_RESULT ethermind_a2dp_avdtp_notify_cb
           (
               UCHAR             event_type,
               UINT16            event_result,
               AVDTP_HANDLE *    avdtp_handle,
               void *            event_data,
               UINT16            event_datalen
           )
{
    API_RESULT retval;
    API_RESULT callbackRet;
    struct bt_a2dp *a2dp = NULL;
    AVDTP_SEP_CAP sep_cap;

    int callbackResult = 0;

    BT_DBG("avdtp cb:%x-%x\r\n", event_type, event_result);
    callbackRet = API_SUCCESS;
    switch (event_type)
    {
        case AVDTP_CONNECT_CNF:
        case AVDTP_DISCOVER_CNF:
        case AVDTP_DISCONNECT_IND:
        case AVDTP_DISCONNECT_CNF:
        case AVDTP_GET_CAPABILITIES_CNF:
        case AVDTP_SET_CONFIGURATION_IND:
        case AVDTP_SET_CONFIGURATION_CNF:
            for (uint8_t index = 0; index < CONFIG_BT_MAX_CONN; ++index)
            {
                if ((0U == memcmp(avdtp_handle->bd_addr, a2dp_instances[index].internal_a2dp.peer_addr, 6)) &&
                    (a2dp_instances[index].allocated == 1U))
                {
                    a2dp = &a2dp_instances[index];
                    break;
                }
            }

            if (a2dp == NULL)
            {
                BT_DBG("don't find the a2dp instance\r\n");
                return API_FAILURE;
            }
            break;
    }

    EDGEFAST_A2DP_LOCK;
    switch (event_type)
    {
        case AVDTP_CONNECT_IND:
            if (API_SUCCESS != event_result)
            {
                break;
            }
            /* allocate one a2dp instance */
            a2dp = a2dp_GetInstance();

            if (a2dp == NULL)
            {
                BT_DBG("don't find the a2dp instance\r\n");
                callbackRet = API_FAILURE;
                break;
            }

            /* a2dp source and sink common codes */
            a2dp->internal_a2dp.a2dp_state = INTERNAL_STATE_AVDTP_CONNECTED;
            a2dp->internal_a2dp.connected_from_peer = 1U;
            /* avdtp_handle only have the addr */
            memcpy(a2dp->internal_a2dp.peer_addr, avdtp_handle->bd_addr, 6U);

            /* Initialize AVDTP Handle */
            AVDTP_INIT_HANDLE (a2dp->internal_a2dp.ethermind_avdtp_handle);
            /* Set AVDTP Handle Callback */
            AVDTP_SET_HANDLE_CALLBACK
            (
                a2dp->internal_a2dp.ethermind_avdtp_handle,
                ethermind_a2dp_avdtp_notify_cb
            );
            a2dp->internal_a2dp.configured_from_peer = 0U;
            /* Set AVDTP Remote BD_ADDR */
            AVDTP_SET_HANDLE_BD_ADDR (a2dp->internal_a2dp.ethermind_avdtp_handle, avdtp_handle->bd_addr);
            a2dp_connectcallback_call(a2dp, A2DP_CB_CONNECTED, 0, NULL);
            break;

        case AVDTP_CONNECT_CNF:
            if (API_SUCCESS == event_result)
            {
                BT_DBG("enter INTERNAL_STATE_AVDTP_CONNECTED\r\n");
                a2dp->internal_a2dp.connected_from_peer = 0U;
                a2dp->internal_a2dp.configured_from_peer = 0U;
                a2dp->internal_a2dp.a2dp_state = INTERNAL_STATE_AVDTP_CONNECTED;
                callbackResult = 0;
            }
            else
            {
                BT_DBG("AVDTP_CONNECT_CNF error code:0x%X\r\n", event_result);
                callbackResult = -EIO;
                if (a2dp != NULL)
                {
                    a2dp_FreeInstance(a2dp);
                }
            }

            a2dp_connectcallback_call(a2dp, A2DP_CB_CONNECTED, callbackResult, NULL);
            break;

        case AVDTP_DISCONNECT_IND:
        case AVDTP_DISCONNECT_CNF:
            if (a2dp->internal_a2dp.a2dp_state != INTERNAL_STATE_IDLE)
            {
                a2dp->internal_a2dp.a2dp_state = INTERNAL_STATE_IDLE;
            }
            a2dp_connectcallback_call(a2dp, A2DP_CB_DISCONNECTED, 0, NULL);
            a2dp_FreeInstance(a2dp);
            break;

        case AVDTP_DISCOVER_CNF:
            if (API_SUCCESS != event_result)
            {
                /* retry */
                a2dp_set_delay_work(a2dp, A2DP_EVENT_RETRY_DISCOVER, 500U);
                BT_DBG("*** Discover RSP Error Code = 0x%X\r\n", event_result);
            }
            else
            {
                /* Update Info Len and start of info element pointer */
                a2dp->internal_a2dp.sep_inf_buflen = event_datalen;
                a2dp->internal_a2dp.sep_inf_buf_start = a2dp->internal_a2dp.sep_inf_buf;
                if (a2dp->internal_a2dp.auto_configure_enabled)
                {
                    if (a2dp_handle_avdtp_discover_rsp(a2dp) != 0)
                    {
                        a2dp_callback_auto_configure(a2dp);
                    }
                }
                else if (a2dp->discover_peer_endpoint_cb != NULL)
                {
                    if (a2dp_handle_avdtp_discover_rsp(a2dp) != 0)
                    {
                        a2dp->discover_peer_endpoint_cb(a2dp, NULL, 0U);
                        a2dp->discover_peer_endpoint_cb = NULL;
                    }
                }
            }
            break;

        case AVDTP_GET_CAPABILITIES_CNF:
            if (API_SUCCESS != event_result)
            {
                a2dp_set_delay_work(a2dp, A2DP_EVENT_RETRY_GET_CAPS, 500U);
                break;
            }

            /* Decode SEP Capabilities - Codec Capabilities */
            retval = BT_avdtp_decode_sep_capabilities
                    (
                        (uint8_t *) event_data,
                        event_datalen,
                        &sep_cap
                    );

            if (API_SUCCESS != retval)
            {
                BT_DBG("*** FAILED to Decode SEP Capabilities = 0x%X\r\n", retval);

                /* Try to get capabilites of next Information element */
                if (a2dp_handle_avdtp_discover_rsp(a2dp) != 0)
                {
                    if (a2dp->internal_a2dp.auto_configure_enabled)
                    {
                        a2dp_callback_auto_configure(a2dp);
                    }
                    else if (a2dp->discover_peer_endpoint_cb != NULL)
                    {
                        /* Try to get capabilites of next Information element */
                        a2dp->discover_peer_endpoint_cb(a2dp, NULL, 0);
                        a2dp->discover_peer_endpoint_cb = NULL;
                    }
                    else
                    {
                        /* MISRA 15.7 : Empty else without comment. */
                    }
                }
                break;
            }

            if (a2dp->internal_a2dp.auto_configure_enabled)
            {
                uint32_t count = a2dp_get_endpoints_count();
                uint32_t firstEndpointIndex = 0xFFU;
                for (uint32_t index = 0; index < count; index++)
                {
                    if (a2dp->peer_endpoint_idle_seid[index])
                    {
                        firstEndpointIndex = index;
                        break;
                    }
                }

                for (uint32_t index = 0; index < count; index++)
                {
                    if ((a2dp_endpoint_internals[index].endpoint->info.sep.tsep != a2dp->internal_a2dp.current_seid_info.tsep) &&
                        (a2dp_endpoint_internals[index].endpoint->codec_id == sep_cap.codec_cap.codec_type))
                    {
                        a2dp->peer_endpoint_idle_seid[index] = a2dp->internal_a2dp.current_seid_info.id;
                        if (index < firstEndpointIndex)
                        {
                            a2dp->select_peer_endpoint.codec_id = sep_cap.codec_cap.codec_type;
                            a2dp->select_peer_endpoint.info.sep = a2dp->internal_a2dp.current_seid_info;
                            a2dp->select_peer_endpoint.preset = NULL;
                            a2dp->select_peer_endpoint_internal_cap.len = sep_cap.codec_cap.codec_ie_len;
                            memcpy(&a2dp->select_peer_endpoint_internal_cap.preset[0], sep_cap.codec_cap.codec_ie,
                                   ((sep_cap.codec_cap.codec_ie_len > BT_A2DP_CODEC_IE_LENGTH_MAX) ? BT_A2DP_CODEC_IE_LENGTH_MAX : sep_cap.codec_cap.codec_ie_len));
                            a2dp->select_peer_endpoint.capabilities = (struct bt_a2dp_preset *)&a2dp->select_peer_endpoint_internal_cap;
                        }
                        break;
                    }
                }
#if 0
                /* save and get next */
                if (a2dp->peer_endpoint_count < (A2DP_MAX_PEER_ENDPOINT_COUNT - 1))
                {
                    a2dp->peer_endpoints[a2dp->peer_endpoint_count].codec_id = sep_cap.codec_cap.codec_type;
                    a2dp->peer_endpoints[a2dp->peer_endpoint_count].info.sep = a2dp->internal_a2dp.current_seid_info;
                    a2dp->peer_endpoints[a2dp->peer_endpoint_count].preset = NULL;
                    a2dp->peer_endpoint_internal_cap[a2dp->peer_endpoint_count].len = sep_cap.codec_cap.codec_ie_len;
                    memcpy(&a2dp->peer_endpoint_internal_cap[a2dp->peer_endpoint_count].preset[0], sep_cap.codec_cap.codec_ie,
                           ((sep_cap.codec_cap.codec_ie_len > BT_A2DP_CODEC_IE_LENGTH_MAX) ? BT_A2DP_CODEC_IE_LENGTH_MAX : sep_cap.codec_cap.codec_ie_len));
                    a2dp->peer_endpoints[a2dp->peer_endpoint_count].capabilities = (struct bt_a2dp_preset *)&a2dp->peer_endpoint_internal_cap[a2dp->peer_endpoint_count];
                    a2dp->peer_endpoint_count++;
                }
#endif

                if (a2dp_handle_avdtp_discover_rsp(a2dp) != 0)
                {
                    a2dp_callback_auto_configure(a2dp);
                }
            }
            else if (a2dp->discover_peer_endpoint_cb != NULL)
            {
                /* callback to app by standalone callback */
                struct bt_a2dp_endpoint peer_endpoint;
                struct bt_a2dp_preset_internal peer_endpoint_internal_cap;

                peer_endpoint.codec_id = sep_cap.codec_cap.codec_type;
                peer_endpoint.info.sep = a2dp->internal_a2dp.current_seid_info;
                peer_endpoint.preset = NULL;
                peer_endpoint_internal_cap.len = sep_cap.codec_cap.codec_ie_len;
                memcpy(&peer_endpoint_internal_cap.preset[0], sep_cap.codec_cap.codec_ie,
                        ((sep_cap.codec_cap.codec_ie_len > BT_A2DP_CODEC_IE_LENGTH_MAX) ? BT_A2DP_CODEC_IE_LENGTH_MAX : sep_cap.codec_cap.codec_ie_len));
                peer_endpoint.capabilities = (struct bt_a2dp_preset *)&peer_endpoint_internal_cap;

                if (a2dp->discover_peer_endpoint_cb(a2dp, &peer_endpoint, 0) == BT_A2DP_DISCOVER_ENDPOINT_CONTINUE)
                {
                    if (a2dp_handle_avdtp_discover_rsp(a2dp) != 0)
                    {
                        a2dp->discover_peer_endpoint_cb(a2dp, NULL, 0);
                        a2dp->discover_peer_endpoint_cb = NULL;
                    }
                }
            }
            else
            {
            }
            break;

        case AVDTP_SET_CONFIGURATION_CNF:
            a2dp->internal_a2dp.configured_from_peer = 0U;
            break;

        case AVDTP_SET_CONFIGURATION_IND:
            a2dp->internal_a2dp.configured_from_peer = 1U;
            break;

        default:
            /* MISRA 16.4 : The switch statement does not have a non-empty default clause. */
            break;
    }
    EDGEFAST_A2DP_UNLOCK;
    return callbackRet;
}

static API_RESULT ethermind_a2dp_notify_cb
           (
               UCHAR    codec_instance,
               UCHAR    event_type,
               UINT16   event_result,
               void  *  event_data,
               UINT16   event_datalen
           )
{
    API_RESULT retval;
    API_RESULT callbackRet;
    struct bt_a2dp *a2dp = NULL;
    uint8_t index;

    BT_DBG("a2dp cb:%x-%x\r\n", event_type, event_result);
    for (index = 0; index < CONFIG_BT_MAX_CONN; index++)
    {
        if (a2dp_instances[index].internal_selected_endpoint != NULL)
        {
            if (codec_instance == bt_a2dp_get_ethermind_a2dp_index(&a2dp_instances[index]))
            {
                /* Matching instance found */
                a2dp = &a2dp_instances[index];
                break;
            }
        }
    }

    if ((a2dp == NULL) && (A2DP_CONNECT_IND != event_type) && (A2DP_CONFIGURE_IND != event_type))
    {
        BT_DBG ("No Matching index found\r\n");
        return API_SUCCESS;
    }

    callbackRet = API_SUCCESS;
    EDGEFAST_A2DP_LOCK;
    switch (event_type)
    {
        case A2DP_CONNECT_CNF:
            if (a2dp->internal_a2dp.auto_configure_enabled)
            {
                if (BT_A2DP_SOURCE == bt_a2dp_get_ethermind_a2dp_sep_type(a2dp))
                {
                    BT_DBG("Received A2DP_CONNECT_CNF\r\n");
                    BT_DBG("    Codec Instance = %d\r\n", codec_instance);
                    BT_DBG("    Event Result   = 0x%X\r\n", event_result);
                    if (API_SUCCESS != event_result)
                    {
                        a2dp_set_delay_work(a2dp, A2DP_EVENT_RETRY_A2DP_CONNECT, 500U);
                    }
                    else
                    {
                        struct a2dp_configure_result configureResult;

                        a2dp->internal_a2dp.a2dp_state = INTERNAL_STATE_CONFIGURED_OPENED;

                        a2dp->internal_selected_endpoint->config_result = (struct bt_a2dp_preset*)&a2dp->a2dp_config_internal;

                        configureResult.err = 0;
                        configureResult.config = a2dp->internal_selected_endpoint->config_result;
                        configureResult.endpoint = a2dp->internal_selected_endpoint->endpoint;
                        a2dp_configure_callback_call(a2dp, 1U, 0, &configureResult);
                    }
                }
                else
                {
                    //todo: sink support auto configure too.
                }
            }
            else
            {
                a2dp->internal_a2dp.a2dp_state = INTERNAL_STATE_CONFIGURED_OPENED;
                a2dp->internal_selected_endpoint->config_result = (struct bt_a2dp_preset*)&a2dp->a2dp_config_internal;
                a2dp_configure_callback_call(a2dp, 0U, event_result, NULL);
            }
            break;

        case A2DP_CONNECT_IND:
            if (API_SUCCESS != event_result)
            {
                break;
            }

            a2dp->internal_a2dp.configured_from_peer = 1U;
            if (a2dp == NULL)
            {
                for (index = 0; index < CONFIG_BT_MAX_CONN; index++)
                {
                    if (a2dp->internal_a2dp.connected_from_peer)
                    {
                        /* Matching instance found */
                        a2dp = &a2dp_instances[index];
                        break;
                    }
                }
            }

            if (a2dp == NULL)
            {
                break;
            }

            /* get the endpoint */
            for (index = 0; index < A2DP_MAX_ENDPOINT_COUNT; ++index)
            {
                if (a2dp_endpoint_internals[index].ethermind_a2dp_codec_index == codec_instance)
                {
                    a2dp->internal_selected_endpoint = &a2dp_endpoint_internals[index];
                    break;
                }
            }

            if (a2dp->internal_selected_endpoint == NULL)
            {
                break;
            }

            if (BT_A2DP_SOURCE == bt_a2dp_get_ethermind_a2dp_sep_type(a2dp))
            {
                /* keep all the retry, disconnect a2dp, the retry will do re-connect again. */
                a2dp->internal_a2dp.a2dp_state = INTERNAL_STATE_DISCONNECT_CONNECT_AGAIN;
                retval = BT_a2dp_disconnect(bt_a2dp_get_ethermind_a2dp_index(a2dp));
#if ((defined CONFIG_BT_DEBUG_A2DP) && (CONFIG_BT_DEBUG_A2DP))
                if (API_SUCCESS != retval)
                {
                    BT_DBG("BT_a2dp_disconnect fail 0x%X\r\n", retval);
                }
                else
                {
                    BT_DBG("BT_a2dp_disconnect success, wait for A2DP_DISCONNECT_CNF\r\n");
                }
#else
                (void)retval;
#endif
            }
            else
            {
                a2dp->internal_a2dp.a2dp_state = INTERNAL_STATE_CONFIGURED_OPENED;
                /* configured callback in the A2DP_CONFIGURE_IND */
            }
            break;

        case A2DP_DISCONNECT_CNF:
            /* for the case that headset connects to board, we disconnect then connect */
            if (a2dp->internal_a2dp.a2dp_state == INTERNAL_STATE_DISCONNECT_CONNECT_AGAIN)
            {
                a2dp->internal_a2dp.configured_from_peer = 0U;
                a2dp->internal_a2dp.a2dp_state = INTERNAL_STATE_AVDTP_CONNECTED;
                /* the retry will connect again */
                /* if the retry has fail and done. */
                if (a2dp->internal_a2dp.auto_configure_enabled)
                {
                    a2dp->internal_a2dp.retry_count = 0U;
                    bt_delayed_work_cancel(&a2dp->retry_work);
                    if (a2dp->internal_a2dp.discover_done)
                    {
                        a2dp->internal_a2dp.discover_done = 0U;
                        a2dp->internal_a2dp.retry_count = A2DP_TRANSFER_RETRY_COUNT;
                        a2dp_set_delay_work(a2dp, A2DP_EVENT_RETRY_A2DP_CONNECT, 0U);
                    }
                    else
                    {
                        bt_a2dp_configure(a2dp, a2dp->configure_cb);
                    }
                }
            }
            else
            {
                if (a2dp->internal_a2dp.a2dp_state != INTERNAL_STATE_IDLE)
                {
                    a2dp->internal_a2dp.a2dp_state = INTERNAL_STATE_AVDTP_CONNECTED;
                    if (BT_A2DP_SINK == bt_a2dp_get_ethermind_a2dp_sep_type(a2dp))
                    {
                        BT_jpl_stop();
                    }

                    if (a2dp->de_configure_cb != NULL)
                    {
                        if (API_SUCCESS != event_result)
                        {
                            a2dp->de_configure_cb(a2dp, -EACCES);
                        }
                        else
                        {
                            a2dp->de_configure_cb(a2dp, 0);
                        }
                        a2dp->de_configure_cb = NULL;
                    }
                }
            }
            break;

        case A2DP_DISCONNECT_IND:
            a2dp->internal_a2dp.configured_from_peer = 0U;
            if (a2dp->internal_a2dp.a2dp_state != INTERNAL_STATE_IDLE)
            {
                a2dp->internal_a2dp.a2dp_state = INTERNAL_STATE_IDLE;
                if (BT_A2DP_SINK == bt_a2dp_get_ethermind_a2dp_sep_type(a2dp))
                {
                    BT_jpl_stop();
                }
            }
            break;

        case A2DP_MEDIA_FRAME_IND:
        {
            if (bt_a2dp_get_ethermind_a2dp_codec_type(a2dp) == BT_A2DP_SBC)
            {
                uint32_t timestamp;
                uint8_t *data;
                uint16_t seq_number;
                API_RESULT retval;

                /**
                 *   Media Packet: 12 Octets of RTP header, followed by Media Payload
                 *   For SBC, the Media Payload will have one byte of header containing
                 *   number of frames, followed by SBC Frames.
                 */
                data = (uint8_t *)event_data;

                /* Extract Sequence Number */
                seq_number = *(data + 2);
                seq_number <<= 8;
                seq_number |= *(data + 3);

                /* Extract Time Stamp */
                timestamp = *(data + 4);
                timestamp <<= 8;
                timestamp |= *(data + 5);
                timestamp <<= 8;
                timestamp |= *(data + 6);
                timestamp <<= 8;
                timestamp |= *(data + 7);

                /**
                 * Add SBC media frames to JPL.
                 *
                 * JPL to be provided with the one byte of SBC header,
                 * along with the SBC frames.
                 */
                retval = BT_jpl_add_frames
                         (
                             seq_number,
                             timestamp,
                             (data + 12),
                             (event_datalen - 12)
                         );
                if (API_SUCCESS != retval)
                {
                    BT_DBG("JPL Add Frames Failed - 0x%X\r\n", retval);
                }
            }
            else
            {
                a2dp_streamer_data_t streamerData;
                streamerData.data = (uint8_t *)event_data;
                streamerData.data_length = event_datalen;
                a2dp_control_callback_call(a2dp, A2DP_CONTROL_SINK_STREAMER_DATA, 0, &streamerData);
            }
            break;
        }

        case A2DP_START_CNF:
            if (BT_A2DP_SOURCE == bt_a2dp_get_ethermind_a2dp_sep_type(a2dp))
            {
                a2dp_control_callback_call(a2dp, A2DP_CONTROL_START_PLAY, 0, NULL);
            }
            else
            {
                if (bt_a2dp_get_ethermind_a2dp_codec_type(a2dp) == BT_A2DP_SBC)
                {
                    /* Start JPL */
                    BT_jpl_start(&a2dp->internal_a2dp.jpl_param);
                }
            }
            break;

        case A2DP_START_IND:
            if (BT_A2DP_SOURCE == bt_a2dp_get_ethermind_a2dp_sep_type(a2dp))
            {
                callbackRet = API_FAILURE;
            }
            else
            {
                jpl_active_a2dp = a2dp;
                a2dp_control_callback_call(a2dp, A2DP_CONTROL_START_PLAY, 0, NULL);
                if (bt_a2dp_get_ethermind_a2dp_codec_type(a2dp) == BT_A2DP_SBC)
                {
                    /* Start JPL */
                    BT_jpl_start(&a2dp->internal_a2dp.jpl_param);
                }
            }
            break;

        case A2DP_SUSPEND_CNF:
            if (BT_A2DP_SOURCE == bt_a2dp_get_ethermind_a2dp_sep_type(a2dp))
            {
                a2dp_control_callback_call(a2dp, A2DP_CONTROL_SUSPEND_PLAY, 0, NULL);
            }
            else
            {
                if (bt_a2dp_get_ethermind_a2dp_codec_type(a2dp) == BT_A2DP_SBC)
                {
                    BT_jpl_stop();
                }
            }
            break;

        case A2DP_SUSPEND_IND:
            if (BT_A2DP_SOURCE == bt_a2dp_get_ethermind_a2dp_sep_type(a2dp))
            {
                /* a2dp source should not send this command. */
                callbackRet = API_FAILURE;
            }
            else
            {
                if (bt_a2dp_get_ethermind_a2dp_codec_type(a2dp) == BT_A2DP_SBC)
                {
                    BT_jpl_stop();
                }
                a2dp_control_callback_call(a2dp, A2DP_CONTROL_SUSPEND_PLAY, 0, NULL);
            }
            break;

        case A2DP_RECONFIGURE_CNF:
            callbackRet = API_SUCCESS;
            if (a2dp->re_configure_cb != NULL)
            {
                struct a2dp_configure_result configureResult;

                a2dp->internal_a2dp.a2dp_state = INTERNAL_STATE_CONFIGURED_OPENED;
                a2dp->internal_selected_endpoint->config_result = (struct bt_a2dp_preset*)&a2dp->a2dp_config_internal;
                configureResult.err = 0;
                configureResult.config = a2dp->internal_selected_endpoint->config_result;
                configureResult.endpoint = a2dp->internal_selected_endpoint->endpoint;
                a2dp->re_configure_cb(a2dp, &configureResult);
            }
            break;

        case A2DP_CONFIGURE_IND:
            if (a2dp == NULL)
            {
                for (index = 0; index < CONFIG_BT_MAX_CONN; index++)
                {
                    if (a2dp_instances[index].internal_a2dp.connected_from_peer)
                    {
                        /* Matching instance found */
                        a2dp = &a2dp_instances[index];
                        break;
                    }
                }
            }

            if (a2dp == NULL)
            {
                break;
            }

            /* get the endpoint */
            for (index = 0; index < A2DP_MAX_ENDPOINT_COUNT; ++index)
            {
                if (a2dp_endpoint_internals[index].ethermind_a2dp_codec_index == codec_instance)
                {
                    a2dp->internal_selected_endpoint = &a2dp_endpoint_internals[index];
                    break;
                }
            }

            if (index >= A2DP_MAX_ENDPOINT_COUNT)
            {
                break;
            }

            if (BT_A2DP_SOURCE == bt_a2dp_get_ethermind_a2dp_sep_type(a2dp))
            {
                a2dp->internal_a2dp.configured_from_peer = 1U;
                /* don't need do anything. */
                callbackRet = API_SUCCESS;
            }
            else
            {
                struct a2dp_configure_result configureResult;
                /* A2DP_CONNECT_IND has the configuration information */
                A2DP_DEVICE_INFO * a2dp_dev_info = (A2DP_DEVICE_INFO *)event_data;
                /* BD Address */
                BT_mem_copy
                (
                    a2dp->internal_a2dp.peer_addr,
                    a2dp_dev_info->bd_addr,
                    BT_BD_ADDR_SIZE
                );

                /* Codec Configuration */
                a2dp->a2dp_config_internal.len = a2dp_dev_info->codec_ie_len;
                memcpy
                (
                    &a2dp->a2dp_config_internal.preset[0],
                    a2dp_dev_info->codec_ie_conf,
                    a2dp_dev_info->codec_ie_len
                );

                configureResult.err = 0;
                a2dp->internal_selected_endpoint->config_result = (struct bt_a2dp_preset*)&a2dp->a2dp_config_internal;
                configureResult.config = a2dp->internal_selected_endpoint->config_result;
                configureResult.endpoint = a2dp->internal_selected_endpoint->endpoint;

                if (bt_a2dp_get_ethermind_a2dp_codec_type(a2dp) == BT_A2DP_SBC)
                {
                    a2dp_jpl_configure(a2dp, &a2dp->a2dp_config_internal.preset[0]);
                }

                a2dp_control_callback_call(a2dp, A2DP_CONTROL_CONFIGURED, 0, &configureResult);
            }
            break;

        default:
            callbackRet = API_FAILURE;
            break;
    }
    EDGEFAST_A2DP_UNLOCK;

    return callbackRet;
}

int bt_a2dp_init(void)
{
    API_RESULT retval;
#ifdef JPL_USE_APP_MEMORY
    JPL_BUFFERS buffer;
#endif

    retval = BT_a2dp_init();
    if (API_SUCCESS != retval)
    {
        return -EIO;
    }

    if (a2dp_process_task_handle == NULL)
    {
        if (KOSA_StatusSuccess == OSA_TaskCreate((osa_task_handle_t)a2dp_process_task_def, OSA_TASK(a2dp_process_task), NULL))
        {
            a2dp_process_task_handle = (osa_task_handle_t)a2dp_process_task_def;
        }
        else
        {
            BT_DBG("fail to create a2dp process task\r\n");
            return -EIO;
        }
    }

    if (NULL == a2dp_lock)
    {
        if (KOSA_StatusSuccess == OSA_MutexCreate((osa_mutex_handle_t)a2dp_lock_mutex))
        {
            a2dp_lock = (osa_mutex_handle_t)a2dp_lock_mutex;
        }
        else
        {
            OSA_TaskDestroy(a2dp_process_task_handle);
            a2dp_process_task_handle = NULL;
            return -EIO;
        }
    }

    for (uint8_t index = 0; index < CONFIG_BT_MAX_CONN; ++index)
    {
        a2dp_instances[index].allocated = 0U;
        bt_delayed_work_init(&a2dp_instances[index].retry_work, a2dp_retry_work_timeout);
    }

    for (uint8_t index = 0; index < A2DP_MAX_ENDPOINT_COUNT; ++index)
    {
        a2dp_endpoint_internals[index].ethermind_a2dp_codec_index = 0xFFu;
        a2dp_endpoint_internals[index].endpoint = NULL;
    }

    BT_avdtp_callback_register(ethermind_a2dp_avdtp_notify_cb);

    /* only for a2dp sink */
    BT_jpl_init(jpl_callback_handle);
#ifdef JPL_USE_APP_MEMORY
    buffer.sbc = a2dp_sbc_buffer;
    buffer.pcm = a2dp_pcm_buffer;
    buffer.silence = a2dp_silence_buffer;
    /* Register memory to JPL */
    BT_jpl_register_buffers(&buffer);
#endif /* JPL_USE_APP_MEMORY */

    return 0;
}

#if 0
int bt_a2dp_deinit(void)
{
    if (a2dp_process_task_handle != NULL)
    {
        OSA_TaskDestroy(a2dp_process_task_handle);
        a2dp_process_task_handle = NULL;
    }

    if (a2dp_lock != NULL)
    {
        OSA_TaskDestroy(a2dp_lock);
        a2dp_lock = NULL;
    }

    /* JPL doesn't have de-init function */
    return 0;
}
#endif

int bt_a2dp_register_endpoint(struct bt_a2dp_endpoint *endpoint, uint8_t media_type, uint8_t role)
{
    AVDTP_SEP_CAP sep_cap;
    uint32_t a2dpRecordHandle;
    uint8_t index;
    API_RESULT retval;

    if (endpoint == NULL)
    {
        return -EIO;
    }

    endpoint->info.sep.id = 0U;
    endpoint->info.sep.inuse = 0U;
    endpoint->info.sep.media_type = media_type;
    endpoint->info.sep.tsep = role;
    /* Initialize Stream End Point Capability */
    AVDTP_SET_SEP_CAPABILITY
    (
        sep_cap,
        ((BT_A2DP_SOURCE == role) ? AVDTP_SEP_SOURCE : AVDTP_SEP_SINK),
        (uint8_t)media_type,
        endpoint->codec_id,
        endpoint->capabilities->len,
        endpoint->capabilities->preset
    );

    for (index = 0; index < A2DP_MAX_ENDPOINT_COUNT; ++index)
    {
        if (a2dp_endpoint_internals[index].ethermind_a2dp_codec_index == 0xFFu)
        {
            a2dp_endpoint_internals[index].endpoint = endpoint;
            retval = BT_a2dp_register_codec
                    (
                        &a2dp_endpoint_internals[index].ethermind_a2dp_codec_index,
                        &sep_cap,
                        ethermind_a2dp_notify_cb
                    );

            if (API_SUCCESS != retval)
            {
                return -EIO;
            }
            break;
        }
    }

    if (index >= A2DP_MAX_ENDPOINT_COUNT)
    {
        return -EIO;
    }

#if 0
    endpoint->next = NULL;
    searchEndpoint = s_Endpoints;
    if (s_Endpoints == NULL)
    {
        s_Endpoints = endpoint;
    }
    else
    {
        while (searchEndpoint->next != NULL)
        {
            searchEndpoint = searchEndpoint->next;
        }
        searchEndpoint->next = endpoint;
    }
#endif

    /* Get the A2DP Source record handle */
    retval = BT_dbase_get_record_handle
                (
                 ((BT_A2DP_SOURCE == role) ? DB_RECORD_A2DP_SOURCE : DB_RECORD_A2DP_SINK),
                    0,
                    &a2dpRecordHandle
                );
    if (API_SUCCESS == retval)
    {
        BT_dbase_activate_record (a2dpRecordHandle);
    }

    return 0;
}

int bt_a2dp_register_control_callback(struct bt_a2dp *a2dp, struct bt_a2dp_control_cb *cb)
{
    if (a2dp != NULL)
    {
        a2dp->control_cb = *cb;
        return 0;
    }
    return -EIO;
}

int bt_a2dp_register_connect_callback(struct bt_a2dp_connect_cb *cb)
{
    connect_cb = *cb;
    return 0;
}

struct bt_a2dp *bt_a2dp_connect(struct bt_conn *conn)
{
    struct bt_conn_info info;
    struct bt_a2dp *a2dp;
    API_RESULT retval;

    a2dp = a2dp_GetInstance();
    if (NULL == a2dp)
    {
        return NULL;
    }

    a2dp->internal_a2dp.a2dp_state = INTERNAL_STATE_IDLE;

    /* source and sink have same codes
        * start avdtp connection
        * Initialize AVDTP Handle */
    AVDTP_INIT_HANDLE (a2dp->internal_a2dp.ethermind_avdtp_handle);
    /* Set AVDTP Handle Callback */
    AVDTP_SET_HANDLE_CALLBACK
    (
        a2dp->internal_a2dp.ethermind_avdtp_handle,
        ethermind_a2dp_avdtp_notify_cb
    );

    bt_conn_get_info(conn, &info);
    if (info.type == BT_CONN_TYPE_LE)
    {
        a2dp_FreeInstance(a2dp);
        return NULL;
    }
    /* Set AVDTP Remote BD_ADDR */
    AVDTP_SET_HANDLE_BD_ADDR
    (
        a2dp->internal_a2dp.ethermind_avdtp_handle,
        (uint8_t*)info.br.dst
    );
    memcpy(a2dp->internal_a2dp.peer_addr, info.br.dst, BT_BD_ADDR_SIZE);
    /* AVDTP Connect */
    retval = BT_avdtp_connect_req
            (
                &a2dp->internal_a2dp.ethermind_avdtp_handle
            );
#if ((defined CONFIG_BT_DEBUG_A2DP) && (CONFIG_BT_DEBUG_A2DP))
    if (API_SUCCESS != retval)
    {
        BT_DBG("BT_avdtp_connect_req fail 0x%X\r\n", retval);
    }
    else
    {
        BT_DBG("BT_avdtp_connect_req success, wait for AVDTP_CONNECT_CNF\r\n");
    }
#else
    (void)retval;
#endif

    return a2dp;
}

int bt_a2dp_configure(struct bt_a2dp *a2dp, bt_a2dp_configure_cb_t cb)
{
    API_RESULT retval;

    if (a2dp == NULL)
    {
        return -EIO;
    }

    if (a2dp->internal_a2dp.a2dp_state != INTERNAL_STATE_AVDTP_CONNECTED)
    {
        return -EIO;
    }

    a2dp->internal_a2dp.auto_configure_enabled = 1U;
    a2dp->configure_cb = cb;

    BT_DBG("enter INTERNAL_STATE_AVDTP_CONNECTED\r\n");
    a2dp->internal_a2dp.retry_count = A2DP_TRANSFER_RETRY_COUNT;
    retval = BT_avdtp_discover_req(&a2dp->internal_a2dp.ethermind_avdtp_handle, a2dp->internal_a2dp.sep_inf_buf, A2DP_MAX_DISCOVER_RESPONSE_LEN);
    if (API_SUCCESS != retval)
    {
        BT_DBG("a2dp discover request fail: %X\r\n", retval);
        a2dp_set_delay_work(a2dp, A2DP_EVENT_RETRY_DISCOVER, 500U);
    }
    else
    {
        BT_DBG("wait AVDTP_DISCOVER_CNF\r\n");
    }

    return 0;
}

int bt_a2dp_src_start(struct bt_a2dp *a2dp, bt_a2dp_start_cb_t cb)
{
    API_RESULT retval;
    uint16_t acl_handle;

    if (a2dp == NULL)
    {
        return -EIO;
    }

    a2dp->source_start_play = cb;
    /* Initialize the task variables and create the task */
    a2dp->internal_a2dp.a2dp_src_wr_th_state = APP_A2DP_SRC_WR_TH_INIT;
    a2dp->internal_a2dp.a2dp_src_buffer_size = A2DP_SRC_MAX_BUFFER_SIZE;
    a2dp->internal_a2dp.a2dp_src_buffer_rd_ptr = 0U;
    a2dp->internal_a2dp.a2dp_src_buffer_wr_ptr = 0U;
    retval = BT_a2dp_start (bt_a2dp_get_ethermind_a2dp_index(a2dp));
    if (API_SUCCESS == retval)
    {
        /* Unsniff the link if required */
        BT_hci_get_acl_connection_handle(a2dp->internal_a2dp.peer_addr, &acl_handle);
        BT_hci_exit_sniff_mode(acl_handle);
    }
    else
    {
        return -EIO;
    }

    return 0;
}

int bt_a2dp_src_suspend(struct bt_a2dp *a2dp, bt_a2dp_suspend_cb_t cb)
{
    API_RESULT retval;

    if (a2dp == NULL)
    {
        return -EIO;
    }

    a2dp->source_suspend_play = cb;
    retval = BT_a2dp_suspend (bt_a2dp_get_ethermind_a2dp_index(a2dp));
    if (API_SUCCESS != retval)
    {
        return -EIO;
    }

    return 0;
}

static void a2dp_src_enqueue
           (
               struct bt_a2dp *a2dp,
               uint8_t *data,
               uint16_t datalen
           )
{
    int32_t  n_free;
    uint32_t count;

    /*
     *  Get amount of free buffer space.
     */
    if (a2dp->internal_a2dp.a2dp_src_buffer_wr_ptr >= a2dp->internal_a2dp.a2dp_src_buffer_rd_ptr)
    {
        /*
         *    ----------DDDDDDDDDDDDDDD--------------X
         *    ^         ^              ^             ^
         *    |         |              |             |
         *    0       rd_ptr         wr_ptr         max
         *
         *  Free Space => '-----------'
         */
        n_free = (a2dp->internal_a2dp.a2dp_src_buffer_size - a2dp->internal_a2dp.a2dp_src_buffer_wr_ptr)
                + a2dp->internal_a2dp.a2dp_src_buffer_rd_ptr;
    }
    else
    {
        /*
         *    DDDDDDDDDD---------------DDDDDDDDDDDDDDX
         *    ^         ^              ^             ^
         *    |         |              |             |
         *    0       wr_ptr         rd_ptr         max
         *
         *  Free Space => '-----------'
         */
        n_free =a2dp->internal_a2dp. a2dp_src_buffer_rd_ptr - a2dp->internal_a2dp.a2dp_src_buffer_wr_ptr;
    }

    /*
     *  Do we have enough space to accomodate new data ?
     *  Buffer Empty Condition: when rd_ptr == wr_ptr
     *  Buffer Full  Condition: when diff(rd_ptr, wr_ptr) == 1
     */
    if ((n_free < 1) || (n_free < datalen))
    {
        /* Overflow ! */
        return;
    }

    /* Store new data into Buffer */
    for (count = 0; count < datalen; count++)
    {
        a2dp->internal_a2dp.a2dp_src_buffer[a2dp->internal_a2dp.a2dp_src_buffer_wr_ptr] = data[count];
        a2dp->internal_a2dp.a2dp_src_buffer_wr_ptr++;

        if(a2dp->internal_a2dp.a2dp_src_buffer_wr_ptr == a2dp->internal_a2dp.a2dp_src_buffer_size)
        {
            a2dp->internal_a2dp.a2dp_src_buffer_wr_ptr = 0;
        }
    }

    EDGEFAST_A2DP_LOCK;
    if (APP_A2DP_SRC_WR_TH_INIT == a2dp->internal_a2dp.a2dp_src_wr_th_state)
    {
        /* Signal the waiting thread */
        a2dp->internal_a2dp.a2dp_src_wr_th_state = APP_A2DP_SRC_WR_TH_PLAYING;
        a2dp_set_delay_work(a2dp, A2DP_EVENT_SEND_SBC_PCM_DATA, 0U);
        EDGEFAST_A2DP_UNLOCK;
    }
    else
    {
        EDGEFAST_A2DP_UNLOCK;
    }

    return;
}


int bt_a2dp_src_media_write(struct bt_a2dp *a2dp, uint8_t *data, uint16_t datalen)
{
    if (a2dp == NULL)
    {
        return -EIO;
    }

    if (bt_a2dp_get_ethermind_a2dp_codec_type(a2dp) == BT_A2DP_SBC)
    {
        a2dp_src_enqueue(a2dp, data, datalen);
    }
    else
    {
        //todo: other codec encoded data sending.
    }
    return 0;
}

int bt_a2dp_snk_media_sync(struct bt_a2dp *a2dp, uint8_t *data, uint16_t datalen)
{
    if (a2dp == NULL)
    {
        return -EIO;
    }

    if (bt_a2dp_get_ethermind_a2dp_codec_type(a2dp) == BT_A2DP_SBC)
    {
        BT_IGNORE_UNUSED_PARAM(data);
        BT_IGNORE_UNUSED_PARAM(datalen);

        BT_jpl_remove_frames(a2dp->internal_a2dp.a2dp_jpl_pof, a2dp->internal_a2dp.a2dp_jpl_poflen);
    }

    return 0;
}

int bt_a2dp_deconfigure(struct bt_a2dp *a2dp, bt_a2dp_deconfigure_cb_t cb)
{
    API_RESULT retval;

    if ((a2dp == NULL) || (cb == NULL))
    {
        return -EINVAL;
    }

    a2dp->de_configure_cb = cb;
    retval = BT_a2dp_disconnect(a2dp->internal_selected_endpoint->ethermind_a2dp_codec_index);
    if (API_SUCCESS != retval)
    {
        return -EACCES;
    }
    else
    {
        return 0;
    }
}

int bt_a2dp_discover_peer_endpoints(struct bt_a2dp *a2dp, bt_a2dp_discover_peer_endpoint_cb_t cb)
{
    API_RESULT retval;

    if ((a2dp == NULL) || (cb == NULL))
    {
        return -EINVAL;
    }

    if (a2dp->internal_a2dp.a2dp_state != INTERNAL_STATE_AVDTP_CONNECTED)
    {
        return -EIO;
    }

    a2dp->discover_peer_endpoint_cb = cb;
    retval = BT_avdtp_discover_req(&a2dp->internal_a2dp.ethermind_avdtp_handle, a2dp->internal_a2dp.sep_inf_buf, A2DP_MAX_DISCOVER_RESPONSE_LEN);
    if (API_SUCCESS != retval)
    {
        return -EACCES;
    }
    else
    {
        return 0;
    }
}

int bt_a2dp_configure_endpoint(struct bt_a2dp *a2dp, struct bt_a2dp_endpoint *endpoint,
							   struct bt_a2dp_preset *config, bt_a2dp_configure_cb_t cb)
{
    API_RESULT retval;
    uint32_t index;
    uint32_t count;

    if ((a2dp == NULL) || (endpoint == NULL) || (config == NULL) || (cb == NULL))
    {
        return -EINVAL;
    }

    if (a2dp->internal_a2dp.a2dp_state != INTERNAL_STATE_AVDTP_CONNECTED)
    {
        return -EINVAL;
    }

    count = a2dp_get_endpoints_count();
    for (index = 0; index < count; index++)
    {
        if (endpoint->codec_id == a2dp_endpoint_internals[index].endpoint->codec_id)
        {
            break;
        }
    }
    if (index >= count)
    {
        return -EINVAL;
    }

    a2dp->a2dp_config_internal.len = config->len;
    memcpy
    (
        &a2dp->a2dp_config_internal.preset[0],
        &config->preset[0],
        config->len
    );

    a2dp->configure_ep_cb = cb;
    memcpy(&a2dp->select_peer_endpoint, endpoint, sizeof(*endpoint));
    a2dp->select_peer_endpoint.preset = NULL;
    a2dp->select_peer_endpoint_internal_cap.len = endpoint->capabilities->len;
    memcpy(&a2dp->select_peer_endpoint_internal_cap.preset[0], &endpoint->capabilities->preset[0],
            ((endpoint->capabilities->len > BT_A2DP_CODEC_IE_LENGTH_MAX) ? BT_A2DP_CODEC_IE_LENGTH_MAX : endpoint->capabilities->len));
    a2dp->select_peer_endpoint.capabilities = (struct bt_a2dp_preset *)&a2dp->select_peer_endpoint_internal_cap;

    /* load the BD addr */
    BT_COPY_BD_ADDR(a2dp->internal_a2dp.peer_addr, a2dp->internal_a2dp.ethermind_avdtp_handle.bd_addr);
    /* Initialize SEP Configuration Parameters */
    AVDTP_INIT_SEP_CONFIGURATION (a2dp->internal_a2dp.sep_conf);
    /* Enable Media Transport Service in SEP Configuration */
    AVDTP_SET_SEP_CONFIGURATION_MEDIA (a2dp->internal_a2dp.sep_conf);
    /* Set AVDTP Handle Remote SEID */
    AVDTP_SET_HANDLE_REMOTE_SEID (a2dp->internal_a2dp.ethermind_avdtp_handle, endpoint->info.sep.id);
    /* Enable Codec Capabilities in SEP Configuration */
    AVDTP_SET_SEP_CONFIGURATION_CODEC_CAP
    (
        a2dp->internal_a2dp.sep_conf,
        BT_A2DP_AUDIO,
        endpoint->codec_id,
        config->len,
        config->preset
    )

    a2dp->internal_selected_endpoint = &a2dp_endpoint_internals[index];
    a2dp->internal_a2dp.retry_count = A2DP_TRANSFER_RETRY_COUNT;
    retval = BT_a2dp_connect
            (
                bt_a2dp_get_ethermind_a2dp_index(a2dp),
                a2dp->internal_a2dp.peer_addr,
                a2dp->internal_a2dp.ethermind_avdtp_handle.remote_seid,
                &a2dp->internal_a2dp.sep_conf
            );
    if (API_SUCCESS != retval)
    {
        BT_DBG("BT_a2dp_connect fail 0x%X\r\n", retval);
        return -EINVAL;
    }
    else
    {
        BT_DBG("BT_a2dp_connect success, wait for A2DP_CONNECT_CNF\r\n");
        //todo: only support sbc codec now.
        a2dp_set_a2dp_source_codec_encoder(a2dp, endpoint->codec_id, config->preset);
    }
    return 0;
}

struct bt_a2dp_endpoint *bt_a2dp_get_configured_peer_endpoint(struct bt_a2dp *a2dp)
{
    return &a2dp->select_peer_endpoint;
}

int bt_a2dp_reconfigure(struct bt_a2dp *a2dp, struct bt_a2dp_preset *config, bt_a2dp_configure_cb_t cb)
{
    API_RESULT retval;

    if ((a2dp == NULL) || (config == NULL) || (cb == NULL))
    {
        return -EINVAL;
    }

    /* Initialize SEP Configuration Parameters */
    AVDTP_INIT_SEP_CONFIGURATION (a2dp->internal_a2dp.sep_conf);
    /* Enable Codec Capabilities in SEP Configuration */
    AVDTP_SET_SEP_CONFIGURATION_CODEC_CAP
    (
        a2dp->internal_a2dp.sep_conf,
        BT_A2DP_AUDIO,
        a2dp->internal_selected_endpoint->endpoint->codec_id,
        config->len,
        config->preset
    )

    a2dp->re_configure_cb = cb;
    a2dp->a2dp_config_internal.len = config->len;
    memcpy
    (
        &a2dp->a2dp_config_internal.preset[0],
        &config->preset[0],
        config->len
    );
    retval = BT_a2dp_reconfigure(a2dp->internal_selected_endpoint->ethermind_a2dp_codec_index,
                                 &a2dp->internal_a2dp.sep_conf);
    if (retval == 0U)
    {
        return 0U;
    }
    else
    {
        return -EACCES;
    }
}
