/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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

#include "fsl_fxos.h"
#include "fsl_debug_console.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
fxos_handle_t g_fxosHandle;
uint8_t g_sensor_address[] = {0x1CU, 0x1EU, 0x1DU, 0x1FU};
uint8_t g_sensorRange = 0;
uint8_t g_dataScale = 0;

/*******************************************************************************
 * Code
******************************************************************************/
void Sensor_ReadData(int16_t *Ax, int16_t *Ay, int16_t *Az, int16_t *Mx, int16_t *My, int16_t *Mz)
{
    fxos_data_t fxos_data;

    if (FXOS_ReadSensorData(&g_fxosHandle, &fxos_data) != kStatus_Success)
    {
        PRINTF("Failed to read acceleration data!\r\n");
    }
    /* Get the accel data from the sensor data structure in 14 bit left format data*/
    *Ax = (int16_t)((uint16_t)((uint16_t)fxos_data.accelXMSB << 8) | (uint16_t)fxos_data.accelXLSB) / 4U;
    *Ay = (int16_t)((uint16_t)((uint16_t)fxos_data.accelYMSB << 8) | (uint16_t)fxos_data.accelYLSB) / 4U;
    *Az = (int16_t)((uint16_t)((uint16_t)fxos_data.accelZMSB << 8) | (uint16_t)fxos_data.accelZLSB) / 4U;
    *Ax *= g_dataScale;
    *Ay *= g_dataScale;
    *Az *= g_dataScale;
    *Mx = (int16_t)((uint16_t)((uint16_t)fxos_data.magXMSB << 8) | (uint16_t)fxos_data.magXLSB);
    *My = (int16_t)((uint16_t)((uint16_t)fxos_data.magYMSB << 8) | (uint16_t)fxos_data.magYLSB);
    *Mz = (int16_t)((uint16_t)((uint16_t)fxos_data.magZMSB << 8) | (uint16_t)fxos_data.magZLSB);
}

int32_t init_mag_accel(void)
{
    fxos_config_t config = {0};
    status_t result = kStatus_Fail;
    uint16_t i = 0;
    uint16_t array_addr_size = 0;

    /* Configure the I2C function */
    config.I2C_SendFunc = BOARD_Accel_I2C_Send;
    config.I2C_ReceiveFunc = BOARD_Accel_I2C_Receive;

    /* Initialize sensor devices */
    array_addr_size = sizeof(g_sensor_address) / sizeof(g_sensor_address[0]);
    for (i = 0; i < array_addr_size; i++)
    {
        config.slaveAddress = g_sensor_address[i];
        /* Initialize accelerometer sensor */
        result = FXOS_Init(&g_fxosHandle, &config);
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
    if (FXOS_ReadReg(&g_fxosHandle, XYZ_DATA_CFG_REG, &g_sensorRange, 1) != kStatus_Success)
    {
        return -1;
    }
    if (g_sensorRange == 0x00)
    {
        g_dataScale = 2U;
    }
    else if (g_sensorRange == 0x01)
    {
        g_dataScale = 4U;
    }
    else if (g_sensorRange == 0x10)
    {
        g_dataScale = 8U;
    }

    return 0;
}
