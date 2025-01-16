/*
 * Copyright 2024 NXP
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
    BOARD_InitPins();
    BOARD_BootClockRUN();

    BOARD_InitDebugConsole();
    
    POWER_DisablePD(kPDRUNCFG_DSR_VDDN_COM);
    POWER_DisablePD(kPDRUNCFG_DSR_VDD2N_MEDIA);
    POWER_ApplyPD();

    CLOCK_AttachClk(kFRO1_DIV1_to_LPSPI14);
    CLOCK_SetClkDiv(kCLOCK_DivLpspi14Clk, 8U);
    RESET_ClearPeripheralReset(kLPSPI14_RST_SHIFT_RSTn);

    RESET_ClearPeripheralReset(kDMA0_RST_SHIFT_RSTn);
    EDMA_EnableRequest(EXAMPLE_LPSPI_MASTER_DMA_BASE, DEMO_LPSPI_TRANSMIT_EDMA_CHANNEL);
    EDMA_EnableRequest(EXAMPLE_LPSPI_MASTER_DMA_BASE, DEMO_LPSPI_RECEIVE_EDMA_CHANNEL);
    
    BOARD_InitAHBSC();
}

uint32_t LPSPI14_GetFreq()
{
    return EXAMPLE_LPSPI_CLOCK_FREQ;
}
/*${function:end}*/
