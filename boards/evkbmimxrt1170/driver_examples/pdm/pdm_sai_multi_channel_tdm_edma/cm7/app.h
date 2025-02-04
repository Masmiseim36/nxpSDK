/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_PDM                     PDM
#define DEMO_SAI                     SAI1
#define DEMO_SAI_CLK_FREQ            24576000
#define DEMO_SAI_CLOCK_SOURCE        (kSAI_BclkSourceMclkDiv)
#define DEMO_PDM_CLK_FREQ            49152000
#define DEMO_PDM_FIFO_WATERMARK      (4)
#define DEMO_PDM_QUALITY_MODE        kPDM_QualityModeHigh
#define DEMO_PDM_CIC_OVERSAMPLE_RATE (0U)
#define DEMO_PDM_ENABLE_CHANNEL_0    (0U)
#define DEMO_PDM_ENABLE_CHANNEL_1    (1U)
#define DEMO_PDM_ENABLE_CHANNEL_2    (2U)
#define DEMO_PDM_ENABLE_CHANNEL_3    (3U)
#define DEMO_PDM_ENABLE_CHANNEL_4    (4U)
#define DEMO_PDM_ENABLE_CHANNEL_5    (5U)
#define DEMO_PDM_ENABLE_CHANNEL_6    (6U)
#define DEMO_PDM_ENABLE_CHANNEL_7    (7U)
#define DEMO_PDM_SAMPLE_CLOCK_RATE   (6144000U) /* 6.144MHZ */
#define DEMO_PDM_CHANNEL_GAIN        kPDM_DfOutputGain7

/* demo audio sample rate */
#define DEMO_AUDIO_SAMPLE_RATE (kSAI_SampleRate48KHz)
/* demo audio master clock */
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (8U)
/* demo audio bit width */
#define DEMO_AUDIO_BIT_WIDTH kSAI_WordWidth32bits

#define DEMO_CS42448_I2C_INSTANCE 6
#define DEMO_CODEC_POWER_GPIO     GPIO9
#define DEMO_CODEC_POWER_GPIO_PIN 9
#define DEMO_CODEC_RESET_GPIO     GPIO11
#define DEMO_CODEC_RESET_GPIO_PIN 11

#define DEMO_EDMA               DMA0
#define DEMO_DMAMUX             DMAMUX0
#define DEMO_PDM_EDMA_CHANNEL_0 0
#define DEMO_PDM_EDMA_CHANNEL_1 1
#define DEMO_SAI_EDMA_CHANNEL   2
#define DEMO_PDM_REQUEST_SOURCE kDmaRequestMuxPdm
#define DEMO_SAI_REQUEST_SOURCE kDmaRequestMuxSai1Tx
#define BOARD_MasterClockConfig()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitDebugConsole(void);
void BORAD_CodecReset(bool state);
/*${prototype:end}*/

#endif /* _APP_H_ */
