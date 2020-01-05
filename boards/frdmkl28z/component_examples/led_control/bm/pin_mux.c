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
processor: MKL28Z512xxx7
package_id: MKL28Z512VLL7
mcu_data: ksdk2_0
processor_version: 4.0.0
board: FRDM-KL28Z
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
- pin_list: []
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
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitBUTTONs:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '5', peripheral: GPIOE, signal: 'GPIO, 4', pin_signal: PTE4/LLWU_P2/LPSPI1_PCS0, direction: INPUT, slew_rate: slow, open_drain: disable, pull_select: up,
    pull_enable: enable}
  - {pin_num: '38', peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: TSI0_CH5/PTA4/LLWU_P3/LPI2C1_SDA/TPM0_CH1/NMI0_b, direction: INPUT, slew_rate: slow, open_drain: disable,
    pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBUTTONs
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBUTTONs(void)
{
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortE);

    gpio_pin_config_t SW2_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTA4 (pin 38)  */
    GPIO_PinInit(BOARD_SW2_GPIO, BOARD_SW2_PIN, &SW2_config);

    gpio_pin_config_t SW3_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTE4 (pin 5)  */
    GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_PIN, &SW3_config);

    /* PORTA4 (pin 38) is configured as PTA4 */
    PORT_SetPinMux(BOARD_SW2_PORT, BOARD_SW2_PIN, kPORT_MuxAsGpio);

    PORTA->PCR[4] =
        ((PORTA->PCR[4] &
          /* Mask bits to zero which are setting */
          (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_SRE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK)))

         /* Pull Select: Internal pulldown resistor is enabled on the corresponding pin, if the corresponding PE
          * field is set. */
         | PORT_PCR_PS(kPORT_PullDown)

         /* Pull Enable: Internal pullup or pulldown resistor is not enabled on the corresponding pin. */
         | PORT_PCR_PE(kPORT_PullDisable)

         /* Slew Rate Enable: Slow slew rate is configured on the corresponding pin, if the pin is configured as
          * a digital output. */
         | PORT_PCR_SRE(kPORT_SlowSlewRate)

         /* Open Drain Enable: Open drain output is disabled on the corresponding pin. */
         | PORT_PCR_ODE(kPORT_OpenDrainDisable));

    const port_pin_config_t SW3 = {/* Internal pull-up resistor is enabled */
                                   kPORT_PullUp,
                                   /* Slow slew rate is configured */
                                   kPORT_SlowSlewRate,
                                   /* Passive filter is disabled */
                                   kPORT_PassiveFilterDisable,
                                   /* Open drain is disabled */
                                   kPORT_OpenDrainDisable,
                                   /* Low drive strength is configured */
                                   kPORT_LowDriveStrength,
                                   /* Pin is configured as PTE4 */
                                   kPORT_MuxAsGpio,
                                   /* Pin Control Register fields [15:0] are not locked */
                                   kPORT_UnlockRegister};
    /* PORTE4 (pin 5) is configured as PTE4 */
    PORT_SetPinConfig(BOARD_SW3_PORT, BOARD_SW3_PIN, &SW3);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDs:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '26', peripheral: GPIOE, signal: 'GPIO, 29', pin_signal: CMP1_IN5/CMP0_IN5/ADC0_SE4b/PTE29/EMVSIM0_CLK/TPM0_CH2/TPM0_CLKIN, direction: OUTPUT, gpio_init_state: 'true',
    slew_rate: slow, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: '28', peripheral: GPIOE, signal: 'GPIO, 31', pin_signal: PTE31/EMVSIM0_VCCEN/TPM0_CH4/TPM2_CLKIN/LPI2C0_HREQ, direction: OUTPUT, gpio_init_state: 'true',
    slew_rate: slow, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: '76', peripheral: GPIOC, signal: 'GPIO, 4', pin_signal: PTC4/LLWU_P8/LPSPI0_PCS0/LPUART1_TX/TPM0_CH3/I2S0_MCLK/CMP1_OUT, direction: OUTPUT, gpio_init_state: 'true',
    slew_rate: slow, open_drain: disable, pull_select: down, pull_enable: disable}
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
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortE);

    gpio_pin_config_t LED_GREEN_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTC4 (pin 76)  */
    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, &LED_GREEN_config);

    gpio_pin_config_t LED_RED_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTE29 (pin 26)  */
    GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, &LED_RED_config);

    gpio_pin_config_t LED_BLUE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTE31 (pin 28)  */
    GPIO_PinInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, &LED_BLUE_config);

    /* PORTC4 (pin 76) is configured as PTC4 */
    PORT_SetPinMux(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, kPORT_MuxAsGpio);

    PORTC->PCR[4] =
        ((PORTC->PCR[4] &
          /* Mask bits to zero which are setting */
          (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_SRE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK)))

         /* Pull Select: Internal pulldown resistor is enabled on the corresponding pin, if the corresponding PE
          * field is set. */
         | PORT_PCR_PS(kPORT_PullDown)

         /* Pull Enable: Internal pullup or pulldown resistor is not enabled on the corresponding pin. */
         | PORT_PCR_PE(kPORT_PullDisable)

         /* Slew Rate Enable: Slow slew rate is configured on the corresponding pin, if the pin is configured as
          * a digital output. */
         | PORT_PCR_SRE(kPORT_SlowSlewRate)

         /* Open Drain Enable: Open drain output is disabled on the corresponding pin. */
         | PORT_PCR_ODE(kPORT_OpenDrainDisable));

    const port_pin_config_t LED_RED = {/* Internal pull-up/down resistor is disabled */
                                       kPORT_PullDisable,
                                       /* Slow slew rate is configured */
                                       kPORT_SlowSlewRate,
                                       /* Passive filter is disabled */
                                       kPORT_PassiveFilterDisable,
                                       /* Open drain is disabled */
                                       kPORT_OpenDrainDisable,
                                       /* Low drive strength is configured */
                                       kPORT_LowDriveStrength,
                                       /* Pin is configured as PTE29 */
                                       kPORT_MuxAsGpio,
                                       /* Pin Control Register fields [15:0] are not locked */
                                       kPORT_UnlockRegister};
    /* PORTE29 (pin 26) is configured as PTE29 */
    PORT_SetPinConfig(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, &LED_RED);

    const port_pin_config_t LED_BLUE = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Slow slew rate is configured */
                                        kPORT_SlowSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as PTE31 */
                                        kPORT_MuxAsGpio,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTE31 (pin 28) is configured as PTE31 */
    PORT_SetPinConfig(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, &LED_BLUE);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitDEBUG_UART:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '62', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH9/PTB16/LPSPI1_SOUT/LPUART0_RX/TPM0_CLKIN/LPSPI2_PCS3/FXIO0_D16, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: '63', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH10/PTB17/LPSPI1_SIN/LPUART0_TX/TPM1_CLKIN/LPSPI2_PCS2/FXIO0_D17, direction: OUTPUT, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitDEBUG_UART
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitDEBUG_UART(void)
{
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortB);

    const port_pin_config_t DEBUG_UART_RX = {/* Internal pull-up/down resistor is disabled */
                                             kPORT_PullDisable,
                                             /* Fast slew rate is configured */
                                             kPORT_FastSlewRate,
                                             /* Passive filter is disabled */
                                             kPORT_PassiveFilterDisable,
                                             /* Open drain is disabled */
                                             kPORT_OpenDrainDisable,
                                             /* Low drive strength is configured */
                                             kPORT_LowDriveStrength,
                                             /* Pin is configured as LPUART0_RX */
                                             kPORT_MuxAlt3,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    /* PORTB16 (pin 62) is configured as LPUART0_RX */
    PORT_SetPinConfig(BOARD_DEBUG_UART_RX_PORT, BOARD_DEBUG_UART_RX_PIN, &DEBUG_UART_RX);

    const port_pin_config_t DEBUG_UART_TX = {/* Internal pull-up/down resistor is disabled */
                                             kPORT_PullDisable,
                                             /* Fast slew rate is configured */
                                             kPORT_FastSlewRate,
                                             /* Passive filter is disabled */
                                             kPORT_PassiveFilterDisable,
                                             /* Open drain is disabled */
                                             kPORT_OpenDrainDisable,
                                             /* Low drive strength is configured */
                                             kPORT_LowDriveStrength,
                                             /* Pin is configured as LPUART0_TX */
                                             kPORT_MuxAlt3,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    /* PORTB17 (pin 63) is configured as LPUART0_TX */
    PORT_SetPinConfig(BOARD_DEBUG_UART_TX_PORT, BOARD_DEBUG_UART_TX_PIN, &DEBUG_UART_TX);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
