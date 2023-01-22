/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "dsp_xaf.h"
#include "dsp_config.h"
#include "message.h"
#include "rpmsg_lite.h"

#include "xaf-api.h"
#include "xa-capturer-api.h"
#include "xa-renderer-api.h"
#include "microspeech/xa-microspeech-frontend-api.h"
#include "microspeech/xa-microspeech-inference-api.h"

#include <xtensa/xos.h>

#include "fsl_common.h"
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
#include "fsl_memory.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Stack size for DSP data processing thread. */
#define STACK_SIZE (4 * 1024)

#define AUDIO_FRMWK_BUF_SIZE (64 * 1024)
#define AUDIO_COMP_BUF_SIZE  (256 * 1024)

typedef struct xaf_format_s {
    UWORD32 sample_rate;
    UWORD32 channels;
    UWORD32 pcm_width;
    UWORD32 input_length;
    UWORD32 output_length;
    UWORD32 output_produced;
} xaf_format_t;

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

int dsp_xaf_start_thread(dsp_handle_t *dsp)
{
    int status;

    /* Start processing thread */
    status = xos_thread_create(&dsp->processing_thread, NULL, dsp_xaf_process_thread, (void *)dsp, "playback",
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

    status = xos_sem_create(&dsp->inference_done, XOS_SEM_WAIT_PRIORITY, 0);
    if (status != XOS_OK)
    {
        xos_fatal_error(status, "Failed to create semaphore.\r\n");
    }

    xaf_get_verinfo(version);

    DSP_PRINTF("\r\n");

    DSP_PRINTF("Cadence Xtensa Audio Framework\r\n");
    DSP_PRINTF("  Library Name    : %s\r\n", version[0]);
    DSP_PRINTF("  Library Version : %s\r\n", version[1]);
    DSP_PRINTF("  API Version     : %s\r\n", version[2]);

    DSP_PRINTF("\r\n");
}

static XAF_ERR_CODE dsp_xaf_get_comp_config(void *p_comp, xaf_format_t *comp_format)
{
    WORD32 param[6];
    XAF_ERR_CODE err_code;

    param[0] = XA_CAPTURER_CONFIG_PARAM_CHANNELS;
    param[2] = XA_CAPTURER_CONFIG_PARAM_SAMPLE_RATE;
    param[4] = XA_CAPTURER_CONFIG_PARAM_PCM_WIDTH;

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

static XAF_ERR_CODE dsp_xaf_capturer_setup(void *p_src, xaf_format_t *format)
{
    WORD32 param[8];

    param[0] = XA_CAPTURER_CONFIG_PARAM_PCM_WIDTH;
    param[1] = format->pcm_width;
    param[2] = XA_CAPTURER_CONFIG_PARAM_CHANNELS;
    param[3] = format->channels;
    param[4] = XA_CAPTURER_CONFIG_PARAM_SAMPLE_RATE;
    param[5] = format->sample_rate;
    param[6] = XA_CAPTURER_CONFIG_PARAM_SAMPLE_END;
    param[7] = 320;

    return (xaf_comp_set_config(p_src, 4, &param[0]));
}

static XAF_ERR_CODE dsp_xaf_capturer_start(void *p_src)
{
    WORD32 param[2];

    param[0] = XA_CAPTURER_CONFIG_PARAM_STATE;
    param[1] = XA_CAPTURER_STATE_START;

    return (xaf_comp_set_config(p_src, 1, &param[0]));
}

static XAF_ERR_CODE dsp_xaf_renderer_setup(void *p_renderer, xaf_format_t *format)
{
    WORD32 param[14];

    param[0] = XA_RENDERER_CONFIG_PARAM_PCM_WIDTH;
    param[1] = format->pcm_width;
    param[2] = XA_RENDERER_CONFIG_PARAM_CHANNELS;
    param[3] = format->channels;
    param[4] = XA_RENDERER_CONFIG_PARAM_SAMPLE_RATE;
    param[5] = format->sample_rate;
    param[6] = XA_RENDERER_CONFIG_PARAM_FRAME_SIZE;
    param[7] = 320 * format->channels * format->pcm_width / 8;
    param[8]  = XA_RENDERER_CONFIG_PARAM_AUDIO_BUFFER_1;
    param[9]  = (WORD32)DSP_AUDIO_BUFFER_1_PING;
    param[10] = XA_RENDERER_CONFIG_PARAM_AUDIO_BUFFER_2;
    param[11] = (WORD32)DSP_AUDIO_BUFFER_1_PONG;
    param[12] = XA_RENDERER_CONFIG_PARAM_I2S_INTERFACE;
    param[13] = AUDIO_I2S_RENDERER_DEVICE;

    return (xaf_comp_set_config(p_renderer, 7, &param[0]));
}

static XAF_ERR_CODE dsp_xaf_renderer_start(void *p_src)
{
    WORD32 param[2];

    param[0] = XA_RENDERER_CONFIG_PARAM_STATE;
    param[1] = XA_RENDERER_STATE_START;

    return (xaf_comp_set_config(p_src, 1, &param[0]));
}

static int dsp_xaf_microspeech_fe_setup(void *p_comp, xaf_format_t* comp_format)
{
    int param[][2] = {
        {
            XA_MICROSPEECH_FE_CONFIG_PARAM_CHANNELS,
            comp_format->channels,
        }, {
            XA_MICROSPEECH_FE_CONFIG_PARAM_SAMPLE_RATE,
            comp_format->sample_rate,
        }, {
            XA_MICROSPEECH_FE_CONFIG_PARAM_PCM_WIDTH,
            comp_format->pcm_width,
        },
    };

    return(xaf_comp_set_config(p_comp, ARRAY_SIZE(param), param[0]));
}

static int dsp_xaf_inference_setup(void *p_comp, xaf_format_t* comp_format)
{
    int param[][2] = {
        {
            XA_MICROSPEECH_INFERENCE_CONFIG_PARAM_CHANNELS,
            comp_format->channels,
        }, {
            XA_MICROSPEECH_INFERENCE_CONFIG_PARAM_SAMPLE_RATE,
            comp_format->sample_rate,
        }, {
            XA_MICROSPEECH_INFERENCE_CONFIG_PARAM_PCM_WIDTH,
            comp_format->pcm_width,
        },
    };

    return(xaf_comp_set_config(p_comp, ARRAY_SIZE(param), param[0]));
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
        dsp->state = DspState_Closing;                               \
        return;                                                      \
    }

    xaf_comp_status comp_status;
    int comp_info[4];
    XAF_ERR_CODE err_code;
    xaf_format_t format;
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

    xaf_comp_config_default_init(&comp_config);

    comp_config.comp_id            = "capturer_dmic";
    comp_config.num_input_buffers  = 0;
    comp_config.num_output_buffers = 0;
    comp_config.pp_inbuf           = NULL;
    comp_config.comp_type          = XAF_CAPTURER;

    err_code = xaf_comp_create(dsp->audio_device, &dsp->comp_capturer, &comp_config);
    CHECK_ERROR(err_code, "xaf_comp_create capturer");

    format.pcm_width = 16;
    format.channels = 1;
    format.sample_rate = 16000;

    /* Configure sample rate converter component */
    err_code = dsp_xaf_capturer_setup(dsp->comp_capturer, &format);
    CHECK_ERROR(err_code, "capturer_setup");

    DSP_PRINTF("[dsp_xaf] Capturer component created\r\n");

    /* Start capturer component */
    err_code = xaf_comp_process(dsp->audio_device, dsp->comp_capturer, NULL, 0, XAF_START_FLAG);
    CHECK_ERROR(err_code, "xaf_comp_process XAF_START_FLAG capturer");

    err_code = xaf_comp_get_status(dsp->audio_device, dsp->comp_capturer, &comp_status, &comp_info[0]);
    CHECK_ERROR(err_code, "xaf_comp_get_status capturer");

    DSP_PRINTF("[dsp_xaf] Capturer component %s\r\n", (comp_status != XAF_INIT_DONE) ? "start failed!" : "started");

    if (dsp->mode == DspMode_Test)
    {
        xaf_comp_config_default_init(&comp_config);

        comp_config.comp_id            = "renderer";
        comp_config.num_input_buffers  = 0;
        comp_config.num_output_buffers = 0;
        comp_config.pp_inbuf           = NULL;
        comp_config.comp_type          = XAF_RENDERER;

        err_code = xaf_comp_create(dsp->audio_device, &dsp->comp_renderer, &comp_config);
        CHECK_ERROR(err_code, "xaf_comp_create renderer");

        /* Get audio format from the input component */
        err_code = dsp_xaf_get_comp_config(dsp->comp_capturer, &format);
        CHECK_ERROR(err_code, "get_comp_config capturer");

        /* Setup renderer to match PCM format */
        err_code = dsp_xaf_renderer_setup(dsp->comp_renderer, &format);
        CHECK_ERROR(err_code, "renderer_setup");

        DSP_PRINTF("[dsp_xaf] Renderer component created\r\n");

        err_code = xaf_connect(dsp->comp_capturer, 0, dsp->comp_renderer, 0, 4);
        CHECK_ERROR(err_code, "xaf_connect capturer -> renderer");

        DSP_PRINTF("[dsp_xaf] Connected capturer -> renderer\r\n");
    }

    err_code = dsp_xaf_capturer_start(dsp->comp_capturer);
    CHECK_ERROR(err_code, "capturer_start");

    if (dsp->mode == DspMode_Test)
    {
        err_code = xaf_comp_process(dsp->audio_device, dsp->comp_renderer, NULL, 0, XAF_START_FLAG);
        CHECK_ERROR(err_code, "xaf_comp_process XAF_START_FLAG renderer");

        err_code = xaf_comp_get_status(dsp->audio_device, dsp->comp_renderer, &comp_status, &comp_info[0]);
        CHECK_ERROR(err_code, "xaf_comp_get_status renderer");

        err_code = dsp_xaf_renderer_start(dsp->comp_renderer);
        CHECK_ERROR(err_code, "renderer_start");
    }
    else
    {
        /* ...create micro speech component */
        xaf_comp_config_default_init(&comp_config);

        comp_config.comp_id            = "post-proc/microspeech_fe";
        comp_config.num_input_buffers  = 0;
        comp_config.num_output_buffers = 0;
        comp_config.pp_inbuf           = NULL;
        comp_config.comp_type          = XAF_POST_PROC;

        err_code = xaf_comp_create(dsp->audio_device, &dsp->comp_microspeech_fe, &comp_config);
        CHECK_ERROR(err_code, "xaf_comp_create microspeech fe");

        err_code = dsp_xaf_microspeech_fe_setup(dsp->comp_microspeech_fe, &format);
        CHECK_ERROR(err_code, "microspeech_fe_setup");

        xaf_comp_config_default_init(&comp_config);

        comp_config.comp_id            = "post-proc/microspeech_inference";
        comp_config.num_input_buffers  = 0;
        comp_config.num_output_buffers = 1;
        comp_config.pp_inbuf           = NULL;
        comp_config.comp_type          = XAF_POST_PROC;

        err_code = xaf_comp_create(dsp->audio_device, &dsp->comp_inference, &comp_config);
        CHECK_ERROR(err_code, "xaf_comp_create inference");

        err_code = dsp_xaf_inference_setup(dsp->comp_inference, &format);
        CHECK_ERROR(err_code, "microspeech_fe_setup");

        err_code = xaf_connect(dsp->comp_capturer, 0, dsp->comp_microspeech_fe, 0, 4);
        CHECK_ERROR(err_code, "xaf_connect capturer -> microspeech FE");

        DSP_PRINTF("[dsp_xaf] Connected capturer -> microspeech FE\r\n");

        err_code = xaf_comp_process(dsp->audio_device, dsp->comp_microspeech_fe, NULL, 0, XAF_START_FLAG);
        CHECK_ERROR(err_code, "xaf_comp_process XAF_START_FLAG microspeech FE");

        err_code = xaf_comp_get_status(dsp->audio_device, dsp->comp_microspeech_fe, &comp_status, &comp_info[0]);
        CHECK_ERROR(err_code, "xaf_comp_get_status microspeech FE");

        DSP_PRINTF("[dsp_xaf] Microspeech FE component %s\r\n", (comp_status != XAF_INIT_DONE) ? "start failed!" : "started");

        err_code = xaf_connect(dsp->comp_microspeech_fe, 1, dsp->comp_inference, 0, 4);
        CHECK_ERROR(err_code, "xaf_connect microspeech fe -> inference");

        DSP_PRINTF("[dsp_xaf] Connected microspeech FE -> inference\r\n");

        err_code = xaf_comp_process(dsp->audio_device, dsp->comp_inference, NULL, 0, XAF_START_FLAG);
        CHECK_ERROR(err_code, "xaf_comp_process XAF_START_FLAG inference");

        err_code = xaf_comp_get_status(dsp->audio_device, dsp->comp_inference, &comp_status, &comp_info[0]);
        CHECK_ERROR(err_code, "xaf_comp_get_status inference");

        DSP_PRINTF("[dsp_xaf] Inference component %s\r\n", (comp_status != XAF_INIT_DONE) ? "start failed!" : "started");

        err_code = xaf_comp_process(dsp->audio_device, dsp->comp_inference, NULL, 0, XAF_EXEC_FLAG);
        CHECK_ERROR(err_code, "xaf_comp_process XAF_EXEC_FLAG inference");

        DSP_PRINTF("\r\n");
    }

    dsp->state = DspState_Running;
}

/*! @brief Executed in Running state */
static void dsp_xaf_process_running(dsp_handle_t *dsp)
{
    xaf_comp_status comp_status;
    XAF_ERR_CODE err_code;
    int32_t comp_info[4];

    if (dsp->mode == DspMode_Inference)
    {
        err_code = xaf_comp_get_status(dsp->audio_device, dsp->comp_inference, &comp_status, &comp_info[0]);
        CHECK_ERROR(err_code, "xaf_comp_get_status inference");

        if (comp_status == XAF_OUTPUT_READY)
        {
            void *p_buf = (void *)comp_info[0];
            int size = comp_info[1];

            memcpy(dsp->inference_results, p_buf, size);
            xos_sem_put(&dsp->inference_done);

            err_code = xaf_comp_process(dsp->audio_device, dsp->comp_inference, (void*)comp_info[0], comp_info[1], XAF_NEED_OUTPUT_FLAG);
            CHECK_ERROR(err_code, "xaf_comp_process XAF_NEED_OUTPUT_FLAG inference");
        }
    }

    xos_thread_sleep_msec(1);
}

/*! @brief Executed in Closing state */
static void dsp_xaf_process_closing(dsp_handle_t *dsp)
{
    xaf_adev_close_flag close_flag = XAF_ADEV_NORMAL_CLOSE;
    XAF_ERR_CODE err_code;
    void** components[] = {
        &dsp->comp_capturer,
        &dsp->comp_renderer,
        &dsp->comp_microspeech_fe,
        &dsp->comp_inference
    };

    /* Clean up and shut down XAF */

    DSP_PRINTF("[dsp_xaf] Closing\r\n");

    for (int i = 0; i < sizeof(components) / sizeof(void**); i++)
    {
        if (*components[i] != NULL)
        {
            err_code       = xaf_comp_delete(*components[i]);
            *components[i] = NULL;
            if (err_code != XAF_NO_ERR)
            {
                DSP_PRINTF("[dsp_xaf] xaf_comp_delete failure: %d\r\n", err_code);
                close_flag = XAF_ADEV_FORCE_CLOSE;
            }
        }
    }

    err_code = xaf_adev_close(dsp->audio_device, close_flag);
    if (err_code != XAF_NO_ERR)
    {
        DSP_PRINTF("[dsp_xaf] xaf_adev_close failure: %d\r\n", err_code);
    }

    DSP_PRINTF("[dsp_xaf] Audio device closed\r\n\r\n");

    dsp->state = DspState_Idle;
}

static int32_t dsp_xaf_process_thread(void *arg, int32_t wake_value)
{
    dsp_handle_t *dsp = (dsp_handle_t *)arg;
    int time_us = 0;

    DSP_PRINTF("[dsp_xaf] Start\r\n");

    dsp->state = DspState_Creating;
    dsp->mode = DspMode_Test;

    while (true)
    {
        switch (dsp->state)
        {
            case DspState_Creating:
                dsp_xaf_process_creating(dsp);
                break;
            case DspState_Running:
                dsp_xaf_process_running(dsp);
                time_us = xos_get_system_cycles() / (xos_get_clock_freq() / (1000 * 1000));
                /* Change mode from audio testing to inference after 2 seconds */
                if ((time_us > 2 * 1000 * 1000) && (dsp->mode == DspMode_Test))
                {
                    dsp->state = DspState_Closing;
                }
                break;
            case DspState_Closing:
                dsp_xaf_process_closing(dsp);
                break;
            case DspState_Idle:
                if (dsp->mode == DspMode_Test)
                {
                    dsp->state = DspState_Creating;
                    dsp->mode = DspMode_Inference;
                }
                break;
            default:
                break;
        }
    }
}
