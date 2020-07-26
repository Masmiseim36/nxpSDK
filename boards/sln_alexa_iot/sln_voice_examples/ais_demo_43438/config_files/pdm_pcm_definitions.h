/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef _PDM_PCM_DEFINITIONS_H_
#define _PDM_PCM_DEFINITIONS_H_

/*******************************************************************************
 * App Config Definitions
 ******************************************************************************/

#define EDMA_TCD_COUNT (2U)

#define USE_TFA9894 (1U)
#define USE_16BIT_PCM (1U)

#define USE_SAI1_RX_DATA0_MIC (1U)
#define USE_SAI1_RX_DATA1_MIC (1U)
#define USE_SAI1_RX_DATA2_MIC (0U) // microphone to be connected on the extension connector J4.4 (data) & J4.3 (clock)
#define USE_SAI1_RX_DATA3_MIC (0U) // microphone to be connected on the extension connector J4.5 (data) & J4.3 (clock)

#define USE_SAI1_CH_MASK ((USE_SAI1_RX_DATA0_MIC << 0U) | (USE_SAI1_RX_DATA1_MIC << 1U) | (USE_SAI1_RX_DATA2_MIC << 2U) | (USE_SAI1_RX_DATA3_MIC << 3U))

#define USE_SAI2_MIC (1U)
#define USE_SAI2_CH_MASK (USE_SAI2_MIC << 0U)

#define PDM_MIC_COUNT (USE_SAI1_RX_DATA0_MIC + USE_SAI1_RX_DATA1_MIC + USE_SAI1_RX_DATA2_MIC + USE_SAI1_RX_DATA3_MIC + USE_SAI2_MIC)
#define SAI1_CH_COUNT (USE_SAI1_RX_DATA0_MIC + USE_SAI1_RX_DATA1_MIC + USE_SAI1_RX_DATA2_MIC + USE_SAI1_RX_DATA3_MIC)
#define SAI_USE_COUNT ((USE_SAI2_MIC) + ((SAI1_CH_COUNT > 0) ? 1 : 0))

/*******************************************************************************
 * PCM Stream Sample Definitions
 ******************************************************************************/
#define PCM_SINGLE_CH_SMPL_COUNT (160U)

#define BUFFER_PCM_LLRR (0U)
#define BUFFER_PCM_LRLR (1U)
#define PCM_FORMAT BUFFER_PCM_LLRR

#define PCM_SAMPLE_SIZE_BYTES (2U)
#define PCM_SAMPLE_RATE_HZ (16000U)
#define PCM_SAMPLE_COUNT (PCM_SINGLE_CH_SMPL_COUNT * PDM_MIC_COUNT)
#define PCM_BUFFER_COUNT (EDMA_TCD_COUNT)
#define PCM_STREAM_MID_POINT (PCM_BUFFER_COUNT / 2)

#define PCM_AMP_SAMPLE_RATE_HZ (48000U)
#define PCM_AMP_SAMPLE_COUNT (PCM_SINGLE_CH_SMPL_COUNT * PCM_AMP_SAMPLE_RATE_HZ / PCM_SAMPLE_RATE_HZ)

typedef int16_t pcmPingPong_t[PCM_BUFFER_COUNT][PCM_SAMPLE_COUNT];

/*******************************************************************************
 * PDM Stream Sample Definitions
 ******************************************************************************/

#define PDM_BUFFER_COUNT (EDMA_TCD_COUNT)
#define PDM_SAMPLE_COUNT (PCM_SINGLE_CH_SMPL_COUNT * 4)
#define PDM_CAPTURE_SIZE_BYTES (4U)
#define PDM_SAMPLE_RATE_HZ (2048000U)

typedef int16_t pdmPingPong_t[PDM_BUFFER_COUNT][PDM_SAMPLE_COUNT];

typedef enum __pcm_event
{
    PCM_PING = 0U,
    PCM_PONG = 1U,
} pcm_event_t;

enum _event_cases
{
    DMA0_PING        = (1U << 0U),
    DMA1_PING        = (1U << 1U),
    DMA0_PONG        = (1U << 2U),
    DMA1_PONG        = (1U << 3U),
    REFERENCE_SIGNAL = (1U << 4U),
};

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

/*! @} */

#endif /* _PDM_PCM_DEFINITIONS_H_ */

/*******************************************************************************
 * API
 ******************************************************************************/
