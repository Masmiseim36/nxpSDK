/*
 * Copyright 2020 - 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))

#include <porting.h>
#include <string.h>
#include <stdio.h>
#include <errno/errno.h>
#include <sys/atomic.h>
#include <sys/util.h>
#include <sys/slist.h>
#include <sys/byteorder.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/a2dp.h>
#include <bluetooth/a2dp_codec_sbc.h>
#include "bt_pal_conn_internal.h"
#include "bt_pal_a2dp_internal.h"
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

#if ((defined(CONFIG_BT_A2DP)) && (CONFIG_BT_A2DP > 0U))

#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
#ifndef AVDTP_HAVE_CONTENT_PROTECTION
#error "please enable the correct configuration";
#endif
#endif

#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))
#ifndef AVDTP_HAVE_RECOVERY_SERVICE
#error "please enable the correct configuration";
#endif
#endif

#if ((defined(CONFIG_BT_A2DP_REPORTING_SERVICE)) && (CONFIG_BT_A2DP_REPORTING_SERVICE > 0U))
#ifndef AVDTP_HAVE_REPORTING_SERVICE
#error "please enable the correct configuration";
#endif
#endif

#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
#ifndef AVDTP_HAVE_DELAY_REPORTING
#error "please enable the correct configuration";
#endif
#endif

#if ((defined(CONFIG_BT_A2DP_HC_SERVICE)) && (CONFIG_BT_A2DP_HC_SERVICE > 0U))
#ifndef AVDTP_HAVE_ROHC_SERVICE
#error "please enable the correct configuration";
#endif
#endif

#if ((defined(CONFIG_BT_A2DP_MULTIPLEXING_SERVICE)) && (CONFIG_BT_A2DP_MULTIPLEXING_SERVICE > 0U))
#ifndef AVDTP_HAVE_MULTIPLEXING
#error "please enable the correct configuration";
#endif
#endif

#if !(CONFIG_BT_A2DP_SOURCE || CONFIG_BT_A2DP_SINK)
#error The feature CONFIG_BT_A2DP_SOURCE or CONFIG_BT_A2DP_SINK should be set.
#endif

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_A2DP)
#define LOG_MODULE_NAME bt_a2dp
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#define SBC_BLOCK_MAX               (512U)
#define BT_A2DP_CODEC_IE_LENGTH_MAX (32U)
#define A2DP_SRC_MAX_BUFFER_SIZE    (4096U)
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
    INTERNAL_STATE_DISCONNECTING,
};

struct bt_a2dp_codec_ie_internal {
    /** Length of preset */
    uint8_t len;
    /** Preset */
    uint8_t codec_ie[BT_A2DP_CODEC_IE_LENGTH_MAX];
};

#if ((defined(CONFIG_BT_A2DP_SOURCE)) && (CONFIG_BT_A2DP_SOURCE > 0U))
struct bt_a2dp_sbc_encoder {
    SBC_ENCODER_IO_STRUCT a2dp_sbc_encoder_io;
    int32_t a2dp_src_buffer_rd_ptr;
    int32_t a2dp_src_buffer_wr_ptr;
    int32_t a2dp_src_buffer_size;
    uint8_t a2dp_src_buffer[A2DP_SRC_MAX_BUFFER_SIZE];
    uint8_t a2dp_src_wr_th_state;
    uint8_t a2dp_sbc_data[SBC_BLOCK_MAX];
    uint8_t pcm_to_send[SBC_BLOCK_MAX];
    uint16_t a2dp_pcm_data_le [SBC_BLOCK_MAX >> 1];
    uint16_t a2dp_pcm_datalen;
};
#endif

#if ((defined(CONFIG_BT_A2DP_SINK)) && (CONFIG_BT_A2DP_SINK > 0U))
struct bt_a2dp_sbc_decoder_buf {
    uint8_t a2dp_sbc_buffer[JPL_SBC_FRAME_SIZE * JPL_SBC_NUM_FRAMES];
    uint8_t a2dp_pcm_buffer[JPL_PCM_BLOCK_SIZE * JPL_PCM_NUM_BLOCKS];
    uint8_t a2dp_silence_buffer[JPL_PCM_BLOCK_SIZE];
};

struct bt_a2dp_sbc_decoder {
    JPL_PARAM jpl_param;
    uint8_t *a2dp_jpl_pof;
    uint16_t a2dp_jpl_poflen;
    struct bt_a2dp_sbc_decoder_buf *buf;
};
#endif

struct bt_a2dp_codec_state {
    union {
#if ((defined(CONFIG_BT_A2DP_SOURCE)) && (CONFIG_BT_A2DP_SOURCE > 0U))
        struct bt_a2dp_sbc_encoder sbc_encoder;
#endif
#if ((defined(CONFIG_BT_A2DP_SINK)) && (CONFIG_BT_A2DP_SINK > 0U))
        struct bt_a2dp_sbc_decoder sbc_decoder;
#endif
    };
};

struct bt_a2dp_endpoint_state
{
    struct bt_a2dp *a2dp;
    struct bt_a2dp_endpoint *endpoint;
    uint8_t ethermind_a2dp_codec_index;
    /* save the config result's content */
    struct bt_a2dp_codec_ie_internal config_internal;
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
    struct bt_a2dp_codec_ie_internal cp_config;
    uint8_t cp_header_len;
#endif
#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))
    struct bt_a2dp_codec_ie_internal recovery_config;
#endif
#if ((defined(CONFIG_BT_A2DP_REPORTING_SERVICE)) && (CONFIG_BT_A2DP_REPORTING_SERVICE > 0U))
    uint8_t reporting_service_config;
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
    uint8_t delay_reporting_config;
#endif
#if ((defined(CONFIG_BT_A2DP_HC_SERVICE)) && (CONFIG_BT_A2DP_HC_SERVICE > 0U))
    uint8_t rohc_config;
#endif
#if ((defined(CONFIG_BT_A2DP_MULTIPLEXING_SERVICE)) && (CONFIG_BT_A2DP_MULTIPLEXING_SERVICE > 0U))
    uint8_t multiplexing_config;
#endif
    struct bt_a2dp_codec_state *codec;
    uint8_t *buffer_points[JPL_INITIAL_NUM_DATA_READ_IND];
    uint8_t buffer_produce;
    uint8_t buffer_consume;
};

struct bt_a2dp
{
    struct bt_delayed_work retry_work;
    AVDTP_HANDLE ethermind_avdtp_handle;
    AVDTP_SEP_CONF sep_conf;
    struct bt_avdtp_seid_info current_seid_info;
    /* Pointer to start of Discover Response Info Element */
    uint8_t *discover_buf_start;
    bt_a2dp_discover_peer_endpoint_cb_t discover_peer_endpoint_cb;
    void (*configure_cb)(int err);
    struct bt_a2dp_codec_ie_internal config_internal;/* after success, copy to endpoint config_internal */
    struct bt_a2dp_codec_ie_internal select_peer_endpoint_internal_cap;
    void *retry_param;
    uint16_t discover_buflen;
    uint8_t discover_buf[A2DP_MAX_DISCOVER_RESPONSE_LEN];
    struct bt_a2dp_codec_ie_internal peer_codec_ie;
    struct bt_avdtp_seid_info peer_seid_info;
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
    /** content protection ie */
    struct bt_a2dp_codec_ie_internal peer_cp_ie[AVDTP_MAX_CP_IE];
    /** content protection ie count */
    uint8_t peer_cp_ie_count;
#endif
#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))
    /** Pointer to recovery service default config */
    struct bt_a2dp_codec_ie_internal peer_recovery_ie;
#endif
#if ((defined(CONFIG_BT_A2DP_REPORTING_SERVICE)) && (CONFIG_BT_A2DP_REPORTING_SERVICE > 0U))
    uint8_t peer_reporting_service;
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
    uint8_t peer_delay_reporting;
    uint8_t sep_cap_buf[A2DP_GET_CAPABILITIES_RESPONSE_LEN + 10U];
#else
    uint8_t sep_cap_buf[A2DP_GET_CAPABILITIES_RESPONSE_LEN];
#endif
#if ((defined(CONFIG_BT_A2DP_HC_SERVICE)) && (CONFIG_BT_A2DP_HC_SERVICE > 0U))
    uint8_t peer_rohc_cap;
#endif
#if ((defined(CONFIG_BT_A2DP_MULTIPLEXING_SERVICE)) && (CONFIG_BT_A2DP_MULTIPLEXING_SERVICE > 0U))
    uint8_t peer_multiplexing;
#endif
    uint8_t peer_addr[BT_BD_ADDR_SIZE];
    uint8_t select_peer_seid;
    uint8_t select_peer_index;
    uint8_t retry_count;
    uint8_t retry_command;
    uint8_t msg_command;
    enum bt_a2dp_internal_state a2dp_state;
    /* Flag - Whether allocated or free */
    uint8_t allocated;
    uint8_t configured_from_peer : 1; //todo: move to endpoint??
    uint8_t discover_done : 1;
    uint8_t connected_from_peer : 1;
    uint8_t auto_configure_enabled : 1;
};

enum a2dp_control_callback_event
{
    A2DP_CONTROL_CONFIGURED,
    A2DP_CONTROL_START_PLAY,
    A2DP_CONTROL_STOP_PLAY,
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

static struct bt_a2dp_endpoint_state a2dp_endpoint_states[CONFIG_BT_A2DP_MAX_ENDPOINT_COUNT];

static struct bt_a2dp_connect_cb connect_cb;

static OSA_TASK_HANDLE_DEFINE(a2dp_process_task_def);
static osa_task_handle_t a2dp_process_task_handle;
static void a2dp_process_task(osa_task_param_t arg);
static OSA_TASK_DEFINE(a2dp_process_task, CONFIG_BT_A2DP_TASK_PRIORITY, 1, CONFIG_BT_A2DP_TASK_STACK_SIZE, 0);

static OSA_MUTEX_HANDLE_DEFINE(a2dp_lock_mutex);
static osa_mutex_handle_t a2dp_lock;
static struct bt_a2dp a2dp_instances[CONFIG_BT_A2DP_MAX_CONN];
static osa_msgq_handle_t a2dp_process_queue;
static OSA_MSGQ_HANDLE_DEFINE(a2dp_process_queue_handle, A2DP_MSG_Q_COUNT, sizeof(a2dp_task_msg_t));
#if ((defined(CONFIG_BT_A2DP_SINK)) && (CONFIG_BT_A2DP_SINK > 0U))
static struct bt_a2dp_endpoint_state *jpl_active_ep;
#endif

#define EDGEFAST_A2DP_LOCK OSA_MutexLock(a2dp_lock, osaWaitForever_c)
#define EDGEFAST_A2DP_UNLOCK OSA_MutexUnlock(a2dp_lock)

typedef struct _a2dp_task_msg_
{
    void *param;
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
    for (uint8_t index = 0; index < CONFIG_BT_A2DP_MAX_CONN; ++index)
    {
        if (a2dp_instances[index].allocated == 0U)
        {
            memset(&a2dp_instances[index].ethermind_avdtp_handle, 0U, sizeof (a2dp_instances[index]) - sizeof (struct bt_delayed_work));
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

    for (index = 0; index < CONFIG_BT_A2DP_MAX_ENDPOINT_COUNT; ++index)
    {
        if (a2dp_endpoint_states[index].endpoint == NULL)
        {
            break;
        }
    }
    return index;
}

static struct bt_a2dp_endpoint_state *bt_a2dp_get_ethermind_endpoint_state(uint8_t ethermind_index)
{
    for (uint8_t index = 0; index < CONFIG_BT_A2DP_MAX_ENDPOINT_COUNT; ++index)
    {
        if (a2dp_endpoint_states[index].ethermind_a2dp_codec_index == ethermind_index)
        {
            return &a2dp_endpoint_states[index];
        }
    }

    return NULL;
}

static struct bt_a2dp_endpoint_state *bt_a2dp_get_endpoint_state(
            struct bt_a2dp_endpoint *endpoint)
{
    uint8_t index;

    for (index = 0; index < CONFIG_BT_A2DP_MAX_ENDPOINT_COUNT; ++index)
    {
        if (a2dp_endpoint_states[index].endpoint == endpoint)
        {
            return &a2dp_endpoint_states[index];
        }
    }

    return NULL;
}

static struct bt_conn *get_conn;
static void a2dp_get_conn(struct bt_conn *conn, void *data)
{
    uint8_t *addr = (uint8_t *)data;

    if (memcmp(&conn->br.dst, addr, 6u) == 0u)
    {
        get_conn = conn;
    }
}

static void a2dp_configure_ep_callback_call(struct bt_a2dp *a2dp,
                struct bt_a2dp_endpoint_state *ep_state,
                int result)
{
    if (a2dp == NULL)
    {
        return;
    }

    if (ep_state->endpoint->control_cbs.configured != NULL)
    {
        struct bt_a2dp_endpoint_configure_result configureResult;

        get_conn = NULL;
        bt_conn_foreach(BT_CONN_TYPE_BR, a2dp_get_conn, &a2dp->peer_addr[0]);
        configureResult.a2dp = a2dp;
        configureResult.conn = get_conn;
        if (result == 0)
        {
            configureResult.err = 0;
            configureResult.config.media_config = (struct bt_a2dp_codec_ie *)&ep_state->config_internal;
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
            if (ep_state->cp_config.len != 0)
            {
                configureResult.config.cp_config = (struct bt_a2dp_codec_ie *)&ep_state->cp_config;
            }
            else
            {
                configureResult.config.cp_config = NULL;
            }
#endif
#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))
            if (ep_state->recovery_config.len != 0)
            {
                configureResult.config.recovery_config = (struct bt_a2dp_codec_ie *)&ep_state->recovery_config;
            }
            else
            {
                configureResult.config.recovery_config = NULL;
            }
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
            configureResult.config.delay_reporting_service_config = ep_state->delay_reporting_config;
#endif
#if ((defined(CONFIG_BT_A2DP_REPORTING_SERVICE)) && (CONFIG_BT_A2DP_REPORTING_SERVICE > 0U))
            configureResult.config.reporting_service_config = ep_state->reporting_service_config;
#endif
#if ((defined(CONFIG_BT_A2DP_HC_SERVICE)) && (CONFIG_BT_A2DP_HC_SERVICE > 0U))
            configureResult.config.header_compression_config = ep_state->rohc_config;
#endif
#if ((defined(CONFIG_BT_A2DP_MULTIPLEXING_SERVICE)) && (CONFIG_BT_A2DP_MULTIPLEXING_SERVICE > 0U))
            configureResult.config.multiplexing_service_config = ep_state->multiplexing_config;
#endif

            ep_state->endpoint->control_cbs.configured(&configureResult);
        }
        else
        {
            configureResult.err = -1;
            configureResult.config.media_config = NULL;
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
            configureResult.config.cp_config = NULL;
#endif
#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))
            configureResult.config.recovery_config = NULL;
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
            configureResult.config.delay_reporting_service_config = 0U;
#endif
#if ((defined(CONFIG_BT_A2DP_REPORTING_SERVICE)) && (CONFIG_BT_A2DP_REPORTING_SERVICE > 0U))
            configureResult.config.reporting_service_config = 0U;
#endif
#if ((defined(CONFIG_BT_A2DP_MULTIPLEXING_SERVICE)) && (CONFIG_BT_A2DP_MULTIPLEXING_SERVICE > 0U))
            configureResult.config.multiplexing_service_config = 0U;
#endif

            ep_state->endpoint->control_cbs.configured(&configureResult);
        }
    }
}

static void a2dp_control_ind_callback_call(struct bt_a2dp_endpoint_state *ep_state, uint8_t event, int result, void *parameter)
{
    struct bt_a2dp_control_cb *cbs = &ep_state->endpoint->control_cbs;

    switch (event)
    {
        case A2DP_CONTROL_CONFIGURED:
            a2dp_configure_ep_callback_call(ep_state->a2dp, ep_state, result);
            break;
        case A2DP_CONTROL_START_PLAY:
            if (cbs->start_play != NULL)
            {
                if (ep_state->endpoint->codec_id == BT_A2DP_SBC)
                {
                    ep_state->buffer_produce = 0u;
                    ep_state->buffer_consume = 0u;
                }
                cbs->start_play(result);
            }
            break;
        case A2DP_CONTROL_STOP_PLAY:
            if (cbs->stop_play != NULL)
            {
                cbs->stop_play(result);
            }
            break;
#if ((defined(CONFIG_BT_A2DP_SINK)) && (CONFIG_BT_A2DP_SINK > 0U))
        case A2DP_CONTROL_SINK_STREAMER_DATA:
            if (cbs->sink_streamer_data != NULL)
            {
                if (ep_state->endpoint->codec_id == BT_A2DP_SBC)
                {
                    ep_state->buffer_points[ep_state->buffer_produce++] = ((a2dp_streamer_data_t *)parameter)->data;
                    if (ep_state->buffer_produce == (sizeof (ep_state->buffer_points) / sizeof (ep_state->buffer_points[0])))
                    {
                        ep_state->buffer_produce = 0u;
                    }
                }
                cbs->sink_streamer_data(((a2dp_streamer_data_t *)parameter)->data, ((a2dp_streamer_data_t *)parameter)->data_length);
            }
            break;
#endif
        default:
            /* MISRA 16.4 : The switch statement does not have a non-empty default clause. */
            break;
    }
}

static void a2dp_connectcallback_call(struct bt_a2dp *a2dp, uint8_t event, int result)
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

static void a2dp_auto_configure_callback_call(struct bt_a2dp *a2dp,
                struct bt_a2dp_endpoint_state *ep_state, int result)
{
    a2dp->auto_configure_enabled = 0U;

    if (result == 0)
    {
        if (a2dp->configure_cb != NULL)
        {
            a2dp->configure_cb(0);
        }
        a2dp_configure_ep_callback_call(a2dp, ep_state, 0);
    }
    else
    {
        if (a2dp->configure_cb != NULL)
        {
            a2dp->configure_cb(result);
        }
    }
}

#if ((defined(CONFIG_BT_A2DP_SOURCE)) && (CONFIG_BT_A2DP_SOURCE > 0U))
static int a2dp_set_a2dp_source_codec_encoder(struct bt_a2dp_codec_state *codec, uint8_t codecType, uint8_t *codec_ie)
{
    API_RESULT retval;
    /* Get SBC Encoder Codec Parameters */
    SBC_CODEC_PARAM * codec_param;
    struct bt_a2dp_sbc_encoder *sbc_encoder = &codec->sbc_encoder;

    if (codecType != AVDTP_CODEC_AUDIO_SBC)
    {
        return 0;
    }

    codec_param = &sbc_encoder->a2dp_sbc_encoder_io.encoder_params;
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

    sbc_encoder->a2dp_pcm_datalen = 2 *
                  codec_param->nrof_subbands *
                  codec_param->nrof_blocks *
                  codec_param->nrof_channels;
    /* Initialize SBC Decoder */
    retval = sbc_encoder_init (&sbc_encoder->a2dp_sbc_encoder_io);
    if (API_SUCCESS != retval)
    {
        BT_DBG("*** Failed to Initialize SBC Encoder: 0x%X\r\n", retval);
        return -EIO;
    }
    return 0;
}
#endif

static void a2dp_send_task_msg(void *param, uint8_t event)
{
    a2dp_task_msg_t message;
    message.param = param;
    message.event = event;
    OSA_MsgQPut(a2dp_process_queue, &message);
}

static void a2dp_retry_work_timeout(struct bt_work *work)
{
    struct bt_a2dp *a2dp = CONTAINER_OF(work, struct bt_a2dp, retry_work);

    EDGEFAST_A2DP_LOCK;
    a2dp->msg_command = a2dp->retry_command;
    EDGEFAST_A2DP_UNLOCK;
    a2dp_send_task_msg(a2dp->retry_param, a2dp->msg_command);
}

static void a2dp_set_delay_work(struct bt_a2dp *a2dp,
                    void *param,
                    uint8_t event, uint32_t delayMs)
{
    a2dp->retry_param = param;
    a2dp->retry_command = event;
    if (delayMs == 0)
    {
        a2dp_retry_work_timeout((struct k_work*)&a2dp->retry_work);
    }
    else
    {
        k_work_schedule(&a2dp->retry_work, BT_MSEC(delayMs));
    }
}

static void a2dp_callback_auto_configure(struct bt_a2dp *a2dp)
{
    uint8_t index;
    uint8_t count;
    API_RESULT retval;

    if (a2dp->a2dp_state != INTERNAL_STATE_AVDTP_CONNECTED)
    {
        return;
    }

    count = a2dp_get_endpoints_count();

    index = a2dp->select_peer_index;
    if ((a2dp->select_peer_seid != 0) && (index < count))
    {
        struct bt_a2dp_codec_ie *config;
        struct bt_a2dp_endpoint *self_endpoint = a2dp_endpoint_states[index].endpoint;
        struct bt_a2dp_endpoint_state *ep_state = &a2dp_endpoint_states[index];

        ep_state->a2dp = a2dp;
        if (self_endpoint->config == NULL)
        {
            a2dp_auto_configure_callback_call(a2dp, ep_state, -EIO);
            return;
        }
        else
        {
            config = self_endpoint->config;
        }

        a2dp->config_internal.len = config->len;
        memcpy
        (
            &a2dp->config_internal.codec_ie[0],
            &config->codec_ie[0],
            config->len
        );

        /* load the BD addr */
        BT_COPY_BD_ADDR(a2dp->peer_addr, a2dp->ethermind_avdtp_handle.bd_addr);
        /* Initialize SEP Configuration Parameters */
        AVDTP_INIT_SEP_CONFIGURATION (a2dp->sep_conf);
        /* Enable Media Transport Service in SEP Configuration */
        AVDTP_SET_SEP_CONFIGURATION_MEDIA (a2dp->sep_conf);
        /* Set AVDTP Handle Remote SEID */
        AVDTP_SET_HANDLE_REMOTE_SEID (a2dp->ethermind_avdtp_handle, a2dp->select_peer_seid);
        /* Enable Codec Capabilities in SEP Configuration */
        AVDTP_SET_SEP_CONFIGURATION_CODEC_CAP
        (
            a2dp->sep_conf,
            BT_A2DP_AUDIO,
            self_endpoint->codec_id,
            config->len,
            config->codec_ie
        )
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
        ep_state->cp_config.len = 0;
        if (self_endpoint->cp_config != NULL)
        {
            uint16_t peer_type;
            uint16_t type = (uint16_t)(self_endpoint->cp_config->codec_ie[0]) | ((uint16_t)self_endpoint->cp_config->codec_ie[1] << 8U);

            for (index = 0; index < a2dp->peer_cp_ie_count; index++)
            {
                peer_type = (uint16_t)(a2dp->peer_cp_ie[index].codec_ie[0]) | ((uint16_t)a2dp->peer_cp_ie[index].codec_ie[1] << 8U);
                if (type == peer_type)
                {
                    AVDTP_SET_SEP_CP_CAPABILITY
                    (
                        (a2dp->sep_conf.cp_cap[0]),
                        type,
                        self_endpoint->cp_config->codec_ie,
                        self_endpoint->cp_config->len - 2
                    );
                    ep_state->cp_config = *((struct bt_a2dp_codec_ie_internal *)self_endpoint->cp_config);
                    break;
                }
            }
        }
#endif
#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))
        ep_state->recovery_config.len = 0u;
        if ((self_endpoint->recovery_config != NULL) && (a2dp->peer_recovery_ie.len != 0))
        {
            AVDTP_SET_SEP_CONFIGURATION_RECOVERY
            (
                a2dp->sep_conf,
                self_endpoint->recovery_config->codec_ie[0],
                self_endpoint->recovery_config->codec_ie[1],
                self_endpoint->recovery_config->codec_ie[2]
            );
            ep_state->recovery_config = *((struct bt_a2dp_codec_ie_internal *)(self_endpoint->recovery_config));
        }
#endif
#if ((defined(CONFIG_BT_A2DP_REPORTING_SERVICE)) && (CONFIG_BT_A2DP_REPORTING_SERVICE > 0U))
        ep_state->reporting_service_config = 0;
        if ((self_endpoint->reporting_service_enable != 0u) && (a2dp->peer_reporting_service != 0))
        {
            AVDTP_SET_SEP_CONFIGURATION_REPORTING(a2dp->sep_conf);
            ep_state->reporting_service_config = 1;
        }
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
        ep_state->delay_reporting_config = 0;
        if ((self_endpoint->delay_reporting_service_enable != 0) && (a2dp->peer_delay_reporting != 0))
        {
            AVDTP_SET_SEP_CONFIGURATION_DELAY_REPORTING (a2dp->sep_conf);
            ep_state->delay_reporting_config = 1;
        }
#endif
#if ((defined(CONFIG_BT_A2DP_HC_SERVICE)) && (CONFIG_BT_A2DP_HC_SERVICE > 0U))
        ep_state->rohc_config = 0;
        if ((self_endpoint->header_compression_config != 0) && (a2dp->peer_rohc_cap != 0))
        {
            AVDTP_SET_SEP_CONFIGURATION_ROHC (a2dp->sep_conf);
            a2dp->sep_conf.rohc_conf = self_endpoint->header_compression_config;
            ep_state->rohc_config = self_endpoint->header_compression_config;
        }
#endif
#if ((defined(CONFIG_BT_A2DP_MULTIPLEXING_SERVICE)) && (CONFIG_BT_A2DP_MULTIPLEXING_SERVICE > 0U))
        ep_state->multiplexing_config = 0;
        if ((self_endpoint->multiplexing_service_enable != 0) && (a2dp->peer_multiplexing != 0))
        {
            ep_state->multiplexing_config = 1;
            AVDTP_SET_SEP_CONFIGURATION_MULTIPLEXING (a2dp->sep_conf);
        }
#endif

        a2dp->discover_done = 1U;
        /* A2DP Connect */
        if (a2dp->connected_from_peer)
        {
            BT_DBG("A2DP connect wait ...\r\n");
            /* the peer may establish the a2dp connection firstly */
            a2dp->retry_count = A2DP_TRANSFER_RETRY_COUNT;
            a2dp_set_delay_work(a2dp, ep_state, A2DP_EVENT_RETRY_A2DP_CONNECT, 500U);
        }
        else
        {
            BT_DBG("A2DP connect ...\r\n");
            a2dp->retry_count = A2DP_TRANSFER_RETRY_COUNT;
            ep_state->a2dp = a2dp;
            retval = BT_a2dp_connect
                    (
                        ep_state->ethermind_a2dp_codec_index,
                        a2dp->peer_addr,
                        a2dp->ethermind_avdtp_handle.remote_seid,
                        &a2dp->sep_conf
                    );
            if (API_SUCCESS != retval)
            {
                BT_DBG("BT_a2dp_connect fail 0x%X\r\n", retval);
            }
            else
            {
                BT_DBG("BT_a2dp_connect success, wait for A2DP_CONNECT_CNF\r\n");
                //todo: only support sbc codec now.
#if ((defined(CONFIG_BT_A2DP_SOURCE)) && (CONFIG_BT_A2DP_SOURCE > 0U))
                a2dp_set_a2dp_source_codec_encoder(ep_state->codec, self_endpoint->codec_id, config->codec_ie);
#endif
            }
        }
    }
    else
    {
        a2dp_auto_configure_callback_call(a2dp, NULL, -EIO);
    }
}

#if ((defined(CONFIG_BT_A2DP_SINK)) && (CONFIG_BT_A2DP_SINK > 0U))
static void a2dp_jpl_configure(struct bt_a2dp_sbc_decoder *sbc_decoder, uint8_t* codec_ie)
{
    SBC_CODEC_PARAM * codec_param;

    codec_param = &sbc_decoder->jpl_param.sbc_param;

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

    sbc_decoder->jpl_param.pcm_num_frames =
        ((3 * (SBC_MAX_BLOCK_SIZE * SBC_MAX_SUBBAND)) /
        (codec_param->nrof_blocks * codec_param->nrof_subbands));

    sbc_decoder->jpl_param.pod_frames =
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

    if (jpl_active_ep != NULL)
    {
        a2dp_streamer_data_t streamerData;
        streamerData.data = jpl_active_ep->codec->sbc_decoder.a2dp_jpl_pof = event_data;
        streamerData.data_length = jpl_active_ep->codec->sbc_decoder.a2dp_jpl_poflen = event_datalen;

        a2dp_control_ind_callback_call(jpl_active_ep, A2DP_CONTROL_SINK_STREAMER_DATA, 0, &streamerData);
    }

    return API_SUCCESS;
}
#endif

static int a2dp_handle_avdtp_discover_rsp (struct bt_a2dp *a2dp)
{
    API_RESULT retval;
    AVDTP_SEP_INFO sep_info;
    uint8_t sepIndex;

    for (sepIndex = 0U; sepIndex < a2dp->discover_buflen; sepIndex += 2U)
    {
        retval = BT_avdtp_decode_sep_information
                 (
                     a2dp->discover_buf_start,
                     (UINT16) a2dp->discover_buflen,
                     &sep_info
                 );

        /* Set response buffer to next SEP info index */
        a2dp->discover_buf_start += 2;
        a2dp->discover_buflen -= 2;

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

            if ((!(a2dp->auto_configure_enabled)) ||
                ((AVDTP_MEDIA_AUDIO == sep_info.media_type) && (0x00 == sep_info.in_use)))
            {
                /* Set AVDTP Handle Remote SEID */
                AVDTP_SET_HANDLE_REMOTE_SEID (a2dp->ethermind_avdtp_handle, sep_info.acp_seid);

                /* AVDTP Get Capabilities */
                BT_DBG("AVDTP Get Capabilities ... ");
                a2dp->current_seid_info.id = sep_info.acp_seid;
                a2dp->current_seid_info.inuse = sep_info.in_use;
                a2dp->current_seid_info.media_type = sep_info.media_type;
                a2dp->current_seid_info.tsep = sep_info.sep_type;

                a2dp->retry_count = A2DP_TRANSFER_RETRY_COUNT;
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
                retval = BT_avdtp_get_all_capabilities_req
                         (
                             &a2dp->ethermind_avdtp_handle,
                             a2dp->sep_cap_buf,
                             A2DP_GET_CAPABILITIES_RESPONSE_LEN + 10U
                         );
#else
                retval = BT_avdtp_get_capabilities_req
                         (
                             &a2dp->ethermind_avdtp_handle,
                             a2dp->sep_cap_buf,
                             A2DP_GET_CAPABILITIES_RESPONSE_LEN
                         );
#endif

                if (API_SUCCESS != retval)
                {
                    BT_DBG("BT_avdtp_get_capabilities_req fail 0x%X\r\n", retval);
                    a2dp_set_delay_work(a2dp, a2dp, A2DP_EVENT_RETRY_GET_CAPS, 500U);
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

#if ((defined(CONFIG_BT_A2DP_SOURCE)) && (CONFIG_BT_A2DP_SOURCE > 0U))
static API_RESULT a2dp_encode_n_send
           (
               struct bt_a2dp_endpoint_state *ep_state,
               uint8_t    *pcm_data,
               uint16_t   pcm_datalen
           )
{
    API_RESULT        retval;
    uint16_t            count;
    uint16_t            sbc_datalen;
    struct bt_a2dp_sbc_encoder *sbc_enc = &ep_state->codec->sbc_encoder;

    /* Convert UCHAR LE16 PCM Data to uint16_t LE16 */
    for (count = 0; count < (pcm_datalen >> 1); count ++)
    {
        sbc_enc->a2dp_pcm_data_le[count] =
            (uint16_t) ((pcm_data[2 * count + 1] << 8) | (pcm_data[2 * count]));
    }

    /* Initialize amount of buffer passed for SBC stream */
    sbc_datalen = sbc_enc->a2dp_pcm_datalen;

    /* Prepare for SBC Encoder */
    sbc_enc->a2dp_sbc_encoder_io.encoder_input_buflen = count;
    sbc_enc->a2dp_sbc_encoder_io.encoder_input_buffer = sbc_enc->a2dp_pcm_data_le;

    sbc_enc->a2dp_sbc_encoder_io.encoder_output_buffer = sbc_enc->a2dp_sbc_data;
    sbc_enc->a2dp_sbc_encoder_io.encoder_output_buflen = sbc_datalen;

    /* Do SBC Encoding */
    retval = sbc_encoder
             (
                 &sbc_enc->a2dp_sbc_encoder_io
             );

    if (API_SUCCESS != retval)
    {
        BT_DBG("SBC Encoder failed - 0x%X\r\n", retval);
        return retval;
    }

    /* Update SBC Frame Size */
    sbc_datalen = sbc_enc->a2dp_sbc_encoder_io.encoder_output_buflen;

    /* Write to A2DP */
#ifndef A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE
    retval = BT_a2dp_media_write
             (
                 ep_state->ethermind_a2dp_codec_index,
                 sbc_enc->a2dp_sbc_data,
                 sbc_datalen
             );
#else
    retval = BT_a2dp_media_write
             (
                 ep_state->ethermind_a2dp_codec_index,
                 1,
                 sbc_enc->a2dp_sbc_data,
                 sbc_datalen
             );
#endif /* A2DP_SUPPORT_MULTIPLE_MEDIA_FRAME_WRITE */
    if (API_SUCCESS != retval)
    {
        BT_DBG("A2DP media write failed - 0x%X\r\n", retval);
    }

    return retval;
}

static void edgefast_a2dp_src_write_task (struct bt_a2dp_endpoint_state *ep_state)
{
    int32_t  rd_ptr, index, remaining;
    uint16_t bytes_to_send, buf_index, encode_len;
    struct bt_a2dp_sbc_encoder *sbc_encoder = &ep_state->codec->sbc_encoder;

    for(;;)
    {
        if (APP_A2DP_SRC_WR_TH_INIT == sbc_encoder->a2dp_src_wr_th_state)
        {
            /* Wait for data in buffer */
            return;
            #if 0
            BT_thread_mutex_lock (&a2dp_src_th_mutex);
            BT_thread_cond_wait (&a2dp_src_th_cond, &a2dp_src_th_mutex);
            BT_thread_mutex_unlock (&a2dp_src_th_mutex);
            #endif
        }
        else if (APP_A2DP_SRC_WR_TH_STOP == sbc_encoder->a2dp_src_wr_th_state)
        {
            /* Exit from task */
            return;
        }

        //BT_thread_mutex_lock (&a2dp_src_th_mutex);
        EDGEFAST_A2DP_LOCK;
        if (sbc_encoder->a2dp_src_buffer_wr_ptr >= sbc_encoder->a2dp_src_buffer_rd_ptr)
        {
            /*
             *    ----------DDDDDDDDDDDDDDD--------------X
             *    ^         ^              ^             ^
             *    |         |              |             |
             *    0       rd_ptr         wr_ptr         max
             *
             *  Free Space => '-----------'
             */
            remaining = sbc_encoder->a2dp_src_buffer_wr_ptr - sbc_encoder->a2dp_src_buffer_rd_ptr;
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
            remaining = sbc_encoder->a2dp_src_buffer_size -
                    (sbc_encoder->a2dp_src_buffer_rd_ptr - sbc_encoder->a2dp_src_buffer_wr_ptr);
        }

        /* Do we really have anything to read ? */
        if (remaining < sbc_encoder->a2dp_pcm_datalen)
        {
            /* Wait for data in buffer */
            sbc_encoder->a2dp_src_wr_th_state = APP_A2DP_SRC_WR_TH_INIT;
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

        rd_ptr = sbc_encoder->a2dp_src_buffer_rd_ptr;

        bytes_to_send = (remaining > sbc_encoder->a2dp_pcm_datalen) ?
            sbc_encoder->a2dp_pcm_datalen : (uint16_t)remaining;

        for (index = 0; index < bytes_to_send; index++)
        {
            sbc_encoder->pcm_to_send[index] = sbc_encoder->a2dp_src_buffer[rd_ptr];

            rd_ptr = rd_ptr + 1;
            if (rd_ptr == sbc_encoder->a2dp_src_buffer_size)
            {
                rd_ptr = 0;
            }
        }

        /* Update the read pointer */
        sbc_encoder->a2dp_src_buffer_rd_ptr = rd_ptr;

        buf_index = 0;

        while (buf_index < bytes_to_send)
        {
            if ((bytes_to_send - buf_index) <= sbc_encoder->a2dp_pcm_datalen)
            {
                encode_len = bytes_to_send - buf_index;
            }
            else
            {
                encode_len = sbc_encoder->a2dp_pcm_datalen;
            }

            a2dp_encode_n_send
            (
                ep_state,
                sbc_encoder->pcm_to_send + buf_index,
                encode_len
            );

            buf_index += encode_len;
        }
    }
}
#endif

static void a2dp_process_task(osa_task_param_t arg)
{
    a2dp_task_msg_t message;
    struct bt_a2dp *a2dp = NULL;
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
            if (NULL == message.param)
            {
                continue;
            }

            EDGEFAST_A2DP_LOCK;
            if ((message.event != A2DP_EVENT_SEND_SBC_PCM_DATA) &&
                (message.event != A2DP_EVENT_RETRY_A2DP_CONNECT))
            {
                a2dp = (struct bt_a2dp *)message.param;
                if (a2dp->msg_command == A2DP_EVENT_INVALID)
                {
                    continue;
                }
                else
                {
                    message.event = a2dp->msg_command;
                    a2dp->msg_command = A2DP_EVENT_INVALID;
                }
            }
            switch (message.event)
            {
                case A2DP_EVENT_RETRY_DISCOVER:
                    BT_DBG("retry discover\r\n");
                    if (a2dp->retry_count > 0U)
                    {
                        a2dp->retry_count--;
                        retval = BT_avdtp_discover_req(&a2dp->ethermind_avdtp_handle,
                                        a2dp->discover_buf, A2DP_MAX_DISCOVER_RESPONSE_LEN);
                        if (API_SUCCESS != retval)
                        {
                            BT_DBG("a2dp discover request fail: %X\r\n", retval);
                            /*  retry */
                            a2dp_set_delay_work(a2dp, a2dp, A2DP_EVENT_RETRY_DISCOVER, 500U);
                        }
                    }
                    else
                    {
                        if (a2dp->auto_configure_enabled)
                        {
                            a2dp_auto_configure_callback_call(a2dp, NULL, -EIO);
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
                    if (a2dp->retry_count > 0U)
                    {
                        a2dp->retry_count--;
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
                        retval = BT_avdtp_get_all_capabilities_req
                             (
                                 &a2dp->ethermind_avdtp_handle,
                                 a2dp->sep_cap_buf,
                                 A2DP_GET_CAPABILITIES_RESPONSE_LEN + 10U
                             );
#else
                        retval = BT_avdtp_get_capabilities_req
                             (
                                 &a2dp->ethermind_avdtp_handle,
                                 a2dp->sep_cap_buf,
                                 A2DP_GET_CAPABILITIES_RESPONSE_LEN
                             );
#endif

                        if (API_SUCCESS != retval)
                        {
                            BT_DBG("BT_avdtp_get_capabilities_req fail 0x%X\r\n", retval);
                            a2dp_set_delay_work(a2dp, a2dp, A2DP_EVENT_RETRY_GET_CAPS, 500U);
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
                            if (a2dp->auto_configure_enabled)
                            {
                                a2dp_callback_auto_configure(a2dp);
                            }
                            else if (a2dp->discover_peer_endpoint_cb != NULL)
                            {
                                a2dp->discover_peer_endpoint_cb(a2dp, NULL, 0U);
                                a2dp->discover_peer_endpoint_cb = NULL;
                            }
                            else
                            {
                            }
                        }
                    }
                    break;

                case A2DP_EVENT_RETRY_A2DP_CONNECT:
                {
                    struct bt_a2dp_endpoint_state *ep_state = (struct bt_a2dp_endpoint_state *)message.param;
                    a2dp = ep_state->a2dp;
                    if (a2dp == NULL)
                    {
                        break;
                    }

                    if (a2dp->retry_count > 0U)
                    {
                        if (a2dp->configured_from_peer)
                        {
                            /* don't need retry */
                            a2dp->retry_count = 0U;
                            BT_DBG("wait peer a2dp connection\r\n");
                        }
                        else
                        {
                            BT_DBG("retry a2dp connecting\r\n");
                            a2dp->retry_count--;
                            retval = BT_a2dp_connect
                                    (
                                        ep_state->ethermind_a2dp_codec_index,
                                        a2dp->peer_addr,
                                        a2dp->ethermind_avdtp_handle.remote_seid,
                                        &a2dp->sep_conf
                                    );
                            if (API_SUCCESS != retval)
                            {
                                BT_DBG("BT_a2dp_connect fail 0x%X\r\n", retval);
                            }
                            else
                            {
                                BT_DBG("BT_a2dp_connect success, wait for A2DP_CONNECT_CNF\r\n");
#if ((defined(CONFIG_BT_A2DP_SOURCE)) && (CONFIG_BT_A2DP_SOURCE > 0U))
                                a2dp_set_a2dp_source_codec_encoder(ep_state->codec,
                                                                    ep_state->endpoint->codec_id,
                                                                    &a2dp->config_internal.codec_ie[0]);
#endif
                            }
                        }
                    }
                    else
                    {
                        if (a2dp->auto_configure_enabled)
                        {
                            a2dp_auto_configure_callback_call(a2dp, ep_state, -1);
                        }
                        else
                        {
                            a2dp_configure_ep_callback_call(a2dp, ep_state, -1);
                        }
                    }
                    break;
                }

#if ((defined(CONFIG_BT_A2DP_SOURCE)) && (CONFIG_BT_A2DP_SOURCE > 0U))
                case A2DP_EVENT_SEND_SBC_PCM_DATA:
                {
                    edgefast_a2dp_src_write_task((struct bt_a2dp_endpoint_state *)message.param);
                    break;
                }
#endif

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
        case AVDTP_GET_ALL_CAPABILITIES_CNF:
        case AVDTP_SET_CONFIGURATION_IND:
        case AVDTP_SET_CONFIGURATION_CNF:
            for (uint8_t index = 0; index < CONFIG_BT_A2DP_MAX_CONN; ++index)
            {
                if ((0U == memcmp(avdtp_handle->bd_addr, a2dp_instances[index].peer_addr, 6)) &&
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
            a2dp->a2dp_state = INTERNAL_STATE_AVDTP_CONNECTED;
            a2dp->connected_from_peer = 1U;
            /* avdtp_handle only have the addr */
            memcpy(a2dp->peer_addr, avdtp_handle->bd_addr, 6U);

            /* Initialize AVDTP Handle */
            AVDTP_INIT_HANDLE (a2dp->ethermind_avdtp_handle);
            /* Set AVDTP Handle Callback */
            AVDTP_SET_HANDLE_CALLBACK
            (
                a2dp->ethermind_avdtp_handle,
                ethermind_a2dp_avdtp_notify_cb
            );
            /* Set AVDTP Remote BD_ADDR */
            AVDTP_SET_HANDLE_BD_ADDR (a2dp->ethermind_avdtp_handle, avdtp_handle->bd_addr);
            a2dp_connectcallback_call(a2dp, A2DP_CB_CONNECTED, 0);
            break;

        case AVDTP_CONNECT_CNF:
            if (API_SUCCESS == event_result)
            {
                BT_DBG("enter INTERNAL_STATE_AVDTP_CONNECTED\r\n");
                a2dp->connected_from_peer = 0U;
                a2dp->configured_from_peer = 0U;
                a2dp->a2dp_state = INTERNAL_STATE_AVDTP_CONNECTED;
                callbackResult = 0;
            }
            else
            {
                BT_DBG("AVDTP_CONNECT_CNF error code:0x%X\r\n", event_result);
                callbackResult = -EIO;
            }

            a2dp_connectcallback_call(a2dp, A2DP_CB_CONNECTED, callbackResult);
            if (callbackResult != 0) {
                if (a2dp != NULL) {
                    a2dp_FreeInstance(a2dp);
                }
            }
            break;

        case AVDTP_DISCONNECT_IND:
        case AVDTP_DISCONNECT_CNF:
            if (a2dp->a2dp_state != INTERNAL_STATE_IDLE)
            {
                a2dp->a2dp_state = INTERNAL_STATE_IDLE;
            }
            a2dp_connectcallback_call(a2dp, A2DP_CB_DISCONNECTED, 0);
            a2dp_FreeInstance(a2dp);
            break;

        case AVDTP_DISCOVER_CNF:
            if (API_SUCCESS != event_result)
            {
                /* retry */
                a2dp_set_delay_work(a2dp, a2dp, A2DP_EVENT_RETRY_DISCOVER, 500U);
                BT_DBG("*** Discover RSP Error Code = 0x%X\r\n", event_result);
            }
            else
            {
                /* Update Info Len and start of info element pointer */
                a2dp->discover_buflen = event_datalen;
                a2dp->discover_buf_start = a2dp->discover_buf;
                if (a2dp_handle_avdtp_discover_rsp(a2dp) != 0)
                {
                    if (a2dp->auto_configure_enabled)
                    {
                        a2dp_callback_auto_configure(a2dp);
                    }
                    else if (a2dp->discover_peer_endpoint_cb != NULL)
                    {
                        a2dp->discover_peer_endpoint_cb(a2dp, NULL, 0U);
                        a2dp->discover_peer_endpoint_cb = NULL;
                    }
                }
            }
            break;

        case AVDTP_GET_CAPABILITIES_CNF:
        case AVDTP_GET_ALL_CAPABILITIES_CNF:
            if (API_SUCCESS != event_result)
            {
                a2dp_set_delay_work(a2dp, a2dp, A2DP_EVENT_RETRY_GET_CAPS, 500U);
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
                    if (a2dp->auto_configure_enabled)
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

            if (a2dp->auto_configure_enabled)
            {
                uint32_t count = a2dp_get_endpoints_count();
                for (uint32_t index = 0; index < count; index++)
                {
                    if ((a2dp_endpoint_states[index].endpoint->info.sep.tsep != a2dp->current_seid_info.tsep) &&
                        (a2dp_endpoint_states[index].endpoint->codec_id == sep_cap.codec_cap.codec_type))
                    {
                        if ((a2dp->select_peer_seid == 0) || (index < a2dp->select_peer_index))
                        {
                            a2dp->peer_seid_info = a2dp->current_seid_info;
                            a2dp->peer_codec_ie.len = sep_cap.codec_cap.codec_ie_len;
                            memcpy(&a2dp->peer_codec_ie.codec_ie[0], sep_cap.codec_cap.codec_ie,
                               ((sep_cap.codec_cap.codec_ie_len > BT_A2DP_CODEC_IE_LENGTH_MAX) ?
                               BT_A2DP_CODEC_IE_LENGTH_MAX : sep_cap.codec_cap.codec_ie_len));
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
                            a2dp->peer_cp_ie_count = 0;
                            for (uint8_t cp_index = 0; cp_index < AVDTP_MAX_CP_IE; cp_index++)
                            {
                                if (sep_cap.cp_cap[cp_index].cp_type != AVDTP_INVALID_CP_TYPE)
                                {
                                    a2dp->peer_cp_ie[cp_index].len = sep_cap.cp_cap[cp_index].cp_ie_len + 2;
                                    a2dp->peer_cp_ie_count++;
                                    a2dp->peer_cp_ie[cp_index].codec_ie[0] = (uint8_t)sep_cap.cp_cap[cp_index].cp_type;
                                    a2dp->peer_cp_ie[cp_index].codec_ie[1] = (uint8_t)(sep_cap.cp_cap[cp_index].cp_type >> 8U);
                                    if (sep_cap.cp_cap[cp_index].cp_ie_len > 0)
                                    {
                                        uint8_t copy_len = sep_cap.cp_cap[cp_index].cp_ie_len;
                                        if (copy_len > (BT_A2DP_CODEC_IE_LENGTH_MAX - 2))
                                        {
                                            copy_len = BT_A2DP_CODEC_IE_LENGTH_MAX - 2;
                                        }
                                        memcpy(&a2dp->peer_cp_ie[cp_index].codec_ie[2], sep_cap.cp_cap[cp_index].cp_ie,
                                            copy_len);
                                    }
                                }
                            }
#endif
#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))

                            if (sep_cap.recovery_cap.recovery_type != AVDTP_INVALID_RECOVERY_TYPE) {
                                a2dp->peer_recovery_ie.len = 3;
                                a2dp->peer_recovery_ie.codec_ie[0] = sep_cap.recovery_cap.recovery_type;
                                a2dp->peer_recovery_ie.codec_ie[1] = sep_cap.recovery_cap.mrws;
                                a2dp->peer_recovery_ie.codec_ie[2] = sep_cap.recovery_cap.mnmp;
                            } else {
                                a2dp->peer_recovery_ie.len = 0;
                            }
#endif
#if ((defined(CONFIG_BT_A2DP_REPORTING_SERVICE)) && (CONFIG_BT_A2DP_REPORTING_SERVICE > 0U))
                            /* ethermind doesn't have this label */
                            a2dp->peer_reporting_service = sep_cap.reporting_cap;
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
                                a2dp->peer_delay_reporting = sep_cap.dr_cap_flag;
#endif
#if ((defined(CONFIG_BT_A2DP_HC_SERVICE)) && (CONFIG_BT_A2DP_HC_SERVICE > 0U))
                            a2dp->peer_rohc_cap = sep_cap.rohc_cap;
#endif
#if ((defined(CONFIG_BT_A2DP_MULTIPLEXING_SERVICE)) && (CONFIG_BT_A2DP_MULTIPLEXING_SERVICE > 0U))
                            a2dp->peer_multiplexing = sep_cap.multiplexing_cap;
#endif
                            a2dp->select_peer_index = index;
                            a2dp->select_peer_seid = a2dp->current_seid_info.id;
                        }
                        break;
                    }
                }

                if (a2dp_handle_avdtp_discover_rsp(a2dp) != 0)
                {
                    a2dp_callback_auto_configure(a2dp);
                }
            }
            else if (a2dp->discover_peer_endpoint_cb != NULL)
            {
                /* callback to app by standalone callback */
                struct bt_a2dp_endpoint peer_endpoint;
                struct bt_a2dp_codec_ie_internal peer_endpoint_internal_cap;
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
                struct bt_a2dp_codec_ie_internal peer_cp_cap[AVDTP_MAX_CP_IE];
                uint8_t peer_cp_count;
#endif
#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))
                struct bt_a2dp_codec_ie_internal peer_recovery_cap;
#endif

                memset(&peer_endpoint, 0, sizeof (peer_endpoint));
                peer_endpoint.codec_id = sep_cap.codec_cap.codec_type;
                peer_endpoint.info.sep = a2dp->current_seid_info;
                peer_endpoint.config = NULL;
                peer_endpoint_internal_cap.len = sep_cap.codec_cap.codec_ie_len;
                memcpy(&peer_endpoint_internal_cap.codec_ie[0], sep_cap.codec_cap.codec_ie,
                        ((sep_cap.codec_cap.codec_ie_len > BT_A2DP_CODEC_IE_LENGTH_MAX) ? BT_A2DP_CODEC_IE_LENGTH_MAX : sep_cap.codec_cap.codec_ie_len));
                peer_endpoint.capabilities = (struct bt_a2dp_codec_ie *)&peer_endpoint_internal_cap;
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
                peer_cp_count = 0;
                for (uint8_t cp_index = 0; cp_index < AVDTP_MAX_CP_IE; cp_index++)
                {
                    if (sep_cap.cp_cap[cp_index].cp_type != AVDTP_INVALID_CP_TYPE)
                    {
                        peer_cp_cap[cp_index].len = sep_cap.cp_cap[cp_index].cp_ie_len + 2;
                        peer_cp_cap[cp_index].codec_ie[0] = (uint8_t)sep_cap.cp_cap[cp_index].cp_type;
                        peer_cp_cap[cp_index].codec_ie[1] = (uint8_t)(sep_cap.cp_cap[cp_index].cp_type >> 8U);
                        if (sep_cap.cp_cap[cp_index].cp_ie_len > 0)
                        {
                            uint8_t copy_len = sep_cap.cp_cap[cp_index].cp_ie_len;
                            if (copy_len > (BT_A2DP_CODEC_IE_LENGTH_MAX - 2))
                            {
                                copy_len = BT_A2DP_CODEC_IE_LENGTH_MAX - 2;
                            }
                            memcpy(&peer_cp_cap[cp_index].codec_ie[2], sep_cap.cp_cap[cp_index].cp_ie,
                                copy_len);
                        }
                        peer_cp_count++;
                    }
                }
                peer_endpoint.cp_ie_count = peer_cp_count;
                peer_endpoint.cp_ie = (struct bt_a2dp_codec_ie *)&peer_cp_cap[0];
#endif
#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))
                if (sep_cap.recovery_cap.recovery_type != AVDTP_INVALID_RECOVERY_TYPE) {
                    peer_recovery_cap.len = 3;
                    peer_recovery_cap.codec_ie[0] = sep_cap.recovery_cap.recovery_type;
                    peer_recovery_cap.codec_ie[1] = sep_cap.recovery_cap.mrws;
                    peer_recovery_cap.codec_ie[2] = sep_cap.recovery_cap.mnmp;
                    peer_endpoint.recovery_ie = (struct bt_a2dp_codec_ie *)&peer_recovery_cap;
                } else {
                    peer_endpoint.recovery_ie = NULL;
                }
#endif
#if ((defined(CONFIG_BT_A2DP_REPORTING_SERVICE)) && (CONFIG_BT_A2DP_REPORTING_SERVICE > 0U))
                peer_endpoint.reporting_service_enable = sep_cap.reporting_cap;
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
                peer_endpoint.delay_reporting_service_enable = sep_cap.dr_cap_flag;
#endif
#if ((defined(CONFIG_BT_A2DP_HC_SERVICE)) && (CONFIG_BT_A2DP_HC_SERVICE > 0U))
                peer_endpoint.header_compression_cap = sep_cap.rohc_cap;
#endif
#if ((defined(CONFIG_BT_A2DP_MULTIPLEXING_SERVICE)) && (CONFIG_BT_A2DP_MULTIPLEXING_SERVICE > 0U))
                peer_endpoint.multiplexing_service_enable = sep_cap.multiplexing_cap;
#endif

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
            a2dp->configured_from_peer = 0U;
            break;

        case AVDTP_SET_CONFIGURATION_IND:
            PRINTF("Noteict AVDTP_SET_CONFIGURATION_IND!!!!!\r\n");
            a2dp->configured_from_peer = 1U;
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
    struct bt_a2dp_endpoint_state *ep_state;
    uint8_t index;

    BT_DBG("a2dp cb:%x-%x\r\n", event_type, event_result);
    ep_state = bt_a2dp_get_ethermind_endpoint_state(codec_instance);
    a2dp = ep_state->a2dp;
    if (ep_state == NULL)
    {
        return API_SUCCESS;
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
            if (a2dp->auto_configure_enabled)
            {
                BT_DBG("Received A2DP_CONNECT_CNF\r\n");
                BT_DBG("    Codec Instance = %d\r\n", codec_instance);
                BT_DBG("    Event Result   = 0x%X\r\n", event_result);
                if (API_SUCCESS != event_result)
                {
                    a2dp_set_delay_work(a2dp, ep_state, A2DP_EVENT_RETRY_A2DP_CONNECT, 500U);
                }
                else
                {
                    a2dp->a2dp_state = INTERNAL_STATE_CONFIGURED_OPENED;
                    //a2dp->internal_selected_endpoint->config_result = (struct bt_a2dp_preset*)&a2dp->config_internal;
                    ep_state->config_internal = a2dp->config_internal;
                    a2dp_auto_configure_callback_call(a2dp, ep_state, 0);
                }
            }
            else
            {
                a2dp->a2dp_state = INTERNAL_STATE_CONFIGURED_OPENED;
                //a2dp->internal_selected_endpoint->config_result = (struct bt_a2dp_preset*)&a2dp->config_internal;
                ep_state->config_internal = a2dp->config_internal;
                a2dp_configure_ep_callback_call(a2dp, ep_state, event_result);
            }

            if (event_result == API_SUCCESS)
            {
#if ((defined(CONFIG_BT_A2DP_SINK)) && (CONFIG_BT_A2DP_SINK > 0U))
                if (BT_A2DP_SINK == ep_state->endpoint->info.sep.tsep)
                {
                    if (ep_state->endpoint->codec_id == BT_A2DP_SBC)
                    {
                        /* only for a2dp sink */
                        BT_jpl_init(jpl_callback_handle);
#ifdef JPL_USE_APP_MEMORY
                        JPL_BUFFERS buffer;
                        buffer.sbc = ep_state->codec->sbc_decoder.buf->a2dp_sbc_buffer;
                        buffer.pcm = ep_state->codec->sbc_decoder.buf->a2dp_pcm_buffer;
                        buffer.silence = ep_state->codec->sbc_decoder.buf->a2dp_silence_buffer;
                        /* Register memory to JPL */
                        BT_jpl_register_buffers(&buffer);
#endif /* JPL_USE_APP_MEMORY */
                        a2dp_jpl_configure(&ep_state->codec->sbc_decoder, &ep_state->config_internal.codec_ie[0]);
                    }
                }
#endif
            }
            else
            {
                ep_state->a2dp = NULL;
            }
            break;

        case A2DP_CONNECT_IND:
            if (API_SUCCESS != event_result)
            {
                break;
            }

            if (a2dp == NULL)
            {
                for (index = 0; index < CONFIG_BT_A2DP_MAX_CONN; index++)
                {
                    if (a2dp_instances[index].connected_from_peer)
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

            a2dp->configured_from_peer = 1U;
            ep_state->a2dp = a2dp;
            if (BT_A2DP_SOURCE == ep_state->endpoint->info.sep.tsep)
            {
#if ((defined(CONFIG_BT_A2DP_SOURCE)) && (CONFIG_BT_A2DP_SOURCE > 0U))
                /* keep all the retry, disconnect a2dp, the retry will do re-connect again. */
                a2dp->a2dp_state = INTERNAL_STATE_DISCONNECT_CONNECT_AGAIN;
                retval = BT_a2dp_disconnect(ep_state->ethermind_a2dp_codec_index);
                if (API_SUCCESS != retval)
                {
                    BT_DBG("BT_a2dp_disconnect fail 0x%X\r\n", retval);
                }
                else
                {
                    BT_DBG("BT_a2dp_disconnect success, wait for A2DP_DISCONNECT_CNF\r\n");
                }
#endif
            }
#if ((defined(CONFIG_BT_A2DP_SINK)) && (CONFIG_BT_A2DP_SINK > 0U))
            else
            {
                a2dp->a2dp_state = INTERNAL_STATE_CONFIGURED_OPENED;
                if (ep_state->endpoint->codec_id == BT_A2DP_SBC)
                {
                    /* only for a2dp sink */
                    BT_jpl_init(jpl_callback_handle);
#ifdef JPL_USE_APP_MEMORY
                    JPL_BUFFERS buffer;
                    buffer.sbc = ep_state->codec->sbc_decoder.buf->a2dp_sbc_buffer;
                    buffer.pcm = ep_state->codec->sbc_decoder.buf->a2dp_pcm_buffer;
                    buffer.silence = ep_state->codec->sbc_decoder.buf->a2dp_silence_buffer;
                    /* Register memory to JPL */
                    BT_jpl_register_buffers(&buffer);
#endif /* JPL_USE_APP_MEMORY */
                    a2dp_jpl_configure(&ep_state->codec->sbc_decoder, &ep_state->config_internal.codec_ie[0]);
                }
            }
#endif
            break;

        case A2DP_DISCONNECT_CNF:
            ep_state->a2dp = NULL;
            /* for the case that headset connects to board, we disconnect then connect */
            if (a2dp->a2dp_state == INTERNAL_STATE_DISCONNECT_CONNECT_AGAIN)
            {
                a2dp->configured_from_peer = 0U;
                a2dp->a2dp_state = INTERNAL_STATE_AVDTP_CONNECTED;
                /* the retry will connect again */
                /* if the retry has fail and done. */
                if (a2dp->auto_configure_enabled)
                {
                    a2dp->retry_count = 0U;
                    k_work_cancel_delayable(&a2dp->retry_work);
                    if (a2dp->discover_done)
                    {
                        a2dp->discover_done = 0U;
                        a2dp->retry_count = A2DP_TRANSFER_RETRY_COUNT;
                        a2dp_set_delay_work(a2dp, ep_state, A2DP_EVENT_RETRY_A2DP_CONNECT, 0U);
                    }
                    else
                    {
                        bt_a2dp_configure(a2dp, a2dp->configure_cb);
                    }
                }
            }
            else
            {
                if (a2dp->a2dp_state != INTERNAL_STATE_IDLE)
                {
                    if (a2dp->a2dp_state != INTERNAL_STATE_DISCONNECTING)
                    {
                        a2dp->a2dp_state = INTERNAL_STATE_AVDTP_CONNECTED;
                    }
#if ((defined(CONFIG_BT_A2DP_SINK)) && (CONFIG_BT_A2DP_SINK > 0U))
                    if ((BT_A2DP_SINK == ep_state->endpoint->info.sep.tsep) &&
                        (BT_A2DP_SBC == ep_state->endpoint->codec_id))
                    {
                        BT_jpl_stop();
                    }
#endif

                    if (ep_state->endpoint->control_cbs.deconfigured != NULL)
                    {
                        if (API_SUCCESS != event_result)
                        {
                            ep_state->endpoint->control_cbs.deconfigured(-1);
                        }
                        else
                        {
                            ep_state->endpoint->control_cbs.deconfigured(0);
                        }
                    }

                    if (a2dp->a2dp_state == INTERNAL_STATE_DISCONNECTING)
                    {
                        a2dp->a2dp_state = INTERNAL_STATE_IDLE;
                        /* disconnect avdtp. */
                        retval = BT_avdtp_disconnect_req(&a2dp->ethermind_avdtp_handle);
    #if ((defined CONFIG_BT_DEBUG_A2DP) && (CONFIG_BT_DEBUG_A2DP))
                        if (API_SUCCESS != retval)
                        {
                            BT_DBG("BT_avdtp_disconnect_req fail 0x%X\r\n", retval);
                        }
                        else
                        {
                            BT_DBG("BT_avdtp_disconnect_req success, wait for AVDTP_CONNECT_CNF\r\n");
                        }
    #else
                        (void)retval;
    #endif
                    }
                }
            }
            break;

        case A2DP_DISCONNECT_IND:
            ep_state->a2dp = NULL;
            a2dp->configured_from_peer = 0U;
            if (a2dp->a2dp_state != INTERNAL_STATE_IDLE)
            {
                a2dp->a2dp_state = INTERNAL_STATE_AVDTP_CONNECTED;
#if ((defined(CONFIG_BT_A2DP_SINK)) && (CONFIG_BT_A2DP_SINK > 0U))
                if ((BT_A2DP_SINK == ep_state->endpoint->info.sep.tsep) &&
                    (BT_A2DP_SBC == ep_state->endpoint->codec_id))
                {
                    BT_jpl_stop();
                }
#endif

                if (ep_state->endpoint->control_cbs.deconfigured != NULL)
                {
                    if (API_SUCCESS != event_result)
                    {
                        ep_state->endpoint->control_cbs.deconfigured(-1);
                    }
                    else
                    {
                        ep_state->endpoint->control_cbs.deconfigured(0);
                    }
                }
            }
            break;

#if ((defined(CONFIG_BT_A2DP_SINK)) && (CONFIG_BT_A2DP_SINK > 0U))
        case A2DP_MEDIA_FRAME_IND:
        {
            uint8_t offset;

            if (ep_state->endpoint->codec_id == BT_A2DP_SBC)
            {
                uint32_t timestamp;
                uint8_t *data;
                uint16_t seq_number;

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

                offset = 12U;
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
                if (ep_state->cp_header_len > 0U) {
                    /* todo: the decrypt may be need in furture. */
                    offset += ep_state->cp_header_len;
                }
#endif
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
                             (data + offset),
                             (event_datalen - offset)
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
                a2dp_control_ind_callback_call(ep_state, A2DP_CONTROL_SINK_STREAMER_DATA, 0, &streamerData);
            }
            break;
        }
#endif

        case A2DP_START_CNF:
        case A2DP_START_IND:
            a2dp_control_ind_callback_call(ep_state, A2DP_CONTROL_START_PLAY, 0, NULL);
#if ((defined(CONFIG_BT_A2DP_SINK)) && (CONFIG_BT_A2DP_SINK > 0U))
            if (BT_A2DP_SINK == ep_state->endpoint->info.sep.tsep)
            {
                if (BT_A2DP_SBC == ep_state->endpoint->codec_id)
                {
                    jpl_active_ep = ep_state;
                    /* Start JPL */
                     BT_jpl_start(&ep_state->codec->sbc_decoder.jpl_param);
                }
            }
#endif
            break;

        case A2DP_SUSPEND_CNF:    
#if ((defined(CONFIG_BT_A2DP_SINK)) && (CONFIG_BT_A2DP_SINK > 0U))
            if (BT_A2DP_SINK == ep_state->endpoint->info.sep.tsep)
            {
                if (BT_A2DP_SBC == ep_state->endpoint->codec_id)
                {
                    BT_jpl_stop();
                }
            }
#endif
            a2dp_control_ind_callback_call(ep_state, A2DP_CONTROL_STOP_PLAY, 0, NULL);
            break;

        case A2DP_SUSPEND_IND:
#if ((defined(CONFIG_BT_A2DP_SINK)) && (CONFIG_BT_A2DP_SINK > 0U))
            if (BT_A2DP_SINK == ep_state->endpoint->info.sep.tsep)
            {
                if (BT_A2DP_SBC == ep_state->endpoint->codec_id)
                {
                    BT_jpl_stop();
                }
            }
#endif
            a2dp_control_ind_callback_call(ep_state, A2DP_CONTROL_STOP_PLAY, 0, NULL);
            break;

        case A2DP_RECONFIGURE_CNF:
            callbackRet = API_SUCCESS;
            a2dp->a2dp_state = INTERNAL_STATE_CONFIGURED_OPENED;
            ep_state->config_internal = a2dp->config_internal;
            a2dp_configure_ep_callback_call(a2dp, ep_state, 0);
            break;

        case A2DP_CONFIGURE_IND:
        {
            /* A2DP_CONNECT_IND has the configuration information */
            A2DP_DEVICE_INFO * a2dp_dev_info = (A2DP_DEVICE_INFO *)event_data;

            if (a2dp == NULL)
            {
                for (index = 0; index < CONFIG_BT_A2DP_MAX_CONN; index++)
                {
                    if (a2dp_instances[index].connected_from_peer)
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

            ep_state->a2dp = a2dp;
#if 0
            if (a2dp->auto_configure_enabled != 0)
            {
                if (BT_A2DP_SOURCE == ep_state->endpoint->info.sep.tsep)
                {
#if ((defined(CONFIG_BT_A2DP_SOURCE)) && (CONFIG_BT_A2DP_SOURCE > 0U))
                    a2dp->configured_from_peer = 1U;
                    /* don't need do anything. */
                    callbackRet = API_SUCCESS;
#endif
                    break;
                }
            }
#endif
            /* Codec Configuration */
            ep_state->config_internal.len = a2dp_dev_info->codec_ie_len;
            memcpy
            (
                &ep_state->config_internal.codec_ie[0],
                a2dp_dev_info->codec_ie_conf,
                a2dp_dev_info->codec_ie_len
            );

#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
            if (a2dp_dev_info->cp_conf.cp_type != AVDTP_INVALID_CP_TYPE)
            {
                ep_state->cp_config.len = a2dp_dev_info->cp_conf.cp_ie_len + 2;
                ep_state->cp_config.codec_ie[0] = (uint8_t)a2dp_dev_info->cp_conf.cp_type;
                ep_state->cp_config.codec_ie[1] = (uint8_t)(a2dp_dev_info->cp_conf.cp_type >> 8U);
                if (a2dp_dev_info->cp_conf.cp_ie_len > 0)
                {
                    uint8_t copy_len = a2dp_dev_info->cp_conf.cp_ie_len;
                    if (copy_len > (BT_A2DP_CODEC_IE_LENGTH_MAX - 2))
                    {
                        copy_len = BT_A2DP_CODEC_IE_LENGTH_MAX - 2;
                    }
                    memcpy(&ep_state->cp_config.codec_ie[2], a2dp_dev_info->cp_conf.cp_ie, copy_len);
                }
            }
            else
            {
                ep_state->cp_config.len = 0;
            }
#endif
#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))
            if (a2dp_dev_info->recovery_conf.recovery_type != AVDTP_INVALID_RECOVERY_TYPE)
            {
                ep_state->recovery_config.len = 3u;
                ep_state->recovery_config.codec_ie[0] = a2dp_dev_info->recovery_conf.recovery_type;
                ep_state->recovery_config.codec_ie[1] = a2dp_dev_info->recovery_conf.mrws;
                ep_state->recovery_config.codec_ie[2] = a2dp_dev_info->recovery_conf.mnmp;
            }
            else
            {
                ep_state->recovery_config.len = 0u;
            }
#endif
#if ((defined(CONFIG_BT_A2DP_REPORTING_SERVICE)) && (CONFIG_BT_A2DP_REPORTING_SERVICE > 0U))
            ep_state->reporting_service_config = a2dp_dev_info->reporting_conf;
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
            ep_state->delay_reporting_config = a2dp_dev_info->dr_config_flag;
#endif
#if ((defined(CONFIG_BT_A2DP_HC_SERVICE)) && (CONFIG_BT_A2DP_HC_SERVICE > 0U))
            ep_state->rohc_config = a2dp_dev_info->rohc_conf;
#endif
#if ((defined(CONFIG_BT_A2DP_MULTIPLEXING_SERVICE)) && (CONFIG_BT_A2DP_MULTIPLEXING_SERVICE > 0U))
            ep_state->multiplexing_config = a2dp_dev_info->multiplexing_conf;
#endif

            if (BT_A2DP_SOURCE == ep_state->endpoint->info.sep.tsep)
            {
#if ((defined(CONFIG_BT_A2DP_SOURCE)) && (CONFIG_BT_A2DP_SOURCE > 0U))
                a2dp->configured_from_peer = 1U;
                /* don't need do anything. */
                callbackRet = API_SUCCESS;
#endif
            }

            a2dp_control_ind_callback_call(ep_state, A2DP_CONTROL_CONFIGURED, 0, NULL);
            break;
        }

#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
        case A2DP_DELAY_REPORT_CNF:
            if (ep_state->endpoint->control_cbs.sink_delay_report_cb != NULL)
            {
                if (event_result == API_SUCCESS)
                {
                    ep_state->endpoint->control_cbs.sink_delay_report_cb(0);
                }
                else
                {
                    ep_state->endpoint->control_cbs.sink_delay_report_cb(-1);
                }
            }
            break;

        case A2DP_DELAY_REPORT_IND:
            if (ep_state->endpoint->control_cbs.source_delay_report_cb != NULL)
            {
                if (event_result == API_SUCCESS)
                {
                    int16_t delay = (int16_t)(((uint8_t *)event_data)[0] << 8U | ((uint8_t *)event_data)[1]);
                    ep_state->endpoint->control_cbs.source_delay_report_cb(0, delay);
                }
                else
                {
                    ep_state->endpoint->control_cbs.source_delay_report_cb(-1, 0);
                }
            }
            break;
#endif

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

    for (uint8_t index = 0; index < CONFIG_BT_A2DP_MAX_CONN; ++index)
    {
        a2dp_instances[index].allocated = 0U;
        k_work_init_delayable(&a2dp_instances[index].retry_work, a2dp_retry_work_timeout);
    }

    memset(&a2dp_endpoint_states[0], 0, sizeof (a2dp_endpoint_states));

    BT_avdtp_callback_register(ethermind_a2dp_avdtp_notify_cb);

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

    memset(&sep_cap, 0, sizeof(sep_cap));
    /* Initialize Stream End Point Capability */
    AVDTP_SET_SEP_CAPABILITY
    (
        sep_cap,
        ((BT_A2DP_SOURCE == role) ? AVDTP_SEP_SOURCE : AVDTP_SEP_SINK),
        (uint8_t)media_type,
        endpoint->codec_id,
        endpoint->capabilities->len,
        endpoint->capabilities->codec_ie
    );
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
    if ((endpoint->cp_ie_count != 0) && (endpoint->cp_ie != NULL))
    {
        uint16_t type;
        for (index = 0; index < endpoint->cp_ie_count; ++index)
        {
            type = (uint16_t)(endpoint->cp_ie[index].codec_ie[0]) | ((uint16_t)endpoint->cp_ie[index].codec_ie[1] << 8U);
            AVDTP_SET_SEP_CP_CAPABILITY
            (
                (sep_cap.cp_cap[0]),
                type,
                endpoint->cp_ie[0].codec_ie,
                endpoint->cp_ie[0].len - 2
            );
        }
    }
#endif
#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))
    if (endpoint->recovery_ie != NULL)
    {
        AVDTP_SET_SEP_RECOVERY_CAPABILITY
        (
            sep_cap,
            endpoint->recovery_ie->codec_ie[0],
            endpoint->recovery_ie->codec_ie[1],
            endpoint->recovery_ie->codec_ie[2]
        );
    }
#endif
#if ((defined(CONFIG_BT_A2DP_REPORTING_SERVICE)) && (CONFIG_BT_A2DP_REPORTING_SERVICE > 0U))
#ifndef AVDTP_HAVE_REPORTING_SERVICE
    return -EIO;
#endif
    if (endpoint->reporting_service_enable == 1)
    {
        sep_cap.reporting_cap = 1u;
    }
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
    if (endpoint->delay_reporting_service_enable)
    {
        sep_cap.dr_cap_flag = BT_TRUE;
    }
    else
    {
        sep_cap.dr_cap_flag = BT_FALSE;
    }
#endif
#if ((defined(CONFIG_BT_A2DP_HC_SERVICE)) && (CONFIG_BT_A2DP_HC_SERVICE > 0U))
    sep_cap.rohc_cap = endpoint->header_compression_cap;
#endif
#if ((defined(CONFIG_BT_A2DP_MULTIPLEXING_SERVICE)) && (CONFIG_BT_A2DP_MULTIPLEXING_SERVICE > 0U))
    sep_cap.multiplexing_cap = endpoint->multiplexing_service_enable;
#endif

    for (index = 0; index < CONFIG_BT_A2DP_MAX_ENDPOINT_COUNT; ++index)
    {
        if (a2dp_endpoint_states[index].endpoint == NULL)
        {
            if (endpoint->codec_buffer != NULL)
            {
                a2dp_endpoint_states[index].codec = (struct bt_a2dp_codec_state *)&endpoint->codec_buffer[0];
            }

#if ((defined(CONFIG_BT_A2DP_SINK)) && (CONFIG_BT_A2DP_SINK > 0U))
            if ((endpoint->codec_id == BT_A2DP_SBC) && (role == BT_A2DP_SINK))
            {
                a2dp_endpoint_states[index].codec->sbc_decoder.buf =
                          (struct bt_a2dp_sbc_decoder_buf *)&endpoint->codec_buffer_nocached[0];
            }
#endif
            a2dp_endpoint_states[index].endpoint = endpoint;
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
            a2dp_endpoint_states[index].cp_header_len = 0u;
#endif
            retval = BT_a2dp_register_codec
                    (
                        &a2dp_endpoint_states[index].ethermind_a2dp_codec_index,
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

    if (index >= CONFIG_BT_A2DP_MAX_ENDPOINT_COUNT)
    {
        return -EIO;
    }

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

    a2dp->a2dp_state = INTERNAL_STATE_IDLE;

    /* source and sink have same codes
        * start avdtp connection
        * Initialize AVDTP Handle */
    AVDTP_INIT_HANDLE (a2dp->ethermind_avdtp_handle);
    /* Set AVDTP Handle Callback */
    AVDTP_SET_HANDLE_CALLBACK
    (
        a2dp->ethermind_avdtp_handle,
        ethermind_a2dp_avdtp_notify_cb
    );

    (void)memset(&info, 0, sizeof(info));
    bt_conn_get_info(conn, &info);
    if (info.type == BT_CONN_TYPE_LE)
    {
        a2dp_FreeInstance(a2dp);
        return NULL;
    }
    /* Set AVDTP Remote BD_ADDR */
    AVDTP_SET_HANDLE_BD_ADDR
    (
        a2dp->ethermind_avdtp_handle,
        (uint8_t*)info.br.dst
    );
    memcpy(a2dp->peer_addr, info.br.dst, BT_BD_ADDR_SIZE);
    /* AVDTP Connect */
    retval = BT_avdtp_connect_req
            (
                &a2dp->ethermind_avdtp_handle
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

int bt_a2dp_disconnect(struct bt_a2dp *a2dp)
{
    uint8_t index;
    API_RESULT retval;

    /* disconnect endpoint */
    for (index = 0; index < CONFIG_BT_A2DP_MAX_ENDPOINT_COUNT; ++index)
    {
        if ((a2dp_endpoint_states[index].a2dp == a2dp) &&
            (a2dp_endpoint_states[index].endpoint != NULL) &&
            ((a2dp_endpoint_states[index].a2dp->a2dp_state == INTERNAL_STATE_AVDTP_CONNECTED) ||
             (a2dp_endpoint_states[index].a2dp->a2dp_state == INTERNAL_STATE_CONFIGURED_OPENED)))
        {
            retval = BT_a2dp_disconnect(a2dp_endpoint_states[index].ethermind_a2dp_codec_index);
            if (API_SUCCESS == retval)
            {
                a2dp_endpoint_states[index].a2dp->a2dp_state = INTERNAL_STATE_DISCONNECTING;
                /* wait callback */
                return 0;
            }
            break;
        }
    }

    /* disconnect avdtp. */
    retval = BT_avdtp_disconnect_req(&a2dp->ethermind_avdtp_handle);
    if (API_SUCCESS != retval)
    {
#if ((defined CONFIG_BT_DEBUG_A2DP) && (CONFIG_BT_DEBUG_A2DP))
        BT_DBG("BT_avdtp_disconnect_req fail 0x%X\r\n", retval);
#endif
        return -EIO;
    }
#if ((defined CONFIG_BT_DEBUG_A2DP) && (CONFIG_BT_DEBUG_A2DP))
    else
    {
        BT_DBG("BT_avdtp_disconnect_req success, wait for AVDTP_CONNECT_CNF\r\n");
    }
#endif

    return 0;
}

int bt_a2dp_configure(struct bt_a2dp *a2dp, void (*result_cb)(int err))
{
    API_RESULT retval;

    if (a2dp == NULL)
    {
        return -EIO;
    }

    if (a2dp->a2dp_state != INTERNAL_STATE_AVDTP_CONNECTED)
    {
        return -EIO;
    }

    a2dp->auto_configure_enabled = 1U;
    a2dp->configure_cb = result_cb;

    BT_DBG("enter INTERNAL_STATE_AVDTP_CONNECTED\r\n");
    a2dp->retry_count = A2DP_TRANSFER_RETRY_COUNT;
    retval = BT_avdtp_discover_req(&a2dp->ethermind_avdtp_handle, a2dp->discover_buf, A2DP_MAX_DISCOVER_RESPONSE_LEN);
    if (API_SUCCESS != retval)
    {
        BT_DBG("a2dp discover request fail: %X\r\n", retval);
        a2dp_set_delay_work(a2dp, a2dp, A2DP_EVENT_RETRY_DISCOVER, 500U);
    }
    else
    {
        BT_DBG("wait AVDTP_DISCOVER_CNF\r\n");
    }

    return 0;
}

#if ((defined(CONFIG_BT_A2DP_SOURCE)) && (CONFIG_BT_A2DP_SOURCE > 0U))
int bt_a2dp_start(struct bt_a2dp_endpoint *endpoint)
{
    API_RESULT retval;
    uint16_t acl_handle;
    struct bt_a2dp_endpoint_state *ep_state;

    if (endpoint == NULL)
    {
        return -EINVAL;
    }
    ep_state = bt_a2dp_get_endpoint_state(endpoint);
    if ((ep_state == NULL) || (ep_state->a2dp == NULL))
    {
        return -EINVAL;
    }

    //ep_state->a2dp->source_start_play = cb;
    if (endpoint->info.sep.tsep == BT_A2DP_SOURCE)
    {
        ep_state->codec->sbc_encoder.a2dp_src_wr_th_state = APP_A2DP_SRC_WR_TH_INIT;
        ep_state->codec->sbc_encoder.a2dp_src_buffer_size = A2DP_SRC_MAX_BUFFER_SIZE;
        ep_state->codec->sbc_encoder.a2dp_src_buffer_rd_ptr = 0U;
        ep_state->codec->sbc_encoder.a2dp_src_buffer_wr_ptr = 0U;
    }

    retval = BT_a2dp_start (ep_state->ethermind_a2dp_codec_index);
    if (API_SUCCESS == retval)
    {
        /* Unsniff the link if required */
        BT_hci_get_acl_connection_handle(ep_state->a2dp->peer_addr, &acl_handle);
        BT_hci_exit_sniff_mode(acl_handle);
    }
    else
    {
        return -EIO;
    }

    return 0;
}

int bt_a2dp_stop(struct bt_a2dp_endpoint *endpoint)
{
    API_RESULT retval;
    struct bt_a2dp_endpoint_state *ep_state;

    if (endpoint == NULL)
    {
        return -EINVAL;
    }
    ep_state = bt_a2dp_get_endpoint_state(endpoint);
    if ((ep_state == NULL) || (ep_state->a2dp == NULL))
    {
        return -EINVAL;
    }

    //ep_state->a2dp->source_suspend_play = cb;
    retval = BT_a2dp_suspend (ep_state->ethermind_a2dp_codec_index);
    if (API_SUCCESS != retval)
    {
        return -EIO;
    }

    return 0;
}

static void a2dp_src_enqueue
           (
               struct bt_a2dp_endpoint *endpoint,
               uint8_t *data,
               uint16_t datalen
           )
{
    int32_t  n_free;
    uint32_t count;
    struct bt_a2dp_endpoint_state *ep_state;
    struct bt_a2dp_sbc_encoder *sbc_encoder;

    ep_state = bt_a2dp_get_endpoint_state(endpoint);
    if (ep_state == NULL)
    {
        return;
    }
    sbc_encoder = &ep_state->codec->sbc_encoder;
    /*
     *  Get amount of free buffer space.
     */
    if (sbc_encoder->a2dp_src_buffer_wr_ptr >= sbc_encoder->a2dp_src_buffer_rd_ptr)
    {
        /*
         *    ----------DDDDDDDDDDDDDDD--------------X
         *    ^         ^              ^             ^
         *    |         |              |             |
         *    0       rd_ptr         wr_ptr         max
         *
         *  Free Space => '-----------'
         */
        n_free = (sbc_encoder->a2dp_src_buffer_size - sbc_encoder->a2dp_src_buffer_wr_ptr)
                + sbc_encoder->a2dp_src_buffer_rd_ptr;
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
        n_free =sbc_encoder->a2dp_src_buffer_rd_ptr - sbc_encoder->a2dp_src_buffer_wr_ptr;
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
        sbc_encoder->a2dp_src_buffer[sbc_encoder->a2dp_src_buffer_wr_ptr] = data[count];
        sbc_encoder->a2dp_src_buffer_wr_ptr++;

        if(sbc_encoder->a2dp_src_buffer_wr_ptr == sbc_encoder->a2dp_src_buffer_size)
        {
            sbc_encoder->a2dp_src_buffer_wr_ptr = 0;
        }
    }

    EDGEFAST_A2DP_LOCK;
    if (APP_A2DP_SRC_WR_TH_INIT == sbc_encoder->a2dp_src_wr_th_state)
    {
        /* Signal the waiting thread */
        sbc_encoder->a2dp_src_wr_th_state = APP_A2DP_SRC_WR_TH_PLAYING;
        //a2dp_set_delay_work(ep_state, A2DP_EVENT_SEND_SBC_PCM_DATA, 0U);
        a2dp_send_task_msg(ep_state, A2DP_EVENT_SEND_SBC_PCM_DATA);
        EDGEFAST_A2DP_UNLOCK;
    }
    else
    {
        EDGEFAST_A2DP_UNLOCK;
    }

    return;
}

int bt_a2dp_src_media_write(struct bt_a2dp_endpoint *endpoint,
                            uint8_t *data, uint16_t datalen)
{
    if ((endpoint == NULL) || (endpoint->info.sep.tsep != BT_A2DP_SOURCE))
    {
        return -EIO;
    }

    if (endpoint->codec_id == BT_A2DP_SBC)
    {
        a2dp_src_enqueue(endpoint, data, datalen);
    }
    else
    {
        //todo: other codec encoded data sending.
    }
    return 0;
}
#endif

#if ((defined(CONFIG_BT_A2DP_SINK)) && (CONFIG_BT_A2DP_SINK > 0U))
int bt_a2dp_snk_media_sync(struct bt_a2dp_endpoint *endpoint,
                            uint8_t *data, uint16_t datalen)
{
    struct bt_a2dp_endpoint_state *ep_state;
    struct bt_a2dp_sbc_decoder *sbc_decoder;

    if (endpoint == NULL)
    {
        return -EINVAL;
    }
    ep_state = bt_a2dp_get_endpoint_state(endpoint);
    if (ep_state == NULL)
    {
        return -EINVAL;
    }
    sbc_decoder = &ep_state->codec->sbc_decoder;

    if (endpoint->codec_id == BT_A2DP_SBC)
    {
        if (data == NULL)
        {
            BT_IGNORE_UNUSED_PARAM(data);
            BT_IGNORE_UNUSED_PARAM(datalen);
            BT_jpl_remove_frames(ep_state->buffer_points[ep_state->buffer_consume], sbc_decoder->a2dp_jpl_poflen);
        }
        else
        {
            BT_jpl_remove_frames(data, datalen);
        }

        ep_state->buffer_consume++;
        if (ep_state->buffer_consume == (sizeof (ep_state->buffer_points) / sizeof (ep_state->buffer_points[0])))
        {
            ep_state->buffer_consume = 0u;
        }
    }

    return 0;
}
#endif

int bt_a2dp_deconfigure(struct bt_a2dp_endpoint *endpoint)
{
    API_RESULT retval;
    struct bt_a2dp_endpoint_state * ep_state;

    if (endpoint == NULL)
    {
        return -EINVAL;
    }

    ep_state = bt_a2dp_get_endpoint_state(endpoint);
    if ((ep_state == NULL) || (ep_state->a2dp == NULL))
    {
        return -EINVAL;
    }
    //ep_state->a2dp->de_configure_cb = cb;
    retval = BT_a2dp_disconnect(ep_state->ethermind_a2dp_codec_index);
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

    if ((a2dp->a2dp_state != INTERNAL_STATE_AVDTP_CONNECTED) &&
        (a2dp->a2dp_state != INTERNAL_STATE_CONFIGURED_OPENED))
    {
        return -EIO;
    }

    a2dp->discover_peer_endpoint_cb = cb;
    retval = BT_avdtp_discover_req(&a2dp->ethermind_avdtp_handle, a2dp->discover_buf, A2DP_MAX_DISCOVER_RESPONSE_LEN);
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
        struct bt_a2dp_endpoint *peer_endpoint,
        struct bt_a2dp_endpoint_config *config)
{
    API_RESULT retval;
    struct bt_a2dp_endpoint_state *ep_state;

    if ((a2dp == NULL) || (endpoint == NULL) || (peer_endpoint == NULL) || (config == NULL))
    {
        return -EINVAL;
    }

    if (a2dp->a2dp_state != INTERNAL_STATE_AVDTP_CONNECTED)
    {
        return -EINVAL;
    }

    ep_state = bt_a2dp_get_endpoint_state(endpoint);
    if (ep_state == NULL)
    {
        return -EINVAL;
    }

    ep_state->a2dp->config_internal.len = config->media_config->len;
    memcpy
    (
        &ep_state->a2dp->config_internal.codec_ie[0],
        &config->media_config->codec_ie[0],
        config->media_config->len
    );

    /* load the BD addr */
    BT_COPY_BD_ADDR(a2dp->peer_addr, a2dp->ethermind_avdtp_handle.bd_addr);
    /* Initialize SEP Configuration Parameters */
    AVDTP_INIT_SEP_CONFIGURATION (a2dp->sep_conf);
    /* Enable Media Transport Service in SEP Configuration */
    AVDTP_SET_SEP_CONFIGURATION_MEDIA (a2dp->sep_conf);
    /* Set AVDTP Handle Remote SEID */
    AVDTP_SET_HANDLE_REMOTE_SEID (a2dp->ethermind_avdtp_handle, endpoint->info.sep.id);
    /* Enable Codec Capabilities in SEP Configuration */
    AVDTP_SET_SEP_CONFIGURATION_CODEC_CAP
    (
        a2dp->sep_conf,
        BT_A2DP_AUDIO,
        endpoint->codec_id,
        config->media_config->len,
        config->media_config->codec_ie
    )
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
    if (config->cp_config != NULL)
    {
        uint8_t type = (uint16_t)(config->cp_config->codec_ie[0]) | ((uint16_t)config->cp_config->codec_ie[1] << 8U);
        AVDTP_SET_SEP_CP_CAPABILITY
        (
            (a2dp->sep_conf.cp_cap[0]),
            type,
            config->cp_config->codec_ie,
            config->cp_config->len - 2
        );
        ep_state->cp_config = *((struct bt_a2dp_codec_ie_internal *)config->cp_config);
    }
#endif
#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))
    AVDTP_SET_SEP_CONFIGURATION_RECOVERY
    (
        a2dp->sep_conf,
        config->recovery_config->codec_ie[0],
        config->recovery_config->codec_ie[1],
        config->recovery_config->codec_ie[2]
    );
    ep_state->recovery_config = *((struct bt_a2dp_codec_ie_internal *)(config->recovery_config));
#endif
#if ((defined(CONFIG_BT_A2DP_REPORTING_SERVICE)) && (CONFIG_BT_A2DP_REPORTING_SERVICE > 0U))
    if (config->reporting_service_config != 0)
    {
        AVDTP_SET_SEP_CONFIGURATION_REPORTING(a2dp->sep_conf);
        ep_state->reporting_service_config = 1;
    }
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
    if (config->delay_reporting_service_config != 0)
    {
        AVDTP_SET_SEP_CONFIGURATION_DELAY_REPORTING (a2dp->sep_conf);
        ep_state->delay_reporting_config = 1;
    }
#endif
#if ((defined(CONFIG_BT_A2DP_HC_SERVICE)) && (CONFIG_BT_A2DP_HC_SERVICE > 0U))
    a2dp->sep_conf.rohc_conf = config->header_compression_config;
    ep_state->rohc_config = config->header_compression_config;
    if (config->header_compression_config)
    {
        AVDTP_SET_SEP_CONFIGURATION_ROHC (a2dp->sep_conf);
    }
#endif
#if ((defined(CONFIG_BT_A2DP_MULTIPLEXING_SERVICE)) && (CONFIG_BT_A2DP_MULTIPLEXING_SERVICE > 0U))
    ep_state->multiplexing_config = 0;
    if (config->multiplexing_service_config != 0)
    {
        ep_state->multiplexing_config = 1;
        AVDTP_SET_SEP_CONFIGURATION_MULTIPLEXING (a2dp->sep_conf);
    }
#endif
    a2dp->retry_count = A2DP_TRANSFER_RETRY_COUNT;
    ep_state->a2dp = a2dp;
    retval = BT_a2dp_connect
            (
                ep_state->ethermind_a2dp_codec_index,
                a2dp->peer_addr,
                a2dp->ethermind_avdtp_handle.remote_seid,
                &a2dp->sep_conf
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
#if ((defined(CONFIG_BT_A2DP_SOURCE)) && (CONFIG_BT_A2DP_SOURCE > 0U))
        a2dp_set_a2dp_source_codec_encoder(ep_state->codec, endpoint->codec_id, config->media_config->codec_ie);
#endif
    }
    return 0;
}

int bt_a2dp_reconfigure(struct bt_a2dp_endpoint *endpoint,
        struct bt_a2dp_endpoint_config *config)
{
    API_RESULT retval;
    struct bt_a2dp_endpoint_state *ep_state;

    if ((endpoint == NULL) || (config == NULL))
    {
        return -EINVAL;
    }

    ep_state = bt_a2dp_get_endpoint_state(endpoint);
    if ((ep_state == NULL) || (ep_state->a2dp == NULL))
    {
        return -EINVAL;
    }

    /* Initialize SEP Configuration Parameters */
    AVDTP_INIT_SEP_CONFIGURATION (ep_state->a2dp->sep_conf);
    /* Enable Codec Capabilities in SEP Configuration */
    AVDTP_SET_SEP_CONFIGURATION_CODEC_CAP
    (
        ep_state->a2dp->sep_conf,
        BT_A2DP_AUDIO,
        endpoint->codec_id,
        config->media_config->len,
        config->media_config->codec_ie
    )
#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
    if (config->cp_config != NULL)
    {
        uint8_t type = (uint16_t)(config->cp_config->codec_ie[0]) | ((uint16_t)config->cp_config->codec_ie[1] << 8U);
        AVDTP_SET_SEP_CP_CAPABILITY
        (
            (ep_state->a2dp->sep_conf.cp_cap[0]),
            type,
            config->cp_config->codec_ie,
            config->cp_config->len - 2
        );
        ep_state->cp_config = *((struct bt_a2dp_codec_ie_internal *)config->cp_config);
    }
#endif
#if ((defined(CONFIG_BT_A2DP_RECOVERY_SERVICE)) && (CONFIG_BT_A2DP_RECOVERY_SERVICE > 0U))
    AVDTP_SET_SEP_CONFIGURATION_RECOVERY
    (
        ep_state->a2dp->sep_conf,
        config->recovery_config->codec_ie[0],
        config->recovery_config->codec_ie[1],
        config->recovery_config->codec_ie[2]
    );
    ep_state->recovery_config = *((struct bt_a2dp_codec_ie_internal *)(config->recovery_config));
#endif
#if ((defined(CONFIG_BT_A2DP_REPORTING_SERVICE)) && (CONFIG_BT_A2DP_REPORTING_SERVICE > 0U))
    if (config->reporting_service_config != 0)
    {
        AVDTP_SET_SEP_CONFIGURATION_REPORTING(ep_state->a2dp->sep_conf);
        ep_state->reporting_service_config = 1;
    }
#endif
#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))
    if (config->delay_reporting_service_config != 0)
    {
        AVDTP_SET_SEP_CONFIGURATION_DELAY_REPORTING (ep_state->a2dp->sep_conf);
        ep_state->delay_reporting_config = 1;
    }
#endif
#if ((defined(CONFIG_BT_A2DP_HC_SERVICE)) && (CONFIG_BT_A2DP_HC_SERVICE > 0U))
    ep_state->a2dp->sep_conf.rohc_conf = config->header_compression_config;
    ep_state->rohc_config = config->header_compression_config;
#endif
#if ((defined(CONFIG_BT_A2DP_MULTIPLEXING_SERVICE)) && (CONFIG_BT_A2DP_MULTIPLEXING_SERVICE > 0U))
    ep_state->multiplexing_config = 0;
    if (config->multiplexing_service_config != 0)
    {
        ep_state->multiplexing_config = 1;
        AVDTP_SET_SEP_CONFIGURATION_MULTIPLEXING (ep_state->a2dp->sep_conf);
    }
#endif

    ep_state->a2dp->config_internal.len = config->media_config->len;
    memcpy
    (
        &ep_state->a2dp->config_internal.codec_ie[0],
        &config->media_config->codec_ie[0],
        config->media_config->len
    );
    retval = BT_a2dp_reconfigure(ep_state->ethermind_a2dp_codec_index,
                                 &ep_state->a2dp->sep_conf);
    if (retval == 0U)
    {
        return 0U;
    }
    else
    {
        return -EACCES;
    }
}

#if ((defined(CONFIG_BT_A2DP_CP_SERVICE)) && (CONFIG_BT_A2DP_CP_SERVICE > 0U))
int bt_a2dp_set_cp_header(struct bt_a2dp_endpoint *endpoint, uint8_t *header, uint16_t header_len)
{
    API_RESULT retval;
    struct bt_a2dp_endpoint_state *ep_state = bt_a2dp_get_endpoint_state(endpoint);

    if (ep_state == NULL)
    {
        return -EINVAL;
    }
    retval = BT_a2dp_set_cp_header(ep_state->ethermind_a2dp_codec_index,
                                   header,
                                   header_len);
    if (retval != API_SUCCESS)
    {
        return -EINVAL;
    }

    ep_state->cp_header_len = header_len;
    return 0;
}
#endif

#if ((defined(CONFIG_BT_A2DP_DR_SERVICE)) && (CONFIG_BT_A2DP_DR_SERVICE > 0U))

int bt_a2dp_set_initial_delay_report(struct bt_a2dp_endpoint *endpoint, int16_t delay)
{
    struct bt_a2dp_endpoint_state *ep_state;

    if (endpoint == NULL)
    {
        return -EINVAL;
    }
    ep_state = bt_a2dp_get_endpoint_state(endpoint);
    if ((ep_state == NULL) || (ep_state->a2dp == NULL))
    {
        return -EINVAL;
    }

    if (BT_A2DP_SINK != endpoint->info.sep.tsep)
    {
        return -EINVAL;
    }

    if (BT_a2dp_set_initial_delay_report(ep_state->ethermind_a2dp_codec_index,
                                         delay) == API_SUCCESS)
    {
        return 0;
    }
    else
    {
        return -EINVAL;
    }
}

int bt_a2dp_send_delay_report(struct bt_a2dp_endpoint *endpoint, int16_t delay)
{
    struct bt_a2dp_endpoint_state *ep_state;

    if (endpoint == NULL)
    {
        return -EINVAL;
    }
    ep_state = bt_a2dp_get_endpoint_state(endpoint);
    if ((ep_state == NULL) || (ep_state->a2dp == NULL))
    {
        return -EINVAL;
    }

    if (BT_A2DP_SINK != endpoint->info.sep.tsep)
    {
        return -EINVAL;
    }

    if (BT_a2dp_send_delay_report(ep_state->ethermind_a2dp_codec_index,
                                  &delay) == API_SUCCESS)
    {
        return 0;
    }
    else
    {
        return -EINVAL;
    }
}

#endif
#endif

#endif /* CONFIG_BT_BREDR */