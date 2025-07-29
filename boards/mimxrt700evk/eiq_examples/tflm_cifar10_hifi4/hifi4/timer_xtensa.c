/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_clock.h"
#include "timer.h"

#include <xtensa/xos.h>

/*******************************************************************************
 * Code
 ******************************************************************************/

void TIMER_Init()
{
#if (defined(MIMXRT798S_hifi4_SERIES))
	xos_set_clock_freq(CLOCK_GetFreq(kCLOCK_Hifi4CpuClk));
#elif  (defined(MIMXRT798S_hifi1_SERIES))
	xos_set_clock_freq(CLOCK_GetFreq(kCLOCK_Hifi1CpuClk));
#elif (defined(MIMXRT595S_dsp_SERIES) || defined(MIMXRT685S_dsp_SERIES))
	xos_set_clock_freq(CLOCK_GetFreq(kCLOCK_DspCpuClk));
#endif
    xos_start_system_timer(-1, 0);
}

int TIMER_GetTimeInUS()
{
    return xos_get_system_cycles() / (xos_get_clock_freq() / 1000000);
}
