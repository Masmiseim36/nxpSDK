/*
 * Copyright 2024 NXP
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
#define WWDT                WWDT0
#define APP_WDT_IRQn        WDT0_IRQn
#define APP_WDT_IRQ_HANDLER WDT0_IRQHandler
#define APP_LED_TOGGLE      LED_BLUE_TOGGLE()
#define APP_LED_INIT        LED_BLUE_INIT(LOGIC_LED_OFF)
#define APP_LED_ON          LED_BLUE_ON()
#define WDT_CLK_FREQ        CLOCK_GetWdtClkFreq(0U)

#define IS_WWDT_RESET (0U != (RSTCTL3->SYSRSTSTAT & RSTCTL3_SYSRSTSTAT_WWDT0_RESET_MASK))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
