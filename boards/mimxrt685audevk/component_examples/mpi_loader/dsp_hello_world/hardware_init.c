/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "board.h"
#include "fsl_mpi_loader.h"
#include "fsl_dsp.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}

/* Enable multicore packed image loader for DSP context copy. */
void SystemInitHook(void)
{
    /* Configure FFRO clock */
    if ((SYSCTL0->PDRUNCFG0 & SYSCTL0_PDRUNCFG0_FFRO_PD_MASK) != 0)
    {
        POWER_DisablePD(kPDRUNCFG_PD_FFRO);  /* Power on FFRO (48/60MHz) */
        CLOCK_EnableFfroClk(kCLOCK_Ffro48M); /* Enable FFRO clock*/
    }
    /* DSP RAM need to be powered up and applied clock */
    CLOCK_AttachClk(kFFRO_to_DSP_MAIN_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivDspCpuClk, 1);
    CLOCK_SetClkDiv(kCLOCK_DivDspRamClk, 1);
    DSP_Init();
    MPI_LoadMultiImages();
}

/*${function:end}*/
