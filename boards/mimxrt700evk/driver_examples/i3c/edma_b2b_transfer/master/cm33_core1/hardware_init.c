/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"
#include "fsl_edma.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitAHBSC();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();

    CLOCK_AttachClk(kSENSE_BASE_to_I3C23);
    CLOCK_SetClkDiv(kCLOCK_DivI3c23Clk, 4U);

    POWER_DisablePD(kPDRUNCFG_APD_DMA2_3);
    POWER_DisablePD(kPDRUNCFG_PPD_DMA2_3);
    POWER_ApplyPD();
    RESET_ClearPeripheralReset(kDMA3_RST_SHIFT_RSTn);
    EDMA_EnableRequest(EXAMPLE_DMA, EXAMPLE_I3C_TX_DMA_CHANNEL_MUX);
    EDMA_EnableRequest(EXAMPLE_DMA, EXAMPLE_I3C_RX_DMA_CHANNEL_MUX);
}
/*${function:end}*/
