/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EIQ_MICRO_CONF_H_
#define _EIQ_MICRO_CONF_H_

#include "fsl_sai.h"
#include "fsl_codec_common.h"
#include "fsl_wm8960.h"
#include "fsl_codec_adapter.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* SAI instance and clock */
#define DEMO_CODEC_WM8960
#define DEMO_SAI SAI1
#define DEMO_SAI_CHANNEL (0)
#define DEMO_SAI_BITWIDTH (kSAI_WordWidth16bits)
#define DEMO_SAI_IRQ SAI1_IRQn

/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER (0U)
/* Clock divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (63U)

/* Get frequency of sai1 clock */
#ifdef CPU_MIMXRT1176DVMAA_cm7
#define DEMO_SAI_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Sai1)
#else
#define DEMO_SAI_CLK_FREQ                                                        \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI1_CLOCK_SOURCE_DIVIDER + 1U) / \
     (DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER + 1U))
#endif

/* I2C instance and clock */

/* Get frequency of sai1 clock */
#ifdef CPU_MIMXRT1176DVMAA_cm7
#define DEMO_I2C LPI2C5
#else
#define DEMO_I2C LPI2C1
#endif

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#ifdef CPU_MIMXRT1176DVMAA_cm7
#define DEMO_I2C_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Lpi2c5)
#else
  #define DEMO_I2C_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (DEMO_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))
#endif

#define OVER_SAMPLE_RATE (384U)
#define BUFFER_NUM (2U)
#define BUFFER_SIZE (1024U)
/* 16Khz */
#define AUDIO_SAMPLES (16U)
#define AUDIO_SAMPLE_SIZE (2U)
/* 16Khz * 2 bytes */
#define AUDIO_NUM (AUDIO_SAMPLES * AUDIO_SAMPLE_SIZE)
/* Two buffers are switching for simultaneously reading and writing. */
#define STRIDE_SIZE (STRIDE_MS * AUDIO_NUM)

/* demo audio sample rate */
#define DEMO_AUDIO_SAMPLE_RATE (kSAI_SampleRate16KHz)
/* demo audio master clock */
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
#define DEMO_AUDIO_MASTER_CLOCK OVER_SAMPLE_RATE *DEMO_AUDIO_SAMPLE_RATE
#else
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
#endif
/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (1U)
/* demo audio bit width */
#define DEMO_AUDIO_BIT_WIDTH kSAI_WordWidth16bits

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

static inline void BOARD_ClearRxInterruptFlags(){
  /* Clear RCSR interrupt flags. */
  DEMO_SAI->RCSR &= ~(I2S_RCSR_FRIE_MASK | I2S_RCSR_FEIE_MASK);
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _EIQ_MICRO_CONF_H_ */
