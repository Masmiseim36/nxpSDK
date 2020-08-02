/*
 * The Clear BSD License
 * Copyright (c) 2013 - 2014, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
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

#include "AT24C02.h"
#include "fsl_lpi2c.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* lpi2c handler */
lpi2c_master_handle_t g_m_handle;

/* The completion flag */
volatile bool completionFlag = false;

/* Completion flag */
static void lpi2c_master_callback(LPI2C_Type *base,
                                  lpi2c_master_handle_t *handle,
                                  status_t completionStatus,
                                  void *userData)
{
    switch (completionStatus)
    {
        /*  Transfer done */
        case kStatus_Success:
            completionFlag = true;
            break;

        default:
            break;
    }
}

/*******************************************************************************
 * Code
 ******************************************************************************/
extern uint32_t AT24C02_GetI2cSrcClkFreq(void);

at24c02_status_t AT24C02_I2CInit(at24c02_handler_t *handler)
{
    /* i2c master init */
    uint32_t sourceClock = 0;
    lpi2c_master_config_t masterConfig;

    /*
     * masterConfig.debugEnable = false;
     * masterConfig.ignoreAck = false;
     * masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * masterConfig.baudRate_Hz = 100000U;
     * masterConfig.busIdleTimeout_ns = 0;
     * masterConfig.pinLowTimeout_ns = 0;
     * masterConfig.sdaGlitchFilterWidth_ns = 0;
     * masterConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = AT24C02_I2C_BAUDRATE;
    sourceClock = AT24C02_GetI2cSrcClkFreq();
    LPI2C_MasterInit(handler->i2cBase, &masterConfig, sourceClock);
    LPI2C_MasterTransferCreateHandle(handler->i2cBase, &g_m_handle, lpi2c_master_callback, NULL);
    return kStatus_AT24C02_Success;
}

at24c02_status_t AT24C02_ByteWrite(at24c02_handler_t *handler, uint8_t address, uint8_t data)
{
    /* Setup the master transfer */
    lpi2c_master_transfer_t masterXfer = {0};
    masterXfer.slaveAddress = handler->device_addr;
    masterXfer.direction = kLPI2C_Write;
    masterXfer.subaddress = address;
    masterXfer.subaddressSize = 1;
    masterXfer.data = &data;
    masterXfer.dataSize = 1;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    LPI2C_MasterTransferNonBlocking(handler->i2cBase, &g_m_handle, &masterXfer);

    while (!completionFlag)
    {
        ;
    }

    completionFlag = false;

    return kStatus_AT24C02_Success;
}

at24c02_status_t AT24C02_ACKPoll(at24c02_handler_t *handler)
{
    /* Used for the Twr time: 5ms */
    lpi2c_master_transfer_t masterXfer = {0};
    masterXfer.slaveAddress = handler->device_addr;
    masterXfer.direction = kLPI2C_Write;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = NULL;
    masterXfer.dataSize = 0;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    LPI2C_MasterTransferNonBlocking(handler->i2cBase, &g_m_handle, &masterXfer);

    while (!completionFlag)
    {
        ;
    }

    completionFlag = false;

    return kStatus_AT24C02_Success;
}

at24c02_status_t AT24C02_ByteRead(at24c02_handler_t *handler, uint8_t address, uint8_t *data)
{
    lpi2c_master_transfer_t masterXfer = {0};
    masterXfer.slaveAddress = handler->device_addr;
    masterXfer.direction = kLPI2C_Read;
    masterXfer.subaddress = address;
    masterXfer.subaddressSize = 1;
    masterXfer.data = data;
    masterXfer.dataSize = 1;
    masterXfer.flags = kLPI2C_TransferDefaultFlag;

    LPI2C_MasterTransferNonBlocking(handler->i2cBase, &g_m_handle, &masterXfer);

    while (!completionFlag)
    {
        ;
    }

    completionFlag = false;

    return kStatus_AT24C02_Success;
}
