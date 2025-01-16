/*
 * Copyright 2024 NXP
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
#define DEMO_PDM_CLK_FREQ            CLOCK_GetMicfilClkFreq()
#define DEMO_PDM_FIFO_WATERMARK      (FSL_FEATURE_PDM_FIFO_DEPTH / 2U)
#define DEMO_PDM_QUALITY_MODE        kPDM_QualityModeHigh
#define DEMO_PDM_CHANNEL_GAIN        (kPDM_DfOutputGain7)
#define DEMO_PDM_CIC_OVERSAMPLE_RATE (16U)
#define DEMO_PDM_ENABLE_CHANNEL_0    (0U)
#define DEMO_PDM_ENABLE_CHANNEL_1    (1U)
#define DEMO_PDM_ENABLE_CHANNEL_2    (2U)
#define DEMO_PDM_ENABLE_CHANNEL_3    (3U)
#define DEMO_PDM_ENABLE_CHANNEL_4    (4U)
#define DEMO_PDM_ENABLE_CHANNEL_5    (5U)
#define DEMO_PDM_ENABLE_CHANNEL_6    (6U)
#define DEMO_PDM_ENABLE_CHANNEL_7    (7U)

/* SAI instance and clock */
#define DEMO_SAI                SAI0
#define DEMO_SAI_CHANNEL        (0)
#define DEMO_SAI_TX_SYNC_MODE   kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE   kSAI_ModeSync
#define DEMO_SAI_MASTER_SLAVE   kSAI_Master
#define DEMO_AUDIO_DATA_CHANNEL (8U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth32bits
/* Note the DMIC has clock limitation, the PDM_CLK can't exceed it's max clock. */
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate24KHz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
#define DEMO_SAI_CLOCK_SOURCE   kSAI_BclkSourceMclkDiv

/* Get frequency of sai clock */
#define DEMO_SAI_CLK_FREQ CLOCK_GetSaiClkFreq()

#define DEMO_EDMA               DMA0
#define DEMO_PDM_EDMA_CHANNEL_0 0U
#define DEMO_SAI_EDMA_CHANNEL   1U
#define DEMO_PDM_EDMA_SOURCE    kDmaRequestMuxMicfil
#define DEMO_SAI_EDMA_SOURCE    kDmaRequestMuxSai0Tx

#define DEMO_CS42448_I2C_INSTANCE 2U
#define DEMO_CODEC_POWER_GPIO     GPIO0
#define DEMO_CODEC_POWER_GPIO_PIN 19
#define DEMO_CODEC_RESET_GPIO     GPIO0
#define DEMO_CODEC_RESET_GPIO_PIN 10

/* Get frequency of lpi2c clock */
#define DEMO_I2C_CLK_FREQ 24000000U

#define DEMO_QUICKACCESS_SECTION_CACHEABLE 1U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_MasterClockConfig(void);
void BORAD_CodecReset(bool state);
/*${prototype:end}*/

#endif /* _APP_H_ */
