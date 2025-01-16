/*
 * Copyright 2020 NXP
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
#define DEMO_LPADC_BASE        LPADC1
#define DEMO_LPADC_CHANNEL_NUM 0U
#define DEMO_LPADC_IRQn        ADC1_IRQn
#define DEMO_LPADC_IRQ_HANDLER ADC1_IRQHandler
#define DEMO_LPADC_VREF_SOURCE kLPADC_ReferenceVoltageAlt1
#define DEMO_DMAMUX_BASE       DMAMUX0

#define DEMO_DMA_BASE    DMA0
#define DEMO_DMA_CHANNEL 0U
#define DEMO_DMA_SOURCE  66U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitADCClock(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
