/*
 * Copyright 2018, 2023 NXP
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
/* SAI instance and clock */
#define DEMO_CODEC_VOLUME     100U
#define DEMO_SAI              SAI1
#define DEMO_SAI_CHANNEL      (0)
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE kSAI_ModeSync
#define DEMO_SAI_MASTER_SLAVE kSAI_Master
#define SAI_UserIRQHandler    SAI1_IRQHandler

/* DMA */
#define DEMO_DMA                 DMA3
#define DEMO_TX_EDMA_CHANNEL     (0U)
#define DEMO_RX_EDMA_CHANNEL     (1U)
#define DEMO_SAI_TX_EDMA_CHANNEL kDma3RequestMuxSai1Tx
#define DEMO_SAI_RX_EDMA_CHANNEL kDma3RequestMuxSai1Rx

/* demo audio master clock */
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

/* IRQ */
#define DEMO_SAI_TX_IRQ SAI1_IRQn
#define DEMO_SAI_RX_IRQ SAI1_IRQn

/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Sai1)

#define BOARD_MASTER_CLOCK_CONFIG()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
