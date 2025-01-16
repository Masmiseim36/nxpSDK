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
#include "RTE_Device.h"
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
    CLOCK_AttachClk(kOSC_CLK_to_FCCLK0);
    CLOCK_AttachClk(kFCCLK0_to_FLEXCOMM8);
    
    RESET_ClearPeripheralReset(kDMA0_RST_SHIFT_RSTn);
    EDMA_EnableRequest(RTE_I2C8_DMA_TX_DMA_BASE, RTE_I2C8_DMA_TX_PERI_SEL);
    EDMA_EnableRequest(RTE_I2C8_DMA_RX_DMA_BASE, RTE_I2C8_DMA_RX_PERI_SEL);
}

uint32_t LPI2C8_GetFreq(void)
{
    return LPI2C_CLOCK_FREQUENCY;
}
/* ${function:end} */
