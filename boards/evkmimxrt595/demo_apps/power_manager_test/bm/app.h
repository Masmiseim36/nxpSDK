/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_inputmux.h"
#include "pmic_support.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_POWER_NAME                                                   \
    {                                                                    \
        "Sleep", "Deep Sleep", "Deep Power Down", "Full Deep Power Down" \
    }
#define APP_TARGET_POWER_NUM (4U)

/*
 * Note that flexcomm15 is used to configure the external PMIC, and its clock source
 * in this project comes from FFRO, the user need to keep SFRO power up or power down
 * SFRO when the MCU entering low power mode and re-enable it when exiting low power mode.
 */
#define APP_SLEEP_CONSTRAINTS                                                                                   \
    17U, PM_RESC_MAIN_CLK_ON, PM_RESC_SYSXTAL_ON, PM_RESC_LPOSC_ON, PM_RESC_SYSPLLLDO_ON, PM_RESC_SYSPLLANA_ON, PM_RESC_FRO_192M96M_ON, \
        PM_RESC_FLEXSPI0_SRAM_ACTIVE, PM_RESC_SRAM22_256KB_ACTIVE, PM_RESC_SRAM23_256KB_ACTIVE,                 \
        PM_RESC_SRAM24_256KB_ACTIVE, PM_RESC_SRAM25_256KB_ACTIVE, PM_RESC_SRAM26_256KB_ACTIVE,                  \
        PM_RESC_SRAM27_256KB_ACTIVE, PM_RESC_SRAM28_256KB_ACTIVE, PM_RESC_SRAM29_256KB_ACTIVE,                  \
        PM_RESC_SRAM30_256KB_ACTIVE, PM_RESC_SRAM31_256KB_ACTIVE

#define APP_DEEP_SLEEP_CONSTRAINTS                                                                        \
    11U, PM_RESC_FLEXSPI0_SRAM_RETENTION, PM_RESC_SRAM22_256KB_RETENTION, PM_RESC_SRAM23_256KB_RETENTION, \
        PM_RESC_SRAM24_256KB_RETENTION, PM_RESC_SRAM25_256KB_RETENTION, PM_RESC_SRAM26_256KB_RETENTION,   \
        PM_RESC_SRAM27_256KB_RETENTION, PM_RESC_SRAM28_256KB_RETENTION, PM_RESC_SRAM29_256KB_RETENTION,   \
        PM_RESC_SRAM30_256KB_RETENTION, PM_RESC_SRAM31_256KB_RETENTION

#define APP_DEEP_POWER_DOWN_CONSTRAINTS 0U

#define APP_FULL_DEEP_POWER_DOWN_CONSTRAINTS 0U

#define APP_USER_WAKEUP_KEY_GPIO         GPIO
#define APP_USER_WAKEUP_KEY_PORT         0
#define APP_USER_WAKEUP_KEY_PIN          10
#define APP_USER_WAKEUP_KEY_INPUTMUX_SEL kINPUTMUX_GpioPort0Pin10ToPintsel

#define APP_USER_WAKEUP_KEY1_GPIO         GPIO
#define APP_USER_WAKEUP_KEY1_PORT         0
#define APP_USER_WAKEUP_KEY1_PIN          25
#define APP_USER_WAKEUP_KEY1_INPUTMUX_SEL kINPUTMUX_GpioPort0Pin25ToPintsel

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
status_t APP_ControlCallback_notify(pm_event_type_t eventType, uint8_t powerState, void *data);
void BOARD_ConfigPMICModes(pca9420_modecfg_t *cfg, uint32_t num);
void APP_InitWakeupSource(void);
void APP_RegisterNotify(void);
uint32_t APP_GetWakeupTimeout(void);
void APP_SetConstraints(uint8_t powerMode);
void APP_ReleaseConstraints(uint8_t powerMode);
/*${prototype:end}*/

#endif /* _APP_H_ */
