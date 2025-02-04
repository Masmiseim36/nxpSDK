/*
 * Copyright 2024 NXP
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
product: Pins v15.0
processor: MIMXRT1189xxxxx
package_id: MIMXRT1189CVM8B
mcu_data: ksdk2_0
processor_version: 0.15.9
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_rgpio.h"
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
  - {pin_num: R14, peripheral: RGPIO4, signal: 'gpio_io, 01', pin_signal: GPIO_AD_01, direction: OUTPUT}
  - {pin_num: N12, peripheral: FLEXIO2, signal: 'IO, 00', pin_signal: GPIO_AD_00}
  - {pin_num: A5, peripheral: LPUART1, signal: RXD, pin_signal: GPIO_AON_09}
  - {pin_num: B1, peripheral: LPUART1, signal: TXD, pin_signal: GPIO_AON_08}
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

  /* GPIO configuration on GPIO_AD_01 (pin R14) */
  rgpio_pin_config_t gpio4_pinR14_config = {
      .pinDirection = kRGPIO_DigitalOutput,
      .outputLogic = 0U,
  };
  /* Initialize GPIO functionality on GPIO_AD_01 (pin R14) */
  RGPIO_PinInit(RGPIO4, 1U, &gpio4_pinR14_config);

  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_00_FLEXIO2_FLEXIO00,     /* GPIO_AD_00 is configured as FLEXIO2_FLEXIO00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_01_GPIO4_IO01,           /* GPIO_AD_01 is configured as GPIO4_IO01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_08_LPUART1_TX,          /* GPIO_AON_08 is configured as LPUART1_TX */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AON_09_LPUART1_RX,          /* GPIO_AON_09 is configured as LPUART1_RX */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
