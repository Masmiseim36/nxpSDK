/*
 * Copyright 2019-2021 NXP
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
#if (MULTICHANNEL_EXAMPLE == 1)
/* DSP_PRINTF is omitted in multi-channel examples because printf causes output audio distortion due to the relatively
 * long print time. */
#define DSP_PRINTF(s, ...)
#else
#define DSP_PRINTF printf
#endif
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DSP_EVENT_STOP (1 << 0)

typedef struct _dsp_handle_t
{
    struct rpmsg_lite_instance *rpmsg;
    struct rpmsg_lite_endpoint *ept;
    rpmsg_queue_handle rpmsg_queue;

    void *audio_device;
    void *comp_codec;
    void *comp_gain;
    void *comp_client_proxy;
    void *comp_renderer;
    void *comp;
    XosThread process_thread;
    XosThread buffer_thread;
    XosThread cleanup_thread;
    XosEvent pipeline_event;

    /* Audio buffer read/write function pointers for processing thread */
    uint32_t (*audio_read)(struct _dsp_handle_t *dsp, char *data, uint32_t size);
    uint32_t (*audio_write)(struct _dsp_handle_t *dsp, char *data, uint32_t size);

    /* File playback ring buffer variables */
    ringbuf_t *audioBuffer;
    XosMutex audioMutex;
    XosMutex rpmsgMutex;

    /* File playback state management variables */
    volatile bool eof;
    volatile bool ipc_waiting;
    volatile bool file_playing;

    XosCond ipc_waiting_cond;
    XosCond read_waiting_cond;
} dsp_handle_t;

/*******************************************************************************
 * API Function Prototypes
 ******************************************************************************/
void *DSP_Malloc(int32_t size, int32_t id);
void DSP_Free(void *ptr, int32_t id);

uint32_t DSP_AudioReadRing(dsp_handle_t *dsp, char *data, uint32_t size);
uint32_t DSP_AudioWriteRing(dsp_handle_t *dsp, char *data, uint32_t size);

int DSP_ProcessThread(void *arg, int wake_value);
int DSP_BufferThread(void *arg, int wake_value);
void DSP_SendFileEnd(dsp_handle_t *dsp);
void DSP_SendFileError(dsp_handle_t *dsp);

#endif /* __SRTM_UTILS_H__ */
