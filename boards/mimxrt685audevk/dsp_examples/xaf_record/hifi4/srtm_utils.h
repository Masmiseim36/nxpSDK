/*
 * Copyright 2019-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SRTM_UTILS_H__
#define __SRTM_UTILS_H__

#include <stdint.h>
#include <xtensa/xos.h>

#include "rpmsg_queue.h"

#ifdef MIMXRT798S_hifi4_SERIES
#define APP_SEMA42 SEMA42_4
#define SEMA_CORE_ID_DSP  4
#else
#define APP_SEMA42 SEMA42
#define SEMA_CORE_ID_DSP  3
#endif
#define SEMA_PRINTF_NUM	  0
#define SEMA_STARTUP_NUM  1
void DSP_PRINTF(const char* ptr, ...);

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
    XosMutex rpmsgMutex;

    /* Audio buffer read/write function pointers for processing thread */
    uint32_t (*audio_read)(struct _dsp_handle_t *dsp, char *data, uint32_t size);
    uint32_t (*audio_write)(struct _dsp_handle_t *dsp, char *data, uint32_t size);

    /* Memory buffer playback variables */
    dsp_buffer_t buffer_in;
    dsp_buffer_t buffer_in2;
    dsp_buffer_t buffer_out;

#if (XA_VIT_PRE_PROC == 1)
    /* VIT variables */
    const unsigned char *VITModelCM_33;
    uint32_t size_of_VIT_model;
#endif

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

#endif /* __SRTM_UTILS_H__ */
