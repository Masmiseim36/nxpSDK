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
#include "fsl_usart.h"
#include "fsl_device_registers.h"
#include "fsl_sctimer.h"
#include "fsl_debug_console.h"
#include "m1_sm_snsless.h"
#include "freemaster_serial_miniusart.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief The UART to use for FreeMASTER communication */
#define BOARD_FMSTR_UART_PORT USART0
#define BOARD_FMSTR_UART_BAUDRATE 19200

#define TestPointPinOn \
    GPIO_PortSet(BOART_INITTESTPOINT_TP_GPIO, BOART_INITTESTPOINT_TP_PORT, 1U << BOART_INITTESTPOINT_TP_PIN);
#define TestPointPinOff \
    GPIO_PortClear(BOART_INITTESTPOINT_TP_GPIO, BOART_INITTESTPOINT_TP_PORT, 1U << BOART_INITTESTPOINT_TP_PIN);
#define TestPointPinToggle \
    GPIO_PortToggle(BOART_INITTESTPOINT_TP_GPIO, BOART_INITTESTPOINT_TP_PORT, 1U << BOART_INITTESTPOINT_TP_PIN);

/* Three instruction added after interrupt flag clearing as required */
#define M1_END_OF_ISR \
    {                 \
        __DSB();      \
        __ISB();      \
    }

/* CPU load measurement SysTick START / STOP macros */
#define SYSTICK_START_COUNT() (SysTick->VAL = SysTick->LOAD)
#define SYSTICK_STOP_COUNT(par1)   \
    uint32_t val  = SysTick->VAL;  \
    uint32_t load = SysTick->LOAD; \
    par1          = load - val

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
void DemoSpeedStimulator(void);

/* Init HW */
void BOARD_Init(void);
/* ADC COCO interrupt */
void ADC1_IRQHandler(void);
/* TMR1 reload ISR called with 1ms period */
void TMR1_IRQHandler(void);
/* SCT0 ISR initiated from CPM (overcurrent protection) */
void SCT0_IRQHandler(void);

void BOARD_InitUART(uint32_t u32BaudRate);
void BOARD_InitSysTick(void);
void BOARD_InitGPIOPins(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _MAIN_H_ */
