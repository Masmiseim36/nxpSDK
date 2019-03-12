/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _POWER_MODE_SWITCH_H_
#define _POWER_MODE_SWITCH_H_

#include "fsl_common.h"
#include "lpm.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define APP_WAKEUP_BUTTON_GPIO BOARD_USER_BUTTON_GPIO
#define APP_WAKEUP_BUTTON_GPIO_PIN BOARD_USER_BUTTON_GPIO_PIN
#define APP_WAKEUP_BUTTON_IRQ BOARD_USER_BUTTON_IRQ
#define APP_WAKEUP_BUTTON_IRQ_HANDLER BOARD_USER_BUTTON_IRQ_HANDLER
#define APP_WAKEUP_BUTTON_NAME BOARD_USER_BUTTON_NAME

#define APP_WAKEUP_GPT_BASE GPT2
#define APP_WAKEUP_GPT_IRQn GPT2_IRQn
#define APP_WAKEUP_GPT_IRQn_HANDLER GPT2_IRQHandler

typedef enum _app_wakeup_source
{
    kAPP_WakeupSourceGPT, /*!< Wakeup by PIT.        */
    kAPP_WakeupSourcePin, /*!< Wakeup by external pin. */
} app_wakeup_source_t;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

extern int32_t is_suspend_reset;

void APP_PowerPreSwitchHook(lpm_power_mode_t targetMode);
void APP_PowerPostSwitchHook(lpm_power_mode_t targetMode);
lpm_power_mode_t APP_GetLPMPowerMode(void);
void BOARD_SetLPClockGate(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _POWER_MODE_SWITCH_H_ */
