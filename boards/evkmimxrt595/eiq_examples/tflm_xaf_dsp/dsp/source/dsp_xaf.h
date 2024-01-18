/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __DSP_XAF_H__
#define __DSP_XAF_H__

#include <stdint.h>
#include <xtensa/xos.h>

#include "xaf-api.h"
#include "xa-mixer-api.h"
#include "dsp_config.h"
#include "message.h"
#include "fsl_debug_console.h"

typedef struct _dsp_handle dsp_handle_t;

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DSP_PRINTF PRINTF

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


/**
 * @brief Working mode
 */
typedef enum _dsp_mode
{
    DspMode_Test,      /*!< Not initialized */
    DspMode_Inference, /*!< Audio chain being created */
} dsp_mode_t;

/* DSP application context */
struct _dsp_handle
{
    /* State management variables */
    volatile dsp_state_t state;
    volatile dsp_mode_t mode;

    /* RPMsg-Lite */
    struct rpmsg_lite_instance *rpmsg;
    struct rpmsg_lite_endpoint *ept;
    XosMsgQueue *rpmsg_queue;

    /* XAF components */
    void *audio_device;
    void *comp_capturer;
    void *comp_renderer;
    void *comp_microspeech_fe;
    void *comp_inference;

    /* Pipeline processing thread */
    XosThread processing_thread;

    /* Set once per inference */
    XosSem inference_done;
    int32_t inference_results[3];
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

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __DSP_XAF_H__ */
