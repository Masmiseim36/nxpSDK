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
#include "fsl_power.h"
/*${header:end}*/

/*${variable:start}*/
/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();

    POWER_DisablePD(kPDRUNCFG_DSR_VDDN_COM);
    POWER_DisablePD(kPDRUNCFG_DSR_VDD2N_MEDIA);
    POWER_ApplyPD();

    CLOCK_AttachClk(kFRO1_DIV1_to_LPSPI14);
    CLOCK_SetClkDiv(kCLOCK_DivLpspi14Clk, 3U);
    RESET_ClearPeripheralReset(kLPSPI14_RST_SHIFT_RSTn);

    RESET_ClearPeripheralReset(kDMA0_RST_SHIFT_RSTn);
    EDMA_EnableRequest(EXAMPLE_LPSPI_MASTER_DMA_BASE, DEMO_LPSPI_TRANSMIT_EDMA_CHANNEL);
    EDMA_EnableRequest(EXAMPLE_LPSPI_MASTER_DMA_BASE, DEMO_LPSPI_RECEIVE_EDMA_CHANNEL);

    BOARD_InitAHBSC();
}
/*${function:end}*/
