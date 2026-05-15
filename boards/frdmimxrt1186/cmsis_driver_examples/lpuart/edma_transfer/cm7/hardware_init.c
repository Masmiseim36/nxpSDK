/*
 * Copyright 2021 NXP
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
#include "fsl_edma.h"
#include "fsl_trdc.h"
/*${header:end}*/

/*${variable:start}*/
edma_config_t edmaConfig            = {0};
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_CommonSetting();
    BOARD_ConfigMPU();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    EDMA_GetDefaultConfig(&edmaConfig);
    EDMA_Init(EXAMPLE_USART_DMA_BASEADDR, &edmaConfig);
    EDMA_SetChannelMux(EXAMPLE_USART_DMA_BASEADDR, RTE_USART1_DMA_TX_CH, RTE_USART1_DMA_TX_PERI_SEL);
    EDMA_SetChannelMux(EXAMPLE_USART_DMA_BASEADDR, RTE_USART1_DMA_RX_CH, RTE_USART1_DMA_RX_PERI_SEL);
}

uint32_t LPUART1_GetFreq(void)
{
    return BOARD_DebugConsoleSrcFreq();
}
/*${function:end}*/
