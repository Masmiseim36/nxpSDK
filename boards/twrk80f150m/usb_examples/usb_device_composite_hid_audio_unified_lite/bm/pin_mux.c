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
processor: MK80FN256xxx15
package_id: MK80FN256VDC15
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
  - {pin_num: B8, peripheral: LPUART1, signal: RX, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: A8, peripheral: LPUART1, signal: TX, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT}
  - {pin_num: K7, peripheral: I2S0, signal: TX_BCLK, pin_signal: PTA5/USB0_CLKIN/FTM0_CH2/FXIO0_D15/I2S0_TX_BCLK/JTAG_TRST_b}
  - {pin_num: K8, peripheral: I2S0, signal: TXD0, pin_signal: PTA12/FTM1_CH0/TRACE_CLKOUT/FXIO0_D18/I2S0_TXD0/FTM1_QD_PHA/TPM1_CH0}
  - {pin_num: L8, peripheral: I2S0, signal: TX_FS, pin_signal: PTA13/LLWU_P4/FTM1_CH1/TRACE_D3/FXIO0_D19/I2S0_TX_FS/FTM1_QD_PHB/TPM1_CH1}
  - {pin_num: H10, peripheral: I2S0, signal: MCLK, pin_signal: PTA17/SPI0_SIN/LPUART0_RTS_b/FXIO0_D23/I2S0_MCLK}
  - {pin_num: L9, peripheral: I2S0, signal: RXD0, pin_signal: PTA15/SPI0_SCK/LPUART0_RX/TRACE_D1/FXIO0_D21/I2S0_RXD0}
  - {pin_num: J5, peripheral: GPIOA, signal: 'GPIO, 21', pin_signal: PTA21/LLWU_P21/I2C0_SDA/LPUART4_RX/FXIO0_D9/EWM_IN, slew_rate: fast, open_drain: disable, pull_select: up,
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
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    const port_pin_config_t porta21_pinJ5_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is disabled */
                                                    kPORT_OpenDrainDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as PTA21 */
                                                    kPORT_MuxAsGpio,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTA21 (pin J5) is configured as PTA21 */
    PORT_SetPinConfig(PORTA, 21U, &porta21_pinJ5_config);

    /* PORTA12 (pin K8) is configured as I2S0_TXD0 */
    PORT_SetPinMux(PORTA, 12U, kPORT_MuxAlt6);

    /* PORTA13 (pin L8) is configured as I2S0_TX_FS */
    PORT_SetPinMux(PORTA, 13U, kPORT_MuxAlt6);

    /* PORTA15 (pin L9) is configured as I2S0_RXD0 */
    PORT_SetPinMux(PORTA, 15U, kPORT_MuxAlt6);

    /* PORTA17 (pin H10) is configured as I2S0_MCLK */
    PORT_SetPinMux(PORTA, 17U, kPORT_MuxAlt6);

    /* PORTA5 (pin K7) is configured as I2S0_TX_BCLK */
    PORT_SetPinMux(PORTA, 5U, kPORT_MuxAlt6);

    /* PORTC3 (pin B8) is configured as LPUART1_RX */
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt3);

    /* PORTC4 (pin A8) is configured as LPUART1_TX */
    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAlt3);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART1TXSRC_MASK | SIM_SOPT5_LPUART1RXSRC_MASK)))

                  /* LPUART1 transmit data source select: LPUART1_TX pin. */
                  | SIM_SOPT5_LPUART1TXSRC(SOPT5_LPUART1TXSRC_LPUART_TX)

                  /* LPUART1 receive data source select: LPUART1_RX pin. */
                  | SIM_SOPT5_LPUART1RXSRC(SOPT5_LPUART1RXSRC_LPUART_RX));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_I2C_ConfigurePins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: A10, peripheral: I2C0, signal: SCL, pin_signal: PTD8/LLWU_P24/I2C0_SCL/FB_A16/FXIO0_D24, slew_rate: fast, open_drain: enable, pull_select: up, pull_enable: enable}
  - {pin_num: A9, peripheral: I2C0, signal: SDA, pin_signal: PTD9/I2C0_SDA/FB_A17/FXIO0_D25, slew_rate: fast, open_drain: enable, pull_select: up, pull_enable: enable}
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
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    const port_pin_config_t portd8_pinA10_config = {/* Internal pull-up resistor is enabled */
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
                                                    kPORT_MuxAlt2,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTD8 (pin A10) is configured as I2C0_SCL */
    PORT_SetPinConfig(PORTD, 8U, &portd8_pinA10_config);

    const port_pin_config_t portd9_pinA9_config = {/* Internal pull-up resistor is enabled */
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
                                                   kPORT_MuxAlt2,
                                                   /* Pin Control Register fields [15:0] are not locked */
                                                   kPORT_UnlockRegister};
    /* PORTD9 (pin A9) is configured as I2C0_SDA */
    PORT_SetPinConfig(PORTD, 9U, &portd9_pinA9_config);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
