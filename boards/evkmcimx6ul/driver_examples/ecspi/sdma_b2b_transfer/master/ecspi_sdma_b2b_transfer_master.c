/*
 * Copyright (c) 2017, Freescale Semiconductor, Inc.
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
#include "fsl_ecspi.h"
#include "fsl_sdma.h"
#include "fsl_ecspi_sdma.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_ECSPI_MASTER_BASEADDR ECSPI4
#define EXAMPLE_ECSPI_DEALY_COUNT 1000000
#define ECSPI_MASTER_CLK_FREQ (CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 8U)
#define EXAMPLE_DMA SDMAARM
#define EXAMPLE_CHANNEL_RX 2
#define EXAMPLE_CHANNEL_TX 3
#define ECSPI_TX_DMA_REQUEST 10
#define ECSPI_RX_DMA_REQUEST 9
#define TRANSFER_SIZE 64U         /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* ECSPI user callback */
void ECSPI_MasterUserCallback(ECSPI_Type *base, ecspi_sdma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t masterRxData[TRANSFER_SIZE], 4) = {0U};
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t masterTxData[TRANSFER_SIZE], 4) = {0U};

AT_NONCACHEABLE_SECTION_ALIGN(ecspi_sdma_handle_t g_ecspiSdmaHandle, 4) = {0};
AT_NONCACHEABLE_SECTION_ALIGN(sdma_handle_t g_ecspiTxSdmaHandle, 4) = {0};
AT_NONCACHEABLE_SECTION_ALIGN(sdma_handle_t g_ecspiRxSdmaHandle, 4) = {0};

AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t contextTx, 4) = {0};
AT_NONCACHEABLE_SECTION_ALIGN(sdma_context_data_t contextRx, 4) = {0};
volatile bool isTransferCompleted = false;
uint8_t dummy_buffer[TRANSFER_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/
void ECSPI_MasterUserCallback(ECSPI_Type *base, ecspi_sdma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        isTransferCompleted = true;
    }

    if (status == kStatus_ECSPI_HardwareOverFlow)
    {
        PRINTF("Hardware overflow occured in this transfer. \r\n");
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    SystemInitIrqTable();
    /* Enable SPBA */
    CLOCK_EnableClock(kCLOCK_Spba);

    CLOCK_SetMux(kCLOCK_EcspiMux, 0); /* Set ECSPI source to PLL3 60M */
    CLOCK_SetDiv(kCLOCK_EcspiDiv, 0); /* Set ECSPI divider to 1 */

    PRINTF("ECSPI board to board sdma example.\r\n");
    PRINTF("This example use one board as master and another as slave.\r\n");
    PRINTF("Master and slave uses sdma way. Slave should start first. \r\n");
    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("ECSPI_master -- ECSPI_slave   \r\n");
    PRINTF("   CLK      --    CLK  \r\n");
    PRINTF("   PCS      --    PCS \r\n");
    PRINTF("   MOSI     --    MOSI \r\n");
    PRINTF("   MISO     --    MISO \r\n");
    PRINTF("   GND      --    GND \r\n");

    uint32_t errorCount;
    uint32_t loopCount = 1;
    uint32_t i;
    ecspi_master_config_t masterConfig;
    ecspi_transfer_t masterXfer;
    sdma_config_t sdmaConfig;
    uint8_t dummy_count;

    for (dummy_count = 0; dummy_count < TRANSFER_SIZE; dummy_count++)
    {
        dummy_buffer[dummy_count] = (uint8_t)ECSPI_DUMMYDATA;
    }

    SDMA_GetDefaultConfig(&sdmaConfig);
    SDMA_Init(EXAMPLE_DMA, &sdmaConfig);

    SDMA_CreateHandle(&g_ecspiRxSdmaHandle, EXAMPLE_DMA, EXAMPLE_CHANNEL_RX, &contextRx);
    SDMA_CreateHandle(&g_ecspiTxSdmaHandle, EXAMPLE_DMA, EXAMPLE_CHANNEL_TX, &contextTx);

    /* Master config:
    * masterConfig.channel = kECSPI_Channel0;
    * masterConfig.burstLength = 8;
    * masterConfig.samplePeriodClock = kECSPI_spiClock;
    * masterConfig.baudRate_Bps = TRANSFER_BAUDRATE;
    * masterConfig.chipSelectDelay = 0;
    * masterConfig.samplePeriod = 0;
    * masterConfig.txFifoThreshold = 1;
    * masterConfig.rxFifoThreshold = 0;
    */
    ECSPI_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = TRANSFER_BAUDRATE;

    ECSPI_MasterInit(EXAMPLE_ECSPI_MASTER_BASEADDR, &masterConfig, ECSPI_MASTER_CLK_FREQ);

    /* Create ECSPI SDMA handle. */
    ECSPI_MasterTransferCreateHandleSDMA(EXAMPLE_ECSPI_MASTER_BASEADDR, &g_ecspiSdmaHandle, ECSPI_MasterUserCallback,
                                         NULL, &g_ecspiTxSdmaHandle, &g_ecspiRxSdmaHandle, ECSPI_TX_DMA_REQUEST,
                                         ECSPI_RX_DMA_REQUEST, EXAMPLE_CHANNEL_TX, EXAMPLE_CHANNEL_RX);

    while (1)
    {
        /* Set up the transfer data */
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            masterTxData[i] = (i + loopCount) % 256U;
            masterRxData[i] = 0U;
        }

        /* Print out transmit buffer */
        PRINTF("\r\n Master transmit:\r\n");
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            /* Print 16 numbers in a line */
            if ((i & 0x0FU) == 0U)
            {
                PRINTF("\r\n");
            }
            PRINTF(" %02X", masterTxData[i]);
        }
        PRINTF("\r\n");

        /* Start master transfer, send data to slave */
        masterXfer.txData = (uint32_t *)masterTxData;
        masterXfer.rxData = (uint32_t *)dummy_buffer;
        masterXfer.dataSize = TRANSFER_SIZE;
        masterXfer.channel = kECSPI_Channel0;
        isTransferCompleted = false;
        ECSPI_MasterTransferSDMA(EXAMPLE_ECSPI_MASTER_BASEADDR, &g_ecspiSdmaHandle, &masterXfer);
        SDMA_SetChannelPriority(EXAMPLE_DMA, EXAMPLE_CHANNEL_TX, 3);
        SDMA_SetChannelPriority(EXAMPLE_DMA, EXAMPLE_CHANNEL_RX, 3);
        /* Wait transfer complete */
        while (!isTransferCompleted)
        {
        }

        /* Delay to wait slave is ready */
        for (i = 0; i < EXAMPLE_ECSPI_DEALY_COUNT; i++)
        {
            __NOP();
        }

        /* Start master transfer, receive data from slave */
        masterXfer.txData = (uint32_t *)dummy_buffer;
        masterXfer.rxData = (uint32_t *)masterRxData;
        masterXfer.dataSize = TRANSFER_SIZE;
        masterXfer.channel = kECSPI_Channel0;
        isTransferCompleted = false;
        ECSPI_MasterTransferSDMA(EXAMPLE_ECSPI_MASTER_BASEADDR, &g_ecspiSdmaHandle, &masterXfer);
        SDMA_SetChannelPriority(EXAMPLE_DMA, EXAMPLE_CHANNEL_TX, 3);
        SDMA_SetChannelPriority(EXAMPLE_DMA, EXAMPLE_CHANNEL_RX, 3);

        while (!isTransferCompleted)
        {
        }

        errorCount = 0U;
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            if (masterTxData[i] != masterRxData[i])
            {
                errorCount++;
            }
        }
        if (errorCount == 0U)
        {
            PRINTF(" \r\nECSPI transfer all data matched! \r\n");
            /* Print out receive buffer */
            PRINTF("\r\n Master received:\r\n");
            for (i = 0U; i < TRANSFER_SIZE; i++)
            {
                /* Print 16 numbers in a line */
                if ((i & 0x0FU) == 0U)
                {
                    PRINTF("\r\n");
                }
                PRINTF(" %02X", masterRxData[i]);
            }
            PRINTF("\r\n");
        }
        else
        {
            PRINTF(" \r\nError occured in ECSPI transfer ! \r\n");
            /* Print out receive buffer */
            PRINTF("\r\n Master received:\r\n");
            for (i = 0U; i < TRANSFER_SIZE; i++)
            {
                /* Print 16 numbers in a line */
                if ((i & 0x0FU) == 0U)
                {
                    PRINTF("\r\n");
                }
                PRINTF(" %02X", masterRxData[i]);
            }
            PRINTF("\r\n");
        }

        /* Wait for press any key */
        PRINTF("\r\n Press any key to run again\r\n");
        GETCHAR();

        /* Increase loop count to change transmit buffer */
        loopCount++;
    }
}
