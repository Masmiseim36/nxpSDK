/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file pedometer_stepcount_mma8451.c
 * @brief The pedometer_stepcount_mma8451.c file implements the ISSDK prdometer
 *        example using MMA8451 sensor as the acceleration source.
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
#include "mma845x_drv.h"

/*******************************************************************************
 * Macro Definitions
 ******************************************************************************/
#define RAW_ACCEL_DATA_SIZE (6)

/*******************************************************************************
 * Constants
 ******************************************************************************/

/*! Prepare the register write list to configure MMA845x in poll mode. */
const registerwritelist_t mma845x_Config_Normal[] = {
    /*! Configure the MMA845x CTRL_REG1 to set mode to STANDBY and odr to 50Hz. */
    {MMA845x_CTRL_REG1, MMA845x_CTRL_REG1_MODE_STANDBY | MMA845x_CTRL_REG1_DR_50HZ, 0},
    /*! Configure the MMA845x to disable FIFO */
    {MMA845x_F_SETUP, MMA845x_F_SETUP_F_MODE_FIFODISABLED, 0},
    /*! Configure the MMA845x CTRL_REG2 to set the Oversampling mode to High Resolution. */
    {MMA845x_CTRL_REG2, MMA845x_CTRL_REG2_SMODS_HIGHRES, 0},
    __END_WRITE_DATA__};

/*! Prepare the register read list to read the Data Ready Status from MMA845x. */
const registerreadlist_t mma845x_Data_Ready[] = {{.readFrom = MMA845x_STATUS, .numBytes = 1}, __END_READ_DATA__};

/*! Prepare the register read list to read the raw Accel data from MMA845x. */
const registerreadlist_t mma845x_Output_Values[] = {
    {.readFrom = MMA845x_OUT_X_MSB, .numBytes = MMA845x_ACCEL_DATA_SIZE}, __END_READ_DATA__};

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

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */

int main(void)
{
    int32_t status;
    uint8_t dataReady;
    uint8_t data[MMA845x_ACCEL_DATA_SIZE];
    mma845x_acceldata_t rawData;

    ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER; // Now using the shield.h value!!!
    mma845x_i2c_sensorhandle_t MMA845xdrv;
    pedometer_t pedometer; /* This handle holds the current configuration and status value for the pedometer.*/

    /*! Initialize the MCU hardware */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\n ISSDK Pedometer Example using MMA8451 sensor as the acceleration source\r\n");

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

    /*! Initialize the MMA845x sensor driver. */
    status = MMA845x_I2C_Initialize(&MMA845xdrv, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, MMA845x_I2C_ADDR,
                                    MMA8451_WHO_AM_I_WHOAMI_VALUE);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n Sensor Initialization Failed\r\n");
        return -1;
    }
    PRINTF("\r\n Successfully Initiliazed Sensor\r\n");

    /*!  Set the task to be executed while waiting for I2C transactions to complete. */
    MMA845x_I2C_SetIdleTask(&MMA845xdrv, (registeridlefunction_t)SMC_SetPowerModeVlpr, SMC);

    /*! Configure the MMA845x sensor driver with Non FIFO mode. */
    status = MMA845x_I2C_Configure(&MMA845xdrv, mma845x_Config_Normal);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n MMA845x Sensor Configuration Failed, Err = %d\r\n", status);
        return -1;
    }
    else
    {
        PRINTF("\r\n Successfully Applied MMA845x Sensor Configuration\r\n");
    }

    /*! Initialize the pedometer*/
    pedometer_init(&pedometer);

    /*! Configure the pedometer*/
    pedometer_configure(&pedometer, &pedo_config);

    for (;;) // Forever loop
    {
        /*! Read the data ready status from MMA845x*/
        status = MMA845x_I2C_ReadData(&MMA845xdrv, mma845x_Data_Ready, &dataReady);
        if (0 == (dataReady & MMA845x_STATUS_ZYXDR_MASK))
        { /* Loop, if new sample is not available. */
            continue;
        }

        /*! Read the raw sensor data from the MMA845x. */
        status = MMA845x_I2C_ReadData(&MMA845xdrv, mma845x_Output_Values, (uint8_t *)&data[0]);
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
