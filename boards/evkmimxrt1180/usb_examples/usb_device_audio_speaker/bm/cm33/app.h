/*
 * Copyright 2022 - 2023 NXP
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
/*${macro:start}*/
#define DEMO_SAI_INSTANCE_INDEX (1U)
#define DEMO_DMA_INDEX          (0U) /* the index is based on the DMA instance array */
#define DEMO_DMA_TX_CHANNEL     (0U)
#define DEMO_SAI_TX_SOURCE      kDma3RequestMuxSai1Tx
#define DEMO_SAI                SAI1
#define DEMO_SAI                SAI1

/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ 12288000U

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
