/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
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
    drvData->gpioConfig.ackPin.gpioInstance = instanceC;
    drvData->gpioConfig.ackPin.gpioPinNumber = 4U;

    /* CS pin. */
    drvData->gpioConfig.csPin.gpioInstance = instanceD;
    drvData->gpioConfig.csPin.gpioPinNumber = 0U;

    SF_SetupGPIOs(&(drvData->gpioConfig));

    /* SPI initialization. */
    drvData->spiConfig.baudRate = 125000U;
    drvData->spiConfig.sourceClkHz = GET_SPI_MODULE_CLK();
    drvData->spiConfig.spiInstance = 0U;

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
