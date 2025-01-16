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
#include "fsl_edma.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();

    /* FLEXIO is in Media domain, disable Security check to allow DMA operate FLEXIO. */
    BOARD_InitAHBSC();
    CLOCK_AttachClk(kMAIN_PLL_PFD3_to_FLEXIO);
    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 20U);
    RESET_ClearPeripheralReset(kFLEXIO0_RST_SHIFT_RSTn);

    EDMA_EnableRequest(EXAMPLE_FLEXIO_UART_DMA_BASEADDR, EXAMPLE_TX_DMA_SOURCE);
    EDMA_EnableRequest(EXAMPLE_FLEXIO_UART_DMA_BASEADDR, EXAMPLE_RX_DMA_SOURCE);
}
/*${function:end}*/
