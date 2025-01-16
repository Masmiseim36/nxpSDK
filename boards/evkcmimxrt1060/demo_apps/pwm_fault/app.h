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
/* The PWM base address */
#define DEMO_PWM_BASEADDR                  PWM1
#define DEMO_PWM_SUBMODULE                 kPWM_Module_0
#define DEMO_PWM_FAULT_INPUT_PIN           kPWM_Fault_0
#define DEMO_PWM_CONTROL_SUBMODULE         kPWM_Control_Module_0
#define DEMO_PWM_CHANNEL                   kPWM_PwmA
#define DEMO_PWM_DELAY_VAL                 0x0FFFU
#define DEMO_PWM_CHANNEL_LOCATION_ON_BOARD "J24-6"

#define DEMO_CMP_BASE                        CMP1
#define DEMO_CMP_USER_CHANNEL                0U
#define DEMO_CMP_DAC_CHANNEL                 7U
#define DEMO_CMP_INPUT_PIN_LOCATION_ON_BOARD "J23-5"

#define DEMO_DEADTIME_VAL 650U
#define PWM_SRC_CLK_FREQ  CLOCK_GetFreq(kCLOCK_IpgClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
