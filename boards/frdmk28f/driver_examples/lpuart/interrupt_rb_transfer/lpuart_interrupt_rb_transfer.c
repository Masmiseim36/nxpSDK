/*
 * The Clear BSD License
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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
#include "fsl_lpuart.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_LPUART LPUART0
#define DEMO_LPUART_CLKSRC kCLOCK_Osc0ErClk
#define DEMO_LPUART_CLK_FREQ CLOCK_GetFreq(kCLOCK_Osc0ErClk)

#define RX_RING_BUFFER_SIZE 20U
#define ECHO_BUFFER_SIZE 8U
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* LPUART user callback */
void LPUART_UserCallback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
lpuart_handle_t g_lpuartHandle;
uint8_t g_tipString[] = "LPUART RX ring buffer example\r\nSend back received data\r\nEcho every 8 types\r\n";
uint8_t g_rxRingBuffer[RX_RING_BUFFER_SIZE] = {0}; /* RX ring buffer. */

uint8_t g_rxBuffer[ECHO_BUFFER_SIZE] = {0}; /* Buffer for receive data to echo. */
uint8_t g_txBuffer[ECHO_BUFFER_SIZE] = {0}; /* Buffer for send data to echo. */
volatile bool rxBufferEmpty = true;
volatile bool txBufferFull = false;
volatile bool txOnGoing = false;
volatile bool rxOnGoing = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

/* LPUART user callback */
void LPUART_UserCallback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_LPUART_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing = false;
    }

    if (kStatus_LPUART_RxIdle == status)
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
    lpuart_config_t config;
    lpuart_transfer_t xfer;
    lpuart_transfer_t sendXfer;
    lpuart_transfer_t receiveXfer;
    size_t receivedBytes = 0U;
    uint32_t i;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    CLOCK_SetLpuartClock(2U);

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kLPUART_ParityDisabled;
     * config.stopBitCount = kLPUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 0;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableTx = true;
    config.enableRx = true;

    LPUART_Init(DEMO_LPUART, &config, DEMO_LPUART_CLK_FREQ);
    LPUART_TransferCreateHandle(DEMO_LPUART, &g_lpuartHandle, LPUART_UserCallback, NULL);
    LPUART_TransferStartRingBuffer(DEMO_LPUART, &g_lpuartHandle, g_rxRingBuffer, RX_RING_BUFFER_SIZE);

    /* Send g_tipString out. */
    xfer.data = g_tipString;
    xfer.dataSize = sizeof(g_tipString) - 1;
    txOnGoing = true;
    LPUART_TransferSendNonBlocking(DEMO_LPUART, &g_lpuartHandle, &xfer);

    /* Wait send finished */
    while (txOnGoing)
    {
    }

    /* Start to echo. */
    sendXfer.data = g_txBuffer;
    sendXfer.dataSize = ECHO_BUFFER_SIZE;
    receiveXfer.data = g_rxBuffer;
    receiveXfer.dataSize = ECHO_BUFFER_SIZE;

    while (1)
    {
        /* If g_txBuffer is empty and g_rxBuffer is full, copy g_rxBuffer to g_txBuffer. */
        if ((!rxBufferEmpty) && (!txBufferFull))
        {
            memcpy(g_txBuffer, g_rxBuffer, ECHO_BUFFER_SIZE);
            rxBufferEmpty = true;
            txBufferFull = true;
        }

        /* If RX is idle and g_rxBuffer is empty, start to read data to g_rxBuffer. */
        if ((!rxOnGoing) && rxBufferEmpty)
        {
            rxOnGoing = true;
            LPUART_TransferReceiveNonBlocking(DEMO_LPUART, &g_lpuartHandle, &receiveXfer, &receivedBytes);
            if (ECHO_BUFFER_SIZE == receivedBytes)
            {
                rxBufferEmpty = false;
                rxOnGoing = false;
            }
        }

        /* If TX is idle and g_txBuffer is full, start to send data. */
        if ((!txOnGoing) && txBufferFull)
        {
            txOnGoing = true;
            LPUART_TransferSendNonBlocking(DEMO_LPUART, &g_lpuartHandle, &sendXfer);
        }

        /* Delay some time, simulate the app is processing other things, input data save to ring buffer. */
        i = 0x10U;
        while (i--)
        {
            __NOP();
        }
    }
}
