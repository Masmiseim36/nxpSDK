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

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_UART UART1
#define DEMO_UART_CLK_FREQ BOARD_DebugConsoleSrcFreq()
#define DEMO_IRQn UART1_IRQn
#define DEMO_UART_BAUDRATE 115200U

/*! @brief Ring buffer size (Unit: Byte). */
#define DEMO_RING_BUFFER_SIZE 16

/*! @brief Ring buffer to save received data. */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_tipString[] =
    "Uart functional API interrupt example\r\nBoard receives characters then sends them out\r\nNow please input:\r\n";

/*
  Ring buffer for data input and output, in this example, input data are saved
  to ring buffer in IRQ handler. The main function polls the ring buffer status,
  if there are new data, then send them out.
  Ring buffer full: (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) == txIndex)
  Ring buffer empty: (rxIndex == txIndex)
*/
uint8_t demoRingBuffer[DEMO_RING_BUFFER_SIZE];
volatile uint16_t txIndex; /* Index of the data to send out. */
volatile uint16_t rxIndex; /* Index of the memory to save new arrived data. */

/*******************************************************************************
 * Code
 ******************************************************************************/


void DEMO_UART_IRQHandler(void)
{
    uint8_t data;

    /* If new data arrived. */
    if ((UART_GetStatusFlag(DEMO_UART, kUART_RxDataReadyFlag)) || (UART_GetStatusFlag(DEMO_UART, kUART_RxOverrunFlag)))
    {
        data = UART_ReadByte(DEMO_UART);

        /* If ring buffer is not full, add data to ring buffer. */
        if (((rxIndex + 1) % DEMO_RING_BUFFER_SIZE) != txIndex)
        {
            demoRingBuffer[rxIndex] = data;
            rxIndex++;
            rxIndex %= DEMO_RING_BUFFER_SIZE;
        }
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    uart_config_t config;

    SystemInitIrqTable();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    SystemInstallIrqHandler(DEMO_IRQn, (system_irq_handler_t)DEMO_UART_IRQHandler, NULL);

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
    config.baudRate_Bps = DEMO_UART_BAUDRATE;
    config.enableTx = true;
    config.enableRx = true;

    UART_Init(DEMO_UART, &config, DEMO_UART_CLK_FREQ);

    /* Send g_tipString out. */
    UART_WriteBlocking(DEMO_UART, g_tipString, sizeof(g_tipString) / sizeof(g_tipString[0]) - 1);

    /* Enable RX interrupt. */
    UART_EnableInterrupts(DEMO_UART, kUART_RxDataReadyEnable | kUART_RxOverrunEnable);
    EnableIRQ(DEMO_IRQn);

    while (1)
    {
        /* Send data only when UART TX register is empty and ring buffer has data to send out. */
        while ((UART_GetStatusFlag(DEMO_UART, kUART_TxReadyFlag)) && (rxIndex != txIndex))
        {
            UART_WriteByte(DEMO_UART, demoRingBuffer[txIndex]);
            txIndex++;
            txIndex %= DEMO_RING_BUFFER_SIZE;
        }
    }
}
