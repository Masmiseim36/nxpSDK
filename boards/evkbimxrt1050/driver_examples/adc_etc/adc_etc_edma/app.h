/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_ADC_BASE                   ADC1
#define DEMO_ADC_USER_CHANNEL           (16U)
#define DEMO_ADC_CHANNEL_GROUP0         (0U)
#define DEMO_ADC_ETC_BASE               ADC_ETC
#define DEMO_ADC_ETC_CHANNEL0           (15U)
#define DEMO_XBARA_BASE                 XBARA1
#define DEMO_XBARA_INPUT_PITCH0         kXBARA1_InputPitTrigger0
#define DEMO_XBARA_OUTPUT_ADC_ETC       kXBARA1_OutputAdcEtcXbar0Trig0
#define DEMO_EDMA_CHANNEL               (0U)
#define DEMO_EDMA_INSTANCE              (DMA0)
#define DEMO_BUFFER_LENGTH              (50U)
#define DEMO_PIT_SOURCE_CLOCK           CLOCK_GetFreq(kCLOCK_OscClk)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
