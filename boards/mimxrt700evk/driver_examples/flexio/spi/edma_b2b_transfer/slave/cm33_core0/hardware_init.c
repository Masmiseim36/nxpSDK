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
#include "fsl_clock.h"
#include "app.h"
#include "fsl_power.h"
#include "fsl_edma_soc.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();

    BOARD_InitAHBSC();

    POWER_DisableLPRequestMask(kPower_MaskFlexio);

    CLOCK_AttachClk(kMAIN_PLL_PFD3_to_FLEXIO);
    CLOCK_SetClkDiv(kCLOCK_DivFlexioClk, 5U);
    RESET_ClearPeripheralReset(kFLEXIO0_RST_SHIFT_RSTn);
    RESET_ClearPeripheralReset(kDMA0_RST_SHIFT_RSTn);

    EDMA_EnableRequest(EXAMPLE_FLEXIO_SPI_DMA_BASEADDR, EXAMPLE_TX_DMA_SOURCE);
    EDMA_EnableRequest(EXAMPLE_FLEXIO_SPI_DMA_BASEADDR, EXAMPLE_RX_DMA_SOURCE);
}
/*${function:end}*/
