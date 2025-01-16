/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_pm_core.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_POWER_NAME                                                                           \
    {                                                                                            \
        "Sleep", "Deep Sleep", "Deep Sleep Retention", "Deep Power Down", "Full Deep Power Down" \
    }
#define APP_TARGET_POWER_NUM (5U)

#define APP_SLEEP_CONSTRAINTS                                                                                     \
    8U, PM_RESC_SENSEP_MAINCLK_ON, PM_RESC_SENSES_MAINCLK_ON, PM_RESC_FRO2_ON, PM_RESC_FRO2_EN, PM_RESC_LPOSC_ON, \
        PM_RESC_SRAM24_128KB_ACTIVE, PM_RESC_SRAM25_128KB_ACTIVE, PM_RESC_SRAM26_512KB_ACTIVE

#define APP_DEEP_SLEEP_CONSTRAINTS \
    3U, PM_RESC_SRAM24_128KB_RETENTION, PM_RESC_SRAM25_128KB_RETENTION, PM_RESC_SRAM26_512KB_RETENTION

#define APP_DSR_CONSTRAINTS APP_DEEP_SLEEP_CONSTRAINTS

#define APP_DEEP_POWER_DOWN_CONSTRAINTS 0U

#define APP_FULL_DEEP_POWER_DOWN_CONSTRAINTS 0U

#define APP_USER_WAKEUP_KEY_GPIO BOARD_SW6_GPIO
#define APP_USER_WAKEUP_KEY_PIN  BOARD_SW6_GPIO_PIN
#define APP_SW_IRQ_HANDLER       GPIO80_IRQHandler
#define APP_SW_IRQN              GPIO80_IRQn
#define APP_SW_RESET_RSTn        kGPIO8_RST_SHIFT_RSTn
#define APP_SW_CLOCK_EN          kCLOCK_Gpio8
#define APP_SW_LP_REQ            kPower_GPIO8_LPREQ

#define APP_RTC             RTC1
#define APP_RTC_IRQN        RTC1_IRQn
#define APP_RTC_IRQ_HANDLER RTC1_IRQHandler

/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitPowerConfig(void);
status_t APP_ControlCallback_notify(pm_event_type_t eventType, uint8_t powerState, void *data);
void APP_InitWakeupSource(void);
void APP_RegisterNotify(void);
uint32_t APP_GetWakeupTimeout(void);
void APP_SetConstraints(uint8_t powerMode);
void APP_ReleaseConstraints(uint8_t powerMode);
void BOARD_RestorePeripheralsAfterDSR(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
