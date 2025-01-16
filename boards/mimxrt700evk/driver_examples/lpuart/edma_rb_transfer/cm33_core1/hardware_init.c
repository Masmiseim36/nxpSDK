/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_edma_soc.h"
#include "fsl_power.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitAHBSC();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    CLOCK_SetClkDiv(BOARD_DEBUG_UART_CLK_DIV, 1U);
    
    POWER_DisablePD(kPDRUNCFG_APD_DMA2_3);
    POWER_DisablePD(kPDRUNCFG_PPD_DMA2_3);
    POWER_ApplyPD();    
    
    EDMA_EnableRequest(EXAMPLE_LPUART_DMA_BASEADDR, LPUART_TX_DMA_REQUEST);
    EDMA_EnableRequest(EXAMPLE_LPUART_DMA_BASEADDR, LPUART_RX_DMA_REQUEST);
}
/*${function:end}*/
