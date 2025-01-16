/*
 * Copyright 2022 NXP
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
#define LPIT_CLK_FREQ        CLOCK_GetRootClockFreq(kCLOCK_Root_Bus_Aon)
#define DEMO_LPIT_BASE       LPIT1
#define LPIT_CHANNEL         kLPIT_Chnl_0
#define DEMO_LPIT_IRQn       LPIT1_IRQn
#define DEMO_LPIT_IRQHandler LPIT1_IRQHandler
#define LPIT_PERIOD          1000000U

#define DEMO_LPADC_BASE             ADC1
#define DEMO_LPADC_IRQn             ADC1_IRQn
#define DEMO_LPADC_IRQ_HANDLER_FUNC ADC1_IRQHandler
#define DEMO_LPADC_USER_CHANNEL     7U
#define DEMO_LPADC_USER_CMDID       1U /* CMD1 */
/* ERRATA051385: ADC INL/DNL degrade under high ADC clock frequency when VREFH selected as reference. */
#define DEMO_LPADC_VREF_SOURCE kLPADC_ReferenceVoltageAlt2
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
