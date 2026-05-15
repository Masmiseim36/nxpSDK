/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef __SERVO_MOTOR_H___
#define __SERVO_MOTOR_H___
#include "fsl_common.h"
#include "mc_periph_init.h"
#include "freemaster.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "fsl_rgpio.h"
#include "fsl_lpuart.h"
#include "m1_sm_snsless_enc.h"

#include "freemaster_serial_lpuart.h"
#include "board.h"
#include "mid_sm_states.h"

RAM_FUNC_LIB
void ADC1_IRQHandler(void);
/* TMR1 reload ISR called with 1ms period */

void Application_Control_BL(void);

RAM_FUNC_LIB
int servo_motor_init(void);
RAM_FUNC_LIB
void SM_StateMachineSlowTask(void);
RAM_FUNC_LIB
int32_t GetPositionActualValue(void);
RAM_FUNC_LIB
acc32_t GetPositionCmdValue(int32_t);
#endif
