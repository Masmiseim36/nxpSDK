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
processor: MK28FN2M0Axxx15
package_id: MK28FN2M0AVMI15
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
  - {pin_num: B7, peripheral: LPUART0, signal: TX, pin_signal: PTC24/LPUART0_TX/FB_A5/SDRAM_D5/QSPI0A_DATA3}
  - {pin_num: A7, peripheral: LPUART0, signal: RX, pin_signal: PTC25/LPUART0_RX/FB_A4/SDRAM_D4/QSPI0A_SCLK}
  - {pin_num: E2, peripheral: GPIOE, signal: 'GPIO, 6', pin_signal: PTE6/LLWU_P16/FXIO0_D12/LPUART3_CTS_b/I2S0_MCLK/QSPI0B_DATA3/FTM3_CH1/SDHC0_D4}
  - {pin_num: E3, peripheral: GPIOE, signal: 'GPIO, 7', pin_signal: PTE7/FXIO0_D13/LPUART3_RTS_b/I2S0_RXD0/QSPI0B_SCLK/FTM3_CH2/QSPI0A_SS1_B}
  - {pin_num: E4, peripheral: GPIOE, signal: 'GPIO, 8', pin_signal: PTE8/I2S0_RXD1/FXIO0_D14/I2S0_RX_FS/QSPI0B_DATA0/FTM3_CH3/SDHC0_D5}
  - {pin_num: L9, peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: TSI0_CH5/PTA4/LLWU_P3/FTM0_CH1/FXIO0_D14/NMI_b}
  - {pin_num: A6, peripheral: GPIOD, signal: 'GPIO, 0', pin_signal: PTD0/LLWU_P12/SPI0_PCS0/LPUART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b/FXIO0_D22}
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
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTA4 (pin L9) is configured as PTA4 */
    PORT_SetPinMux(PORTA, 4U, kPORT_MuxAsGpio);

    /* PORTC24 (pin B7) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTC, 24U, kPORT_MuxAlt3);

    /* PORTC25 (pin A7) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTC, 25U, kPORT_MuxAlt3);

    /* PORTD0 (pin A6) is configured as PTD0 */
    PORT_SetPinMux(PORTD, 0U, kPORT_MuxAsGpio);

    /* PORTE6 (pin E2) is configured as PTE6 */
    PORT_SetPinMux(PORTE, 6U, kPORT_MuxAsGpio);

    /* PORTE7 (pin E3) is configured as PTE7 */
    PORT_SetPinMux(PORTE, 7U, kPORT_MuxAsGpio);

    /* PORTE8 (pin E4) is configured as PTE8 */
    PORT_SetPinMux(PORTE, 8U, kPORT_MuxAsGpio);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK)))

                  /* LPUART0 transmit data source select: LPUART0_TX pin. */
                  | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX)

                  /* LPUART0 receive data source select: LPUART0_RX pin. */
                  | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_I2C_ConfigurePins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: C6, peripheral: I2C3, signal: SDA, pin_signal: PTC28/I2C3_SDA/FB_A1/SDRAM_D1/QSPI0A_DATA1, slew_rate: fast, open_drain: enable, pull_select: up, pull_enable: enable}
  - {pin_num: B6, peripheral: I2C3, signal: SCL, pin_signal: PTC29/I2C3_SCL/FB_A0/SDRAM_D0/QSPI0A_SS0_B, slew_rate: fast, open_drain: enable, pull_select: up, pull_enable: enable}
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
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    const port_pin_config_t portc28_pinC6_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C3_SDA */
                                                    kPORT_MuxAlt2,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTC28 (pin C6) is configured as I2C3_SDA */
    PORT_SetPinConfig(PORTC, 28U, &portc28_pinC6_config);

    const port_pin_config_t portc29_pinB6_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C3_SCL */
                                                    kPORT_MuxAlt2,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};
    /* PORTC29 (pin B6) is configured as I2C3_SCL */
    PORT_SetPinConfig(PORTC, 29U, &portc29_pinB6_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART0_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: A7, peripheral: LPUART0, signal: RX, pin_signal: PTC25/LPUART0_RX/FB_A4/SDRAM_D4/QSPI0A_SCLK}
  - {pin_num: B7, peripheral: LPUART0, signal: TX, pin_signal: PTC24/LPUART0_TX/FB_A5/SDRAM_D5/QSPI0A_DATA3}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPUART0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPUART0_InitPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC24 (pin B7) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTC, 24U, kPORT_MuxAlt3);

    /* PORTC25 (pin A7) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTC, 25U, kPORT_MuxAlt3);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK)))

                  /* LPUART0 transmit data source select: LPUART0_TX pin. */
                  | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX)

                  /* LPUART0 receive data source select: LPUART0_RX pin. */
                  | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART0_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: A7, peripheral: n/a, signal: disabled, pin_signal: PTC25/LPUART0_RX/FB_A4/SDRAM_D4/QSPI0A_SCLK}
  - {pin_num: B7, peripheral: n/a, signal: disabled, pin_signal: PTC24/LPUART0_TX/FB_A5/SDRAM_D5/QSPI0A_DATA3}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPUART0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPUART0_DeinitPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC24 (pin B7) is disabled */
    PORT_SetPinMux(PORTC, 24U, kPORT_PinDisabledOrAnalog);

    /* PORTC25 (pin A7) is disabled */
    PORT_SetPinMux(PORTC, 25U, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART1_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: A12, peripheral: LPUART1, signal: RX, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: B11, peripheral: LPUART1, signal: TX, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPUART1_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPUART1_InitPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC3 (pin A12) is configured as LPUART1_RX */
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt3);

    /* PORTC4 (pin B11) is configured as LPUART1_TX */
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
LPUART1_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: B11, peripheral: n/a, signal: disabled, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT}
  - {pin_num: A12, peripheral: CMP1, signal: 'IN, 1', pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPUART1_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPUART1_DeinitPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC3 (pin A12) is configured as CMP1_IN1 */
    PORT_SetPinMux(PORTC, 3U, kPORT_PinDisabledOrAnalog);

    /* PORTC4 (pin B11) is disabled */
    PORT_SetPinMux(PORTC, 4U, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART2_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: A4, peripheral: LPUART2, signal: RX, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/LPUART2_RX/FTM3_CH2/FB_AD4/SDRAM_A12/I2C0_SCL}
  - {pin_num: B4, peripheral: LPUART2, signal: TX, pin_signal: PTD3/SPI0_SIN/LPUART2_TX/FTM3_CH3/FB_AD3/SDRAM_A11/I2C0_SDA}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPUART2_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPUART2_InitPins(void)
{
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTD2 (pin A4) is configured as LPUART2_RX */
    PORT_SetPinMux(PORTD, 2U, kPORT_MuxAlt3);

    /* PORTD3 (pin B4) is configured as LPUART2_TX */
    PORT_SetPinMux(PORTD, 3U, kPORT_MuxAlt3);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART2_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: B4, peripheral: n/a, signal: disabled, pin_signal: PTD3/SPI0_SIN/LPUART2_TX/FTM3_CH3/FB_AD3/SDRAM_A11/I2C0_SDA}
  - {pin_num: A4, peripheral: n/a, signal: disabled, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/LPUART2_RX/FTM3_CH2/FB_AD4/SDRAM_A12/I2C0_SCL}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPUART2_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPUART2_DeinitPins(void)
{
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTD2 (pin A4) is disabled */
    PORT_SetPinMux(PORTD, 2U, kPORT_PinDisabledOrAnalog);

    /* PORTD3 (pin B4) is disabled */
    PORT_SetPinMux(PORTD, 3U, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART3_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: G9, peripheral: LPUART3, signal: RX, pin_signal: PTB10/SPI1_PCS0/LPUART3_RX/I2C2_SCL/FB_AD19/SDRAM_D19/FTM0_FLT1/FXIO0_D4}
  - {pin_num: E13, peripheral: LPUART3, signal: TX, pin_signal: PTB11/SPI1_SCK/LPUART3_TX/I2C2_SDA/FB_AD18/SDRAM_D18/FTM0_FLT2/FXIO0_D5}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPUART3_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPUART3_InitPins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    /* PORTB10 (pin G9) is configured as LPUART3_RX */
    PORT_SetPinMux(PORTB, 10U, kPORT_MuxAlt3);

    /* PORTB11 (pin E13) is configured as LPUART3_TX */
    PORT_SetPinMux(PORTB, 11U, kPORT_MuxAlt3);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART3_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: E13, peripheral: n/a, signal: disabled, pin_signal: PTB11/SPI1_SCK/LPUART3_TX/I2C2_SDA/FB_AD18/SDRAM_D18/FTM0_FLT2/FXIO0_D5}
  - {pin_num: G9, peripheral: n/a, signal: disabled, pin_signal: PTB10/SPI1_PCS0/LPUART3_RX/I2C2_SCL/FB_AD19/SDRAM_D19/FTM0_FLT1/FXIO0_D4}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPUART3_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPUART3_DeinitPins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    /* PORTB10 (pin G9) is disabled */
    PORT_SetPinMux(PORTB, 10U, kPORT_PinDisabledOrAnalog);

    /* PORTB11 (pin E13) is disabled */
    PORT_SetPinMux(PORTB, 11U, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART4_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: C8, peripheral: LPUART4, signal: RX, pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20}
  - {pin_num: D8, peripheral: LPUART4, signal: TX, pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPUART4_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPUART4_InitPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC14 (pin C8) is configured as LPUART4_RX */
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt3);

    /* PORTC15 (pin D8) is configured as LPUART4_TX */
    PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt3);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART4_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: D8, peripheral: n/a, signal: disabled, pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21}
  - {pin_num: C8, peripheral: n/a, signal: disabled, pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPUART4_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPUART4_DeinitPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC14 (pin C8) is disabled */
    PORT_SetPinMux(PORTC, 14U, kPORT_PinDisabledOrAnalog);

    /* PORTC15 (pin D8) is disabled */
    PORT_SetPinMux(PORTC, 15U, kPORT_PinDisabledOrAnalog);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
