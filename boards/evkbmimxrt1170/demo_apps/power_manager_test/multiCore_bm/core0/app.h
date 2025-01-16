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
#define APP_CORE1_BOOT_ADDRESS (0x20200000)

#define APP_TARGET_POWER_NUM (4U)
#define APP_CHN_MU_REG_NUM   0U
#define APP_POWER_NAME                                                                                \
    {                                                                                                 \
        "Setpoint0, CM7 domain WAIT, CM4 domain WAIT", "Setpoint1, CM7 domain STOP, CM4 domain WAIT", \
            "Setpoint10, CM7 domain SUSPEND, CM4 domain SUSPEND",                                     \
            "Setpoint15, CM7 domain SUSPEND, CM4 domain SUSPEND",                                     \
    }

#define APP_SETPOINT0_CONSTRAINTS  1U, PM_RESC_CORE_DOMAIN_WAIT
#define APP_SETPOINT1_CONSTRAINTS  3U, PM_RESC_CORE_DOMAIN_STOP, PM_RESC_SYS_PLL1_ON, PM_RESC_LPSR_DIG_LDO_OFF
#define APP_SETPOINT10_CONSTRAINTS 2U, PM_RESC_CORE_DOMAIN_SUSPEND, PM_RESC_WAKEUP_MIX_ON
#define APP_SETPOINT15_CONSTRAINTS 2U, PM_RESC_CORE_DOMAIN_SUSPEND, PM_RESC_OSC_RC_16M_ON

#define APP_WAKEUP_BUTTON_GPIO        BOARD_USER_BUTTON_GPIO
#define APP_WAKEUP_BUTTON_GPIO_PIN    BOARD_USER_BUTTON_GPIO_PIN
#define APP_WAKEUP_BUTTON_IRQ         BOARD_USER_BUTTON_IRQ
#define APP_WAKEUP_BUTTON_IRQ_HANDLER BOARD_USER_BUTTON_IRQ_HANDLER
#define APP_WAKEUP_BUTTON_NAME        BOARD_USER_BUTTON_NAME
#define APP_WAKEUP_BUTTON_WSID        PM_WSID_GPIO13_COMBINED_0_31_IRQ

#define APP_CHN_MU_REG_NUM 0U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_GetPreviousState(void);
void APP_InitWakeupSource(void);
void APP_RegisterNotify(void);
void APP_SetConstraints(uint8_t powerMode);
void APP_ReleaseConstraints(uint8_t powerMode);
uint32_t APP_GetWakeupTimeout(void);
void APP_BootSecondCore(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
