/*
 * Copyright 2023 NXP
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
#define APP_MU            MU1_MUB
#define APP_MU_IRQHandler MU1_B_IRQHandler
#define APP_MU_IRQn       MU1_B_IRQn
#define LED_INIT()        LED_RED_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE()      LED_RED_TOGGLE()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
