/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
#include "fsl_edma_soc.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/
/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

    /* attach FC0 clock to LP_FLEXCOMM (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_FCCLK_ATTACH);
    CLOCK_SetClkDiv(BOARD_DEBUG_UART_FCCLK_DIV, 1U);
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    EDMA_EnableRequest(EXAMPLE_LPUART_DMA_BASEADDR, DEMO_LPUART_TX_EDMA_CHANNEL);
    EDMA_EnableRequest(EXAMPLE_LPUART_DMA_BASEADDR, DEMO_LPUART_RX_EDMA_CHANNEL);
}
/*${function:end}*/
