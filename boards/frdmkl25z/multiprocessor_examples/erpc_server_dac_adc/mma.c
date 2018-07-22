/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#include "fsl_mma.h"
#include "fsl_debug_console.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ACCEL_I2C_CLK_SRC I2C0_CLK_SRC
/*******************************************************************************
 * Variables
 ******************************************************************************/
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
extern void FXOS_master_callback(LPI2C_Type *base, lpi2c_master_handle_t *handle, status_t status, void *userData);
lpi2c_master_handle_t g_MasterHandle;
#else
i2c_master_handle_t g_MasterHandle;
#endif

const uint8_t g_accel_address[] = {0x1CU, 0x1DU, 0x1EU, 0x1FU};
mma_handle_t mmaHandle = {0};
uint8_t dataScale = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
void i2cInit()
{
#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    lpi2c_master_config_t i2cConfig;
#else
    i2c_master_config_t i2cConfig;
#endif

    uint32_t i2cSourceClock;

#if defined(FSL_FEATURE_SOC_LPI2C_COUNT) && (FSL_FEATURE_SOC_LPI2C_COUNT)
    i2cSourceClock = LPI2C_CLOCK_FREQUENCY;
    /*
     * i2cConfig.debugEnable = false;
     * i2cConfig.ignoreAck = false;
     * i2cConfig.pinConfig = kLPI2C_2PinOpenDrain;
     * i2cConfig.baudRate_Bps = 100000U;
     * i2cConfig.busIdleTimeout_ns = 0;
     * i2cConfig.pinLowTimeout_ns = 0;
     * i2cConfig.sdaGlitchFilterWidth_ns = 0;
     * i2cConfig.sclGlitchFilterWidth_ns = 0;
     */
    LPI2C_MasterGetDefaultConfig(&i2cConfig);
    LPI2C_MasterInit(BOARD_ACCEL_I2C_BASEADDR, &i2cConfig, i2cSourceClock);
    LPI2C_MasterTransferCreateHandle(BOARD_ACCEL_I2C_BASEADDR, &g_MasterHandle, FXOS_master_callback, NULL);
#else
    i2cSourceClock = CLOCK_GetFreq(ACCEL_I2C_CLK_SRC);
    /*
     * i2cConfig.baudRate_Bps = 100000U;
     * i2cConfig.enableStopHold = false;
     * i2cConfig.glitchFilterWidth = 0U;
     * i2cConfig.enableMaster = true;
     */
    I2C_MasterGetDefaultConfig(&i2cConfig);
    I2C_MasterInit(BOARD_ACCEL_I2C_BASEADDR, &i2cConfig, i2cSourceClock);
    I2C_MasterTransferCreateHandle(BOARD_ACCEL_I2C_BASEADDR, &g_MasterHandle, NULL, NULL);
#endif
}

int32_t init_mag_accel()
{
    uint8_t regResult = 0;
    uint16_t i = 0;
    mmaHandle.base = BOARD_ACCEL_I2C_BASEADDR;
    mmaHandle.i2cHandle = &g_MasterHandle;
    uint8_t array_addr_size = 0;
    bool foundDevice = false;
    uint8_t sensorRange = 0;

    /* Find sensor devices */
    array_addr_size = sizeof(g_accel_address) / sizeof(g_accel_address[0]);
    for (i = 0; i < array_addr_size; i++)
    {
        mmaHandle.xfer.slaveAddress = g_accel_address[i];
        if (MMA_ReadReg(&mmaHandle, kMMA8451_WHO_AM_I, &regResult) == kStatus_Success)
        {
            foundDevice = true;
            break;
        }
        if ((i == (array_addr_size - 1)) && (!foundDevice))
        {
            PRINTF("\r\nDo not found sensor device\r\n");
            while (1)
            {
            };
        }
    }

    /* Init accelerometer sensor */
    if (MMA_Init(&mmaHandle) != kStatus_Success)
    {
        return -1;
    }

    /* Get sensor range */
    if (MMA_ReadReg(&mmaHandle, kMMA8451_XYZ_DATA_CFG, &sensorRange) != kStatus_Success)
    {
        return -1;
    }
    if (sensorRange == 0x00)
    {
        dataScale = 2U;
    }
    else if (sensorRange == 0x01)
    {
        dataScale = 4U;
    }
    else if (sensorRange == 0x10)
    {
        dataScale = 8U;
    }

    return 0;
}

void Sensor_ReadData(int16_t *Ax, int16_t *Ay, int16_t *Az, int16_t *Mx, int16_t *My, int16_t *Mz)
{
    mma_data_t sensorData = {0};

    /* Get new accelerometer data. */
    if (MMA_ReadSensorData(&mmaHandle, &sensorData) != kStatus_Success)
    {
        PRINTF("Failed to read acceleration data!\r\n");
    }

    /* Get the X and Y data from the sensor data structure in 14 bit left format data*/
    *Ax = (int16_t)((uint16_t)((uint16_t)sensorData.accelXMSB << 8) | (uint16_t)sensorData.accelXLSB) / 4U;
    *Ay = (int16_t)((uint16_t)((uint16_t)sensorData.accelYMSB << 8) | (uint16_t)sensorData.accelYLSB) / 4U;
    *Az = (int16_t)((uint16_t)((uint16_t)sensorData.accelZMSB << 8) | (uint16_t)sensorData.accelZLSB) / 4U;

    *Ax *= dataScale;
    *Ay *= dataScale;
    *Az *= dataScale;

    *Mx *= 0;
    *My *= 0;
    *Mz *= 0;
}
