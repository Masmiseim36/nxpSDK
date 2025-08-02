/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "fsl_assert.h"
#include "fsl_mmc.h"
#include "fsl_sd.h"
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
                                           IOMUXC_SW_PAD_CTL_PAD_ODE(0) | IOMUXC_SW_PAD_CTL_PAD_IBE_OFF(0) |
                                           IOMUXC_SW_PAD_CTL_PAD_DWP(0) | IOMUXC_SW_PAD_CTL_PAD_DWP_LOCK(0);
const uint32_t usdhc2_reset_pin_settings = IOMUXC_SW_PAD_CTL_PAD_PDRV(0) | IOMUXC_SW_PAD_CTL_PAD_PULL(3) |
                                           IOMUXC_SW_PAD_CTL_PAD_ODE(0) | IOMUXC_SW_PAD_CTL_PAD_DWP(0) |
                                           IOMUXC_SW_PAD_CTL_PAD_DWP_LOCK(0);
const uint32_t usdhc_vselect_pin_settings = IOMUXC_SW_PAD_CTL_PAD_SRE(1) | IOMUXC_SW_PAD_CTL_PAD_DSE(0) |
                                            IOMUXC_SW_PAD_CTL_PAD_PUE(1) | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |
                                            IOMUXC_SW_PAD_CTL_PAD_ODE(0) | IOMUXC_SW_PAD_CTL_PAD_IBE_OFF(0) |
                                            IOMUXC_SW_PAD_CTL_PAD_DWP(0) | IOMUXC_SW_PAD_CTL_PAD_DWP_LOCK(0);

const uint32_t usdhc_cmd_pin_settings = IOMUXC_SW_PAD_CTL_PAD_PDRV(0) | IOMUXC_SW_PAD_CTL_PAD_PULL(1) |
                                        IOMUXC_SW_PAD_CTL_PAD_ODE(0) | IOMUXC_SW_PAD_CTL_PAD_DWP(0) |
                                        IOMUXC_SW_PAD_CTL_PAD_DWP_LOCK(0);
const uint32_t usdhc_clk_pin_settings = IOMUXC_SW_PAD_CTL_PAD_PDRV(0) | IOMUXC_SW_PAD_CTL_PAD_PULL(1) |
                                        IOMUXC_SW_PAD_CTL_PAD_ODE(0) | IOMUXC_SW_PAD_CTL_PAD_DWP(0) |
                                        IOMUXC_SW_PAD_CTL_PAD_DWP_LOCK(0);
const uint32_t usdhc_data_pin_settings = IOMUXC_SW_PAD_CTL_PAD_PDRV(0) | IOMUXC_SW_PAD_CTL_PAD_PULL(1) |
                                         IOMUXC_SW_PAD_CTL_PAD_ODE(0) | IOMUXC_SW_PAD_CTL_PAD_DWP(0) |
                                         IOMUXC_SW_PAD_CTL_PAD_DWP_LOCK(0);
const uint32_t usdhc_data_pin_nopull = IOMUXC_SW_PAD_CTL_PAD_PDRV(0) | IOMUXC_SW_PAD_CTL_PAD_PULL(3) |
                                       IOMUXC_SW_PAD_CTL_PAD_ODE(0) | IOMUXC_SW_PAD_CTL_PAD_DWP(0) |
                                       IOMUXC_SW_PAD_CTL_PAD_DWP_LOCK(0);

/*******************************************************************************
 * Code
 ******************************************************************************/
void usdhc_power_control_init(USDHC_Type *base)
{
    if (base == BOARD_USDHC1_BASEADDR)
    {
        IOMUXC_SetPinMux(BOARD_USDHC1_RESET_B_GPIO_IOMUXC, false);
        IOMUXC_SetPinConfig(BOARD_USDHC1_RESET_B_GPIO_IOMUXC, usdhc1_reset_pin_settings);
        rgpio_pin_config_t sw_config = { kRGPIO_DigitalOutput, 0 };
        RGPIO_PinInit(BOARD_USDHC1_RESET_B_GPIO_BASE, BOARD_USDHC1_RESET_B_GPIO_PIN, &sw_config);
    }
    else if (base == BOARD_USDHC2_BASEADDR)
    {
        IOMUXC_SetPinMux(BOARD_USDHC2_RESET_B_GPIO_IOMUXC, false);
        IOMUXC_SetPinConfig(BOARD_USDHC2_RESET_B_GPIO_IOMUXC, usdhc2_reset_pin_settings);
        rgpio_pin_config_t sw_config = { kRGPIO_DigitalOutput, 0 };
        RGPIO_PinInit(BOARD_USDHC2_RESET_B_GPIO_BASE, BOARD_USDHC2_RESET_B_GPIO_PIN, &sw_config);
    }
}

void usdhc_power_control(USDHC_Type *base, bool state)
{
    if (base == BOARD_USDHC1_BASEADDR)
    {
        RGPIO_WritePinOutput(BOARD_USDHC1_RESET_B_GPIO_BASE, BOARD_USDHC1_RESET_B_GPIO_PIN, state);
    }
    else if (base == BOARD_USDHC2_BASEADDR)
    {
        RGPIO_WritePinOutput(BOARD_USDHC2_RESET_B_GPIO_BASE, BOARD_USDHC2_RESET_B_GPIO_PIN, state);
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
            case kSD_DataBusWidth4Bit:
                IOMUXC_SetPinMux(BOARD_USDHC1_DATA1_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC1_DATA2_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC1_DATA3_IOMUXC, false);
                IOMUXC_SetPinConfig(BOARD_USDHC1_DATA1_IOMUXC, usdhc_data_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC1_DATA2_IOMUXC, usdhc_data_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC1_DATA3_IOMUXC, usdhc_data_pin_settings);
            case kSD_DataBusWidth1Bit:
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
            case kSD_DataBusWidth4Bit:
                IOMUXC_SetPinMux(BOARD_USDHC2_DATA1_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC2_DATA2_IOMUXC, false);
                IOMUXC_SetPinMux(BOARD_USDHC2_DATA3_IOMUXC, false);
                IOMUXC_SetPinConfig(BOARD_USDHC2_DATA1_IOMUXC, usdhc_data_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC2_DATA2_IOMUXC, usdhc_data_pin_settings);
                IOMUXC_SetPinConfig(BOARD_USDHC2_DATA3_IOMUXC, usdhc_data_pin_settings);
            case kSD_DataBusWidth1Bit:
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
