/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  SDK Included Files */
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_i2c.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_I2C_SLAVE_BASE    I2C11
#define I2C_SLAVE_CLOCK_FREQUENCY CLOCK_GetFlexcommClkFreq(11)

#define EXAMPLE_I2C_SLAVE ((I2C_Type *)EXAMPLE_I2C_SLAVE_BASE)

#define I2C_MASTER_SLAVE_ADDR_7BIT (0x7EU)
#define I2C_DATA_LENGTH            (34) /* MAX is 256 */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_slave_buff[I2C_DATA_LENGTH];
i2c_slave_handle_t g_s_handle;
volatile bool g_SlaveCompletionFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void i2c_slave_callback(I2C_Type *base, volatile i2c_slave_transfer_t *xfer, void *userData)
{
    switch (xfer->event)
    {
        /*  Address match event */
        case kI2C_SlaveAddressMatchEvent:
            xfer->rxData = NULL;
            xfer->rxSize = 0;
            break;
        /*  Receive request */
        case kI2C_SlaveReceiveEvent:
            xfer->rxData = g_slave_buff;
            xfer->rxSize = I2C_DATA_LENGTH;
            break;

        /*  Transfer done */
        case kI2C_SlaveCompletionEvent:
            /*  Transmit request */
            g_SlaveCompletionFlag = true;
            break;

        /*  Transmit request */
        case kI2C_SlaveTransmitEvent:
            xfer->txData = &g_slave_buff[2];
            xfer->txSize = g_slave_buff[1];
            break;

        default:
            g_SlaveCompletionFlag = false;
            break;
    }
}

int main(void)
{
    i2c_slave_config_t slaveConfig;

    /* Use 48 MHz clock for the FLEXCOMM11 */
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM11);

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\nI2C board2board DMA example -- Slave transfer.\r\n\r\n");

    /*Set up i2c slave first*/
    /*
     * slaveConfig.addressingMode = kI2C_Address7bit;
     * slaveConfig.enableGeneralCall = false;
     * slaveConfig.enableWakeUp = false;
     * slaveConfig.enableBaudRateCtl = false;
     * slaveConfig.enableSlave = true;
     */
    I2C_SlaveGetDefaultConfig(&slaveConfig);

    /* Change the slave address */
    slaveConfig.address0.address = I2C_MASTER_SLAVE_ADDR_7BIT;

    /* Initialize the I2C slave peripheral */
    I2C_SlaveInit(EXAMPLE_I2C_SLAVE, &slaveConfig, I2C_SLAVE_CLOCK_FREQUENCY);

    memset(g_slave_buff, 0, sizeof(g_slave_buff));
    memset(&g_s_handle, 0, sizeof(g_s_handle));

    I2C_SlaveTransferCreateHandle(EXAMPLE_I2C_SLAVE, &g_s_handle, i2c_slave_callback, NULL);
    I2C_SlaveTransferNonBlocking(EXAMPLE_I2C_SLAVE, &g_s_handle,
                                 kI2C_SlaveCompletionEvent | kI2C_SlaveAddressMatchEvent);

    /*  wait for transfer completed. */
    while (!g_SlaveCompletionFlag)
    {
    }
    g_SlaveCompletionFlag = false;

    PRINTF("Slave received data :");
    for (uint32_t i = 0U; i < g_slave_buff[1]; i++)
    {
        if (i % 8 == 0)
        {
            PRINTF("\r\n");
        }
        PRINTF("0x%2x  ", g_slave_buff[2 + i]);
    }
    PRINTF("\r\n\r\n");

    /* Wait for master receive completed.*/
    while (!g_SlaveCompletionFlag)
    {
    }
    g_SlaveCompletionFlag = false;

    PRINTF("\r\nEnd of I2C example .\r\n");

    while (1)
    {
    }
}
