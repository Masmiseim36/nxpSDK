/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * Description: This file implements the ISSDK FXAS21002 and FXOS8700 sensor 
 * drivers. It is used for reading data from FXAS21002 and FXOS8700 sensors.
 */

/*  SDK Includes */
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "board.h"

/* CMSIS Includes */
#include "Driver_I2C.h"

/* ISSDK Includes */
#include "issdk_hal.h"

#include "get_sensor_data.h"

/*******************************************************************************
 * Macro Definitions
 ******************************************************************************/
#define RAW_ACCELMAG_DATA_SIZE (12)
#define SAMPLE_NUM 1000

/*******************************************************************************
 * Constants
 ******************************************************************************/
/*! Prepare the register write list to configure FXAS21002 in non-FIFO mode. */
const registerwritelist_t fxas21002_Config_Normal[] = {
    /*! Configure CTRL_REG1 register to put FXAS21002 to 12.5Hz sampling rate. */
    {FXAS21002_CTRL_REG1, FXAS21002_CTRL_REG1_DR_12_5HZ, FXAS21002_CTRL_REG1_DR_MASK},
    __END_WRITE_DATA__};

/*! Prepare the register read list to read FXAS21002 DataReady status. */
const registerreadlist_t fxas21002_DRDY[] = {{.readFrom = FXAS21002_STATUS, .numBytes = 1}, __END_READ_DATA__};

/*! Prepare the register read list to read the raw gyro data from the FXAS21002. */
const registerreadlist_t fxas21002_Output_Values[] = {
    {.readFrom = FXAS21002_OUT_X_MSB, .numBytes = FXAS21002_GYRO_DATA_SIZE}, __END_READ_DATA__};

/*! Prepare the register write list to configure FXOS8700 in Hybrid mode. */
const registerwritelist_t fxos8700_Config_Hybrid[] = {
    /*! System and Control registers. */
    /*! Configure the FXOS8700 to 25Hz Hybrid sampling rate. */
    {FXOS8700_CTRL_REG1, FXOS8700_CTRL_REG1_DR_HYBRID_25_HZ, FXOS8700_CTRL_REG1_DR_MASK},
    {FXOS8700_M_CTRL_REG1, FXOS8700_M_CTRL_REG1_M_ACAL_EN | FXOS8700_M_CTRL_REG1_M_HMS_HYBRID_MODE,
                           FXOS8700_M_CTRL_REG1_M_ACAL_MASK | FXOS8700_M_CTRL_REG1_M_HMS_MASK}, /*! Enable the Hybrid Mode. */
    {FXOS8700_M_CTRL_REG2, FXOS8700_M_CTRL_REG2_M_AUTOINC_HYBRID_MODE | FXOS8700_M_CTRL_REG2_M_RST_CNT_DISABLE,
                           FXOS8700_M_CTRL_REG2_M_AUTOINC_MASK | FXOS8700_M_CTRL_REG2_M_RST_CNT_MASK}, /*! Enable the Data read with Hybrid Mode. */
    __END_WRITE_DATA__};

/*! Command definition to read the Data Ready Status */
const registerreadlist_t FXOS8700_STATUS_READ[] = {{.readFrom = FXOS8700_STATUS, .numBytes = 1}, __END_READ_DATA__};

/*! Command definition to read the Accel + Mag Data */
const registerreadlist_t FXOS8700_ACCELMAG_READ[] = {{.readFrom = FXOS8700_OUT_X_MSB, .numBytes = 12},
                                                     __END_READ_DATA__};

/*! Command definition to read the Accel + Mag Data (in 2 TXNs). */
const registerreadlist_t FXOS8700_ACCEL_MAG_READ[] = {{.readFrom = FXOS8700_OUT_X_MSB, .numBytes = 6},
                                                      {.readFrom = FXOS8700_M_OUT_X_MSB, .numBytes = 6},
                                                      __END_READ_DATA__};

static uint8_t dataGyro[FXAS21002_GYRO_DATA_SIZE];
static uint8_t dataAccel[RAW_ACCELMAG_DATA_SIZE];

static ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER; // Now using the shield.h value!!!
static fxas21002_i2c_sensorhandle_t FXAS21002drv;
static fxos8700_i2c_sensorhandle_t FXOS8700drv;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
status_t init_sensors(void)
{
    int32_t status;

    /* GPIO reset pin REVB */
    gpio_pin_config_t rst_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    /* Init output RST GPIO. This is quick fix for FRDM_STBC_AG01 REV B board*/
    GPIO_PinInit(GPIO1, 21, &rst_config);

    /*! Initialize the I2C driver. */
    status = I2Cdrv->Initialize(I2C_S_SIGNAL_EVENT);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Initialization Failed\r\n");
        return status;
    }

    /*! Set the I2C Power mode. */
    status = I2Cdrv->PowerControl(ARM_POWER_FULL);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Power Mode setting Failed\r\n");
        return status;
    }

    /*! Set the I2C bus speed. */
    status = I2Cdrv->Control(ARM_I2C_BUS_SPEED, ARM_I2C_BUS_SPEED_FAST);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n I2C Control Mode setting Failed\r\n");
        return status;
    }

    /* Init output RST GPIO. This is quick fix for FRDM_STBC_AG01 REV B board*/
    GPIO_PinWrite(GPIO1, 21, 1);

    /*! Initialize the FXAS21002 sensor driver. */
    status = FXAS21002_I2C_Initialize(&FXAS21002drv, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, FXAS21002_I2C_ADDR,
                                      FXAS21002_WHO_AM_I_WHOAMI_PROD_VALUE);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n FXAS21002 Sensor Initialization Failed\r\n");
        return status;
    }

    /*!  Set the task to be executed while waiting for I2C transactions to complete. */
    FXAS21002_I2C_SetIdleTask(&FXAS21002drv, (registeridlefunction_t)NULL, SMC);

    /*! Configure the FXAS21002 sensor driver. */
    status = FXAS21002_I2C_Configure(&FXAS21002drv, fxas21002_Config_Normal);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n FXAS21002 Sensor Configuration Failed, Err = %d\r\n", (int)status);
        return status;
    }

    /*! Initialize the FXOS8700 sensor driver. */
    status = FXOS8700_I2C_Initialize(&FXOS8700drv, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, FXOS8700_I2C_ADDR,
                                     FXOS8700_WHO_AM_I_PROD_VALUE);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n FXOS8700 Sensor Initialization Failed\r\n");
        return status;
    }

    /*!  Set the task to be executed while waiting for I2C transactions to complete. */
    FXOS8700_I2C_SetIdleTask(&FXOS8700drv, (registeridlefunction_t)NULL, SMC);

    /*! Configure the fxos8700 sensor driver. */
    status = FXOS8700_I2C_Configure(&FXOS8700drv, fxos8700_Config_Hybrid);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n FXOS8700 Sensor Configuration Failed, Err = %d\r\n", (int)status);
        return status;
    }

    return status;
}

status_t run_sensors(fxas21002_gyrodata_t *rawDataGyro,
    fxos8700_accelmagdata_t *rawDataAccel){

  status_t status = kStatus_Success;
  uint8_t dataReady[1] = {0};

  /*! Wait for data ready from the FXAS21002. */
  do{
    status = FXAS21002_I2C_ReadData(&FXAS21002drv, fxas21002_DRDY, dataReady);
  }while(0 == (dataReady[0] & FXAS21002_DR_STATUS_ZYXDR_MASK));

  /*! Wait for data ready from the FXOS8700. */
  do{
    status = FXOS8700_I2C_ReadData(&FXOS8700drv, FXOS8700_STATUS_READ, dataReady);
  }while(0 == (dataReady[0] & FXOS8700_DR_STATUS_ZYXDR_MASK));

  /*! Read the raw sensor data from the FXAS21002. */
  status = FXAS21002_I2C_ReadData(&FXAS21002drv, fxas21002_Output_Values, dataGyro);
  if (ARM_DRIVER_OK != status)
  {
      PRINTF("\r\n FXOS8700 Read Failed. \r\n");
      return status;
  }

  /*! Read the raw sensor data from the fxos8700. */
  status = FXOS8700_I2C_ReadData(&FXOS8700drv, FXOS8700_ACCELMAG_READ, dataAccel);
  if (ARM_DRIVER_OK != status)
  {
      PRINTF("\r\n FXOS8700 Read Failed. \r\n");
      return status;
  }

  registerreadlist_t FXOS8700_ACCELMAG_INIT_READ[] = {{.readFrom = FXOS8700_MAX_X_MSB, .numBytes = 12},
                                                       __END_READ_DATA__};
  uint8_t dataAccelInit[6];
  status = FXOS8700_I2C_ReadData(&FXOS8700drv, FXOS8700_ACCELMAG_INIT_READ, dataAccelInit);
  if (ARM_DRIVER_OK != status)
  {
      PRINTF("\r\n FXOS8700 Init Read Failed. \r\n");
      return -1;
  }

  /*! Convert the raw sensor data to signed 16-bit container for display to the debug port. */
  rawDataGyro->gyro[0] = ((int16_t)dataGyro[0] << 8) | dataGyro[1];
  rawDataGyro->gyro[1] = ((int16_t)dataGyro[2] << 8) | dataGyro[3];
  rawDataGyro->gyro[2] = ((int16_t)dataGyro[4] << 8) | dataGyro[5];

  /*! Convert the raw sensor data to signed 16-bit container for display to the debug port. */
  rawDataAccel->accel[0] = ((int16_t)dataAccel[0] << 8) | dataAccel[1];
  rawDataAccel->accel[0] /= 4;
  rawDataAccel->accel[1] = ((int16_t)dataAccel[2] << 8) | dataAccel[3];
  rawDataAccel->accel[1] /= 4;
  rawDataAccel->accel[2] = ((int16_t)dataAccel[4] << 8) | dataAccel[5];
  rawDataAccel->accel[2] /= 4;
  rawDataAccel->mag[0] = ((int16_t)dataAccel[6] << 8) | dataAccel[7];
  rawDataAccel->mag[1] = ((int16_t)dataAccel[8] << 8) | dataAccel[9];
  rawDataAccel->mag[2] = ((int16_t)dataAccel[10] << 8) | dataAccel[11];

  /* NOTE: PRINTF is relatively expensive in terms of CPU time, specially when used with-in execution loop. */
  /*
  PRINTF("\r\n Gyro X = %d  Y = %d  Z = %d\r\n", rawDataGyro->gyro[0], rawDataGyro->gyro[1], rawDataGyro->gyro[2]);
  PRINTF("\r\n Accel X = %d  Y = %d  Z = %d\r\n", rawDataAccel->accel[0], rawDataAccel->accel[1], rawDataAccel->accel[2]);
  PRINTF("\r\n Mag   X = %d  Y = %d  Z = %d\r\n", rawDataAccel->mag[0], rawDataAccel->mag[1], rawDataAccel->mag[2]);
  ASK_USER_TO_RESUME(100); Ask for user input after processing 100 samples.
  */

  return status;
}
