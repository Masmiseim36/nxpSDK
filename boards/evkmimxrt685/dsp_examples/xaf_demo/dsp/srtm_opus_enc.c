/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <xtensa/xos.h>

#include "xaf-api.h"
#include "audio/xa-opus-encoder-api.h"

#include "fsl_common.h"
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
#include "fsl_memory.h"
#endif

#include "srtm_utils.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AUDIO_FRMWK_BUF_SIZE (64 * 1024)
#define AUDIO_COMP_BUF_SIZE (256 * 1024)

#define OPUS_ENC_PCM_WIDTH 16
#define OPUS_ENC_SAMPLE_RATE 16000
#define OPUS_ENC_NUM_CH 1
#define OPUS_ENC_APPLICATION XA_OPUS_APPLICATION_VOIP
#define OPUS_ENC_BITRATE 20000
#define OPUS_ENC_FRAME_SIZE 320
#define OPUS_ENC_MAX_PAYLOAD 1500
#define OPUS_ENC_COMPLEXITY 10

#define RENDERER_FRAME_SIZE (4 * 1024)

/* Stack size for DSP data processing thread. */
#define STACK_SIZE (4 * 1024)

/*******************************************************************************
 * Component Setup/ Config
 ******************************************************************************/
static XAF_ERR_CODE srtm_opus_setup(void *p_encoder)
{
#define OPUS_ENC_NUM_SET_PARAMS 8
    int param[OPUS_ENC_NUM_SET_PARAMS * 2];

    param[0 * 2 + 0] = XA_OPUS_ENC_CONFIG_PARAM_PCM_WIDTH;
    param[0 * 2 + 1] = OPUS_ENC_PCM_WIDTH;
    param[1 * 2 + 0] = XA_OPUS_ENC_CONFIG_PARAM_SAMPLE_RATE;
    param[1 * 2 + 1] = OPUS_ENC_SAMPLE_RATE;
    param[2 * 2 + 0] = XA_OPUS_ENC_CONFIG_PARAM_CHANNELS;
    param[2 * 2 + 1] = OPUS_ENC_NUM_CH;
    param[3 * 2 + 0] = XA_OPUS_ENC_CONFIG_PARAM_APPLICATION;
    param[3 * 2 + 1] = OPUS_ENC_APPLICATION;
    param[4 * 2 + 0] = XA_OPUS_ENC_CONFIG_PARAM_BITRATE;
    param[4 * 2 + 1] = OPUS_ENC_BITRATE;
    param[5 * 2 + 0] = XA_OPUS_ENC_CONFIG_PARAM_FRAME_SIZE;
    param[5 * 2 + 1] = OPUS_ENC_FRAME_SIZE;
    param[6 * 2 + 0] = XA_OPUS_ENC_CONFIG_PARAM_MAX_PAYLOAD;
    param[6 * 2 + 1] = OPUS_ENC_MAX_PAYLOAD;
    param[7 * 2 + 0] = XA_OPUS_ENC_CONFIG_PARAM_COMPLEXITY;
    param[7 * 2 + 1] = OPUS_ENC_COMPLEXITY;

    return (xaf_comp_set_config(p_encoder, OPUS_ENC_NUM_SET_PARAMS, &param[0]));
}

/*******************************************************************************
 * Commands processing
 ******************************************************************************/
int srtm_opus_enc(dsp_handle_t *dsp, unsigned int *pCmdParams)
{
    XAF_ERR_CODE ret;
    void *p_adev    = NULL;
    void *p_encoder = NULL;
    XosThread enc_thread;
    char enc_stack[STACK_SIZE];
    xaf_comp_status enc_status;
    int enc_info[4];
    void *enc_inbuf[1];
    uint32_t read_length;
    int i;
    uint32_t *input_size, *output_size;

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

    input_size  = &pCmdParams[4];
    output_size = &pCmdParams[5];

    ret = xaf_adev_open(&p_adev, AUDIO_FRMWK_BUF_SIZE, AUDIO_COMP_BUF_SIZE, DSP_Malloc, DSP_Free);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_adev_open failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("[DSP Codec] Audio Device Ready\r\n");

    /* Create encoder component
     * 1 input buffer, 1 output buffer */
    ret = xaf_comp_create(p_adev, &p_encoder, "audio-encoder/opus", 1, 1, &enc_inbuf[0], XAF_ENCODER);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_comp_create failure: %d\r\n", ret);
        return -1;
    }

    ret = srtm_opus_setup(p_encoder);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("srtm_opus_setup failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("[DSP Codec] Encoder created\r\n");

    /* Start encoder component */
    ret = xaf_comp_process(p_adev, p_encoder, NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_comp_process XAF_START_FLAG failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("[DSP Codec] Encoder component started\r\n");

    /* Feed input to encoder component until initialization is complete */
    while (1)
    {
        read_length = DSP_AudioRead(dsp, enc_inbuf[0], XAF_INBUF_SIZE);
        if (read_length)
        {
            ret = xaf_comp_process(p_adev, p_encoder, enc_inbuf[0], read_length, XAF_INPUT_READY_FLAG);
            if (ret != XAF_NO_ERROR)
            {
                DSP_PRINTF("xaf_comp_process XAF_INPUT_READY_FLAG failure: %d\r\n", ret);
                return -1;
            }
        }
        else
        {
            break;
        }

        ret = xaf_comp_get_status(p_adev, p_encoder, &enc_status, &enc_info[0]);
        if (ret != XAF_NO_ERROR)
        {
            DSP_PRINTF("xaf_comp_get_status failure: %d\r\n", ret);
            return -1;
        }

        if (enc_status == XAF_INIT_DONE || enc_status == XAF_EXEC_DONE)
        {
            break;
        }
    }

    if (enc_status != XAF_INIT_DONE)
    {
        DSP_PRINTF("ERROR: Failed to initialize encoder component\r\n");
        return -1;
    }

    /* Store encoder component into context to pass to processing thread */
    dsp->comp = p_encoder;

    /* Start processing thread */
    xos_thread_create(&enc_thread, NULL, DSP_ProcessThread, (void *)dsp, "DSP_ProcessThread", enc_stack, STACK_SIZE, 7,
                      0, 0);

    /* Wait for processing thread to complete before exiting. */
    xos_thread_join(&enc_thread, &i);
    xos_thread_delete(&enc_thread);

    /* Clean up and shut down XAF */
    ret = xaf_comp_delete(p_encoder);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_comp_delete failure: %d\r\n", ret);
        return -1;
    }

    ret = xaf_adev_close(p_adev, XAF_ADEV_NORMAL_CLOSE);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_adev_close failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("[DSP Codec] Audio device closed\r\n\r\n");

    /* Report the size of the input and encoded output buffer */
    *input_size  = dsp->buffer_in.index;
    *output_size = dsp->buffer_out.index;

    return 0;
}
