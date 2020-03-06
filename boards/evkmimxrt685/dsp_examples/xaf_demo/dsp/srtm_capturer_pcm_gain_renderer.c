/*
 * Copyright 2018-2019 NXP
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

#include "xaf-api.h"
#include "audio/xa-renderer-api.h"
#include "audio/xa-pcm-gain-api.h"
#include "audio/xa-capturer-api.h"

#include "srtm_utils.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AUDIO_FRMWK_BUF_SIZE (128 * 1024)
#define AUDIO_COMP_BUF_SIZE (512 * 1024)

enum
{
    XA_COMP = -1,
    XA_CAPTURER_0,
    XA_GAIN_0,
    XA_RENDERER_0,
    NUM_COMP_IN_GRAPH,
};

const int comp_create_order[] = {XA_CAPTURER_0, XA_GAIN_0, XA_RENDERER_0};

#define MAX_INP_STRMS 1
#define MAX_OUT_STRMS 1
#define MIN_INP_STRMS 1

#define RENDERER_FRAME_SIZE (4 * 1024)
#define PCM_GAIN_FRAME_SIZE (4 * 1024)
#define CAPTURER_FRAME_SIZE (4 * 1024)

/*******************************************************************************
 * Component Setup/ Config
 ******************************************************************************/
static int renderer_setup(void *p_renderer, xaf_format_t *format, bool i2s)
{
    int param[20];

    param[0]  = XA_RENDERER_CONFIG_PARAM_PCM_WIDTH;
    param[1]  = format->pcm_width;
    param[2]  = XA_RENDERER_CONFIG_PARAM_CHANNELS;
    param[3]  = format->channels;
    param[4]  = XA_RENDERER_CONFIG_PARAM_SAMPLE_RATE;
    param[5]  = format->sample_rate;
    param[6]  = XA_RENDERER_CONFIG_PARAM_FRAME_SIZE;
    param[7]  = RENDERER_FRAME_SIZE;
    param[8]  = XA_RENDERER_CONFIG_PARAM_I2S_MASTER_SLAVE;
    param[9]  = 1;
    param[10] = XA_RENDERER_CONFIG_PARAM_I2S_MODE;
    param[11] = (int)kI2S_ModeI2sClassic;
    param[12] = XA_RENDERER_CONFIG_PARAM_I2S_SCK_POLARITY;
    param[13] = 0;
    param[14] = XA_RENDERER_CONFIG_PARAM_I2S_WS_POLARITY;
    param[15] = 0;

    return xaf_comp_set_config(p_renderer, 8, &param[0]);
}

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
    param[7] = PCM_GAIN_FRAME_SIZE;
    param[8] = XA_PCM_GAIN_CONFIG_PARAM_GAIN_FACTOR;
    param[9] = 1;

    return xaf_comp_set_config(p_comp, 5, &param[0]);
}

static int capturer_setup(void *p_capturer, xaf_format_t *format, bool i2s)
{
    int param[20];
    int num_params = 4;

    param[0] = XA_CAPTURER_CONFIG_PARAM_PCM_WIDTH;
    param[1] = format->pcm_width;
    param[2] = XA_CAPTURER_CONFIG_PARAM_CHANNELS;
    param[3] = format->channels;
    param[4] = XA_CAPTURER_CONFIG_PARAM_SAMPLE_RATE;
    param[5] = format->sample_rate;
    param[6] = XA_CAPTURER_CONFIG_PARAM_FRAME_SIZE;
    param[7] = CAPTURER_FRAME_SIZE;

    if (i2s)
    {
        num_params = 8;

        param[8]  = XA_CAPTURER_CONFIG_PARAM_I2S_MASTER_SLAVE;
        param[9]  = 0;
        param[10] = XA_CAPTURER_CONFIG_PARAM_I2S_MODE;
        param[11] = (int)kI2S_ModeI2sClassic;
        param[12] = XA_CAPTURER_CONFIG_PARAM_I2S_SCK_POLARITY;
        param[13] = 0;
        param[14] = XA_CAPTURER_CONFIG_PARAM_I2S_WS_POLARITY;
        param[15] = 0;
    }

    return xaf_comp_set_config(p_capturer, num_params, &param[0]);
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

    ret = xaf_adev_open(&p_adev, AUDIO_FRMWK_BUF_SIZE, AUDIO_COMP_BUF_SIZE, DSP_Malloc, DSP_Free);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_adev_open failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("\nAudio Device Ready\n\r");

    /* Create and setup all components */
    for (k = 0; k < NUM_COMP_IN_GRAPH; k++)
    {
        cid = comp_create_order[k];

        ret = xaf_comp_create(p_adev, &p_comp[cid], comp_id[cid], comp_ninbuf[cid], comp_noutbuf[cid], NULL,
                              comp_type[cid]);
        if (ret != XAF_NO_ERROR)
        {
            DSP_PRINTF("xaf_comp_create[%d] failure: %d\r\n", k, ret);
            return -1;
        }

        comp_setup[cid](p_comp[cid], &comp_format[cid], i2s);
    }

    /* Start capturer */
    ret = xaf_comp_process(p_adev, p_comp[XA_CAPTURER_0], NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_comp_process XAF_START_FLAG CAPTURER_0 failure: %d\r\n", ret);
        return -1;
    }

    ret = xaf_comp_get_status(p_adev, p_comp[XA_CAPTURER_0], &comp_status, &info[0]);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_comp_get_status CAPTURER_0 failure: %d\r\n", ret);
        return -1;
    }

    /* Start renderer component */
    ret = xaf_comp_process(p_adev, p_comp[XA_RENDERER_0], NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_comp_process XAF_START_FLAG RENDERER_0 failure: %d\r\n", ret);
        return -1;
    }

    ret = xaf_comp_get_status(p_adev, p_comp[XA_RENDERER_0], &comp_status, &info[0]);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_comp_get_status RENDERER_0 failure: %d\r\n", ret);
        return -1;
    }

    /* Connect all the non-input components and then only START each of the dest components */
    ret = xaf_connect(p_comp[XA_CAPTURER_0], 0, p_comp[XA_GAIN_0], 0, 4);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_connect CAPTURER_0 -> GAIN_0 failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("connected CAPTURER -> GAIN_0\n\r");

    /* Start PCM gain */
    ret = xaf_comp_process(p_adev, p_comp[XA_GAIN_0], NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_comp_process XAF_START_FLAG GAIN_0 failure: %d\r\n", ret);
        return -1;
    }

    ret = xaf_comp_get_status(p_adev, p_comp[XA_GAIN_0], &comp_status, &info[0]);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_comp_get_status GAIN_0 failure: %d\r\n", ret);
        return -1;
    }

    /* Start renderer DMA output.  Will output I2S zeros until valid data is
     * available. */
    renderer_start_operation(p_comp[XA_RENDERER_0]);

    ret = xaf_connect(p_comp[XA_GAIN_0], 1, p_comp[XA_RENDERER_0], 0, 4);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_connect GAIN_0 -> RENDERER_0 failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("connected XA_GAIN_0 -> XA_RENDERER_0\n\r");

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

        if (ret != XAF_NO_ERROR)
        {
            DSP_PRINTF("renderer get_config error:%x\n", ret);
            return -1;
        }
    }

    for (k = 0; k < NUM_COMP_IN_GRAPH; k++)
    {
        cid = comp_create_order[k];
        ret = xaf_comp_delete(p_comp[cid]);
        if (ret != XAF_NO_ERROR)
        {
            DSP_PRINTF("xaf_comp_delete[%d] failure: %d\r\n", k, ret);
            return -1;
        }
    }

    xaf_adev_close(p_adev, XAF_ADEV_NORMAL_CLOSE);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_adev_close failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("Audio device closed\n\n\r\r");

    return 0;
}
