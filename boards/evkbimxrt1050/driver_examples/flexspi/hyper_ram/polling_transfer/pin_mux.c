/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
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
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v4.0
processor: MIMXRT1052xxxxx
package_id: MIMXRT1052DVL6B
mcu_data: i_mx_1_0
processor_version: 0.0.0
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
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: L14, peripheral: LPUART1, signal: RX, pin_signal: GPIO_AD_B0_13, software_input_on: Disable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_100, drive_strength: R0_6, slew_rate: Slow}
  - {pin_num: K14, peripheral: LPUART1, signal: TX, pin_signal: GPIO_AD_B0_12, software_input_on: Disable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_100, drive_strength: R0_6, slew_rate: Slow}
  - {pin_num: P3, peripheral: FLEXSPI, signal: FLEXSPI_A_DATA0, pin_signal: GPIO_SD_B1_08, software_input_on: Enable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_6, slew_rate: Fast}
  - {pin_num: N4, peripheral: FLEXSPI, signal: FLEXSPI_A_DATA1, pin_signal: GPIO_SD_B1_09, software_input_on: Enable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_6, slew_rate: Fast}
  - {pin_num: P4, peripheral: FLEXSPI, signal: FLEXSPI_A_DATA2, pin_signal: GPIO_SD_B1_10, software_input_on: Enable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_6, slew_rate: Fast}
  - {pin_num: P5, peripheral: FLEXSPI, signal: FLEXSPI_A_DATA3, pin_signal: GPIO_SD_B1_11, software_input_on: Enable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_6, slew_rate: Fast}
  - {pin_num: N3, peripheral: FLEXSPI, signal: FLEXSPI_A_DQS, pin_signal: GPIO_SD_B1_05, software_input_on: Enable, hysteresis_enable: Enable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Pull, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_6, slew_rate: Fast}
  - {pin_num: L4, peripheral: FLEXSPI, signal: FLEXSPI_A_SCLK, pin_signal: GPIO_SD_B1_07, software_input_on: Enable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_6, slew_rate: Fast}
  - {pin_num: L3, peripheral: FLEXSPI, signal: FLEXSPI_A_SS0_B, pin_signal: GPIO_SD_B1_06, software_input_on: Enable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_6, slew_rate: Fast}
  - {pin_num: M4, peripheral: FLEXSPI, signal: FLEXSPI_B_DATA0, pin_signal: GPIO_SD_B1_03, software_input_on: Enable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_6, slew_rate: Fast}
  - {pin_num: M3, peripheral: FLEXSPI, signal: FLEXSPI_B_DATA1, pin_signal: GPIO_SD_B1_02, software_input_on: Enable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_6, slew_rate: Fast}
  - {pin_num: M5, peripheral: FLEXSPI, signal: FLEXSPI_B_DATA2, pin_signal: GPIO_SD_B1_01, software_input_on: Enable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_6, slew_rate: Fast}
  - {pin_num: L5, peripheral: FLEXSPI, signal: FLEXSPI_B_DATA3, pin_signal: GPIO_SD_B1_00, software_input_on: Enable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_6, slew_rate: Fast}
  - {pin_num: P2, peripheral: FLEXSPI, signal: FLEXSPI_B_SCLK, pin_signal: GPIO_SD_B1_04, software_input_on: Enable, hysteresis_enable: Disable, pull_up_down_config: Pull_Down_100K_Ohm,
    pull_keeper_select: Keeper, pull_keeper_enable: Enable, open_drain: Disable, speed: MHZ_200, drive_strength: R0_6, slew_rate: Fast}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03u */

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 is configured as LPUART1_TX */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 is configured as LPUART1_RX */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_00_FLEXSPIB_DATA03,   /* GPIO_SD_B1_00 is configured as FLEXSPIB_DATA03 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_00 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_01_FLEXSPIB_DATA02,   /* GPIO_SD_B1_01 is configured as FLEXSPIB_DATA02 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_01 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_02_FLEXSPIB_DATA01,   /* GPIO_SD_B1_02 is configured as FLEXSPIB_DATA01 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_02 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_03_FLEXSPIB_DATA00,   /* GPIO_SD_B1_03 is configured as FLEXSPIB_DATA00 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_03 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_04_FLEXSPIB_SCLK,     /* GPIO_SD_B1_04 is configured as FLEXSPIB_SCLK */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_04 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_05_FLEXSPIA_DQS,      /* GPIO_SD_B1_05 is configured as FLEXSPIA_DQS */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_05 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B,    /* GPIO_SD_B1_06 is configured as FLEXSPIA_SS0_B */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_06 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK,     /* GPIO_SD_B1_07 is configured as FLEXSPIA_SCLK */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_07 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00,   /* GPIO_SD_B1_08 is configured as FLEXSPIA_DATA00 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_08 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01,   /* GPIO_SD_B1_09 is configured as FLEXSPIA_DATA01 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_09 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02,   /* GPIO_SD_B1_10 is configured as FLEXSPIA_DATA02 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_10 */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03,   /* GPIO_SD_B1_11 is configured as FLEXSPIA_DATA03 */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_SD_B1_11 */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_12_LPUART1_TX,        /* GPIO_AD_B0_12 PAD functional properties : */
      0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_13_LPUART1_RX,        /* GPIO_AD_B0_13 PAD functional properties : */
      0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_00_FLEXSPIB_DATA03,   /* GPIO_SD_B1_00 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_01_FLEXSPIB_DATA02,   /* GPIO_SD_B1_01 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_02_FLEXSPIB_DATA01,   /* GPIO_SD_B1_02 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_03_FLEXSPIB_DATA00,   /* GPIO_SD_B1_03 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_04_FLEXSPIB_SCLK,     /* GPIO_SD_B1_04 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_05_FLEXSPIA_DQS,      /* GPIO_SD_B1_05 PAD functional properties : */
      0x0130F1u);                             /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Pull
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Enabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B,    /* GPIO_SD_B1_06 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK,     /* GPIO_SD_B1_07 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00,   /* GPIO_SD_B1_08 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01,   /* GPIO_SD_B1_09 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02,   /* GPIO_SD_B1_10 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03,   /* GPIO_SD_B1_11 PAD functional properties : */
      0x10F1u);                               /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
