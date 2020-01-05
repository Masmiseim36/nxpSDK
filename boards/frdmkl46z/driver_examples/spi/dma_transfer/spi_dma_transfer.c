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

#include "fsl_spi_dma.h"
#include "fsl_dmamux.h"
#include "board.h"
#include "fsl_debug_console.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_SPI_MASTER SPI0
#define EXAMPLE_SPI_SLAVE SPI1
#define EXAMPLE_DMA DMA0
#define EXAMPLE_DMAMUX DMAMUX0
#define EXAMPLE_SPI_MASTER_TX_CHANNEL 0U
#define EXAMPLE_SPI_MASTER_RX_CHANNEL 1U
#define EXAMPLE_SPI_SLAVE_TX_CHANNEL 2U
#define EXAMPLE_SPI_SLAVE_RX_CHANNEL 3U
#define EXAMPLE_DMA_SPI_MASTER_TX_SOURCE kDmaRequestMux0SPI0Tx
#define EXAMPLE_DMA_SPI_MASTER_RX_SOURCE kDmaRequestMux0SPI0Rx
#define EXAMPLE_DMA_SPI_SLAVE_TX_SOURCE kDmaRequestMux0SPI1Tx
#define EXAMPLE_DMA_SPI_SLAVE_RX_SOURCE kDmaRequestMux0SPI1Rx
#define EXAMPLE_SPI_MASTER_SOURCE_CLOCK kCLOCK_BusClk
#define EXAMPLE_SPI_MASTER_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)

#define BUFFER_SIZE (64)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint8_t srcBuff[BUFFER_SIZE];
static uint8_t destBuff[BUFFER_SIZE];
static spi_dma_handle_t masterHandle;
static spi_dma_handle_t slaveHandle;
static dma_handle_t masterTxHandle;
static dma_handle_t masterRxHandle;
static dma_handle_t slaveTxHandle;
static dma_handle_t slaveRxHandle;
static volatile bool masterFinished = false;
static volatile bool slaveFinished = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void masterCallback(SPI_Type *base, spi_dma_handle_t *handle, status_t status, void *userData)
{
    masterFinished = true;
}

static void slaveCallback(SPI_Type *base, spi_dma_handle_t *handle, status_t status, void *userData)
{
    slaveFinished = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    spi_master_config_t masterConfig = {0};
    spi_slave_config_t slaveConfig = {0};
    spi_transfer_t xfer = {0};
    uint32_t sourceClock = 0;
    uint32_t i = 0;
    uint32_t err = 0;

    /* Init the boards */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\nSPI one board dma example started!\r\n");

    /* Init DMAMUX */
    DMAMUX_Init(EXAMPLE_DMAMUX);
    /* Set channel for SPI master */
    DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_SPI_MASTER_TX_CHANNEL, EXAMPLE_DMA_SPI_MASTER_TX_SOURCE);
    DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_SPI_MASTER_RX_CHANNEL, EXAMPLE_DMA_SPI_MASTER_RX_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_SPI_MASTER_TX_CHANNEL);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_SPI_MASTER_RX_CHANNEL);

    /* Set channel for SPI slave */
    DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_SPI_SLAVE_TX_CHANNEL, EXAMPLE_DMA_SPI_SLAVE_TX_SOURCE);
    DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_SPI_SLAVE_RX_CHANNEL, EXAMPLE_DMA_SPI_SLAVE_RX_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_SPI_SLAVE_TX_CHANNEL);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_SPI_SLAVE_RX_CHANNEL);

    /* Init the DMA module */
    DMA_Init(EXAMPLE_DMA);
    DMA_CreateHandle(&masterTxHandle, EXAMPLE_DMA, EXAMPLE_SPI_MASTER_TX_CHANNEL);
    DMA_CreateHandle(&masterRxHandle, EXAMPLE_DMA, EXAMPLE_SPI_MASTER_RX_CHANNEL);

    DMA_CreateHandle(&slaveTxHandle, EXAMPLE_DMA, EXAMPLE_SPI_SLAVE_TX_CHANNEL);
    DMA_CreateHandle(&slaveRxHandle, EXAMPLE_DMA, EXAMPLE_SPI_SLAVE_RX_CHANNEL);

    /* Init SPI master */
    /*
     * masterConfig.enableStopInWaitMode = false;
     * masterConfig.polarity = kSPI_ClockPolarityActiveHigh;
     * masterConfig.phase = kSPI_ClockPhaseFirstEdge;
     * masterConfig.direction = kSPI_MsbFirst;
     * masterConfig.dataMode = kSPI_8BitMode;
     * masterConfig.txWatermark = kSPI_TxFifoOneHalfEmpty;
     * masterConfig.rxWatermark = kSPI_RxFifoOneHalfFull;
     * masterConfig.pinMode = kSPI_PinModeNormal;
     * masterConfig.outputMode = kSPI_SlaveSelectAutomaticOutput;
     * masterConfig.baudRate_Bps = 500000U;
     */
    SPI_MasterGetDefaultConfig(&masterConfig);
    sourceClock = EXAMPLE_SPI_MASTER_CLK_FREQ;
    SPI_MasterInit(EXAMPLE_SPI_MASTER, &masterConfig, sourceClock);
    /* Create handle for SPI master */
    SPI_MasterTransferCreateHandleDMA(EXAMPLE_SPI_MASTER, &masterHandle, masterCallback, NULL, &masterTxHandle,
                                      &masterRxHandle);

    /* Init SPI slave */
    /*
     * slaveConfig.polarity = kSPI_ClockPolarityActiveHigh;
     * slaveConfig.phase = kSPI_ClockPhaseFirstEdge;
     * slaveConfig.direction = kSPI_MsbFirst;
     * slaveConfig.enableStopInWaitMode = false;
     * slaveConfig.dataMode = kSPI_8BitMode;
     * slaveConfig.txWatermark = kSPI_TxFifoOneHalfEmpty;
     * slaveConfig.rxWatermark = kSPI_RxFifoOneHalfFull;
     */
    SPI_SlaveGetDefaultConfig(&slaveConfig);
    SPI_SlaveInit(EXAMPLE_SPI_SLAVE, &slaveConfig);
    SPI_SlaveTransferCreateHandleDMA(EXAMPLE_SPI_SLAVE, &slaveHandle, slaveCallback, NULL, &slaveTxHandle,
                                     &slaveRxHandle);

    /* Init source buffer */
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        srcBuff[i] = i;
    }

    /* SPI slave transfer */
    xfer.rxData = destBuff;
    xfer.dataSize = BUFFER_SIZE;
    xfer.txData = NULL;
    SPI_SlaveTransferDMA(EXAMPLE_SPI_SLAVE, &slaveHandle, &xfer);

    /* SPI master start transfer */
    xfer.txData = srcBuff;
    xfer.rxData = NULL;
    xfer.dataSize = BUFFER_SIZE;
    SPI_MasterTransferDMA(EXAMPLE_SPI_MASTER, &masterHandle, &xfer);

    while ((masterFinished != true) || (slaveFinished != true))
    {
    }

    /* Check the data received */
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        if (destBuff[i] != srcBuff[i])
        {
            PRINTF("\r\nThe %d data is wrong, the data received is %d \r\n", i, destBuff[i]);
            err++;
        }
    }
    if (err == 0)
    {
        PRINTF("\r\nSPI transfer finished!\r\n");
    }

    while (1)
    {
    }
}
