/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if !defined(__SDMMC_INIT_H__)
#define __SDMMC_INIT_H__

#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#if FSL_FEATURE_SOC_PORT_COUNT
#include "fsl_port.h"
#endif
#include "bootloader_common.h"
#include "fsl_sdmmc_common.h"
#include "fsl_sdmmc_host.h"
#include "fsl_sdmmc_spec.h"
/******************************************************************************
 * Definitions.
 *****************************************************************************/
enum
{
    kSDMMC_PWR_DOWN_20MS = 0,
    kSDMMC_PWR_DOWN_10MS = 1,
    kSDMMC_PWR_DOWN_5MS = 2,
    kSDMMC_PWR_DOWN_2D5MS = 3,

    kSDMMC_PWR_UP_5MS = 0,
    kSDMMC_PWR_UP_2D5MS = 1,
};

////////////////////////////////////////////////////////////////////////////////
// Basic Definitions
////////////////////////////////////////////////////////////////////////////////
/*! @brief USDHC base address. */
#define BOARD_USDHC1_BASEADDR USDHC1
#define BOARD_USDHC2_BASEADDR USDHC2
/*! @brief USDHC frequency. */
#define BOARD_USDHC1_CLK_FREQ (CLOCK_GetFreq(kCLOCK_SysPll2Pfd2) / CLOCK_GetRootClockDiv(kCLOCK_Root_Usdhc1))
#define BOARD_USDHC2_CLK_FREQ (CLOCK_GetFreq(kCLOCK_SysPll2Pfd2) / CLOCK_GetRootClockDiv(kCLOCK_Root_Usdhc2))
/*! @brief USDHC pin port. */
#define BOARD_USDHC1_CD_GPIO_BASE (GPIO3)
#define BOARD_USDHC1_CD_GPIO_PIN (31)
#define BOARD_USDHC1_CD_GPIO_IOMUXC IOMUXC_GPIO_AD_32_GPIO_MUX3_IO31
#define BOARD_USDHC1_CD_IOMUXC IOMUXC_GPIO_AD_32_USDHC1_CD_B
#define BOARD_USDHC1_CMD_IOMUXC IOMUXC_GPIO_SD_B1_00_USDHC1_CMD
#define BOARD_USDHC1_CLK_IOMUXC IOMUXC_GPIO_SD_B1_01_USDHC1_CLK
#define BOARD_USDHC1_DATA0_IOMUXC IOMUXC_GPIO_SD_B1_02_USDHC1_DATA0
#define BOARD_USDHC1_DATA1_IOMUXC IOMUXC_GPIO_SD_B1_03_USDHC1_DATA1
#define BOARD_USDHC1_DATA2_IOMUXC IOMUXC_GPIO_SD_B1_04_USDHC1_DATA2
#define BOARD_USDHC1_DATA3_IOMUXC IOMUXC_GPIO_SD_B1_05_USDHC1_DATA3
#define BOARD_USDHC1_VSELECT_IOMUXC IOMUXC_GPIO_AD_34_USDHC1_VSELECT
#define BOARD_USDHC1_RESET_B_GPIO_BASE (GPIO4)
#define BOARD_USDHC1_RESET_B_GPIO_PIN (2)
#define BOARD_USDHC1_RESET_B_GPIO_IOMUXC IOMUXC_GPIO_AD_35_GPIO_MUX4_IO02

#define BOARD_USDHC2_CD_GPIO_BASE (GPIO3)
#define BOARD_USDHC2_CD_GPIO_PIN (25)
#define BOARD_USDHC2_CD_GPIO_IOMUXC IOMUXC_GPIO_AD_26_GPIO_MUX3_IO25
#define BOARD_USDHC2_CD_IOMUXC IOMUXC_GPIO_AD_26_USDHC2_CD_B
#define BOARD_USDHC2_CMD_IOMUXC IOMUXC_GPIO_SD_B2_05_USDHC2_CMD
#define BOARD_USDHC2_CLK_IOMUXC IOMUXC_GPIO_SD_B2_04_USDHC2_CLK
#define BOARD_USDHC2_DATA0_IOMUXC IOMUXC_GPIO_SD_B2_03_USDHC2_DATA0
#define BOARD_USDHC2_DATA1_IOMUXC IOMUXC_GPIO_SD_B2_02_USDHC2_DATA1
#define BOARD_USDHC2_DATA2_IOMUXC IOMUXC_GPIO_SD_B2_01_USDHC2_DATA2
#define BOARD_USDHC2_DATA3_IOMUXC IOMUXC_GPIO_SD_B2_00_USDHC2_DATA3
#define BOARD_USDHC2_DATA4_IOMUXC IOMUXC_GPIO_SD_B2_08_USDHC2_DATA4
#define BOARD_USDHC2_DATA5_IOMUXC IOMUXC_GPIO_SD_B2_09_USDHC2_DATA5
#define BOARD_USDHC2_DATA6_IOMUXC IOMUXC_GPIO_SD_B2_10_USDHC2_DATA6
#define BOARD_USDHC2_DATA7_IOMUXC IOMUXC_GPIO_SD_B2_11_USDHC2_DATA7
#define BOARD_USDHC2_VSELECT_IOMUXC IOMUXC_GPIO_AD_28_USDHC2_VSELECT
#define BOARD_USDHC2_RESET_B_GPIO_BASE (GPIO4)
#define BOARD_USDHC2_RESET_B_GPIO_PIN (15)
#define BOARD_USDHC2_RESET_B_GPIO_IOMUXC IOMUXC_GPIO_SD_B2_06_GPIO_MUX4_IO15

#if BL_FEATURE_SD_MODULE_HAS_CARD_DETECT
#if defined(BL_FEATURE_SD_MODULE_CARD_DETECT_LEVEL)
#define BOARD_USDHC_CARD_INSERT_CD_LEVEL (BL_FEATURE_SD_MODULE_CARD_DETECT_LEVEL)
#endif
#if defined(BL_FEATURE_SD_MODULE_CARD_DETECT_TIMEOUT)
#define BOARD_TIMEOUT_CARD_DETECT (BL_FEATURE_SD_MODULE_CARD_DETECT_TIMEOUT)
#else
#define BOARD_TIMEOUT_CARD_DETECT 0
#endif
#endif

#if BL_FEATURE_ENABLE_MKBOOT_DEVICE_MMC
#define BOARD_USDHC_ADMA_TABLE_MAX_ENTRIES (512)
#endif
////////////////////////////////////////////////////////////////////////////////
// Basic FUNC Definitions
////////////////////////////////////////////////////////////////////////////////
extern void usdhc_power_control_init(USDHC_Type *base);
extern void usdhc_power_control(USDHC_Type *base, bool state);
extern void usdhc_vselect_init(USDHC_Type *base);
extern void usdhc_cd_init(USDHC_Type *base, const sdmmchost_detect_card_type_t cdType);
extern uint32_t usdhc_cd_gpio_status(USDHC_Type *base);
extern void mmc_pinmux_config(USDHC_Type *base, mmc_data_bus_width_t busWidth);
extern void mmc_io_update(USDHC_Type *base, mmc_data_bus_width_t busWidth, uint32_t speed, uint32_t strength);
extern void sd_pinmux_config(USDHC_Type *base, sd_data_bus_width_t busWidth);
extern void sd_io_update(USDHC_Type *base, sd_data_bus_width_t busWidth, uint32_t speed, uint32_t strength);

////////////////////////////////////////////////////////////////////////////////
// Board FUNC Definitions
////////////////////////////////////////////////////////////////////////////////
/*! @brief Power control init function. */
#define BOARD_USDHC_POWER_CONTROL_INIT(base) usdhc_power_control_init(base)
/*! @brief Power control enable/disable function. */
#define BOARD_USDHC_POWER_CONTROL(base, state) usdhc_power_control(base, state)

/*! @brief vselect function. */
#define BOARD_USDHC_VSELECT_INIT(base) usdhc_vselect_init(base)

/*! @brief CD pin init function. */
#define BOARD_USDHC_CD_INIT(base, cdType) usdhc_cd_init(base, cdType)

/*! @brief Get CD GPIO levle. */
#define BOARD_USDHC_CD_GPIO_STATUS(base) usdhc_cd_gpio_status(base)

/*! @brief the CD pin voltage level when card is inserted. */
#define BOARD_USDHC_CARD_INSERT_CD_LEVEL (0)

/*! @brief SD power control init function. Not used for ROM */
#define BOARD_USDHC_SDCARD_POWER_CONTROL_INIT()
/*! @brief SD power enable/disable function. Not used for ROM */
#define BOARD_USDHC_SDCARD_POWER_CONTROL(state)

/*! @brief SD power control init function. Used for ROM */
#define BOARD_USDHC_SDCARD_RESET_CONTROL_INIT(base) BOARD_USDHC_POWER_CONTROL_INIT(base)
/*! @brief SD power enable/disable function. Used for ROM */
#define BOARD_USDHC_SDCARD_RESET_CONTROL(base, state) BOARD_USDHC_POWER_CONTROL(base, state)

/*! @brief SD vselect init function. */
#define BOARD_USDHC_SDCARD_VSELECT_INIT(base) BOARD_USDHC_VSELECT_INIT(base)

/*! @brief SD MUX/PAD config fucntion */
#define BOARD_SD_MUX_CONFIG(base, busWidth) sd_pinmux_config(base, busWidth)

/*! @brief SD config fucntion for KSDK, not really used for ROM */
#define BOARD_SD_PIN_CONFIG(speed, strength)
/*! @brief Define for SD config IO driver strength dynamic */
#define BOARD_SD_IO_UPDATE(base, busWidth, speed, strength) /* No need to update for ROM */

/*! @brief SD card detection pin config fucntion for KSDK, not really used for ROM */
#define BOARD_USDHC_CD_GPIO_INIT()
/*! @brief SD card detection status fucntion for KSDK, not really used for ROM */
#define BOARD_USDHC_CD_STATUS() (0)

/*! @brief MMC power control init function. Not used for ROM */
#define BOARD_USDHC_MMCCARD_POWER_CONTROL_INIT()
/*! @brief MMC power enable/disable function. Not used for ROM */
#define BOARD_USDHC_MMCCARD_POWER_CONTROL(state)

/*! @brief MMC power control init function. Used for ROM */
#define BOARD_USDHC_MMCCARD_RESET_CONTROL_INIT(base) BOARD_USDHC_POWER_CONTROL_INIT(base)
/*! @brief MMC power enable/disable function. Used for ROM */
#define BOARD_USDHC_MMCCARD_RESET_CONTROL(base, state) BOARD_USDHC_POWER_CONTROL(base, state)

/*! @brief MMC vselect init function. */
#define BOARD_USDHC_MMCCARD_VSELECT_INIT(base) BOARD_USDHC_VSELECT_INIT(base)

/*! @brief MMC MUX/PAD config function. */
#define BOARD_MMC_MUX_CONFIG(base, busWidth) mmc_pinmux_config(base, busWidth)

/*! @brief MMC config fucntion for KSDK, not used for ROM */
#define BOARD_MMC_PIN_CONFIG(speed, strength)
/*! @brief Define for MMC config IO driver strength dynamic */
#define BOARD_MMC_IO_UPDATE(base, busWidth, speed, strength) /* No need to update for ROM */

#endif // __SDMMC_INIT_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
