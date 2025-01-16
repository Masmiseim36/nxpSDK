/*
 * Copyright 2021, 2023 NXP
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

#define DEMO_PIT_BASE        PIT1
#define DEMO_PIT_CHANNEL_NUM kPIT_Chnl_0
#define DEMO_PIT_IRQn        PIT1_IRQn
#define DEMO_PIT_IRQ_HANDLER PIT1_IRQHandler
#define DEMO_PIT_CLOCK_FREQ  CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)

#define DEMO_CLOCK_FUNCTION_PARAMETER_COUNT 1U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitADCClock(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
