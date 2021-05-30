/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file pedometer_stepcount_fxos8700.c
 * @brief The pedometer_stepcount_fxos8700.c file implements the ISSDK prdometer
 *        example using FXOS8700 sensor as the acceleration source.
 */

/* SDK Includes */
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

/* CMSIS Includes */
#include "Driver_I2C.h"

/* ISSDK Includes */
#include "issdk_hal.h"
#include "pedometer.h"
#include "fxos8700_drv.h"

/*******************************************************************************
 * Macro Definitions
 ******************************************************************************/
#define RAW_ACCEL_DATA_SIZE (6)

/*******************************************************************************
 * Constants
 ******************************************************************************/

/* Configure the fxos8700 to STANDBY and 50Hz sampling rate.
 * fxos8700 is used as the acceleration source for the pedometer */
const registerwritelist_t fxos8700_Config[] = {
    /*! System and Control registers. */
    /*! Configure the fxos8700 to 50Hz sampling rate. */
    {FXOS8700_CTRL_REG1, FXOS8700_CTRL_REG1_DR_SINGLE_50_HZ, 0},
    /* Configure the fxos8700 as accel only mode.*/
    {FXOS8700_M_CTRL_REG2, FXOS8700_M_CTRL_REG2_M_AUTOINC_ACCEL_ONLY_MODE, 0},
    __END_WRITE_DATA__};
/* Pedometer configuration. These configuration are algorithm and user dependent data. */
static const pedometer_config_t pedo_config = {
    .sleepcount_threshold = 1,
    .bits = {.config = 1},
    .keynetik =
        {
            .height = 175,
            .weight = 80,
            .filtersteps = PEDO_FILTER_STEPS_DEFAULT,
            .bits =
                {
                    .filtertime = PEDO_FILTER_TIME_DEFAULT,
                },
            .speedperiod = PEDO_SPEED_PERIOD_DEFAULT,
            .stepthreshold = PEDO_STEP_THRESHOLD_DEFAULT,
        },
    .stepcoalesce = 1,
    .oneG = PEDO_ONEG_2G,             // It is the One G representation in 2G scale.
    .frequency = PEDO_FREQHZ_DEFAULT, // It is 50 HZ

};

/*! Command definition to read the Data Ready Status */
const registerreadlist_t FXOS8700_STATUS_READ[] = {{.readFrom = FXOS8700_STATUS, .numBytes = 1}, __END_READ_DATA__};

/*! Command definition to read the Accel Data */
const registerreadlist_t FXOS8700_ACCEL_READ[] = {{.readFrom = FXOS8700_OUT_X_MSB, .numBytes = RAW_ACCEL_DATA_SIZE},
                                                  __END_READ_DATA__};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */

int main(void)
{
    int32_t status;
    uint8_t readBuffer[RAW_ACCEL_DATA_SIZE];
    fxos8700_acceldata_t rawData;
    FXOS8700_DR_STATUS_t drStatus;

    ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER; // Now using the shield.h value!!!
    fxos8700_i2c_sensorhandle_t FXOS8700drv;
    pedometer_t pedometer; /* This handle holds the current configuration and status value for the pedometer.*/

    /*! Initialize the MCU hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\n ISSDK Pedometer Example using FXOS8700 sensor as the acceleration source\r\n");

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

    /*! Initialize the FXOS8700 sensor driver. */
    status = FXOS8700_I2C_Initialize(&FXOS8700drv, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, FXOS8700_I2C_ADDR,
                                     FXOS8700_WHO_AM_I_PROD_VALUE);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n Sensor Initialization Failed\r\n");
        return -1;
    }
    PRINTF("\r\n Successfully Initialized Sensor\r\n");

    /*!  Set the task to be executed while waiting for I2C transactions to complete. */
    FXOS8700_I2C_SetIdleTask(&FXOS8700drv, (registeridlefunction_t)SMC_SetPowerModeVlpr, SMC);

    /*! Configure the fxos8700 sensor driver. */
    status = FXOS8700_I2C_Configure(&FXOS8700drv, fxos8700_Config);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n FXOS8700 Sensor Configuration Failed, Err = %d\r\n", status);
        return -1;
    }
    PRINTF("\r\n Successfully Applied FXOS8700 Sensor Configuration\r\n");

    /* Initialize the pedometer*/
    pedometer_init(&pedometer);

    /*configure the pedometer*/
    pedometer_configure(&pedometer, &pedo_config);

    for (;;)
    {
        /* Poll, waiting for the data ready event.*/
        status = FXOS8700_I2C_ReadData(&FXOS8700drv, FXOS8700_STATUS_READ, (uint8_t *)&drStatus);
        if (0 == (drStatus.b.zyxdr))
        {
            continue;
        }

        /* Read the raw accel data from the fxos8700.*/
        status = FXOS8700_I2C_ReadData(&FXOS8700drv, FXOS8700_ACCEL_READ, readBuffer);
        if (ARM_DRIVER_OK != status)
        {
            continue;
        }
        rawData.accel[0] = ((int16_t)readBuffer[0] << 8) | readBuffer[1];
        rawData.accel[1] = ((int16_t)readBuffer[2] << 8) | readBuffer[3];
        rawData.accel[2] = ((int16_t)readBuffer[4] << 8) | readBuffer[5];

        /*  Execute the pedometer Algorithm */
        pedometer_run(&pedometer, (ped_accel_t *)&rawData.accel);

        PRINTF("\r\n StepCount = %d \r\n", pedometer.status.stepcount);
    }
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
