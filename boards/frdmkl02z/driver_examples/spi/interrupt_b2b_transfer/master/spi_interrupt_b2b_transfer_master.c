/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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

#include "fsl_spi.h"
#include "board.h"
#include "fsl_debug_console.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_SPI_MASTER (SPI0)
#define EXAMPLE_SPI_MASTER_SOURCE_CLOCK (kCLOCK_BusClk)
#define EXAMPLE_SPI_MASTER_CLK_FREQ CLOCK_GetFreq((kCLOCK_BusClk))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
#define BUFFER_SIZE (64)
static uint8_t srcBuff[BUFFER_SIZE];
static uint8_t destBuff[BUFFER_SIZE];
spi_master_handle_t handle;
static volatile bool masterFinished = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
static void masterCallback(SPI_Type *base, spi_master_handle_t *masterHandle, status_t status, void *userData)
{
    masterFinished = true;
}

int main(void)
{
    spi_transfer_t xfer = {0};
    spi_master_config_t userConfig;
    uint32_t err = 0;
    uint32_t i = 0;
    uint32_t srcFreq = 0;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    PRINTF("\n\rMaster Start...\n\r");
    /*
     * userConfig.enableStopInWaitMode = false;
     * userConfig.polarity = kSPI_ClockPolarityActiveHigh;
     * userConfig.phase = kSPI_ClockPhaseFirstEdge;
     * userConfig.direction = kSPI_MsbFirst;
     * userConfig.dataMode = kSPI_8BitMode;
     * userConfig.txWatermark = kSPI_TxFifoOneHalfEmpty;
     * userConfig.rxWatermark = kSPI_RxFifoOneHalfFull;
     * userConfig.pinMode = kSPI_PinModeNormal;
     * userConfig.outputMode = kSPI_SlaveSelectAutomaticOutput;
     * userConfig.baudRate_Bps = 500000U;
     */
    SPI_MasterGetDefaultConfig(&userConfig);
    srcFreq = EXAMPLE_SPI_MASTER_CLK_FREQ;
    SPI_MasterInit(EXAMPLE_SPI_MASTER, &userConfig, srcFreq);

    /* Init Buffer */
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        srcBuff[i] = i;
    }

    /* Send to slave */
    xfer.txData = srcBuff;
    xfer.rxData = destBuff;
    xfer.dataSize = BUFFER_SIZE;
    SPI_MasterTransferCreateHandle(EXAMPLE_SPI_MASTER, &handle, masterCallback, NULL);
    SPI_MasterTransferNonBlocking(EXAMPLE_SPI_MASTER, &handle, &xfer);

    while (masterFinished != true)
    {
    }

    /* Check if the data is right */
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        if (srcBuff[i] != destBuff[i])
        {
            err++;
            PRINTF("The %d is wrong! data is %d\n\r", i, destBuff[i]);
        }
    }
    if (err == 0)
    {
        PRINTF("Succeed!\n\r");
    }

    while (1)
    {
    }
}
