/*
 * Copyright 2021 NXP
 * All rights reserved.
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
/* @TEST_ANCHOR */

/*${macro:start}*/
/* SAI instance and clock */
#ifndef DEMO_CODEC_WM8962
#define DEMO_CODEC_WM8962 1
#endif
#ifndef DEMO_CODEC_CS42448
#define DEMO_CODEC_CS42448 0
#endif
#if DEMO_CODEC_WM8962 && DEMO_CODEC_CS42448
#error "Duplicate codec defined"
#endif
#define DEMO_SAI             SAI1
#define DEMO_SAI_CHANNEL     (0)
#define DEMO_SAI_IRQ         SAI1_IRQn
#define DEMO_SAITxIRQHandler SAI1_IRQHandler

#define DEMO_SAI_MASTER_SLAVE kSAI_Master

#define DEMO_AUDIO_DATA_CHANNEL (2U)
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth16bits
#define DEMO_AUDIO_SAMPLE_RATE  (kSAI_SampleRate44100Hz)
#define DEMO_AUDIO_MASTER_CLOCK DEMO_SAI_CLK_FREQ

/* IRQ */
#define DEMO_SAI_TX_IRQ SAI1_IRQn
#define DEMO_SAI_RX_IRQ SAI1_IRQn

/* DMA */
#define DEMO_DMA             DMA0
#define DEMO_DMAMUX          DMAMUX
#define DEMO_TX_EDMA_CHANNEL (0U)
#define DEMO_RX_EDMA_CHANNEL (1U)
#define DEMO_SAI_TX_SOURCE   kDmaRequestMuxSai1Tx
#define DEMO_SAI_RX_SOURCE   kDmaRequestMuxSai1Rx

#if DEMO_CODEC_CS42448
#define DEMO_CS42448_I2C_INSTANCE      3
#define DEMO_CODEC_POWER_GPIO          GPIO1
#define DEMO_CODEC_POWER_GPIO_PIN      0
#define DEMO_CODEC_RESET_GPIO          GPIO1
#define DEMO_CODEC_RESET_GPIO_PIN      2
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (11U)
#define DEMO_SAI_MASTER_SLAVE          kSAI_Master
#else
#define DEMO_SAI_TX_SYNC_MODE          kSAI_ModeAsync
#define DEMO_SAI_RX_SYNC_MODE          kSAI_ModeSync
#define DEMO_WM8962_I2C_INSTANCE       1
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (3U)
#define DEMO_SAI_MASTER_SLAVE          kSAI_Master
#endif

/* Select Audio/Video PLL (180.6 MHz) as sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER (3U)
/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ                                                        \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI1_CLOCK_SOURCE_DIVIDER + 1U) / \
     (DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER + 1U))

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define DEMO_I2C_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (DEMO_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

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
