/*
 * Copyright 2018-2023 NXP
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
#if XA_AAC_DECODER
#include "xa_aac_dec_api.h"
#endif
#if XA_MP3_DECODER
#include "xa_mp3_dec_api.h"
#endif
#if XA_SBC_DECODER
#include "xa_sbc_dec_api.h"
#endif
#if XA_VORBIS_DECODER
#include "xa_vorbis_dec_api.h"
#endif
#if XA_OPUS_DECODER
#include "audio/xa-opus-decoder-api.h"
#endif
#include "audio/xa-audio-decoder-api.h"
#include "audio/xa-renderer-api.h"

#include "fsl_common.h"
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
#include "fsl_memory.h"
#endif

#include "dsp_config.h"
#include "srtm_config.h"
#include "srtm_utils.h"
#include "srtm_config_audio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AUDIO_FRMWK_BUF_SIZE (64 * 1024)
#define AUDIO_COMP_BUF_SIZE  (256 * 1024)

int audio_frmwk_buf_size;
int audio_comp_buf_size;

#define AAC_DEC_PCM_WIDTH 16

#define MP3_DEC_PCM_WIDTH 16

#define OPUS_DEC_NUM_CH             2
#define OPUS_DEC_PCM_WIDTH          16
#define OPUS_DEC_LOST_FLAG          0
#define OPUS_DEC_NB_STREAMS         1
#define OPUS_DEC_NB_COUPLED         1
#define OPUS_DEC_CHANNEL_MAPPING    0
#define OPUS_DEC_NO_RANGE_DEC_STATE 0

#define VORBIS_DEC_RAW_VORBIS_LAST_PKT_GRANULE_POS -1
#define VORBIS_DEC_OGG_MAX_PAGE_SIZE               12
#define VORBIS_DEC_RUNTIME_MEM                     0
#define VORBIS_DEC_SAMPLE_RATE                     48000

#define RENDERER_FRAME_SIZE (4 * 1024)
#define TASK_STACK_SIZE     (11 * 1024)

/*******************************************************************************
 * Component Setup/ Config
 ******************************************************************************/
static XAF_ERR_CODE get_dec_config(void *p_comp, xaf_format_t *comp_format)
{
    XAF_ERR_CODE ret;
    int param[6];

    param[0] = XA_CODEC_CONFIG_PARAM_CHANNELS;
    param[2] = XA_CODEC_CONFIG_PARAM_SAMPLE_RATE;
    param[4] = XA_CODEC_CONFIG_PARAM_PCM_WIDTH;

    ret = xaf_comp_get_config(p_comp, 3, &param[0]);
    if (ret != XAF_NO_ERR)
        return ret;

    comp_format->channels    = param[1];
    comp_format->sample_rate = param[3];
    comp_format->pcm_width   = param[5];

    return XAF_NO_ERR;
}

static XAF_ERR_CODE renderer_setup(void *p_renderer, xaf_format_t *format)
{
    int param[18];
    int numParam;

    numParam = 7;

    param[0]  = XA_RENDERER_CONFIG_PARAM_PCM_WIDTH;
    param[1]  = format->pcm_width;
    param[2]  = XA_RENDERER_CONFIG_PARAM_CHANNELS;
    param[3]  = format->channels;
    param[4]  = XA_RENDERER_CONFIG_PARAM_SAMPLE_RATE;
    param[5]  = format->sample_rate;
    param[6]  = XA_RENDERER_CONFIG_PARAM_FRAME_SIZE;
    param[7]  = RENDERER_FRAME_SIZE;
    param[8]  = XA_RENDERER_CONFIG_PARAM_AUDIO_BUFFER_1;
    param[9]  = (int)DSP_AUDIO_BUFFER_1_PING;
    param[10] = XA_RENDERER_CONFIG_PARAM_AUDIO_BUFFER_2;
    param[11] = (int)DSP_AUDIO_BUFFER_1_PONG;
    param[12] = XA_RENDERER_CONFIG_PARAM_I2S_INTERFACE;
    param[13] = AUDIO_I2S_RENDERER_DEVICE;

    return xaf_comp_set_config(p_renderer, numParam, &param[0]);
}

/*******************************************************************************
 * Commands processing
 ******************************************************************************/
int srtm_decoder(dsp_handle_t *dsp, unsigned int *pCmdParams)
{
    int ret;
    void *p_adev     = NULL;
    void *p_decoder  = NULL;
    void *p_renderer = NULL;
    XosThread dec_thread;
    char dec_stack[TASK_STACK_SIZE];
    xaf_comp_status dec_status;
    int dec_info[4];
    void *dec_inbuf[1];
    uint32_t read_length;
    int32_t exitcode;
    xaf_adev_config_t device_config;
    xaf_comp_config_t comp_config;

    int param[16];
    int param_num;
    xf_id_t dec_id;

    xaf_format_t dec_format;

    /* Param 0 Decoder type */
	/* Param 1 Buffer address with encoded data */
	/* Param 2 Encoded data size */

#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
    dsp->buffer_in.data = (char *)MEMORY_ConvertMemoryMapAddress(pCmdParams[1], kMEMORY_Local2DMA);
#else
    dsp->buffer_in.data  = (char *)pCmdParams[1];
#endif
    dsp->buffer_in.size = (uint32_t)pCmdParams[2];

    dsp->buffer_in.index  = 0;

    switch (pCmdParams[0])
    {
#if XA_AAC_DECODER
        case SRTM_Decoder_AAC:
            param[0]  = XA_AACDEC_CONFIG_PARAM_PCM_WDSZ;
            param[1]  = AAC_DEC_PCM_WIDTH;
            param_num = 1;
            dec_id    = "audio-decoder/aac";
            break;
#endif
#if XA_MP3_DECODER
        case SRTM_Decoder_MP3:
            param[0]  = XA_MP3DEC_CONFIG_PARAM_PCM_WDSZ;
            param[1]  = MP3_DEC_PCM_WIDTH;
            param_num = 1;
            dec_id    = "audio-decoder/mp3";
            break;
#endif
#if XA_OPUS_DECODER
        case SRTM_Decoder_OPUS:
            param[0]  = XA_OPUS_DEC_CONFIG_PARAM_CHANNELS;
            param[1]  = OPUS_DEC_NUM_CH;
            param[2]  = XA_OPUS_DEC_CONFIG_PARAM_PCM_WIDTH;
            param[3]  = OPUS_DEC_PCM_WIDTH;
            param[4]  = XA_OPUS_DEC_CONFIG_PARAM_LOST_PACKET;
            param[5]  = OPUS_DEC_LOST_FLAG;
            param[6]  = XA_OPUS_DEC_CONFIG_PARAM_NUM_STREAMS;
            param[7]  = OPUS_DEC_NB_STREAMS;
            param[8]  = XA_OPUS_DEC_CONFIG_PARAM_NUM_COUPLED_STREAMS;
            param[9]  = OPUS_DEC_NB_COUPLED;
            param[10] = XA_OPUS_DEC_CONFIG_PARAM_CHAN_MAPPING;
            param[11] = OPUS_DEC_CHANNEL_MAPPING;
            param[12] = XA_OPUS_DEC_CONFIG_PARAM_EXCLUDE_RANGE_DEC_STATE;
            param[13] = OPUS_DEC_NO_RANGE_DEC_STATE;
            param[14] = XA_OPUS_DEC_CONFIG_PARAM_STREAM_TYPE;
            param[15] = XA_RAW_OPUS_STREAM;
            param_num = 8;
            dec_id    = "audio-decoder/opus";
            break;
#endif
#if XA_SBC_DECODER
        case SRTM_Decoder_SBC:
            /* Workaround: SBC decoder has not config params to set,
             * but XAF requires set config to be called in order to get through
             * XA_API_CMD_INIT / XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS phase */
            param[0]  = XA_SBC_DEC_CONFIG_PARAM_NUM_CHANNELS;
            param[1]  = 2;
            param_num = 1;
            dec_id    = "audio-decoder/sbc";
            break;
#endif
#if XA_VORBIS_DECODER
        case SRTM_Decoder_VORBIS_OGG:
        case SRTM_Decoder_VORBIS_RAW:
            param[0] = XA_VORBISDEC_CONFIG_PARAM_RAW_VORBIS_FILE_MODE;
            param[1] = pCmdParams[0] == SRTM_Decoder_VORBIS_RAW ? 1 : 0;
            param[2] = XA_VORBISDEC_CONFIG_PARAM_OGG_MAX_PAGE_SIZE;
            param[3] = VORBIS_DEC_OGG_MAX_PAGE_SIZE;
            param[4] = XA_VORBISDEC_CONFIG_PARAM_RUNTIME_MEM;
            param[5] = VORBIS_DEC_RUNTIME_MEM;
            if (pCmdParams[0] == SRTM_Decoder_VORBIS_RAW)
            {
                param[6]  = XA_VORBISDEC_CONFIG_PARAM_RAW_VORBIS_LAST_PKT_GRANULE_POS;
                param[7]  = VORBIS_DEC_RAW_VORBIS_LAST_PKT_GRANULE_POS;
                param_num = 4;
            }
            else
            {
                param_num = 3;
            }
            dec_id = "audio-decoder/vorbis";
            break;
#endif
        /* Unknown decoder. */
        default:
            DSP_PRINTF("Decoder failure: unknown codec!\r\n");
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
    dsp->audio_device = p_adev;

    xaf_comp_config_default_init(&comp_config);

    comp_config.comp_id            = dec_id;
    comp_config.num_input_buffers  = 1;
    comp_config.num_output_buffers = 0;
    comp_config.pp_inbuf           = (pVOID(*)[XAF_MAX_INBUFS]) & dec_inbuf[0];
    comp_config.comp_type          = XAF_DECODER;

    ret = xaf_comp_create(p_adev, &p_decoder, &comp_config);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_create failure: %d\r\n", ret);
        goto error_cleanup;
    }

    ret = xaf_comp_set_config(p_decoder, param_num, &param[0]);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("srtm_dec_setup failure: %d\r\n", ret);
        goto error_cleanup;
    }

    DSP_PRINTF("[DSP Codec] Decoder created\r\n");

    /* Start decoder component */
    ret = xaf_comp_process(p_adev, p_decoder, NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_process XAF_START_FLAG failure: %d\r\n", ret);
        goto error_cleanup;
    }

    DSP_PRINTF("[DSP Codec] Decoder component started\r\n");

    /* Feed input to decoder component until initialization is complete */
    while (1)
    {
        read_length = DSP_AudioRead(dsp, dec_inbuf[0], XAF_INBUF_SIZE);
        if (read_length)
        {
            ret = xaf_comp_process(p_adev, p_decoder, dec_inbuf[0], read_length, XAF_INPUT_READY_FLAG);
            if (ret != XAF_NO_ERR)
            {
                DSP_PRINTF("xaf_comp_process XAF_INPUT_READY_FLAG failure: %d\r\n", ret);
                goto error_cleanup;
            }
        }
        else
        {
            break;
        }

        ret = xaf_comp_get_status(p_adev, p_decoder, &dec_status, &dec_info[0]);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("xaf_comp_get_status failure: %d\r\n", ret);
            goto error_cleanup;
        }

        if (dec_status == XAF_INIT_DONE || dec_status == XAF_EXEC_DONE)
        {
            break;
        }
    }

    if (dec_status != XAF_INIT_DONE)
    {
        DSP_PRINTF("ERROR: Failed to initialize decoder component: %d\r\n", dec_status);
        goto error_cleanup;
    }

    /* Store decoder component into context to pass to processing thread */
    dsp->comp = p_decoder;

	/* Get decoder format from the decoder */
	ret = get_dec_config(p_decoder, &dec_format);
	if (ret != XAF_NO_ERR)
	{
		DSP_PRINTF("get_dec_config failure: %d\r\n", ret);
		goto error_cleanup;
	}

	xaf_comp_config_default_init(&comp_config);

	comp_config.comp_id            = "renderer";
	comp_config.num_input_buffers  = 0;
	comp_config.num_output_buffers = 0;
	comp_config.pp_inbuf           = NULL;
	comp_config.comp_type          = XAF_RENDERER;

	ret = xaf_comp_create(p_adev, &p_renderer, &comp_config);
	if (ret != XAF_NO_ERR)
	{
		DSP_PRINTF("xaf_comp_create failure: %d\r\n", ret);
		goto error_cleanup;
	}

	if ((pCmdParams[0] == SRTM_Decoder_VORBIS_OGG) || (pCmdParams[0] == SRTM_Decoder_VORBIS_RAW))
	{
		dec_format.sample_rate = VORBIS_DEC_SAMPLE_RATE;
	}

	/* Setup renderer to match decoded PCM format */
	ret = renderer_setup(p_renderer, &dec_format);
	if (ret != XAF_NO_ERR)
	{
		DSP_PRINTF("renderer_setup failure: %d\r\n", ret);
		goto error_cleanup;
	}

	DSP_PRINTF("[DSP Codec] Renderer component created\r\n");

	ret = xaf_connect(p_decoder, 1, p_renderer, 0, 4);
	if (ret != XAF_NO_ERR)
	{
		DSP_PRINTF("xaf_connect failure: %d\r\n", ret);
		goto error_cleanup;
	}
	DSP_PRINTF("[DSP Codec] Connected XA_DECODER -> XA_RENDERER\r\n");

	ret = xaf_comp_process(p_adev, p_renderer, NULL, 0, XAF_START_FLAG);
	if (ret != XAF_NO_ERR)
	{
		DSP_PRINTF("xaf_comp_process XAF_START_FLAG renderer failure: %d\r\n", ret);
		goto error_cleanup;
	}

	ret = xaf_comp_get_status(p_adev, p_renderer, &dec_status, &dec_info[0]);
	if (ret != XAF_NO_ERR)
	{
		DSP_PRINTF("xaf_comp_get_status XA_RENDERER failure: %d\r\n", ret);
		return -1;
	}

    /* Initialize buffer read/write functions */
    dsp->audio_read  = DSP_AudioRead;
    dsp->audio_write = DSP_AudioWrite;

    /* Start processing thread */
    ret = xos_thread_create(&dec_thread, NULL, DSP_ProcessThread, (void *)dsp, "DSP_ProcessThread", dec_stack,
                            TASK_STACK_SIZE, 7, 0, 0);
    if (ret != XOS_OK)
    {
        DSP_PRINTF("xos_thread_create failure: %d\r\n", ret);
        goto error_cleanup;
    }

    /* Wait for processing thread to complete before exiting. */
    ret = xos_thread_join(&dec_thread, &exitcode);
    if (ret != XOS_OK)
    {
        DSP_PRINTF("xos_thread_join failure: %d\r\n", ret);
        ret = xos_thread_delete(&dec_thread);
        if (ret != XOS_OK)
        {
            DSP_PRINTF("xos_thread_delete failure: %d\r\n", ret);
        }
        goto error_cleanup;
    }
    if (exitcode != 0)
    {
        DSP_PRINTF("DSP_ProcessThread exit code: %d\r\n", exitcode);
        ret = xos_thread_delete(&dec_thread);
        if (ret != XOS_OK)
        {
            DSP_PRINTF("xos_thread_delete failure: %d\r\n", ret);
        }
        goto error_cleanup;
    }
    ret = xos_thread_delete(&dec_thread);
    if (ret != XOS_OK)
    {
        DSP_PRINTF("xos_thread_delete failure: %d\r\n", ret);
        goto error_cleanup;
    }

    /* Clean up and shut down XAF */
    ret = xaf_comp_delete(p_decoder);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_delete failure: %d\r\n", ret);
        goto error_cleanup;
    }
    p_decoder = NULL;

	ret = xaf_comp_delete(p_renderer);
	if (ret != XAF_NO_ERR)
	{
		DSP_PRINTF("xaf_comp_delete failure: %d\r\n", ret);
		goto error_cleanup;
	}
	p_renderer = NULL;

    TST_CHK_API_ADEV_CLOSE(p_adev, XAF_ADEV_NORMAL_CLOSE, device_config, "xaf_adev_close");
    p_adev = NULL;

    DSP_PRINTF("[DSP Codec] Audio device closed\r\n\r\n");

    return 0;

error_cleanup:
    if (p_adev != NULL)
    {
        TST_CHK_API_ADEV_CLOSE(p_adev, XAF_ADEV_FORCE_CLOSE, device_config, "xaf_adev_close");
    }

    return -1;
}
