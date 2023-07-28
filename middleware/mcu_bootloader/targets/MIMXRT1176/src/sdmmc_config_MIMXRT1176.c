/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "fsl_assert.h"
#include "fsl_device_registers.h"
#include "fsl_mmc.h"
#include "fsl_sd.h"
#include "fusemap.h"
#include "peripherals_pinmux.h"
#include "sdmmc_init.h"
/*******************************************************************************
 * Definitons
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variable
 ******************************************************************************/
const uint32_t usdhc1_reset_pin_settings = IOMUXC_SW_PAD_CTL_PAD_SRE(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(0) |
                                           IOMUXC_SW_PAD_CTL_PAD_PUE(0) | IOMUXC_SW_PAD_CTL_PAD_PUS(0) |
                                           IOMUXC_SW_PAD_CTL_PAD_ODE(0) | IOMUXC_SW_PAD_CTL_PAD_APC(0);
const uint32_t usdhc2_reset_pin_settings = IOMUXC_SW_PAD_CTL_PAD_PDRV(0) | IOMUXC_SW_PAD_CTL_PAD_PULL(3) |
                                           IOMUXC_SW_PAD_CTL_PAD_ODE(0) | IOMUXC_SW_PAD_CTL_PAD_APC(0);
const uint32_t usdhc_vselect_pin_settings = IOMUXC_SW_PAD_CTL_PAD_SRE(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(0) |
                                            IOMUXC_SW_PAD_CTL_PAD_PUE(1) | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |
                                            IOMUXC_SW_PAD_CTL_PAD_ODE(0) | IOMUXC_SW_PAD_CTL_PAD_APC(0);
const uint32_t usdhc_cd_pin_settings = IOMUXC_SW_PAD_CTL_PAD_SRE(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(0) |
                                       IOMUXC_SW_PAD_CTL_PAD_PUE(1) | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |
                                       IOMUXC_SW_PAD_CTL_PAD_ODE(0) | IOMUXC_SW_PAD_CTL_PAD_APC(0);
/*
 * Config DATA3 pin as the CD pin. DATA3 pin must be pulled down to act as a card detection pin.
 */
const uint32_t usdhc_cd_dat3_settings = IOMUXC_SW_PAD_CTL_PAD_SRE(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(0) |
                                        IOMUXC_SW_PAD_CTL_PAD_PUE(1) | IOMUXC_SW_PAD_CTL_PAD_PUS(0) |
                                        IOMUXC_SW_PAD_CTL_PAD_ODE(0) | IOMUXC_SW_PAD_CTL_PAD_APC(0);
const uint32_t usdhc_cmd_pin_settings = IOMUXC_SW_PAD_CTL_PAD_PDRV(0) | IOMUXC_SW_PAD_CTL_PAD_PULL(1) |
                                        IOMUXC_SW_PAD_CTL_PAD_ODE(0) | IOMUXC_SW_PAD_CTL_PAD_APC(0);
const uint32_t usdhc_clk_pin_settings = IOMUXC_SW_PAD_CTL_PAD_PDRV(0) | IOMUXC_SW_PAD_CTL_PAD_PULL(1) |
                                        IOMUXC_SW_PAD_CTL_PAD_ODE(0) | IOMUXC_SW_PAD_CTL_PAD_APC(0);
const uint32_t usdhc_data_pin_settings = IOMUXC_SW_PAD_CTL_PAD_PDRV(0) | IOMUXC_SW_PAD_CTL_PAD_PULL(1) |
                                         IOMUXC_SW_PAD_CTL_PAD_ODE(0) | IOMUXC_SW_PAD_CTL_PAD_APC(0);
const uint32_t usdhc_data_pin_nopull = IOMUXC_SW_PAD_CTL_PAD_PDRV(0) | IOMUXC_SW_PAD_CTL_PAD_PULL(3) |
                                       IOMUXC_SW_PAD_CTL_PAD_ODE(0) | IOMUXC_SW_PAD_CTL_PAD_APC(0);
/*******************************************************************************
 * Code
 ******************************************************************************/
void usdhc_power_control_init(USDHC_Type *base)
{
    if (base == BOARD_USDHC1_BASEADDR)
    {
        IOMUXC_SetPinMux(BOARD_USDHC1_RESET_B_GPIO_IOMUXC, false);
        IOMUXC_SetPinConfig(BOARD_USDHC1_RESET_B_GPIO_IOMUXC, usdhc1_reset_pin_settings);
        gpio_pin_config_t sw_config = {
            kGPIO_DigitalOutput,
            !FUSE_SD1_RST_ACTIVE_POLARITY_VALUE,
            kGPIO_NoIntmode,
        };
        GPIO_PinInit(BOARD_USDHC1_RESET_B_GPIO_BASE, BOARD_USDHC1_RESET_B_GPIO_PIN, &sw_config);
    }
    else if (base == BOARD_USDHC2_BASEADDR)
    {
        IOMUXC_SetPinMux(BOARD_USDHC2_RESET_B_GPIO_IOMUXC, false);
        IOMUXC_SetPinConfig(BOARD_USDHC2_RESET_B_GPIO_IOMUXC, usdhc2_reset_pin_settings);
        gpio_pin_config_t sw_config = {
            kGPIO_DigitalOutput,
            !FUSE_SD2_RST_ACTIVE_POLARITY_VALUE,
            kGPIO_NoIntmode,
        };
        GPIO_PinInit(BOARD_USDHC2_RESET_B_GPIO_BASE, BOARD_USDHC2_RESET_B_GPIO_PIN, &sw_config);
    }
}

void usdhc_power_control(USDHC_Type *base, bool state)
{
    if (base == BOARD_USDHC1_BASEADDR)
    {
        GPIO_WritePinOutput(BOARD_USDHC1_RESET_B_GPIO_BASE, BOARD_USDHC1_RESET_B_GPIO_PIN, state);
    }
    else if (base == BOARD_USDHC2_BASEADDR)
    {
        GPIO_WritePinOutput(BOARD_USDHC2_RESET_B_GPIO_BASE, BOARD_USDHC2_RESET_B_GPIO_PIN, state);
    }
}

void usdhc_vselect_init(USDHC_Type *base)
{
    if (base == BOARD_USDHC1_BASEADDR)
    {
        IOMUXC_SetPinMux(BOARD_USDHC1_VSELECT_IOMUXC, false);
        IOMUXC_SetPinConfig(BOARD_USDHC1_VSELECT_IOMUXC, usdhc_vselect_pin_settings);
    }
    else if (base == BOARD_USDHC2_BASEADDR)
    {
        IOMUXC_SetPinMux(BOARD_USDHC2_VSELECT_IOMUXC, false);
        IOMUXC_SetPinConfig(BOARD_USDHC2_VSELECT_IOMUXC, usdhc_vselect_pin_settings);
    }
}

void usdhc_cd_init(USDHC_Type *base, const sdmmchost_detect_card_type_t cdType)
{
    if (cdType == kSDMMCHOST_DetectCardByGpioCD)
    {
        // Config CD pin as a GPIO
        if (base == BOARD_USDHC1_BASEADDR)
        {
            IOMUXC_SetPinMux(BOARD_USDHC1_CD_GPIO_IOMUXC, false);
            IOMUXC_SetPinConfig(BOARD_USDHC1_CD_GPIO_IOMUXC, usdhc_cd_pin_settings);
            gpio_pin_config_t sw_config = {
                kGPIO_DigitalInput,
                0,
                kGPIO_NoIntmode,
            };
            GPIO_PinInit(BOARD_USDHC1_CD_GPIO_BASE, BOARD_USDHC1_CD_GPIO_PIN, &sw_config);
        }
        else if (base == BOARD_USDHC2_BASEADDR)
        {
            IOMUXC_SetPinMux(BOARD_USDHC2_CD_GPIO_IOMUXC, false);
            IOMUXC_SetPinConfig(BOARD_USDHC2_CD_GPIO_IOMUXC, usdhc_cd_pin_settings);
            gpio_pin_config_t sw_config = {
                kGPIO_DigitalInput,
                0,
                kGPIO_NoIntmode,
            };
            GPIO_PinInit(BOARD_USDHC2_CD_GPIO_BASE, BOARD_USDHC2_CD_GPIO_PIN, &sw_config);
        }
    }
    else if (cdType == kSDMMCHOST_DetectCardByHostCD)
    {
        // Config CD pin as the uSDHC CD pin
        if (base == BOARD_USDHC1_BASEADDR)
        {
            IOMUXC_SetPinMux(BOARD_USDHC1_CD_IOMUXC, false);
            IOMUXC_SetPinConfig(BOARD_USDHC1_CD_IOMUXC, usdhc_cd_pin_settings);
        }
        else if (base == BOARD_USDHC2_BASEADDR)
        {
            IOMUXC_SetPinMux(BOARD_USDHC2_CD_IOMUXC, false);
            IOMUXC_SetPinConfig(BOARD_USDHC2_CD_IOMUXC, usdhc_cd_pin_settings);
        }
    }
    else if (cdType == kSDMMCHOST_DetectCardByHostDATA3)
    {
        if (base == BOARD_USDHC1_BASEADDR)
        {
            IOMUXC_SetPinMux(BOARD_USDHC1_DATA3_IOMUXC, false);
            IOMUXC_SetPinConfig(BOARD_USDHC1_DATA3_IOMUXC, usdhc_cd_dat3_settings);
        }
        else if (base == BOARD_USDHC2_BASEADDR)
        {
            IOMUXC_SetPinMux(BOARD_USDHC2_DATA3_IOMUXC, false);
            IOMUXC_SetPinConfig(BOARD_USDHC2_DATA3_IOMUXC, usdhc_cd_dat3_settings);
        }
    }
}

uint32_t usdhc_cd_gpio_status(USDHC_Type *base)
{
    if (base == BOARD_USDHC1_BASEADDR)
    {
        return GPIO_ReadPinInput(BOARD_USDHC1_CD_GPIO_BASE, BOARD_USDHC1_CD_GPIO_PIN);
    }
    else if (base == BOARD_USDHC2_BASEADDR)
    {
        return GPIO_ReadPinInput(BOARD_USDHC2_CD_GPIO_BASE, BOARD_USDHC2_CD_GPIO_PIN);
    }
    else
    {
        return 0;
    }
}

void mmc_pinmux_config(USDHC_Type *base, mmc_data_bus_width_t busWidth)
{
    if (base == BOARD_USDHC1_BASEADDR)
    {
        switch (busWidth)
        {
            case kMMC_DataBusWidth8bitDDR:
            case kMMC_DataBusWidth8bit:
            case kMMC_DataBusWidth4bitDDR:
            case kMMC_DataBusWidth4bit:
                IOMUXC_SetPinMux(BOARD_USDHC1_DATA1_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC1_DATA2_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC1_DATA3_IOMUXC, false);
                IOMUXC_SetPinConfig(BOARD_USDHC1_DATA1_IOMUXC, usdhc_data_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC1_DATA2_IOMUXC, usdhc_data_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC1_DATA3_IOMUXC, usdhc_data_pin_settings);
            case kMMC_DataBusWidth1bit:
            default:
                IOMUXC_SetPinMux(BOARD_USDHC1_CMD_IOMUXC, true);
                IOMUXC_SetPinMux(BOARD_USDHC1_CLK_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC1_DATA0_IOMUXC, false);
                IOMUXC_SetPinConfig(BOARD_USDHC1_CMD_IOMUXC, usdhc_cmd_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC1_CLK_IOMUXC, usdhc_clk_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC1_DATA0_IOMUXC, usdhc_data_pin_settings);
        }
    }
    else if (base == BOARD_USDHC2_BASEADDR)
    {
        switch (busWidth)
        {
            case kMMC_DataBusWidth8bitDDR:
            case kMMC_DataBusWidth8bit:
                IOMUXC_SetPinMux(BOARD_USDHC2_DATA4_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC2_DATA5_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC2_DATA6_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC2_DATA7_IOMUXC, false);
                IOMUXC_SetPinConfig(BOARD_USDHC2_DATA4_IOMUXC, usdhc_data_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC2_DATA5_IOMUXC, usdhc_data_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC2_DATA6_IOMUXC, usdhc_data_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC2_DATA7_IOMUXC, usdhc_data_pin_settings);
            case kMMC_DataBusWidth4bitDDR:
            case kMMC_DataBusWidth4bit:
                IOMUXC_SetPinMux(BOARD_USDHC2_DATA1_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC2_DATA2_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC2_DATA3_IOMUXC, false);
                IOMUXC_SetPinConfig(BOARD_USDHC2_DATA1_IOMUXC, usdhc_data_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC2_DATA2_IOMUXC, usdhc_data_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC2_DATA3_IOMUXC, usdhc_data_pin_settings);
            case kMMC_DataBusWidth1bit:
            default:
                IOMUXC_SetPinMux(BOARD_USDHC2_CMD_IOMUXC, true);
                IOMUXC_SetPinMux(BOARD_USDHC2_CLK_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC2_DATA0_IOMUXC, false);
                IOMUXC_SetPinConfig(BOARD_USDHC2_CMD_IOMUXC, usdhc_cmd_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC2_CLK_IOMUXC, usdhc_clk_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC2_DATA0_IOMUXC, usdhc_data_pin_settings);
        }
    }
}

status_t get_mmc_default_configuration(mmc_card_t *card)
{
    status_t status = kStatus_Fail;
    return status;
}

void sd_pinmux_config(USDHC_Type *base, sd_data_bus_width_t busWidth)
{
    if (base == BOARD_USDHC1_BASEADDR)
    {
        switch (busWidth)
        {
            case kUSDHC_DataBusWidth4Bit:
                IOMUXC_SetPinMux(BOARD_USDHC1_DATA1_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC1_DATA2_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC1_DATA3_IOMUXC, false);
                IOMUXC_SetPinConfig(BOARD_USDHC1_DATA1_IOMUXC, usdhc_data_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC1_DATA2_IOMUXC, usdhc_data_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC1_DATA3_IOMUXC, usdhc_data_pin_settings);
            case kUSDHC_DataBusWidth1Bit:
                IOMUXC_SetPinMux(BOARD_USDHC1_CMD_IOMUXC, true);
                IOMUXC_SetPinMux(BOARD_USDHC1_CLK_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC1_DATA0_IOMUXC, false);
                IOMUXC_SetPinConfig(BOARD_USDHC1_CMD_IOMUXC, usdhc_cmd_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC1_CLK_IOMUXC, usdhc_clk_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC1_DATA0_IOMUXC, usdhc_data_pin_settings);
        }
        if (busWidth == kSD_DataBusWidth1Bit)
        {
            /*
             * Note: Pull-down on DATA3 pin is enabled by default. Disable it to let SD Card enter the SD mode.
             */
            IOMUXC_SetPinConfig(BOARD_USDHC1_DATA3_IOMUXC, usdhc_data_pin_nopull);
        }
    }
    else if (base == BOARD_USDHC2_BASEADDR)
    {
        switch (busWidth)
        {
            case kUSDHC_DataBusWidth4Bit:
                IOMUXC_SetPinMux(BOARD_USDHC2_DATA1_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC2_DATA2_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC2_DATA3_IOMUXC, false);
                IOMUXC_SetPinConfig(BOARD_USDHC2_DATA1_IOMUXC, usdhc_data_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC2_DATA2_IOMUXC, usdhc_data_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC2_DATA3_IOMUXC, usdhc_data_pin_settings);
            case kUSDHC_DataBusWidth1Bit:
                IOMUXC_SetPinMux(BOARD_USDHC2_CMD_IOMUXC, true);
                IOMUXC_SetPinMux(BOARD_USDHC2_CLK_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC2_DATA0_IOMUXC, false);
                IOMUXC_SetPinConfig(BOARD_USDHC2_CMD_IOMUXC, usdhc_cmd_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC2_CLK_IOMUXC, usdhc_clk_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC2_DATA0_IOMUXC, usdhc_data_pin_settings);
        }
        if (busWidth == kSD_DataBusWidth1Bit)
        {
            /*
             * Note: Pull-down on DATA3 pin is enabled by default. Disable it to let SD Card enter the SD mode.
             */
            IOMUXC_SetPinConfig(BOARD_USDHC2_DATA3_IOMUXC, usdhc_data_pin_nopull);
        }
    }
}

status_t get_sd_default_configuration(sd_card_t *card)
{
    status_t status = kStatus_Fail;
    return status;
}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
