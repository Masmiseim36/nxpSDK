/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_common.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* SAI instance and clock */
#define DEMO_CODEC_CS42448 0

#define DEMO_SAI SAI1
/* enable four sai channel */
#define DEMO_SAI_CHANNEL_MASK (1U | 2U)
#define DEMO_SAI_IRQ          SAI1_IRQn
#define DEMO_SAITxIRQHandler  SAI1_IRQHandler
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE kSAI_ModeSync
#define DEMO_SAI_MCLK_OUTPUT  true

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate48KHz)

#define DEMO_CS42448_I2C_INSTANCE 6
#define DEMO_CODEC_POWER_GPIO     GPIO9
#define DEMO_CODEC_POWER_GPIO_PIN 9
#define DEMO_CODEC_RESET_GPIO     GPIO11
#define DEMO_CODEC_RESET_GPIO_PIN 11

#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (7U)
#define DEMO_SAI_MASTER_SLAVE          kSAI_Master
#define DEMO_AUDIO_MASTER_CLOCK        24576000

/* Select Audio/Video PLL (393.24 MHz) as sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER (0U)

/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ \
    (CLOCK_GetFreq(kCLOCK_AudioPll) / (DEMO_SAI1_CLOCK_SOURCE_DIVIDER + 1U) / (DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER + 1U))

/* DMA */
#define DEMO_DMA             DMA1
#define DEMO_DMAMUX          DMAMUX1
#define DEMO_EDMA_CHANNEL    (0U)
#define DEMO_TX_EDMA_CHANNEL (0U)
#define DEMO_RX_EDMA_CHANNEL (1U)
#define DEMO_SAI_TX_SOURCE   kDmaRequestMuxSai1Tx
#define DEMO_SAI_RX_SOURCE   kDmaRequestMuxSai1Rx

#define BOARD_MASTER_CLOCK_CONFIG()
#define BOARD_SAI_RXCONFIG(config, mode)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
#if defined DEMO_CODEC_CS42448
void BORAD_CodecReset(bool state);
#endif
/*${prototype:end}*/

#endif /* _APP_H_ */
