/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
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
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
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
#include "fsl_flexio_uart_edma.h"
#include "fsl_dma_manager.h"

#include "clock_config.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_FLEXIO_BASE FLEXIO0
#define FLEXIO_UART_TX_PIN 5U
#define FLEXIO_UART_RX_PIN 4U
#define FLEXIO_CLOCK_FREQUENCY CLOCK_GetFreq(kCLOCK_Osc0ErClk)
#define FLEXIO_DMA_REQUEST_BASE kDmaRequestMux0Group1FlexIO0Channel0
#define ECHO_BUFFER_LENGTH 8

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* UART user callback */
void FLEXIO_UART_UserCallback(FLEXIO_UART_Type *base,
                              flexio_uart_edma_handle_t *handle,
                              status_t status,
                              void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
flexio_uart_edma_handle_t g_uartHandle;
FLEXIO_UART_Type uartDev;
edma_handle_t g_uartTxEdmaHandle;
edma_handle_t g_uartRxEdmaHandle;

uint8_t g_tipString[] =
    "Flexio uart edma example\r\nBoard receives 8 characters then sends them out\r\nNow please input:\r\n";

uint8_t g_txBuffer[ECHO_BUFFER_LENGTH] = {0};
uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH] = {0};
volatile bool rxBufferEmpty = true;
volatile bool txBufferFull = false;
volatile bool txOnGoing = false;
volatile bool rxOnGoing = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* UART user callback */
void FLEXIO_UART_UserCallback(FLEXIO_UART_Type *base,
                              flexio_uart_edma_handle_t *handle,
                              status_t status,
                              void *userData)
{
    userData = userData;

    if (kStatus_FLEXIO_UART_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing = false;
    }

    if (kStatus_FLEXIO_UART_RxIdle == status)
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
    flexio_uart_config_t config;
    flexio_uart_transfer_t xfer;
    flexio_uart_transfer_t sendXfer;
    flexio_uart_transfer_t receiveXfer;
    dma_request_source_t dma_request_source_tx;
    dma_request_source_t dma_request_source_rx;

    BOARD_InitPins();
    BOARD_BootClockRUN();

    CLOCK_SetFlexio0Clock(2U);

    /*
     * config.enableUart = true;
     * config.enableInDoze = false;
     * config.enableInDebug = true;
     * config.enableFastAccess = false;
     * config.baudRate_Bps = 115200U;
     * config.bitCountPerChar = kFLEXIO_UART_8BitsPerChar;
     */
    FLEXIO_UART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableUart = true;

    uartDev.flexioBase = BOARD_FLEXIO_BASE;
    uartDev.TxPinIndex = FLEXIO_UART_TX_PIN;
    uartDev.RxPinIndex = FLEXIO_UART_RX_PIN;
    uartDev.shifterIndex[0] = 0U;
    uartDev.shifterIndex[1] = 1U;
    uartDev.timerIndex[0] = 0U;
    uartDev.timerIndex[1] = 1U;

    FLEXIO_UART_Init(&uartDev, &config, FLEXIO_CLOCK_FREQUENCY);

    /*Init DMA for example*/
    DMAMGR_Init();

    dma_request_source_tx = (dma_request_source_t)(FLEXIO_DMA_REQUEST_BASE + uartDev.shifterIndex[0]);
    dma_request_source_rx = (dma_request_source_t)(FLEXIO_DMA_REQUEST_BASE + uartDev.shifterIndex[1]);

    /* Request DMA channels for TX & RX. */
    if (dma_request_source_tx < 0x200)
    {
        DMAMGR_RequestChannel((dma_request_source_t)dma_request_source_tx, 0, &g_uartTxEdmaHandle);
        DMAMGR_RequestChannel((dma_request_source_t)dma_request_source_rx, 1, &g_uartRxEdmaHandle);
    }
    else
    {
        DMAMGR_RequestChannel((dma_request_source_t)dma_request_source_tx, 16, &g_uartTxEdmaHandle);
        DMAMGR_RequestChannel((dma_request_source_t)dma_request_source_rx, 17, &g_uartRxEdmaHandle);
    }

    FLEXIO_UART_TransferCreateHandleEDMA(&uartDev, &g_uartHandle, FLEXIO_UART_UserCallback, NULL, &g_uartTxEdmaHandle,
                                         &g_uartRxEdmaHandle);

    /* Send g_tipString out. */
    xfer.data = g_tipString;
    xfer.dataSize = sizeof(g_tipString) - 1;
    txOnGoing = true;
    FLEXIO_UART_TransferSendEDMA(&uartDev, &g_uartHandle, &xfer);

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
            FLEXIO_UART_TransferReceiveEDMA(&uartDev, &g_uartHandle, &receiveXfer);
        }

        /* If TX is idle and g_txBuffer is full, start to send data. */
        if ((!txOnGoing) && txBufferFull)
        {
            txOnGoing = true;
            FLEXIO_UART_TransferSendEDMA(&uartDev, &g_uartHandle, &sendXfer);
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
