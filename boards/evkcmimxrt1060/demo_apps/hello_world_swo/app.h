/*
 * Copyright 2021-2022 NXP
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
#define DEMO_DEBUG_CONSOLE_SWO_PORT     (0U)
#define DEMO_DEBUG_CONSOLE_SWO_BAUDRATE (4000000U)

#define APP_SW_PORT              BOARD_USER_BUTTON_GPIO
#define APP_SW_PIN               BOARD_USER_BUTTON_GPIO_PIN
#define APP_GPIO_INTA_IRQHandler GPIO5_Combined_0_15_IRQHandler
#define APP_SW_IRQ               GPIO5_Combined_0_15_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitDebugConsoleSWO(uint32_t port, uint32_t baudrate);
/*${prototype:end}*/

#endif /* _APP_H_ */
