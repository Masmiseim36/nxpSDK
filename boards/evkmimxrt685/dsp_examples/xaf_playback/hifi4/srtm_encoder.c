/*
 * Copyright 2019-2023 NXP
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

#include "xaf-utils-test.h"
#include "xaf-fio-test.h"

#include "srtm_config_audio.h"
#if XA_OPUS_ENCODER
#include "audio/xa-opus-encoder-api.h"
#endif
#if XA_SBC_ENCODER
#include "xa_sbc_enc_api.h"
#endif

#include "fsl_common.h"
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
#include "fsl_memory.h"
#endif

#include "srtm_utils.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AUDIO_FRMWK_BUF_SIZE (64 * 1024)
#define AUDIO_COMP_BUF_SIZE  (256 * 1024)

extern int audio_frmwk_buf_size;
extern int audio_comp_buf_size;

#define OPUS_ENC_PCM_WIDTH   16
#define OPUS_ENC_SAMPLE_RATE 16000
#define OPUS_ENC_NUM_CH      1
#define OPUS_ENC_APPLICATION XA_OPUS_APPLICATION_VOIP
#define OPUS_ENC_BITRATE     20000
#define OPUS_ENC_FRAME_SIZE  320
#define OPUS_ENC_MAX_PAYLOAD 1500
#define OPUS_ENC_COMPLEXITY  10

#define SBC_ENC_SAMP_FREQ 44100
#define SBC_ENC_BITPOOL   31
#define SBC_ENC_CHMODE    XA_SBC_ENC_CHMODE_DUAL

#define TASK_STACK_SIZE (11 * 1024)

/*******************************************************************************
 * Commands processing
 ******************************************************************************/
int srtm_encoder(dsp_handle_t *dsp, unsigned int *pCmdParams)
{
    int ret;
    void *p_adev    = NULL;
    void *p_encoder = NULL;
    XosThread enc_thread;
    char enc_stack[TASK_STACK_SIZE];
    xaf_comp_status enc_status;
    int enc_info[4];
    void *enc_inbuf[1];
    uint32_t read_length;
    int32_t exitcode;
    uint32_t *input_size, *output_size;
    int param[16];
    int param_num;
    xf_id_t enc_id;
    xaf_adev_config_t device_config;
    xaf_comp_config_t comp_config;

    /* Param 0 Encoder type */
    /* Param 1 Buffer address with PCM data */
    /* Param 2 PCM data size */
    /* Param 3 Buffer address to store encoded data */
    /* Param 4 Buffer size to store encoded data */
    /* Param 5 Return parameter, actual read size */
    /* Param 6 Return parameter, actual write size */

#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
    dsp->buffer_in.data = (char *)MEMORY_ConvertMemoryMapAddress(pCmdParams[1], kMEMORY_Local2DMA);
#else
    dsp->buffer_in.data  = (char *)pCmdParams[1];
#endif
    dsp->buffer_in.size = (uint32_t)pCmdParams[2];

#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
    dsp->buffer_out.data = (char *)MEMORY_ConvertMemoryMapAddress(pCmdParams[3], kMEMORY_Local2DMA);
#else
    dsp->buffer_out.data = (char *)pCmdParams[3];
#endif
    dsp->buffer_out.size = (uint32_t)pCmdParams[4];

    dsp->buffer_in.index  = 0;
    dsp->buffer_out.index = 0;

    input_size  = &pCmdParams[5];
    output_size = &pCmdParams[6];

    switch (pCmdParams[0])
    {
#if XA_OPUS_ENCODER
        case SRTM_Encoder_OPUS:
            param[0]  = XA_OPUS_ENC_CONFIG_PARAM_PCM_WIDTH;
            param[1]  = OPUS_ENC_PCM_WIDTH;
            param[2]  = XA_OPUS_ENC_CONFIG_PARAM_SAMPLE_RATE;
            param[3]  = OPUS_ENC_SAMPLE_RATE;
            param[4]  = XA_OPUS_ENC_CONFIG_PARAM_CHANNELS;
            param[5]  = OPUS_ENC_NUM_CH;
            param[6]  = XA_OPUS_ENC_CONFIG_PARAM_APPLICATION;
            param[7]  = OPUS_ENC_APPLICATION;
            param[8]  = XA_OPUS_ENC_CONFIG_PARAM_BITRATE;
            param[9]  = OPUS_ENC_BITRATE;
            param[10] = XA_OPUS_ENC_CONFIG_PARAM_FRAME_SIZE;
            param[11] = OPUS_ENC_FRAME_SIZE;
            param[12] = XA_OPUS_ENC_CONFIG_PARAM_MAX_PAYLOAD;
            param[13] = OPUS_ENC_MAX_PAYLOAD;
            param[14] = XA_OPUS_ENC_CONFIG_PARAM_COMPLEXITY;
            param[15] = OPUS_ENC_COMPLEXITY;
            param_num = 8;
            enc_id    = "audio-encoder/opus";
            break;
#endif
#if XA_SBC_ENCODER
        case SRTM_Encoder_SBC:
            param[0]  = XA_SBC_ENC_CONFIG_PARAM_SAMP_FREQ;
            param[1]  = SBC_ENC_SAMP_FREQ;
            param[2]  = XA_SBC_ENC_CONFIG_PARAM_BITPOOL;
            param[3]  = SBC_ENC_BITPOOL;
            param[4]  = XA_SBC_ENC_CONFIG_PARAM_CHMODE;
            param[5]  = SBC_ENC_CHMODE;
            param_num = 3;
            enc_id    = "audio-encoder/sbc";
            break;
#endif
        /* Unknown encoder. */
        default:
            DSP_PRINTF("[DSP Codec] Encoder failure: unknown codec!\r\n");
            goto error_cleanup;
    }
    xaf_adev_config_default_init(&device_config);

    audio_frmwk_buf_size = AUDIO_FRMWK_BUF_SIZE;
    audio_comp_buf_size = AUDIO_COMP_BUF_SIZE;
    device_config.mem_pool[XAF_MEM_ID_COMP].size = AUDIO_COMP_BUF_SIZE;
    device_config.mem_pool[XAF_MEM_ID_DEV].size = AUDIO_FRMWK_BUF_SIZE;
    device_config.core = XF_CORE_ID;
    TST_CHK_API_ADEV_OPEN(p_adev, device_config, "[DSP Codec] Audio Device Open\r\n");
    DSP_PRINTF("[DSP Codec] Audio Device Ready\r\n");

    /* Create encoder component
     * 1 input buffer, 1 output buffer */
    xaf_comp_config_default_init(&comp_config);

    comp_config.comp_id            = enc_id;
    comp_config.num_input_buffers  = 1;
    comp_config.num_output_buffers = 1;
    comp_config.pp_inbuf           = (pVOID(*)[XAF_MAX_INBUFS]) & enc_inbuf[0];
    comp_config.comp_type          = XAF_ENCODER;
    ret                            = xaf_comp_create(p_adev, &p_encoder, &comp_config);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("[DSP Codec] xaf_comp_create failure: %d\r\n", ret);
        goto error_cleanup;
    }

    ret = xaf_comp_set_config(p_encoder, param_num, &param[0]);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("[DSP Codec] xaf_comp_set_config failure: %d\r\n", ret);
        goto error_cleanup;
    }

    DSP_PRINTF("[DSP Codec] Encoder created\r\n");

    /* Start encoder component */
    ret = xaf_comp_process(p_adev, p_encoder, NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("[DSP Codec] xaf_comp_process XAF_START_FLAG failure: %d\r\n", ret);
        goto error_cleanup;
    }

    DSP_PRINTF("[DSP Codec] Encoder component started\r\n");

    /* Feed input to encoder component until initialization is complete */
    while (1)
    {
        read_length = DSP_AudioRead(dsp, enc_inbuf[0], XAF_INBUF_SIZE);
        if (read_length)
        {
            ret = xaf_comp_process(p_adev, p_encoder, enc_inbuf[0], read_length, XAF_INPUT_READY_FLAG);
            if (ret != XAF_NO_ERR)
            {
                DSP_PRINTF("[DSP Codec] xaf_comp_process XAF_INPUT_READY_FLAG failure: %d\r\n", ret);
                goto error_cleanup;
            }
        }
        else
        {
            break;
        }

        ret = xaf_comp_get_status(p_adev, p_encoder, &enc_status, &enc_info[0]);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("[DSP Codec] xaf_comp_get_status failure: %d\r\n", ret);
            goto error_cleanup;
        }

        if (enc_status == XAF_INIT_DONE || enc_status == XAF_EXEC_DONE)
        {
            break;
        }
    }

    if (enc_status != XAF_INIT_DONE)
    {
        DSP_PRINTF("[DSP Codec] ERROR: Failed to initialize encoder component\r\n");
        goto error_cleanup;
    }

    /* Store encoder component into context to pass to processing thread */
    dsp->comp = p_encoder;

    /* Initialize buffer read/write functions */
    dsp->audio_read  = DSP_AudioRead;
    dsp->audio_write = DSP_AudioWrite;

    /* Start processing thread */
    ret = xos_thread_create(&enc_thread, NULL, DSP_ProcessThread, (void *)dsp, "DSP_ProcessThread", enc_stack,
                            TASK_STACK_SIZE, 7, 0, 0);
    if (ret != XOS_OK)
    {
        DSP_PRINTF("[DSP Codec] xos_thread_create failure: %d\r\n", ret);
        goto error_cleanup;
    }

    /* Wait for processing thread to complete before exiting. */
    ret = xos_thread_join(&enc_thread, &exitcode);
    if (ret != XOS_OK)
    {
        DSP_PRINTF("[DSP Codec] xos_thread_join failure: %d\r\n", ret);
        ret = xos_thread_delete(&enc_thread);
        if (ret != XOS_OK)
        {
            DSP_PRINTF("[DSP Codec] xos_thread_delete failure: %d\r\n", ret);
        }
        goto error_cleanup;
    }
    if (exitcode != 0)
    {
        DSP_PRINTF("[DSP Codec] DSP_ProcessThread exit code: %d\r\n", exitcode);
        ret = xos_thread_delete(&enc_thread);
        if (ret != XOS_OK)
        {
            DSP_PRINTF("[DSP Codec] xos_thread_delete failure: %d\r\n", ret);
        }
        goto error_cleanup;
    }
    ret = xos_thread_delete(&enc_thread);
    if (ret != XOS_OK)
    {
        DSP_PRINTF("[DSP Codec] xos_thread_delete failure: %d\r\n", ret);
        goto error_cleanup;
    }

    /* Clean up and shut down XAF */
    ret = xaf_comp_delete(p_encoder);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("[DSP Codec] xaf_comp_delete failure: %d\r\n", ret);
        goto error_cleanup;
    }
    p_encoder = NULL;

    TST_CHK_API_ADEV_CLOSE(p_adev, XAF_ADEV_NORMAL_CLOSE, device_config, "xaf_adev_close");
    p_adev = NULL;

    DSP_PRINTF("[DSP Codec] Audio device closed\r\n\r\n");

    /* Report the size of the input and encoded output buffer */
    *input_size  = dsp->buffer_in.index;
    *output_size = dsp->buffer_out.index;

    return 0;

error_cleanup:
    if (p_adev != NULL)
    {
        TST_CHK_API_ADEV_CLOSE(p_adev, XAF_ADEV_FORCE_CLOSE, device_config, "xaf_adev_close");
    }

    /* Report the size of the input and encoded output buffer */
    *input_size  = dsp->buffer_in.index;
    *output_size = dsp->buffer_out.index;

    return -1;
}
