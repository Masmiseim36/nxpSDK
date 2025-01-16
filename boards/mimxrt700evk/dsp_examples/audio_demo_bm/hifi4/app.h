/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "dsp_config.h"
#include "fsl_sai_edma.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_PDM                      PDM
#define DEMO_PDM_CLK_FREQ             CLOCK_GetMicfilClkFreq()
#define DEMO_PDM_FIFO_WATERMARK       (FSL_FEATURE_PDM_FIFO_DEPTH / 2U)
#define DEMO_PDM_QUALITY_MODE         kPDM_QualityModeHigh
#define DEMO_PDM_CHANNEL_GAIN         (kPDM_DfOutputGain5)
#define DEMO_PDM_CIC_OVERSAMPLE_RATE  (16U)
#define DEMO_PDM_ENABLE_CHANNEL_LEFT  (0U)
#define DEMO_PDM_ENABLE_CHANNEL_RIGHT (1U)

/* SAI instance and clock */
#define DEMO_SAI                SAI0
#define DEMO_SAI_CHANNEL        (0)
#define DEMO_SAI_TX_SYNC_MODE   kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE   kSAI_ModeSync
#define DEMO_SAI_MASTER_SLAVE   kSAI_Master
#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth32bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate16KHz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ
#define DEMO_SAI_CLOCK_SOURCE   kSAI_BclkSourceMclkDiv

#define DEMO_DMA              DMA0
#define DEMO_PDM_EDMA_CHANNEL 0
#define DEMO_SAI_EDMA_CHANNEL 1
#define DEMO_PDM_EDMA_SOURCE  kDmaRequestMuxMicfil
#define DEMO_SAI_EDMA_SOURCE  kDmaRequestMuxSai0Tx

/* Get frequency of sai clock */
#define DEMO_SAI_CLK_FREQ CLOCK_GetSaiClkFreq()

/* Get frequency of lpi2c clock */
#define DEMO_I2C_CLK_FREQ 24000000U /* CLOCK_GetLPI2cClkFreq(2) */
#define BOARD_SAI_RXCONFIG(config, mode)

#define DEMO_QUICKACCESS_SECTION_CACHEABLE 1U
#define BUFFER_SIZE                        (1024)
#define BUFFER_NUM                         (4U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_LoopbackFunc(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
