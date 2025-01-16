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
#include "app_definitions.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if (SEMC_ENABLED == 1)
#define DEMO_SEMC               SEMC
#define DEMO_SEMC_START_ADDRESS (0x80000000U)
#define DEMO_SEMC_CLK_FREQ      CLOCK_GetFreq(kCLOCK_SemcClk)
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
status_t BOARD_InitSEMC(void);

/*${prototype:end}*/

#endif /* _APP_H_ */
