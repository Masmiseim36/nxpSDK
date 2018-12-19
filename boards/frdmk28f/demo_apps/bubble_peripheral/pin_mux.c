/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v4.1
processor: MK28FN2M0xxx15
package_id: MK28FN2M0VMI15
mcu_data: ksdk2_0
processor_version: 4.0.0
board: FRDM-K28F
pin_labels:
- {pin_num: C6, pin_signal: PTC28/I2C3_SDA/FB_A1/SDRAM_D1/QSPI0A_DATA1, label: 'U6[6]/I2C SDA', identifier: ACCEL_I2C_SDA}
- {pin_num: B6, pin_signal: PTC29/I2C3_SCL/FB_A0/SDRAM_D0/QSPI0A_SS0_B, label: 'U6[4]/I2C_SCL', identifier: ACCEL_I2C_SCL}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: A7, peripheral: LPUART0, signal: RX, pin_signal: PTC25/LPUART0_RX/FB_A4/SDRAM_D4/QSPI0A_SCLK}
  - {pin_num: B7, peripheral: LPUART0, signal: TX, pin_signal: PTC24/LPUART0_TX/FB_A5/SDRAM_D5/QSPI0A_DATA3}
  - {pin_num: E2, peripheral: FTM3, signal: 'CH, 1', pin_signal: PTE6/LLWU_P16/FXIO0_D12/LPUART3_CTS_b/I2S0_MCLK/QSPI0B_DATA3/FTM3_CH1/SDHC0_D4, direction: OUTPUT}
  - {pin_num: E3, peripheral: FTM3, signal: 'CH, 2', pin_signal: PTE7/FXIO0_D13/LPUART3_RTS_b/I2S0_RXD0/QSPI0B_SCLK/FTM3_CH2/QSPI0A_SS1_B, direction: OUTPUT}
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
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTC24 (pin B7) is configured as LPUART0_TX */
    PORT_SetPinMux(BOARD_DEBUG_UART_TX_PORT, BOARD_DEBUG_UART_TX_PIN, kPORT_MuxAlt3);

    /* PORTC25 (pin A7) is configured as LPUART0_RX */
    PORT_SetPinMux(BOARD_DEBUG_UART_RX_PORT, BOARD_DEBUG_UART_RX_PIN, kPORT_MuxAlt3);

    /* PORTE6 (pin E2) is configured as FTM3_CH1 */
    PORT_SetPinMux(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, kPORT_MuxAlt6);

    /* PORTE7 (pin E3) is configured as FTM3_CH2 */
    PORT_SetPinMux(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, kPORT_MuxAlt6);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK)))

                  /* LPUART0 transmit data source select: LPUART0_TX pin. */
                  | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX)

                  /* LPUART0 receive data source select: LPUART0_RX pin. */
                  | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX));

    SIM->SOPT8 = ((SIM->SOPT8 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT8_FTM3OCH1SRC_MASK | SIM_SOPT8_FTM3OCH2SRC_MASK)))

                  /* FTM3 channel 1 output source: FTM3_CH1 pin is output of FTM3 channel 1 output. */
                  | SIM_SOPT8_FTM3OCH1SRC(SOPT8_FTM3OCH1SRC_FTM)

                  /* FTM3 channel 2 output source: FTM3_CH2 pin is output of FTM3 channel 2 output. */
                  | SIM_SOPT8_FTM3OCH2SRC(SOPT8_FTM3OCH2SRC_FTM));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_I2C_ConfigurePins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: B6, peripheral: I2C3, signal: SCL, pin_signal: PTC29/I2C3_SCL/FB_A0/SDRAM_D0/QSPI0A_SS0_B, identifier: '', slew_rate: fast, open_drain: enable, pull_select: up,
    pull_enable: enable}
  - {pin_num: C6, peripheral: I2C3, signal: SDA, pin_signal: PTC28/I2C3_SDA/FB_A1/SDRAM_D1/QSPI0A_DATA1, identifier: '', slew_rate: fast, open_drain: enable, pull_select: up,
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
BOARD_Init_I2C_GPIO_pins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: C6, peripheral: GPIOC, signal: 'GPIO, 28', pin_signal: PTC28/I2C3_SDA/FB_A1/SDRAM_D1/QSPI0A_DATA1, direction: OUTPUT, gpio_init_state: 'true', pull_select: up}
  - {pin_num: B6, peripheral: GPIOC, signal: 'GPIO, 29', pin_signal: PTC29/I2C3_SCL/FB_A0/SDRAM_D0/QSPI0A_SS0_B, direction: OUTPUT, gpio_init_state: 'true', pull_select: up}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_Init_I2C_GPIO_pins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_Init_I2C_GPIO_pins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    gpio_pin_config_t ACCEL_I2C_SDA_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTC28 (pin C6)  */
    GPIO_PinInit(BOARD_ACCEL_I2C_SDA_GPIO, BOARD_ACCEL_I2C_SDA_PIN, &ACCEL_I2C_SDA_config);

    gpio_pin_config_t ACCEL_I2C_SCL_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTC29 (pin B6)  */
    GPIO_PinInit(BOARD_ACCEL_I2C_SCL_GPIO, BOARD_ACCEL_I2C_SCL_PIN, &ACCEL_I2C_SCL_config);

    /* PORTC28 (pin C6) is configured as PTC28 */
    PORT_SetPinMux(BOARD_ACCEL_I2C_SDA_PORT, BOARD_ACCEL_I2C_SDA_PIN, kPORT_MuxAsGpio);

    PORTC->PCR[28] = ((PORTC->PCR[28] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK)))

                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | PORT_PCR_PS(kPORT_PullUp));

    /* PORTC29 (pin B6) is configured as PTC29 */
    PORT_SetPinMux(BOARD_ACCEL_I2C_SCL_PORT, BOARD_ACCEL_I2C_SCL_PIN, kPORT_MuxAsGpio);

    PORTC->PCR[29] = ((PORTC->PCR[29] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK)))

                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | PORT_PCR_PS(kPORT_PullUp));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
