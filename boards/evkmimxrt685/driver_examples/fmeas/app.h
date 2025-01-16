/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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
#define FMEAS_SYSCON                           FREQME
#define EXAMPLE_REFERENCE_CLOCK_REGISTRY_INDEX 0
/* According to user manual, FMEASURE_CH0_SEL is reference clock select register. */
#define EXAMPLE_REFERENCE_CLOCK_NAME        "main system clock"
#define EXAMPLE_REFERENCE_CLOCK             kINPUTMUX_MainSysClkToFreqmeas
#define EXAMPLE_REFERENCE_CLOCK_FREQUENCY   CLOCK_GetFreq(kCLOCK_BusClk)
#define EXAMPLE_TARGET_CLOCK_REGISTRY_INDEX 1
#define EXAMPLE_TARGET_CLOCK_1_NAME         "External clock (clk_in)"
#define EXAMPLE_TARGET_CLOCK_2_NAME         "SFRO clock"
#define EXAMPLE_TARGET_CLOCK_3_NAME         "FFRO clock"
#define EXAMPLE_TARGET_CLOCK_1              kINPUTMUX_XtalinToFreqmeas
#define EXAMPLE_TARGET_CLOCK_2              kINPUTMUX_SfroToFreqmeas
#define EXAMPLE_TARGET_CLOCK_3              kINPUTMUX_FfroToFreqmeas
#define EXAMPLE_TARGET_CLOCK_1_FREQUENCY    CLOCK_GetXtalInClkFreq()
#define EXAMPLE_TARGET_CLOCK_2_FREQUENCY    CLOCK_GetSFroFreq()
#define EXAMPLE_TARGET_CLOCK_3_FREQUENCY    CLOCK_GetFFroFreq()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
