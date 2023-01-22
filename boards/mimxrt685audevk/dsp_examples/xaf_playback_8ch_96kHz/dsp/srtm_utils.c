/*
 * Copyright 2019-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "dsp_config.h"
#include "rpmsg_lite.h"
#include "srtm_utils.h"
#include "srtm_config.h"

#include "xaf-utils-test.h"

#include "fsl_gpio.h"

#define AUDIO_BUFFER_FILL_THRESHOLD (16 * 1024)

/*******************************************************************************
 * Utility Functions
 ******************************************************************************/

/* Wrap stdlib malloc() for audio framework allocator */
void *DSP_Malloc(int32_t size, int32_t id)
{
    /* If malloc can return un-aligned data, must return 4-byte aligned pointer for XAF. */
    return malloc(size);
}

/* Wrap stdlib free() for audio framework allocator */
void DSP_Free(void *ptr, int32_t id)
{
    free(ptr);
}

uint32_t DSP_AudioReadRing(dsp_handle_t *dsp, char *data, uint32_t size)
{
    uint32_t bytes_read;

    xos_mutex_lock(&dsp->audioMutex);
    bytes_read = ringbuf_read(dsp->audioBuffer, (uint8_t *)data, size);
    xos_mutex_unlock(&dsp->audioMutex);

    if (bytes_read != size)
    {
        /* UNDERRUN */
    }

    /* Send signal to wake DSP_BufferThread */
    if (dsp->file_playing)
        xos_cond_signal(&dsp->read_waiting_cond, 0);

    return bytes_read;
}

uint32_t DSP_AudioWriteRing(dsp_handle_t *dsp, char *data, uint32_t size)
{
    uint32_t written;

    xos_mutex_lock(&dsp->audioMutex);
    written = ringbuf_write(dsp->audioBuffer, (uint8_t *)data, size);
    xos_mutex_unlock(&dsp->audioMutex);

    if (written != size)
    {
        /* OVERFLOW */
    }

    return written;
}

uint32_t DSP_AudioSizeRing(dsp_handle_t *dsp)
{
    uint32_t size;

    xos_mutex_lock(&dsp->audioMutex);
    size = ringbuf_get_occupancy(dsp->audioBuffer);
    xos_mutex_unlock(&dsp->audioMutex);

    return size;
}

static void DSP_RequestData(dsp_handle_t *dsp)
{
    srtm_message msg = {0};

    msg.head.type         = SRTM_MessageTypeRequest;
    msg.head.majorVersion = SRTM_VERSION_MAJOR;
    msg.head.minorVersion = SRTM_VERSION_MINOR;

    msg.head.category = SRTM_MessageCategory_AUDIO;
    msg.head.command  = SRTM_Command_FileData;

    dsp->ipc_waiting = true;

    xos_mutex_lock(&dsp->rpmsgMutex);
    rpmsg_lite_send(dsp->rpmsg, dsp->ept, MCU_EPT_ADDR, (char *)&msg, sizeof(srtm_message), RL_DONT_BLOCK);
    xos_mutex_unlock(&dsp->rpmsgMutex);
}

void DSP_SendFileEnd(dsp_handle_t *dsp)
{
    srtm_message msg = {0};

    msg.head.type         = SRTM_MessageTypeRequest;
    msg.head.majorVersion = SRTM_VERSION_MAJOR;
    msg.head.minorVersion = SRTM_VERSION_MINOR;

    msg.head.category = SRTM_MessageCategory_AUDIO;
    msg.head.command  = SRTM_Command_FileEnd;

    xos_mutex_lock(&dsp->rpmsgMutex);
    rpmsg_lite_send(dsp->rpmsg, dsp->ept, MCU_EPT_ADDR, (char *)&msg, sizeof(srtm_message), RL_DONT_BLOCK);
    xos_mutex_unlock(&dsp->rpmsgMutex);
}

void DSP_SendFileError(dsp_handle_t *dsp)
{
    srtm_message msg = {0};

    msg.head.type         = SRTM_MessageTypeRequest;
    msg.head.majorVersion = SRTM_VERSION_MAJOR;
    msg.head.minorVersion = SRTM_VERSION_MINOR;

    msg.head.category = SRTM_MessageCategory_AUDIO;
    msg.head.command  = SRTM_Command_FileError;

    xos_mutex_lock(&dsp->rpmsgMutex);
    rpmsg_lite_send(dsp->rpmsg, dsp->ept, MCU_EPT_ADDR, (char *)&msg, sizeof(srtm_message), RL_DONT_BLOCK);
    xos_mutex_unlock(&dsp->rpmsgMutex);
}

int DSP_BufferThread(void *arg, int wake_value)
{
    dsp_handle_t *ctx = (dsp_handle_t *)arg;

    DSP_PRINTF("[DSP_BufferThread] start\r\n");

    while (ctx->file_playing)
    {
        /* Send request to ARM core for more input data */
        DSP_RequestData(ctx);

        while (ctx->file_playing && ctx->ipc_waiting)
            xos_cond_wait(&ctx->ipc_waiting_cond, NULL, NULL);

        if (ctx->eof)
            break;

        xos_mutex_lock(&ctx->audioMutex);
        while (ctx->file_playing && (ringbuf_get_occupancy(ctx->audioBuffer) >= AUDIO_BUFFER_FILL_THRESHOLD))
            xos_cond_wait_mutex(&ctx->read_waiting_cond, &ctx->audioMutex);
        xos_mutex_unlock(&ctx->audioMutex);
    }

    DSP_PRINTF("[DSP_BufferThread] exiting\r\n");

    return 0;
}

/* Thread for processing DSP pipeline
 *
 * This thread will take care of polling the audio framework for status, feeding
 * data when needed, and consuming output when needed.  It will end when the
 * input or output data is exhausted.
 */
int DSP_ProcessThread(void *arg, int wake_value)
{
    dsp_handle_t *ctx = (dsp_handle_t *)arg;
    xaf_comp_status status;
    XAF_ERR_CODE ret;
    char *buffer;
    uint32_t buffer_len;
    uint32_t event_state;
    int info[4];
    int read_size, write_size;
    bool cmd_pending = true;

    DSP_PRINTF("[DSP_ProcessThread] start\r\n");

    ret = xaf_comp_process(NULL, ctx->comp, NULL, 0, XAF_EXEC_FLAG);
    if (ret != XAF_NO_ERR)
    {
        DSP_PRINTF("[DSP_ProcessThread] xaf_comp_process XAF_EXEC_FLAG failure: %d\r\n", ret);
        return -1;
    }

    while (1)
    {
        /* Check for external events to the processing thread */
        xos_event_get(&ctx->pipeline_event, &event_state);
        if (event_state & DSP_EVENT_STOP)
        {
            xos_event_clear(&ctx->pipeline_event, DSP_EVENT_STOP);
            /* Send INPUT_OVER to decoder to gracefully shutdown pipeline */
            ret = xaf_comp_process(NULL, ctx->comp, NULL, 0, XAF_INPUT_OVER_FLAG);
            if (ret != XAF_NO_ERR)
            {
                DSP_PRINTF("[DSP_ProcessThread] xaf_comp_process XAF_INPUT_OVER_FLAG failure: %d\r\n", ret);
            }

            break;
        }

        if (cmd_pending)
        {
            ret = xaf_comp_get_status(NULL, ctx->comp, &status, &info[0]);
            if (ret != XAF_NO_ERR)
            {
                DSP_PRINTF("[DSP_ProcessThread] xaf_comp_get_status failure: %d\r\n", ret);
                ctx->file_playing = false;
                DSP_PRINTF("[DSP_ProcessThread] Error, exiting\r\n");
                DSP_SendFileError(ctx);
                return -1;
            }

            cmd_pending = false;
        }

        if (status == XAF_EXEC_DONE)
        {
            DSP_PRINTF("[DSP_ProcessThread] Execution complete - exiting\r\n");
            break;
        }
        else if (status == XAF_NEED_INPUT)
        {
            /* Read input and feed data to pipeline for processing */
            buffer     = (char *)info[0];
            buffer_len = (uint32_t)info[1];

            read_size = ctx->audio_read(ctx, buffer, buffer_len);

            // DSP_PRINTF("[DSP_ProcessThread] Processing %d bytes\r\n", read_size);

            if (read_size > 0)
            {
                ret = xaf_comp_process(NULL, ctx->comp, (void *)buffer, read_size, XAF_INPUT_READY_FLAG);
                if (ret != XAF_NO_ERR)
                {
                    DSP_PRINTF("[DSP_ProcessThread] xaf_comp_process XAF_INPUT_READY_FLAG failure: %d\r\n", ret);
                    return -1;
                }

                cmd_pending = true;
            }
            else
            {
                // DSP_PRINTF("Read size is 0\r\n");
                if ((ctx->eof && ctx->file_playing) || !ctx->file_playing)
                {
                    ret = xaf_comp_process(NULL, ctx->comp, NULL, 0, XAF_INPUT_OVER_FLAG);
                    if (ret != XAF_NO_ERR)
                    {
                        DSP_PRINTF("[DSP_ProcessThread] xaf_comp_process XAF_INPUT_OVER_FLAG failure: %d\r\n", ret);
                        return -1;
                    }

                    cmd_pending = true;
                    // DSP_PRINTF("[DSP_ProcessThread] input over\r\n");
                }
            }
        }
        else if (status == XAF_OUTPUT_READY)
        {
            /* Consume output from pipeline */
            buffer     = (char *)info[0];
            buffer_len = (uint32_t)info[1];

            if (buffer_len == 0)
            {
                /* No output available */
            }
            else
            {
                write_size = ctx->audio_write(ctx, buffer, buffer_len);

                ret = xaf_comp_process(NULL, ctx->comp, buffer, buffer_len, XAF_NEED_OUTPUT_FLAG);
                if (ret != XAF_NO_ERR)
                {
                    DSP_PRINTF("[DSP_ProcessThread] xaf_comp_process XAF_NEED_OUTPUT_FLAG failure: %d\r\n", ret);
                    return -1;
                }

                cmd_pending = true;
            }
        }
        else
        {
            /* Error or nonstandard response. */
            DSP_PRINTF("[DSP_ProcessThread] unexpected status: %d\r\n", status);
            cmd_pending = true;
        }
    }

    ctx->file_playing = false;

    DSP_PRINTF("[DSP_ProcessThread] exiting\r\n");

    return 0;
}
