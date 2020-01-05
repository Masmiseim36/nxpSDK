/*
 * The Clear BSD License
 * Copyright (c) 2013-2015, Freescale Semiconductor, Inc.
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

#include "fsl_mma.h"

/******************************************************************************
 * Code
 ******************************************************************************/
status_t MMA_Init(mma_handle_t *handle, mma_config_t *config)
{
    assert(handle);
    assert(config);

    uint8_t val = 0;

    /* Initialize the callback function. */
    handle->I2C_SendFunc = config->I2C_SendFunc;
    handle->I2C_ReceiveFunc = config->I2C_ReceiveFunc;
    /* Set Slave Address. */    
    handle->slaveAddress = config->slaveAddress;

    if(MMA_ReadReg(handle, kMMA8451_WHO_AM_I, &val) != kStatus_Success)
    {
        return kStatus_Fail;
    }

    if (val != kMMA8451_WHO_AM_I_Device_ID)
    {
        return kStatus_Fail;
    }

    /* Reset sensor */
    if(MMA_ReadReg(handle, kMMA8451_CTRL_REG2, &val) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    val |= (uint8_t)0x40;
    if(MMA_WriteReg(handle, kMMA8451_CTRL_REG2, val) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    /* Put the mma8451 into standby mode */
    if(MMA_ReadReg(handle, kMMA8451_CTRL_REG1, &val) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    val &= (uint8_t)(~(0x01));
    if(MMA_WriteReg(handle, kMMA8451_CTRL_REG1, val) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    /* Set the range, -4g to 4g */
    if(MMA_ReadReg(handle, kMMA8451_XYZ_DATA_CFG, &val) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    val &= (uint8_t)(~0x03);
    val |= 0x01;
    if(MMA_WriteReg(handle, kMMA8451_XYZ_DATA_CFG, val) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    /* Set the F_MODE, disable FIFO */
    if(MMA_ReadReg(handle, kMMA8451_F_SETUP, &val) != kStatus_Success)
    {
        return kStatus_Fail;
    }

    val &= 0x3F;
    if(MMA_WriteReg(handle, kMMA8451_F_SETUP, val) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    /* Put the mma8451 into active mode */
    if(MMA_ReadReg(handle, kMMA8451_CTRL_REG1, &val) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    val |= 0x01;
    val &= (uint8_t)(~0x02); /* set F_READ to 0 */
    if(MMA_WriteReg(handle, kMMA8451_CTRL_REG1, val) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    return kStatus_Success;
}

status_t MMA_ReadSensorData(mma_handle_t *handle, mma_data_t *accel)
{
    uint8_t val = 0U;
    uint8_t ucStatus = 0;

    do
    {
        if(MMA_ReadReg(handle, kMMA8451_STATUS, &ucStatus) != kStatus_Success)
        {
            return kStatus_Fail;
        }
    } while (!(ucStatus & 0x08));

    if(MMA_ReadReg(handle, kMMA8451_OUT_X_MSB, &val) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    accel->accelXMSB = val;
    if(MMA_ReadReg(handle, kMMA8451_OUT_X_LSB, &val) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    accel->accelXLSB = val;

    if(MMA_ReadReg(handle, kMMA8451_OUT_Y_MSB, &val) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    accel->accelYMSB = val;
    if(MMA_ReadReg(handle, kMMA8451_OUT_Y_LSB, &val) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    accel->accelYLSB = val;

    if(MMA_ReadReg(handle, kMMA8451_OUT_Z_MSB, &val) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    accel->accelZMSB = val;
    if(MMA_ReadReg(handle, kMMA8451_OUT_Z_LSB, &val) != kStatus_Success)
    {
        return kStatus_Fail;
    }
    accel->accelZLSB = val;

    return kStatus_Success;
}

status_t MMA_ReadReg(mma_handle_t *handle, uint8_t reg, uint8_t *val)
{
    assert(handle);
    assert(val);

    if (!handle->I2C_ReceiveFunc)
    {
        return kStatus_Fail;
    }

    return handle->I2C_ReceiveFunc(handle->slaveAddress, reg, 1, val, 1);
}

status_t MMA_WriteReg(mma_handle_t *handle, uint8_t reg, uint8_t val)
{
    assert(handle);

    if (!handle->I2C_SendFunc)
    {
        return kStatus_Fail;
    }

    return handle->I2C_SendFunc(handle->slaveAddress, reg, 1, val);
}
