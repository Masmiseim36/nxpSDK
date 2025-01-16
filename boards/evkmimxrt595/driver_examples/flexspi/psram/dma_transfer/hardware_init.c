/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "fsl_dma.h"
#include "fsl_inputmux.h"
#include "app.h"
/*${header:end}*/

/*${function:start}*/

dma_handle_t dmaTxHandle = {0};
dma_handle_t dmaRxHandle = {0};

void BOARD_InitHardware(void)
{
    BOARD_InitPins();
    BOARD_InitPsRamPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    status_t status = BOARD_InitPsRam();
    if (status != kStatus_Success)
    {
        assert(false);
    }

    /* Configure DMAMUX. */
    RESET_PeripheralReset(kINPUTMUX_RST_SHIFT_RSTn);

    INPUTMUX_Init(INPUTMUX);
    INPUTMUX_AttachSignal(INPUTMUX, EXAMPLE_TX_CHANNEL, kINPUTMUX_Flexspi1TxToDma0);
    INPUTMUX_AttachSignal(INPUTMUX, EXAMPLE_RX_CHANNEL, kINPUTMUX_Flexspi1RxToDma0);
    /* Enable trigger. */
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Dmac0InputTriggerFlexspi1TxEna, true);
    INPUTMUX_EnableSignal(INPUTMUX, kINPUTMUX_Dmac0InputTriggerFlexspi1RxEna, true);
    /* Turnoff clock to inputmux to save power. Clock is only needed to make changes */
    INPUTMUX_Deinit(INPUTMUX);

    /* DMA init */
    DMA_Init(EXAMPLE_DMA);

    DMA_EnableChannel(EXAMPLE_DMA, EXAMPLE_TX_CHANNEL);
    DMA_EnableChannel(EXAMPLE_DMA, EXAMPLE_RX_CHANNEL);
    DMA_CreateHandle(&dmaTxHandle, EXAMPLE_DMA, EXAMPLE_TX_CHANNEL);
    DMA_CreateHandle(&dmaRxHandle, EXAMPLE_DMA, EXAMPLE_RX_CHANNEL);
}

/*${function:end}*/
