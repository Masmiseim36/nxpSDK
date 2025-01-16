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
#define DEMO_LPIT_BASE       LPIT1
#define DEMO_LPIT_IRQn       LPIT1_IRQn
#define DEMO_LPIT_IRQHandler LPIT1_IRQHandler
/* Get source clock for LPIT driver */
#define LPIT_SOURCECLOCK CLOCK_GetRootClockFreq(kCLOCK_Root_Bus_Aon)

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
