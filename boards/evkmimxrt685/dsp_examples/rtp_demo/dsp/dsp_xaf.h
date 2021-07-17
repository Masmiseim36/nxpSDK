/*
 * Copyright 2019-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __DSP_XAF_H__
#define __DSP_XAF_H__

#include <stdint.h>
#include <stdio.h>
#include <xtensa/xos.h>
#include "xaf-api.h"
#include "xa_g711_codec_api.h"
#include "xa-mixer-api.h"

#include "dsp_config.h"
#include "message.h"
#include "rtp.h"

typedef struct _dsp_handle dsp_handle_t;

#include "session.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if (INIT_DEBUG_CONSOLE == 1)
#define DSP_PRINTF PRINTF
#else
#define DSP_PRINTF printf
#endif

/* @brief Number of buffers shared between processor cores */
#define SHARED_BUFFER_COUNT \
    ((AUDIO_SHARED_BUFFER_1_SIZE / RTP_PACKET_SIZE) + (AUDIO_SHARED_BUFFER_2_SIZE / RTP_PACKET_SIZE))

/**
 * @brief Enable Packet Loss Concealment (PLC) in G.711 codec library.
 * Set to 1 for enabled, 0 to disable.
 */
#ifndef PLC_ENABLED
#define PLC_ENABLED 1
#endif /* PLC_ENABLED */

#if ((PLC_ENABLED != 0) && (PLC_ENABLED != 1))
#error "((PLC_ENABLED != 0) && (PLC_ENABLED != 1))"
#endif

/**
 * @brief Maximum number of simultaneously played streams
 */
#ifndef MAX_INPUT_STREAMS
#define MAX_INPUT_STREAMS 4
#endif /* MAX_INPUT_STREAMS */

#if MAX_INPUT_STREAMS < 1
#error "MAX_INPUT_STREAMS < 1"
#endif

#if MAX_INPUT_STREAMS > XA_MIXER_MAX_TRACK_NUMBER
#error \
    "MAX_INPUT_STREAMS > XA_MIXER_MAX_TRACK_NUMBER and chaining of multiple mixer components to mix additional streams is currently not implemented."
#endif

/**
 * @brief Minimum number of RTP frames (10 ms long) to receive from each stream before its playback can start
 */
#ifndef BUFFERING_FRAMES
#define BUFFERING_FRAMES 10
#endif /* BUFFERING_FRAMES */

/**
 * @brief Theoretical maximum number of RTP frames held in shared buffers.
 *
 * Practically, each packet will usually hold only 1 or 2 frames, so the number will be lower.
 */
#define MAX_FRAME_LIST_SIZE \
    (SHARED_BUFFER_COUNT * ((RTP_PACKET_SIZE - RTP_HEADER_SIZE) / XA_G711_MAX_NUM_BYTES_PER_FRAME))

/**
 * @brief Maximum number of frames (10 ms long) buffered for a session.
 *
 * The actual number of buffered frames is limited by SHARED_BUFFER_COUNT
 * and by the number of actually running concurrent input streams.
 */
#ifndef FRAME_LIST_SIZE
#define FRAME_LIST_SIZE 100
#endif /* FRAME_LIST_SIZE */

#if BUFFERING_FRAMES > FRAME_LIST_SIZE
#error "BUFFERING_FRAMES > FRAME_LIST_SIZE"
#endif

#if FRAME_LIST_SIZE > MAX_FRAME_LIST_SIZE
#error "FRAME_LIST_SIZE > MAX_FRAME_LIST_SIZE"
#endif

/**
 * @brief Processing state
 */
typedef enum _dsp_state
{
    DspState_Idle,     /*!< Not initialized */
    DspState_Creating, /*!< Audio chain being created */
    DspState_Running,  /*!< Running */
    DspState_Closing,  /*!< Deinitializing */
} dsp_state_t;

/* DSP application context */
struct _dsp_handle
{
    /* State management variable */
    volatile dsp_state_t state;

    /* RPMsg-Lite */
    struct rpmsg_lite_instance *rpmsg;
    struct rpmsg_lite_endpoint *ept;
    XosMsgQueue *rpmsg_queue;

    /* XAF components */
    void *audio_device;
    void *comp_input[MAX_INPUT_STREAMS];
    void *comp_mixer;
    void *comp_src;
    void *comp_renderer;

    /* XAF input component buffers */
    void *inbuf[MAX_INPUT_STREAMS];

    /* Decoder handles */
    xa_codec_handle_t dec_handle[MAX_INPUT_STREAMS];

    /* Decoder scratch memories */
    pVOID dec_scratch[MAX_INPUT_STREAMS];

    /* Pipeline processing thread */
    XosThread playback_thread;

    /* RTP frame timer */
    XosTimer rtp_timer;

    /* Set once per RTP frame period */
    XosSem frame_elapsed;

    /* Playback RTP sessions */
    session_t sessions[MAX_INPUT_STREAMS];
};

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*! @brief Initializes XAF context */
void dsp_xaf_init(dsp_handle_t *dsp);

/*! @brief Creates and starts processing thread */
int dsp_xaf_start_thread(dsp_handle_t *dsp);

/*! @brief Adds packet into queue for playback */
void dsp_xaf_add_packet(dsp_handle_t *dsp, rtp_header_t *packet, uint32_t size);

/*! @brief Releases packet back to CM33 core */
void dsp_xaf_packet_consumed(dsp_handle_t *dsp, rtp_header_t *packet);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __DSP_XAF_H__ */
