/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file pedometer_stepcount_mma8652.c
 * @brief The pedometer_stepcount_mma8652.c file implements the ISSDK prdometer
 *        example using MMA8652 sensor as the acceleration source.
 */

/* SDK Includes */
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

/* CMSIS Includes */
#include "Driver_I2C.h"

/* ISSDK Includes */
#include "issdk_hal.h"
#include "pedometer.h"
#include "mma865x_drv.h"

/*******************************************************************************
 * Macro Definitions
 ******************************************************************************/
#define MMA865x_ACCEL_DATA_SIZE (6) /* 2 byte X,Y,Z Axis Data each. */

/*******************************************************************************
 * Constants
 ******************************************************************************/
/*! Prepare the register write list to configure MMA865x in Normal mode. */
const registerwritelist_t cMma865xConfigNormal[] =
    {/*! Configure the MMA865x to set FS Range as 2g. */
     {MMA865x_XYZ_DATA_CFG, MMA865x_XYZ_DATA_CFG_FS_2G, MMA865x_XYZ_DATA_CFG_FS_MASK},
     /*! Configure the MMA865x to set ODR to 50Hz. */
     {MMA865x_CTRL_REG1, MMA865x_CTRL_REG1_DR_50HZ, MMA865x_CTRL_REG1_DR_MASK},
     /*! Configure the MMA865x to set High Resolution mode. */
     {MMA865x_CTRL_REG2, MMA865x_CTRL_REG2_MODS_HR, MMA865x_CTRL_REG2_MODS_MASK},
     __END_WRITE_DATA__};

/*! Prepare the register read list to read the Data Ready Status from MMA865x. */
const registerreadlist_t cMma865xDataReady[] = {{.readFrom = MMA865x_STATUS, .numBytes = 1}, __END_READ_DATA__};

/*! Prepare the register read list to read the raw Accel data from MMA865x. */
const registerreadlist_t cMma865xOutputValues[] = {{.readFrom = MMA865x_OUT_X_MSB, .numBytes = MMA865x_ACCEL_DATA_SIZE},
                                                   __END_READ_DATA__};

/* Pedometer configuration. These configuration are algorithm and user dependent data. */
static const pedometer_config_t pedo_config = {
    .sleepcount_threshold = 1,
    .bits                 = {.config = 1},
    .keynetik =
        {
            .height      = 175,
            .weight      = 80,
            .filtersteps = PEDO_FILTER_STEPS_DEFAULT,
            .bits =
                {
                    .filtertime = PEDO_FILTER_TIME_DEFAULT,
                },
            .speedperiod   = PEDO_SPEED_PERIOD_DEFAULT,
            .stepthreshold = PEDO_STEP_THRESHOLD_DEFAULT,
        },
    .stepcoalesce = 1,
    .oneG         = PEDO_ONEG_2G,        // It is the One G representation in 2G scale.
    .frequency    = PEDO_FREQHZ_DEFAULT, // It is 50 HZ

};

/*******************************************************************************
 * Globals
 ******************************************************************************/
volatile uint8_t gMma865xDataReady;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */

int main(void)
{
    int32_t status;
    uint8_t data[MMA865x_ACCEL_DATA_SIZE];
    mma865x_acceldata_t rawData;

    ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER; // Now using the shield.h value!!!
    mma865x_i2c_sensorhandle_t mma865xDriver;
    pedometer_t pedometer; /* This handle holds the current configuration and status value for the pedometer.*/

    /*! Initialize the MCU hardware */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\n ISSDK Pedometer Example using MMA8652 sensor as the acceleration source\r\n");

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

    /*! Initialize the MMA865x sensor driver. */
    status = MMA865x_I2C_Initialize(&mma865xDriver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, MMA8652_I2C_ADDR,
                                    MMA8652_WHOAMI_VALUE);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n Sensor Initialization Failed\r\n");
        return -1;
    }
    else
    {
        PRINTF("\r\n Successfully Initiliazed Sensor\r\n");
    }
    gMma865xDataReady = 0; /* Set data not ready, data will be ready after sensor is configured. */

    /*!  Set the task to be executed while waiting for I2C transactions to complete. */
    MMA865x_I2C_SetIdleTask(&mma865xDriver, (registeridlefunction_t)SMC_SetPowerModeWait, SMC);

    /*! Configure the MMA865x sensor driver with Non FIFO mode. */
    status = MMA865x_I2C_Configure(&mma865xDriver, cMma865xConfigNormal);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n MMA865x Sensor Configuration Failed, Err = %d\r\n", status);
        return -1;
    }
    else
    {
        PRINTF("\r\n Successfully Applied MMA865x Sensor Configuration\r\n");
    }

    /*! Initialize the pedometer*/
    pedometer_init(&pedometer);

    /*! Configure the pedometer*/
    pedometer_configure(&pedometer, &pedo_config);

    for (;;) // Forever loop
    {
        /*! Read the data ready status from MMA865x*/
        status = MMA865x_I2C_ReadData(&mma865xDriver, cMma865xDataReady, (uint8_t *)&gMma865xDataReady);
        if (0 == (gMma865xDataReady & MMA865x_STATUS_ZYXDR_MASK))
        {
            continue;
        }

        /*! Read the raw sensor data from the MMA865x. */
        status = MMA865x_I2C_ReadData(&mma865xDriver, cMma865xOutputValues, data);
        if (ARM_DRIVER_OK != status)
        {
            continue; // Read did not work, so loop.
        }

        /*! Convert the raw sensor data for display to the debug port. */
        rawData.accel[0] = ((int16_t)data[0] << 8) | data[1];
        rawData.accel[1] = ((int16_t)data[2] << 8) | data[3];
        rawData.accel[2] = ((int16_t)data[4] << 8) | data[5];

        /*!  Execute the pedometer Algorithm */
        pedometer_run(&pedometer, (ped_accel_t *)&rawData.accel);

        PRINTF("\r\n StepCount = %d \r\n", pedometer.status.stepcount);
    }
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
