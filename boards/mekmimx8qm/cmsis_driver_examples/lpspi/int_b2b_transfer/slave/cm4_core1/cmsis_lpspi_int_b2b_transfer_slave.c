/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_lpspi_cmsis.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_irqsteer.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*Master related*/
#define DRIVER_SLAVE_SPI Driver_SPI2
#define EXAMPLE_LPSPI_SLAVE_DMA_BASEADDR DMA__EDMA0
#define TRANSFER_SIZE 64U /*! Transfer dataSize */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* LPSPI user SlaveSignalEvent */
void LPSPI_SlaveSignalEvent_t(uint32_t event);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t slaveRxData[TRANSFER_SIZE] = {0U};

volatile bool isTransferCompleted = false;
volatile bool isSlaveOnTransmit   = false;
volatile bool isSlaveOnReceive    = false;
/*******************************************************************************
 * Code
 ******************************************************************************/

void LPSPI2_InitPins(void)
{
    sc_ipc_t ipc = SystemGetScfwIpcHandle();

    LPSPI2_InitPinsEx(ipc);
}

void LPSPI2_DeinitPins(void)
{
    sc_ipc_t ipc = SystemGetScfwIpcHandle();

    LPSPI2_DeinitPinsEx(ipc);
}

uint32_t LPSPI0_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_DMA_Lpspi0);
}

uint32_t LPSPI1_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_DMA_Lpspi1);
}

uint32_t LPSPI2_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_DMA_Lpspi2);
}

uint32_t LPSPI3_GetFreq(void)
{
    return CLOCK_GetIpFreq(kCLOCK_DMA_Lpspi3);
}
void LPSPI_SlaveSignalEvent_t(uint32_t event)
{
    /* user code */
    if (true == isSlaveOnReceive)
    {
        PRINTF("This is LPSPI_SlaveSignalEvent_t\r\n");
        PRINTF("Slave receive data from master has completed!\r\n");
        isSlaveOnReceive = false;
    }
    if (true == isSlaveOnTransmit)
    {
        PRINTF("This is LPSPI_SlaveSignalEvent_t\r\n");
        PRINTF("Slave transmit data to master has completed!\r\n");
        isSlaveOnTransmit = false;
    }
    isTransferCompleted = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    sc_ipc_t ipc;
    uint32_t freq;

    ipc = BOARD_InitRpc();

    BOARD_InitPins(ipc);
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    /* Power on SPI. */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_SPI_2, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on LPSPI\r\n");
    }

    /* Setup LPSPI clock */
    freq = CLOCK_SetIpFreq(kCLOCK_DMA_Lpspi2, SC_66MHZ);
    if (freq == 0)
    {
        PRINTF("Error: Failed to set LPSPI clock frequency\r\n");
    }

    /* Enable LPSPI interrupt in IRQSTEER */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_IRQSTR_M4_1, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on IRQSTEER!\r\n");
    }
    IRQSTEER_Init(IRQSTEER);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA_SPI2_INT_IRQn);

    PRINTF("LPSPI CMSIS driver board to board interrupt example.\r\n");

    uint32_t i;

    /*LPSPI slave init*/
    DRIVER_SLAVE_SPI.Initialize(LPSPI_SlaveSignalEvent_t);
    DRIVER_SLAVE_SPI.PowerControl(ARM_POWER_FULL);
    DRIVER_SLAVE_SPI.Control(ARM_SPI_MODE_SLAVE, false);

    while (1)
    {
        PRINTF("\r\n Slave example is running...\r\n");

        /* Reset the receive buffer */
        for (i = 0; i < TRANSFER_SIZE; i++)
        {
            slaveRxData[i] = 0U;
        }
        isTransferCompleted = false;
        isSlaveOnReceive    = true;
        /* Set slave transfer to receive data */
        DRIVER_SLAVE_SPI.Receive(slaveRxData, TRANSFER_SIZE);

        while (!isTransferCompleted)
        {
        }

        isTransferCompleted = false;
        isSlaveOnTransmit   = true;
        /* Set slave transfer to send back data */
        DRIVER_SLAVE_SPI.Send(slaveRxData, TRANSFER_SIZE);

        while (!isTransferCompleted)
        {
        }
        /* Print out receive buffer */
        PRINTF("\r\n Slave receive:");
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            /* Print 16 numbers in a line */
            if ((i & 0x0FU) == 0U)
            {
                PRINTF("\r\n    ");
            }
            PRINTF(" %02X", slaveRxData[i]);
        }
        PRINTF("\r\n");
    }
}
