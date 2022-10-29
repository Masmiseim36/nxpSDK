/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file dbap_app.c
 * @brief The dbap_app.c file implements the DBAP sensor driver example
 *
 */

//-----------------------------------------------------------------------
// SDK Includes
//-----------------------------------------------------------------------
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "systick_utils.h"
#include "issdk_hal.h"
//-----------------------------------------------------------------------
// CMSIS Includes
//-----------------------------------------------------------------------
#include "Driver_I2C.h"
#include "dbap_drv.h"
//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------
#define FXPS7250D4 1
#define DBAP_I2C_ADDR     0x60
#define DBAP_DATA_SIZE    0x3
#define TO_LSB             68
#define TO_SEN             1
#if  FXPS7250D4
#define PABSOffLSB         28661.6
#define PABSSENS           30.42
#elif FXPS7115D4
#define PABSOffLSB         25538.8
#define PABSSENS           69.96
#elif FXPS7550D4
#define PABSOffLSB         28990
#define PABSSENS           14
#else
#define PABSOffLSB         28661.6
#define PABSSENS           30.42
#endif
//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------

/*! @brief Register settings for confirmation. */
const registerwritelist_t cDBAPConfig[] = {
    {FXPS7250_DSP_CFG_U1, FXPS7250_DSP_CFG_U1_LPF_370HZ_2POLE, FXPS7250_DSP_CFG_U1_LPF_MASK},
    __END_WRITE_DATA__};

/*! @brief Address of Part Number */
const registerreadlist_t cDBAPPartNumber[] = {
    {.readFrom = FXPS7250_PN0, .numBytes = 2},
	//{.readFrom = FXPS7250_P_CAL_ZERO_H, .numBytes = 1},
    __END_READ_DATA__};

/*! @brief Address and size of Raw Pressure data and TemperatureData */
const registerreadlist_t cDBAPPTData[] = {
    {.readFrom = FXPS7250_SNSDATA0_L, .numBytes = 2},
	//{.readFrom = FXPS7250_SNSDATA0_H, .numBytes = 1},
	{.readFrom = FXPS7250_TEMPERATURE, .numBytes = 1},
     __END_READ_DATA__};

/*! @brief Address of Device Status */
const registerreadlist_t cDBAPDeviceStatus[] = {
    {.readFrom = FXPS7250_DEVSTAT_COPY, .numBytes = 1},
    __END_READ_DATA__};
//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------
/*!
 * @brief Main function
 */
int main(void)
{
    int32_t status;
    int8_t tempInDegrees;
    float pressureKPa;
    dbap_pressuredata_t rawData;
    uint16_t partNumber;
    uint8_t deviceStatus, data[DBAP_DATA_SIZE];
    uint8_t data_part[11];
    dbap_i2c_sensorhandle_t dbapDriver;
    ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_SystickEnable();
    BOARD_InitDebugConsole();

    PRINTF("\r\n DBAP Sensor Example !\r\n");

    /*! Initialize the I2C driver. */
    status = I2Cdrv->Initialize(I2C_S_SIGNAL_EVENT);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Initialization Failed.\r\n");
        return -1;
    }

    /*! Set the I2C Power mode. */
    status = I2Cdrv->PowerControl(ARM_POWER_FULL);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Power Mode setting Failed.\r\n");
        return -1;
    }

    /*! Set the I2C bus speed. */
    status = I2Cdrv->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Control Mode setting Failed.\r\n");
        return -1;
    }

	status = DBAP_I2C_Initialize(&dbapDriver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, DBAP_I2C_ADDR,
			FXPS7250D4_WHOAMI_VALUE);
	if (SENSOR_ERROR_NONE == status)
	{
		PRINTF("\r\n Initiliazed DBAP Sensor with WHO_AM_I = [0x%X].\r\n", FXPS7250D4_WHOAMI_VALUE);
	}
	DBAP_I2C_ReadData(&dbapDriver, cDBAPPartNumber, data_part);
	partNumber = (data_part[0]<<8)|data_part[1];
	PRINTF("\r\n DBAP Part Number = [0x%X].\r\n", partNumber);

    /*!  Set the task to be executed while waiting for I2C transactions to complete. */
    DBAP_I2C_SetIdleTask(&dbapDriver, (registeridlefunction_t)SMC_SetPowerModeVlpr, SMC);

    /*! Configure the DBAP sensor. */
    status = DBAP_I2C_Configure(&dbapDriver, cDBAPConfig);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\nDBAP now active and entering data read loop...\r\n");
        return -1;
    }
    for (;;) /* Forever loop */
    {
        /* Read sensor status register */
        status = DBAP_I2C_ReadData(&dbapDriver, cDBAPDeviceStatus, &deviceStatus);
        if (ARM_DRIVER_OK != status)
        {
            PRINTF("\r\n Read Failed. \r\n");
            return -1;
        }

        /*! Read new raw sensor data from the DBAP. */
        status = DBAP_I2C_ReadData(&dbapDriver, cDBAPPTData, data);
        if (ARM_DRIVER_OK != status)
        {
            PRINTF("\r\n Read Failed. \r\n");
            return -1;
        }

        /*! Process the sample and convert the raw sensor data. */
        rawData.pressure = ((uint16_t)(data[1]) << 8) | data[0];
        pressureKPa = (rawData.pressure - PABSOffLSB ) / PABSSENS;

        rawData.temperature = (int8_t)(data[2]);
        tempInDegrees = (rawData.temperature - TO_LSB)/TO_SEN;


        PRINTF("\r\n Raw Pressure   = %d \r\n", rawData.pressure);
        PRINTF("\r\n Pressure    = %0.3f KPa", pressureKPa);
        PRINTF("\r\n Temperature = %d in degree\r\n", tempInDegrees);

        ASK_USER_TO_RESUME(50); /* Ask for user input after processing 50 samples. */
    }
}
