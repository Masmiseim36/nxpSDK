/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "fsl_common.h"
#include "pin_mux.h"
#include "board.h"
#include "app.h"

#if ((defined FSL_FEATURE_SOC_INTMUX_COUNT) && (FSL_FEATURE_SOC_INTMUX_COUNT))
#include "fsl_intmux.h"
#endif

/*${header:end}*/

/*${function:start}*/
extern void LPSPI1_DriverIRQHandler(void);

void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

#if defined(ENABLE_RAM_VECTOR_TABLE)
    InstallIRQHandler(LPSPI1_IRQn, (uint32_t)LPSPI1_DriverIRQHandler);
#endif
}

uint32_t LPSPI1_GetFreq(void)
{
    return EXAMPLE_LPSPI_CLOCK_FREQ;
}
/*${function:end}*/
