/*
 * Copyright 2019-2023 NXP
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
#include "xaf-fio-test.h"

#include "fsl_gpio.h"
#include "fsl_sema42.h"

#define AUDIO_BUFFER_FILL_THRESHOLD (16 * 1024)

xa_app_event_handler_fxn_t *g_app_handler_fn;
XosMsgQueue *event_queue = NULL;
int g_execution_abort_flag = 0;
xf_thread_t g_disconnect_thread;
int g_num_comps_in_graph = 0;
xf_thread_t *g_comp_thread;

_XA_API_ERR_MAP error_map_table_api[XA_NUM_API_ERRS]=
{
    {(int)XAF_RTOS_ERR,       "rtos error"},
    {(int)XAF_INVALIDVAL_ERR,    "invalid value"},
    {(int)XAF_ROUTING_ERR,    "routing error"},
    {(int)XAF_INVALIDPTR_ERR,        "invalid pointer"},
    {(int)XAF_API_ERR,          "API error"},
    {(int)XAF_TIMEOUT_ERR,   "message queue Timeout"},
    {(int)XAF_MEMORY_ERR,   "memory error"},
};

/*******************************************************************************
 * Utility Functions
 ******************************************************************************/
void DSP_PRINTF(const char* ptr, ...)
{
    va_list ap;
    SEMA42_Lock(APP_SEMA42, SEMA_PRINTF_NUM, SEMA_CORE_ID_DSP);
    va_start(ap, ptr);
    DbgConsole_Vprintf(ptr, ap);
    va_end(ap);
    SEMA42_Unlock(APP_SEMA42, SEMA_PRINTF_NUM);
}

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
#ifdef FSL_FEATURE_SOC_XCACHE_COUNT
    xthal_dcache_region_writeback(data, size);
#endif
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
#ifdef FSL_FEATURE_SOC_XCACHE_COUNT
    xthal_dcache_region_invalidate(data, size);
#endif
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

/* Read audio data for DSP processing
 *
 * param buffer Input buffer from application
 * param out Output buffer
 * param size Size of data to read in bytes
 * return number of bytes read */
uint32_t DSP_AudioRead(dsp_handle_t *dsp, char *data, uint32_t size)
{
    uint32_t read_size = size;

    if (size + dsp->buffer_in.index > dsp->buffer_in.size)
    {
        read_size = dsp->buffer_in.size - dsp->buffer_in.index;
    }

#ifdef FSL_FEATURE_SOC_XCACHE_COUNT
    xthal_dcache_region_invalidate(&dsp->buffer_in.data[dsp->buffer_in.index], read_size);
#endif
    memcpy(data, &dsp->buffer_in.data[dsp->buffer_in.index], read_size);

    dsp->buffer_in.index += read_size;

    return read_size;
}

/* Consume audio data output from DSP processing
 *
 * param buffer Output buffer
 * param in Input buffer from DSP
 * param size Size of data to write in bytes
 * return number of bytes written */
uint32_t DSP_AudioWrite(dsp_handle_t *dsp, char *data, uint32_t size)
{
    uint32_t write_size = size;

    if (size + dsp->buffer_out.index > dsp->buffer_out.size)
    {
        write_size = dsp->buffer_out.size - dsp->buffer_out.index;
    }

    memcpy(&dsp->buffer_out.data[dsp->buffer_out.index], data, write_size);
#ifdef FSL_FEATURE_SOC_XCACHE_COUNT
    xthal_dcache_region_writeback(&dsp->buffer_out.data[dsp->buffer_out.index], write_size);
#endif

    dsp->buffer_out.index += write_size;

    return write_size;
}

static void DSP_RequestData(dsp_handle_t *dsp)
{
    srtm_message msg = {0};

    msg.head.type         = SRTM_MessageTypeRequest;
    msg.head.majorVersion = SRTM_VERSION_MAJOR;
    msg.head.minorVersion = SRTM_VERSION_MINOR;

    msg.head.category = SRTM_MessageCategory_AUDIO;
    msg.head.command  = SRTM_Command_FileData;

    msg.param[0] = dsp->file_frame_size;

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

    while (!ctx->event_occur)
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

int abort_blocked_threads()
{
    int i;

    /*...set global abort flag immediately */
    g_execution_abort_flag = 1;

    /* Ignore if not enabled in the testbench */
    if ( g_num_comps_in_graph == 0 )
        return -1;

    for( i=0; i<g_num_comps_in_graph; i++ )
    {
        if ( __xf_thread_get_state(&g_comp_thread[i]) == XF_THREAD_STATE_BLOCKED )
        {
            fprintf(stderr, "Aborting thread: %d\n", i);
            __xf_thread_cancel( (xf_thread_t *) &g_comp_thread[i] );
        }
    }

    if ( __xf_thread_get_state(&g_disconnect_thread) == XF_THREAD_STATE_BLOCKED )
    {
        fprintf(stderr, "Aborting disconnect thread\n");
        __xf_thread_cancel( &g_disconnect_thread );
    }

    return 0;
}

#ifndef XA_DISABLE_EVENT
void *event_handler_entry(void *arg)
{
    dsp_handle_t *dsp = (dsp_handle_t *)arg;
    if (dsp == NULL)
        return (void *)1;

    if (!g_app_handler_fn)
        return (void *)0;

    while (1)
    {
        xa_app_process_events(arg);

        if (dsp->g_event_handler_exit)
            break;
    }

    return (void *)0;
}
#endif

int xa_app_initialize_event_queue()
{
    event_queue = malloc(XOS_MSGQ_SIZE(10, sizeof(event_info_t)));
    if (event_queue == NULL)
        return 1;
    xos_msgq_create(event_queue, 10, sizeof(event_info_t), XOS_MSGQ_WAIT_PRIORITY);

    return 0;
}

/* ... just receive the event, copy event data and return immediately */
int xa_app_receive_events_cb(void *comp, uint32_t event_id, void *buf, uint32_t buf_size, uint32_t comp_error_flag)
{
    event_info_t e;
    int32_t ret;

    if (event_queue == NULL)
        return 1;

    e.event_buf = malloc(buf_size);
    if (e.event_buf == NULL)
        return 1;

    e.comp_addr       = (uint32_t)comp;
    e.event_id        = event_id;
    e.comp_error_flag = comp_error_flag;
    e.buf_size        = buf_size;
    memcpy(e.event_buf, buf, buf_size);

    ret = xos_msgq_put_timeout(event_queue, (uint32_t *)&e, xos_msecs_to_cycles(50));
    if ((ret != XOS_OK) && (ret != XOS_ERR_TIMEOUT))
        DSP_PRINTF("xa_app_receive_events_cb error\r\n");
    /*
    if (ret == XOS_OK)
     DSP_PRINTF("Received new event. Event ID: %08x\n", event_id);
     */

    return 0;
}

void xa_app_process_events(void *arg)
{
    event_info_t e;
    int32_t ret;

    if (event_queue == NULL)
        return;

    /* Block until receive event from callback */
    ret = xos_msgq_get_timeout(event_queue, (uint32_t *)&e, xos_msecs_to_cycles(50));
    if (ret == XOS_OK)
    {
        /* Process event */
        if (g_app_handler_fn)
            g_app_handler_fn(&e, arg);

        if (e.event_buf != NULL)
        {
            free(e.event_buf);
            e.event_buf = NULL;
        }
    }
    else if (ret != XOS_ERR_TIMEOUT)
        DSP_PRINTF("xa_app_process_events error\r\n");
}

void xa_app_free_event_queue()
{
    event_info_t e;
    int32_t ret;

    if (event_queue == NULL)
        return;

    while (xos_msgq_empty(event_queue) == 0)
    {
        ret = xos_msgq_get_timeout(event_queue, (uint32_t *)&e, 0);
        if (ret == XOS_OK)
        {
            if (e.event_buf != NULL)
            {
                free(e.event_buf);
                e.event_buf = NULL;
            }
        }
    }

    xos_msgq_delete(event_queue);
    free(event_queue);
    event_queue = NULL;
}

void print_mem_stats(void * p_adev, xaf_adev_config_t adev_config)
{
    /* collect memory stats before closing the device */
    WORD32 meminfo[3 + XAF_MEM_ID_MAX], k, i;
    if(xaf_get_mem_stats(p_adev, adev_config.core, &meminfo[0]))
    {
        FIO_PRINTF(stdout,"Init is incomplete, reliable memory stats are unavailable.\n");
    }
    else
    {
        FIO_PRINTF(stderr,"Local Memory used by DSP Components, in bytes            : %8d of %8d\n", meminfo[0], adev_config.mem_pool[XAF_MEM_ID_COMP].size);
        FIO_PRINTF(stderr,"Shared Memory used by Components and Framework, in bytes : %8d of %8d\n", meminfo[1], adev_config.mem_pool[XAF_MEM_ID_DEV].size);
        FIO_PRINTF(stderr,"Local Memory used by Framework, in bytes                 : %8d\n", meminfo[2]);

        for(k = XAF_MEM_ID_COMP+1, i=5 ; k<XAF_MEM_ID_MAX ; k++, i++)
        {
            if(meminfo[i])
            {
                FIO_PRINTF(stderr,"Local Memory type[%d] used by DSP Components, in bytes    : %8d of %8d\n", k, meminfo[i], adev_config.mem_pool[k].size);
            }
        }
    }
}
