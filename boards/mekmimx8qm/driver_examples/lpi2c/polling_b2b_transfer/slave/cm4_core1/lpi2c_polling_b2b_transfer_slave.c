/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*  Standard C Included Files */
#include <stdio.h>
#include <string.h>
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_irqsteer.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_I2C_SLAVE_BASE DMA__LPI2C0
#define LPI2C_SLAVE_CLOCK_FREQUENCY CLOCK_GetIpFreq(kCLOCK_DMA_Lpi2c0)

#define EXAMPLE_I2C_SLAVE ((LPI2C_Type *)EXAMPLE_I2C_SLAVE_BASE)

#define I2C_MASTER_SLAVE_ADDR_7BIT 0x7EU
#define I2C_DATA_LENGTH 34U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

uint8_t g_slave_buff[I2C_DATA_LENGTH];
lpi2c_slave_handle_t g_s_handle;
volatile bool g_SlaveCompletionFlag = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void lpi2c_slave_callback(LPI2C_Type *base, lpi2c_slave_transfer_t *xfer, void *param)
{
    switch (xfer->event)
    {
        /*  Address match event */
        case kLPI2C_SlaveAddressMatchEvent:
            xfer->data     = NULL;
            xfer->dataSize = 0;
            break;
        /*  Transmit request */
        case kLPI2C_SlaveTransmitEvent:
            /*  Update information for transmit process */
            xfer->data     = &g_slave_buff[2];
            xfer->dataSize = g_slave_buff[1];
            break;

        /*  Receive request */
        case kLPI2C_SlaveReceiveEvent:
            /*  Update information for received process */
            xfer->data     = g_slave_buff;
            xfer->dataSize = I2C_DATA_LENGTH;
            break;

        /*  Transfer done */
        case kLPI2C_SlaveCompletionEvent:
            g_SlaveCompletionFlag = true;
            xfer->data            = NULL;
            xfer->dataSize        = 0;
            break;

        default:
            g_SlaveCompletionFlag = false;
            break;
    }
}

/*!
 * @brief Main function
 */
int main(void)
{
    lpi2c_slave_config_t slaveConfig;
    status_t reVal = kStatus_Fail;

    sc_ipc_t ipc;
    uint32_t freq;

    ipc = BOARD_InitRpc();

    BOARD_InitPins(ipc);
    BOARD_BootClockRUN();
    BOARD_InitMemory();
    BOARD_InitDebugConsole();

    /* Power on LPI2C. */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_I2C_0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on LPI2C\r\n");
    }

    /* Set LPI2C clock */
    freq = CLOCK_SetIpFreq(kCLOCK_DMA_Lpi2c0, SC_24MHZ);
    if (freq == 0)
    {
        PRINTF("Error: Failed to set LPI2C frequency\r\n");
    }

    /* Enable interrupt in irqsteer */
    if (sc_pm_set_resource_power_mode(ipc, SC_R_IRQSTR_M4_1, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
    {
        PRINTF("Error: Failed to power on IRQSTEER!\r\n");
    }
    IRQSTEER_Init(IRQSTEER);
    IRQSTEER_EnableInterrupt(IRQSTEER, DMA_I2C0_INT_IRQn);

    PRINTF("\r\nLPI2C board2board polling example -- Slave transfer.\r\n\r\n");

    /* Set up i2c slave first */
    /*
     * slaveConfig.address0 = 0U;
     * slaveConfig.address1 = 0U;
     * slaveConfig.addressMatchMode = kLPI2C_MatchAddress0;
     * slaveConfig.filterDozeEnable = true;
     * slaveConfig.filterEnable = true;
     * slaveConfig.enableGeneralCall = false;
     * slaveConfig.ignoreAck = false;
     * slaveConfig.enableReceivedAddressRead = false;
     * slaveConfig.sdaGlitchFilterWidth_ns = 0;
     * slaveConfig.sclGlitchFilterWidth_ns = 0;
     * slaveConfig.dataValidDelay_ns = 0;
     * slaveConfig.clockHoldTime_ns = 0;
     */
    LPI2C_SlaveGetDefaultConfig(&slaveConfig);

    /* Change the slave address */
    slaveConfig.address0 = I2C_MASTER_SLAVE_ADDR_7BIT;

    /* Initialize the LPI2C slave peripheral */
    LPI2C_SlaveInit(EXAMPLE_I2C_SLAVE, &slaveConfig, LPI2C_SLAVE_CLOCK_FREQUENCY);

    memset(g_slave_buff, 0, sizeof(g_slave_buff));

    /* Create the LPI2C handle for the non-blocking transfer */
    LPI2C_SlaveTransferCreateHandle(EXAMPLE_I2C_SLAVE, &g_s_handle, lpi2c_slave_callback, NULL);

    /* Start accepting I2C transfers on the LPI2C slave peripheral */
    reVal = LPI2C_SlaveTransferNonBlocking(EXAMPLE_I2C_SLAVE, &g_s_handle,
                                           kLPI2C_SlaveCompletionEvent | kLPI2C_SlaveAddressMatchEvent);
    if (reVal != kStatus_Success)
    {
        return -1;
    }

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

    PRINTF("\r\nEnd of LPI2C example .\r\n");

    while (1)
    {
    }
}
