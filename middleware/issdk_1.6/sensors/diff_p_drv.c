/*
 * Copyright (c) 2015-2016, Freescale Semiconductor, Inc.
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

/**
 * @file  diff_p_drv.c
 * @brief The diff_p_drv.c file implements the diff_p functional interface.
 */

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "diff_p_drv.h"
#include "systick_utils.h"

//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------
int32_t DIFF_P_I2C_Initialize(
    diff_p_i2c_sensorhandle_t *pSensorHandle, ARM_DRIVER_I2C *pBus, uint8_t index, uint16_t sAddress, uint8_t whoAmi)
{
    int32_t status;
    uint8_t reg, retries = 1;

    /*! Check the input parameters. */
    if ((pSensorHandle == NULL) || (pBus == NULL))
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    pSensorHandle->deviceInfo.deviceInstance = index;
    pSensorHandle->deviceInfo.functionParam = NULL;
    pSensorHandle->deviceInfo.idleFunction = NULL;

    /*!  Read and store the device's WHO_AM_I.*/
    status = Register_I2C_Read(pBus, &pSensorHandle->deviceInfo, sAddress, DIFF_P_WHO_AM_I, 1, &reg);
    if ((ARM_DRIVER_OK != status) || (whoAmi != reg))
    {
        pSensorHandle->isInitialized = false;
        return SENSOR_ERROR_INIT;
    }

    /*! Initialize the sensor handle. */
    pSensorHandle->pCommDrv = pBus;
    pSensorHandle->slaveAddress = sAddress;
    pSensorHandle->isInitialized = true;

    do
    { /*! Put the device into standby mode so that we can run calibration can be done. */
        status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
                                    DIFF_P_CTRL_REG1, DIFF_P_CTRL_REG1_SBYB_STANDBY, DIFF_P_CTRL_REG1_SBYB_MASK, false);
        if (ARM_DRIVER_OK != status)
        {
            return SENSOR_ERROR_INIT;
        }

        /*! Run Calibration algorithm. */
        status =
            Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
                               DIFF_P_CTRL_REG2, DIFF_P_CTRL_REG2_CTRL_AC_CALRUN, DIFF_P_CTRL_REG2_CTRL_AC_MASK, false);
        if (ARM_DRIVER_OK != status)
        {
            return SENSOR_ERROR_INIT;
        }

        do
        { /*!  Wait for calibration to finish.*/
            status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
                                       DIFF_P_CTRL_REG2, 1, &reg);
            if (ARM_DRIVER_OK != status)
            {
                return SENSOR_ERROR_INIT;
            }
        } while (reg & DIFF_P_CTRL_REG2_CTRL_AC_MASK);

        /* Check Calibration Result. */
        status = Register_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
                                   DIFF_P_STATUS, 1, &reg);
        if (ARM_DRIVER_OK != status)
        {
            return SENSOR_ERROR_INIT;
        }
        if (reg & DIFF_P_STATUS_STAT_EP_MASK == 0)
        {
            break;
        }
        BOARD_DELAY_ms(1); /* Wait and then retry calibration. */
    } while (retries--);

    return SENSOR_ERROR_NONE;
}

int32_t DIFF_P_I2C_Configure(diff_p_i2c_sensorhandle_t *pSensorHandle, const registerwritelist_t *pRegWriteList)
{
    int32_t status;

    /*! Validate for the correct handle and register write list.*/
    if ((pSensorHandle == NULL) || (pRegWriteList == NULL))
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before applying configuration.*/
    if (pSensorHandle->isInitialized != true)
    {
        return SENSOR_ERROR_INIT;
    }

    /*! Put the device into standby mode so that configuration can be applied.*/
    status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
                                DIFF_P_CTRL_REG1, DIFF_P_CTRL_REG1_SBYB_STANDBY, DIFF_P_CTRL_REG1_SBYB_MASK, false);
    if (ARM_DRIVER_OK != status)
    {
        return SENSOR_ERROR_WRITE;
    }

    /*! Apply the Sensor Configuration based on the Register Write List */
    status = Sensor_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
                              pRegWriteList);
    if (ARM_DRIVER_OK != status)
    {
        return SENSOR_ERROR_WRITE;
    }

    /*! Put the device into active mode and ready for reading data.*/
    status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
                                DIFF_P_CTRL_REG1, DIFF_P_CTRL_REG1_SBYB_ACTIVE, DIFF_P_CTRL_REG1_SBYB_MASK, false);
    if (ARM_DRIVER_OK != status)
    {
        return SENSOR_ERROR_WRITE;
    }

    return SENSOR_ERROR_NONE;
}

int32_t DIFF_P_I2C_ReadData(diff_p_i2c_sensorhandle_t *pSensorHandle,
                            const registerreadlist_t *pReadList,
                            uint8_t *pBuffer)
{
    int32_t status;

    /*! Validate for the correct handle and register read list.*/
    if ((pSensorHandle == NULL) || (pReadList == NULL) || (pBuffer == NULL))
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before reading sensor data.*/
    if (pSensorHandle->isInitialized != true)
    {
        return SENSOR_ERROR_INIT;
    }

    /*! Parse through the read list and read the data one by one. */
    status = Sensor_I2C_Read(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
                             pReadList, pBuffer);
    if (ARM_DRIVER_OK != status)
    {
        return SENSOR_ERROR_READ;
    }

    return SENSOR_ERROR_NONE;
}

int32_t DIFF_P_I2C_DeInit(diff_p_i2c_sensorhandle_t *pSensorHandle)
{
    int32_t status;

    if (pSensorHandle == NULL)
    {
        return SENSOR_ERROR_INVALID_PARAM;
    }

    /*! Check whether sensor handle is initialized before triggering sensor reset.*/
    if (pSensorHandle->isInitialized != true)
    {
        return SENSOR_ERROR_INIT;
    }

    /*! Trigger sensor device reset.*/
    status = Register_I2C_Write(pSensorHandle->pCommDrv, &pSensorHandle->deviceInfo, pSensorHandle->slaveAddress,
                                DIFF_P_CTRL_REG1, DIFF_P_CTRL_REG1_RST_RESET, DIFF_P_CTRL_REG1_RST_MASK, false);
    if (ARM_DRIVER_OK != status)
    {
        return SENSOR_ERROR_WRITE;
    }
    else
    {
        /*! De-initialize sensor handle. */
        pSensorHandle->isInitialized = false;
    }

    return SENSOR_ERROR_NONE;
}
