/*
 * Copyright 2021-2022 NXP
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
#define DEMO_ADC_BASE           LPADC1
#define DEMO_ADC_USER_CHANNEL0  0U
#define DEMO_ADC_USER_CHANNEL1  4U
#define DEMO_ADC_USER_CMDID1    1U
#define DEMO_ADC_USER_CMDID2    2U
#define DEMO_ADC_CHANNEL_GROUP0 0U
#define DEMO_ADC_CHANNEL_GROUP1 1U

#define DEMO_ADC_ETC_BASE          ADC_ETC
#define DEMO_ADC_ETC_CHAIN_LENGTH  1U /* Chain length is 2. */
#define DEMO_ADC_ETC_CHANNEL1      1U
#define DEMO_ADC_ETC_CHANNEL2      2U
#define DEMO_ADC_ETC_TRIGGER_GROUP 0U
#define DEMO_ADC_ETC_DONE0_Handler ADC_ETC_IRQ0_IRQHandler
#define DEMO_ADC_ETC_DONE1_Handler ADC_ETC_IRQ1_IRQHandler

#define DEMO_XBARA_BASE           XBARA1
#define DEMO_XBARA_INPUT_PITCH0   kXBARA1_InputPit1Trigger0
#define DEMO_XBARA_OUTPUT_ADC_ETC kXBARA1_OutputAdcEtc0Coco0

/*
 * Clock source:
 * PIT1: Bus clock
 * PIT2: Bus LPSR clock
 */
#define DEMO_PIT_BASE         PIT1
#define DEMO_PIT_CLOCK_SOURCE CLOCK_GetRootClockFreq(kCLOCK_Root_Bus)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
