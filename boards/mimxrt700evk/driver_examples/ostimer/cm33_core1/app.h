/*
 * Copyright 2023 NXP
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
#define EXAMPLE_OSTIMER_FREQ         CLOCK_GetLpOscFreq()
#define EXAMPLE_OSTIMER              OSTIMER_CPU1
#define EXAMPLE_OSTIMER_IRQn         OS_EVENT_IRQn
#define EXAMPLE_EnableDeepSleepIRQ() EnableDeepSleepIRQ(OS_EVENT_IRQn)

#define APP_DEEPSLEEP_SLEEPCFG    (0U)
#define APP_DEEPSLEEP_PDSLEEPCFG0 (0U)
#define APP_DEEPSLEEP_RAM_APD     0x3FFC0000U /* 0x580000 - 0x67FFFF([PT18-PT26]) keep powered */
#define APP_DEEPSLEEP_RAM_PPD     0x0U
#define APP_EXCLUDE_FROM_DEEPSLEEP                                                                     \
    (((const uint32_t[]){APP_DEEPSLEEP_SLEEPCFG, APP_DEEPSLEEP_PDSLEEPCFG0, 0U, APP_DEEPSLEEP_RAM_APD, \
                         APP_DEEPSLEEP_RAM_PPD, 0U, 0U}))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/* Enter deep sleep mode. */
void EXAMPLE_EnterDeepSleep(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
