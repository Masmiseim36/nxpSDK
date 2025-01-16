/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define WWDT                WWDT0
#define APP_LED_INIT        LED_RED_INIT(LOGIC_LED_ON)
#define APP_LED_ON          LED_RED_ON()
#define APP_LED_TOGGLE      LED_RED_TOGGLE()
#define APP_WDT_IRQn        WDT0_IRQn
#define APP_WDT_IRQ_HANDLER WDT0_IRQHandler
#define WDT_CLK_FREQ        CLOCK_GetWdtClkFreq(1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
