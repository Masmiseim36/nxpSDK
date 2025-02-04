/*
 * Copyright 2020 NXP
 * All rights reserved.
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
product: Pins v8.0
processor: MIMXRT1024xxxxx
package_id: MIMXRT1024DAG5A
mcu_data: ksdk2_0
processor_version: 0.8.0
board: MIMXRT1024-EVK
pin_labels:
- {pin_num: '106', pin_signal: GPIO_AD_B0_05, label: USER_LED, identifier: USER_LED}
- {pin_num: '99', pin_signal: GPIO_AD_B0_09, label: WL_REG_ON, identifier: ENET_RXD1;WL_REG_ON}
- {pin_num: '83', pin_signal: GPIO_AD_B1_07, label: SD0_VSELECT, identifier: SD0_VSELECT}
- {pin_num: '82', pin_signal: GPIO_AD_B1_08, label: 'UART_TX/J17[2]', identifier: USER_LED}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 * 
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 * 
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void) {
    BOARD_InitPins();

/* GPIO_AD_B1_00~GPIO_AD_B1_05 can only be configured as flexspi function. Note that it can't be modified here */
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_00_FLEXSPI_A_DATA03,1U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_01_FLEXSPI_A_SCLK,1U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_02_FLEXSPI_A_DATA00,1U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_03_FLEXSPI_A_DATA02,1U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_04_FLEXSPI_A_DATA01,1U);
    IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B1_05_FLEXSPI_A_SS0_B,1U);
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '101', peripheral: LPUART1, signal: RX, pin_signal: GPIO_AD_B0_07, slew_rate: Slow, software_input_on: Disable, open_drain: Disable, speed: MHZ_100,
    drive_strength: R0_6, pull_keeper_select: Keeper, pull_keeper_enable: Enable, pull_up_down_config: Pull_Down_100K_Ohm, hysteresis_enable: Disable}
  - {pin_num: '105', peripheral: LPUART1, signal: TX, pin_signal: GPIO_AD_B0_06, slew_rate: Slow, software_input_on: Disable, open_drain: Disable, speed: MHZ_100,
    drive_strength: R0_6, pull_keeper_select: Keeper, pull_keeper_enable: Enable, pull_up_down_config: Pull_Down_100K_Ohm, hysteresis_enable: Disable}
  - {pin_num: '98', peripheral: ENET, signal: 'enet_rx_data, 0', pin_signal: GPIO_AD_B0_10, slew_rate: Fast, software_input_on: Disable, open_drain: Disable, speed: MHZ_200,
    drive_strength: R0_5, pull_keeper_select: Pull, pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_100K_Ohm, hysteresis_enable: Disable}
  - {pin_num: '99', peripheral: ENET, signal: 'enet_rx_data, 1', pin_signal: GPIO_AD_B0_09, slew_rate: Fast, software_input_on: Disable, open_drain: Disable, speed: MHZ_200,
    drive_strength: R0_5, pull_keeper_select: Pull, pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_100K_Ohm, hysteresis_enable: Disable}
  - {pin_num: '97', peripheral: ENET, signal: enet_rx_en, pin_signal: GPIO_AD_B0_11, slew_rate: Fast, software_input_on: Disable, open_drain: Disable, speed: MHZ_200,
    drive_strength: R0_5, pull_keeper_select: Pull, pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_100K_Ohm, hysteresis_enable: Disable}
  - {pin_num: '94', peripheral: ENET, signal: 'enet_tx_data, 0', pin_signal: GPIO_AD_B0_14, slew_rate: Fast, software_input_on: Disable, open_drain: Disable, speed: MHZ_200,
    drive_strength: R0_5, pull_keeper_select: Pull, pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_100K_Ohm, hysteresis_enable: Disable}
  - {pin_num: '93', peripheral: ENET, signal: 'enet_tx_data, 1', pin_signal: GPIO_AD_B0_15, slew_rate: Fast, software_input_on: Disable, open_drain: Disable, speed: MHZ_200,
    drive_strength: R0_5, pull_keeper_select: Pull, pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_100K_Ohm, hysteresis_enable: Disable}
  - {pin_num: '95', peripheral: ENET, signal: enet_tx_en, pin_signal: GPIO_AD_B0_13, slew_rate: Fast, software_input_on: Disable, open_drain: Disable, speed: MHZ_200,
    drive_strength: R0_5, pull_keeper_select: Pull, pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_100K_Ohm, hysteresis_enable: Disable}
  - {pin_num: '100', peripheral: ENET, signal: enet_ref_clk, pin_signal: GPIO_AD_B0_08, slew_rate: Fast, software_input_on: Enable, open_drain: Disable, speed: MHZ_200,
    drive_strength: R0_5, pull_keeper_select: Pull, pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_100K_Ohm, hysteresis_enable: Disable}
  - {pin_num: '96', peripheral: ENET, signal: enet_rx_er, pin_signal: GPIO_AD_B0_12, slew_rate: Fast, software_input_on: Disable, open_drain: Disable, speed: MHZ_200,
    drive_strength: R0_5, pull_keeper_select: Pull, pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_100K_Ohm, hysteresis_enable: Disable}
  - {pin_num: '116', peripheral: ENET, signal: enet_mdio, pin_signal: GPIO_EMC_40, slew_rate: Fast, software_input_on: Disable, open_drain: Disable, speed: MHZ_200,
    drive_strength: R0_5, pull_keeper_select: Pull, pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_100K_Ohm, hysteresis_enable: Disable}
  - {pin_num: '115', peripheral: ENET, signal: enet_mdc, pin_signal: GPIO_EMC_41, slew_rate: Fast, software_input_on: Disable, open_drain: Disable, speed: MHZ_200,
    drive_strength: R0_5, pull_keeper_select: Pull, pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_100K_Ohm, hysteresis_enable: Disable}
  - {pin_num: '107', peripheral: GPIO1, signal: 'gpio_io, 04', pin_signal: GPIO_AD_B0_04, slew_rate: Fast, software_input_on: Disable, open_drain: Disable, speed: MHZ_100,
    drive_strength: R0_5, pull_keeper_select: Pull, pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_100K_Ohm, hysteresis_enable: Disable}
  - {pin_num: '84', peripheral: GPIO1, signal: 'gpio_io, 22', pin_signal: GPIO_AD_B1_06, slew_rate: Fast, software_input_on: Disable, open_drain: Disable, speed: MHZ_100,
    drive_strength: R0_5, pull_keeper_select: Pull, pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_100K_Ohm, hysteresis_enable: Disable}
  - {pin_num: '82', peripheral: GPIO1, signal: 'gpio_io, 24', pin_signal: GPIO_AD_B1_08, direction: OUTPUT, gpio_init_state: 'false', slew_rate: Slow, software_input_on: Disable,
    open_drain: Disable, speed: MHZ_100, drive_strength: R0_6, pull_keeper_select: Keeper, pull_keeper_enable: Enable, pull_up_down_config: Pull_Down_100K_Ohm, hysteresis_enable: Disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */

  /* GPIO configuration of USER_LED on GPIO_AD_B1_08 (pin 82) */
  gpio_pin_config_t USER_LED_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_AD_B1_08 (pin 82) */
  GPIO_PinInit(GPIO1, 24U, &USER_LED_config);

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_04_GPIO1_IO04,        /* GPIO_AD_B0_04 is configured as GPIO1_IO04 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_06_LPUART1_TX,        /* GPIO_AD_B0_06 is configured as LPUART1_TX */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_07_LPUART1_RX,        /* GPIO_AD_B0_07 is configured as LPUART1_RX */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_08_ENET_REF_CLK1,     /* GPIO_AD_B0_08 is configured as ENET_REF_CLK1 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_AD_B0_08 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_09_ENET_RDATA01,      /* GPIO_AD_B0_09 is configured as ENET_RDATA01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_10_ENET_RDATA00,      /* GPIO_AD_B0_10 is configured as ENET_RDATA00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_11_ENET_RX_EN,        /* GPIO_AD_B0_11 is configured as ENET_RX_EN */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_12_ENET_RX_ER,        /* GPIO_AD_B0_12 is configured as ENET_RX_ER */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_13_ENET_TX_EN,        /* GPIO_AD_B0_13 is configured as ENET_TX_EN */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_14_ENET_TDATA00,      /* GPIO_AD_B0_14 is configured as ENET_TDATA00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_15_ENET_TDATA01,      /* GPIO_AD_B0_15 is configured as ENET_TDATA01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_06_GPIO1_IO22,        /* GPIO_AD_B1_06 is configured as GPIO1_IO22 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B1_08_GPIO1_IO24,        /* GPIO_AD_B1_08 is configured as GPIO1_IO24 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_40_ENET_MDIO,           /* GPIO_EMC_40 is configured as ENET_MDIO */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_41_ENET_MDC,            /* GPIO_EMC_41 is configured as ENET_MDC */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_04_GPIO1_IO04,        /* GPIO_AD_B0_04 PAD functional properties : */
      0xB0A9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_06_LPUART1_TX,        /* GPIO_AD_B0_06 PAD functional properties : */
      0x10B0U);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_07_LPUART1_RX,        /* GPIO_AD_B0_07 PAD functional properties : */
      0x10B0U);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_08_ENET_REF_CLK1,     /* GPIO_AD_B0_08 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_09_ENET_RDATA01,      /* GPIO_AD_B0_09 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_10_ENET_RDATA00,      /* GPIO_AD_B0_10 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_11_ENET_RX_EN,        /* GPIO_AD_B0_11 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_12_ENET_RX_ER,        /* GPIO_AD_B0_12 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_13_ENET_TX_EN,        /* GPIO_AD_B0_13 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_14_ENET_TDATA00,      /* GPIO_AD_B0_14 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_15_ENET_TDATA01,      /* GPIO_AD_B0_15 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B1_06_GPIO1_IO22,        /* GPIO_AD_B1_06 PAD functional properties : */
      0xB0A9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B1_08_GPIO1_IO24,        /* GPIO_AD_B1_08 PAD functional properties : */
      0x10B0U);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_EMC_40_ENET_MDIO,           /* GPIO_EMC_40 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_EMC_41_ENET_MDC,            /* GPIO_EMC_41 PAD functional properties : */
      0xB0E9U);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/5
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_I2C_ConfigurePins:
- options: {callFromInitBoot: 'false', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '30', peripheral: LPI2C4, signal: SCL, pin_signal: GPIO_SD_B1_02, slew_rate: Slow, software_input_on: Enable, open_drain: Enable, speed: MHZ_100, drive_strength: R0_6,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_22K_Ohm, hysteresis_enable: Disable}
  - {pin_num: '28', peripheral: LPI2C4, signal: SDA, pin_signal: GPIO_SD_B1_03, slew_rate: Slow, software_input_on: Enable, open_drain: Enable, speed: MHZ_100, drive_strength: R0_6,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_22K_Ohm, hysteresis_enable: Disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_I2C_ConfigurePins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_I2C_ConfigurePins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_02_LPI2C4_SCL,        /* GPIO_SD_B1_02 is configured as LPI2C4_SCL */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_02 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_03_LPI2C4_SDA,        /* GPIO_SD_B1_03 is configured as LPI2C4_SDA */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_03 */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_02_LPI2C4_SCL,        /* GPIO_SD_B1_02 PAD functional properties : */
      0xD8B0U);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Enabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 22K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_03_LPI2C4_SDA,        /* GPIO_SD_B1_03 PAD functional properties : */
      0xD8B0U);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Enabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 22K Ohm Pull Up
                                                 Hyst. Enable Field: Hysteresis Disabled */
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
