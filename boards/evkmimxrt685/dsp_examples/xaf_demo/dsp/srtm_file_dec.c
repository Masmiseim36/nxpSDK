/*
 * Copyright 2018-2019 NXP
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
#include "audio/xa_mp3_dec_api.h"
#include "audio/xa_aac_dec_api.h"
#include "audio/xa_vorbis_dec_api.h"
#include "audio/xa-renderer-api.h"
#include "audio/xa-audio-decoder-api.h"

#include "fsl_common.h"
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
#include "fsl_memory.h"
#endif

#include "srtm_utils.h"
#include "srtm_config_audio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AUDIO_FRMWK_BUF_SIZE (64 * 1024)
#define AUDIO_COMP_BUF_SIZE (256 * 1024)

#define VORBIS_DEC_RAW_VORBIS_LAST_PKT_GRANULE_POS -1
#define VORBIS_DEC_OGG_MAX_PAGE_SIZE 12
#define VORBIS_DEC_RUNTIME_MEM 0

#define MP3_DEC_PCM_WIDTH 16
#define AAC_DEC_PCM_WIDTH 16
#define RENDERER_FRAME_SIZE (4 * 1024)

/* Stack size for DSP data processing thread. */
#define STACK_SIZE (4 * 1024)

char dec_stack[STACK_SIZE];
char buffer_stack[STACK_SIZE];
char cleanup_stack[STACK_SIZE];

int srtm_file_dec_close(void *arg, int wake_value);

/*******************************************************************************
 * Component Setup/ Config
 ******************************************************************************/
static XAF_ERR_CODE srtm_vorbis_setup(void *p_decoder)
{
    int param[8];

    param[0] = XA_VORBISDEC_CONFIG_PARAM_RAW_VORBIS_FILE_MODE;
    param[1] = 0;
    param[2] = XA_VORBISDEC_CONFIG_PARAM_RAW_VORBIS_LAST_PKT_GRANULE_POS;
    param[3] = VORBIS_DEC_RAW_VORBIS_LAST_PKT_GRANULE_POS;
    param[4] = XA_VORBISDEC_CONFIG_PARAM_OGG_MAX_PAGE_SIZE;
    param[5] = VORBIS_DEC_OGG_MAX_PAGE_SIZE;
    param[6] = XA_VORBISDEC_CONFIG_PARAM_RUNTIME_MEM;
    param[7] = VORBIS_DEC_RUNTIME_MEM;

    return xaf_comp_set_config(p_decoder, 4, &param[0]);
}

static XAF_ERR_CODE srtm_aac_setup(void *p_decoder)
{
    int param[2];

    param[0] = XA_AACDEC_CONFIG_PARAM_PCM_WDSZ;
    param[1] = AAC_DEC_PCM_WIDTH;

    return (xaf_comp_set_config(p_decoder, 1, &param[0]));
}

static XAF_ERR_CODE srtm_mp3_setup(void *p_decoder)
{
    int param[2];

    param[0] = XA_MP3DEC_CONFIG_PARAM_PCM_WDSZ;
    param[1] = MP3_DEC_PCM_WIDTH;

    return xaf_comp_set_config(p_decoder, 1, &param[0]);
}

static XAF_ERR_CODE renderer_setup(void *p_renderer, xaf_format_t *format)
{
    int param[8];

    param[0] = XA_RENDERER_CONFIG_PARAM_PCM_WIDTH;
    param[1] = format->pcm_width;
    param[2] = XA_RENDERER_CONFIG_PARAM_CHANNELS;
    param[3] = format->channels;
    param[4] = XA_RENDERER_CONFIG_PARAM_SAMPLE_RATE;
    param[5] = format->sample_rate;
    param[6] = XA_RENDERER_CONFIG_PARAM_FRAME_SIZE;
    param[7] = RENDERER_FRAME_SIZE;

    return xaf_comp_set_config(p_renderer, 4, &param[0]);
}

static XAF_ERR_CODE get_dec_config(void *p_comp, xaf_format_t *comp_format)
{
    XAF_ERR_CODE ret;
    int param[6];

    param[0] = XA_CODEC_CONFIG_PARAM_CHANNELS;
    param[2] = XA_CODEC_CONFIG_PARAM_SAMPLE_RATE;
    param[4] = XA_CODEC_CONFIG_PARAM_PCM_WIDTH;

    ret = xaf_comp_get_config(p_comp, 3, &param[0]);
    if (ret != XAF_NO_ERROR)
        return ret;

    comp_format->channels    = param[1];
    comp_format->sample_rate = param[3];
    comp_format->pcm_width   = param[5];

    return XAF_NO_ERROR;
}

/*******************************************************************************
 * Commands processing
 ******************************************************************************/
int srtm_file_dec_create(dsp_handle_t *dsp, srtm_audio_component_t type)
{
    xaf_comp_status dec_status;
    int dec_info[4];
    void *dec_inbuf[2];
    uint32_t read_length;
    XAF_ERR_CODE ret;
    xaf_format_t dec_format;
    const char *comp_type;
    const char *dec_name;
    XAF_ERR_CODE (*comp_setup)(void *p_comp);

    if (type == DSP_COMPONENT_MP3)
    {
        comp_type  = "audio-decoder/mp3";
        comp_setup = srtm_mp3_setup;
        dec_name   = "mp3_dec";
    }
    else if (type == DSP_COMPONENT_AAC)
    {
        comp_type  = "audio-decoder/aac";
        comp_setup = srtm_aac_setup;
        dec_name   = "aacplus_dec";
    }
    else if (type == DSP_COMPONENT_VORBIS)
    {
        comp_type  = "audio-decoder/vorbis";
        comp_setup = srtm_vorbis_setup;
        dec_name   = "vorbis_dec";
    }
    else
    {
        DSP_PRINTF("invalid codec type: %d\r\n", (int)type);
        return -1;
    }

    ret = xaf_adev_open(&dsp->audio_device, AUDIO_FRMWK_BUF_SIZE, AUDIO_COMP_BUF_SIZE, DSP_Malloc, DSP_Free);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_adev_open failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("[DSP Codec] Audio Device Ready\r\n");

    /* Create decoder component
     * 2 input buffers
     * 1 output buffer for decode to memory buffer
     * 0 output buffer for decode to renderer
     */
    ret = xaf_comp_create(dsp->audio_device, &dsp->comp_codec, comp_type, 2, 0, &dec_inbuf[0], XAF_DECODER);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_comp_create failure: %d\r\n", ret);
        goto error_cleanup;
    }

    comp_setup(dsp->comp_codec);

    DSP_PRINTF("[DSP Codec] Decoder created\r\n");

    /* Start decoder component */
    ret = xaf_comp_process(dsp->audio_device, dsp->comp_codec, NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_comp_process XAF_START_FLAG failure: %d\r\n", ret);
        goto error_cleanup;
    }

    DSP_PRINTF("[DSP Codec] Decoder component started\r\n");

    /* Feed input to decoder component until initialization is complete */
    while (1)
    {
        read_length = DSP_AudioReadRing(dsp, dec_inbuf[0], XAF_INBUF_SIZE);
        if (read_length)
        {
            ret = xaf_comp_process(dsp->audio_device, dsp->comp_codec, dec_inbuf[0], read_length, XAF_INPUT_READY_FLAG);
            if (ret != XAF_NO_ERROR)
            {
                DSP_PRINTF("xaf_comp_process XAF_INPUT_READY_FLAG failure: %d\r\n", ret);
                goto error_cleanup;
            }
        }
        else
        {
            break;
        }

        ret = xaf_comp_get_status(dsp->audio_device, dsp->comp_codec, &dec_status, &dec_info[0]);
        if (ret != XAF_NO_ERROR)
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
        DSP_PRINTF("ERROR: Failed to initialize decoder component\r\n");
        goto error_cleanup;
    }

    /* Store decoder component into context to pass to processing thread */
    dsp->comp = dsp->comp_codec;

    /* Get PCM data format from the decoder */
    get_dec_config(dsp->comp_codec, &dec_format);

    DSP_PRINTF("[DSP Codec] Setting decode playback format:\r\n");
    DSP_PRINTF("  Decoder    : %s\r\n", dec_name);
    DSP_PRINTF("  Sample rate: %d\r\n", dec_format.sample_rate);
    DSP_PRINTF("  Bit Width  : %d\r\n", dec_format.pcm_width);
    DSP_PRINTF("  Channels   : %d\r\n", dec_format.channels);

    ret = xaf_comp_create(dsp->audio_device, &dsp->comp_renderer, "renderer", 0, 0, NULL, XAF_RENDERER);

    /* Setup renderer to match decoded PCM format */
    renderer_setup(dsp->comp_renderer, &dec_format);

    DSP_PRINTF("[DSP Codec] Renderer component created\r\n");

    ret = xaf_connect(dsp->comp_codec, 1, dsp->comp_renderer, 0, 4);

    DSP_PRINTF("[DSP Codec] Connected XA_DECODER -> XA_RENDERER\r\n");

    ret = xaf_comp_process(dsp->comp_codec, dsp->comp_renderer, NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_comp_process XAF_START_FLAG renderer failure: %d\r\n", ret);
        goto error_cleanup;
    }

    ret = xaf_comp_get_status(dsp->audio_device, dsp->comp_renderer, &dec_status, &dec_info[0]);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_comp_get_status XA_RENDERER failure: %d\r\n", ret);
        return -1;
    }

    /* Delete previous cleanup thread if valid */
    if (xos_thread_get_state(&dsp->cleanup_thread) != XOS_THREAD_STATE_INVALID)
    {
        xos_thread_delete(&dsp->cleanup_thread);
        memset((void *)&dsp->cleanup_thread, 0, sizeof(XosThread));
    }

    /* Initialize playback state */
    dsp->file_playing = true;
    dsp->eof          = false;
    xos_event_clear(&dsp->pipeline_event, XOS_EVENT_BITS_ALL);

    /* Initialize buffer read/write functions */
    dsp->audio_read  = DSP_AudioReadRing;
    dsp->audio_write = DSP_AudioWriteRing;

    /* Start processing thread */
    xos_thread_create(&dsp->dec_thread, NULL, DSP_ProcessThread, (void *)dsp, "DSP_ProcessThread", dec_stack,
                      STACK_SIZE, 7, 0, 0);

    /* Start buffer notification thread */
    xos_thread_create(&dsp->buffer_thread, NULL, DSP_BufferThread, (void *)dsp, "DSP_BufferThread", buffer_stack,
                      STACK_SIZE, 7, 0, 0);

    /* Start cleanup/exit thread */
    xos_thread_create(&dsp->cleanup_thread, NULL, srtm_file_dec_close, (void *)dsp, "srtm_file_dec_close",
                      cleanup_stack, STACK_SIZE, 7, 0, 0);

    return 0;

error_cleanup:
    ret = xaf_adev_close(dsp->audio_device, XAF_ADEV_FORCE_CLOSE);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_adev_close failure: %d\r\n", ret);
    }
    else
    {
        DSP_PRINTF("[DSP Codec] Audio device closed\r\n\r\n");
    }

    return ret;
}

int srtm_file_dec_close(void *arg, int wake_value)
{
    XAF_ERR_CODE ret;
    int32_t exitcode;
    dsp_handle_t *dsp = (dsp_handle_t *)arg;

    /* Wait for processing thread to complete before exiting. */
    xos_thread_join(&dsp->dec_thread, &exitcode);
    xos_thread_delete(&dsp->dec_thread);

    /* Wait for buffer request thread to complete before exiting. */
    xos_thread_join(&dsp->buffer_thread, &exitcode);
    xos_thread_delete(&dsp->buffer_thread);

    /* Clean up and shut down XAF */
    ret = xaf_comp_delete(dsp->comp_codec);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_comp_delete failure: %d\r\n", ret);
        return -1;
    }

    ret = xaf_comp_delete(dsp->comp_renderer);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_comp_delete failure: %d\r\n", ret);
        return -1;
    }

    ret = xaf_adev_close(dsp->audio_device, XAF_ADEV_NORMAL_CLOSE);
    if (ret != XAF_NO_ERROR)
    {
        DSP_PRINTF("xaf_adev_close failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("[DSP Codec] Audio device closed\r\n\r\n");

    /* Send notification to ARM core that file playback has completed */
    DSP_SendFileEnd(dsp);

    return 0;
}
