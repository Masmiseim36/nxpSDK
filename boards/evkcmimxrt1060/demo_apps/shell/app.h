/*
 * Copyright 2021 NXP
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
#define LED_NUMBERS  1U
#define LED_1_INIT() USER_LED_INIT(LOGIC_LED_OFF)
#define LED_1_ON()   USER_LED_ON()
#define LED_1_OFF()  USER_LED_OFF()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void Led_Init(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
