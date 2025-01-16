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
#define FMEAS_SYSCON FREQME
/* FMEASURE_CH0_SEL is reference clock selector, FMEASURE_CH1_SEL is target clock selector. */
#define EXAMPLE_REFERENCE_CLOCK_REGISTRY_INDEX 0
#define EXAMPLE_REFERENCE_CLOCK_NAME           "main system clock"
#define EXAMPLE_REFERENCE_CLOCK                kINPUTMUX_MainSysClkToFreqmeas
#define EXAMPLE_REFERENCE_CLOCK_FREQUENCY      CLOCK_GetFreq(kCLOCK_BusClk)
#define EXAMPLE_TARGET_CLOCK_REGISTRY_INDEX    1
#define EXAMPLE_TARGET_CLOCK_1_NAME            "External clock (clk_in)"
#define EXAMPLE_TARGET_CLOCK_2_NAME            "LPOSC clock"
#define EXAMPLE_TARGET_CLOCK_3_NAME            "FRO192M clock"
#define EXAMPLE_TARGET_CLOCK_1                 kINPUTMUX_XtalinToFreqmeas
#define EXAMPLE_TARGET_CLOCK_2                 kINPUTMUX_LposcToFreqmeas
#define EXAMPLE_TARGET_CLOCK_3                 kINPUTMUX_Fro192mToFreqmeas
#define EXAMPLE_TARGET_CLOCK_1_FREQUENCY       CLOCK_GetXtalInClkFreq()
#define EXAMPLE_TARGET_CLOCK_2_FREQUENCY       CLOCK_GetLpOscFreq()
#define EXAMPLE_TARGET_CLOCK_3_FREQUENCY       CLK_FRO_CLK
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
