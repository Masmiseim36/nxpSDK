/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_wm8962.h"
#include "fsl_sai.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* SAI and I2C instance and clock */
/* SAI and I2C instance and clock */
#define DEMO_CODEC_WM8962
#define DEMO_I2C         LPI2C1
#define DEMO_FLEXIO_BASE FLEXIO2
#define DEMO_SAI         SAI1
/* Select Audio PLL (393.216 MHz) as sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER (3U)
/* Clock divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (15U)
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

/* Select Audio PLL (393.216 MHz) as flexio clock source, need to sync with sai clock, or the codec may not work */
#define DEMO_FLEXIO_CLKSRC_SEL (0U)
/* Clock pre divider for flexio clock source */
#define DEMO_FLEXIO_CLKSRC_PRE_DIV (3U)
/* Clock divider for flexio clock source */
#define DEMO_FLEXIO_CLKSRC_DIV (15U)
#define DEMO_FLEXIO_CLK_FREQ \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_FLEXIO_CLKSRC_DIV + 1U) / (DEMO_FLEXIO_CLKSRC_PRE_DIV + 1U))

#define BCLK_PIN                (8U)
#define FRAME_SYNC_PIN          (7U)
#define TX_DATA_PIN             (6U)
#define RX_DATA_PIN             (5U)
#define FLEXIO_TX_SHIFTER_INDEX 0
#define FLEXIO_RX_SHIFTER_INDEX 2

#define EXAMPLE_DMAMUX        DMAMUX
#define EXAMPLE_DMA           DMA0
#define EXAMPLE_TX_CHANNEL    1U
#define EXAMPLE_RX_CHANNEL    0U
#define EXAMPLE_TX_DMA_SOURCE kDmaRequestMuxFlexIO2Request0Request1
#define EXAMPLE_RX_DMA_SOURCE kDmaRequestMuxFlexIO2Request2Request3
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
