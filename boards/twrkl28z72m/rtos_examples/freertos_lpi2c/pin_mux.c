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
processor: MKL28Z512xxx7
package_id: MKL28Z512VLL7
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
  - {pin_num: '38', peripheral: TSI0, signal: 'CH, 5', pin_signal: TSI0_CH5/PTA4/LLWU_P3/LPI2C1_SDA/TPM0_CH1/NMI0_b}
  - {pin_num: '35', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH2/PTA1/LPUART0_RX/TPM2_CH0}
  - {pin_num: '36', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH3/PTA2/LPUART0_TX/TPM2_CH1}
  - {pin_num: '1', peripheral: LPI2C1, signal: SDA, pin_signal: ADC0_SE16/PTE0/RTC_CLKOUT/LPSPI1_SIN/LPUART1_TX/CMP0_OUT/LPI2C1_SDA, slew_rate: fast, open_drain: enable,
    pull_select: up, pull_enable: enable}
  - {pin_num: '2', peripheral: LPI2C1, signal: SCL, pin_signal: ADC0_SE17/PTE1/LLWU_P0/LPSPI1_SOUT/LPUART1_RX/LPI2C1_SCL, slew_rate: fast, open_drain: enable, pull_select: up,
    pull_enable: enable}
  - {pin_num: '42', peripheral: LPI2C2, signal: SCL, pin_signal: PTA12/TPM1_CH0/LPI2C2_SCL/I2S0_TXD0, slew_rate: fast, open_drain: enable, pull_select: up, pull_enable: enable}
  - {pin_num: '43', peripheral: LPI2C2, signal: SDA, pin_signal: PTA13/LLWU_P4/TPM1_CH1/LPI2C2_SDA/I2S0_TX_FS, slew_rate: fast, open_drain: enable, pull_select: up,
    pull_enable: enable}
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
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTA1 (pin 35) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTA, 1U, kPORT_MuxAlt2);

    const port_pin_config_t porta12_pin42_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as LPI2C2_SCL */
                                                    kPORT_MuxAlt5,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTA12 (pin 42) is configured as LPI2C2_SCL */
    PORT_SetPinConfig(PORTA, 12U, &porta12_pin42_config);

    const port_pin_config_t porta13_pin43_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as LPI2C2_SDA */
                                                    kPORT_MuxAlt5,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTA13 (pin 43) is configured as LPI2C2_SDA */
    PORT_SetPinConfig(PORTA, 13U, &porta13_pin43_config);

    /* PORTA2 (pin 36) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt2);

    /* PORTA4 (pin 38) is configured as TSI0_CH5 */
    PORT_SetPinMux(PORTA, 4U, kPORT_PinDisabledOrAnalog);

    const port_pin_config_t porte0_pin1_config = {/* Internal pull-up resistor is enabled */
                                                  kPORT_PullUp,
                                                  /* Fast slew rate is configured */
                                                  kPORT_FastSlewRate,
                                                  /* Passive filter is disabled */
                                                  kPORT_PassiveFilterDisable,
                                                  /* Open drain is enabled */
                                                  kPORT_OpenDrainEnable,
                                                  /* Low drive strength is configured */
                                                  kPORT_LowDriveStrength,
                                                  /* Pin is configured as LPI2C1_SDA */
                                                  kPORT_MuxAlt6,
                                                  /* Pin Control Register fields [15:0] are not locked */
                                                  kPORT_UnlockRegister};
    /* PORTE0 (pin 1) is configured as LPI2C1_SDA */
    PORT_SetPinConfig(PORTE, 0U, &porte0_pin1_config);

    const port_pin_config_t porte1_pin2_config = {/* Internal pull-up resistor is enabled */
                                                  kPORT_PullUp,
                                                  /* Fast slew rate is configured */
                                                  kPORT_FastSlewRate,
                                                  /* Passive filter is disabled */
                                                  kPORT_PassiveFilterDisable,
                                                  /* Open drain is enabled */
                                                  kPORT_OpenDrainEnable,
                                                  /* Low drive strength is configured */
                                                  kPORT_LowDriveStrength,
                                                  /* Pin is configured as LPI2C1_SCL */
                                                  kPORT_MuxAlt6,
                                                  /* Pin Control Register fields [15:0] are not locked */
                                                  kPORT_UnlockRegister};
    /* PORTE1 (pin 2) is configured as LPI2C1_SCL */
    PORT_SetPinConfig(PORTE, 1U, &porte1_pin2_config);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
