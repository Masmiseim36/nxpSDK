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
 * @file sf_setup.h
 *
 * This module contains configuration of MCU peripherals used by the OL2361/OL2385.
 *
 * User can either use prepared functions to setup MCU peripherals for the needs
 * of this driver or take responsibility for this setup by doing it himself.
 */

#ifndef SOURCE_SF_SETUP_H_
#define SOURCE_SF_SETUP_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "common_aml.h"
#include "gpio_aml.h"
#include "spi_aml/spi_aml.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*!
 * @addtogroup sf_conf_types_group
 * @{
 */
/*! @brief Structure for SPI configuration. */
typedef struct 
{
    aml_instance_t spiInstance; /*!< Used SPI instance. */
    uint32_t sourceClkHz;       /*!< Clock for SPI peripheral. */
    uint32_t baudRate;          /*!< Baudrate in Hz. */
} sf_spi_config_t;

/*! @brief Structure for GPIO selection. */
typedef struct 
{
    gpio_aml_instance_t gpioInstance; /*!< Used GPIO instance (port). */
    uint32_t gpioPinNumber;           /*!< Used GPIO pin number. */
} sf_gpio_sel_t;

/*! @brief Structure for GPIO configuration. */
typedef struct 
{
    sf_gpio_sel_t ackPin; /*!< GPIO selection for ACK. */
    sf_gpio_sel_t csPin;  /*!< GPIO selection for SW chip select. */
} sf_gpio_config_t;
/*! @} */

/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @addtogroup sf_conf_func_group
 * @{
 */
/*!
 * @brief Configures SPI module used by the SIGFOX device.
 *
 * User can either provide required inputs or pass through SDK configuration
 * structure. In former case SDK configuration structure is created automatically,
 * filled with provided inputs and the rest of configuration items uses default
 * values. In the latter case it is passed without any changes directly to the
 * initialization function.
 * 
 * Example of automatic creation of the SPI SDK configuration structure with  
 * use of spiConfig parameter:
 * @code
 *      // Data used by SIGFOX driver. You should use this structure in all
 *      // driver's functions.
 *      sf_drv_data_t drvData;
 *
 *      drvData.spiConfig.spiInstance = 1U;     // SPI 1 peripheral.
 *      drvData.spiConfig.baudRate = 125000U;
 *      drvData.spiConfig.sourceClkHz = CLOCK_GetFreq(kCLOCK_BusClk);
 *
 *      SF_SetupSPI(&(drvData.spiConfig), NULL);
 * @endcode
 *
 * Example of SPI configuration in SDK 2.3, where user must provide master
 * configuration structure (spiSdkConfig parameter):
 * @code
 *      // SDK 2.3 SPI master configuration used by SPI driver.
 *      spi_master_config_t masterConfig = {0};
 *      // Data used by SIGFOX driver. You should use this structure in all
 *      // driver's functions.
 *      sf_drv_data_t drvData;
 *
 *      drvData.spiConfig.spiInstance = 1U;     // SPI 1 peripheral.
 *      drvData.spiConfig.baudRate = 125000U;
 *      drvData.spiConfig.sourceClkHz = CLOCK_GetFreq(kCLOCK_BusClk);
 *
 *      // Use default configuration and update baudrate and chip select mode 
 *      // (CS is handled by GPIO).
 *      SPI_MasterGetDefaultConfig(&masterConfig);
 *      masterConfig.baudRate_Bps = 125000U;
 *      masterConfig.outputMode = kSPI_SlaveSelectAsGpio;
 *
 *      SF_SetupSPI(&(drvData.spiConfig), &masterConfig);
 * @endcode
 *
 * @param spiConfig Structure for SPI configuration.
 * @param userSpiSdkConfig SDK configuration structure.
 *
 * @return None
 */
void SF_SetupSPI(sf_spi_config_t *spiConfig, const spi_sdk_master_config_t *userSpiSdkConfig);

/*!
 * @brief Configuration of GPIOs used for the SIGFOX device.
 *        Note that this function should not be used in SDK S32 when using
 *        pin_mux component.
 *
 * Example:
 * @code
 *      // Data used by SIGFOX driver. You should use this structure in all
 *      // driver's functions.
 *      sf_drv_data_t drvData;
 *
 *      // ACK pin.
 *      drvData.gpioConfig.ackPin.gpioInstance = instanceD;
 *      drvData.gpioConfig.ackPin.gpioPinNumber = 2U;
 *      // SPI chip select pin.
 *      drvData.gpioConfig.csPin.gpioInstance = instanceD;
 *      drvData.gpioConfig.csPin.gpioPinNumber = 4U;
 *
 *      SF_SetupGPIOs(&(drvData.gpioConfig));
 * @endcode
 *
 * @param gpioConfig Structure for GPIO configuration.
 *
 * @return None
 */
void SF_SetupGPIOs(sf_gpio_config_t *gpioConfig);
/*! @} */

#endif /* SOURCE_SF_SETUP_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
