/*
 * Copyright 2022 NXP
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
#define DEMO_LPTMR_BASE   LPTMR1
#define DEMO_LPTMR_IRQn   LPTMR1_IRQn
#define LPTMR_LED_HANDLER LPTMR1_IRQHandler
/* Get source clock for LPTMR driver */
#define LPTMR_SOURCE_CLOCK CLOCK_GetRootClockFreq(kCLOCK_Root_Lptimer1)
/* Define LPTMR microseconds counts value */
#define LPTMR_USEC_COUNT 1000000U

#define LED_INIT()   USER_LED_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE() USER_LED_TOGGLE()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
