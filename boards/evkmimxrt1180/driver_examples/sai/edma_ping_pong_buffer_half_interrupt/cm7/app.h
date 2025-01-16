/*
 * Copyright 2019,2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_wm8962.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* SAI settings */
#define DEMO_CODEC_VOLUME     75
#define DEMO_SAI              SAI1
#define DEMO_SAI_CHANNEL      (0)
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE kSAI_ModeSync
#define DEMO_SAI_MASTER_SLAVE kSAI_Master

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate16KHz)

/* DMA */
#define DEMO_DMA                     DMA3
#define DEMO_EDMA_CHANNEL            (0U)
#define DEMO_SAI_EDMA_CHANNEL        kDma3RequestMuxSai1Tx
#define DEMO_EDMA_HAS_CHANNEL_CONFIG 1

/* demo audio master clock */
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

/* Clock settings */
#define DEMO_SAI_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Sai1)
#define BOARD_MASTER_CLOCK_CONFIG()

/* IRQ */
#define DEMO_SAITxIRQHandler SAI1_IRQHandler
#define DEMO_SAI_IRQ         SAI1_IRQn

/* Misc settings */
#define BOARD_SAI_RXCONFIG(config, mode)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
