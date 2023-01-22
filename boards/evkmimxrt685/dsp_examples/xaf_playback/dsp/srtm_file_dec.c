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
#include <stdint.h>

#include <xtensa/xos.h>

#include "xaf-utils-test.h"
#include "xa_error_standards.h"
#include "xa_src_pp_api.h"
#if XA_MP3_DECODER
#include "xa_mp3_dec_api.h"
#endif
#if XA_AAC_DECODER
#include "xa_aac_dec_api.h"
#endif
#if XA_VORBIS_DECODER
#include "xa_vorbis_dec_api.h"
#endif
#include "audio/xa-renderer-api.h"
#include "audio/xa-audio-decoder-api.h"
#if XA_CLIENT_PROXY
#include "client_proxy_api.h"
#endif

#include "dsp_config.h"
#include "srtm_utils.h"
#include "srtm_config_audio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AUDIO_FRMWK_BUF_SIZE (64 * 1024)
#define AUDIO_COMP_BUF_SIZE  (256 * 1024)

#define VORBIS_DEC_RAW_VORBIS_LAST_PKT_GRANULE_POS -1
#define VORBIS_DEC_OGG_MAX_PAGE_SIZE               12
#define VORBIS_DEC_RUNTIME_MEM                     0

#define MP3_DEC_PCM_WIDTH 16
#define AAC_DEC_PCM_WIDTH 16

/*
 * 23220us @44100 x2 channels 16 bit = 4096B buffer (1024 16bit-samples per channel)
 * 5806 @44100 x2 channels 16 bit = 1024B buffer (256 samples for each channel)
 * 10000us @44100 x2 channels 16 bit = 4096B buffer (1024 16bit-samples per channel)
 */
#define CLIENT_PROXY_FRAME_SIZE_US (10000)
#define OUTPUT_SAMPLE_RATE         (48000)

static char dec_stack[STACK_SIZE];
static char buffer_stack[STACK_SIZE];
static char cleanup_stack[STACK_SIZE];
static unsigned char event_handler_stack[STACK_SIZE];

int srtm_file_dec_close(void *arg, int wake_value);

/*******************************************************************************
 * Component Setup/ Config
 ******************************************************************************/
static bool need_src(uint32_t sampling_rate)
{
    if (sampling_rate != OUTPUT_SAMPLE_RATE)
    {
        return true;
    }

    return false;
}

static char *comp_name(dsp_handle_t *dsp, void *comp)
{
    if (comp == dsp->comp_src)
    {
        return "XA_SRC";
    }
    else if (comp == dsp->comp_codec)
    {
        return "XA_DECODER";
    }
    else
    {
        return "XA_UNKNOWN";
    }
}

static int src_setup(
    void *p_comp, int channels, int in_sample_rate, int out_sample_rate, int in_frame_size, int pcm_width_bytes)
{
    int param[12];
    param[0]  = XA_SRC_PP_CONFIG_PARAM_INPUT_CHANNELS;
    param[1]  = channels;
    param[2]  = XA_SRC_PP_CONFIG_PARAM_INPUT_SAMPLE_RATE;
    param[3]  = in_sample_rate;
    param[4]  = XA_SRC_PP_CONFIG_PARAM_OUTPUT_SAMPLE_RATE;
    param[5]  = out_sample_rate;
    param[6]  = XA_SRC_PP_CONFIG_PARAM_INPUT_CHUNK_SIZE;
    param[7]  = in_frame_size;
    param[8]  = XA_SRC_PP_CONFIG_PARAM_BYTES_PER_SAMPLE;
    param[9]  = (pcm_width_bytes == 4) ? 3 : 2; // src library only supports 16 or MSB-aligned 24 bit input
    param[10] = XA_SRC_PP_CONFIG_PARAM_ENABLE_CUBIC;
    param[11] = 1;

    return (xaf_comp_set_config(p_comp, 6, &param[0]));
}

#if XA_VORBIS_DECODER
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
#endif

#if XA_AAC_DECODER
static XAF_ERR_CODE srtm_aac_setup(void *p_decoder)
{
    int param[2];

    param[0] = XA_AACDEC_CONFIG_PARAM_PCM_WDSZ;
    param[1] = AAC_DEC_PCM_WIDTH;

    return (xaf_comp_set_config(p_decoder, 1, &param[0]));
}
#endif

#if XA_MP3_DECODER
static XAF_ERR_CODE srtm_mp3_setup(void *p_decoder)
{
    int param[2];

    param[0] = XA_MP3DEC_CONFIG_PARAM_PCM_WDSZ;
    param[1] = MP3_DEC_PCM_WIDTH;

    return xaf_comp_set_config(p_decoder, 1, &param[0]);
}
#endif

static XAF_ERR_CODE renderer_setup(void *p_renderer, xaf_format_t *format)
{
    int param[18];
    int numParam;

    uint32_t sample_rate = format->sample_rate;

    /* Set renderer to use 48 kHz output with 44.1/22.05 kHz media. */
    if (need_src(sample_rate))
    {
        sample_rate = OUTPUT_SAMPLE_RATE;
    }

    numParam = 7;

    param[0]  = XA_RENDERER_CONFIG_PARAM_PCM_WIDTH;
    param[1]  = format->pcm_width;
    param[2]  = XA_RENDERER_CONFIG_PARAM_CHANNELS;
    param[3]  = format->channels;
    param[4]  = XA_RENDERER_CONFIG_PARAM_SAMPLE_RATE;
    param[5]  = sample_rate;
    param[6]  = XA_RENDERER_CONFIG_PARAM_FRAME_SIZE;
    param[7]  = (format->pcm_width >> 3) * format->channels * (sample_rate / 100);
    param[8]  = XA_RENDERER_CONFIG_PARAM_AUDIO_BUFFER_1;
    param[9]  = (int)DSP_AUDIO_BUFFER_1_PING;
    param[10] = XA_RENDERER_CONFIG_PARAM_AUDIO_BUFFER_2;
    param[11] = (int)DSP_AUDIO_BUFFER_1_PONG;
    param[12] = XA_RENDERER_CONFIG_PARAM_I2S_INTERFACE;
    param[13] = AUDIO_I2S_RENDERER_DEVICE;

    return xaf_comp_set_config(p_renderer, numParam, &param[0]);
}

#if XA_CLIENT_PROXY
static int client_proxy_setup(void *p_comp, xaf_format_t *format)
{
    int param[10];
    uint32_t sample_rate = format->sample_rate;

    /* Set client_proxy to use 48 kHz output with 44.1/22.05 kHz media. */
    if (need_src(sample_rate))
    {
        sample_rate = OUTPUT_SAMPLE_RATE;
    }

    param[0] = XA_CLIENT_PROXY_CONFIG_PARAM_CHANNELS;
    param[1] = format->channels;
    param[2] = XA_CLIENT_PROXY_CONFIG_PARAM_SAMPLE_RATE;
    param[3] = sample_rate;
    param[4] = XA_CLIENT_PROXY_CONFIG_PARAM_PCM_WIDTH;
    param[5] = format->pcm_width;
    param[6] = XA_CLIENT_PROXY_CONFIG_PARAM_INPUT_FRAME_SIZE_US;
    param[7] = CLIENT_PROXY_FRAME_SIZE_US;

    return xaf_comp_set_config(p_comp, 4, &param[0]);
}
#endif

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

/*******************************************************************************
 * Commands processing
 ******************************************************************************/
#ifndef XA_DISABLE_EVENT
static int playback_event_handler(event_info_t *event, void *arg)
{
    dsp_handle_t *dsp = (dsp_handle_t *)arg;

    if (!event->comp_error_flag)
        /* ...only fatal error is handled in playback */
        return 0;

    if (dsp->comp_codec != (void *)event->comp_addr)
        /* ...only codec component */
        return 0;

    int error_code = *(int *)event->event_buf;

    // DSP_PRINTF("Playback Event Handler: Error code =%x received for codec \n", error_code);

    if (error_code > 0)
    {
        // DSP_PRINTF("Playback Event Handler: Non Fatal error received, not taking any action\n");
        return 0;
    }

    DSP_PRINTF("Playback Event Handler: Got event, id = %08x\n", event->event_id);
    dsp->event_occur = true;

    return 0;
}
#endif /* XA_DISABLE_EVENT */

int srtm_file_dec_create(dsp_handle_t *dsp, srtm_audio_component_t type)
{
    xaf_comp_status comp_status;
    int comp_info[4];
    void *dec_inbuf[1];
    void *comp;
    uint32_t read_length;
    XAF_ERR_CODE ret;
    xaf_format_t dec_format;
    const char *comp_type = NULL;
    const char *dec_name;
    XAF_ERR_CODE (*comp_setup)(void *p_comp);
    xaf_adev_config_t device_config;
    xaf_comp_config_t comp_config;
#ifndef XA_DISABLE_EVENT
    uint32_t g_enable_error_channel_flag = XAF_ERR_CHANNEL_DISABLE;
#endif /* XA_DISABLE_EVENT */

#if XA_MP3_DECODER
    if (type == DSP_COMPONENT_MP3)
    {
        comp_type  = "audio-decoder/mp3";
        comp_setup = srtm_mp3_setup;
        dec_name   = "mp3_dec";
    }
#endif
#if XA_AAC_DECODER
    if (type == DSP_COMPONENT_AAC)
    {
        comp_type  = "audio-decoder/aac";
        comp_setup = srtm_aac_setup;
        dec_name   = "aacplus_dec";
    }
#endif
#if XA_VORBIS_DECODER
    if (type == DSP_COMPONENT_VORBIS)
    {
        comp_type  = "audio-decoder/vorbis";
        comp_setup = srtm_vorbis_setup;
        dec_name   = "vorbis_dec";
    }
#endif
    if (!comp_type)
    {
        DSP_PRINTF("invalid codec type: %d\r\n", (int)type);
        return -1;
    }

#ifndef XA_DISABLE_EVENT
    dsp->g_event_handler_exit = false;
    if (xa_app_initialize_event_queue() != 0)
        return -1;
    g_app_handler_fn = playback_event_handler;

    g_enable_error_channel_flag = XAF_ERR_CHANNEL_ALL;
#endif

    xaf_adev_config_default_init(&device_config);

    device_config.pmem_malloc = DSP_Malloc;
    device_config.pmem_free   = DSP_Free;
#ifndef XA_DISABLE_EVENT
    device_config.app_event_handler_cb = xa_app_receive_events_cb;
#endif
    device_config.audio_component_buffer_size = AUDIO_COMP_BUF_SIZE;
    device_config.audio_framework_buffer_size = AUDIO_FRMWK_BUF_SIZE;

    ret = xaf_adev_open(&dsp->audio_device, &device_config);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_adev_open failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("[DSP Codec] Audio Device Ready\r\n");

#ifndef XA_DISABLE_EVENT
    /* Start event thread */
    xos_thread_create(&dsp->event_thread, NULL, (XosThreadFunc *)event_handler_entry, (void *)dsp,
                      "Event Handler Thread", event_handler_stack, STACK_SIZE, 7, 0, 0);
#endif
    /* Create decoder component
     * 2 input buffers
     * 1 output buffer for decode to memory buffer
     * 0 output buffer for decode to renderer
     */
    xaf_comp_config_default_init(&comp_config);

#ifndef XA_DISABLE_EVENT
    comp_config.error_channel_ctl = g_enable_error_channel_flag;
#endif /* XA_DISABLE_EVENT */

    comp_config.comp_id            = comp_type;
    comp_config.num_input_buffers  = 1;
    comp_config.num_output_buffers = 0;
    comp_config.pp_inbuf           = (pVOID(*)[XAF_MAX_INBUFS]) & dec_inbuf[0];
    comp_config.comp_type          = XAF_DECODER;

    ret = xaf_comp_create(dsp->audio_device, &dsp->comp_codec, &comp_config);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_create failure: %d\r\n", ret);
        goto error_cleanup;
    }

    ret = comp_setup(dsp->comp_codec);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("comp_setup failure: %d\r\n", ret);
        goto error_cleanup;
    }

    /* Start decoder component */
    ret = xaf_comp_process(dsp->audio_device, dsp->comp_codec, NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERR)
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

        ret = xaf_comp_get_status(dsp->audio_device, dsp->comp_codec, &comp_status, &comp_info[0]);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("xaf_comp_get_status failure: %d\r\n", ret);
            goto error_cleanup;
        }

        if (comp_status == XAF_INIT_DONE || comp_status == XAF_EXEC_DONE)
        {
            break;
        }
    }

    if (comp_status != XAF_INIT_DONE)
    {
        DSP_PRINTF("ERROR: Failed to initialize decoder component: %d\r\n", comp_status);
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

    dsp->file_frame_size = (dec_format.pcm_width >> 3) * dec_format.channels * (OUTPUT_SAMPLE_RATE / 100);
    dsp->channel_num     = dec_format.channels;

    if (need_src(dec_format.sample_rate))
    {
        xaf_comp_config_default_init(&comp_config);

        comp_config.comp_id            = "post-proc/src-pp";
        comp_config.num_input_buffers  = 0;
        comp_config.num_output_buffers = 0;
        comp_config.pp_inbuf           = NULL;
        comp_config.comp_type          = XAF_POST_PROC;

        ret = xaf_comp_create(dsp->audio_device, &dsp->comp_src, &comp_config);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("xaf_comp_create src-pp failure: %d\r\n", ret);
            goto error_cleanup;
        }

        ret = src_setup(dsp->comp_src, dec_format.channels, dec_format.sample_rate, OUTPUT_SAMPLE_RATE, 512,
                        dec_format.pcm_width / 8);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("src_setup failure: %d\r\n", ret);
            goto error_cleanup;
        }

        ret = xaf_comp_process(dsp->audio_device, dsp->comp_src, NULL, 0, XAF_START_FLAG);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("xaf_comp_process SRC XAF_START_FLAG failure: %d\r\n", ret);
            goto error_cleanup;
        }

        ret = xaf_comp_get_status(dsp->audio_device, dsp->comp_src, &comp_status, &comp_info[0]);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("xaf_comp_get_status SRC failure: %d\r\n", ret);
            goto error_cleanup;
        }

        DSP_PRINTF("[DSP Codec] SRC component started\r\n");
    }

#if XA_CLIENT_PROXY
    xaf_comp_config_default_init(&comp_config);

    comp_config.comp_id            = "post-proc/client_proxy";
    comp_config.num_input_buffers  = 0;
    comp_config.num_output_buffers = 0;
    comp_config.pp_inbuf           = NULL;
    comp_config.comp_type          = XAF_POST_PROC;

    /* Create and setup client proxy component */
    ret = xaf_comp_create(dsp->audio_device, &dsp->comp_client_proxy, &comp_config);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_create client_proxy failure: %d\r\n", ret);
        goto error_cleanup;
    }

    ret = client_proxy_setup(dsp->comp_client_proxy, &dec_format);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("client_proxy_setup failure: %d\r\n", ret);
        goto error_cleanup;
    }

    /* Start client proxy component */
    ret = xaf_comp_process(dsp->audio_device, dsp->comp_client_proxy, NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_process XAF_START_FLAG CLIENT_PROXY failure: %d\r\n", ret);
        return ret;
    }

    ret = xaf_comp_get_status(dsp->audio_device, dsp->comp_client_proxy, &comp_status, &comp_info[0]);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_get_status CLIENT_PROXY failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("[DSP Codec] EAP filter component started\r\n");
#endif

    xaf_comp_config_default_init(&comp_config);

    comp_config.comp_id            = "renderer";
    comp_config.num_input_buffers  = 0;
    comp_config.num_output_buffers = 0;
    comp_config.pp_inbuf           = NULL;
    comp_config.comp_type          = XAF_RENDERER;

    ret = xaf_comp_create(dsp->audio_device, &dsp->comp_renderer, &comp_config);

    /* Setup renderer to match decoded PCM format */
    ret = renderer_setup(dsp->comp_renderer, &dec_format);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("renderer_setup failure: %d\r\n", ret);
        goto error_cleanup;
    }

    /* Start renderer component */
    ret = xaf_comp_process(dsp->audio_device, dsp->comp_renderer, NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_process XAF_START_FLAG renderer failure: %d\r\n", ret);
        goto error_cleanup;
    }

    ret = xaf_comp_get_status(dsp->audio_device, dsp->comp_renderer, &comp_status, &comp_info[0]);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_get_status XA_RENDERER failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("[DSP Codec] Renderer component started\r\n");

    // codec -> src -> client_proxy -> renderer

    // if SRC
    //   connect(codec -> src)
    //   comp = src
    // else
    //   comp = codec

    // if CLIENT_PROXY
    //   connect(comp -> client_proxy)
    //   connect(client_proxy -> renderer)
    // else
    //   connect(comp -> renderer)

    if (need_src(dec_format.sample_rate))
    {
        ret = xaf_connect(dsp->comp_codec, 1, dsp->comp_src, 0, 4);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("xaf_connect failure: %d\r\n", ret);
            goto error_cleanup;
        }

        comp = dsp->comp_src;

        DSP_PRINTF("[DSP Codec] Connected XA_DECODER -> XA_SRC\r\n");
    }
    else
    {
        comp = dsp->comp_codec;
    }

#if XA_CLIENT_PROXY
    /* Connect all the non-input components and then only START each of the dest components */
    ret = xaf_connect(comp, 1, dsp->comp_client_proxy, 0, 4);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_connect CLIENT_PROXY failure: %d\r\n", ret);
        return ret;
    }

    DSP_PRINTF("[DSP Codec] Connected %s -> XA_EAP_FILTER\n\r", comp_name(dsp, comp));

    ret = xaf_connect(dsp->comp_client_proxy, 1, dsp->comp_renderer, 0, 4);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_connect RENDERER failure: %d\r\n", ret);
        return ret;
    }

    DSP_PRINTF("[DSP Codec] Connected XA_EAP_FILTER -> XA_RENDERER\n\r");

#else
    ret = xaf_connect(comp, 1, dsp->comp_renderer, 0, 4);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_connect RENDERER failure: %d\r\n", ret);
        goto error_cleanup;
    }

    DSP_PRINTF("[DSP Codec] Connected %s -> XA_RENDERER\r\n", comp_name(dsp, comp));
#endif

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
    /* Close audio device */
    if (dsp->audio_device != NULL)
    {
        ret = xaf_adev_close(dsp->audio_device, XAF_ADEV_FORCE_CLOSE);
        if (ret != XAF_NO_ERR)
            DSP_PRINTF("xaf_adev_close failure: %d\r\n", ret);
        else
            DSP_PRINTF("[DSP Codec] Audio device closed\r\n\r\n");
    }

#ifndef XA_DISABLE_EVENT
    dsp->g_event_handler_exit = true;
    xos_thread_join(&dsp->event_thread, NULL);
    xos_thread_delete(&dsp->event_thread);
    xa_app_free_event_queue();
#endif

    DSP_SendFileError(dsp);
    return -1;
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
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_delete failure: %d\r\n", ret);
        return -1;
    }

    if (dsp->comp_src)
    {
        ret = xaf_comp_delete(dsp->comp_src);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("xaf_comp_delete failure: %d\r\n", ret);
            return -1;
        }
        dsp->comp_src = NULL;
    }

    ret = xaf_comp_delete(dsp->comp_renderer);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_delete failure: %d\r\n", ret);
        return -1;
    }

#if XA_CLIENT_PROXY
    ret = xaf_comp_delete(dsp->comp_client_proxy);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_comp_delete failure: %d\r\n", ret);
        return -1;
    }
#endif

    ret = xaf_adev_close(dsp->audio_device, XAF_ADEV_NORMAL_CLOSE);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("xaf_adev_close failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("[DSP Codec] Audio device closed\r\n\r\n");

#ifndef XA_DISABLE_EVENT
    dsp->g_event_handler_exit = true;
    xos_thread_join(&dsp->event_thread, &exitcode);
    xos_thread_delete(&dsp->event_thread);
    xa_app_free_event_queue();
#endif

    /* Send notification to ARM core that file playback has completed */
    DSP_SendFileEnd(dsp);

    return 0;
}

#if XA_CLIENT_PROXY
int client_proxy_filter(dsp_handle_t *dsp, int filterNum)
{
    int param[2];

    param[0] = XA_CLIENT_PROXY_CONFIG_PARAM_EAP;
    param[1] = filterNum;

    return xaf_comp_set_config(dsp->comp_client_proxy, 1, &param[0]);
}
#endif
