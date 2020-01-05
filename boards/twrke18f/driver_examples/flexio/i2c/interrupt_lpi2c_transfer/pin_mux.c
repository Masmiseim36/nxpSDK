/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MKE18F512xxx16
package_id: MKE18F512VLL16
mcu_data: ksdk2_0
processor_version: 0.0.8
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"



/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '54', peripheral: LPUART0, signal: RX, pin_signal: ADC0_SE4/PTB0/LPUART0_RX/LPSPI0_PCS0/LPTMR0_ALT3/PWT_IN3}
  - {pin_num: '53', peripheral: LPUART0, signal: TX, pin_signal: ADC0_SE5/PTB1/LPUART0_TX/LPSPI0_SOUT/TCLK0}
  - {pin_num: '4', peripheral: FLEXIO, signal: 'D, 0', pin_signal: ADC2_SE0/PTD0/FTM0_CH2/LPSPI1_SCK/FTM2_CH0/FXIO_D0/TRGMUX_OUT1}
  - {pin_num: '3', peripheral: FLEXIO, signal: 'D, 1', pin_signal: ADC2_SE1/PTD1/FTM0_CH3/LPSPI1_SIN/FTM2_CH1/FXIO_D1/TRGMUX_OUT2}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void)
{
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTB0 (pin 54) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt2);

    /* PORTB1 (pin 53) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt2);

    /* PORTD0 (pin 4) is configured as FXIO_D0 */
    PORT_SetPinMux(PORTD, 0U, kPORT_MuxAlt6);

    /* PORTD1 (pin 3) is configured as FXIO_D1 */
    PORT_SetPinMux(PORTD, 1U, kPORT_MuxAlt6);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_I2C_ConfigurePins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '72', peripheral: LPI2C0, signal: SCL, pin_signal: ADC1_SE1/PTA3/FTM3_CH1/LPI2C0_SCL/EWM_IN/LPUART0_TX, drive_strength: low, pull_select: up, pull_enable: enable,
    passive_filter: disable}
  - {pin_num: '73', peripheral: LPI2C0, signal: SDA, pin_signal: ADC1_SE0/PTA2/FTM3_CH0/LPI2C0_SDA/EWM_OUT_b/LPUART0_RX, drive_strength: low, pull_select: up, pull_enable: enable,
    passive_filter: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_I2C_ConfigurePins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_I2C_ConfigurePins(void)
{
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortA);

    const port_pin_config_t porta2_pin73_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as LPI2C0_SDA */
                                                   kPORT_MuxAlt3,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTA2 (pin 73) is configured as LPI2C0_SDA */
    PORT_SetPinConfig(PORTA, 2U, &porta2_pin73_config);

    const port_pin_config_t porta3_pin72_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as LPI2C0_SCL */
                                                   kPORT_MuxAlt3,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTA3 (pin 72) is configured as LPI2C0_SCL */
    PORT_SetPinConfig(PORTA, 3U, &porta3_pin72_config);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
