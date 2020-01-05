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

/*! @file  fxas21002_poll_spi.c
 *  @brief The fxas21002_poll_spi.c file implements the ISSDK FXAS21002 SPI sensor driver
 *         example demonstration for SPI Mode with polling.
 */

//-----------------------------------------------------------------------
// SDK Includes
//-----------------------------------------------------------------------
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

//-----------------------------------------------------------------------
// ISSDK Includes
//-----------------------------------------------------------------------
#include "issdk_hal.h"
#include "gpio_driver.h"
#include "fxas21002_drv.h"

//-----------------------------------------------------------------------
// CMSIS Includes
//-----------------------------------------------------------------------
#include "Driver_SPI.h"

//-----------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------
/*! Prepare the register write list to configure FXAS21002 in non-FIFO mode. */
const registerwritelist_t cFxas21002_Config_Normal[] = {
    /*! Configure CTRL_REG1 register to put FXAS21002 to 12.5Hz sampling rate. */
    {FXAS21002_CTRL_REG1, FXAS21002_CTRL_REG1_DR_12_5HZ, FXAS21002_CTRL_REG1_DR_MASK},
    __END_WRITE_DATA__};

/*! Prepare the register read list to read FXAS21002 DataReady status. */
const registerreadlist_t cFxas21002_DataReady[] = {{.readFrom = FXAS21002_STATUS, .numBytes = 1}, __END_READ_DATA__};

/*! Prepare the register read list to read the raw gyro data from the FXAS21002. */
const registerreadlist_t cFxas21002_Output_Values[] = {
    {.readFrom = FXAS21002_OUT_X_MSB, .numBytes = FXAS21002_GYRO_DATA_SIZE}, __END_READ_DATA__};

//-----------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------
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
    uint8_t gFxas21002DataReady;
    uint8_t data[FXAS21002_GYRO_DATA_SIZE];
    fxas21002_gyrodata_t rawData;

    ARM_DRIVER_SPI *pSPIdriver = &SPI_S_DRIVER;
    fxas21002_spi_sensorhandle_t fxas21002Driver;

    /*! Initialize the MCU hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\n ISSDK FXAS21002 sensor driver example demonstration for SPI with Poll Mode.\r\n");

    /*! Initialize the SPI driver. */
    status = pSPIdriver->Initialize(SPI_S_SIGNAL_EVENT);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n SPI Initialization Failed\r\n");
        return -1;
    }

    /*! Set the SPI Power mode. */
    status = pSPIdriver->PowerControl(ARM_POWER_FULL);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n SPI Power Mode setting Failed\r\n");
        return -1;
    }

    /*! Set the SPI Slave speed. */
    status = pSPIdriver->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA0, SPI_S_BAUDRATE);
    if (ARM_DRIVER_OK != status)
    {
        PRINTF("\r\n SPI Control Mode setting Failed\r\n");
        return -1;
    }

    /*! Initialize the FXAS21002 sensor driver. */
    status = FXAS21002_SPI_Initialize(&fxas21002Driver, &SPI_S_DRIVER, SPI_S_DEVICE_INDEX, &FXAS21002_CS,
                                      FXAS21002_WHO_AM_I_WHOAMI_PROD_VALUE);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n FXAS21002 Sensor Initialization Failed\r\n");
        return -1;
    }
    PRINTF("\r\n Successfully Initiliazed FXAS21002 Sensor\r\n");

    /*!  Set the task to be executed while waiting for SPI transactions to complete. */
    FXAS21002_SPI_SetIdleTask(&fxas21002Driver, (registeridlefunction_t)SMC_SetPowerModeWait, SMC);

    /*! Configure the FXAS21002 sensor driver. */
    status = FXAS21002_SPI_Configure(&fxas21002Driver, cFxas21002_Config_Normal);
    if (SENSOR_ERROR_NONE != status)
    {
        PRINTF("\r\n FXAS21002 Sensor Configuration Failed, Err = %d\r\n", status);
        return -1;
    }
    PRINTF("\r\n Successfully Applied FXAS21002 Sensor Configuration\r\n");

    for (;;) /* Forever loop */
    {
        /*! Wait for data ready from the FXAS21002. */
        status = FXAS21002_SPI_ReadData(&fxas21002Driver, cFxas21002_DataReady, &gFxas21002DataReady);
        if (0 == (gFxas21002DataReady & FXAS21002_DR_STATUS_ZYXDR_MASK))
        { /* Loop, if new sample is not available. */
            continue;
        }

        /*! Read the raw sensor data from the FXAS21002. */
        status = FXAS21002_SPI_ReadData(&fxas21002Driver, cFxas21002_Output_Values, data);
        if (ARM_DRIVER_OK != status)
        {
            PRINTF("\r\nRead Failed.\r\n");
            return -1;
        }

        /*! Convert the raw sensor data for display to the debug port. */
        rawData.gyro[0] = ((int16_t)data[0] << 8) | data[1];
        rawData.gyro[1] = ((int16_t)data[2] << 8) | data[3];
        rawData.gyro[2] = ((int16_t)data[4] << 8) | data[5];

        /* NOTE: PRINTF is relatively expensive in terms of CPU time, specially when used with-in execution loop. */
        PRINTF("\r\n Gyro X = %d  Y = %d  Z = %d \r\n", rawData.gyro[0], rawData.gyro[1], rawData.gyro[2]);
    }
}
