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
#define EXAMPLE_ECSPI_SLAVE_BASEADDR ECSPI4
#define TRANSFER_SIZE 64U /*! Transfer dataSize */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* ECSPI user callback */
void ECSPI_SlaveUserCallback(ECSPI_Type *base, ecspi_slave_handle_t *handle, status_t status, void *userData);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint32_t slaveRxData[TRANSFER_SIZE] = {0U};

ecspi_slave_handle_t g_s_handle;
volatile bool isTransferCompleted = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

void ECSPI_SlaveUserCallback(ECSPI_Type *base, ecspi_slave_handle_t *handle, status_t status, void *userData)
{
    if (status == kStatus_Success)
    {
        PRINTF("This is ECSPI slave transfer completed callback. \r\n");
        PRINTF("It's a successful transfer. \r\n\r\n");
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

    CLOCK_SetMux(kCLOCK_EcspiMux, 0); /* Set ECSPI source to PLL3 60M */
    CLOCK_SetDiv(kCLOCK_EcspiDiv, 0); /* Set ECSPI divider to 1 */

    PRINTF("ECSPI board to board polling example.\r\n");

    uint32_t i;
    ecspi_slave_config_t slaveConfig;
    ecspi_transfer_t slaveXfer;

    /* Slave config:
     * slaveConfig.channel = kECSPI_Channel0;
     * slaveConfig.burstLength = 8;
     * slaveConfig.txFifoThreshold = 1;
     * slaveConfig.rxFifoThreshold = 0;
     */
    ECSPI_SlaveGetDefaultConfig(&slaveConfig);
    ECSPI_SlaveInit(EXAMPLE_ECSPI_SLAVE_BASEADDR, &slaveConfig);

    ECSPI_SlaveTransferCreateHandle(EXAMPLE_ECSPI_SLAVE_BASEADDR, &g_s_handle, ECSPI_SlaveUserCallback, NULL);

    while (1)
    {
        PRINTF("\r\n Slave example is running...\r\n\r\n");

        /* Reset the receive buffer */
        for (i = 0U; i < TRANSFER_SIZE; i++)
        {
            slaveRxData[i] = 0U;
        }

        /* Set slave transfer ready to receive data */
        isTransferCompleted = false;
        slaveXfer.txData = NULL;
        slaveXfer.rxData = slaveRxData;
        slaveXfer.dataSize = TRANSFER_SIZE;
        slaveXfer.channel = kECSPI_Channel0;

        /* Slave start receive */
        PRINTF("Slave starts to receive data!\r\n");
        ECSPI_SlaveTransferNonBlocking(EXAMPLE_ECSPI_SLAVE_BASEADDR, &g_s_handle, &slaveXfer);

        while (!isTransferCompleted)
        {
        }

        /* Set slave transfer ready to send back data */
        isTransferCompleted = false;
        slaveXfer.txData = slaveRxData;
        slaveXfer.rxData = NULL;
        slaveXfer.dataSize = TRANSFER_SIZE;
        slaveXfer.channel = kECSPI_Channel0;

        /* Slave start send */
        PRINTF("Slave starts to transmit data!\r\n");
        ECSPI_SlaveTransferNonBlocking(EXAMPLE_ECSPI_SLAVE_BASEADDR, &g_s_handle, &slaveXfer);

        while (!isTransferCompleted)
        {
        }

        /* Print out receive buffer */
        PRINTF("\r\n Slave received:");
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
