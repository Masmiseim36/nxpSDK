/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include "board.h"
#include "mcdrv.h"
#include "freemaster.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_lpuart.h"
#include "m1_sm_snsless_enc.h"

#include "freemaster_serial_lpuart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The UART to use for FreeMASTER communication */
#define BOARD_FMSTR_UART_PORT LPUART1
#define BOARD_FMSTR_UART_BAUDRATE 19200U

#define BOARD_USER_BUTTON_PRIORITY 4

/* CPU load measurement SysTick START / STOP macros */
#define SYSTICK_START_COUNT() (SysTick->VAL = SysTick->LOAD)
#define SYSTICK_STOP_COUNT(par1) uint32_t val = SysTick->VAL; uint32_t load = SysTick->LOAD; par1 = load - val
   
/* Three instruction added after interrupt flag clearing as required */
#define M1_END_OF_ISR \
    {                 \
        __DSB(); \
        __ISB(); \
    }

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
  
/* Init SDK HW */
void BOARD_Init(void);
/* ADC COCO interrupt */
void ADC1_IRQHandler(void);
/* TMR1 reload ISR called with 1ms period */
void TMR1_IRQHandler(void);
/* SW8 Button interrupt handler */
void GPIO5_Combined_0_15_IRQHandler(void);
/* Demo Speed Stimulator */
void DemoSpeedStimulator(void);
/* Demo Position Stimulator */
void DemoPositionStimulator(void);
    
void BOARD_InitUART(uint32_t u32BaudRate);
void BOARD_InitSysTick(void);
void BOARD_InitGPIO(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _MAIN_H_ */
