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

#define APP_SLEEP_CONSTRAINTS                                                                                       \
    17U, PM_RESC_COMP_MAINCLK_ON, PM_RESC_SENSES_MAINCLK_ON, PM_RESC_COMN_MAINCLK_ON, PM_RESC_VNCOM_ON,             \
        PM_RESC_FRO0_ON, PM_RESC_FRO0_EN, PM_RESC_LPOSC_ON, PM_RESC_SRAM8_256KB_ACTIVE, PM_RESC_SRAM9_256KB_ACTIVE, \
        PM_RESC_SRAM10_512KB_ACTIVE, PM_RESC_SRAM11_512KB_ACTIVE, PM_RESC_SRAM12_1MB_ACTIVE,                        \
        PM_RESC_SRAM13_1MB_ACTIVE, PM_RESC_SRAM_CPU0_ICACHE_ACTIVE, PM_RESC_SRAM_CPU0_DCACHE_ACTIVE,                \
        PM_RESC_SRAM_XSPI0_ACTIVE, PM_RESC_SRAM_OCOTP_RETENTION

#define APP_DEEP_SLEEP_CONSTRAINTS                                                                    \
    9U, PM_RESC_SRAM8_256KB_RETENTION, PM_RESC_SRAM9_256KB_RETENTION, PM_RESC_SRAM10_512KB_RETENTION, \
        PM_RESC_SRAM11_512KB_RETENTION, PM_RESC_SRAM12_1MB_RETENTION, PM_RESC_SRAM13_1MB_RETENTION,   \
        PM_RESC_SRAM_CPU0_ICACHE_RETENTION, PM_RESC_SRAM_CPU0_DCACHE_RETENTION, PM_RESC_SRAM_OCOTP_RETENTION

/* CPU0 cache is not retented during DSR. */
#define APP_DSR_CONSTRAINTS                                                                           \
    7U, PM_RESC_SRAM8_256KB_RETENTION, PM_RESC_SRAM9_256KB_RETENTION, PM_RESC_SRAM10_512KB_RETENTION, \
        PM_RESC_SRAM11_512KB_RETENTION, PM_RESC_SRAM12_1MB_RETENTION, PM_RESC_SRAM13_1MB_RETENTION,   \
        PM_RESC_SRAM_OCOTP_RETENTION

#define APP_DEEP_POWER_DOWN_CONSTRAINTS 0U

#define APP_FULL_DEEP_POWER_DOWN_CONSTRAINTS 0U

#define BOOT_FLAG                0x1U /* Flag indicates Core1 Boot Up*/
#define APP_USER_WAKEUP_KEY_GPIO BOARD_SW5_GPIO
#define APP_USER_WAKEUP_KEY_PIN  BOARD_SW5_GPIO_PIN
#define APP_USER_WAKEUP_KEY_NAME "SW5"
#define APP_SW_IRQ_HANDLER       GPIO00_IRQHandler
#define APP_SW_IRQN              GPIO00_IRQn
#define APP_SW_RESET_RSTn        kGPIO0_RST_SHIFT_RSTn
#define APP_SW_CLOCK_EN          kCLOCK_Gpio0
#define APP_SW_LP_REQ            kPower_GPIO0_LPREQ

#define APP_RTC             RTC0
#define APP_RTC_IRQN        RTC0_IRQn
#define APP_RTC_IRQ_HANDLER RTC0_IRQHandler

#define APP_MU MU1_MUA

/************************************************************************/
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_RestorePeripheralsAfterDSR(void);
void BOARD_DisablePll(void);
void BOARD_RestorePll(void);
void DEMO_InitDebugConsole(void);
void DEMO_DeinitDebugConsole(void);

status_t APP_ControlCallback_notify(pm_event_type_t eventType, uint8_t powerState, void *data);
void APP_InitWakeupSource(void);
void APP_RegisterNotify(void);
uint32_t APP_GetWakeupTimeout(void);
void APP_SetConstraints(uint8_t powerMode);
void APP_ReleaseConstraints(uint8_t powerMode);
/*${prototype:end}*/

#endif /* _APP_H_ */
