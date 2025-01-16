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
#define DEMO_SAI_TX_SOURCE      kDma3RequestMuxSai1Tx
#define DEMO_SAI_RX_SOURCE      kDma3RequestMuxSai1Rx
#define DEMO_SAI                SAI1
#define DEMO_DMA_INDEX          (0U) /* the index is based on the DMA instance array */
#define DEMO_DMA_TX_CHANNEL     (0U)
#define DEMO_DMA_RX_CHANNEL     (1U)

/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ 12288000U

#define BOARD_SW_GPIO        BOARD_USER_BUTTON_GPIO
#define BOARD_SW_GPIO_PIN    BOARD_USER_BUTTON_GPIO_PIN
#define BOARD_SW_IRQ         BOARD_USER_BUTTON_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_USER_BUTTON_IRQ_HANDLER
#define BOARD_SW_NAME        BOARD_USER_BUTTON_NAME
#define BOARD_SW_INT_OUTPUT  kRGPIO_InterruptOutput0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
