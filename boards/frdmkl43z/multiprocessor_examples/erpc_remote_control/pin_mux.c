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
processor: MKL43Z256xxx4
package_id: MKL43Z256VLH4
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
  - {pin_num: '23', peripheral: LPUART0, signal: RX, pin_signal: PTA1/LPUART0_RX/TPM2_CH0}
  - {pin_num: '24', peripheral: LPUART0, signal: TX, pin_signal: PTA2/LPUART0_TX/TPM2_CH1}
  - {pin_num: '19', peripheral: GPIOE, signal: 'GPIO, 31', pin_signal: PTE31/TPM0_CH4}
  - {pin_num: '62', peripheral: GPIOD, signal: 'GPIO, 5', pin_signal: LCD_P45/ADC0_SE6b/PTD5/SPI1_SCK/UART2_TX/TPM0_CH5/FXIO0_D5}
  - {pin_num: '26', peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: PTA4/I2C1_SDA/TPM0_CH1/NMI_b}
  - {pin_num: '46', peripheral: GPIOC, signal: 'GPIO, 3', pin_signal: LCD_P23/PTC3/LLWU_P7/SPI1_SCK/LPUART1_RX/TPM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: '20', peripheral: I2C0, signal: SCL, pin_signal: PTE24/TPM0_CH0/I2C0_SCL}
  - {pin_num: '21', peripheral: I2C0, signal: SDA, pin_signal: PTE25/TPM0_CH1/I2C0_SDA}
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

    /* PORTA1 (pin 23) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTA, 1U, kPORT_MuxAlt2);

    /* PORTA2 (pin 24) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt2);

    /* PORTA4 (pin 26) is configured as PTA4 */
    PORT_SetPinMux(PORTA, 4U, kPORT_MuxAsGpio);

    /* PORTC3 (pin 46) is configured as PTC3 */
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAsGpio);

    /* PORTD5 (pin 62) is configured as PTD5 */
    PORT_SetPinMux(PORTD, 5U, kPORT_MuxAsGpio);

    /* PORTE24 (pin 20) is configured as I2C0_SCL */
    PORT_SetPinMux(PORTE, 24U, kPORT_MuxAlt5);

    /* PORTE25 (pin 21) is configured as I2C0_SDA */
    PORT_SetPinMux(PORTE, 25U, kPORT_MuxAlt5);

    /* PORTE31 (pin 19) is configured as PTE31 */
    PORT_SetPinMux(PORTE, 31U, kPORT_MuxAsGpio);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK)))

                  /* LPUART0 Transmit Data Source Select: LPUART0_TX pin. */
                  | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX)

                  /* LPUART0 Receive Data Source Select: LPUART_RX pin. */
                  | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_I2C_ConfigurePins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '21', peripheral: I2C0, signal: SDA, pin_signal: PTE25/TPM0_CH1/I2C0_SDA, slew_rate: slow, pull_select: up, pull_enable: enable}
  - {pin_num: '20', peripheral: I2C0, signal: SCL, pin_signal: PTE24/TPM0_CH0/I2C0_SCL, slew_rate: slow, pull_select: up, pull_enable: enable}
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

    const port_pin_config_t porte24_pin20_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Slow slew rate is configured */
                                                    kPORT_SlowSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C0_SCL */
                                                    kPORT_MuxAlt5};
    /* PORTE24 (pin 20) is configured as I2C0_SCL */
    PORT_SetPinConfig(PORTE, 24U, &porte24_pin20_config);

    const port_pin_config_t porte25_pin21_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Slow slew rate is configured */
                                                    kPORT_SlowSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C0_SDA */
                                                    kPORT_MuxAlt5};
    /* PORTE25 (pin 21) is configured as I2C0_SDA */
    PORT_SetPinConfig(PORTE, 25U, &porte25_pin21_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART0_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '23', peripheral: LPUART0, signal: RX, pin_signal: PTA1/LPUART0_RX/TPM2_CH0}
  - {pin_num: '24', peripheral: LPUART0, signal: TX, pin_signal: PTA2/LPUART0_TX/TPM2_CH1}
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
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);

    /* PORTA1 (pin 23) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTA, 1U, kPORT_MuxAlt2);

    /* PORTA2 (pin 24) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt2);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK)))

                  /* LPUART0 Transmit Data Source Select: LPUART0_TX pin. */
                  | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_LPUART_TX)

                  /* LPUART0 Receive Data Source Select: LPUART_RX pin. */
                  | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_LPUART_RX));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART0_DeinitPins:
- options: {coreID: core0, enableClock: 'false'}
- pin_list:
  - {pin_num: '24', peripheral: n/a, signal: disabled, pin_signal: PTA2/LPUART0_TX/TPM2_CH1}
  - {pin_num: '23', peripheral: n/a, signal: disabled, pin_signal: PTA1/LPUART0_RX/TPM2_CH0}
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

    /* PORTA1 (pin 23) is disabled */
    PORT_SetPinMux(PORTA, 1U, kPORT_PinDisabledOrAnalog);

    /* PORTA2 (pin 24) is disabled */
    PORT_SetPinMux(PORTA, 2U, kPORT_PinDisabledOrAnalog);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
