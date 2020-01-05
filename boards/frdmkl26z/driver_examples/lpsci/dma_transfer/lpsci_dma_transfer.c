/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "board.h"
#include "fsl_lpsci_dma.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* LPSCI instance and clock */
#define DEMO_LPSCI UART0
#define DEMO_LPSCI_CLKSRC kCLOCK_CoreSysClk
#define DEMO_LPSCI_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define LPSCI_TX_DMA_CHANNEL 0U
#define LPSCI_RX_DMA_CHANNEL 1U
#define LPSCI_TX_DMA_REQUEST kDmaRequestMux0LPSCI0Tx
#define LPSCI_RX_DMA_REQUEST kDmaRequestMux0LPSCI0Rx
#define EXAMPLE_LPSCI_DMAMUX_BASEADDR DMAMUX0
#define EXAMPLE_LPSCI_DMA_BASEADDR DMA0
#define ECHO_BUFFER_LENGTH 8

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* LPSCI user callback */
void LPSCI_UserCallback(UART0_Type *base, lpsci_dma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/

lpsci_dma_handle_t g_lpsciDmaHandle;
dma_handle_t g_lpsciTxDmaHandle;
dma_handle_t g_lpsciRxDmaHandle;
uint8_t g_tipString[] = "LPSCI DMA example\r\nSend back received data\r\nEcho every 8 characters\r\n";
uint8_t g_txBuffer[ECHO_BUFFER_LENGTH] = {0};
uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH] = {0};
volatile bool rxBufferEmpty = true;
volatile bool txBufferFull = false;
volatile bool txOnGoing = false;
volatile bool rxOnGoing = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* LPSCI user callback */
void LPSCI_UserCallback(UART0_Type *base, lpsci_dma_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_LPSCI_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing = false;
    }

    if (kStatus_LPSCI_RxIdle == status)
    {
        rxBufferEmpty = false;
        rxOnGoing = false;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    lpsci_config_t config;
    lpsci_transfer_t xfer;
    lpsci_transfer_t sendXfer;
    lpsci_transfer_t receiveXfer;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    CLOCK_SetLpsci0Clock(0x1U);

    /* Initialize the LPSCI. */
    /*
     * config.parityMode = kLPSCI_ParityDisabled;
     * config.stopBitCount = kLPSCI_OneStopBit;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPSCI_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableTx = true;
    config.enableRx = true;

    LPSCI_Init(DEMO_LPSCI, &config, DEMO_LPSCI_CLK_FREQ);

    /* Init DMAMUX */
    DMAMUX_Init(EXAMPLE_LPSCI_DMAMUX_BASEADDR);
    
    /* Set channel for LPSCI  */
    DMAMUX_SetSource(EXAMPLE_LPSCI_DMAMUX_BASEADDR, LPSCI_TX_DMA_CHANNEL, LPSCI_TX_DMA_REQUEST);
    DMAMUX_EnableChannel(EXAMPLE_LPSCI_DMAMUX_BASEADDR, LPSCI_TX_DMA_CHANNEL);
    DMAMUX_SetSource(EXAMPLE_LPSCI_DMAMUX_BASEADDR, LPSCI_RX_DMA_CHANNEL, LPSCI_RX_DMA_REQUEST);
    DMAMUX_EnableChannel(EXAMPLE_LPSCI_DMAMUX_BASEADDR, LPSCI_RX_DMA_CHANNEL);
    
    /* Init the DMA module */
    DMA_Init(EXAMPLE_LPSCI_DMA_BASEADDR);
    DMA_CreateHandle(&g_lpsciTxDmaHandle, EXAMPLE_LPSCI_DMA_BASEADDR, LPSCI_TX_DMA_CHANNEL);
    DMA_CreateHandle(&g_lpsciRxDmaHandle, EXAMPLE_LPSCI_DMA_BASEADDR, LPSCI_RX_DMA_CHANNEL);
    
    /* Create LPSCI DMA handle. */
    LPSCI_TransferCreateHandleDMA(DEMO_LPSCI, &g_lpsciDmaHandle, LPSCI_UserCallback, NULL, &g_lpsciTxDmaHandle,
                          &g_lpsciRxDmaHandle);

    /* Send g_tipString out. */
    xfer.data = g_tipString;
    xfer.dataSize = sizeof(g_tipString) - 1;
    txOnGoing = true;
    LPSCI_TransferSendDMA(DEMO_LPSCI, &g_lpsciDmaHandle, &xfer);

    /* Wait send finished */
    while (txOnGoing)
    {
    }

    /* Start to echo. */
    sendXfer.data = g_txBuffer;
    sendXfer.dataSize = ECHO_BUFFER_LENGTH;
    receiveXfer.data = g_rxBuffer;
    receiveXfer.dataSize = ECHO_BUFFER_LENGTH;

    while (1)
    {
        /* If RX is idle and g_rxBuffer is empty, start to read data to g_rxBuffer. */
        if ((!rxOnGoing) && rxBufferEmpty)
        {
            rxOnGoing = true;
            LPSCI_TransferReceiveDMA(DEMO_LPSCI, &g_lpsciDmaHandle, &receiveXfer);
        }

        /* If TX is idle and g_txBuffer is full, start to send data. */
        if ((!txOnGoing) && txBufferFull)
        {
            txOnGoing = true;
            LPSCI_TransferSendDMA(DEMO_LPSCI, &g_lpsciDmaHandle, &sendXfer);
        }

        /* If g_txBuffer is empty and g_rxBuffer is full, copy g_rxBuffer to g_txBuffer. */
        if ((!rxBufferEmpty) && (!txBufferFull))
        {
            memcpy(g_txBuffer, g_rxBuffer, ECHO_BUFFER_LENGTH);
            rxBufferEmpty = true;
            txBufferFull = true;
        }
    }
}
