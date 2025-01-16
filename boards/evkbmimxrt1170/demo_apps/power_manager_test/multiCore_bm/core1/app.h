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
#define APP_CHN_MU_REG_NUM      (0U)
#define APP_MU_GENINT_ENABLE    (kMU_GenInt0InterruptEnable)
#define APP_MU_RXFULLINT_ENABLE (kMU_Rx0FullInterruptEnable)

#define APP_SETPOINT0_CONSTRAINTS  1U, PM_RESC_CORE_DOMAIN_WAIT
#define APP_SETPOINT1_CONSTRAINTS  3U, PM_RESC_CORE_DOMAIN_WAIT, PM_RESC_SYS_PLL1_ON, PM_RESC_LPSR_DIG_LDO_OFF
#define APP_SETPOINT10_CONSTRAINTS 2U, PM_RESC_CORE_DOMAIN_SUSPEND, PM_RESC_WAKEUP_MIX_ON
#define APP_SETPOINT15_CONSTRAINTS 2U, PM_RESC_CORE_DOMAIN_SUSPEND, PM_RESC_OSC_RC_16M_ON
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_InitWakeupSource(void);
void APP_RegisterNotify(void);
void APP_SetConstraints(uint8_t powerMode);
void APP_ReleaseConstraints(uint8_t powerMode);
uint32_t APP_GetWakeupTimeout(void);
/*${prototype:end}*/
#endif /* _APP_H_ */
