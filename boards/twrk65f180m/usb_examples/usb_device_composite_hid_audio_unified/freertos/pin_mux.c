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
processor: MK65FN2M0xxx18
package_id: MK65FN2M0VMI18
mcu_data: ksdk2_0
processor_version: 0.0.10
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
  - {pin_num: F5, peripheral: UART2, signal: RX, pin_signal: ADC0_SE5a/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3/TPM_CLKIN1}
  - {pin_num: H3, peripheral: UART2, signal: TX, pin_signal: ADC0_SE4a/PTE16/SPI0_PCS0/UART2_TX/FTM_CLKIN0/FTM0_FLT3/TPM_CLKIN0}
  - {pin_num: F4, peripheral: I2S0, signal: TXD0, pin_signal: PTE10/LLWU_P18/I2C3_SDA/I2S0_TXD0/LPUART0_CTS_b/FTM3_CH5/USB1_ID}
  - {pin_num: G4, peripheral: I2S0, signal: TX_FS, pin_signal: PTE11/I2C3_SCL/I2S0_TX_FS/LPUART0_RTS_b/FTM3_CH6}
  - {pin_num: H4, peripheral: I2S0, signal: TX_BCLK, pin_signal: PTE12/I2S0_TX_BCLK/FTM3_CH7}
  - {pin_num: E2, peripheral: I2S0, signal: MCLK, pin_signal: PTE6/LLWU_P16/SPI1_PCS3/UART3_CTS_b/I2S0_MCLK/FTM3_CH1/USB0_SOF_OUT}
  - {pin_num: E3, peripheral: I2S0, signal: RXD0, pin_signal: PTE7/UART3_RTS_b/I2S0_RXD0/FTM3_CH2}
  - {pin_num: N11, peripheral: GPIOA, signal: 'GPIO, 10', pin_signal: PTA10/LLWU_P22/FTM2_CH0/MII0_RXD2/FTM2_QD_PHA/TPM2_CH0/TRACE_D0, slew_rate: fast, open_drain: disable,
    pull_select: up, pull_enable: enable}
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
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t porta10_pinN11_config = {/* Internal pull-up resistor is enabled */
                                                     kPORT_PullUp,
                                                     /* Fast slew rate is configured */
                                                     kPORT_FastSlewRate,
                                                     /* Passive filter is disabled */
                                                     kPORT_PassiveFilterDisable,
                                                     /* Open drain is disabled */
                                                     kPORT_OpenDrainDisable,
                                                     /* Low drive strength is configured */
                                                     kPORT_LowDriveStrength,
                                                     /* Pin is configured as PTA10 */
                                                     kPORT_MuxAsGpio,
                                                     /* Pin Control Register fields [15:0] are not locked */
                                                     kPORT_UnlockRegister};
    /* PORTA10 (pin N11) is configured as PTA10 */
    PORT_SetPinConfig(PORTA, 10U, &porta10_pinN11_config);

    /* PORTE10 (pin F4) is configured as I2S0_TXD0 */
    PORT_SetPinMux(PORTE, 10U, kPORT_MuxAlt4);

    /* PORTE11 (pin G4) is configured as I2S0_TX_FS */
    PORT_SetPinMux(PORTE, 11U, kPORT_MuxAlt4);

    /* PORTE12 (pin H4) is configured as I2S0_TX_BCLK */
    PORT_SetPinMux(PORTE, 12U, kPORT_MuxAlt4);

    /* PORTE16 (pin H3) is configured as UART2_TX */
    PORT_SetPinMux(PORTE, 16U, kPORT_MuxAlt3);

    /* PORTE17 (pin F5) is configured as UART2_RX */
    PORT_SetPinMux(PORTE, 17U, kPORT_MuxAlt3);

    /* PORTE6 (pin E2) is configured as I2S0_MCLK */
    PORT_SetPinMux(PORTE, 6U, kPORT_MuxAlt4);

    /* PORTE7 (pin E3) is configured as I2S0_RXD0 */
    PORT_SetPinMux(PORTE, 7U, kPORT_MuxAlt4);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_I2C_ConfigurePins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: F6, peripheral: I2C0, signal: SDA, pin_signal: ADC0_SE6a/PTE18/LLWU_P20/SPI0_SOUT/UART2_CTS_b/I2C0_SDA, slew_rate: fast, open_drain: enable, pull_select: up,
    pull_enable: enable}
  - {pin_num: F7, peripheral: I2C0, signal: SCL, pin_signal: ADC0_SE7a/PTE19/SPI0_SIN/UART2_RTS_b/I2C0_SCL/CMP3_OUT, slew_rate: fast, open_drain: enable, pull_select: up,
    pull_enable: enable}
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
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t porte18_pinF6_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C0_SDA */
                                                    kPORT_MuxAlt4,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTE18 (pin F6) is configured as I2C0_SDA */
    PORT_SetPinConfig(PORTE, 18U, &porte18_pinF6_config);

    const port_pin_config_t porte19_pinF7_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C0_SCL */
                                                    kPORT_MuxAlt4,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTE19 (pin F7) is configured as I2C0_SCL */
    PORT_SetPinConfig(PORTE, 19U, &porte19_pinF7_config);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
