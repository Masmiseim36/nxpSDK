/*
 * Copyright 2021 NXP
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
#define DEMO_LPADC_BASE             LPADC1
#define DEMO_LPADC_IRQn             ADC1_IRQn
#define DEMO_LPADC_IRQ_HANDLER_FUNC ADC1_IRQHandler
#define DEMO_LPADC_USER_CHANNEL     0U
#define DEMO_LPADC_USER_CMDID       1U /* CMD1 */
#define DEMO_LPADC_VREF_SOURCE      kLPADC_ReferenceVoltageAlt1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
