/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <xtensa/xos.h>

#include "xaf-utils-test.h"
#include "xa_error_standards.h"

#include "audio/xa-pcm-gain-api.h"
#include "audio/xa-renderer-api.h"

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

#define PCM_GAIN_FRAME_SIZE (192)
#define RENDERER_FRAME_SIZE (192)

enum
{
    XA_COMP = -1,
    XA_GAIN_0,
#if XA_CLIENT_PROXY
    XA_CLIENT_PROXY_0,
#endif
    XA_RENDERER_0,
    NUM_COMP_IN_GRAPH,
};

const int comp_get_order[] = {XA_GAIN_0,
#if XA_CLIENT_PROXY
                              XA_CLIENT_PROXY_0,
#endif
                              XA_RENDERER_0};

#if XA_CLIENT_PROXY
#define CLIENT_PROXY_FRAME_SIZE_US \
    (1000) // 1000us @48000 x2 channels 16 bit = 4096B buffer (192 16bit-samples per channel)
#endif

/*******************************************************************************
 * Component Setup/ Config
 ******************************************************************************/
unsigned char UsbSpeaker_process_stack[STACK_SIZE];
unsigned char UsbSpeaker_cleanup_stack[STACK_SIZE];
int srtm_usbSpeaker_cleanup(void *arg, int wake_value);

static void *comp_get_pointer(dsp_handle_t *dsp, int cid, bool addr)
{
    switch (cid)
    {
        case XA_GAIN_0:
            return (addr ? &dsp->comp_gain : dsp->comp_gain);
#if XA_CLIENT_PROXY
        case XA_CLIENT_PROXY_0:
            return (addr ? &dsp->comp_client_proxy : dsp->comp_client_proxy);
#endif
        case XA_RENDERER_0:
            return (addr ? &dsp->comp_renderer : dsp->comp_renderer);
        default:
            DSP_PRINTF("[DSP_USB_SPEAKER] Check component type in comp_get_order table\n\r");
            return NULL;
    }
}

static void comp_clean(dsp_handle_t *dsp)
{
    dsp->audio_device      = NULL;
    dsp->comp_gain         = NULL;
    dsp->comp_renderer     = NULL;
    dsp->comp_client_proxy = NULL;
    dsp->comp              = NULL;
}

static int UsbSpeaker_close(dsp_handle_t *dsp, bool success)
{
    int i, cid;
    XAF_ERR_CODE ret;

    for (i = 0; i < NUM_COMP_IN_GRAPH; i++)
    {
        cid = comp_get_order[i];
        if (comp_get_pointer(dsp, cid, false) == NULL)
            continue;
        ret = xaf_comp_delete(comp_get_pointer(dsp, cid, false));
        if (ret != XAF_NO_ERR)
            DSP_PRINTF("[DSP_USB_SPEAKER] xaf_comp_delete[%d] failure: %d\r\n", i, ret);
    }

    ret = xaf_adev_close(dsp->audio_device, XAF_ADEV_NORMAL_CLOSE);
    if (ret != XAF_NO_ERR)
        DSP_PRINTF("[DSP_USB_SPEAKER] xaf_adev_close failure: %d\r\n", ret);
    else
        DSP_PRINTF("[DSP_USB_SPEAKER] Audio device closed\r\n\r\n");

    /* Send message to the application */
    if (success)
        DSP_SendUsbEnd(dsp, USB_SpeakerDevice);
    else
        DSP_SendUsbError(dsp, USB_SpeakerDevice);

    return (success ? 0 : -1);
}

static int pcm_gain_setup(void *p_comp, xaf_format_t *format)
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
    param[9] = 10;

    return xaf_comp_set_config(p_comp, 5, &param[0]);
}

#if XA_CLIENT_PROXY
static int client_proxy_setup(void *p_comp, xaf_format_t *format)
{
    int param[10];

    param[0] = XA_CLIENT_PROXY_CONFIG_PARAM_CHANNELS;
    param[1] = format->channels;
    param[2] = XA_CLIENT_PROXY_CONFIG_PARAM_SAMPLE_RATE;
    param[3] = format->sample_rate;
    param[4] = XA_CLIENT_PROXY_CONFIG_PARAM_PCM_WIDTH;
    param[5] = format->pcm_width;
    param[6] = XA_CLIENT_PROXY_CONFIG_PARAM_INPUT_FRAME_SIZE_US;
    param[7] = CLIENT_PROXY_FRAME_SIZE_US;

    return xaf_comp_set_config(p_comp, 4, &param[0]);
}
#endif

static XAF_ERR_CODE renderer_setup(void *p_renderer, xaf_format_t *format)
{
    int param[14];

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

    return xaf_comp_set_config(p_renderer, 7, &param[0]);
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
int srtm_usb_speaker_init(dsp_handle_t *dsp, unsigned int *pCmdParams)
{
    int channels, sampling_rate, width, cid, cid_prev, i;
    xaf_format_t comp_format[NUM_COMP_IN_GRAPH];
    int (*comp_setup[NUM_COMP_IN_GRAPH])(void *p_comp, xaf_format_t *);
    void *usbSpeaker_inbuf[1];
    xaf_comp_type comp_type[NUM_COMP_IN_GRAPH];
    xf_id_t comp_id[NUM_COMP_IN_GRAPH];
    int comp_ninbuf[NUM_COMP_IN_GRAPH];
    int comp_noutbuf[NUM_COMP_IN_GRAPH];
    XAF_ERR_CODE ret;
    xaf_comp_status comp_status;
    xaf_adev_config_t device_config;
    xaf_comp_config_t comp_config[NUM_COMP_IN_GRAPH];
    int comp_info[4];
    uint32_t read_length;

    /* Parameters reading */
    channels      = pCmdParams[0];
    sampling_rate = pCmdParams[1];
    width         = pCmdParams[2];

    comp_clean(dsp);

    /* Component data preparation */
    for (i = 0; i < NUM_COMP_IN_GRAPH; i++)
    {
        cid = comp_get_order[i];
        memset(&comp_format[cid], 0, sizeof(xaf_format_t));
        comp_setup[cid]   = NULL;
        comp_type[cid]    = -1;
        comp_id[cid]      = NULL;
        comp_ninbuf[cid]  = 0;
        comp_noutbuf[cid] = 0;

        switch (cid)
        {
            case XA_GAIN_0:
                comp_format[cid].sample_rate = sampling_rate;
                comp_format[cid].channels    = channels;
                comp_format[cid].pcm_width   = width;
                comp_setup[cid]              = pcm_gain_setup;
                comp_type[cid]               = XAF_POST_PROC;
                comp_id[cid]                 = "post-proc/pcm_gain";
                comp_ninbuf[cid]             = 1;
                break;
#if XA_CLIENT_PROXY
            case XA_CLIENT_PROXY_0:
                comp_format[cid].sample_rate = sampling_rate;
                comp_format[cid].channels    = channels;
                comp_format[cid].pcm_width   = width;
                comp_setup[cid]              = client_proxy_setup;
                comp_type[cid]               = XAF_POST_PROC;
                comp_id[cid]                 = "post-proc/client_proxy";
                break;
#endif
            case XA_RENDERER_0:
                comp_format[cid].sample_rate = sampling_rate;
                comp_format[cid].channels    = channels;
                comp_format[cid].pcm_width   = width;
                comp_setup[cid]              = renderer_setup;
                comp_type[cid]               = XAF_RENDERER;
                comp_id[cid]                 = "renderer";
                break;

            default:
                DSP_PRINTF("[DSP_USB_SPEAKER] Check component type in comp_get_order table\n\r");
                return -1;
        }
    }

    /* Initialize XAF */
    xaf_adev_config_default_init(&device_config);

    device_config.pmem_malloc                 = DSP_Malloc;
    device_config.pmem_free                   = DSP_Free;
    device_config.audio_component_buffer_size = AUDIO_COMP_BUF_SIZE;
    device_config.audio_framework_buffer_size = AUDIO_FRMWK_BUF_SIZE;

    ret = xaf_adev_open(&dsp->audio_device, &device_config);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("[DSP_USB_SPEAKER] xaf_adev_open failure: %d\r\n", ret);
        return -1;
    }

    DSP_PRINTF("[DSP_USB_SPEAKER] Audio Device Ready\n\r");

    /* Create and setup all components */
    for (i = 0; i < NUM_COMP_IN_GRAPH; i++)
    {
        cid = comp_get_order[i];

        xaf_comp_config_default_init(&comp_config[cid]);

        comp_config[cid].comp_id            = comp_id[cid];
        comp_config[cid].num_input_buffers  = comp_ninbuf[cid];
        comp_config[cid].num_output_buffers = comp_noutbuf[cid];
        comp_config[cid].comp_type          = comp_type[cid];
        comp_config[cid].pp_inbuf = comp_ninbuf[cid] > 0 ? (pVOID(*)[XAF_MAX_INBUFS]) & usbSpeaker_inbuf[0] : NULL;

        /* Create component */
        ret = xaf_comp_create(dsp->audio_device, comp_get_pointer(dsp, cid, true), &comp_config[cid]);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("[DSP_USB_SPEAKER] xaf_comp_create[%d] failure: %d\r\n", i, ret);
            goto error_cleanup;
        }

        /* Setup component */
        comp_setup[cid](comp_get_pointer(dsp, cid, false), &comp_format[cid]);

        /* Start component */
        ret = xaf_comp_process(dsp->audio_device, comp_get_pointer(dsp, cid, false), NULL, 0, XAF_START_FLAG);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("[DSP_USB_SPEAKER] xaf_comp_process XAF_START_FLAG %s failure: %d\r\n", comp_id[cid], ret);
            goto error_cleanup;
        }

        /* Check status of the component */
        ret = xaf_comp_get_status(dsp->audio_device, comp_get_pointer(dsp, cid, false), &comp_status, &comp_info[0]);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("[DSP_USB_SPEAKER] xaf_comp_get_status %s failure: %d\r\n", comp_id[cid], ret);
            goto error_cleanup;
        }

        if (i == 0)
        {
            if (comp_status != XAF_INIT_DONE)
            {
                DSP_PRINTF("[DSP_USB_SPEAKER] xaf_comp_get_status failed to init\n\r");
                goto error_cleanup;
            }
            dsp->comp = comp_get_pointer(dsp, cid, false);
        }

        DSP_PRINTF("[DSP_USB_SPEAKER] %s component started\r\n", comp_id[cid]);

        /* Start renderer DMA output.  Will output I2S zeros until valid data is available. */
        if (cid == XA_RENDERER_0)
            renderer_start_operation(comp_get_pointer(dsp, cid, false));

        /* Connect all components */
        if (i > 0)
        {
            cid_prev = comp_get_order[i - 1];
            ret      = xaf_connect(comp_get_pointer(dsp, cid_prev, false), 1, comp_get_pointer(dsp, cid, false), 0, 4);
            if (ret != XAF_NO_ERR)
            {
                DSP_PRINTF("[DSP_USB_SPEAKER] xaf_connect failure: %d\r\n", ret);
                goto error_cleanup;
            }

            DSP_PRINTF("[DSP_USB_SPEAKER] Connected %s -> %s\n\r", comp_id[cid_prev], comp_id[cid]);
        }
    }

    read_length = DSP_AudioReadRing(dsp, usbSpeaker_inbuf[0], PCM_GAIN_FRAME_SIZE);
    if (read_length > 0)
        ret = xaf_comp_process(dsp->audio_device, dsp->comp, usbSpeaker_inbuf[0], read_length, XAF_INPUT_READY_FLAG);
    else
    {
        DSP_PRINTF("[DSP_USB_SPEAKER] Initial data not available\r\n");
        goto error_cleanup;
    }
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("[DSP_USB_SPEAKER] xaf_comp_process XAF_INPUT_READY_FLAG failure: %d\r\n", ret);
        goto error_cleanup;
    }

    /* Delete previous cleanup thread if valid */
    if (xos_thread_get_state(&dsp->cleanup_thread) != XOS_THREAD_STATE_INVALID)
    {
        xos_thread_delete(&dsp->cleanup_thread);
        memset((void *)&dsp->cleanup_thread, 0, sizeof(XosThread));
    }

    /* Initialize playback state */
    dsp->usb_playing = true;
    xos_event_clear(&dsp->pipeline_event, XOS_EVENT_BITS_ALL);

    /* Initialize buffer read/write functions */
    dsp->audio_read  = DSP_AudioReadRing;
    dsp->audio_write = DSP_AudioWriteRing;

    /* Threads */
    /* Start processing thread */
    xos_thread_create(&dsp->process_thread, NULL, DSP_ProcessThread, (void *)dsp, "DSP_ProcessThread",
                      UsbSpeaker_process_stack, STACK_SIZE, 7, 0, 0);

    /* Start cleanup/exit thread */
    xos_thread_create(&dsp->cleanup_thread, NULL, srtm_usbSpeaker_cleanup, (void *)dsp, "srtm_usbSpeaker_cleanup",
                      UsbSpeaker_cleanup_stack, STACK_SIZE, 7, 0, 0);

    return 0;

error_cleanup:
    return UsbSpeaker_close(dsp, false);
}

int srtm_usbSpeaker_cleanup(void *arg, int wake_value)
{
    int32_t exitcode;
    dsp_handle_t *dsp = (dsp_handle_t *)arg;

    DSP_PRINTF("[DSP_CleanupThread] start\r\n");

    /* Wait for processing thread to complete before exiting. */
    xos_thread_join(&dsp->process_thread, &exitcode);
    xos_thread_delete(&dsp->process_thread);

    return UsbSpeaker_close(dsp, true);
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
