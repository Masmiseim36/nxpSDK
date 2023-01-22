/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <xtensa/xos.h>

#include "xaf-utils-test.h"

#include "audio/xa-pcm-gain-api.h"

#include "srtm_utils.h"

#include "fsl_common.h"
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
#include "fsl_memory.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AUDIO_FRMWK_BUF_SIZE (256 * 256)
#define AUDIO_COMP_BUF_SIZE  (128 * 1024)

// component parameters
static unsigned int SRTM_PCM_GAIN_SAMPLE_WIDTH = 16;
// supports only 16-bit PCM
static unsigned int SRTM_PCM_GAIN_NUM_CH = 1;
// supports 1 and 2 channels only
static unsigned int SRTM_PCM_GAIN_IDX_FOR_GAIN = 4;
// gain index range is 0 to 6 -> {0db, -6db, -12db, -18db, 6db, 12db, 18db}
static unsigned int SRTM_PCM_GAIN_SAMPLE_RATE = 44100;

/*******************************************************************************
 * Component Setup/ Config
 ******************************************************************************/
static int pcm_gain_setup(void *p_comp)
{
    int param[10];
    int pcm_width   = SRTM_PCM_GAIN_SAMPLE_WIDTH; // supports only 16-bit PCM
    int num_ch      = SRTM_PCM_GAIN_NUM_CH;       // supports 1 and 2 channels only
    int sample_rate = SRTM_PCM_GAIN_SAMPLE_RATE;
    int gain_idx =
        SRTM_PCM_GAIN_IDX_FOR_GAIN; // gain index range is 0 to 6 -> {0db, -6db, -12db, -18db, 6db, 12db, 18db}
    int frame_size = XAF_INBUF_SIZE;

    param[0] = XA_PCM_GAIN_CONFIG_PARAM_CHANNELS;
    param[1] = num_ch;
    param[2] = XA_PCM_GAIN_CONFIG_PARAM_SAMPLE_RATE;
    param[3] = sample_rate;
    param[4] = XA_PCM_GAIN_CONFIG_PARAM_PCM_WIDTH;
    param[5] = pcm_width;
    param[6] = XA_PCM_GAIN_CONFIG_PARAM_FRAME_SIZE;
    param[7] = frame_size;
    param[8] = XA_PCM_GAIN_CONFIG_PARAM_GAIN_FACTOR;
    param[9] = gain_idx;

    return (xaf_comp_set_config(p_comp, 5, &param[0]));
}

static int get_comp_config(void *p_comp, xaf_format_t *comp_format)
{
    int param[6];
    int ret;

    param[0] = XA_PCM_GAIN_CONFIG_PARAM_CHANNELS;
    param[2] = XA_PCM_GAIN_CONFIG_PARAM_PCM_WIDTH;
    param[4] = XA_PCM_GAIN_CONFIG_PARAM_SAMPLE_RATE;

    ret = xaf_comp_get_config(p_comp, 3, &param[0]);
    if (ret < 0)
        return ret;

    comp_format->channels    = param[1];
    comp_format->pcm_width   = param[3];
    comp_format->sample_rate = param[5];

    return 0;
}

/*******************************************************************************
 * Commands processing
 ******************************************************************************/
int srtm_pcm_gain(dsp_handle_t *dsp, unsigned int *pCmdParams)
{
    void *p_adev = NULL;
    XosThread pcm_gain_thread;
    unsigned char pcm_gain_stack[STACK_SIZE];
    void *p_pcm_gain = NULL;
    xaf_comp_status pcm_gain_status;
    int pcm_gain_info[4];
    void *pcm_gain_inbuf[2];
    int read_length;
    int i;
    xaf_format_t pcm_gain_format;
    int (*comp_setup)(void *p_comp);
    uint32_t *input_size, *output_size;
    XAF_ERR_CODE ret;
    xaf_adev_config_t device_config;
    xaf_comp_config_t comp_config;

#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
    dsp->buffer_in.data = (char *)MEMORY_ConvertMemoryMapAddress(pCmdParams[0], kMEMORY_Local2DMA);
#else
    dsp->buffer_in.data  = (char *)pCmdParams[0];
#endif
    dsp->buffer_in.size = (uint32_t)pCmdParams[1];

#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
    dsp->buffer_out.data = (char *)MEMORY_ConvertMemoryMapAddress(pCmdParams[2], kMEMORY_Local2DMA);
#else
    dsp->buffer_out.data = (char *)pCmdParams[2];
#endif
    dsp->buffer_out.size = (uint32_t)pCmdParams[3];

    dsp->buffer_in.index  = 0;
    dsp->buffer_out.index = 0;

    unsigned int srtm_sampling_rate = (unsigned int)(pCmdParams[4]);
    unsigned int srtm_channel_num   = (unsigned int)(pCmdParams[5]);
    unsigned int srtm_sample_width  = (unsigned int)(pCmdParams[6]);
    unsigned int srtm_gain_index    = (unsigned int)(pCmdParams[7]);

    input_size  = &pCmdParams[8];
    output_size = &pCmdParams[9];

    comp_setup = pcm_gain_setup;

    SRTM_PCM_GAIN_SAMPLE_RATE  = srtm_sampling_rate;
    SRTM_PCM_GAIN_NUM_CH       = srtm_channel_num;
    SRTM_PCM_GAIN_SAMPLE_WIDTH = srtm_sample_width;
    SRTM_PCM_GAIN_IDX_FOR_GAIN = srtm_gain_index;

    if ((dsp->buffer_in.data == 0) || (dsp->buffer_in.size == 0) || (dsp->buffer_out.data == 0) ||
        (dsp->buffer_out.size == 0))
    {
        DSP_PRINTF("Invalid IO Buffers!\n");
        exit(-1);
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

    DSP_PRINTF("[DSP Gain] Audio Device Ready\r\n");

    /* ...create pcm gain component */
    xaf_comp_config_default_init(&comp_config);
    comp_config.pp_inbuf = (pVOID(*)[XAF_MAX_INBUFS]) & pcm_gain_inbuf[0];

    ret = xaf_comp_create(p_adev, &p_pcm_gain, &comp_config);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_create failure: %d\r\n", ret);
        return -1;
    }

    comp_setup(p_pcm_gain);

    /* ...start pcm gain component */
    xaf_comp_process(p_adev, p_pcm_gain, NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_process XAF_START_FLAG failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("[DSP Gain] Gainer component started\r\n");

    /* Feed input to gainer component until initialization is complete */
    while (1)
    {
        read_length = DSP_AudioRead(dsp, pcm_gain_inbuf[0], XAF_INBUF_SIZE);
        if (read_length)
        {
            ret = xaf_comp_process(p_adev, p_pcm_gain, pcm_gain_inbuf[0], read_length, XAF_INPUT_READY_FLAG);
            if (ret != XAF_NO_ERR)
            {
                DSP_PRINTF("xaf_comp_process XAF_INPUT_READY_FLAG failure: %d\r\n", ret);
                return -1;
            }
        }
        else
        {
            break;
        }

        ret = xaf_comp_get_status(p_adev, p_pcm_gain, &pcm_gain_status, &pcm_gain_info[0]);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("xaf_comp_get_status failure: %d\r\n", ret);
            return -1;
        }

        if (pcm_gain_status == XAF_INIT_DONE || pcm_gain_status == XAF_EXEC_DONE)
        {
            break;
        }
    }

    if (pcm_gain_status != XAF_INIT_DONE)
    {
        DSP_PRINTF("ERROR: Failed to initialize gainer component\r\n");
        return -1;
    }

    get_comp_config(p_pcm_gain, &pcm_gain_format);

    /* Store gain component into context to pass to processing thread */
    dsp->comp = p_pcm_gain;

    /* Initialize buffer read/write functions */
    dsp->audio_read  = DSP_AudioRead;
    dsp->audio_write = DSP_AudioWrite;

    /* Start processing thread */
    xos_thread_create(&pcm_gain_thread, NULL, DSP_ProcessThread, (void *)dsp, "DSP_ProcessThread", pcm_gain_stack,
                      STACK_SIZE, 7, 0, 0);

    /* Wait for processing thread to complete before exiting. */
    xos_thread_join(&pcm_gain_thread, &i);
    xos_thread_delete(&pcm_gain_thread);

    /* Clean up and shut down XAF */
    ret = xaf_comp_delete(p_pcm_gain);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_delete failure: %d\r\n", ret);
        return -1;
    }

    ret = xaf_adev_close(p_adev, XAF_ADEV_NORMAL_CLOSE);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_adev_close failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("[DSP Gain] Audio device closed\r\n\r\n");

    /* Report the size of the input and decoded output buffer */
    *input_size  = dsp->buffer_in.index;
    *output_size = dsp->buffer_out.index;

    return 0;
}
