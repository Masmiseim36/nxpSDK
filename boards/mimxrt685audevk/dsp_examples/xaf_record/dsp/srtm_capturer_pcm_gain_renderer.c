/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <xtensa/xos.h>

#include "fsl_i2s.h"

#include "xaf-utils-test.h"
#include "audio/xa-renderer-api.h"
#include "audio/xa-pcm-gain-api.h"
#include "audio/xa-capturer-api.h"

#include "dsp_config.h"
#include "srtm_utils.h"

#if XA_VIT_PRE_PROC
#if XA_VOICE_SEEKER
#include "voice_seeker.h"
#endif
#include "vit_pre_proc.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AUDIO_FRMWK_BUF_SIZE (32 * 1024)
#define AUDIO_COMP_BUF_SIZE  (140 * 1024)

enum
{
    XA_COMP = -1,
    XA_CAPTURER_0,
    XA_GAIN_0,
#if XA_VIT_PRE_PROC
#if XA_VOICE_SEEKER
    XA_VOICE_SEEKER_0,
#endif

    XA_VIT_PRE_PROC_0,
#endif
    XA_RENDERER_0,
    NUM_COMP_IN_GRAPH,
};

const int comp_create_order[] = {XA_CAPTURER_0, XA_GAIN_0,
#if XA_VIT_PRE_PROC
#if XA_VOICE_SEEKER
                                 XA_VOICE_SEEKER_0,
#endif
                                 XA_VIT_PRE_PROC_0,
#endif
                                 XA_RENDERER_0};

#define MAX_INP_STRMS 1
#define MAX_OUT_STRMS 1
#define MIN_INP_STRMS 1
#define MAX_CHANNELS  8
#define MAX_WIDTH     32

#if XA_VOICE_SEEKER
#define FRAME_SIZE_MS (30)
#else
#define FRAME_SIZE_MS (10)
#endif

/*******************************************************************************
 * Component Setup/ Config
 ******************************************************************************/
static int renderer_setup(void *p_renderer, xaf_format_t *format, bool i2s)
{
    int param[26];
    int numParam;

#if XA_VIT_PRE_PROC
    int numChannels = 1;
#else
    int numChannels = format->channels;
#endif

    numParam = 7;

    param[0]  = XA_RENDERER_CONFIG_PARAM_PCM_WIDTH;
    param[1]  = format->pcm_width;
    param[2]  = XA_RENDERER_CONFIG_PARAM_CHANNELS;
    param[3]  = numChannels;
    param[4]  = XA_RENDERER_CONFIG_PARAM_SAMPLE_RATE;
    param[5]  = format->sample_rate;
    param[6]  = XA_RENDERER_CONFIG_PARAM_FRAME_SIZE;
    param[7]  = (format->pcm_width / 8) * numChannels * (format->sample_rate * FRAME_SIZE_MS / 1000);
    param[8]  = XA_RENDERER_CONFIG_PARAM_AUDIO_BUFFER_1;
    param[9]  = (int)DSP_AUDIO_BUFFER_1_PING;
    param[10] = XA_RENDERER_CONFIG_PARAM_AUDIO_BUFFER_2;
    param[11] = (int)DSP_AUDIO_BUFFER_1_PONG;
    param[12] = XA_RENDERER_CONFIG_PARAM_I2S_INTERFACE;
    param[13] = AUDIO_I2S_RENDERER_DEVICE;

    if (numChannels > 2)
    {
        numParam = 13;

        param[14] = XA_RENDERER_CONFIG_PARAM_I2S_MODE;
        param[15] = kI2S_ModeDspWsShort;
        param[16] = XA_RENDERER_CONFIG_PARAM_I2S_WS_POLARITY;
        param[17] = true;
        param[18] = XA_RENDERER_CONFIG_PARAM_I2S_POSITION;
        param[19] = 1U;
        param[20] = XA_RENDERER_CONFIG_PARAM_CODEC_PCM_WIDTH;
        param[21] = MAX_WIDTH;
        param[22] = XA_RENDERER_CONFIG_PARAM_CODEC_CHANNELS;
        param[23] = MAX_CHANNELS;
        param[24] = XA_RENDERER_CONFIG_PARAM_CODEC_FRAME_SIZE;
        param[25] = (MAX_WIDTH >> 3) * MAX_CHANNELS * (format->sample_rate * FRAME_SIZE_MS / 1000);
    }

    return xaf_comp_set_config(p_renderer, numParam, &param[0]);
}

#if XA_VIT_PRE_PROC
#if XA_VOICE_SEEKER
static int voice_seeker_setup(void *p_comp, xaf_format_t *format, bool i2s)
{
    int param[8];

    param[0] = XA_VOICE_SEEKER_CONFIG_PARAM_CHANNELS;
    param[1] = format->channels;
    param[2] = XA_VOICE_SEEKER_CONFIG_PARAM_SAMPLE_RATE;
    param[3] = format->sample_rate;
    param[4] = XA_VOICE_SEEKER_CONFIG_PARAM_PCM_WIDTH;
    param[5] = format->pcm_width;
    param[6] = XA_VOICE_SEEKER_CONFIG_PARAM_INPUT_FRAME_SIZE_US;
    param[7] = FRAME_SIZE_MS * 1000;

    return xaf_comp_set_config(p_comp, 4, &param[0]);
}
#endif
static int vit_pre_proc_setup(void *p_comp, xaf_format_t *format, bool i2s)
{
    int param[6];

    param[0] = XA_VIT_PRE_PROC_CONFIG_PARAM_SAMPLE_RATE;
    param[1] = format->sample_rate;
    param[2] = XA_VIT_PRE_PROC_CONFIG_PARAM_PCM_WIDTH;
    param[3] = format->pcm_width;
    param[4] = XA_VIT_PRE_PROC_CONFIG_PARAM_INPUT_FRAME_SIZE_US;
    param[5] = FRAME_SIZE_MS * 1000;

    return xaf_comp_set_config(p_comp, 3, &param[0]);
}
#endif

static int pcm_gain_setup(void *p_comp, xaf_format_t *format, bool i2s)
{
    int param[10];

    param[0] = XA_PCM_GAIN_CONFIG_PARAM_CHANNELS;
    param[1] = format->channels;
    param[2] = XA_PCM_GAIN_CONFIG_PARAM_SAMPLE_RATE;
    param[3] = format->sample_rate;
    param[4] = XA_PCM_GAIN_CONFIG_PARAM_PCM_WIDTH;
    param[5] = format->pcm_width;
    param[6] = XA_PCM_GAIN_CONFIG_PARAM_FRAME_SIZE;
    param[7] = (format->pcm_width >> 3) * format->channels * (format->sample_rate * FRAME_SIZE_MS / 1000);
    param[8] = XA_PCM_GAIN_CONFIG_PARAM_GAIN_FACTOR;
    param[9] = 1;

    return xaf_comp_set_config(p_comp, 5, &param[0]);
}

static int capturer_setup(void *p_capturer, xaf_format_t *format, bool i2s)
{
    int param[22];
    int num_params = 3;

    param[0] = XA_CAPTURER_CONFIG_PARAM_PCM_WIDTH;
    param[1] = format->pcm_width;
    param[2] = XA_CAPTURER_CONFIG_PARAM_CHANNELS;
    param[3] = format->channels;
    param[4] = XA_CAPTURER_CONFIG_PARAM_SAMPLE_RATE;
    param[5] = format->sample_rate;

    if (i2s)
    {
        num_params = 11;

        param[8]  = XA_CAPTURER_CONFIG_PARAM_I2S_MASTER_SLAVE;
        param[9]  = 0;
        param[10] = XA_CAPTURER_CONFIG_PARAM_I2S_MODE;
        param[11] = (int)kI2S_ModeI2sClassic;
        param[12] = XA_CAPTURER_CONFIG_PARAM_I2S_SCK_POLARITY;
        param[13] = 0;
        param[14] = XA_CAPTURER_CONFIG_PARAM_I2S_WS_POLARITY;
        param[15] = 0;
        param[16] = XA_CAPTURER_CONFIG_PARAM_AUDIO_BUFFER_1;
        param[17] = (int)DSP_AUDIO_BUFFER_2_PING;
        param[18] = XA_CAPTURER_CONFIG_PARAM_AUDIO_BUFFER_2;
        param[19] = (int)DSP_AUDIO_BUFFER_2_PONG;
        param[20] = XA_CAPTURER_CONFIG_PARAM_I2S_INTERFACE;
        param[21] = AUDIO_I2S_CAPTURER_DEVICE;
    }

    return xaf_comp_set_config(p_capturer, num_params, &param[0]);
}

/* Explicitly start the capturer component.
 * This will initiate the I2S DMA input. */
static int capturer_start_operation(void *p_comp)
{
    int param[2];

    param[0] = XA_CAPTURER_CONFIG_PARAM_STATE;
    param[1] = XA_CAPTURER_STATE_START;

    return xaf_comp_set_config(p_comp, 1, &param[0]);
}

/* Explicitly start the renderer component.
 * This will begin the I2S DMA output with zeros until valid data comes in. */
static int renderer_start_operation(void *p_comp)
{
    int param[2];

    param[0] = XA_RENDERER_CONFIG_PARAM_STATE;
    param[1] = XA_RENDERER_STATE_START;

    return xaf_comp_set_config(p_comp, 1, &param[0]);
}

/*******************************************************************************
 * Commands processing
 ******************************************************************************/
int srtm_capturer_gain_renderer_init(unsigned int *pCmdParams, bool i2s)
{
    void *p_adev = NULL;
    void *p_comp[NUM_COMP_IN_GRAPH];
    xf_id_t comp_id[NUM_COMP_IN_GRAPH];
    int i, k, cid;
    int (*comp_setup[NUM_COMP_IN_GRAPH])(void *p_comp, xaf_format_t *, bool);
    xaf_comp_type comp_type[NUM_COMP_IN_GRAPH];
    xaf_format_t comp_format[NUM_COMP_IN_GRAPH];
    xaf_comp_status comp_status;
    int comp_ninbuf[NUM_COMP_IN_GRAPH];
    int comp_noutbuf[NUM_COMP_IN_GRAPH];
    int param[4], renderer_state;
    int channels, sampling_rate, width;
    int info[4];
    XAF_ERR_CODE ret;
    xaf_adev_config_t device_config;
    xaf_comp_config_t comp_config[NUM_COMP_IN_GRAPH];

    channels      = pCmdParams[0];
    sampling_rate = pCmdParams[1];
    width         = pCmdParams[2];

    for (i = 0; i < NUM_COMP_IN_GRAPH; i++)
    {
        cid         = comp_create_order[i];
        p_comp[cid] = NULL;
        memset(&comp_format[cid], 0, sizeof(xaf_format_t));
        comp_setup[cid]   = NULL;
        comp_type[cid]    = -1;
        comp_id[cid]      = NULL;
        comp_ninbuf[cid]  = 0;
        comp_noutbuf[cid] = 0;

        switch (cid)
        {
            case XA_CAPTURER_0:
                comp_format[cid].sample_rate = sampling_rate;
                comp_format[cid].channels    = channels;
                comp_format[cid].pcm_width   = width;
                comp_setup[cid]              = capturer_setup;
                comp_type[cid]               = XAF_CAPTURER;
                if (i2s)
                {
                    comp_id[cid] = "capturer_i2s";
                }
                else
                {
                    comp_id[cid] = "capturer_dmic";
                }
                break;

#if XA_VIT_PRE_PROC
#if XA_VOICE_SEEKER
            case XA_VOICE_SEEKER_0:
                comp_format[cid].sample_rate = sampling_rate;
                comp_format[cid].channels    = channels;
                comp_format[cid].pcm_width   = width;
                comp_setup[cid]              = voice_seeker_setup;
                comp_type[cid]               = XAF_PRE_PROC;
                comp_id[cid]                 = "pre-proc/voice_seeker";
                break;
#endif
            case XA_VIT_PRE_PROC_0:
                comp_format[cid].sample_rate = sampling_rate;
                comp_format[cid].channels    = channels;
                comp_format[cid].pcm_width   = width;
                comp_setup[cid]              = vit_pre_proc_setup;
                comp_type[cid]               = XAF_PRE_PROC;
                comp_id[cid]                 = "pre-proc/vit_pre_proc";
                break;
#endif
            case XA_GAIN_0:
                comp_format[cid].sample_rate = sampling_rate;
                comp_format[cid].channels    = channels;
                comp_format[cid].pcm_width   = width;
                comp_setup[cid]              = pcm_gain_setup;
                comp_type[cid]               = XAF_POST_PROC;
                comp_id[cid]                 = "post-proc/pcm_gain";
                break;
            case XA_RENDERER_0:
                comp_format[cid].sample_rate = sampling_rate;
                comp_format[cid].channels    = channels;
                comp_format[cid].pcm_width   = width;
                comp_setup[cid]              = renderer_setup;
                comp_type[cid]               = XAF_RENDERER;
                comp_id[cid]                 = "renderer";
                break;

            default:
                DSP_PRINTF("Check component type in comp_create_order table\n\r");
                return -1;
        }
    }

    xaf_adev_config_default_init(&device_config);

    device_config.pmem_malloc                 = DSP_Malloc;
    device_config.pmem_free                   = DSP_Free;
    device_config.audio_component_buffer_size = AUDIO_COMP_BUF_SIZE;
    device_config.audio_framework_buffer_size = AUDIO_FRMWK_BUF_SIZE;

    ret = xaf_adev_open(&p_adev, &device_config);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_adev_open failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("\nAudio Device Ready\n\r");

    /* Create and setup all components */
    for (k = 0; k < NUM_COMP_IN_GRAPH; k++)
    {
        cid = comp_create_order[k];

        xaf_comp_config_default_init(&comp_config[cid]);

        comp_config[cid].comp_id            = comp_id[cid];
        comp_config[cid].num_input_buffers  = comp_ninbuf[cid];
        comp_config[cid].num_output_buffers = comp_noutbuf[cid];
        comp_config[cid].comp_type          = comp_type[cid];

        ret = xaf_comp_create(p_adev, &p_comp[cid], &comp_config[cid]);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("xaf_comp_create[%d] failure: %d\r\n", k, ret);
            return -1;
        }

        ret = comp_setup[cid](p_comp[cid], &comp_format[cid], i2s);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("comp_setup failure: %d\r\n", ret);
            return -1;
        }
    }

    /* Start capturer */
    ret = xaf_comp_process(p_adev, p_comp[XA_CAPTURER_0], NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_process XAF_START_FLAG CAPTURER_0 failure: %d\r\n", ret);
        return -1;
    }

    ret = xaf_comp_get_status(p_adev, p_comp[XA_CAPTURER_0], &comp_status, &info[0]);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_get_status CAPTURER_0 failure: %d\r\n", ret);
        return -1;
    }

    /* Start renderer component */
    ret = xaf_comp_process(p_adev, p_comp[XA_RENDERER_0], NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_process XAF_START_FLAG RENDERER_0 failure: %d\r\n", ret);
        return -1;
    }

    ret = xaf_comp_get_status(p_adev, p_comp[XA_RENDERER_0], &comp_status, &info[0]);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_get_status RENDERER_0 failure: %d\r\n", ret);
        return -1;
    }

    /* Connect all the non-input components and then only START each of the dest components */
    ret = xaf_connect(p_comp[XA_CAPTURER_0], 0, p_comp[XA_GAIN_0], 0, 4);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_connect CAPTURER_0 -> GAIN_0 failure: %d\r\n", ret);
        return -1;
    }
    ret = capturer_start_operation(p_comp[XA_CAPTURER_0]);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("Capturer start operation failure.\r\n");
        return -1;
    }

    DSP_PRINTF("connected CAPTURER -> GAIN_0\n\r");

    /* Start PCM gain */
    ret = xaf_comp_process(p_adev, p_comp[XA_GAIN_0], NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_process XAF_START_FLAG GAIN_0 failure: %d\r\n", ret);
        return -1;
    }

    ret = xaf_comp_get_status(p_adev, p_comp[XA_GAIN_0], &comp_status, &info[0]);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_get_status GAIN_0 failure: %d\r\n", ret);
        return -1;
    }

#if XA_VIT_PRE_PROC
#if XA_VOICE_SEEKER
    ret = xaf_connect(p_comp[XA_GAIN_0], 1, p_comp[XA_VOICE_SEEKER_0], 0, 4);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_connect XA_GAIN_0 -> XA_VOICE_SEEKER_0 failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("connected XA_GAIN_0 -> XA_VOICE_SEEKER_0\n\r");

    /* Start VoiceSeeker pre processing */
    ret = xaf_comp_process(p_adev, p_comp[XA_VOICE_SEEKER_0], NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_process XAF_START_FLAG XA_VOICE_SEEKER_0 failure: %d\r\n", ret);
        return -1;
    }

    ret = xaf_comp_get_status(p_adev, p_comp[XA_VOICE_SEEKER_0], &comp_status, &info[0]);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_get_status XA_VOICE_SEEKER_0 failure: %d\r\n", ret);
        return -1;
    }

    ret = xaf_connect(p_comp[XA_VOICE_SEEKER_0], 1, p_comp[XA_VIT_PRE_PROC_0], 0, 4);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_connect XA_VOICE_SEEKER_0 -> XA_VIT_PRE_PROC_0 failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("connected XA_VOICE_SEEKER_0 -> XA_VIT_PRE_PROC_0\n\r");

#else
    ret = xaf_connect(p_comp[XA_GAIN_0], 1, p_comp[XA_VIT_PRE_PROC_0], 0, 4);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_connect XA_GAIN_0 -> VIT_PRE_PROC_0 failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("connected XA_GAIN_0 -> XA_VIT_PRE_PROC_0\n\r");
#endif

    /* Start VIT pre processing */
    ret = xaf_comp_process(p_adev, p_comp[XA_VIT_PRE_PROC_0], NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_process XAF_START_FLAG VIT_PRE_PROC_0 failure: %d\r\n", ret);
        return -1;
    }

    ret = xaf_comp_get_status(p_adev, p_comp[XA_VIT_PRE_PROC_0], &comp_status, &info[0]);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_get_status VIT_PRE_PROC_0 failure: %d\r\n", ret);
        return -1;
    }

    /* Start renderer DMA output.  Will output I2S zeros until valid data is
     * available. */
    renderer_start_operation(p_comp[XA_RENDERER_0]);

    ret = xaf_connect(p_comp[XA_VIT_PRE_PROC_0], 1, p_comp[XA_RENDERER_0], 0, 4);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_connect VIT_PRE_PROC_0 -> RENDERER_0 failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("connected XA_VIT_PRE_PROC_0 -> XA_RENDERER_0\n\r");
#else

    /* Start renderer DMA output.  Will output I2S zeros until valid data is
     * available. */
    renderer_start_operation(p_comp[XA_RENDERER_0]);

    ret = xaf_connect(p_comp[XA_GAIN_0], 1, p_comp[XA_RENDERER_0], 0, 4);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_connect GAIN_0 -> RENDERER_0 failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("connected XA_GAIN_0 -> XA_RENDERER_0\n\r");
#endif

    while (1)
    {
        /* Sleep 100 msec */
        xos_thread_sleep_msec(100);

        /* Check renderer for errors */
        cid            = XA_RENDERER_0;
        param[0]       = XA_RENDERER_CONFIG_PARAM_STATE;
        param[2]       = XA_RENDERER_CONFIG_PARAM_BYTES_PRODUCED;
        ret            = xaf_comp_get_config(p_comp[cid], 2, &param[0]);
        renderer_state = param[1];

        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("renderer get_config error:%x\n", ret);
            return -1;
        }
    }

    for (k = 0; k < NUM_COMP_IN_GRAPH; k++)
    {
        cid = comp_create_order[k];
        ret = xaf_comp_delete(p_comp[cid]);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("xaf_comp_delete[%d] failure: %d\r\n", k, ret);
            return -1;
        }
    }

    xaf_adev_close(p_adev, XAF_ADEV_NORMAL_CLOSE);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_adev_close failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("Audio device closed\n\n\r\r");

    return 0;
}
