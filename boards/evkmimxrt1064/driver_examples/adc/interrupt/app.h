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
#define DEMO_ADC_BASE          ADC1
#define DEMO_ADC_IRQn          ADC1_IRQn
#define DEMO_ADC_USER_CHANNEL  0U
#define DEMO_ADC_CHANNEL_GROUP 0U
#define EXAMPLE_ADC_IRQHandler ADC1_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
