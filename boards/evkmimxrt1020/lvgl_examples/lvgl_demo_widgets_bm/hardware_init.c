/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"
/*${header:end}*/

/*${macro:start}*/
#define LPSPI_MASTER_DMA_BASEADDR     DMA0
#define LPSPI_MASTER_DMA_MUX_BASEADDR DMAMUX
/*${macro:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* DMA Mux init and EDMA init */
    edma_config_t edmaConfig = {0};
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(LPSPI_MASTER_DMA_BASEADDR, &edmaConfig);
#if (defined(FSL_FEATURE_SOC_DMAMUX_COUNT) && FSL_FEATURE_SOC_DMAMUX_COUNT)
    DMAMUX_Init(LPSPI_MASTER_DMA_MUX_BASEADDR);
#endif
}
/*${function:end}*/
