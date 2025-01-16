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
#include "fsl_wm8960.h"
#include "fsl_sai.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* SAI and I2C instance and clock */
#define DEMO_CODEC_WM8960
#define DEMO_I2C         LPI2C1
#define DEMO_FLEXIO_BASE FLEXIO3
#define DEMO_SAI         SAI1
/* Select Audio PLL (786.43 MHz) as sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER (3U)
/* Clock divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (31U)
/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ                                                            \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER + 1U) / \
     (DEMO_SAI1_CLOCK_SOURCE_DIVIDER + 1U))

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define DEMO_I2C_CLK_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (DEMO_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

/* Select Audio PLL (786.43 MHz) as flexio clock source, need to sync with sai clock, or the codec may not work */
#define DEMO_FLEXIO_CLKSRC_SEL (0U)
/* Clock pre divider for flexio clock source */
#define DEMO_FLEXIO_CLKSRC_PRE_DIV (7U)
/* Clock divider for flexio clock source */
#define DEMO_FLEXIO_CLKSRC_DIV (7U)
#define DEMO_FLEXIO_CLK_FREQ \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_FLEXIO_CLKSRC_PRE_DIV + 1U) / (DEMO_FLEXIO_CLKSRC_DIV + 1U))

#define BCLK_PIN       (7U)
#define FRAME_SYNC_PIN (6U)
#define TX_DATA_PIN    (5U)
#define RX_DATA_PIN    (4U)
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
