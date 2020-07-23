/*
 * Copyright 2019-2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v5.0
processor: MKW38A512xxx4
package_id: MKW38A512VFT4
mcu_data: ksdk2_0
processor_version: 0.0.0
pin_labels:
- {pin_num: '37', pin_signal: PTC1/I2C0_SDA/LPUART0_RTS_b/TPM0_CH2/SPI1_SCK, label: LED_R, identifier: LED_R}
- {pin_num: '4', pin_signal: PTA16/LLWU_P4/SPI1_SOUT/LPUART1_RTS_b/TPM0_CH0, label: LED_G, identifier: LED_G}
- {pin_num: '18', pin_signal: ADC0_SE3/CMP0_IN3/PTB2/LLWU_P9/TPM0_CH0/TPM1_CH0/TPM2_CH0, label: LED_B, identifier: LED_B}
- {pin_num: '19', pin_signal: ADC0_SE2/CMP0_IN4/PTB3/ERCLK32K/LPUART1_RTS_b/TPM0_CH1/CLKOUT/TPM1_CH1/RTC_CLKOUT/TPM2_CH1, label: LED, identifier: LED}
- {pin_num: '23', pin_signal: ADC0_SE4/CMP0_IN2/PTB18/LPUART1_CTS_b/I2C1_SCL/TPM_CLKIN0/TPM0_CH0/NMI_b, label: SW2, identifier: SW2}
- {pin_num: '38', pin_signal: PTC2/LLWU_P10/I2C1_SCL/LPUART0_RX/CMT_IRO/SPI1_SOUT, label: SW3, identifier: SW3}
- {pin_num: '42', pin_signal: PTC6/LLWU_P14/I2C1_SCL/LPUART0_RX/TPM2_CH0, label: LPUART0_RX, identifier: LPUART0_RX}
- {pin_num: '43', pin_signal: PTC7/LLWU_P15/SPI0_PCS2/I2C1_SDA/LPUART0_TX/TPM2_CH1, label: LPUART0_TX, identifier: LPUART0_TX}
- {pin_num: '6', pin_signal: PTA18/LLWU_P6/SPI1_SCK/LPUART1_TX/CAN0_RX/TPM2_CH0, label: LPUART1_TX, identifier: LPUART1_TX}
- {pin_num: '5', pin_signal: PTA17/LLWU_P5/SPI1_SIN/LPUART1_RX/CAN0_TX/TPM_CLKIN1, label: LPUART1_RX, identifier: LPUART1_RX}
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
    BOARD_InitLPUART();
    BOARD_InitLEDs();
    BOARD_InitButtons();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLPUART:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '42', peripheral: LPUART0, signal: RX, pin_signal: PTC6/LLWU_P14/I2C1_SCL/LPUART0_RX/TPM2_CH0}
  - {pin_num: '43', peripheral: LPUART0, signal: TX, pin_signal: PTC7/LLWU_P15/SPI0_PCS2/I2C1_SDA/LPUART0_TX/TPM2_CH1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitLPUART
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitLPUART(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC6 (pin 42) is configured as LPUART0_RX */
    PORT_SetPinMux(BOARD_INITLPUART_LPUART0_RX_PORT, BOARD_INITLPUART_LPUART0_RX_PIN, kPORT_MuxAlt4);

    /* PORTC7 (pin 43) is configured as LPUART0_TX */
    PORT_SetPinMux(BOARD_INITLPUART_LPUART0_TX_PORT, BOARD_INITLPUART_LPUART0_TX_PIN, kPORT_MuxAlt4);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART0TXSRC_MASK | SIM_SOPT5_LPUART0RXSRC_MASK)))

                  /* LPUART0 Transmit Data Source Select: LPUART0_TX pin. */
                  | SIM_SOPT5_LPUART0TXSRC(SOPT5_LPUART0TXSRC_0b00)

                  /* LPUART0 Receive Data Source Select: LPUART_RX pin. */
                  | SIM_SOPT5_LPUART0RXSRC(SOPT5_LPUART0RXSRC_0b0));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDs:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '19', peripheral: GPIOB, signal: 'GPIO, 3', pin_signal: ADC0_SE2/CMP0_IN4/PTB3/ERCLK32K/LPUART1_RTS_b/TPM0_CH1/CLKOUT/TPM1_CH1/RTC_CLKOUT/TPM2_CH1,
    direction: OUTPUT}
  - {pin_num: '37', peripheral: GPIOC, signal: 'GPIO, 1', pin_signal: PTC1/I2C0_SDA/LPUART0_RTS_b/TPM0_CH2/SPI1_SCK, direction: OUTPUT}
  - {pin_num: '18', peripheral: GPIOB, signal: 'GPIO, 2', pin_signal: ADC0_SE3/CMP0_IN3/PTB2/LLWU_P9/TPM0_CH0/TPM1_CH0/TPM2_CH0, direction: OUTPUT}
  - {pin_num: '4', peripheral: GPIOA, signal: 'GPIO, 16', pin_signal: PTA16/LLWU_P4/SPI1_SOUT/LPUART1_RTS_b/TPM0_CH0, direction: OUTPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitLEDs
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitLEDs(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    gpio_pin_config_t LED_G_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTA16 (pin 4)  */
    GPIO_PinInit(BOARD_INITLEDS_LED_G_GPIO, BOARD_INITLEDS_LED_G_PIN, &LED_G_config);

    gpio_pin_config_t LED_B_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTB2 (pin 18)  */
    GPIO_PinInit(BOARD_INITLEDS_LED_B_GPIO, BOARD_INITLEDS_LED_B_PIN, &LED_B_config);

    gpio_pin_config_t LED_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTB3 (pin 19)  */
    GPIO_PinInit(BOARD_INITLEDS_LED_GPIO, BOARD_INITLEDS_LED_PIN, &LED_config);

    gpio_pin_config_t LED_R_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC1 (pin 37)  */
    GPIO_PinInit(BOARD_INITLEDS_LED_R_GPIO, BOARD_INITLEDS_LED_R_PIN, &LED_R_config);

    /* PORTA16 (pin 4) is configured as PTA16 */
    PORT_SetPinMux(BOARD_INITLEDS_LED_G_PORT, BOARD_INITLEDS_LED_G_PIN, kPORT_MuxAsGpio);

    /* PORTB2 (pin 18) is configured as PTB2 */
    PORT_SetPinMux(BOARD_INITLEDS_LED_B_PORT, BOARD_INITLEDS_LED_B_PIN, kPORT_MuxAsGpio);

    /* PORTB3 (pin 19) is configured as PTB3 */
    PORT_SetPinMux(BOARD_INITLEDS_LED_PORT, BOARD_INITLEDS_LED_PIN, kPORT_MuxAsGpio);

    /* PORTC1 (pin 37) is configured as PTC1 */
    PORT_SetPinMux(BOARD_INITLEDS_LED_R_PORT, BOARD_INITLEDS_LED_R_PIN, kPORT_MuxAsGpio);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitButtons:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '23', peripheral: GPIOB, signal: 'GPIO, 18', pin_signal: ADC0_SE4/CMP0_IN2/PTB18/LPUART1_CTS_b/I2C1_SCL/TPM_CLKIN0/TPM0_CH0/NMI_b, direction: INPUT,
    pull_enable: enable}
  - {pin_num: '38', peripheral: GPIOC, signal: 'GPIO, 2', pin_signal: PTC2/LLWU_P10/I2C1_SCL/LPUART0_RX/CMT_IRO/SPI1_SOUT, direction: INPUT, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitButtons
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitButtons(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    gpio_pin_config_t SW2_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTB18 (pin 23)  */
    GPIO_PinInit(BOARD_INITBUTTONS_SW2_GPIO, BOARD_INITBUTTONS_SW2_PIN, &SW2_config);

    gpio_pin_config_t SW3_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC2 (pin 38)  */
    GPIO_PinInit(BOARD_INITBUTTONS_SW3_GPIO, BOARD_INITBUTTONS_SW3_PIN, &SW3_config);

    /* PORTB18 (pin 23) is configured as PTB18 */
    PORT_SetPinMux(BOARD_INITBUTTONS_SW2_PORT, BOARD_INITBUTTONS_SW2_PIN, kPORT_MuxAsGpio);

    PORTB->PCR[18] = ((PORTB->PCR[18] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                      /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin. */
                      | (uint32_t)(PORT_PCR_PE_MASK));

    /* PORTC2 (pin 38) is configured as PTC2 */
    PORT_SetPinMux(BOARD_INITBUTTONS_SW3_PORT, BOARD_INITBUTTONS_SW3_PIN, kPORT_MuxAsGpio);

    PORTC->PCR[2] = ((PORTC->PCR[2] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                     /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin. */
                     | (uint32_t)(PORT_PCR_PE_MASK));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
