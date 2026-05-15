/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_pwm.h"
#include "fsl_xbar.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_FLEXIO_BASE         FLEXIO1

#define FLEXIO_BISS_DATA_PIN      0U
#define FLEXIO_BISS_CLOCK_PIN     2U
#define FLEXIO_BISS_BASE_SHIFTER  0U
#define FLEXIO_BISS_BASE_TIMER    0U

#define DEMO_FLEXIO_CLOCK         (kCLOCK_Root_Flexio1)
#define FLEXIO_CLOCK_FREQUENCY    (CLOCK_GetRootClockFreq(DEMO_FLEXIO_CLOCK))
#define BOARD_FLEXIO_IRQHandler   FLEXIO1_IRQHandler
#define BOARD_FLEXIO_IRQn         FLEXIO1_IRQn

#define BOARD_PWM_INSTANCE              PWM1
#define PWM_SRC_CLK_FREQ                CLOCK_GetRootClockFreq(kCLOCK_Root_Bus_Aon)
#define DEMO_PWM_CLOCK_DEVIDER          kPWM_Prescale_Divide_128
#define DEMO_PWM_FREQUENCE              (1UL)
#define TRANSACTION_TIME_US             10U
#define ENCODER_ACCESS_FREQ_VS_PWM_FRE0 16
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void PWM_Trigger_Init(PWM_Type *PWMBase);
/*${prototype:end}*/

#endif /* _APP_H_ */
