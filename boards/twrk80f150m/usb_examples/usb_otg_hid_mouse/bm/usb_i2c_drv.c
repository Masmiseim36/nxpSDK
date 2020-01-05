/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "usb_otg.h"
#include "fsl_i2c.h"
#include "usb_i2c_drv.h"

usb_status_t USB_I2CMasterInit(void *base, uint32_t baudRate, uint32_t clockSource)
{
    i2c_master_config_t masterConfig;
    uint32_t sourceClock;

    /* initialize IIC */
    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = baudRate;
    sourceClock = CLOCK_GetFreq((clock_name_t)clockSource);
    I2C_MasterInit(base, &masterConfig, sourceClock);

    return kStatus_USB_Success;
}

usb_status_t USB_I2CMasterDeinit(void *base)
{
    I2C_MasterDeinit(base);
    return kStatus_USB_Success;
}

usb_status_t USB_I2CMasterWriteRegister(void *base, uint8_t slaveAddress, uint8_t registerAddress, uint8_t value)
{
    i2c_master_transfer_t masterXfer;
    status_t status;

    masterXfer.slaveAddress = slaveAddress;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = registerAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data = &value;
    masterXfer.dataSize = 1;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    status = I2C_MasterTransferBlocking(base, &masterXfer);
    if (status != kStatus_Success)
    {
        return kStatus_USB_Error;
    }

    return kStatus_USB_Success;
}

usb_status_t USB_I2CMasterReadRegister(void *base, uint8_t slaveAddress, uint8_t registerAddress, uint8_t *value)
{
    i2c_master_transfer_t masterXfer;
    status_t status;

    masterXfer.slaveAddress = slaveAddress;
    masterXfer.direction = kI2C_Read;
    masterXfer.subaddress = registerAddress;
    masterXfer.subaddressSize = 1;
    masterXfer.data = value;
    masterXfer.dataSize = 1;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    status = I2C_MasterTransferBlocking(base, &masterXfer);
    if (status != kStatus_Success)
    {
        return kStatus_USB_Error;
    }

    return kStatus_USB_Success;
}
