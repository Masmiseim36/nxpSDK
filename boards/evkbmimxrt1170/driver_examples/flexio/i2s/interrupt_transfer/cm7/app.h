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
#include "fsl_sai.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* SAI and I2C instance and clock */
#define DEMO_CODEC_WM8962
#define DEMO_I2C         LPI2C5
#define DEMO_FLEXIO_BASE FLEXIO2
#define DEMO_SAI         SAI1

/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ (CLOCK_GetFreq(kCLOCK_AudioPllOut))

/* Get frequency of lpi2c clock */
#define DEMO_I2C_CLK_FREQ (CLOCK_GetRootClockFreq(kCLOCK_Root_Lpi2c5))

#define DEMO_FLEXIO_CLK_FREQ (CLOCK_GetRootClockFreq(kCLOCK_Root_Flexio2))

#define BCLK_PIN       (13U)
#define FRAME_SYNC_PIN (12U)
#define TX_DATA_PIN    (11U)
#define RX_DATA_PIN    (10U)
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
