/*
 * Copyright 2019-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SRTM_UTILS_H__
#define __SRTM_UTILS_H__

#include <stdint.h>
#include <xtensa/xos.h>

#include "ringbuffer.h"
#include "rpmsg_queue.h"

#if (INIT_DEBUG_CONSOLE == 1)
#define DSP_PRINTF PRINTF
#else
#define DSP_PRINTF printf
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DSP_EVENT_STOP (1 << 0)

typedef struct _dsp_buffer_t
{
    char *data;
    uint32_t size;
    uint32_t index;
} dsp_buffer_t;

typedef struct _dsp_handle_t
{
    struct rpmsg_lite_instance *rpmsg;
    struct rpmsg_lite_endpoint *ept;
    rpmsg_queue_handle rpmsg_queue;

    void *audio_device;
    void *comp_codec;
    void *comp_src;
    void *comp_renderer;
    void *comp_client_proxy;
    void *comp;
    XosThread dec_thread;
    XosThread buffer_thread;
    XosThread cleanup_thread;
    XosEvent pipeline_event;

#ifndef XA_DISABLE_EVENT
    XosThread event_thread;
    bool g_event_handler_exit;
#endif

    /* Audio buffer read/write function pointers for processing thread */
    uint32_t (*audio_read)(struct _dsp_handle_t *dsp, char *data, uint32_t size);
    uint32_t (*audio_write)(struct _dsp_handle_t *dsp, char *data, uint32_t size);

    /* Memory buffer playback variables */
    dsp_buffer_t buffer_in;
    dsp_buffer_t buffer_in2;
    dsp_buffer_t buffer_out;

    /* File playback ring buffer variables */
    ringbuf_t *audioBuffer;
    XosMutex audioMutex;
    XosMutex rpmsgMutex;

    /* File playback state management variables */
    volatile bool eof;
    volatile bool ipc_waiting;
    volatile bool file_playing;
    volatile bool event_occur;
    XosCond ipc_waiting_cond;
    XosCond read_waiting_cond;
    uint32_t file_frame_size;
    uint8_t channel_num;

} dsp_handle_t;

/*******************************************************************************
 * API Function Prototypes
 ******************************************************************************/
void *DSP_Malloc(int32_t size, int32_t id);
void DSP_Free(void *ptr, int32_t id);

uint32_t DSP_AudioRead(dsp_handle_t *dsp, char *data, uint32_t size);
uint32_t DSP_AudioWrite(dsp_handle_t *dsp, char *data, uint32_t size);
uint32_t DSP_AudioReadRing(dsp_handle_t *dsp, char *data, uint32_t size);
uint32_t DSP_AudioWriteRing(dsp_handle_t *dsp, char *data, uint32_t size);

int DSP_ProcessThread(void *arg, int wake_value);
int DSP_BufferThread(void *arg, int wake_value);
void DSP_SendFileEnd(dsp_handle_t *dsp);
void DSP_SendFileError(dsp_handle_t *dsp);

void *event_handler_entry(void *arg);

int xa_app_initialize_event_queue(void);
int xa_app_receive_events_cb(void *comp, uint32_t event_id, void *buf, uint32_t buf_size, uint32_t comp_error_flag);
void xa_app_free_event_queue(void);

#endif /* __SRTM_UTILS_H__ */
