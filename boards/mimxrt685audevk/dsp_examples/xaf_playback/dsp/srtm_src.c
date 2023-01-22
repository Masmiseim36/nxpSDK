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

#include "xa_error_standards.h"
#include "xa_src_pp_api.h"
#include "srtm_utils.h"

#include "fsl_common.h"
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
#include "fsl_memory.h"
#endif

#define AUDIO_FRMWK_BUF_SIZE (64 * 1024)
#define AUDIO_COMP_BUF_SIZE  (256 * 1024)
#define MAX_SRC_FRAME_ADJUST 2
#define MAX_INPUT_CHUNK_LEN  512

static int src_setup(
    void *p_comp, int channels, int in_sample_rate, int out_sample_rate, int in_frame_size, int pcm_width_bytes)
{
    int param[10];
    param[0] = XA_SRC_PP_CONFIG_PARAM_INPUT_CHANNELS;
    param[1] = channels;
    param[2] = XA_SRC_PP_CONFIG_PARAM_INPUT_SAMPLE_RATE;
    param[3] = in_sample_rate;
    param[4] = XA_SRC_PP_CONFIG_PARAM_OUTPUT_SAMPLE_RATE;
    param[5] = out_sample_rate;
    param[6] = XA_SRC_PP_CONFIG_PARAM_INPUT_CHUNK_SIZE;
    param[7] = in_frame_size;
    param[8] = XA_SRC_PP_CONFIG_PARAM_BYTES_PER_SAMPLE;
    param[9] = (pcm_width_bytes == 4) ? 3 : 2; // src library only supports 16 or MSB-aligned 24 bit input

    return (xaf_comp_set_config(p_comp, 5, &param[0]));
}

static int get_comp_config(void *p_comp, xaf_format_t *comp_format)
{
    int param[6];
    int ret;

    param[0] = XA_SRC_PP_CONFIG_PARAM_INPUT_CHANNELS;
    param[2] = XA_SRC_PP_CONFIG_PARAM_OUTPUT_SAMPLE_RATE;
    param[4] = XA_SRC_PP_CONFIG_PARAM_BYTES_PER_SAMPLE;

    ret = xaf_comp_get_config(p_comp, 3, &param[0]);
    if (ret < 0)
        return ret;

    comp_format->channels    = param[1];
    comp_format->sample_rate = param[3];
    comp_format->pcm_width   = param[5] * 8;

    return 0;
}

/*******************************************************************************
 * Commands processing
 ******************************************************************************/
int srtm_src(dsp_handle_t *dsp, unsigned int *pCmdParams)
{
    void *p_adev = NULL;
    XosThread src_thread;
    unsigned char src_stack[STACK_SIZE];
    void *p_src = NULL;
    xaf_comp_status src_status;
    int src_info[4];
    void *src_inbuf[1];
    int read_length;
    int i;
    xaf_format_t src_format;
    xf_id_t comp_id   = "post-proc/src-pp";
    int in_frame_size = 0;
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
    dsp->buffer_out.data = (char *)pCmdParams[5];
#endif
    dsp->buffer_out.size = (uint32_t)pCmdParams[6];

    dsp->buffer_in.index  = 0;
    dsp->buffer_out.index = 0;

    unsigned int srtm_input_sampling_rate  = (unsigned int)(pCmdParams[2]);
    unsigned int srtm_input_channels       = (unsigned int)(pCmdParams[3]);
    unsigned int srtm_input_sample_width   = (unsigned int)(pCmdParams[4]);
    unsigned int srtm_output_sampling_rate = (unsigned int)(pCmdParams[7]);

    input_size  = &pCmdParams[8];
    output_size = &pCmdParams[9];

    if ((dsp->buffer_in.data == 0) || (dsp->buffer_in.size == 0) || (dsp->buffer_out.data == 0) ||
        (dsp->buffer_out.size == 0))
    {
        DSP_PRINTF("Invalid IO Buffers!\n");
        exit(-1);
    }

    if ((srtm_input_sampling_rate < 8000) || (srtm_input_sampling_rate > 48000) || (srtm_output_sampling_rate < 8000) ||
        (srtm_output_sampling_rate > 48000))
    {
        DSP_PRINTF("Invalid In/ Out Sampling Rate!\n");
        exit(-2);
    }

    if ((srtm_input_sample_width != 16) && (srtm_input_sample_width != 32))
    {
        DSP_PRINTF("Invalid Sample Width, Must be 16 or 32!\n");
        exit(-3);
    }
    srtm_input_sample_width = srtm_input_sample_width / 8;

    in_frame_size = XAF_INBUF_SIZE / (srtm_input_sample_width * srtm_input_channels * MAX_SRC_FRAME_ADJUST);
    in_frame_size = (in_frame_size > MAX_INPUT_CHUNK_LEN) ? MAX_INPUT_CHUNK_LEN : in_frame_size;

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

    DSP_PRINTF("[DSP SRC] Audio Device Ready\r\n");
    /* ...create src component */
    xaf_comp_config_default_init(&comp_config);

    comp_config.comp_id            = comp_id;
    comp_config.num_input_buffers  = 1;
    comp_config.num_output_buffers = 1;
    comp_config.pp_inbuf           = (pVOID(*)[XAF_MAX_INBUFS]) & src_inbuf[0];
    comp_config.comp_type          = XAF_POST_PROC;

    ret = xaf_comp_create(p_adev, &p_src, &comp_config);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_create failure: %d\r\n", ret);
        return -1;
    }
    src_setup(p_src, srtm_input_channels, srtm_input_sampling_rate, srtm_output_sampling_rate, in_frame_size,
              srtm_input_sample_width);

    DSP_PRINTF("[DSP SRC] XAF SRC component created\r\n");

    /* ...start src component */
    ret = xaf_comp_process(p_adev, p_src, NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_process XAF_START_FLAG failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("[DSP SRC] SRC component started\r\n");

    /* Feed input to decoder component until initialization is complete */
    while (1)
    {
        read_length = DSP_AudioRead(dsp, src_inbuf[0], XAF_INBUF_SIZE);
        if (read_length)
        {
            ret = xaf_comp_process(p_adev, p_src, src_inbuf[0], read_length, XAF_INPUT_READY_FLAG);
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

        ret = xaf_comp_get_status(p_adev, p_src, &src_status, &src_info[0]);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("xaf_comp_get_status failure: %d\r\n", ret);
            return -1;
        }

        if (src_status == XAF_INIT_DONE || src_status == XAF_EXEC_DONE)
        {
            break;
        }
    }

    if (src_status != XAF_INIT_DONE)
    {
        DSP_PRINTF("ERROR: Failed to initialize decoder component\r\n");
        return -1;
    }

    get_comp_config(p_src, &src_format);

    /* Store SRC component into context to pass to processing thread */
    dsp->comp = p_src;

    /* Initialize buffer read/write functions */
    dsp->audio_read  = DSP_AudioRead;
    dsp->audio_write = DSP_AudioWrite;

    /* Start processing thread */
    xos_thread_create(&src_thread, NULL, DSP_ProcessThread, (void *)dsp, "DSP_ProcessThread", src_stack, STACK_SIZE, 7,
                      0, 0);

    /* Wait for processing thread to complete before exiting. */
    xos_thread_join(&src_thread, &i);
    xos_thread_delete(&src_thread);

    /* Clean up and shut down XAF */
    ret = xaf_comp_delete(p_src);
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

    DSP_PRINTF("[DSP SRC] Audio device closed\r\n\r\n");

    /* Report the size of the input and decoded output buffer */
    *input_size  = dsp->buffer_in.index;
    *output_size = dsp->buffer_out.index;

    return 0;
}
