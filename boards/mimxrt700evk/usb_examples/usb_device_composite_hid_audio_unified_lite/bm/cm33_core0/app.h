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
#define DEMO_SAI_INSTANCE_INDEX (0U)
#define DEMO_DMA_INDEX          (0U) /* the index is based on the DMA instance array */
#define DEMO_DMA_TX_CHANNEL     (0U)
#define DEMO_DMA_RX_CHANNEL     (1U)
#define DEMO_SAI_TX_SOURCE      kDmaRequestMuxSai0Tx
#define DEMO_SAI_RX_SOURCE      kDmaRequestMuxSai0Rx
#define DEMO_SAI                SAI0

/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ 24576000U
#define DEMO_I2C_CLK_FREQ 24000000U /* CLOCK_GetLPI2cClkFreq(2) */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
