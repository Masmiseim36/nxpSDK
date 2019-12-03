/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_irqsteer.h"
#include "fsl_lpuart_cmsis.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_USART Driver_USART1
#define ECHO_BUFFER_LENGTH 8
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* USART user SignalEvent */
void USART_SignalEvent_t(uint32_t event);
/*******************************************************************************
 * Variables
 ******************************************************************************/
const uint8_t g_tipString[] =
    "USART CMSIS interrupt example\r\nBoard receives 8 characters then sends them out\r\nNow please input:\r\n";
uint8_t g_txBuffer[ECHO_BUFFER_LENGTH] = {0};
uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH] = {0};
volatile bool rxBufferEmpty            = true;
volatile bool txBufferFull             = false;
volatile bool txOnGoing                = false;
volatile bool rxOnGoing                = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void LPUART1_InitPins(void)
{
    sc_ipc_t ipc;

    ipc = BOARD_InitRpc();
    LPUART_InitPins(ipc);
}

void LPUART1_DeinitPins(void)
{
    sc_ipc_t ipc;

    ipc = BOARD_InitRpc();
    LPUART_DeinitPins(ipc);
}

uint32_t LPUART0_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_M4_0_Lpuart);
}

uint32_t LPUART1_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_DMA_Lpuart0);
}

uint32_t LPUART2_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_DMA_Lpuart1);
}

uint32_t LPUART3_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_DMA_Lpuart2);
}

uint32_t LPUART4_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_DMA_Lpuart3);
}
void USART_SignalEvent_t(uint32_t event)
{
    if (ARM_USART_EVENT_SEND_COMPLETE == event)
    {
        txBufferFull = false;
        txOnGoing    = false;
    }

    if (ARM_USART_EVENT_RECEIVE_COMPLETE == event)
    {
        rxBufferEmpty = false;
        rxOnGoing     = false;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t i;

    sc_ipc_t ipc;
    uint32_t freq;

    ipc = BOARD_InitRpc();
    BOARD_BootClockRUN();
    BOARD_InitMemory();

    /* Power on LPUART. */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_UART_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        assert(false);
    }

    /* Set UART clock Frequncy */
    freq = CLOCK_SetIpFreq(kCLOCK_DMA_Lpuart0, SC_133MHZ);
    if (freq == 0)
    {
        assert(freq);
    }

    /* Power on IRQSteer . */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_IRQSTR_M4_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on IRQSTR\r\n");
    }

    /* Enable interrupt in irqsteer */
    IRQSTEER_Init(IRQSTEER);
    IRQSTEER_EnableInterrupt(IRQSTEER, ADMA_UART0_INT_IRQn);

    DEMO_USART.Initialize(USART_SignalEvent_t);
    DEMO_USART.PowerControl(ARM_POWER_FULL);

    /* Set baudrate. */
    DEMO_USART.Control(ARM_USART_MODE_ASYNCHRONOUS, BOARD_DEBUG_UART_BAUDRATE);

    txOnGoing = true;
    DEMO_USART.Send(g_tipString, sizeof(g_tipString) - 1);

    /* Wait send finished */
    while (txOnGoing)
    {
    }

    while (1)
    {
        /* If g_txBuffer is empty and g_rxBuffer is full, copy g_rxBuffer to g_txBuffer. */
        if ((!rxBufferEmpty) && (!txBufferFull))
        {
            memcpy(g_txBuffer, g_rxBuffer, ECHO_BUFFER_LENGTH);
            rxBufferEmpty = true;
            txBufferFull  = true;
        }
        /* If g_txBuffer is empty and g_rxBuffer is full, copy g_rxBuffer to g_txBuffer. */
        if ((!rxBufferEmpty) && (!txBufferFull))
        {
            memcpy(g_txBuffer, g_rxBuffer, ECHO_BUFFER_LENGTH);
            rxBufferEmpty = true;
            txBufferFull  = true;
        }

        /* If RX is idle and g_rxBuffer is empty, start to read data to g_rxBuffer. */
        if ((!rxOnGoing) && rxBufferEmpty)
        {
            rxOnGoing = true;
            DEMO_USART.Receive(g_rxBuffer, ECHO_BUFFER_LENGTH);
        }

        /* If TX is idle and g_txBuffer is full, start to send data. */
        if ((!txOnGoing) && txBufferFull)
        {
            txOnGoing = true;
            DEMO_USART.Send(g_txBuffer, ECHO_BUFFER_LENGTH);
        }

        /* Delay some time, simulate the app is processing other things, input data save to ring buffer. */
        i = 0x10000U;
        while (i--)
        {
            __NOP();
        }
    }
}
