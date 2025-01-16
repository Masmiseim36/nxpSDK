/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define NORMAL_GPIO_OUT_PORT                GPIO3
#define NORMAL_GPIO_OUT_PIN                 3
#define NORMAL_GPIO_IN_PORT                 GPIO3
#define NORMAL_GPIO_IN_PIN                  4U
#define NORMAL_GPIO_OP_CLK_IN_HZ            CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)
#define NORMAL_GPIO_OUT_PIN_SET_TO_NORMAL() IOMUXC_GPR->GPR42 &= ~(1UL << NORMAL_GPIO_OUT_PIN)
#define NORMAL_GPIO_IN_PIN_SET_TO_NORMAL()  IOMUXC_GPR->GPR42 &= ~(1UL << NORMAL_GPIO_IN_PIN)

#define FAST_GPIO_OUT_PORT              CM7_GPIO3
#define FAST_GPIO_OUT_PIN               3U
#define FAST_GPIO_IN_PORT               CM7_GPIO3
#define FAST_GPIO_IN_PIN                4U
#define FAST_GPIO_OP_CLK_IN_HZ          CLOCK_GetRootClockFreq(kCLOCK_Root_M7)
#define FAST_GPIO_OUT_PIN_SET_TO_FAST() IOMUXC_GPR->GPR42 |= (1UL << FAST_GPIO_OUT_PIN)
#define FAST_GPIO_IN_PIN_SET_TO_FAST()  IOMUXC_GPR->GPR42 |= (1UL << FAST_GPIO_IN_PIN)

#define FAST_GPIO_IRQ_HANDLER     CM7_GPIO2_3_IRQHandler
#define FAST_GPIO_IN_IRQ          CM7_GPIO2_3_IRQn
#define FAST_GPIO_IN_IRQ_PRIORITY 1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
