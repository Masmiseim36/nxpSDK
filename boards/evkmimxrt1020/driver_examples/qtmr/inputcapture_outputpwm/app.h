/*
 * Copyright 2018-2020 NXP
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
#define BOARD_QTMR_BASEADDR              TMR2
#define BOARD_QTMR_INPUT_CAPTURE_CHANNEL kQTMR_Channel_0
#define BOARD_QTMR_PWM_CHANNEL           kQTMR_Channel_1
#define QTMR_CounterInputPin             kQTMR_Counter0InputPin

/* Interrupt number and interrupt handler for the QTMR instance used */
#define QTMR_IRQ_ID      TMR2_IRQn
#define QTMR_IRQ_HANDLER TMR2_IRQHandler

/* QTMR Clock source divider for Ipg clock source, the value of two macros below should be aligned. */
#define QTMR_PRIMARY_SOURCE       (kQTMR_ClockDivide_4)
#define QTMR_CLOCK_SOURCE_DIVIDER (4U)
/* The frequency of the source clock after divided. */
#define QTMR_SOURCE_CLOCK (CLOCK_GetFreq(kCLOCK_IpgClk) / QTMR_CLOCK_SOURCE_DIVIDER)
/* The frequency of the PWM signal QTMR generated. */
#define QTMR_PWM_OUTPUT_FREQUENCY (50000U)
/* The dutycycle of the PTM signal QTMR generated. */
#define QTMR_DUTYCYCLE_PERCENT (50U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
