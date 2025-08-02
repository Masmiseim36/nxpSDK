/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "bootloader_common.h"
#include "fsl_iomuxc.h"
#include "fsl_lpspi.h"
#include "peripherals_pinmux.h"
#include "spi_nor_eeprom_memory.h"

#if FSL_FEATURE_SOC_GPIO_COUNT
#include "fsl_gpio.h"
#elif FSL_FEATURE_SOC_IGPIO_COUNT
#include "fsl_gpio.h"
#elif FSL_FEATURE_SOC_RGPIO_COUNT
#include "fsl_rgpio.h"
#endif
#if FSL_FEATURE_SOC_PORT_COUNT
#include "fsl_port.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RECOVERY_SPI_CLOCK_DEFAULT_FREQUENCY (10 * 1000)
#define RECOVERY_SPI_CLOCK_SOURCE (kCLOCK_SysPll2Out)
#define RECOVERY_SPI_CLOCK_SOURCE_MUX (3)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void spi_pcs_pin_control(uint32_t instance, uint8_t pcsx, bool isSelected);
void spi_clock_config(uint32_t instance, spi_module_clock_freq_t freq);
void spi_iomux_config(spi_nor_eeprom_peripheral_config_t *config);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
//!@brief Control pin for LPSPI Peripheral
void spi_pcs_pin_control(uint32_t instance, uint8_t pcsx, bool isSelected)
{
    uint32_t pcsPinNumber;
    GPIO_Type *gpioBase = NULL;
    switch (instance)
    {
        case 1:
            pcsPinNumber = RECOVERY_SPI1_PCS0_GPIO_PIN_NUM;
            gpioBase = RECOVERY_SPI1_PCS0_GPIO_BASE;
            break;
        case 2:
            pcsPinNumber = RECOVERY_SPI2_PCS0_GPIO_PIN_NUM;
            gpioBase = RECOVERY_SPI2_PCS0_GPIO_BASE;
            break;
        case 3:
            /* Not supported. */
            break;
        case 4:
            pcsPinNumber = RECOVERY_SPI4_PCS0_GPIO_PIN_NUM;
            gpioBase = RECOVERY_SPI4_PCS0_GPIO_BASE;
            break;
        case 5:
            pcsPinNumber = RECOVERY_SPI5_PCS0_GPIO_PIN_NUM;
            gpioBase = RECOVERY_SPI5_PCS0_GPIO_BASE;
            break;
        default:
            return;
    }

    GPIO_WritePinOutput(gpioBase, pcsPinNumber, !isSelected);
}

//!@brief Configure clock for LPSPI peripheral
void spi_clock_config(uint32_t instance, spi_module_clock_freq_t freq)
{
#ifndef BL_TARGET_FPGA
    clock_root_t clockRoot;
    clock_name_t clockSource = kCLOCK_SysPll2Out;
    switch (instance)
    {
        case 1:
        case 2:
            clockRoot = kCLOCK_Root_Lpspi0102;
            break;
        case 3:
        case 4:
            clockRoot = kCLOCK_Root_Lpspi0304;
            break;
        case 5:
        case 6:
            clockRoot = kCLOCK_Root_Lpspi0506;
            break;
        default:
            return;
    }

    uint32_t clockDiv = (CLOCK_GetFreq(RECOVERY_SPI_CLOCK_SOURCE) + freq - 1) / freq;
    CCM->CLOCK_ROOT[clockRoot].CONTROL =
        CCM_CLOCK_ROOT_CONTROL_DIV(clockDiv - 1) | CCM_CLOCK_ROOT_CONTROL_MUX(RECOVERY_SPI_CLOCK_SOURCE_MUX);
#endif
}

//!@brief Configure IOMUX for LPSPI Peripheral
void spi_iomux_config(spi_nor_eeprom_peripheral_config_t *config)
{
    switch (config->spiIndex)
    {
        case 1:
        {
            /* LPSPI1_SCK*/
            IOMUXC_SetPinMux(RECOVERY_SPI1_SCK_IOMUXC_MUX_FUNC, false);
            IOMUXC_SetPinConfig(RECOVERY_SPI1_SCK_IOMUXC_MUX_FUNC, RECOVERY_SPI_PAD_CTRL);
            /* LPSPI1_SDO*/
            IOMUXC_SetPinMux(RECOVERY_SPI1_SDO_IOMUXC_MUX_FUNC, false);
            IOMUXC_SetPinConfig(RECOVERY_SPI1_SDO_IOMUXC_MUX_FUNC, RECOVERY_SPI_PAD_CTRL);
            /* LPSPI1_SDI*/
            IOMUXC_SetPinMux(RECOVERY_SPI1_SDI_IOMUXC_MUX_FUNC, false);
            IOMUXC_SetPinConfig(RECOVERY_SPI1_SDI_IOMUXC_MUX_FUNC, RECOVERY_SPI_PAD_CTRL);
            /* LPSPI1_PCS */
            /* Note: So far we cannot do separated send and recive operation in one active pcs period in LPSPI driver
             *  To achieve this goal, we should config PCSx pin as GPIO and control it manually. */
            IOMUXC_SetPinMux(RECOVERY_SPI1_PCS0_IOMUXC_MUX_GPIO, false);
            IOMUXC_SetPinConfig(RECOVERY_SPI1_PCS0_IOMUXC_MUX_GPIO, RECOVERY_SPI_PCS_PAD_CTRL);
            gpio_pin_config_t sw_config = {
                kGPIO_DigitalOutput, /* Set PCS pin direction as general-purpose output */
                1                    /* Set PCS pin output as logic 1 */
            };
            GPIO_PinInit(RECOVERY_SPI1_PCS0_GPIO_BASE, RECOVERY_SPI1_PCS0_GPIO_PIN_NUM, &sw_config);
        }
        break;
        case 2:
        {
            /* LPSPI2_SCK*/
            IOMUXC_SetPinMux(RECOVERY_SPI2_SCK_IOMUXC_MUX_FUNC, false);
            IOMUXC_SetPinConfig(RECOVERY_SPI2_SCK_IOMUXC_MUX_FUNC, RECOVERY_SPI_PAD_CTRL);
            /* LPSPI2_SDO*/
            IOMUXC_SetPinMux(RECOVERY_SPI2_SDO_IOMUXC_MUX_FUNC, false);
            IOMUXC_SetPinConfig(RECOVERY_SPI2_SDO_IOMUXC_MUX_FUNC, RECOVERY_SPI_PAD_CTRL);
            /* LPSPI2_SDI*/
            IOMUXC_SetPinMux(RECOVERY_SPI2_SDI_IOMUXC_MUX_FUNC, false);
            IOMUXC_SetPinConfig(RECOVERY_SPI2_SDI_IOMUXC_MUX_FUNC, RECOVERY_SPI_PAD_CTRL);
            /* LPSPI2_PCS */
            /* Note: So far we cannot do separated send and recive operation in one active pcs period in LPSPI driver
             *  To achieve this goal, we should config PCSx pin as GPIO and control it manually. */
            IOMUXC_SetPinMux(RECOVERY_SPI2_PCS0_IOMUXC_MUX_GPIO, false);
            IOMUXC_SetPinConfig(RECOVERY_SPI2_PCS0_IOMUXC_MUX_GPIO, RECOVERY_SPI_PCS_PAD_CTRL);
            gpio_pin_config_t sw_config = {
                kGPIO_DigitalOutput, /* Set PCS pin direction as general-purpose output */
                1                    /* Set PCS pin output as logic 1 */
            };
            GPIO_PinInit(RECOVERY_SPI2_PCS0_GPIO_BASE, RECOVERY_SPI2_PCS0_GPIO_PIN_NUM, &sw_config);
        }
        break;
        case 3:
            /* Not supported. */
            break;
        case 4:
        {
            /* LPSPI4_SCK*/
            IOMUXC_SetPinMux(RECOVERY_SPI4_SCK_IOMUXC_MUX_FUNC, false);
            IOMUXC_SetPinConfig(RECOVERY_SPI4_SCK_IOMUXC_MUX_FUNC, RECOVERY_SPI_PAD_CTRL);
            /* LPSPI4_SDO*/
            IOMUXC_SetPinMux(RECOVERY_SPI4_SDO_IOMUXC_MUX_FUNC, false);
            IOMUXC_SetPinConfig(RECOVERY_SPI4_SDO_IOMUXC_MUX_FUNC, RECOVERY_SPI_PAD_CTRL);
            /* LPSPI4_SDI*/
            IOMUXC_SetPinMux(RECOVERY_SPI4_SDI_IOMUXC_MUX_FUNC, false);
            IOMUXC_SetPinConfig(RECOVERY_SPI4_SDI_IOMUXC_MUX_FUNC, RECOVERY_SPI_PAD_CTRL);
            /* LPSPI4_PCS */
            /* Note: So far we cannot do separated send and recive operation in one active pcs period in LPSPI driver
             *  To achieve this goal, we should config PCSx pin as GPIO and control it manually. */
            IOMUXC_SetPinMux(RECOVERY_SPI4_PCS0_IOMUXC_MUX_GPIO, false);
            IOMUXC_SetPinConfig(RECOVERY_SPI4_PCS0_IOMUXC_MUX_GPIO, RECOVERY_SPI_PCS_PAD_CTRL);
            gpio_pin_config_t sw_config = {
                kGPIO_DigitalOutput, /* Set PCS pin direction as general-purpose output */
                1                    /* Set PCS pin output as logic 1 */
            };
            GPIO_PinInit(RECOVERY_SPI4_PCS0_GPIO_BASE, RECOVERY_SPI4_PCS0_GPIO_PIN_NUM, &sw_config);
            break;
        }
        case 5:
        {
            /* LPSPI5_SCK*/
            IOMUXC_SetPinMux(RECOVERY_SPI5_SCK_IOMUXC_MUX_FUNC, false);
            IOMUXC_SetPinConfig(RECOVERY_SPI5_SCK_IOMUXC_MUX_FUNC, RECOVERY_SPI_PAD_CTRL);
            /* LPSPI5_SDO*/
            IOMUXC_SetPinMux(RECOVERY_SPI5_SDO_IOMUXC_MUX_FUNC, false);
            IOMUXC_SetPinConfig(RECOVERY_SPI5_SDO_IOMUXC_MUX_FUNC, RECOVERY_SPI_PAD_CTRL);
            /* LPSPI5_SDI*/
            IOMUXC_SetPinMux(RECOVERY_SPI5_SDI_IOMUXC_MUX_FUNC, false);
            IOMUXC_SetPinConfig(RECOVERY_SPI5_SDI_IOMUXC_MUX_FUNC, RECOVERY_SPI_PAD_CTRL);
            /* LPSPI5_PCS */
            /* Note: So far we cannot do separated send and recive operation in one active pcs period in LPSPI driver
             *  To achieve this goal, we should config PCSx pin as GPIO and control it manually. */
            IOMUXC_SetPinMux(RECOVERY_SPI5_PCS0_IOMUXC_MUX_GPIO, false);
            IOMUXC_SetPinConfig(RECOVERY_SPI5_PCS0_IOMUXC_MUX_GPIO, RECOVERY_SPI_PCS_PAD_CTRL);
            gpio_pin_config_t sw_config = {
                kGPIO_DigitalOutput, /* Set PCS pin direction as general-purpose output */
                1                    /* Set PCS pin output as logic 1 */
            };
            GPIO_PinInit(RECOVERY_SPI5_PCS0_GPIO_BASE, RECOVERY_SPI5_PCS0_GPIO_PIN_NUM, &sw_config);
            break;
        }
        default:
            break;
    }
}
