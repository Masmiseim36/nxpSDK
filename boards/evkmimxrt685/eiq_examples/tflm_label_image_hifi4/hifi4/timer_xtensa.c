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
#if (defined(CPU_MIMXRT798SGAWAR_hifi4) || defined(CPU_MIMXRT798SGFOA_hifi4))
	xos_set_clock_freq(CLOCK_GetFreq(kCLOCK_Hifi4CpuClk));
#elif  (defined(CPU_MIMXRT798SGAWAR_hifi1) || defined(CPU_MIMXRT798SGFOA_hifi1))
	xos_set_clock_freq(CLOCK_GetFreq(kCLOCK_Hifi1CpuClk));
#elif (defined(CPU_MIMXRT685SFAWBR_dsp) || defined(CPU_MIMXRT685SFFOB_dsp) || defined(CPU_MIMXRT685SFVKB_dsp) || defined(CPU_MIMXRT685SVFVKB_dsp) ||(defined(CPU_MIMXRT595SFAWC_dsp) || defined(CPU_MIMXRT595SFFOC_dsp)))
	xos_set_clock_freq(CLOCK_GetFreq(kCLOCK_DspCpuClk));
#endif
    xos_start_system_timer(-1, 0);
}

int TIMER_GetTimeInUS()
{
    return xos_get_system_cycles() / (xos_get_clock_freq() / 1000000);
}
