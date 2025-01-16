/*
 * Copyright 2021 NXP
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
#define APP_BOARD_TEST_LED_PORT BOARD_LED_BLUE_GPIO_PORT
#define APP_BOARD_TEST_LED_PIN  BOARD_LED_BLUE_GPIO_PIN
#define APP_SW_PORT             BOARD_SW2_GPIO_PORT
#define APP_SW_PIN              BOARD_SW2_GPIO_PIN
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
