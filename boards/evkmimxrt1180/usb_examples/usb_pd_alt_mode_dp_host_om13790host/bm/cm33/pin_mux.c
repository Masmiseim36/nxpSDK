/*
 * Copyright 2022 - 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v14.0
processor: MIMXRT1189xxxxx
package_id: MIMXRT1189CVM8B
mcu_data: ksdk2_0
processor_version: 0.14.8
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 * 
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 * 
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void) {
    BOARD_InitPins();
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: A5, peripheral: LPUART1, signal: RXD, pin_signal: GPIO_AON_09, software_input_on: Disable, pull_up_down_config: Pull_Down, pull_keeper_select: Keeper,
    open_drain: Disable, drive_strength: High, slew_rate: Slow}
  - {pin_num: B1, peripheral: LPUART1, signal: TXD, pin_signal: GPIO_AON_08, software_input_on: Disable, pull_up_down_config: Pull_Down, pull_keeper_select: Keeper,
    open_drain: Disable, drive_strength: High, slew_rate: Slow}
  - {pin_num: N16, peripheral: RGPIO4, signal: 'gpio_io, 15', pin_signal: GPIO_AD_15, software_input_on: Disable, pull_up_down_config: Pull_Down, pull_keeper_select: Pull,
    open_drain: Disable, drive_strength: High, slew_rate: Slow, force_ibe_off: Disable}
  - {pin_num: P17, peripheral: RGPIO4, signal: 'gpio_io, 12', pin_signal: GPIO_AD_12, software_input_on: Disable, pull_up_down_config: Pull_Up, pull_keeper_select: Pull,
    open_drain: Disable, drive_strength: High, slew_rate: Slow, force_ibe_off: Disable}
  - {pin_num: N15, peripheral: RGPIO4, signal: 'gpio_io, 13', pin_signal: GPIO_AD_13, software_input_on: Disable, pull_up_down_config: Pull_Down, pull_keeper_select: Pull,
    open_drain: Disable, drive_strength: High, slew_rate: Slow, force_ibe_off: Disable}
  - {pin_num: T17, peripheral: RGPIO4, signal: 'gpio_io, 07', pin_signal: GPIO_AD_07, software_input_on: Disable, pull_up_down_config: Pull_Down, pull_keeper_select: Pull,
    open_drain: Disable, drive_strength: High, slew_rate: Slow, force_ibe_off: Disable}
  - {pin_num: R16, peripheral: RGPIO4, signal: 'gpio_io, 09', pin_signal: GPIO_AD_09, software_input_on: Disable, pull_up_down_config: Pull_Down, pull_keeper_select: Pull,
    open_drain: Disable, drive_strength: High, slew_rate: Slow, force_ibe_off: Disable}
  - {pin_num: R17, peripheral: RGPIO4, signal: 'gpio_io, 10', pin_signal: GPIO_AD_10, software_input_on: Disable, pull_up_down_config: Pull_Down, pull_keeper_select: Pull,
    open_drain: Disable, drive_strength: High, slew_rate: Slow, force_ibe_off: Disable}
  - {pin_num: N13, peripheral: RGPIO4, signal: 'gpio_io, 06', pin_signal: GPIO_AD_06, software_input_on: Disable, pull_up_down_config: Pull_Down, pull_keeper_select: Pull,
    open_drain: Disable, drive_strength: High, slew_rate: Slow, force_ibe_off: Disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins, assigned for the Cortex-M33 core.
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc1);          /* Turn on LPCG: LPCG is ON. */
  CLOCK_EnableClock(kCLOCK_Iomuxc2);          /* Turn on LPCG: LPCG is ON. */

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_06_GPIO4_IO06,           /* GPIO_AD_06 is configured as GPIO4_IO06 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_07_GPIO4_IO07,           /* GPIO_AD_07 is configured as GPIO4_IO07 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_09_GPIO4_IO09,           /* GPIO_AD_09 is configured as GPIO4_IO09 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_10_GPIO4_IO10,           /* GPIO_AD_10 is configured as GPIO4_IO10 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_12_GPIO4_IO12,           /* GPIO_AD_12 is configured as GPIO4_IO12 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_13_GPIO4_IO13,           /* GPIO_AD_13 is configured as GPIO4_IO13 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_15_GPIO4_IO15,           /* GPIO_AD_15 is configured as GPIO4_IO15 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_08_LPUART1_TX,          /* GPIO_AON_08 is configured as LPUART1_TX */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_09_LPUART1_RX,          /* GPIO_AON_09 is configured as LPUART1_RX */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_06_GPIO4_IO06,           /* GPIO_AD_06 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled
                                                 Force ibe off Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_07_GPIO4_IO07,           /* GPIO_AD_07 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled
                                                 Force ibe off Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_09_GPIO4_IO09,           /* GPIO_AD_09 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled
                                                 Force ibe off Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_10_GPIO4_IO10,           /* GPIO_AD_10 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled
                                                 Force ibe off Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_12_GPIO4_IO12,           /* GPIO_AD_12 PAD functional properties : */
      0x0EU);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull up
                                                 Open Drain Field: Disabled
                                                 Force ibe off Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_13_GPIO4_IO13,           /* GPIO_AD_13 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled
                                                 Force ibe off Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_15_GPIO4_IO15,           /* GPIO_AD_15 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled
                                                 Force ibe off Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AON_08_LPUART1_TX,          /* GPIO_AON_08 PAD functional properties : */
      0x02U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Disable, Highz
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AON_09_LPUART1_RX,          /* GPIO_AON_09 PAD functional properties : */
      0x02U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Disable, Highz
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled */
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPI2C2_InitPins:
- options: {callFromInitBoot: 'false', coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: B3, peripheral: LPI2C2, signal: SDA, pin_signal: GPIO_AON_15, software_input_on: Enable, pull_up_down_config: Pull_Down, pull_keeper_select: Keeper,
    open_drain: Disable, drive_strength: Normal, slew_rate: Slow}
  - {pin_num: C5, peripheral: LPI2C2, signal: SCL, pin_signal: GPIO_AON_16, software_input_on: Enable, pull_up_down_config: Pull_Down, pull_keeper_select: Keeper,
    open_drain: Disable, drive_strength: Normal, slew_rate: Slow}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPI2C2_InitPins, assigned for the Cortex-M33 core.
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPI2C2_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc2);          /* Turn on LPCG: LPCG is ON. */

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_15_LPI2C2_SDA,          /* GPIO_AON_15 is configured as LPI2C2_SDA */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_AON_15 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_16_LPI2C2_SCL,          /* GPIO_AON_16 is configured as LPI2C2_SCL */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_AON_16 */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AON_15_LPI2C2_SDA,          /* GPIO_AON_15 PAD functional properties : */
      0x00U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: normal driver
                                                 Pull / Keep Select Field: Pull Disable, Highz
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AON_16_LPI2C2_SCL,          /* GPIO_AON_16 PAD functional properties : */
      0x00U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: normal driver
                                                 Pull / Keep Select Field: Pull Disable, Highz
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled */
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPI2C2_DeinitPins:
- options: {callFromInitBoot: 'false', coreID: cm33, enableClock: 'true'}
- pin_list:
  - {pin_num: B3, peripheral: RGPIO1, signal: 'gpio_io, 15', pin_signal: GPIO_AON_15, software_input_on: Disable, pull_up_down_config: Pull_Down, pull_keeper_select: Pull,
    open_drain: Disable, drive_strength: High, slew_rate: Slow}
  - {pin_num: C5, peripheral: RGPIO1, signal: 'gpio_io, 16', pin_signal: GPIO_AON_16, software_input_on: Disable, pull_up_down_config: Pull_Down, pull_keeper_select: Pull,
    open_drain: Disable, drive_strength: High, slew_rate: Slow}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPI2C2_DeinitPins, assigned for the Cortex-M33 core.
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPI2C2_DeinitPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc2);          /* Turn on LPCG: LPCG is ON. */

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_15_GPIO1_IO15,          /* GPIO_AON_15 is configured as GPIO1_IO15 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_16_GPIO1_IO16,          /* GPIO_AON_16 is configured as GPIO1_IO16 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AON_15_GPIO1_IO15,          /* GPIO_AON_15 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AON_16_GPIO1_IO16,          /* GPIO_AON_16 PAD functional properties : */
      0x06U);                                 /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: high driver
                                                 Pull / Keep Select Field: Pull Enable
                                                 Pull Up / Down Config. Field: Weak pull down
                                                 Open Drain Field: Disabled */
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
