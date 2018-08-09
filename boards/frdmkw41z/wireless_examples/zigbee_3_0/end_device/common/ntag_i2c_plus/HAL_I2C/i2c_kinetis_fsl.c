/*
* The Clear BSD License
* Copyright (c) 2016-2018, NXP Semiconductor
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "HAL_I2C_kinetis_fsl.h"
//---------------------------------------------------------------------
int16_t I2C_InitDevice(uint32_t bitrate, clock_name_t input_clock, I2C_Type *instance)
{
    i2c_master_config_t masterConfig;
    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = bitrate;
    uint32_t sourceClock;
    sourceClock = CLOCK_GetFreq(input_clock);

    I2C_MasterInit(instance, &masterConfig, sourceClock);

    return instance->S;
}

//---------------------------------------------------------------------
int16_t TransmitPoll(I2C_Type *instance, uint8_t address, uint8_t *bytes, size_t len)
{
    uint8_t status;
    i2c_master_transfer_t xfer;

    memset(&xfer, 0, sizeof(xfer));

    xfer.slaveAddress = address;
    xfer.direction = kI2C_Write;
    xfer.data = bytes;
    xfer.dataSize = len;

    status = I2C_MasterTransferBlocking(instance, &xfer);

    return status;
}

//---------------------------------------------------------------------
int16_t ReceivePoll(I2C_Type *instance, uint8_t address, uint8_t *bytes, size_t len)
{
    uint8_t status;
    i2c_master_transfer_t xfer;

    memset(&xfer, 0, sizeof(xfer));

    xfer.slaveAddress = address;
    xfer.direction = kI2C_Read;
    xfer.data = bytes;
    xfer.dataSize = len;

    status = I2C_MasterTransferBlocking(instance, &xfer);

    return status;
}
