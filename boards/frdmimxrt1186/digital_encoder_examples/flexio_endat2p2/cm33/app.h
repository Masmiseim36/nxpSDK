/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef APP_H_
#define APP_H_

/*${header:start}*/
#include "fsl_flexio_endat2p2.h"
#include "pin_mux.h"
#include "fsl_pwm.h"
#include "fsl_xbar.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define BOARD_FLEXIO_INSTANCE           FLEXIO1
#define BOARD_FLEXIO_CLOCK_ROOT			kCLOCK_Root_Flexio1
#define BOARD_FLEXIO_CLOCK_ROOT_MUX     kCLOCK_FLEXIO2_ClockRoot_MuxSysPll3Div2
#define BOARD_FLEXIO_IRQHandler         FLEXIO1_IRQHandler
#define BOARD_FLEXIO_IRQn               FLEXIO1_IRQn

#define BOARD_PWM_INSTANCE              PWM1
#define PWM_SRC_CLK_FREQ                CLOCK_GetRootClockFreq(kCLOCK_Root_Bus_Aon)
#define DEMO_PWM_CLOCK_DEVIDER          kPWM_Prescale_Divide_128
#define DEMO_PWM_FREQUENCE              (1UL)
#define TRANSACTION_TIME_US             10U
#define ENCODER_ACCESS_FREQ_VS_PWM_FRE0 16

#define DEMO_BIT_RATE                   8000000
#define DEMO_ENCODER_MT_LEN             0
#define DEMO_ENCODER_ST_LEN             25
#define DEMO_ENCODER_RECOVERY_TIME_III  900  // 900ns

#define BOARD_INITPINS_RXD_CHANNEL 0
#define BOARD_INITPINS_TXD_CHANNEL  3
#define BOARD_INITPINS_CLK_CHANNEL  2
#define BOARD_INITPINS_DIR_CHANNEL 1

#define BOARD_FLEXIO_EXTERNAl_TRIGGER
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void PWM_Trigger_Init(PWM_Type *PWMBase);
/*${prototype:end}*/

#endif /* APP_H_ */
