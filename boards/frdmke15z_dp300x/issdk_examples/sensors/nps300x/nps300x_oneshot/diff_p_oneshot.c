/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file diff_p_oneshot.c
 * @brief The diff_p_oneshot.c file implements the ISSDK DIFF_P sensor driver
 *        example demonstration with One-Shot mode.
 */

//-----------------------------------------------------------------------
// SDK Includes
//-----------------------------------------------------------------------
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

//-----------------------------------------------------------------------
// CMSIS Includes
//-----------------------------------------------------------------------
#include "Driver_I2C.h"

/* ISSDK Includes */
#include "issdk_hal.h"
#include "diff_p_drv.h"
#include "systick_utils.h"

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------
#define DIFF_P_DATA_SIZE (3) /* 2 byte Pressure and 1 byte Temperature. */

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
volatile bool diff_pInterrupt = false;

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------
/*! @brief Register settings for OST Mode. */
const registerwritelist_t cDiffPConfigOneShot[] = {
    {DIFF_P_CTRL_REG3, DIFF_P_CTRL_REG3_IPOL1_ACTIVE_HIGH, DIFF_P_CTRL_REG3_IPOL1_MASK},
    {DIFF_P_CTRL_REG1, DIFF_P_CTRL_REG1_OSR_OSR2048, DIFF_P_CTRL_REG1_OSR_MASK},
    {DIFF_P_INT_MASK0, DIFF_P_INT_MASK0_TDR_INT_EN | DIFF_P_INT_MASK0_PDR_INT_EN,
     DIFF_P_INT_MASK0_TDR_MASK | DIFF_P_INT_MASK0_PDR_MASK},
    __END_WRITE_DATA__};

/*! @brief Register settings for Triggring One-Shot Sampling. */
const registerwritelist_t cDiffPTriggerOneShot[] = {
    /* Set the One ShoT Bit. */
    {DIFF_P_CTRL_REG1, DIFF_P_CTRL_REG1_OST_ONESHOT, DIFF_P_CTRL_REG1_OST_MASK},
    __END_WRITE_DATA__};

/*! @brief Register settings for Clearing Pressure and Temperature Data Ready Bits. */
const registerwritelist_t cDiffPClearStatusBits[] = {
    {DIFF_P_INT_STATUS_0, 0x00, DIFF_P_INT_STATUS_0_TDR_MASK | DIFF_P_INT_STATUS_0_PDR_MASK}, __END_WRITE_DATA__};

/*! @brief Address of Status Register. */
const registerreadlist_t cDiffPStatus[] = {{.readFrom = DIFF_P_INT_STATUS_0, .numBytes = 1}, __END_READ_DATA__};

/*! @brief Address and size of Raw Pressure+Temperature Data in Normal Mode. */
const registerreadlist_t cDiffPOutputNormal[] = {{.readFrom = DIFF_P_OUT_P_LSB, .numBytes = DIFF_P_DATA_SIZE},
                                                 __END_READ_DATA__};

//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------
/*! -----------------------------------------------------------------------
 *  @brief       This is the Sensor Data Ready ISR implementation.
 *  @details     This function sets the flag which indicates if a new sample(s) is available for reading.
 *  @param[in]   pUserData This is a void pointer to the instance of the user specific data structure for the ISR.
 *  @return      void  There is no return value.
 *  @constraints None
 *  @reeentrant  Yes
 *  -----------------------------------------------------------------------*/
void diff_p_isr(void *pUserData)
{ /*! @brief Set flag to indicate Sensor has signalled data ready. */
    diff_pInterrupt = true;
}

/*!
 * @brief Main function
 */
int main(void)
{
    int32_t status;
    int8_t tempInDegrees;
    float pressureInPascals;
    diff_p_pressuredata_t rawData;
    uint8_t whoAmI, conversionFactor;
    uint8_t dataReady, data[DIFF_P_DATA_SIZE];

    diff_p_i2c_sensorhandle_t diff_pDriver;
    ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER;
    GENERIC_DRIVER_GPIO *gpioDriver = &Driver_GPIO_KSDK;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_SystickEnable();
    BOARD_InitDebugConsole();

    PRINTF("\r\n ISSDK DIFF-P Sensor Example !\r\n");

    /*! Initialize INT1_DIFF_P pin used by FRDM board */
    gpioDriver->pin_init(&DIFF_P_INT1, GPIO_DIRECTION_IN, NULL, &diff_p_isr, NULL);

    /*! Initialize RGB LED pin used by FRDM board */
    gpioDriver->pin_init(&GREEN_LED, GPIO_DIRECTION_OUT, NULL, NULL, NULL);

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

    do
    { /*! Initialize DIFF_P sensor driver. */
        status = DIFF_P_I2C_Initialize(&diff_pDriver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, DIFF_P_I2C_ADDR,
                                       DIFF_P_NPS3000VV_WHOAMI_VALUE);
        if (SENSOR_ERROR_NONE == status)
        {
            whoAmI = DIFF_P_NPS3000VV_WHOAMI_VALUE;
            conversionFactor = NPS3000VV_PRESSURE_DIV_FACTOR;
            break;
        }
        status = DIFF_P_I2C_Initialize(&diff_pDriver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, DIFF_P_I2C_ADDR,
                                       DIFF_P_NPS3001DV_WHOAMI_VALUE);
        if (SENSOR_ERROR_NONE == status)
        {
            whoAmI = DIFF_P_NPS3001DV_WHOAMI_VALUE;
            conversionFactor = NPS3001DV_PRESSURE_DIV_FACTOR;
            break;
        }
        status = DIFF_P_I2C_Initialize(&diff_pDriver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, DIFF_P_I2C_ADDR,
                                       DIFF_P_NPS3002VV_WHOAMI_VALUE);
        if (SENSOR_ERROR_NONE == status)
        {
            whoAmI = DIFF_P_NPS3002VV_WHOAMI_VALUE;
            conversionFactor = NPS3002VV_PRESSURE_DIV_FACTOR;
            break;
        }
        status = DIFF_P_I2C_Initialize(&diff_pDriver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, DIFF_P_I2C_ADDR,
                                       DIFF_P_NPS3005DV_WHOAMI_VALUE);
        if (SENSOR_ERROR_NONE == status)
        {
            whoAmI = DIFF_P_NPS3005DV_WHOAMI_VALUE;
            conversionFactor = NPS3005DV_PRESSURE_DIV_FACTOR;
            break;
        }

        PRINTF("\r\n Sensor Initialization Failed.\r\n");
        return -1;
    } while (false);
    PRINTF("\r\n Initiliazed Sensor with WHO_AM_I = [0x%X].\r\n", whoAmI);

    /*!  Set the task to be executed while waiting for I2C transactions to complete. */
    DIFF_P_I2C_SetIdleTask(&diff_pDriver, (registeridlefunction_t)SMC_SetPowerModeVlpr, SMC);

    /*! We do not need to call DIFF_P_I2C_Configure() in this case as we are going to read samples on demand.
     *  Instead we directly write register settings for One-Shot Mode... */
    status = Sensor_I2C_Write(diff_pDriver.pCommDrv, &diff_pDriver.deviceInfo, diff_pDriver.slaveAddress,
                              cDiffPConfigOneShot);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\nDIFF_P now configured for OST Mode and entering data read loop...\r\n");
        return -1;
    }
    PRINTF("\r\n Successfully Applied Sensor Interrupt Configuration.\r\n");

    for (;;) /* Forever loop */
    {        /* Trigger acquisition of One Sample. */
        status = Sensor_I2C_Write(diff_pDriver.pCommDrv, &diff_pDriver.deviceInfo, diff_pDriver.slaveAddress,
                                  cDiffPTriggerOneShot);
        if (ARM_DRIVER_OK != status)
        {
            PRINTF("\r\nWrite Failed.\r\n");
            return SENSOR_ERROR_WRITE;
        }

        while (false == diff_pInterrupt)
        { /* Loop, until new sample is not available. */
            SMC_SetPowerModeWait(SMC);
        }

        /*! Clear the data ready flag, it will be set again by the ISR. */
        diff_pInterrupt = false;
        gpioDriver->toggle_pin(&GREEN_LED);

        do
        { /* Read INT_STATUS register */
            status = DIFF_P_I2C_ReadData(&diff_pDriver, cDiffPStatus, &dataReady);
            if (ARM_DRIVER_OK != status)
            {
                PRINTF("\r\n Read Failed. \r\n");
                return -1;
            }

            /*! Check for both data ready bits from the DIFF_P. */
            if ((DIFF_P_INT_STATUS_0_PDR_DRDY | DIFF_P_INT_STATUS_0_TDR_DRDY) !=
                (dataReady & (DIFF_P_INT_STATUS_0_PDR_MASK | DIFF_P_INT_STATUS_0_TDR_MASK)))
            { /* Loop, if new samples are not available. */
                continue;
            }
        } while (false);

        /*! Read new raw sensor data from the DIFF_P. */
        status = DIFF_P_I2C_ReadData(&diff_pDriver, cDiffPOutputNormal, data);
        if (ARM_DRIVER_OK != status)
        {
            PRINTF("\r\n Read Failed. \r\n");
            return -1;
        }

        /*! Explicitly clear the Status Bits. */
        status = Sensor_I2C_Write(diff_pDriver.pCommDrv, &diff_pDriver.deviceInfo, diff_pDriver.slaveAddress,
                                  cDiffPClearStatusBits);
        if (ARM_DRIVER_OK != status)
        {
            PRINTF("\r\n Write Failed. \r\n");
            return -1;
        }

        /*! Process the sample and convert the raw sensor data. */
        rawData.pressure = ((int16_t)(data[1]) << 8) | data[0];
        pressureInPascals = (1.0 * rawData.pressure) / conversionFactor;

        rawData.temperature = (int8_t)(data[2]);
        tempInDegrees = rawData.temperature;

        PRINTF("\r\n Pressure    = %0.3f Pa", pressureInPascals);
        PRINTF("\r\n Temperature = %d degC\r\n", tempInDegrees);
        ASK_USER_TO_RESUME(50); /* Ask for user input after processing 50 samples. */
    }
}
