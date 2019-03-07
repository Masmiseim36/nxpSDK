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
#include "fsl_ecspi.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_ECSPI_MASTER_BASEADDR ECSPI4
#define EXAMPLE_ECSPI_DEALY_COUNT 1000000U
#define ECSPI_MASTER_CLK_FREQ (CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 8U)
#define TRANSFER_SIZE 64U         /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint32_t masterRxData[TRANSFER_SIZE] = {0U};
uint32_t masterTxData[TRANSFER_SIZE] = {0U};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetMux(kCLOCK_EcspiMux, 0); /* Set ECSPI source to PLL3 60M */
    CLOCK_SetDiv(kCLOCK_EcspiDiv, 0); /* Set ECSPI divider to 1 */

    PRINTF("ECSPI board to board polling example.\r\n");
    PRINTF("This example use one board as master and another as slave.\r\n");
    PRINTF("Master uses polling way and slave uses interrupt way. \r\n");
    PRINTF("Please make sure you make the correct line connection. Basically, the connection is: \r\n");
    PRINTF("ECSPI_master -- ECSPI_slave   \r\n");
    PRINTF("   CLK      --    CLK  \r\n");
    PRINTF("   PCS      --    PCS \r\n");
    PRINTF("   MOSI     --    MOSI \r\n");
    PRINTF("   MISO     --    MISO  \r\n");
    PRINTF("   GND      --    GND \r\n");

    uint32_t errorCount;
    uint32_t loopCount = 1U;
    uint32_t i;
    ecspi_master_config_t masterConfig;
    ecspi_transfer_t masterXfer;

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
        masterXfer.txData = masterTxData;
        masterXfer.rxData = NULL;
        masterXfer.dataSize = TRANSFER_SIZE;
        masterXfer.channel = kECSPI_Channel0;
        ECSPI_MasterTransferBlocking(EXAMPLE_ECSPI_MASTER_BASEADDR, &masterXfer);

        /* Delay to wait slave is ready */
        for (i = 0U; i < EXAMPLE_ECSPI_DEALY_COUNT; i++)
        {
            __NOP();
        }

        /* Start master transfer, receive data from slave */
        masterXfer.txData = NULL;
        masterXfer.rxData = masterRxData;
        masterXfer.dataSize = TRANSFER_SIZE;
        masterXfer.channel = kECSPI_Channel0;
        ECSPI_MasterTransferBlocking(EXAMPLE_ECSPI_MASTER_BASEADDR, &masterXfer);

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
            PRINTF("\r\nError occured in ECSPI transfer ! \r\n");
        }

        /* Wait for press any key */
        PRINTF("\r\n Press any key to run again\r\n");
        GETCHAR();

        /* Increase loop count to change transmit buffer */
        loopCount++;
    }
}
