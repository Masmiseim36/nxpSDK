/*
 * Copyright 2021 NXP
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
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /*Set clock source for LPSPI*/
    CLOCK_SetMux(kCLOCK_LpspiMux, EXAMPLE_LPSPI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_LpspiDiv, EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER);
}
/*${function:end}*/
