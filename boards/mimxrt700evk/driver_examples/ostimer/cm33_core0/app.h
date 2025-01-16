/*
 * Copyright 2023-2024 NXP
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
#define EXAMPLE_OSTIMER_FREQ         CLOCK_GetLpOscFreq()
#define EXAMPLE_OSTIMER              OSTIMER_CPU0
#define EXAMPLE_OSTIMER_IRQn         OS_EVENT_IRQn
#define EXAMPLE_EnableDeepSleepIRQ() EnableDeepSleepIRQ(OS_EVENT_IRQn)

#define APP_DEEPSLEEP_SLEEPCFG    (SLEEPCON0_SLEEPCFG_LPOSC_PD_MASK)
#define APP_DEEPSLEEP_PDSLEEPCFG0 (0U)
#define APP_DEEPSLEEP_RAM_APD     0x1F00U /* 0x80000 - 0x2FFFFF([PT8-PT12]) keep powered */
#define APP_DEEPSLEEP_RAM_PPD     (0U)
#define APP_DEEPSLEEP_PDSLEEPCFG4 \
    (PMC_PDSLEEPCFG4_CPU0_CCACHE_MASK | PMC_PDSLEEPCFG4_CPU0_SCACHE_MASK | PMC_PDSLEEPCFG4_OCOTP_MASK)
#define APP_DEEPSLEEP_PDSLEEPCFG5 (0U)
#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                     \
    (((const uint32_t[]){APP_DEEPSLEEP_SLEEPCFG, APP_DEEPSLEEP_PDSLEEPCFG0, 0U, APP_DEEPSLEEP_RAM_APD, \
                         APP_DEEPSLEEP_RAM_PPD, APP_DEEPSLEEP_PDSLEEPCFG4, APP_DEEPSLEEP_PDSLEEPCFG5}))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/* Enter deep sleep mode. */
void EXAMPLE_EnterDeepSleep(void);
/* Restore clock configuration. */
void BOARD_RestoreClockConfig(void);
/* Change to a safe clock source for power mode change. */
void BOARD_ClockSafeConfig(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
