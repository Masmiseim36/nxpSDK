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
#define BOARD_LED_GPIO       BOARD_LED_BLUE_GPIO
#define BOARD_LED_GPIO_PIN   BOARD_LED_BLUE_GPIO_PIN
#define BOARD_SW_GPIO        GPIO0
#define BOARD_SW_GPIO_PIN    9U
#define BOARD_SW_IRQ         GPIO00_IRQn
#define BOARD_SW_IRQ_HANDLER GPIO00_IRQHandler
#define BOARD_SW_NAME        "SW5"
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
