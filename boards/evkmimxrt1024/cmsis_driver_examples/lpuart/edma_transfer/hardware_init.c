/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "app.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_BootClockRUN();

    /* DMAMux init and EDMA init */
    DMAMUX_Init(EXAMPLE_USART_DMAMUX_BASEADDR);
    edma_config_t edmaConfig = {0};
    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(EXAMPLE_USART_DMA_BASEADDR, &edmaConfig);
}

uint32_t LPUART1_GetFreq(void)
{
    return BOARD_DebugConsoleSrcFreq();
}
/*${function:end}*/
