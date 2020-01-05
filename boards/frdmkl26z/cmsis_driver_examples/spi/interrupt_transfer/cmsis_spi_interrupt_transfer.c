/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
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

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_spi.h"
#include "board.h"
#include "fsl_spi_cmsis.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define EXAMPLE_SPI_MASTER_IRQ SPI0_IRQn
#define EXAMPLE_SPI_SLAVE_IRQ SPI1_IRQn
#define DRIVER_MASTER_SPI Driver_SPI0
#define DRIVER_SLAVE_SPI Driver_SPI1
#define TRANSFER_SIZE 256U        /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* SPI user SignalEvent */
void SPI_MasterSignalEvent_t(uint32_t event);
void SPI_SlaveSignalEvent_t(uint32_t event);
/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t masterRxData[TRANSFER_SIZE] = {0U};
uint8_t masterTxData[TRANSFER_SIZE] = {0U};
uint8_t slaveRxData[TRANSFER_SIZE] = {0U};
uint8_t slaveTxData[TRANSFER_SIZE] = {0U};

volatile bool isTransferCompleted = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
uint32_t SPI0_GetFreq(void)
{
    return CLOCK_GetBusClkFreq();
}
uint32_t SPI1_GetFreq(void)
{
    return CLOCK_GetBusClkFreq();
}
void SPI_MasterSignalEvent_t(uint32_t event)
{
    isTransferCompleted = true;
    PRINTF("This is SPI_MasterSignalEvent_t\r\n"); /* user code */
}
void SPI_SlaveSignalEvent_t(uint32_t event)
{
    /* user code */
    isTransferCompleted = true;
    PRINTF("This is SPI_SlaveSignalEvent_t\r\n");
}
/*!
 * @brief Main function
 */
int main(void)
{
    uint32_t errorCount;
    uint32_t i;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("SPI CMSIS interrupt transfer start.\r\n");
    PRINTF("This example use one spi instance as master and another as slave on one board.\r\n");
    PRINTF("Master and slave are both use interrupt way.\r\n");
    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("SPI_master -- SPI_slave   \r\n");
    PRINTF("   CLK      --    CLK  \r\n");
    PRINTF("   PCS      --    PCS  \r\n");
    PRINTF("   MOSI     --    MOSI \r\n");
    PRINTF("   MISO     --    MISO \r\n");

    /*SPI master init*/
    DRIVER_MASTER_SPI.Initialize(SPI_MasterSignalEvent_t);
    DRIVER_MASTER_SPI.PowerControl(ARM_POWER_FULL);
    DRIVER_MASTER_SPI.Control(ARM_SPI_MODE_MASTER | ARM_SPI_SS_MASTER_HW_OUTPUT, TRANSFER_BAUDRATE);

    /*SPI slave init*/
    DRIVER_SLAVE_SPI.Initialize(SPI_SlaveSignalEvent_t);
    DRIVER_SLAVE_SPI.PowerControl(ARM_POWER_FULL);
    DRIVER_SLAVE_SPI.Control(ARM_SPI_MODE_SLAVE, false);

    NVIC_SetPriority(EXAMPLE_SPI_MASTER_IRQ, 1U);
    NVIC_SetPriority(EXAMPLE_SPI_SLAVE_IRQ, 0U);
    /* Set up the transfer data */
    for (i = 0U; i < TRANSFER_SIZE; i++)
    {
        masterTxData[i] = i % 256U;
        masterRxData[i] = 0U;

        slaveTxData[i] = ~masterTxData[i];
        slaveRxData[i] = 0U;
    }

    isTransferCompleted = false;
    /* Start slave transfer */
    DRIVER_SLAVE_SPI.Transfer(slaveTxData, slaveRxData, TRANSFER_SIZE);

    /* Start master transfer */
    DRIVER_MASTER_SPI.Transfer(masterTxData, masterRxData, TRANSFER_SIZE);

    /* Wait slave received all data. */
    while (!isTransferCompleted)
    {
    }

    errorCount = 0U;
    for (i = 0U; i < TRANSFER_SIZE; i++)
    {
        if (masterTxData[i] != slaveRxData[i])
        {
            errorCount++;
        }

        if (slaveTxData[i] != masterRxData[i])
        {
            errorCount++;
        }
    }
    if (errorCount == 0U)
    {
        PRINTF(" \r\nSPI transfer all data matched! \r\n");
    }
    else
    {
        PRINTF(" \r\nError occured in SPI transfer ! \r\n");
    }

    DRIVER_MASTER_SPI.PowerControl(ARM_POWER_OFF);
    DRIVER_SLAVE_SPI.PowerControl(ARM_POWER_OFF);
    DRIVER_MASTER_SPI.Uninitialize();
    DRIVER_SLAVE_SPI.Uninitialize();

    while (1)
    {
    }
}
