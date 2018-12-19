/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*!
 * @file sf_setup.c
 *
 * This module contains configuration of peripherals used by the OL2361/OL2385.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
/* User includes. */
#include "sf_setup.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

void SF_SetupSPI(sf_spi_config_t *spiConfig,
        const spi_sdk_master_config_t *userSpiSdkConfig)
{
    /* Needed when userSpiSdkConfig argument is NULL. */
    spi_sdk_master_config_t spiSdkConfig;
    spi_aml_master_config_t spiAmlConfig;

    if (userSpiSdkConfig == NULL)
    {
        spiAmlConfig.bitCount = 8U;
        spiAmlConfig.baudRateHz = spiConfig->baudRate;
        spiAmlConfig.clkPhase = spiClockPhaseSecondEdge;
        spiAmlConfig.clkPolarity = spiClockPolarityActiveHigh;
        spiAmlConfig.lsbFirst = false;
        spiAmlConfig.pcsPolarity = spiPcsActiveLow;
        spiAmlConfig.sourceClockHz = spiConfig->sourceClkHz;

        SPI_AML_MasterFillSdkConfig(&spiAmlConfig, &spiSdkConfig);
        SPI_AML_MasterInit(spiConfig->spiInstance, &spiSdkConfig,
                spiConfig->sourceClkHz);
    }
    else
    {   /* Use provided userSpiSdkConfig configuration. */
        SPI_AML_MasterInit(spiConfig->spiInstance, userSpiSdkConfig,
                spiConfig->sourceClkHz);
    }
}

void SF_SetupGPIOs(sf_gpio_config_t *gpioConfig)
{
    assert(gpioConfig);

#if (SDK_VERSION == SDK_KSDK)

    /* Set ACK pin as input. */
    GPIO_AML_SetDirection(gpioConfig->ackPin.gpioInstance, \
        gpioConfig->ackPin.gpioPinNumber, gpioDirDigitalInput);

    /* Set CSB pin as output with default value high (active low). */
    GPIO_AML_SetDirection(gpioConfig->csPin.gpioInstance, \
            gpioConfig->csPin.gpioPinNumber, gpioDirDigitalOutput);
    GPIO_AML_SetOutput(gpioConfig->csPin.gpioInstance, \
            gpioConfig->csPin.gpioPinNumber);

#elif (SDK_VERSION == SDK_S32)
    /*
     * GPIOs are initialized using pin_mux component.
     *
     * Following function call must be used in user code in order to initialize
     * pins properly.
     *
     * Pins_DRV_Init(NUM_OF_CONFIGURED_PINS, pin_settings_config_t *config);
     */
#endif /* END of SDK check. */
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
