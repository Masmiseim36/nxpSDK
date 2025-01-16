/*
 * Copyright 2019-2021 NXP
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
#define CPU_NAME "iMXRT1011"

#define APP_WAKEUP_BUTTON_GPIO        GPIO1
#define APP_WAKEUP_BUTTON_GPIO_PIN    0
#define APP_WAKEUP_BUTTON_IRQ         GPIO1_Combined_0_15_IRQn
#define APP_WAKEUP_BUTTON_IRQ_HANDLER GPIO1_Combined_0_15_IRQHandler
#define APP_WAKEUP_BUTTON_NAME        BOARD_USER_BUTTON_NAME

#define APP_WAKEUP_GPT_BASE         GPT2
#define APP_WAKEUP_GPT_IRQn         GPT2_IRQn
#define APP_WAKEUP_GPT_IRQn_HANDLER GPT2_IRQHandler

#define APP_WAKEUP_SNVS_IRQ         SNVS_HP_WRAPPER_IRQn
#define APP_WAKEUP_SNVS_IRQ_HANDLER SNVS_HP_WRAPPER_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
