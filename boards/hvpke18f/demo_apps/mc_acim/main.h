/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MAIN_H
#define __MAIN_H

#include "board.h"
#include "mcdrv.h"
#include "freemaster.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "fsl_lpuart.h"
#include "m1_sm_ref_sol.h"
#include "freemaster_serial_lpuart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Three instruction added after interrupt flag clearing as required */
#define M1_END_OF_ISR \
    {                 \
        __DSB(); \
        __ISB(); \
    }

/*******************************************************************************
 * FreeMASTER communication constants
 ******************************************************************************/
/*! @brief The UART to use for FreeMASTER communication */
#define BOARD_FMSTR_UART_PORT LPUART2
#define BOARD_FMSTR_UART_BAUDRATE 19200

/* CPU load measurement SysTick START / STOP macros */
#define SYSTICK_START_COUNT() (SysTick->VAL = SysTick->LOAD)
#define SYSTICK_STOP_COUNT(par1) uint32_t val = SysTick->VAL; uint32_t load = SysTick->LOAD; par1 = load - val

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

void BOARD_Init(void);
void BOARD_InitGPIO(void);
void InitSysTick(void);
void BOARD_InitUART(uint32_t u32UClockSpeedinHz, uint32_t u32BaudRate);
void DemoSpeedStimulator(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _MAIN_H_ */
