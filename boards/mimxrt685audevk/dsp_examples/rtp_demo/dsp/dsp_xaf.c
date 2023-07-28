/*
 * Copyright 2019-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dsp_xaf.h"
#include "dsp_config.h"
#include "message.h"
#include "rpmsg_lite.h"
#include "rtp.h"

#include "xaf-utils-test.h"
#include "xa-audio-decoder-api.h"
#include "xa-mixer-api.h"
#include "xa-renderer-api.h"
#include "xa_g711_codec_api.h"
#include "xa_src_pp_api.h"

#include "audio/xa-pcm-stereo-api.h"
#include "audio/xa-renderer-api.h"

#include <xtensa/xos.h>

#include "fsl_common.h"
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
#include "fsl_memory.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define AUDIO_FRMWK_BUF_SIZE (64 * 1024)
#define AUDIO_COMP_BUF_SIZE  (256 * 1024)

#define OUTPUT_SAMPLE_RATE 48000

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @brief Pipeline processing thread */
static int32_t dsp_xaf_process_thread(void *arg, int32_t wake_value);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static char playback_stack[STACK_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/

/*! @brief Memory alloc for XAF */
static void *dsp_xaf_malloc(int32_t size, int32_t id)
{
    /* If malloc can return un-aligned data, must return 4-byte aligned pointer for XAF. */
    return malloc(size);
}

/*! @brief Memory free for XAF */
static void dsp_xaf_free(void *ptr, int32_t id)
{
    free(ptr);
}

void DSP_PRINTF(const char* ptr, ...)
{
    va_list ap;
    SEMA42_Lock(APP_SEMA42, SEMA_PRINTF_NUM, SEMA_CORE_ID_DSP);
    va_start(ap, ptr);
    DbgConsole_Vprintf(ptr, ap);
    va_end(ap);
    SEMA42_Unlock(APP_SEMA42, SEMA_PRINTF_NUM);
}

static void timer_expired(void *arg)
{
    XosSem *frame_elapsed = (XosSem *)arg;
    int32_t status;

    status = xos_sem_put(frame_elapsed);
    if (status != XOS_OK)
    {
        DSP_PRINTF("[dsp_xaf] xos_sem_put failed: %d\r\n", status);
    }
}

int dsp_xaf_start_thread(dsp_handle_t *dsp)
{
    int status;

    /* Start processing thread */
    status = xos_thread_create(&dsp->playback_thread, NULL, dsp_xaf_process_thread, (void *)dsp, "playback",
                               playback_stack, STACK_SIZE, (XOS_MAX_PRIORITY - 2), 0, 0);
    if (status != XOS_OK)
    {
        DSP_PRINTF("[dsp_xaf] xos_thread_create failure: %d\r\n", status);
        return -1;
    }

    return 0;
}

void dsp_xaf_init(dsp_handle_t *dsp)
{
    uint8_t *version[3];
    int status;
    int i;

    status = xos_sem_create(&dsp->frame_elapsed, XOS_SEM_WAIT_PRIORITY, 0);
    if (status != XOS_OK)
    {
        xos_fatal_error(status, "Failed to create semaphore.\r\n");
    }

    status = xos_timer_init(&dsp->rtp_timer);
    if (status != XOS_OK)
    {
        xos_fatal_error(status, "Failed to create timer.\r\n");
    }

    /* Start timer with period of one RTP frame duration (10 ms) */
    status = xos_timer_start(&dsp->rtp_timer, (XOS_CLOCK_FREQ / (RTP_SAMPLE_RATE / XA_G711_MAX_SAMPLES_PER_FRAME)),
                             XOS_TIMER_PERIODIC, timer_expired, (void *)&dsp->frame_elapsed);
    if (status != XOS_OK)
    {
        xos_fatal_error(status, "Failed to start timer.\r\n");
    }

    for (i = 0; i < MAX_INPUT_STREAMS; i++)
    {
        if (!session_init(&dsp->sessions[i]))
        {
            xos_fatal_error(status, "Failed to create session.\r\n");
        }
    }

    xaf_get_verinfo(version);

    DSP_PRINTF("\r\n");

    DSP_PRINTF("[dsp_xaf] Cadence Xtensa Audio Framework\r\n");
    DSP_PRINTF("[dsp_xaf] Library Name    : %s\r\n", version[0]);
    DSP_PRINTF("[dsp_xaf] Library Version : %s\r\n", version[1]);
    DSP_PRINTF("[dsp_xaf] API Version     : %s\r\n", version[2]);
    DSP_PRINTF("\r\n");

    DSP_PRINTF("[dsp_xaf] %s\r\n", xa_g711_get_lib_name_string());
    DSP_PRINTF("[dsp_xaf] Library Version : %s\r\n", xa_g711_get_lib_version_string());
    DSP_PRINTF("[dsp_xaf] API Version     : %s\r\n", xa_g711_get_lib_api_version_string());

    DSP_PRINTF("\r\n");
}

void dsp_xaf_add_packet(dsp_handle_t *dsp, rtp_header_t *packet, uint32_t size)
{
    session_t *session;

    session = session_get(dsp, packet->ssrc_id);
    session_add_packet(dsp, session, packet, size);
}

void dsp_xaf_packet_consumed(dsp_handle_t *dsp, rtp_header_t *packet)
{
    message_t msg;

    memset(&msg, 0U, sizeof(message_t));
    msg.command   = (uint8_t)AudioEvent_PacketConsumed;
    msg.params[0] = MessageParam_PacketAddress;
    msg.params[1] = (uint32_t)packet;
    msg.params[2] = MessageParam_NULL;

    int32_t res = rpmsg_lite_send(dsp->rpmsg, dsp->ept, MCU_EPT_ADDR, (char *)&msg, sizeof(message_t), RL_BLOCK);
    if (res != RL_SUCCESS)
    {
        DSP_PRINTF("rpmsg_lite_send failed: %d\r\n", res);
    }
}

static rtp_frame_t *dsp_xaf_get_input_frame(dsp_handle_t *dsp, session_t *session)
{
    rtp_frame_t *frame;

    session_lock(session);

    if (session->buffering_countdown != 0U)
    {
        session_unlock(session);
        return NULL;
    }

    frame = frame_list_first(session->frame_list);
    if (frame == NULL)
    {
        /* Nothing to play */
        if (session->frame_list->last_played_set)
        {
            /* Already played something in this session but buffer emptied now - start pre-buffering frames again */
            session_reset(session);
            session->frame_list->last_played_timestamp = 0U;
            session->frame_list->last_played_set       = false;
            session->underrun_count++;
        }
        session_unlock(session);
        return NULL;
    }

    if (session->frame_list->last_played_set &&
        !TIMESTAMP_LESS_THAN_OR_EQUAL(frame->timestamp,
                                      session->frame_list->last_played_timestamp + XA_G711_MAX_SAMPLES_PER_FRAME))
    {
        /* This frame will be played in future */
        session->frame_list->last_played_timestamp += XA_G711_MAX_SAMPLES_PER_FRAME;
        session_unlock(session);
        return NULL;
    }

    session->frame_list->last_played_timestamp = frame->timestamp;
    session->frame_list->last_played_set       = true;
    session->last_law                          = frame->law;

    session_unlock(session);
    return frame;
}

static void dsp_xaf_remove_input_frame(dsp_handle_t *dsp, session_t *session)
{
    rtp_frame_t *frame;
    rtp_header_t *dealloc_packet;

    session_lock(session);
    frame          = frame_list_first(session->frame_list);
    dealloc_packet = frame->dealloc_packet;
    frame_list_remove_first(session->frame_list);
    session_unlock(session);

    if (dealloc_packet != NULL)
    {
        dsp_xaf_packet_consumed(dsp, dealloc_packet);
    }
}

/*! @brief Updates state */
static void dsp_xaf_set_state(dsp_handle_t *dsp, dsp_state_t state)
{
    dsp->state = state;
}

static XAF_ERR_CODE dsp_xaf_get_comp_config(void *p_comp, xaf_format_t *comp_format)
{
    WORD32 param[6];
    XAF_ERR_CODE err_code;

    param[0] = XA_CODEC_CONFIG_PARAM_CHANNELS;
    param[2] = XA_CODEC_CONFIG_PARAM_SAMPLE_RATE;
    param[4] = XA_CODEC_CONFIG_PARAM_PCM_WIDTH;

    err_code = xaf_comp_get_config(p_comp, 3, &param[0]);
    if (err_code < 0)
    {
        return err_code;
    }

    comp_format->channels    = param[1];
    comp_format->sample_rate = param[3];
    comp_format->pcm_width   = param[5];

    return XAF_NO_ERR;
}

static XAF_ERR_CODE dsp_xaf_stereo_setup(void *p_stereo)
{
    WORD32 param[8];

    param[0] = XA_PCM_STEREO_CONFIG_PARAM_PCM_WIDTH;
    param[1] = XA_G711_NUM_BITS_PER_SAMPLE;
    param[2] = XA_PCM_STEREO_CONFIG_PARAM_CHANNELS;
    param[3] = 2;
    param[4] = XA_PCM_STEREO_CONFIG_PARAM_SAMPLE_RATE;
    param[5] = RTP_SAMPLE_RATE;
    param[6] = XA_PCM_STEREO_CONFIG_PARAM_FRAME_SIZE;
    param[7] = XA_G711_MAX_SAMPLES_PER_FRAME * 1;

    return (xaf_comp_set_config(p_stereo, 4, &param[0]));
}

static XAF_ERR_CODE dsp_xaf_mixer_setup(void *p_mixer, xaf_format_t *format)
{
    WORD32 param[8];

    param[0] = XA_MIXER_CONFIG_PARAM_PCM_WIDTH;
    param[1] = format->pcm_width;
    param[2] = XA_MIXER_CONFIG_PARAM_CHANNELS;
    param[3] = format->channels;
    param[4] = XA_MIXER_CONFIG_PARAM_SAMPLE_RATE;
    param[5] = format->sample_rate;
    param[6] = XA_MIXER_CONFIG_PARAM_FRAME_SIZE;
    param[7] = XA_G711_MAX_SAMPLES_PER_FRAME * 1;

    return (xaf_comp_set_config(p_mixer, 4, &param[0]));
}

static XAF_ERR_CODE dsp_xaf_src_setup(void *p_src, xaf_format_t *format)
{
    WORD32 param[10];
    uint32_t pcm_width_bytes = format->pcm_width / 8;

    param[0] = XA_SRC_PP_CONFIG_PARAM_INPUT_CHANNELS;
    param[1] = format->channels;
    param[2] = XA_SRC_PP_CONFIG_PARAM_INPUT_SAMPLE_RATE;
    param[3] = format->sample_rate;
    param[4] = XA_SRC_PP_CONFIG_PARAM_OUTPUT_SAMPLE_RATE;
    param[5] = OUTPUT_SAMPLE_RATE;
    param[6] = XA_SRC_PP_CONFIG_PARAM_INPUT_CHUNK_SIZE;
    param[7] = XA_G711_MAX_SAMPLES_PER_FRAME * format->channels * pcm_width_bytes;
    param[8] = XA_SRC_PP_CONFIG_PARAM_BYTES_PER_SAMPLE;
    param[9] = (pcm_width_bytes == 4) ? 3 : 2; // SRC library only supports 16 or MSB-aligned 24 bit input

    return (xaf_comp_set_config(p_src, 5, &param[0]));
}

static XAF_ERR_CODE dsp_xaf_renderer_setup(void *p_renderer, xaf_format_t *format)
{
    WORD32 param[14];

    param[0] = XA_RENDERER_CONFIG_PARAM_PCM_WIDTH;
    param[1] = XA_G711_NUM_BITS_PER_SAMPLE; // seems format->pcm_width obtained from SRC component returns bytes
                                            // unlike other components which return bits
    param[2] = XA_RENDERER_CONFIG_PARAM_CHANNELS;
    param[3] = format->channels;
    param[4] = XA_RENDERER_CONFIG_PARAM_SAMPLE_RATE;
    param[5] = format->sample_rate;
    param[6] = XA_RENDERER_CONFIG_PARAM_FRAME_SIZE;
    param[7] = XA_G711_MAX_SAMPLES_PER_FRAME * format->channels * (XA_G711_NUM_BITS_PER_SAMPLE / 8) *
               (OUTPUT_SAMPLE_RATE / RTP_SAMPLE_RATE);
    param[8]  = XA_RENDERER_CONFIG_PARAM_AUDIO_BUFFER_1;
    param[9]  = (WORD32)DSP_AUDIO_BUFFER_1_PING;
    param[10] = XA_RENDERER_CONFIG_PARAM_AUDIO_BUFFER_2;
    param[11] = (WORD32)DSP_AUDIO_BUFFER_1_PONG;
    param[12] = XA_RENDERER_CONFIG_PARAM_I2S_INTERFACE;
    param[13] = AUDIO_I2S_RENDERER_DEVICE;

    return (xaf_comp_set_config(p_renderer, 7, &param[0]));
}

/**
 * @brief Creates XAF playback component chain
 *
 * @param dsp DSP context handle
 */
static void dsp_xaf_process_creating(dsp_handle_t *dsp)
{
#define CHECK_ERROR(err_code, action)                                \
    if (err_code != XAF_NO_ERR)                                      \
    {                                                                \
        DSP_PRINTF("[dsp_xaf] " action " failed: %d\r\n", err_code); \
        dsp_xaf_set_state(dsp, DspState_Closing);                    \
        return;                                                      \
    }

    xaf_comp_status comp_status;
    int comp_info[4];
    XAF_ERR_CODE err_code;
    xaf_format_t format;
    int i;
    xaf_adev_config_t device_config;
    xaf_comp_config_t comp_config;

    xaf_adev_config_default_init(&device_config);

    device_config.pmem_malloc                 = dsp_xaf_malloc;
    device_config.pmem_free                   = dsp_xaf_free;
    device_config.audio_component_buffer_size = AUDIO_COMP_BUF_SIZE;
    device_config.audio_framework_buffer_size = AUDIO_FRMWK_BUF_SIZE;

    err_code = xaf_adev_open(&dsp->audio_device, &device_config);
    CHECK_ERROR(err_code, "xaf_adev_open");

    DSP_PRINTF("[dsp_xaf] Audio device open\r\n");

    for (i = 0; i < MAX_INPUT_STREAMS; i++)
    {
        /* Input component converts mono to stereo */
        xaf_comp_config_default_init(&comp_config);

        comp_config.comp_id            = "post-proc/pcm_stereo";
        comp_config.num_input_buffers  = 1;
        comp_config.num_output_buffers = 0;
        comp_config.pp_inbuf           = (pVOID(*)[XAF_MAX_INBUFS]) & dsp->inbuf[i];
        comp_config.comp_type          = XAF_POST_PROC;

        err_code = xaf_comp_create(dsp->audio_device, &dsp->comp_input[i], &comp_config);
        CHECK_ERROR(err_code, "xaf_comp_create stereo");

        err_code = dsp_xaf_stereo_setup(dsp->comp_input[i]);
        CHECK_ERROR(err_code, "stereo_setup");

        err_code = xaf_comp_process(dsp->audio_device, dsp->comp_input[i], NULL, 0, XAF_START_FLAG);
        CHECK_ERROR(err_code, "xaf_comp_process XAF_START_FLAG stereo");

        err_code = xaf_comp_get_status(dsp->audio_device, dsp->comp_input[i], &comp_status, &comp_info[0]);
        CHECK_ERROR(err_code, "xaf_comp_get_status stereo");

        DSP_PRINTF("[dsp_xaf] STEREO component created\r\n");
    }

    /* Mixer component */
    xaf_comp_config_default_init(&comp_config);

    comp_config.comp_id            = "mixer";
    comp_config.num_input_buffers  = 0;
    comp_config.num_output_buffers = 0;
    comp_config.pp_inbuf           = NULL;
    comp_config.comp_type          = XAF_MIXER;

    err_code = xaf_comp_create(dsp->audio_device, &dsp->comp_mixer, &comp_config);
    CHECK_ERROR(err_code, "xaf_comp_create mixer");

    /* Get audio format from the input component */
    err_code = dsp_xaf_get_comp_config(dsp->comp_input[0], &format);
    CHECK_ERROR(err_code, "xaf_get_comp_config stereo");

    /* Configure mixer component */
    err_code = dsp_xaf_mixer_setup(dsp->comp_mixer, &format);
    CHECK_ERROR(err_code, "mixer_setup");

    DSP_PRINTF("[dsp_xaf] MIXER component created\r\n");

    for (i = 0; i < MAX_INPUT_STREAMS; i++)
    {
        err_code = xaf_connect(dsp->comp_input[i], 1, dsp->comp_mixer, i, 10);
        CHECK_ERROR(err_code, "xaf_connect stereo->mixer");

        DSP_PRINTF("[dsp_xaf] Connected STEREO -> MIXER\r\n");
    }

    /* Pause input ports of unused MIXER tracks */
    for (i = MAX_INPUT_STREAMS; i < XA_MIXER_MAX_TRACK_NUMBER; i++)
    {
        err_code = xaf_pause(dsp->comp_mixer, i);
        CHECK_ERROR(err_code, "xaf_pause");
    }

    /* Start MIXER component */
    err_code = xaf_comp_process(dsp->audio_device, dsp->comp_mixer, NULL, 0, XAF_START_FLAG);
    CHECK_ERROR(err_code, "xaf_comp_process XAF_START_FLAG mixer");

    err_code = xaf_comp_get_status(dsp->audio_device, dsp->comp_mixer, &comp_status, &comp_info[0]);
    CHECK_ERROR(err_code, "xaf_comp_get_status mixer");

    DSP_PRINTF("[dsp_xaf] MIXER component started\r\n");

    /* Sample rate converter component */
    xaf_comp_config_default_init(&comp_config);

    comp_config.comp_id            = "post-proc/src-pp";
    comp_config.num_input_buffers  = 0;
    comp_config.num_output_buffers = 0;
    comp_config.pp_inbuf           = NULL;
    comp_config.comp_type          = XAF_POST_PROC;

    err_code = xaf_comp_create(dsp->audio_device, &dsp->comp_src, &comp_config);
    CHECK_ERROR(err_code, "xaf_comp_create SRC");

    /* Configure sample rate converter component */
    err_code = dsp_xaf_src_setup(dsp->comp_src, &format);
    CHECK_ERROR(err_code, "src_setup");

    DSP_PRINTF("[dsp_xaf] SRC component created\r\n");

    err_code = xaf_connect(dsp->comp_mixer, 4, dsp->comp_src, 0, 4);
    CHECK_ERROR(err_code, "xaf_connect mixer->SRC");

    DSP_PRINTF("[dsp_xaf] Connected MIXER -> SRC\r\n");

    /* Start SRC component */
    err_code = xaf_comp_process(dsp->audio_device, dsp->comp_src, NULL, 0, XAF_START_FLAG);
    CHECK_ERROR(err_code, "xaf_comp_process XAF_START_FLAG SRC");

    err_code = xaf_comp_get_status(dsp->audio_device, dsp->comp_src, &comp_status, &comp_info[0]);
    CHECK_ERROR(err_code, "xaf_comp_get_status SRC");

    DSP_PRINTF("[dsp_xaf] SRC component started\r\n");

    xaf_comp_config_default_init(&comp_config);

    comp_config.comp_id            = "renderer";
    comp_config.num_input_buffers  = 0;
    comp_config.num_output_buffers = 0;
    comp_config.pp_inbuf           = NULL;
    comp_config.comp_type          = XAF_RENDERER;

    err_code = xaf_comp_create(dsp->audio_device, &dsp->comp_renderer, &comp_config);
    CHECK_ERROR(err_code, "xaf_comp_create renderer");

    /* Get audio format from the input component */
    err_code = dsp_xaf_get_comp_config(dsp->comp_src, &format);
    CHECK_ERROR(err_code, "get_comp_config SRC");

    /* Setup renderer to match PCM format */
    err_code = dsp_xaf_renderer_setup(dsp->comp_renderer, &format);
    CHECK_ERROR(err_code, "renderer_setup");

    DSP_PRINTF("[dsp_xaf] RENDERER component created\r\n");

    err_code = xaf_connect(dsp->comp_src, 1, dsp->comp_renderer, 0, 4);
    CHECK_ERROR(err_code, "xaf_connect SRC->renderer");

    DSP_PRINTF("[dsp_xaf] Connected SRC -> RENDERER\r\n");

    err_code = xaf_comp_process(dsp->audio_device, dsp->comp_renderer, NULL, 0, XAF_START_FLAG);
    CHECK_ERROR(err_code, "xaf_comp_process XAF_START_FLAG renderer");

    err_code = xaf_comp_get_status(dsp->audio_device, dsp->comp_renderer, &comp_status, &comp_info[0]);
    CHECK_ERROR(err_code, "xaf_comp_get_status renderer");

    for (i = 0; i < MAX_INPUT_STREAMS; i++)
    {
        err_code = xaf_comp_process(dsp->audio_device, dsp->comp_input[i], NULL, 0, XAF_EXEC_FLAG);
        CHECK_ERROR(err_code, "xaf_comp_process XAF_EXEC_FLAG stereo");

        err_code = xaf_comp_get_status(dsp->audio_device, dsp->comp_input[i], &comp_status, &comp_info[0]);
        CHECK_ERROR(err_code, "xaf_comp_get_status stereo");

        if (comp_status != XAF_NEED_INPUT)
        {
            DSP_PRINTF("[dsp_xaf] xaf_get_status unexpected status: %d\r\n", comp_status);
            dsp_xaf_set_state(dsp, DspState_Closing);
            return;
        }

        dsp->dec_handle[i] = (xa_codec_handle_t)malloc(xa_g711_dec_get_handle_byte_size());
        if (dsp->dec_handle[i] == NULL)
        {
            DSP_PRINTF("[dsp_xaf] failed to allocate decoder handle\r\n");
            dsp_xaf_set_state(dsp, DspState_Closing);
            return;
        }

        dsp->dec_scratch[i] = (pVOID)malloc(xa_g711_dec_get_scratch_byte_size());
        if (dsp->dec_scratch[i] == NULL)
        {
            DSP_PRINTF("[dsp_xaf] failed to allocate decoder scratch memory\r\n");
            dsp_xaf_set_state(dsp, DspState_Closing);
            return;
        }

        XA_ERRORCODE res = xa_g711_dec_init(dsp->dec_handle[i], dsp->dec_scratch[i], PLC_ENABLED);
        if (res != XA_NO_ERROR)
        {
            DSP_PRINTF("[dsp_xaf] xa_g711_dec_init failed: %d\r\n", res);
            dsp_xaf_set_state(dsp, DspState_Closing);
            return;
        }
    }

    dsp_xaf_set_state(dsp, DspState_Running);
}

/*! @brief Executed in Running state */
static void dsp_xaf_process_running(dsp_handle_t *dsp)
{
    static uint8_t dummy[XA_G711_MAX_NUM_BYTES_PER_FRAME];
    xaf_comp_status comp_status;
    XAF_ERR_CODE err_code;
    int32_t comp_info[4];
    session_t *session;
    rtp_frame_t *frame;
    int16_t *buffer;
    int status;
    int i;

    /* Wait until RTP frame period has elapsed */
    status = xos_sem_get(&dsp->frame_elapsed);
    if (status != XOS_OK)
    {
        DSP_PRINTF("[dsp_xaf] xos_sem_get failed: %d\r\n", status);
        dsp_xaf_set_state(dsp, DspState_Closing);
        return;
    }

    /* Decode input frames */
    for (i = 0; i < MAX_INPUT_STREAMS; i++)
    {
        buffer  = (int16_t *)dsp->inbuf[i];
        session = &dsp->sessions[i];
        status  = XA_NO_ERROR;

        /* Get input frame to be played for this stream */
        frame = dsp_xaf_get_input_frame(dsp, session);

        if (frame != NULL)
        {
            session->frames_played++;
            status = xa_g711_dec(dsp->dec_handle[i], frame->data, buffer, frame->law, XA_G711_FRAME_NORMAL);
            dsp_xaf_remove_input_frame(dsp, session);
        }
        else if (session->frames_played > 0U)
        {
            if (session->ssrc_id != 0U)
            {
                session->frames_dropped++;
            }
#if PLC_ENABLED
            /* Use packet loss concealment for dropped data */
            status = xa_g711_dec(dsp->dec_handle[i], &dummy[0], buffer, session->last_law, XA_G711_FRAME_ERASURE);
#else
            /* Fill PCM buffer with zeroes */
            memset((void *)&buffer[0], 0, XA_G711_MAX_NUM_BYTES_PER_FRAME * 2);
#endif /* PLC_ENABLED */
        }
        else
        {
            /* Nothing played for the stream yet, fill PCM buffer with zeroes */
            memset((void *)&buffer[0], 0, XA_G711_MAX_NUM_BYTES_PER_FRAME * 2);
        }

        if (status != XA_NO_ERROR)
        {
            DSP_PRINTF("[dsp_xaf] xa_g711_dec failed %d\r\n", status);
            dsp_xaf_set_state(dsp, DspState_Closing);
            return;
        }
    }

    /* Feed decoded data into audio pipeline */
    for (i = 0; i < MAX_INPUT_STREAMS; i++)
    {
        buffer = (int16_t *)dsp->inbuf[i];

        err_code = xaf_comp_process(dsp->audio_device, dsp->comp_input[i], buffer, XA_G711_MAX_NUM_BYTES_PER_FRAME * 2,
                                    XAF_INPUT_READY_FLAG);
        if (err_code != XAF_NO_ERR)
        {
            DSP_PRINTF("[dsp_xaf] xaf_comp_process (flag = %d) failure: %d\r\n", XAF_INPUT_READY_FLAG, err_code);
            dsp_xaf_set_state(dsp, DspState_Closing);
            return;
        }
    }

    /* Wait for async results of feeding the data */
    for (i = 0; i < MAX_INPUT_STREAMS; i++)
    {
        err_code = xaf_comp_get_status(dsp->audio_device, dsp->comp_input[i], &comp_status, &comp_info[0]);
        if (err_code != XAF_NO_ERR)
        {
            DSP_PRINTF("[dsp_xaf] xaf_comp_get_status failure: %d\r\n", err_code);
            dsp_xaf_set_state(dsp, DspState_Closing);
            return;
        }

        if (comp_status != XAF_NEED_INPUT)
        {
            DSP_PRINTF("[dsp_xaf] unexpected status: %d\r\n", comp_status);
            dsp_xaf_set_state(dsp, DspState_Closing);
            return;
        }
    }
}

/*! @brief Executed in Closing state */
static void dsp_xaf_process_closing(dsp_handle_t *dsp)
{
    xaf_adev_close_flag close_flag = XAF_ADEV_NORMAL_CLOSE;
    XAF_ERR_CODE err_code;
    int i;

    /* Clean up and shut down XAF */

    DSP_PRINTF("[dsp_xaf] closing\r\n");

    for (i = 0; i < MAX_INPUT_STREAMS; i++)
    {
        if (dsp->comp_input[i] != NULL)
        {
            err_code           = xaf_comp_delete(dsp->comp_input[i]);
            dsp->comp_input[i] = NULL;
            if (err_code != XAF_NO_ERR)
            {
                DSP_PRINTF("[dsp_xaf] xaf_comp_delete failure: %d\r\n", err_code);
                close_flag = XAF_ADEV_FORCE_CLOSE;
            }
        }

        if (dsp->dec_scratch[i] != NULL)
        {
            free(dsp->dec_scratch[i]);
            dsp->dec_scratch[i] = NULL;
        }

        if (dsp->dec_handle[i] != NULL)
        {
            free(dsp->dec_handle[i]);
            dsp->dec_handle[i] = NULL;
        }
    }

    if (dsp->comp_mixer != NULL)
    {
        err_code        = xaf_comp_delete(dsp->comp_mixer);
        dsp->comp_mixer = NULL;
        if (err_code != XAF_NO_ERR)
        {
            DSP_PRINTF("[dsp_xaf] xaf_comp_delete failure: %d\r\n", err_code);
            close_flag = XAF_ADEV_FORCE_CLOSE;
        }
    }

    if (dsp->comp_src != NULL)
    {
        err_code      = xaf_comp_delete(dsp->comp_src);
        dsp->comp_src = NULL;
        if (err_code != XAF_NO_ERR)
        {
            DSP_PRINTF("[dsp_xaf] xaf_comp_delete failure: %d\r\n", err_code);
            close_flag = XAF_ADEV_FORCE_CLOSE;
        }
    }

    if (dsp->comp_renderer != NULL)
    {
        err_code           = xaf_comp_delete(dsp->comp_renderer);
        dsp->comp_renderer = NULL;
        if (err_code != XAF_NO_ERR)
        {
            DSP_PRINTF("[dsp_xaf] xaf_comp_delete failure: %d\r\n", err_code);
            close_flag = XAF_ADEV_FORCE_CLOSE;
        }
    }

    err_code = xaf_adev_close(dsp->audio_device, close_flag);
    if (err_code != XAF_NO_ERR)
    {
        DSP_PRINTF("[dsp_xaf] xaf_adev_close failure: %d\r\n", err_code);
    }

    DSP_PRINTF("[dsp_xaf] audio device closed\r\n\r\n");

    dsp_xaf_set_state(dsp, DspState_Idle);
}

static int32_t dsp_xaf_process_thread(void *arg, int32_t wake_value)
{
    dsp_handle_t *dsp = (dsp_handle_t *)arg;

    DSP_PRINTF("[dsp_xaf] start\r\n");

    dsp_xaf_set_state(dsp, DspState_Creating);

    while (true)
    {
        switch (dsp->state)
        {
            case DspState_Creating:
                dsp_xaf_process_creating(dsp);
                break;
            case DspState_Running:
                dsp_xaf_process_running(dsp);
                break;
            case DspState_Closing:
                dsp_xaf_process_closing(dsp);
                return 0;
            default:
                break;
        }
    }
}
