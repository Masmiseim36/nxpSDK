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

#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_dspi.h"
#include "board.h"
#include "fsl_dspi_edma.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_DSPI_MASTER_BASEADDR SPI0
#define EXAMPLE_DSPI_MASTER_DMA_MUX_BASEADDR DMAMUX
#define EXAMPLE_DSPI_MASTER_DMA_BASEADDR DMA0
#define EXAMPLE_DSPI_MASTER_DMA_RX_REQUEST_SOURCE kDmaRequestMux0SPI0Rx
#define EXAMPLE_DSPI_MASTER_DMA_TX_REQUEST_SOURCE kDmaRequestMux0SPI0Tx
#define DSPI_MASTER_CLK_SRC DSPI0_CLK_SRC
#define DSPI_MASTER_CLK_FREQ CLOCK_GetFreq(DSPI0_CLK_SRC)
#define EXAMPLE_DSPI_MASTER_PCS_FOR_INIT kDSPI_Pcs0
#define EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER kDSPI_MasterPcs0
#define EXAMPLE_DSPI_DEALY_COUNT 0xfffffU
#define TRANSFER_SIZE 64U         /* Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /* Transfer baudrate - 500k */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* DSPI user callback */
void DSPI_MasterUserCallback(SPI_Type *base, dspi_master_edma_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t masterRxData[TRANSFER_SIZE] = {0};
uint8_t masterTxData[TRANSFER_SIZE] = {0};

dspi_master_edma_handle_t g_dspi_edma_m_handle;
edma_handle_t dspiEdmaMasterRxRegToRxDataHandle;
edma_handle_t dspiEdmaMasterTxDataToIntermediaryHandle;
edma_handle_t dspiEdmaMasterIntermediaryToTxRegHandle;

volatile bool isTransferCompleted = false;
/*******************************************************************************
 * Code
 ******************************************************************************/

void DSPI_MasterUserCallback(SPI_Type *base, dspi_master_edma_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        PRINTF("This is DSPI master edma transfer completed callback. \r\n\r\n");
    }

    isTransferCompleted = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("DSPI board to board edma example.\r\n");
    PRINTF("This example use one board as master and another as slave.\r\n");
    PRINTF("Master and slave uses EDMA way. Slave should start first. \r\n");
    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("DSPI_master -- DSPI_slave   \r\n");
    PRINTF("   CLK      --    CLK  \r\n");
    PRINTF("   PCS      --    PCS \r\n");
    PRINTF("   SOUT     --    SIN  \r\n");
    PRINTF("   SIN      --    SOUT \r\n");
    PRINTF("   GND      --    GND \r\n");

    /* DMA Mux setting and EDMA init */
    uint32_t masterRxChannel, masterIntermediaryChannel, masterTxChannel;
    edma_config_t userConfig;

    masterRxChannel = 0U;
    masterIntermediaryChannel = 1U;
    masterTxChannel = 2U;

    /* DMA MUX init */
    DMAMUX_Init(EXAMPLE_DSPI_MASTER_DMA_MUX_BASEADDR);

    DMAMUX_SetSource(EXAMPLE_DSPI_MASTER_DMA_MUX_BASEADDR, masterRxChannel,(uint8_t)EXAMPLE_DSPI_MASTER_DMA_RX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DSPI_MASTER_DMA_MUX_BASEADDR, masterRxChannel);

#if (defined EXAMPLE_DSPI_MASTER_DMA_TX_REQUEST_SOURCE)
    DMAMUX_SetSource(EXAMPLE_DSPI_MASTER_DMA_MUX_BASEADDR, masterTxChannel,(uint8_t)EXAMPLE_DSPI_MASTER_DMA_TX_REQUEST_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DSPI_MASTER_DMA_MUX_BASEADDR, masterTxChannel);
#endif

    /* EDMA init */
    /*
     * userConfig.enableRoundRobinArbitration = false;
     * userConfig.enableHaltOnError = true;
     * userConfig.enableContinuousLinkMode = false;
     * userConfig.enableDebugMode = false;
     */
    EDMA_GetDefaultConfig(&userConfig);

    EDMA_Init(EXAMPLE_DSPI_MASTER_DMA_BASEADDR, &userConfig);

    uint32_t srcClock_Hz;
    uint32_t errorCount;
    uint32_t loopCount = 1U;
    uint32_t i;
    dspi_master_config_t masterConfig;
    dspi_transfer_t masterXfer;

    /* Master config */
    masterConfig.whichCtar = kDSPI_Ctar0;
    masterConfig.ctarConfig.baudRate = TRANSFER_BAUDRATE;
    masterConfig.ctarConfig.bitsPerFrame = 8;
    masterConfig.ctarConfig.cpol = kDSPI_ClockPolarityActiveHigh;
    masterConfig.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
    masterConfig.ctarConfig.direction = kDSPI_MsbFirst;
    masterConfig.ctarConfig.pcsToSckDelayInNanoSec = 1000000000U / TRANSFER_BAUDRATE;
    masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 1000000000U / TRANSFER_BAUDRATE;
    masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 1000000000U / TRANSFER_BAUDRATE;

    masterConfig.whichPcs = EXAMPLE_DSPI_MASTER_PCS_FOR_INIT;
    masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

    masterConfig.enableContinuousSCK = false;
    masterConfig.enableRxFifoOverWrite = false;
    masterConfig.enableModifiedTimingFormat = false;
    masterConfig.samplePoint = kDSPI_SckToSin0Clock;

    srcClock_Hz = DSPI_MASTER_CLK_FREQ;
    DSPI_MasterInit(EXAMPLE_DSPI_MASTER_BASEADDR, &masterConfig, srcClock_Hz);

    /* Set up dspi master */
    memset(&(dspiEdmaMasterRxRegToRxDataHandle), 0, sizeof(dspiEdmaMasterRxRegToRxDataHandle));
    memset(&(dspiEdmaMasterTxDataToIntermediaryHandle), 0, sizeof(dspiEdmaMasterTxDataToIntermediaryHandle));
    memset(&(dspiEdmaMasterIntermediaryToTxRegHandle), 0, sizeof(dspiEdmaMasterIntermediaryToTxRegHandle));

    EDMA_CreateHandle(&(dspiEdmaMasterRxRegToRxDataHandle), EXAMPLE_DSPI_MASTER_DMA_BASEADDR, masterRxChannel);
    EDMA_CreateHandle(&(dspiEdmaMasterTxDataToIntermediaryHandle), EXAMPLE_DSPI_MASTER_DMA_BASEADDR,
                      masterIntermediaryChannel);
    EDMA_CreateHandle(&(dspiEdmaMasterIntermediaryToTxRegHandle), EXAMPLE_DSPI_MASTER_DMA_BASEADDR, masterTxChannel);

    DSPI_MasterTransferCreateHandleEDMA(EXAMPLE_DSPI_MASTER_BASEADDR, &g_dspi_edma_m_handle, DSPI_MasterUserCallback,
                                        NULL, &dspiEdmaMasterRxRegToRxDataHandle,
                                        &dspiEdmaMasterTxDataToIntermediaryHandle,
                                        &dspiEdmaMasterIntermediaryToTxRegHandle);

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
        for (i = 0; i < TRANSFER_SIZE; i++)
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
        isTransferCompleted = false;
        masterXfer.txData = masterTxData;
        masterXfer.rxData = NULL;
        masterXfer.dataSize = TRANSFER_SIZE;
        masterXfer.configFlags = kDSPI_MasterCtar0 | EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER | kDSPI_MasterPcsContinuous;
        if (kStatus_Success !=
            DSPI_MasterTransferEDMA(EXAMPLE_DSPI_MASTER_BASEADDR, &g_dspi_edma_m_handle, &masterXfer))
        {
            PRINTF("There is error when start DSPI_MasterTransferEDMA \r\n ");
        }

        /* Wait until transfer completed */
        while (!isTransferCompleted)
        {
        }

        /* Delay to wait slave is ready */
        for (i = 0U; i < EXAMPLE_DSPI_DEALY_COUNT; i++)
        {
            __NOP();
        }

        /* Start master transfer, receive data from slave */
        isTransferCompleted = false;
        masterXfer.txData = NULL;
        masterXfer.rxData = masterRxData;
        masterXfer.dataSize = TRANSFER_SIZE;
        masterXfer.configFlags = kDSPI_MasterCtar0 | EXAMPLE_DSPI_MASTER_PCS_FOR_TRANSFER | kDSPI_MasterPcsContinuous;
        if (kStatus_Success !=
            DSPI_MasterTransferEDMA(EXAMPLE_DSPI_MASTER_BASEADDR, &g_dspi_edma_m_handle, &masterXfer))
        {
            PRINTF("There is error when start DSPI_MasterTransferEDMA \r\n ");
        }

        /* Wait until transfer completed */
        while (!isTransferCompleted)
        {
        }

        errorCount = 0;
        for (i = 0; i < TRANSFER_SIZE; i++)
        {
            if (masterTxData[i] != masterRxData[i])
            {
                errorCount++;
            }
        }
        if (errorCount == 0)
        {
            PRINTF(" \r\nDSPI transfer all data matched! \r\n");
            /* Print out receive buffer */
            PRINTF("\r\n Master received:\r\n");
            for (i = 0; i < TRANSFER_SIZE; i++)
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
            PRINTF(" \r\nError occured in DSPI transfer ! \r\n");
        }

        /* Wait for press any key */
        PRINTF("\r\n Press any key to run again\r\n");
        GETCHAR();
        /* Increase loop count to change transmit buffer */
        loopCount++;
    }
}
