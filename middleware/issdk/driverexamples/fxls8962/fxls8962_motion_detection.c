/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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
 * @file fxls8962_motion_detection.c
 * @brief The fxls8962_motion_detection.c file implements the ISSDK FXLS8962 sensor
 *        driver example demonstration for Motion Detection.
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

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "issdk_hal.h"
#include "gpio_driver.h"
#include "fxls8962_drv.h"

//-----------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------
#define MD_SDCD_WT_DBCNT 0x00 /* Debounce count value. */
#define SDCD_LTHS_LSB 0xC0    /* Lower Threshold LSB value. */
#define SDCD_LTHS_MSB 0x0F    /* Lower Threshold MSB value. */
#define SDCD_UTHS_LSB 0x40    /* Upper Threshold LSB value. */
#define SDCD_UTHS_MSB 0x00    /* Upper Threshold MSB value. */

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------
/*! @brief Register Start Motion Detect Mode Register Write List. */
const registerwritelist_t cFxls8962ConfigMotionDetect[] = {
    {FXLS8962_SENS_CONFIG3, FXLS8962_SENS_CONFIG3_WAKE_ODR_0_781HZ, FXLS8962_SENS_CONFIG3_WAKE_ODR_MASK},
    {FXLS8962_SENS_CONFIG4, FXLS8962_SENS_CONFIG4_DRDY_PUL_DIS, FXLS8962_SENS_CONFIG4_DRDY_PUL_MASK},
    {FXLS8962_INT_EN, FXLS8962_INT_EN_SDCD_OT_EN_EN, FXLS8962_INT_EN_SDCD_OT_EN_MASK},
    {FXLS8962_ORIENT_CONFIG, FXLS8962_ORIENT_CONFIG_ORIENT_ENABLE_DIS, FXLS8962_ORIENT_CONFIG_ORIENT_ENABLE_MASK},
    {FXLS8962_SDCD_CONFIG1,
     FXLS8962_SDCD_CONFIG1_X_OT_EN_EN | FXLS8962_SDCD_CONFIG1_Y_OT_EN_EN | FXLS8962_SDCD_CONFIG1_Z_OT_EN_EN,
     FXLS8962_SDCD_CONFIG1_X_OT_EN_MASK | FXLS8962_SDCD_CONFIG1_Y_OT_EN_MASK | FXLS8962_SDCD_CONFIG1_Z_OT_EN_MASK},
    {FXLS8962_SDCD_CONFIG2, FXLS8962_SDCD_CONFIG2_SDCD_EN_EN | FXLS8962_SDCD_CONFIG2_REF_UPDM_SDCD_REF |
                                FXLS8962_SDCD_CONFIG2_OT_DBCTM_CLEARED | FXLS8962_SDCD_CONFIG2_WT_DBCTM_CLEARED,
     FXLS8962_SDCD_CONFIG2_SDCD_EN_MASK | FXLS8962_SDCD_CONFIG2_REF_UPDM_MASK | FXLS8962_SDCD_CONFIG2_OT_DBCTM_MASK |
         FXLS8962_SDCD_CONFIG2_WT_DBCTM_MASK},
    {FXLS8962_SDCD_WT_DBCNT, MD_SDCD_WT_DBCNT, 0},
    {FXLS8962_SDCD_LTHS_LSB, SDCD_LTHS_LSB, 0},
    {FXLS8962_SDCD_LTHS_MSB, SDCD_LTHS_MSB, 0},
    {FXLS8962_SDCD_UTHS_LSB, SDCD_UTHS_LSB, 0},
    {FXLS8962_SDCD_UTHS_MSB, SDCD_UTHS_MSB, 0},
    __END_WRITE_DATA__};

//-----------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------
volatile bool gFxls8962EventReady = false;

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
void fxls8962_int_callback(void *pUserData)
{ /*! @brief Set flag to indicate Sensor has signalled data ready. */
    gFxls8962EventReady = true;
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

    ARM_DRIVER_I2C *I2Cdrv = &I2C_S_DRIVER; // Now using the shield.h value!!!
    fxls8962_i2c_sensorhandle_t fxls8962Driver;
    GENERIC_DRIVER_GPIO *pGpioDriver = &Driver_GPIO_KSDK;

    /*! Initialize the MCU hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\n ISSDK FXLS8962 sensor driver example for Orientation and Motion Detection.\r\n");

    /*! Initialize FXLS8962 pin used by FRDM board */
    pGpioDriver->pin_init(&FXLS8962_INT1, GPIO_DIRECTION_IN, NULL, &fxls8962_int_callback, NULL);

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

    /*! Initialize FXLS8962 sensor driver. */
    status = FXLS8962_I2C_Initialize(&fxls8962Driver, &I2C_S_DRIVER, I2C_S_DEVICE_INDEX, FXLS8962_I2C_ADDR,
                                     FXLS8962_WHOAMI_VALUE);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n Sensor Initialization Failed\r\n");
        return -1;
    }
    PRINTF("\r\n Successfully Initiliazed Sensor\r\n");

    /*!  Set the task to be executed while waiting for I2C transactions to complete. */
    FXLS8962_I2C_SetIdleTask(&fxls8962Driver, (registeridlefunction_t)SMC_SetPowerModeVlpr, SMC);

    /* Apply FXLS8962 Configuration for Motion Detection. */
    status = FXLS8962_I2C_Configure(&fxls8962Driver, cFxls8962ConfigMotionDetect);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n Write FXLS8962 Motion Configuration Failed!\r\n");
        return -1;
    }
    PRINTF("\r\n Successfully Applied FXLS8962 Sensor Configuration\r\n");

    for (;;)
    {
        PRINTF("\r\n");
        PRINTF("\r\n Waiting for Motion | MCU going to Deep Sleep Mode ...\r\n");
        while (false == gFxls8962EventReady)
        { /* Sleep, until Motion detection Interrupt is received. */
            SMC_SetPowerModeWait(SMC);
        }

        gFxls8962EventReady = false;
        PRINTF("\r\n Motion detected | MCU Wake-up ...\r\n");
    }
}
