/*
 * Copyright 2017-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "bootloader_common.h"
#include "fsl_assert.h"
#include "fsl_device_registers.h"
#include "fsl_sdmmc_common.h"
#include "fusemap.h"
#include "sdmmc_init.h"
#if BL_FEATURE_MMC_MODULE
#include "mmc_memory.h"
#endif
#if BL_FEATURE_SD_MODULE
#include "sd_memory.h"
#endif
/*******************************************************************************
 * Definitons
 ******************************************************************************/
#define SD_SELECT (1)
#define MMC_SELECT (2)

#if BL_FEATURE_MMC_MODULE
#if BL_FEATURE_MMC_MODULE_PERIPHERAL_INSTANCE == 1
#define MMC_SELECTED_INSTANCE (0)
#define ROM_OCOTP_MMC_RST_ACTIVE_POLARITY_VALUE() ROM_OCOTP_SD1_RST_ACTIVE_POLARITY_VALUE()
#define ROM_OCOTP_MMC_VOLTAGE_SELECTION_VALUE() ROM_OCOTP_SD1_VOLTAGE_SELECTION_VALUE()
#elif BL_FEATURE_MMC_MODULE_PERIPHERAL_INSTANCE == 2
#define MMC_SELECTED_INSTANCE (1)
#define ROM_OCOTP_MMC_RST_ACTIVE_POLARITY_VALUE() ROM_OCOTP_SD2_RST_ACTIVE_POLARITY_VALUE()
#define ROM_OCOTP_MMC_VOLTAGE_SELECTION_VALUE() ROM_OCOTP_SD2_VOLTAGE_SELECTION_VALUE()
#else
#error "incorrect MMC instance."
#endif
#endif // #if BL_FEATURE_MMC_MODULE

#if BL_FEATURE_SD_MODULE
#if BL_FEATURE_SD_MODULE_PERIPHERAL_INSTANCE == 1
#define SD_SELECTED_INSTANCE (0)
#define ROM_OCOTP_SD_RST_ACTIVE_POLARITY_VALUE() ROM_OCOTP_SD1_RST_ACTIVE_POLARITY_VALUE()
#define ROM_OCOTP_SD_VOLTAGE_SELECTION_VALUE() ROM_OCOTP_SD1_VOLTAGE_SELECTION_VALUE()
#elif BL_FEATURE_SD_MODULE_PERIPHERAL_INSTANCE == 2
#define SD_SELECTED_INSTANCE (1)
#define ROM_OCOTP_SD_RST_ACTIVE_POLARITY_VALUE() ROM_OCOTP_SD2_RST_ACTIVE_POLARITY_VALUE()
#define ROM_OCOTP_SD_VOLTAGE_SELECTION_VALUE() ROM_OCOTP_SD2_VOLTAGE_SELECTION_VALUE()
#else
#error "incorrect SD instance."
#endif
#endif // #if BL_FEATURE_SD_MODULE

/*******************************************************************************
 * Variable
 ******************************************************************************/

const uint32_t usdhc_vselect_pin_settings = IOMUXC_SW_PAD_CTL_PAD_SRE_MASK | IOMUXC_SW_PAD_CTL_PAD_PKE_MASK |
                                            IOMUXC_SW_PAD_CTL_PAD_PUE_MASK | IOMUXC_SW_PAD_CTL_PAD_HYS_MASK |
                                            IOMUXC_SW_PAD_CTL_PAD_SPEED(2) | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |
                                            IOMUXC_SW_PAD_CTL_PAD_DSE(1);

/*******************************************************************************
 * Code
 ******************************************************************************/
void usdhc_power_control_init(USDHC_Type *base)
{
    if (base == BOARD_USDHC1_BASEADDR)
    {
        IOMUXC_SetPinMux(BOARD_USDHC1_RESET_B_GPIO_IOMUXC, false);
        gpio_pin_config_t sw_config = {
            kGPIO_DigitalOutput,
            0,
            kGPIO_NoIntmode,
        };
        GPIO_PinInit(BOARD_USDHC1_RESET_B_GPIO_BASE, BOARD_USDHC1_RESET_B_GPIO_PIN, &sw_config);
    }
    else if (base == BOARD_USDHC2_BASEADDR)
    {
        IOMUXC_SetPinMux(BOARD_USDHC2_RESET_B_GPIO_IOMUXC, false);
        gpio_pin_config_t sw_config = {
            kGPIO_DigitalOutput,
            0,
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

#if BL_FEATURE_MMC_MODULE
status_t get_mmc_default_configuration(mmc_card_t *card)
{
    if ((ROM_OCOTP_SDMMC_TYPE_SEL_VALUE() == MMC_SELECT) && (ROM_OCOTP_SDMMC_PORT_SEL_VALUE() == MMC_SELECTED_INSTANCE))
    {
        card->userConfig.timing = (mmc_high_speed_timing_t)ROM_OCOTP_MMC_SPEED_VALUE();

        switch (ROM_OCOTP_MMC_BUS_WIDTH_VALUE())
        {
            default:
            case 0:
                card->userConfig.busWidth = kMMC_DataBusWidth4bit;
                break;
            case 1:
                card->userConfig.busWidth = kMMC_DataBusWidth8bit;
                break;
            case 2:
                card->userConfig.busWidth = kMMC_DataBusWidth4bitDDR;
                break;
            case 3:
                card->userConfig.busWidth = kMMC_DataBusWidth8bitDDR;
                break;
        }

        card->userConfig.enablePowerCycle = ROM_OCOTP_SDMMC_POWER_CYCLE_ENABLE_VALUE();
        card->userConfig.powerPolarity = ROM_OCOTP_MMC_RST_ACTIVE_POLARITY_VALUE();

        switch (ROM_OCOTP_SDMMC_PWR_CYCLE_SEL_VALUE())
        {
            default:
            case kSDMMC_PWR_DOWN_20MS:
                card->userConfig.powerDownDelay_US = 20 * 1000;
                break;
            case kSDMMC_PWR_DOWN_10MS:
                card->userConfig.powerDownDelay_US = 10 * 1000;
                break;
            case kSDMMC_PWR_DOWN_5MS:
                card->userConfig.powerDownDelay_US = 5 * 1000;
                break;
            case kSDMMC_PWR_DOWN_2D5MS:
                card->userConfig.powerDownDelay_US = 25 * 100;
                break;
        }

        switch (ROM_OCOTP_SDMMC_PWR_STABLE_CYCLE_SEL_VALUE())
        {
            default:
            case kSDMMC_PWR_UP_5MS:
                card->userConfig.powerUpDelay_US = 5 * 1000;
                break;
            case kSDMMC_PWR_UP_2D5MS:
                card->userConfig.powerUpDelay_US = 25 * 100;
                break;
        }
        card->userConfig.switch1V8 = ROM_OCOTP_MMC_VOLTAGE_SELECTION_VALUE();

        return kStatus_Success;
    }
    else
    {
        // if current usdhc instance is not selected as MMC boot device, skip the boot time init.
        // user must init the MMC device by configure-memory command.
        return kStatus_Fail;
    }
}
#endif // #if BL_FEATURE_MMC_MODULE

#if BL_FEATURE_SD_MODULE
status_t get_sd_default_configuration(sd_card_t *card)
{
    if ((ROM_OCOTP_SDMMC_TYPE_SEL_VALUE() == SD_SELECT) && (ROM_OCOTP_SDMMC_PORT_SEL_VALUE() == SD_SELECTED_INSTANCE))
    {
        // do something here.
        card->userConfig.timing = (sd_timing_mode_t)ROM_OCOTP_SD_SPEED_VALUE();
        card->userConfig.busWidth = (sd_data_bus_width_t)ROM_OCOTP_SD_BUS_WIDTH_VALUE();
        card->userConfig.enablePowerCycle = ROM_OCOTP_SDMMC_POWER_CYCLE_ENABLE_VALUE();
        card->userConfig.powerPolarity = ROM_OCOTP_SD_RST_ACTIVE_POLARITY_VALUE();

        switch (ROM_OCOTP_SDMMC_PWR_CYCLE_SEL_VALUE())
        {
            default:
            case kSDMMC_PWR_DOWN_20MS:
                card->userConfig.powerDownDelay_US = 20 * 1000;
                break;
            case kSDMMC_PWR_DOWN_10MS:
                card->userConfig.powerDownDelay_US = 10 * 1000;
                break;
            case kSDMMC_PWR_DOWN_5MS:
                card->userConfig.powerDownDelay_US = 5 * 1000;
                break;
            case kSDMMC_PWR_DOWN_2D5MS:
                card->userConfig.powerDownDelay_US = 25 * 100;
                break;
        }

        switch (ROM_OCOTP_SDMMC_PWR_STABLE_CYCLE_SEL_VALUE())
        {
            default:
            case kSDMMC_PWR_UP_5MS:
                card->userConfig.powerUpDelay_US = 5 * 1000;
                break;
            case kSDMMC_PWR_UP_2D5MS:
                card->userConfig.powerUpDelay_US = 25 * 100;
                break;
        }
        return kStatus_Success;
    }
    else
    {
        // if current usdhc instance is not selected as SD boot device, skip the boot time init.
        // user must init the SD device by configure-memory command.
        return kStatus_Fail;
    }
}
#endif // #if BL_FEATURE_SD_MODULE
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
