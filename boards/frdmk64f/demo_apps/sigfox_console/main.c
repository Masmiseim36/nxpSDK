/*
 * Copyright (c) 2016 - 2018, NXP Semiconductors, Inc.
 * All rights reserved.
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
 
/*!
 * @file main.c
 * @attention Tested with the following OL2385 firmware version: 1.12.0.2.8.20.17.
 *
 * Console Control for OL2385 based on Sigfox SDK SW driver.
 *
 * This example enables user to control OL2385 using console interface.
 * Sigfox SDK SW driver is used to access/communicate with OL2385 device.
 */
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

/* Sigfox */
#include "sf.h"
#include "sf_cmd.h"

/*!
 * @brief Gets source clock for SPI peripheral.
 */
#define GET_SPI_MODULE_CLK() \
    (CLOCK_GetFreq(DSPI0_CLK_SRC))

/*!
 * @brief Initializes SIGFOX device and SW driver.
 *
 * @param drvData Driver run-time data.
 *
 * @return Status result of the function.
 */
static status_t SetupSigfoxDriver(sf_drv_data_t *drvData)
{
    sf_user_config_t userConfig;

    SF_GetDefaultConfig(&userConfig);

    /* GPIOs initialization.
     * Note: GPIO settings are place in pin_mux.h file. */
    /* ACK pin. */
    drvData->gpioConfig.ackPin.gpioInstance = SF_ACK_INST;
    drvData->gpioConfig.ackPin.gpioPinNumber = SF_ACK_PIN;

    /* CS pin. */
    drvData->gpioConfig.csPin.gpioInstance = SF_CS_INST;
    drvData->gpioConfig.csPin.gpioPinNumber = SF_CS_PIN;

    SF_SetupGPIOs(&(drvData->gpioConfig));

    /* SPI initialization. */
    drvData->spiConfig.baudRate = 125000U;
    drvData->spiConfig.sourceClkHz = GET_SPI_MODULE_CLK();
    drvData->spiConfig.spiInstance = SF_SPI_INST;

    SF_SetupSPI(&(drvData->spiConfig), NULL);

    /* Sigfox driver initialization.
     * Note: drvData->gpioConfig and drvData->spiConfig structures are used
     * by SF_SetupGPIOs, SF_SetupSPI and SF_Init. */
    return SF_Init(drvData, &userConfig);
}

/*
 * @brief   Application entry point.
 */
int main(void) {
    status_t status = kStatus_Success;
    sf_drv_data_t sfDrvData;        /* Sigfox driver data needed by the
                                       driver's functions. */
    uint8_t cmd;                    /* Command for OL2385. */
    bool sfError = false;           /* If an error is caused by Sigfox. */
    /* Current networking standard. */
    sf_net_standard_t sfNetStandard = sfNetStandardETSI;

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();

    /* Initialize SPI, GPIOs and Sigfox driver, */
    status = SetupSigfoxDriver(&sfDrvData);
    if (status != kStatus_Success)
    {
        PRINTF("An error occurred in SetupSigfoxDriver (%d)\r\n", status);
    }
    else
    {
        PRINTF("\r\n====================================================\r\n");
        PRINTF("SIGFOX FIRMWARE VERSION ");
        /* No version displayed when an error occurred. */
        (void)PrintFirmwareVersion(&sfDrvData);
        PRINTF("\r\n");
        PRINTF("====================================================\r\n");

        PrintCommands();

        while (1)
        {
            cmd = GetUserCommand();

            status = ProcessCommand(&sfDrvData, (sf_spi_cmd_t)cmd, &sfNetStandard, &sfError);

            PrintStatus(&sfDrvData, status, sfError);
            PRINTF("\r\n");
        }
    }

    for(;;)  /* Infinite loop to avoid leaving the main function */
    {
        __asm("NOP");
    }
}
