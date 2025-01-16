/*
 * Copyright 2018 NXP
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
#define LED_INIT() USER_LED_INIT(LOGIC_LED_OFF)
#define LED_ON()   USER_LED_ON()
#define LED_OFF()  USER_LED_OFF()

#define DEMO_CMP_BASE             CMP1
#define DEMO_CMP_USER_CHANNEL     0U
#define DEMO_CMP_DAC_CHANNEL      7U
#define DEMO_CMP_IRQ_ID           ACMP1_IRQn
#define DEMO_CMP_IRQ_HANDLER_FUNC ACMP1_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
