/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_mma.h"
#include "fsl_debug_console.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Variables
 ******************************************************************************/
const uint8_t g_accel_address[] = {0x1CU, 0x1DU, 0x1EU, 0x1FU};
mma_handle_t mmaHandle          = {0};
uint8_t dataScale               = 0;

/*******************************************************************************
 * Code
 ******************************************************************************/
int32_t init_mag_accel()
{
    status_t result         = kStatus_Fail;
    mma_config_t config     = {0};
    uint16_t i              = 0;
    uint8_t array_addr_size = 0;
    uint8_t sensorRange     = 0;

    /* Configure the I2C function */
    config.I2C_SendFunc    = BOARD_Accel_I2C_Send;
    config.I2C_ReceiveFunc = BOARD_Accel_I2C_Receive;

    /* Initialize sensor devices */
    array_addr_size = sizeof(g_accel_address) / sizeof(g_accel_address[0]);
    for (i = 0; i < array_addr_size; i++)
    {
        config.slaveAddress = g_accel_address[i];
        /* Initialize accelerometer sensor */
        result = MMA_Init(&mmaHandle, &config);
        if (result == kStatus_Success)
        {
            break;
        }
    }

    if (result != kStatus_Success)
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
