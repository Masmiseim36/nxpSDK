/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_clock.h"
#include "timer.h"

#include <stdint.h>
#include <time.h>

/*******************************************************************************
 * Code
 ******************************************************************************/

void TIMER_Init()
{
	CLOCK_SetXtalFreq(24000000);
}

int TIMER_GetTimeInUS()
{
    return clock() / (CLOCK_GetFreq(kCLOCK_DspCpuClk) / 1000000);
}
