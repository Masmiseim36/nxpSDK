/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_
/*${header:start}*/
#include "fsl_power.h"
/*${header:end}*/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#if defined(OSTIMER0)
#define EXAMPLE_OSTIMER OSTIMER0
#else
#define EXAMPLE_OSTIMER OSTIMER
#endif
#define EXAMPLE_OSTIMER_FREQ CLOCK_GetLpOscFreq()
#define EXAMPLE_DEEPSLEEP_RUNCFG0 \
    (SYSCTL0_PDRUNCFG0_LPOSC_PD_MASK)         /*!< Power down all unnecessary blocks during deep sleep*/
#define EXAMPLE_DEEPSLEEP_RAM_APD 0xFFC00000U /* 0x280000 - 0x4FFFFF keep powered */
#define EXAMPLE_DEEPSLEEP_RAM_PPD 0x0U
#define EXAMPLE_EXCLUDE_FROM_DEEPSLEEP                                                                              \
    (((const uint32_t[]){EXAMPLE_DEEPSLEEP_RUNCFG0,                                                                 \
                         (SYSCTL0_PDSLEEPCFG1_FLEXSPI0_SRAM_APD_MASK | SYSCTL0_PDSLEEPCFG1_FLEXSPI1_SRAM_APD_MASK | \
                          SYSCTL0_PDSLEEPCFG1_FLEXSPI0_SRAM_PPD_MASK | SYSCTL0_PDSLEEPCFG1_FLEXSPI1_SRAM_PPD_MASK), \
                         EXAMPLE_DEEPSLEEP_RAM_APD, EXAMPLE_DEEPSLEEP_RAM_PPD}))
#define EXAMPLE_EnableDeepSleepIRQ() EnableDeepSleepIRQ(OS_EVENT_IRQn)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
/* Enter deep sleep mode. */
void EXAMPLE_EnterDeepSleep(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
