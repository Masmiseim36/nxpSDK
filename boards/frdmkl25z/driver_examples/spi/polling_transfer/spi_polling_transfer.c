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

#include "fsl_spi.h"
#include "board.h"
#include "fsl_debug_console.h"

#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_SPI_MASTER SPI0
#define EXAMPLE_SPI_SLAVE SPI1
#define EXAMPLE_SPI_MASTER_SOURCE_CLOCK kCLOCK_BusClk
#define EXAMPLE_SPI_MASTER_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
#define BUFFER_SIZE 64U
static uint8_t srcBuff[BUFFER_SIZE];
static uint8_t destBuff[BUFFER_SIZE];
static spi_slave_handle_t slaveHandle;
static volatile bool slaveFinished = false;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void slaveCallback(SPI_Type *base, spi_slave_handle_t *handle, status_t status, void *userData)
{
    slaveFinished = true;
}

int main(void)
{
    spi_master_config_t masterConfig = {0};
    spi_slave_config_t slaveConfig = {0};
    spi_transfer_t xfer = {0};
    uint32_t sourceClock = 0U;
    uint32_t i = 0U;
    uint32_t err = 0U;

    /* Init the boards */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\nSPI one board polling example started!\r\n");

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

    /* Init SPI slave, use interrupt mode, as no slave polling interface */
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
    SPI_SlaveTransferCreateHandle(EXAMPLE_SPI_SLAVE, &slaveHandle, slaveCallback, NULL);

    /* Init source buffer */
    for (i = 0U; i < BUFFER_SIZE; i++)
    {
        srcBuff[i] = i;
    }

    /* SPI slave transfer */
    xfer.rxData = destBuff;
    xfer.dataSize = BUFFER_SIZE;
    xfer.txData = NULL;
    SPI_SlaveTransferNonBlocking(EXAMPLE_SPI_SLAVE, &slaveHandle, &xfer);

    /* SPI master start transfer */
    xfer.txData = srcBuff;
    xfer.rxData = NULL;
    xfer.dataSize = BUFFER_SIZE;
    SPI_MasterTransferBlocking(EXAMPLE_SPI_MASTER, &xfer);

    while (slaveFinished != true)
    {
    }

    /* Check the data received */
    for (i = 0U; i < BUFFER_SIZE; i++)
    {
        if (destBuff[i] != srcBuff[i])
        {
            PRINTF("\r\nThe %d data is wrong, the data received is %d \r\n", i, destBuff[i]);
            err++;
        }
    }
    if (err == 0U)
    {
        PRINTF("\r\nSPI transfer finished!\r\n");
    }

    while (1)
    {
    }
}
