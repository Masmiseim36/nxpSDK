/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
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
 * @file fxas21002_fifo.c
 * @brief The fxas21002_fifo.c file implements the ISSDK FXAS21002 sensor driver
 *        example demonstration with FIFO mode.
 */

/*  SDK Includes */
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

/* CMSIS Includes */
#include "Driver_I2C.h"
#include "fsl_i2c_cmsis.h"

/* ISSDK Includes */
#include "issdk_hal.h"
#include "fxas21002_drv.h"
#include "systick_utils.h"

/*******************************************************************************
 * Macro Definitions
 ******************************************************************************/
/*---------------------------------------------------------------------------*/
/*! @def    FIFO_SIZE
 *  @brief  The watermark value configured for FXAS21002 FIFO Buffer.
 */
#define FIFO_SIZE 4

/*******************************************************************************
 * Constants
 ******************************************************************************/
/*! Prepare the register write list to configure FXAS21002 in FIFO mode. */
const registerwritelist_t fxas21002_Config_with_Fifo[] = {
    /*! Configure CTRL_REG1 register to put FXAS21002 to 12.5Hz sampling rate. */
    {FXAS21002_CTRL_REG1, FXAS21002_CTRL_REG1_DR_12_5HZ, FXAS21002_CTRL_REG1_DR_MASK},
    /*! Configure F_SETUP register to set FIFO in Stop mode, Watermark of 16. */
    {FXAS21002_F_SETUP, FXAS21002_F_SETUP_F_MODE_STOP_MODE | (FIFO_SIZE << FXAS21002_F_SETUP_F_WMRK_SHIFT),
     FXAS21002_F_SETUP_F_MODE_MASK | FXAS21002_F_SETUP_F_WMRK_MASK},
    {FXAS21002_CTRL_REG3, FXAS21002_CTRL_REG3_WRAPTOONE_ROLL_DATA, FXAS21002_CTRL_REG3_WRAPTOONE_MASK},
    __END_WRITE_DATA__};

/*! Prepare the register read list to read FXAS21002 FIFO event. */
const registerreadlist_t fxas21002_Fifo_Event[] = {{.readFrom = FXAS21002_F_EVENT, .numBytes = 1}, __END_READ_DATA__};

/*! Prepare the register read list to read the raw gyro data from the FXAS21002. */
const registerreadlist_t fxas21002_Output_Values[] = {
    {.readFrom = FXAS21002_OUT_X_MSB, .numBytes = FXAS21002_GYRO_DATA_SIZE * FIFO_SIZE}, __END_READ_DATA__};

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    int32_t status;
    uint8_t fifoEvent;
    uint8_t data[FXAS21002_GYRO_DATA_SIZE * FIFO_SIZE];
    fxas21002_gyrodata_t rawData;

    ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER; // Now using the shield.h value!!!
    fxas21002_i2c_sensorhandle_t FXAS21002drv;

    /*! Initialize the MCU hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_SystickEnable();
    BOARD_InitDebugConsole();

    PRINTF("\r\n ISSDK FXAS21002 sensor driver example demonstration with fifo mode\r\n");

    /*! Initialize the I2C driver. */
    status = I2Cdrv->Initialize(I2C_S_SIGNAL_EVENT);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Initialization Failed\r\n");
        return -1;
    }

    /*! Set the I2C Power mode. */
    status = I2Cdrv->PowerControl(ARM_POWER_FULL);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Power Mode setting Failed\r\n");
        return -1;
    }

    /*! Set the I2C bus speed. */
    status = I2Cdrv->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Control Mode setting Failed\r\n");
        return -1;
    }

    /*! Initialize the FXAS21002 sensor driver. */
    status = FXAS21002_I2C_Initialize(&FXAS21002drv, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, FXAS21002_I2C_ADDR,
                                      FXAS21002_WHO_AM_I_WHOAMI_PROD_VALUE);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n Sensor Initialization Failed\r\n");
        return -1;
    }
    PRINTF("\r\n Successfully Initiliazed Sensor\r\n");

    /*!  Set the task to be executed while waiting for I2C transactions to complete. */
    FXAS21002_I2C_SetIdleTask(&FXAS21002drv, (registeridlefunction_t)SMC_SetPowerModeWait, SMC);

    /*! Configure the FXAS21002 sensor driver. */
    status = FXAS21002_I2C_Configure(&FXAS21002drv, fxas21002_Config_with_Fifo);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n FXAS21002 Sensor Configuration Failed, Err = %d\r\n", status);
        return -1;
    }
    PRINTF("\r\n Successfully Applied FXAS21002 Sensor Configuration\r\n");

    for (;;) /* Forever loop */
    {
        /*! Wait for the FIFO watermark event. */
        status = FXAS21002_I2C_ReadData(&FXAS21002drv, fxas21002_Fifo_Event, &fifoEvent);
        if (0 == (fifoEvent & FXAS21002_F_EVENT_F_EVENT_MASK))
        {
            continue;
        }

        /*! Read the raw sensor data from the FXAS21002. */
        status = FXAS21002_I2C_ReadData(&FXAS21002drv, fxas21002_Output_Values, data);
        if (ARM_DRIVER_OK != status)
        {
            PRINTF("\r\n Read Failed. \r\n");
            return -1;
        }

        for (uint8_t i = 0; i < FIFO_SIZE; i++)
        {
            /*! Convert the raw sensor data to signed 16-bit container for display to the debug port. */
            rawData.gyro[0] =
                ((int16_t)data[i * FXAS21002_GYRO_DATA_SIZE + 0] << 8) | data[i * FXAS21002_GYRO_DATA_SIZE + 1];
            rawData.gyro[1] =
                ((int16_t)data[i * FXAS21002_GYRO_DATA_SIZE + 2] << 8) | data[i * FXAS21002_GYRO_DATA_SIZE + 3];
            rawData.gyro[2] =
                ((int16_t)data[i * FXAS21002_GYRO_DATA_SIZE + 4] << 8) | data[i * FXAS21002_GYRO_DATA_SIZE + 5];
        }
        /* NOTE: PRINTF is relatively expensive in terms of CPU time, specially when used with-in execution loop. */
        PRINTF("\r\n Gyro X = %d  Y = %d  Z = %d\r\n", rawData.gyro[0], rawData.gyro[1], rawData.gyro[2]);
        ASK_USER_TO_RESUME(100 / FIFO_SIZE); /* Ask for user input after processing 100 samples. */
    }
}
