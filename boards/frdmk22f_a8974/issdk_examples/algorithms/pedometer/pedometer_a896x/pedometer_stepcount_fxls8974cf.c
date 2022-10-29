/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file pedometer_stepcount_fxls8974cf.c
 * @brief The pedometer_stepcount_fxls8974cf.c file implements the ISSDK FXLS8974CF
 *        sensor driver example demonstration for Motion Activated Pedometer.
 */

//-----------------------------------------------------------------------
// SDK Includes
//-----------------------------------------------------------------------
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"

//-----------------------------------------------------------------------
// CMSIS Includes
//-----------------------------------------------------------------------
#include "Driver_I2C.h"

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "issdk_hal.h"
#include "pedometer.h"
#include "gpio_driver.h"
#include "fxls8974_drv.h"

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------
#define FXLS8974_DATA_SIZE 6  /* 2 byte X,Y,Z Axis Data each. */

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------
/*! @brief FXLS896x Motion based Pedometer Register Write List. */
const registerwritelist_t cFxls8974ConfigInitialize[] = {
    /* Set Full-scale range as 2G. */
    {FXLS8974_SENS_CONFIG1, FXLS8974_SENS_CONFIG1_FSR_2G, FXLS8974_SENS_CONFIG1_FSR_MASK},
   /* Set ODR */
    {FXLS8974_SENS_CONFIG3, FXLS8974_SENS_CONFIG3_WAKE_ODR_50HZ, FXLS8974_SENS_CONFIG3_WAKE_ODR_MASK},
    __END_WRITE_DATA__};

/*! @brief Address of DATA Ready Status Register. */
const registerreadlist_t cFxls8974DRDYEvent[] = {{.readFrom = FXLS8974_INT_STATUS, .numBytes = 1}, __END_READ_DATA__};

/*! @brief Address of Data Output Registers. */
const registerreadlist_t cFxls8974Output[] = {{.readFrom = FXLS8974_OUT_X_LSB, .numBytes = FXLS8974_DATA_SIZE}, __END_READ_DATA__};

/*! @brief Pedometer Mode Name Strings. */
const char *pActivity[5] = {"Unknown ", "Rest    ", "Walking ", "Jogging ", "Running "};

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
/* Pedometer configuration. These configuration are algorithm and user dependent data. */
static const pedometer_config_t cPedoConfig = {
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

gpioConfigKSDK_t gpioConfigINT1 =
{
    .pinConfig = {kGPIO_DigitalInput, 0},
    .portPinConfig = {.pullSelect = kPORT_PullUp, .mux = kPORT_MuxAsGpio},
    .interruptMode = kPORT_InterruptFallingEdge,
};

//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------

/*! -----------------------------------------------------------------------
 *  @brief       This is the The main function implementation.
 *  @details     This function invokes board initializes routines, then then brings up the sensor and
 *               finally enters an endless loop to continuously read available samples.
 *  @param[in]   void This is no input parameter.
 *  @return      void  There is no return value.
 *  -----------------------------------------------------------------------*/
int main(void)
{
    int32_t status;
    uint8_t whoami;
    uint8_t dataReady;
    uint16_t lastReportedSteps;
    uint8_t data[FXLS8974_DATA_SIZE];

    pedometer_t pedometer; /* This handle holds the current configuration and status value for the pedometer.*/
    fxls8974_acceldata_t rawData;
    fxls8974_i2c_sensorhandle_t fxls8974Driver;

    ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER; // Now using the shield.h value!!!
    GENERIC_DRIVER_GPIO *pGpioDriver = &Driver_GPIO_KSDK;

    /*! Initialize the MCU hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\n ISSDK FXLS896x sensor driver example for Pedometer.\r\n");

    /*! Initialize RGB LED pin used by FRDM board */
    pGpioDriver->pin_init(&GREEN_LED, GPIO_DIRECTION_OUT, NULL, NULL, NULL);

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

    /*! Initialize FXLS896x sensor driver. */
    status = FXLS8974_I2C_Initialize(&fxls8974Driver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, FXLS8974_I2C_ADDR,
                                     &whoami);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n Sensor Initialization Failed\r\n");
        return -1;
    }
    if ((FXLS8964_WHOAMI_VALUE == whoami) || (FXLS8967_WHOAMI_VALUE == whoami))
    {
    	PRINTF("\r\n Successfully Initialized Gemini with WHO_AM_I = 0x%X\r\n", whoami);
    }
    else if ((FXLS8974_WHOAMI_VALUE == whoami) || (FXLS8968_WHOAMI_VALUE == whoami))
    {
    	PRINTF("\r\n Successfully Initialized Timandra with WHO_AM_I = 0x%X\r\n", whoami);
    }
    else if (FXLS8962_WHOAMI_VALUE == whoami)
    {
    	PRINTF("\r\n Successfully Initialized Newstein with WHO_AM_I = 0x%X\r\n", whoami);
    }
    else
    {
    	PRINTF("\r\n Bad WHO_AM_I = 0x%X\r\n", whoami);
        return -1;
    }

    /*!  Set the task to be executed while waiting for I2C transactions to complete. */
    FXLS8974_I2C_SetIdleTask(&fxls8974Driver, (registeridlefunction_t)SMC_SetPowerModeWait, SMC);

    /* Apply FXLS896x Configuration for Motion Activated Pedometer. */
    /*! Configure the FXLS896x sensor. */
    status = FXLS8974_I2C_Configure(&fxls8974Driver, cFxls8974ConfigInitialize);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n FXLS896x Sensor Configuration Failed, Err = %d\r\n", status);
        return -1;
    }
    PRINTF("\r\n Successfully Applied FXLS8974 Sensor Configuration\r\n");

    /*! Initialize the pedometer*/
    pedometer_init(&pedometer);
    PRINTF("\r\n Pedometer successfully Initialized.");
    PRINTF("\r\n\r\n Ready for measurements.");
    
    for (;;) /* Forever loop for Motion Detection */
    {
        /*! Configure the pedometer*/
        pedometer_configure(&pedometer, &cPedoConfig);

        pGpioDriver->set_pin(&GREEN_LED);

        for (;;) /* Loop for Motion Detection */
        {   /* In ISR Mode we do not need to check Data Ready Register.
             * The receipt of interrupt will indicate event is ready. */
          /*! Wait for data ready from the FXLS896x. */
          status = FXLS8974_I2C_ReadData(&fxls8974Driver, cFxls8974DRDYEvent, &dataReady);
          if (0 == (dataReady & FXLS8974_INT_STATUS_SRC_DRDY_MASK))
          {
              continue;
          }

            /*! Read the Output from the FXLS896x. */
            status = FXLS8974_I2C_ReadData(&fxls8974Driver, cFxls8974Output, data);
            if (ARM_DRIVER_OK != status)
            {
                PRINTF("\r\n ERROR : Read Data Failed!\r\n");
                return -1;
            }

            /*! Convert the raw sensor data for feeding to pedometer algorithm. */
            rawData.accel[0] = ((int16_t)data[1] << 8) | data[0];
            rawData.accel[0] *= 16; /* Pedometer requires signed 12-bit Left justified data @1024 counts/g. */
            rawData.accel[1] = ((int16_t)data[3] << 8) | data[2];
            rawData.accel[1] *= 16; /* Pedometer requires signed 12-bit Left justified data @1024 counts/g. */
            rawData.accel[2] = ((int16_t)data[5] << 8) | data[4];
            rawData.accel[2] *= 16; /* Pedometer requires signed 12-bit Left justified data @1024 counts/g. */

            /*!  Execute the pedometer Algorithm */
            pedometer_run(&pedometer, (ped_accel_t *)&rawData.accel);

            /* Display Steps when there is a change */
            if (pedometer.status.stepcount && pedometer.status.stepcount != lastReportedSteps)
            {
                pGpioDriver->toggle_pin(&GREEN_LED);
                lastReportedSteps = pedometer.status.stepcount;
                if(pedometer.status.stepcount == 4)
                {
                    PRINTF("\r\n\r\n | Steps |\r\n");
                }
                PRINTF("\033[K | %5d |\r",pedometer.status.stepcount);
            }
        }
    }
}
