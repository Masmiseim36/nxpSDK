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
#define APP_MU      MU4_MUB
#define APP_MU_IRQn DSP_INT0_SEL1_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_MU_IRQHandler();
void LED_INIT();
void LED_TOGGLE();
/*${prototype:end}*/

#endif /* _APP_H_ */
