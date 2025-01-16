/*
 * Copyright 2018 NXP
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
/* The QTMR instance/channel used for board */
#define BOARD_QTMR_BASEADDR              TMR4
#define BOARD_QTMR_INPUT_CAPTURE_CHANNEL kQTMR_Channel_0
#define BOARD_QTMR_PWM_CHANNEL           kQTMR_Channel_1
#define QTMR_PWM_OUTPUT_FREQUENCY        50000
#define QTMR_DUTYCYCLE_PERCENT           50
#define QTMR_CounterInputPin             kQTMR_Counter0InputPin

/* Interrupt number and interrupt handler for the QTMR instance used */
#define QTMR_IRQ_ID      TMR4_IRQn
#define QTMR_IRQ_HANDLER TMR4_IRQHandler

/* QTMR Clock source divider for Ipg clock source, the value of two macros below should be aligned. */
#define QTMR_PRIMARY_SOURCE       (kQTMR_ClockDivide_8)
#define QTMR_CLOCK_SOURCE_DIVIDER (8U)

/* Get source clock for QTMR driver */
#define QTMR_SOURCE_CLOCK (CLOCK_GetRootClockFreq(kCLOCK_Root_Bus) / QTMR_CLOCK_SOURCE_DIVIDER)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
