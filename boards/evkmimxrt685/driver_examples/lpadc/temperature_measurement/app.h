/*
 * Copyright 2020, 2024 NXP
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
#define DEMO_LPADC_BASE              ADC0
#define DEMO_LPADC_TEMP_SENS_CHANNEL 7U
#define DEMO_LPADC_USER_CMDID        15U /* The available command numbers are 1-15 */
#define DEMO_LPADC_TEMPERATURE_SLOPE                                                              \
    -1.536f                              /* Temperature sensor slope with the unit as mV/Celsius. \
                                          Please refer to the Data Sheet for details. */
#define DEMO_LPADC_TEMPERATURE_INTERCEPT                                                      \
    807.0f /* The voltage of the temperature sensor channel at 0 Celsius with the unit as mV. \
           Please refer to the Data Sheet for details. */
#define DEMO_LPADC_USE_HIGH_RESOLUTION 0U
#define DEMO_LPADC_IRQn                ADC0_IRQn
#define DEMO_LPADC_IRQ_HANDLER_FUNC    ADC0_IRQHandler
#define DEMO_LPADC_SAMPLE_CHANNEL_MODE kLPADC_SampleChannelDiffBothSideAB
#define DEMO_LPADC_VREEFP              1.8f /* LPADC positive reference voltage value with the unit as V. */
#define DEMO_LPADC_VREFN               0.0f /* LPADC negative reference voltage value with the unit as V. */
#define DEMO_LPADC_HARDWARE_AVERAGE    kLPADC_HardwareAverageCount128
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
float DEMO_MeasureTemperature(ADC_Type *base, uint32_t commandId, uint32_t index);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
