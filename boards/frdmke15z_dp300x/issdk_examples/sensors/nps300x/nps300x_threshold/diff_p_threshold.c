/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file diff_p_threshold.c
 * @brief The diff_p_threshold.c file implements the ISSDK DIFF_P sensor driver
 *        example demonstration with Threshold detection mode.
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
/* Default Threshold is 800 Counts */
#define P_TGT0_MSB 0x03
#define P_TGT0_LSB 0x20

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
volatile bool diff_pInterrupt = false;

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------
/*! @brief Register settings for Normal (non buffered) mode. */
const registerwritelist_t cDiffPConfigNormal[] = {
    {DIFF_P_CTRL_REG3, DIFF_P_CTRL_REG3_IPOL1_ACTIVE_HIGH, DIFF_P_CTRL_REG3_IPOL1_MASK},
    {DIFF_P_CTRL_REG2, DIFF_P_CTRL_REG2_ODR_ODR6P25, DIFF_P_CTRL_REG2_ODR_MASK},
    {DIFF_P_CTRL_REG1, DIFF_P_CTRL_REG1_OSR_OSR512, DIFF_P_CTRL_REG1_OSR_MASK},
    {DIFF_P_INT_MASK1, DIFF_P_INT_MASK1_P_TGT0_INT_EN, DIFF_P_INT_MASK1_P_TGT0_MASK},
    {DIFF_P_P_TGT0_LSB, P_TGT0_LSB, 0},
    {DIFF_P_P_TGT0_MSB, P_TGT0_MSB, 0},
    __END_WRITE_DATA__};

/*! @brief Register settings for Clearing Pressure and Temperature Data Ready Bits. */
const registerwritelist_t cDiffPClearStatusBits[] = {{DIFF_P_INT_STATUS_1, 0x00, DIFF_P_INT_STATUS_1_P_TGT0_MASK},
                                                     __END_WRITE_DATA__};

/*! @brief Address of Status Register. */
const registerreadlist_t cDiffPStatus[] = {{.readFrom = DIFF_P_INT_STATUS_1, .numBytes = 1}, __END_READ_DATA__};

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
    uint16_t thresholdValue;
    uint8_t whoAmI, conversionFactor, dataReady;

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
    thresholdValue = (((int16_t)(P_TGT0_MSB) << 8) | P_TGT0_LSB) / conversionFactor;
    PRINTF("\r\n Initiliazed Sensor with WHO_AM_I = [0x%X] Threshold = [%dPa].\r\n", whoAmI, thresholdValue);

    /*!  Set the task to be executed while waiting for I2C transactions to complete. */
    DIFF_P_I2C_SetIdleTask(&diff_pDriver, (registeridlefunction_t)SMC_SetPowerModeVlpr, SMC);

    /*! Configure the DIFF_P sensor. */
    status = DIFF_P_I2C_Configure(&diff_pDriver, cDiffPConfigNormal);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\nDIFF_P now active and entering data read loop...\r\n");
        return -1;
    }
    PRINTF("\r\n Successfully Applied Sensor Interrupt Configuration.\r\n");

    for (;;) /* Forever loop */
    {
        if (false == diff_pInterrupt)
        { /* Loop, if new sample is not available. */
            SMC_SetPowerModeWait(SMC);
            continue;
        }
        else
        { /*! Clear the data ready flag, it will be set again by the ISR. */
            diff_pInterrupt = false;
            gpioDriver->toggle_pin(&GREEN_LED);
        }

        /* Read INT_STATUS register */
        status = DIFF_P_I2C_ReadData(&diff_pDriver, cDiffPStatus, &dataReady);
        if (ARM_DRIVER_OK != status)
        {
            PRINTF("\r\n Read Failed. \r\n");
            return -1;
        }

        /*! Check for event ready bits from the DIFF_P. */
        if (DIFF_P_INT_STATUS_1_P_TGT0_REACHED != (dataReady & DIFF_P_INT_STATUS_1_P_TGT0_MASK))
        { /* Loop, if new samples are not available. */
            continue;
        }

        /*! Explicitly clear the Status Bits. */
        status = Sensor_I2C_Write(diff_pDriver.pCommDrv, &diff_pDriver.deviceInfo, diff_pDriver.slaveAddress,
                                  cDiffPClearStatusBits);
        if (ARM_DRIVER_OK != status)
        {
            PRINTF("\r\n Write Failed. \r\n");
            return -1;
        }

        PRINTF("\r\n Threshold Event... \r\n", conversionFactor);
    }
}
