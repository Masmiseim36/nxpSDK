/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_wm8962.h"
#include "fsl_edma.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* SAI instance and clock */
#define DEMO_CODEC_WM8960
#define DEMO_SAI              SAI1
#define DEMO_SAI_IRQ          SAI1_IRQn
#define SAI_TxIRQHandler      SAI1_IRQHandler
#define DEMO_SAI_CHANNEL      0
#define DEMO_ASRC_IN_CHANNEL  0
#define DEMO_ASRC_OUT_CHANNEL 1
// #define DEMO_SAI_TX_CHANNEL   0
/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */
// #define DEMO_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
// #define DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER (1U)
/* Clock divider for sai1 clock source */
// #define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (63U)
/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ                                                        \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI1_CLOCK_SOURCE_DIVIDER + 1U) / \
     (DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER + 1U))

#define DEMO_AUDIO_MASTER_CLOCK          (24576000U / 2U)
#define DEMO_ASRC_OUTPUT_SOURCE_CLOCK_HZ (16 * 48000 * 2)
#define DEMO_ASRC_PERIPHERAL_CLOCK       200000000U
#define DEMO_ASRC                        ASRC
#define DEMO_ASRC_CHANNEL_PAIR           kASRC_ChannelPairA

#define DEMO_AUDIO_SAMPLE_RATE_IN  (kSAI_SampleRate48KHz)
#define DEMO_AUDIO_SAMPLE_RATE_OUT (kSAI_SampleRate32KHz)

/* I2C instance and clock */
// #define DEMO_I2C LPI2C1

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
// #define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
// #define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
// #define DEMO_I2C_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (DEMO_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

/* DMA */
#define DEMO_ASRC_DMA        DMA4
#define DEMO_SAI_DMA         DMA3
#define DEMO_SAI_DMA_CHANNEL kDma3RequestMuxSai1Tx

// #define EXAMPLE_CHANNEL (0U)

#define DEMO_CODEC_VOLUME            (80U)
#define DEMO_EDMA_HAS_CHANNEL_CONFIG 1
#define DEMO_ASRC_IN_EDMA_CHANNEL    kDma4RequestMuxASRCRequest1
#define DEMO_ASRC_OUT_EDMA_CHANNEL   kDma4RequestMuxASRCRequest4

#define BOARD_SAI_EDMA_CONFIG(config)  Board_SaiEdmaConfig(config)
#define BOARD_ASRC_EDMA_CONFIG(config) Board_AsrcEdmaConfig(config)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void Board_SaiEdmaConfig(edma_config_t *config);
void Board_AsrcEdmaConfig(edma_config_t *config);
/*${prototype:end}*/

#endif /* _APP_H_ */
