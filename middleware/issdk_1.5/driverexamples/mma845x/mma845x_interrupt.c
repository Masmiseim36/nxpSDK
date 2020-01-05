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
 * @file mma845x_interrupt.c
*   @brief The mma845x_interrupt.c file implements the ISSDK MMA845x sensor driver
*        example demonstration with Interrupt mode.
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
#include "Driver_GPIO.h"
#include "fsl_i2c_cmsis.h"

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "issdk_hal.h"
#include "mma845x_drv.h"
#include "systick_utils.h"
#include "gpio_driver.h"

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------
/*! Prepare the register write list to configure MMA845x in non-FIFO and ISR mode. */
const registerwritelist_t mma845x_Config_Isr[] = {
    /*! Configure the MMA845x CTRL_REG1 to set mode to STANDBY and odr to 12.5Hz. */
    {MMA845x_CTRL_REG1, MMA845x_CTRL_REG1_MODE_STANDBY | MMA845x_CTRL_REG1_DR_12DOT5HZ, 0},
    /*! Configure the MMA845x to disable FIFO */
    {MMA845x_F_SETUP, MMA845x_F_SETUP_F_MODE_FIFODISABLED, 0},
    /*! Configure the MMA845x CTRL_REG2 to set the Oversampling mode to High Resolution. */
    {MMA845x_CTRL_REG2, MMA845x_CTRL_REG2_SMODS_HIGHRES, 0},
    /*! Configure the MMA845x CTRL_REG3 to set the Interrupt polarity to ACTIVE high. */
    {MMA845x_CTRL_REG3, MMA845x_CTRL_REG3_IPOL_HIGH, 0},
    /*! Configure the MMA845x CTRL_REG4 to enable the data ready interrupt. */
    {MMA845x_CTRL_REG4, MMA845x_CTRL_REG4_INT_EN_DRDY_ENABLED, 0},
    /*! Configure the MMA845x CTRL_REG5 to route Interrupt to INT1 pin. */
    {MMA845x_CTRL_REG5, MMA845x_CTRL_REG5_INT_CFG_DRDY_INT1, 0},
    __END_WRITE_DATA__};

/*! Prepare the register read list to read the Data Ready Status from MMA845x. */
const registerreadlist_t MMA845x_Data_Ready[] = {{.readFrom = MMA845x_STATUS, .numBytes = 1}, __END_READ_DATA__};

/*! Prepare the register read list to read the raw Accel data from MMA845x. */
const registerreadlist_t mma845x_Output_Values[] = {
    {.readFrom = MMA845x_OUT_X_MSB, .numBytes = MMA845x_ACCEL_DATA_SIZE}, __END_READ_DATA__};

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
volatile bool mma845xInterrupt = false;

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
void mma845x_isr(void *pUserData)
{ /*! @brief Set flag to indicate Sensor has signalled data ready. */
    mma845xInterrupt = true;
}

/*! -----------------------------------------------------------------------
 *  @brief       This is the The main function implementation.
 *  @details     This function invokes board initializes routines, then then brings up the sensor and
 *               finally enters an endless loop to continuously read available samples.
 *  @param[in]   void This is no input parameter.
 *  @return      void  There is no return value.
 *  @constraints None
 *  @reeentrant  No
 *  -----------------------------------------------------------------------*/
int main(void)
{
    int32_t status;
    uint8_t data[MMA845x_ACCEL_DATA_SIZE];
    mma845x_acceldata_t rawData;

    ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER; // Now using the shield.h value!!!
    mma845x_i2c_sensorhandle_t MMA845xdrv;
    GENERIC_DRIVER_GPIO *gpioDriver = &Driver_GPIO_KSDK;

    /*! Initialize the MCU hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_SystickEnable();
    /*! Initialize the debug console. */
    BOARD_InitDebugConsole();

    PRINTF("\r\n ISSDK MMA845x sensor driver example for interrupt mode.\r\n");

    /*! Initialize INT1 pin used by FRDM board */
    gpioDriver->pin_init(&MMA845x_INT1, GPIO_DIRECTION_IN, NULL, &mma845x_isr, NULL);
    /*! Initialize RGB LED pin used by FRDM board */
    gpioDriver->pin_init(&BLUE_LED, GPIO_DIRECTION_OUT, NULL, NULL, NULL);

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
    MMA845x_I2C_SetIdleTask(&MMA845xdrv, (registeridlefunction_t)SMC_SetPowerModeWait, SMC);

    /*! Configure the MMA845x sensor driver with No FIFO mode. */
    status = MMA845x_I2C_Configure(&MMA845xdrv, mma845x_Config_Isr);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n MMA845x Sensor Configuration Failed, Err = %d\r\n", status);
        return -1;
    }
    PRINTF("\r\n Successfully Applied MMA845x Sensor Configuration\r\n");

    for (;;) /* Forever loop */
    {        /* In ISR Mode we do not need to check Data Ready Register.
              * The receipt of interrupt will indicate data is ready. */
        if (false == mma845xInterrupt)
        { /* Loop, if new sample is not available. */
            continue;
        }
        else
        { /*! Clear the data ready flag, it will be set again by the ISR. */
            mma845xInterrupt = false;
            gpioDriver->toggle_pin(&BLUE_LED);
        }

        /*! Read the raw sensor data from the MMA845x. */
        status = MMA845x_I2C_ReadData(&MMA845xdrv, mma845x_Output_Values, data);
        if (ARM_DRIVER_OK != status)
        {
            PRINTF("\r\n Read Failed. \r\n");
            return -1;
        }

        /*! Convert the raw sensor data to signed 16-bit container for display to the debug port. */
        rawData.accel[0] = ((int16_t)data[0] << 8) | data[1];
        rawData.accel[0] /= 4;
        rawData.accel[1] = ((int16_t)data[2] << 8) | data[3];
        rawData.accel[1] /= 4;
        rawData.accel[2] = ((int16_t)data[4] << 8) | data[5];
        rawData.accel[2] /= 4;

        /* NOTE: PRINTF is relatively expensive in terms of CPU time, specially when used with-in execution loop. */
        PRINTF("\r\n Accel X = %d  Y = %d  Z = %d\r\n", rawData.accel[0], rawData.accel[1], rawData.accel[2]);
        ASK_USER_TO_RESUME(100); /* Ask for user input after processing 100 samples. */
    }
}
