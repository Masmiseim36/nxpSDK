/*
 * Copyright 2020, 2023-2024 NXP
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
#define DEMO_LPADC_BASE        ADC1
#define DEMO_LPADC_CHANNEL_NUM 1U
#define DEMO_LPADC_IRQn        ADC1_IRQn
#define DEMO_LPADC_IRQ_HANDLER ADC1_IRQHandler

#define DEMO_LPIT_BASE        LPIT3
#define DEMO_LPIT_CHANNEL_NUM kLPIT_Chnl_0
#define DEMO_LPIT_IRQn        LPIT3_IRQn
#define DEMO_LPIT_IRQ_HANDLER LPIT3_IRQHandler
#define DEMO_LPIT_CLOCK_FREQ  CLOCK_GetRootClockFreq(kCLOCK_Root_Lpit3)

#define DEMO_CLOCK_FUNCTION_PARAMETER_COUNT 2U
#define DEMO_LPADC_DO_OFFSET_CALIBRATION    true
#define DEMO_LPADC_OFFSET_CALIBRATION_MODE  kLPADC_OffsetCalibration12bitMode
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitADCClock(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
