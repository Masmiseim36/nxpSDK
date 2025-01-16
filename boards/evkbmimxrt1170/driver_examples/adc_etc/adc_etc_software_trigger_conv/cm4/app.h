/*
 * Copyright 2019 NXP
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
#define DEMO_ADC_BASE          LPADC1
#define DEMO_ADC_USER_CHANNEL  0U
#define DEMO_ADC_USER_CMDID    1U
#define DEMO_ADC_CHANNEL_GROUP 0U

#define DEMO_ADC_ETC_BASE          ADC_ETC
#define DEMO_ADC_ETC_TRIGGER_GROUP 0U
#define DEMO_ADC_ETC_CHANNEL       0U
#define DEMO_ADC_ETC_DONE0_Handler ADC_ETC_IRQ0_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
