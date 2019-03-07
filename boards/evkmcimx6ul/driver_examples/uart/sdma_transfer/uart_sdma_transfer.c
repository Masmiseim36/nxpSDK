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
#include "fsl_uart.h"
#include "fsl_uart_sdma.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_UART UART1
#define DEMO_UART_CLK_FREQ BOARD_DebugConsoleSrcFreq()
#define EXAMPLE_UART_DMA_BASEADDR SDMAARM
#define UART_RX_DMA_CHANNEL 1U
#define UART_TX_DMA_CHANNEL 2U
#define UART_RX_DMA_REQUEST (25)
#define UART_TX_DMA_REQUEST (26)

#define ECHO_BUFFER_LENGTH 8

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* UART user callback */
void UART_UserCallback(UART_Type *base, uart_sdma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(uart_sdma_handle_t g_uartSdmaHandle, 4) = {0};
AT_NONCACHEABLE_SECTION_ALIGN(sdma_handle_t g_uartTxSdmaHandle, 4) = {0};
AT_NONCACHEABLE_SECTION_ALIGN(sdma_handle_t g_uartRxSdmaHandle, 4) = {0};

AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t context_Tx, 4) = {0};
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t context_Rx, 4) = {0};

AT_NONCACHEABLE_SECTION_ALIGN(uart_transfer_t sendXfer, 4) = {0};
AT_NONCACHEABLE_SECTION_ALIGN(uart_transfer_t receiveXfer, 4) = {0};

AT_NONCACHEABLE_SECTION_ALIGN(uint8_t g_tipString[], 4) =
    "Uart sdma transfer example\r\nBoard receives 8 characters then sends them out\r\nNow please input:\r\n";

AT_NONCACHEABLE_SECTION_ALIGN(uint8_t g_txBuffer[ECHO_BUFFER_LENGTH], 4) = {0};
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH], 4) = {0};

volatile bool rxBufferEmpty = true;
volatile bool txBufferFull = false;
volatile bool txOnGoing = false;
volatile bool rxOnGoing = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* UART user callback */
void UART_UserCallback(UART_Type *base, uart_sdma_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_UART_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing = false;
    }

    if (kStatus_UART_RxIdle == status)
    {
        rxBufferEmpty = false;
        rxOnGoing = false;
    }
}

/*!e
 * @brief Main function
 */
int main(void)
{
    uart_config_t config;
    sdma_config_t sdmaConfig;
    uart_transfer_t xfer;

    SystemInitIrqTable();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    /* Enable SPBA */
    CLOCK_EnableClock(kCLOCK_Spba);
    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.dataBitsCount = kUART_EightDataBits;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 2;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.rxFifoWatermark = 1;
    config.enableTx = true;
    config.enableRx = true;

    UART_Init(DEMO_UART, &config, DEMO_UART_CLK_FREQ);

    /* Init the SDMA module */
    SDMA_GetDefaultConfig(&sdmaConfig);
    SDMA_Init(EXAMPLE_UART_DMA_BASEADDR, &sdmaConfig);
    SDMA_CreateHandle(&g_uartTxSdmaHandle, EXAMPLE_UART_DMA_BASEADDR, UART_TX_DMA_CHANNEL, &context_Tx);
    SDMA_CreateHandle(&g_uartRxSdmaHandle, EXAMPLE_UART_DMA_BASEADDR, UART_RX_DMA_CHANNEL, &context_Rx);
    SDMA_SetChannelPriority(EXAMPLE_UART_DMA_BASEADDR, UART_TX_DMA_CHANNEL, 3U);
    SDMA_SetChannelPriority(EXAMPLE_UART_DMA_BASEADDR, UART_RX_DMA_CHANNEL, 4U);

    /* Create UART DMA handle. */
    UART_TransferCreateHandleSDMA(DEMO_UART, &g_uartSdmaHandle, UART_UserCallback, NULL, &g_uartTxSdmaHandle,
                                  &g_uartRxSdmaHandle, UART_TX_DMA_REQUEST, UART_RX_DMA_REQUEST);

    /* Send g_tipString out. */
    xfer.data = g_tipString;
    xfer.dataSize = sizeof(g_tipString) - 1;
    txOnGoing = true;
    UART_SendSDMA(DEMO_UART, &g_uartSdmaHandle, &xfer);

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
            UART_ReceiveSDMA(DEMO_UART, &g_uartSdmaHandle, &receiveXfer);
        }
        /* If TX is idle and g_txBuffer is full, start to send data. */
        if ((!txOnGoing) && txBufferFull)
        {
            txOnGoing = true;
            UART_SendSDMA(DEMO_UART, &g_uartSdmaHandle, &sendXfer);
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
