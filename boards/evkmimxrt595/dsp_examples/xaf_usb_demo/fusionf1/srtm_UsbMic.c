/*
 * Copyright 2021-2023 NXP
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

#ifdef RENDERER_I2S
#include "fsl_i2s.h"
#else
#include "fsl_sai.h"
#endif
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
#include "fsl_memory.h"
#endif
#include "xaf-utils-test.h"
#include "xaf-fio-test.h"
#include "xa_error_standards.h"
#include "audio/xa-capturer-api.h"
#include "audio/xa-pcm-gain-api.h"

#include "dsp_config.h"
#include "srtm_utils.h"
#include "srtm_config_audio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define AUDIO_FRMWK_BUF_SIZE (32 * 1024)
#define AUDIO_COMP_BUF_SIZE  (101 * 1024)
int audio_frmwk_buf_size;
int audio_comp_buf_size;

#define CAPTURER_FRAME_SIZE (192)
#define PCM_GAIN_FRAME_SIZE (192)

enum
{
    XA_COMP = -1,
    XA_CAPTURER_0,
    XA_GAIN_0,
    NUM_COMP_IN_GRAPH,
};

const int comp_get_order_mic[] = {XA_CAPTURER_0, XA_GAIN_0};

/*******************************************************************************
 * Component Setup/ Config
 ******************************************************************************/
unsigned char UsbMic_process_stack[STACK_SIZE];
unsigned char UsbMic_cleanup_stack[STACK_SIZE];
int srtm_usbMic_cleanup(void *arg, int wake_value);
static xaf_adev_config_t device_config;

static void *comp_get_pointer(dsp_handle_t *dsp, int cid, bool addr)
{
    switch (cid)
    {
        case XA_CAPTURER_0:
            return (addr ? &dsp->comp_capturer : dsp->comp_capturer);
        case XA_GAIN_0:
            return (addr ? &dsp->comp_gain : dsp->comp_gain);
        default:
            DSP_PRINTF("[DSP_USB_MIC] Check component type in comp_get_order_mic table\n\r");
            return NULL;
    }
}

static void comp_clean(dsp_handle_t *dsp)
{
    dsp->audio_device  = NULL;
    dsp->comp          = NULL;
    dsp->comp_capturer = NULL;
    dsp->comp_gain     = NULL;
}

static int UsbMic_close(dsp_handle_t *dsp, bool success)
{
    int i, cid;
    XAF_ERR_CODE ret;

    for (i = 0; i < NUM_COMP_IN_GRAPH; i++)
    {
        cid = comp_get_order_mic[i];
        if (comp_get_pointer(dsp, cid, false) == NULL)
            continue;
        ret = xaf_comp_delete(comp_get_pointer(dsp, cid, false));
        if (ret != XAF_NO_ERR)
            DSP_PRINTF("[DSP_USB_MIC] xaf_comp_delete[%d] failure: %d\r\n", i, ret);
    }

    void * p_adev = dsp->audio_device;
    TST_CHK_API_ADEV_CLOSE(p_adev, XAF_ADEV_NORMAL_CLOSE, device_config, "xaf_adev_close");

    /* Send message to the application */
    if (success)
        DSP_SendUsbEnd(dsp, USB_MicDevice);
    else
        DSP_SendUsbError(dsp, USB_MicDevice);

    return (success ? 0 : -1);
}

static int capturer_setup(void *p_capturer, xaf_format_t *format, bool i2s)
{
    int param[22];
    int num_params = 4;

    param[0] = XA_CAPTURER_CONFIG_PARAM_PCM_WIDTH;
    param[1] = format->pcm_width;
    param[2] = XA_CAPTURER_CONFIG_PARAM_CHANNELS;
    param[3] = format->channels;
    param[4] = XA_CAPTURER_CONFIG_PARAM_SAMPLE_RATE;
    param[5] = format->sample_rate;
    param[6] = XA_CAPTURER_CONFIG_PARAM_FRAME_SIZE;
    param[7] = CAPTURER_FRAME_SIZE;

#ifdef AUDIO_I2S_CAPTURER_DEVICE
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
#endif

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

/*******************************************************************************
 * Commands processing
 ******************************************************************************/
int srtm_usb_mic_init(dsp_handle_t *dsp, unsigned int *pCmdParams, bool i2s)
{
    int channels, sampling_rate, width, cid, cid_prev, i;
    xaf_format_t comp_format[NUM_COMP_IN_GRAPH];
    int (*comp_setup[NUM_COMP_IN_GRAPH])(void *p_comp, xaf_format_t *, bool);
    void *usbMic_inbuf[1];
    void *p_adev =  dsp->audio_device;
    xaf_comp_type comp_type[NUM_COMP_IN_GRAPH];
    xf_id_t comp_id[NUM_COMP_IN_GRAPH];
    int comp_ninbuf[NUM_COMP_IN_GRAPH];
    int comp_noutbuf[NUM_COMP_IN_GRAPH];
    XAF_ERR_CODE ret;
    xaf_comp_status comp_status;
    xaf_comp_config_t comp_config[NUM_COMP_IN_GRAPH];
    int comp_info[4];

    /* Parameters reading */
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
    dsp->buffer_out.data = (char *)MEMORY_ConvertMemoryMapAddress(pCmdParams[0], kMEMORY_Local2DMA);
#else
    dsp->buffer_out.data = (char *)pCmdParams[0];
#endif

    dsp->buffer_out.size  = (uint32_t)pCmdParams[1];
    dsp->buffer_out.index = 0;
    channels              = pCmdParams[2];
    sampling_rate         = pCmdParams[3];
    width                 = pCmdParams[4];

    comp_clean(dsp);

    /* Component data preparation */
    for (i = 0; i < NUM_COMP_IN_GRAPH; i++)
    {
        cid = comp_get_order_mic[i];
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
                comp_id[cid]                 = i2s ? "capturer_i2s" : "capturer";
                break;
            case XA_GAIN_0:
                comp_format[cid].sample_rate = sampling_rate;
                comp_format[cid].channels    = channels;
                comp_format[cid].pcm_width   = width;
                comp_setup[cid]              = pcm_gain_setup;
                comp_type[cid]               = XAF_POST_PROC;
                comp_id[cid]                 = "post-proc/pcm_gain";
                comp_noutbuf[cid]            = 1;
                break;
            default:
                DSP_PRINTF("[DSP_USB_MIC] Check component type in comp_get_order_mic table\n\r");
                return -1;
        }
    }

    /* Initialize XAF */
    xaf_adev_config_default_init(&device_config);

    audio_frmwk_buf_size = AUDIO_FRMWK_BUF_SIZE;
    audio_comp_buf_size = AUDIO_COMP_BUF_SIZE;
    device_config.mem_pool[XAF_MEM_ID_COMP].size = AUDIO_COMP_BUF_SIZE;
    device_config.mem_pool[XAF_MEM_ID_DEV].size = AUDIO_FRMWK_BUF_SIZE;
    device_config.core = XF_CORE_ID;

    TST_CHK_API_ADEV_OPEN(p_adev, device_config, "[DSP Codec] Audio Device Open\r\n");
    dsp->audio_device = p_adev;
    DSP_PRINTF("[DSP_USB_MIC] Audio Device Ready\n\r");

    /* Create and setup all components */
    for (i = 0; i < NUM_COMP_IN_GRAPH; i++)
    {
        cid = comp_get_order_mic[i];

        xaf_comp_config_default_init(&comp_config[cid]);

        comp_config[cid].comp_id            = comp_id[cid];
        comp_config[cid].num_input_buffers  = comp_ninbuf[cid];
        comp_config[cid].num_output_buffers = comp_noutbuf[cid];
        comp_config[cid].comp_type          = comp_type[cid];
        comp_config[cid].pp_inbuf = comp_ninbuf[cid] > 0 ? (pVOID(*)[XAF_MAX_INBUFS]) & usbMic_inbuf[0] : NULL;

        /* Create component */
        ret = xaf_comp_create(p_adev, comp_get_pointer(dsp, cid, true), &comp_config[cid]);
        if (ret != XAF_NO_ERR)
        {
            DSP_PRINTF("[DSP_USB_MIC] xaf_comp_create[%d] failure: %d\r\n", i, ret);
            goto error_cleanup;
        }

        /* Setup component */
        comp_setup[cid](comp_get_pointer(dsp, cid, false), &comp_format[cid], i2s);

        if (i == (NUM_COMP_IN_GRAPH - 1))
            dsp->comp = comp_get_pointer(dsp, cid, false);
    }

    /* Start capturer */
    cid = XA_CAPTURER_0;
    ret = xaf_comp_process(p_adev, comp_get_pointer(dsp, cid, false), NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("[DSP_USB_MIC] xaf_comp_process XAF_START_FLAG %s failure: %d\r\n", comp_id[cid], ret);
        goto error_cleanup;
    }

    ret = xaf_comp_get_status(p_adev, comp_get_pointer(dsp, cid, false), &comp_status, &comp_info[0]);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("[DSP_USB_MIC] xaf_comp_get_status %s failure: %d\r\n", comp_id[cid], ret);
        goto error_cleanup;
    }

    /* Connect all the non-input components and then only START each of the dest components */
    cid_prev = XA_CAPTURER_0;
    cid      = XA_GAIN_0;
    ret      = xaf_connect(comp_get_pointer(dsp, cid_prev, false), 0, comp_get_pointer(dsp, cid, false), 0, 4);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("[DSP_USB_MIC] xaf_connect failure: %d\r\n", ret);
        goto error_cleanup;
    }

    DSP_PRINTF("[DSP_USB_MIC] Connected %s -> %s\n\r", comp_id[cid_prev], comp_id[cid]);

    cid = XA_CAPTURER_0;
    ret = capturer_start_operation(comp_get_pointer(dsp, cid, false));
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("[DSP_USB_MIC] Capturer start operation failure.\r\n");
        goto error_cleanup;
    }

    /* Start PCM gain */
    cid = XA_GAIN_0;
    ret = xaf_comp_process(p_adev, comp_get_pointer(dsp, cid, false), NULL, 0, XAF_START_FLAG);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("[DSP_USB_MIC] xaf_comp_process XAF_START_FLAG %s failure: %d\r\n", comp_id[cid], ret);
        goto error_cleanup;
    }

    ret = xaf_comp_get_status(p_adev, comp_get_pointer(dsp, cid, false), &comp_status, &comp_info[0]);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("[DSP_USB_MIC] xaf_comp_get_status %s failure: %d\r\n", comp_id[cid], ret);
        goto error_cleanup;
    }

    /* Delete previous cleanup thread if valid */
    if (xos_thread_get_state(&dsp->cleanup_thread) != XOS_THREAD_STATE_INVALID)
    {
        xos_thread_delete(&dsp->cleanup_thread);
        memset((void *)&dsp->cleanup_thread, 0, sizeof(XosThread));
    }

    /* Initialize record state */
    dsp->usb_recording = true;
    xos_event_clear(&dsp->pipeline_event, XOS_EVENT_BITS_ALL);

    /* Initialize buffer read/write functions */
    dsp->audio_read  = DSP_AudioRead;
    dsp->audio_write = DSP_AudioWrite;

    /* Threads */
    /* Start processing thread */
    xos_thread_create(&dsp->process_thread, NULL, DSP_ProcessThread, (void *)dsp, "DSP_ProcessThread",
                      UsbMic_process_stack, STACK_SIZE, 7, 0, 0);

    /* Start cleanup/exit thread */
    xos_thread_create(&dsp->cleanup_thread, NULL, srtm_usbMic_cleanup, (void *)dsp, "srtm_usbMic_cleanup",
                      UsbMic_cleanup_stack, STACK_SIZE, 7, 0, 0);

    return 0;

error_cleanup:
    return UsbMic_close(dsp, false);
}

int srtm_usbMic_cleanup(void *arg, int wake_value)
{
    int32_t exitcode;
    dsp_handle_t *dsp = (dsp_handle_t *)arg;

    DSP_PRINTF("[DSP_USB_MIC] Cleanup start\r\n");

    /* Wait for processing thread to complete before exiting. */
    xos_thread_join(&dsp->process_thread, &exitcode);
    xos_thread_delete(&dsp->process_thread);
    memset((void *)&dsp->process_thread, 0, sizeof(XosThread));

    return UsbMic_close(dsp, true);
}
