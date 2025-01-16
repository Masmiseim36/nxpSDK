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

/*!< Power down all unnecessary blocks and enable RBB during deep sleep. */
#define APP_DEEPSLEEP_RUNCFG0 (SYSCTL0_PDRUNCFG0_LPOSC_PD_MASK | SYSCTL0_PDSLEEPCFG0_RBB_PD_MASK)
#define APP_DEEPSLEEP_RAM_APD 0xFFFFF8U
#define APP_DEEPSLEEP_RAM_PPD 0x0U
#define EXAMPLE_EXCLUDE_FROM_DEEPSLEEP                                                                            \
    (((const uint32_t[]){APP_DEEPSLEEP_RUNCFG0,                                                                   \
                         (SYSCTL0_PDSLEEPCFG1_FLEXSPI_SRAM_APD_MASK | SYSCTL0_PDSLEEPCFG1_FLEXSPI_SRAM_PPD_MASK), \
                         APP_DEEPSLEEP_RAM_APD, APP_DEEPSLEEP_RAM_PPD}))
#define EXAMPLE_EnableDeepSleepIRQ() EnableDeepSleepIRQ(OS_EVENT_IRQn)
/* Enter deep sleep mode. */
void EXAMPLE_EnterDeepSleep(void);

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
