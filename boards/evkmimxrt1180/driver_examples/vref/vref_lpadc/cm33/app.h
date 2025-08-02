/*
 * Copyright 2024 NXP
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
#define DEMO_LPADC_BASE                         ADC1
#define DEMO_LPADC_USER_CHANNEL                 7U
#define DEMO_LPADC_USER_CMDID                   1U /* CMD1 */
/* ERRATA051385: ADC INL/DNL degrade under high ADC clock frequency when VREFH selected as reference. */
#define DEMO_LPADC_VREF_SOURCE                  kLPADC_ReferenceVoltageAlt2
#define DEMO_LPADC_USE_HIGH_RESOLUTION          true
#define DEMO_LPADC_OFFSET_CALIBRATION_MODE      kLPADC_OffsetCalibration16bitMode
#define DEMO_LPADC_DO_OFFSET_CALIBRATION        true
#define DEMO_LPADC_RESOLUTION_MODE              kLPADC_ConversionResolutionHigh
#define DEMO_LPADC_REFERENCE_VOLTAGE            1813.0F /* The unit is mv, the value is board specific,
                                                please measure the VDDA_ADC_1P8 pin to get the correct value. */
#define DEMO_VREF_BASE                          VREF
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitADCClock(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
