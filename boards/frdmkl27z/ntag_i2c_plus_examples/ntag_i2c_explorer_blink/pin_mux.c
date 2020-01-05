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
processor: MKL27Z64xxx4
package_id: MKL27Z64VLH4
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
  - {pin_num: '20', peripheral: GPIOE, signal: 'GPIO, 24', pin_signal: PTE24/TPM0_CH0/I2C0_SCL, direction: INPUT, slew_rate: fast, pull_select: up, pull_enable: enable}
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
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t porte24_pin20_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as PTE24 */
                                                    kPORT_MuxAsGpio};
    /* PORTE24 (pin 20) is configured as PTE24 */
    PORT_SetPinConfig(PORTE, 24U, &porte24_pin20_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDs:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '29', peripheral: GPIOA, signal: 'GPIO, 13', pin_signal: PTA13/TPM1_CH1, direction: OUTPUT, slew_rate: slow, pull_select: down, pull_enable: disable}
  - {pin_num: '41', peripheral: GPIOB, signal: 'GPIO, 18', pin_signal: PTB18/TPM2_CH0, direction: OUTPUT, slew_rate: slow, pull_select: down, pull_enable: disable}
  - {pin_num: '42', peripheral: GPIOB, signal: 'GPIO, 19', pin_signal: PTB19/TPM2_CH1, direction: OUTPUT, slew_rate: slow, pull_select: down, pull_enable: disable}
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

    const port_pin_config_t porta13_pin29_config = {/* Internal pull-up/down resistor is disabled */
                                                    kPORT_PullDisable,
                                                    /* Slow slew rate is configured */
                                                    kPORT_SlowSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as PTA13 */
                                                    kPORT_MuxAsGpio};
    /* PORTA13 (pin 29) is configured as PTA13 */
    PORT_SetPinConfig(PORTA, 13U, &porta13_pin29_config);

    const port_pin_config_t portb18_pin41_config = {/* Internal pull-up/down resistor is disabled */
                                                    kPORT_PullDisable,
                                                    /* Slow slew rate is configured */
                                                    kPORT_SlowSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as PTB18 */
                                                    kPORT_MuxAsGpio};
    /* PORTB18 (pin 41) is configured as PTB18 */
    PORT_SetPinConfig(PORTB, 18U, &portb18_pin41_config);

    const port_pin_config_t portb19_pin42_config = {/* Internal pull-up/down resistor is disabled */
                                                    kPORT_PullDisable,
                                                    /* Slow slew rate is configured */
                                                    kPORT_SlowSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as PTB19 */
                                                    kPORT_MuxAsGpio};
    /* PORTB19 (pin 42) is configured as PTB19 */
    PORT_SetPinConfig(PORTB, 19U, &portb19_pin42_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitButtons:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '26', peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: PTA4/I2C1_SDA/TPM0_CH1/NMI_b, direction: INPUT, slew_rate: fast, pull_select: up, pull_enable: enable,
    passive_filter: disable}
  - {pin_num: '44', peripheral: GPIOC, signal: 'GPIO, 1', pin_signal: ADC0_SE15/PTC1/LLWU_P6/RTC_CLKIN/I2C1_SCL/TPM0_CH0, direction: INPUT, slew_rate: fast, pull_select: up,
    pull_enable: enable}
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
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    const port_pin_config_t porta4_pin26_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as PTA4 */
                                                   kPORT_MuxAsGpio};
    /* PORTA4 (pin 26) is configured as PTA4 */
    PORT_SetPinConfig(PORTA, 4U, &porta4_pin26_config);

    const port_pin_config_t portc1_pin44_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as PTC1 */
                                                   kPORT_MuxAsGpio};
    /* PORTC1 (pin 44) is configured as PTC1 */
    PORT_SetPinConfig(PORTC, 1U, &portc1_pin44_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitTSI:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '17', peripheral: GPIOE, signal: 'GPIO, 29', pin_signal: CMP0_IN5/ADC0_SE4b/PTE29/TPM0_CH2/TPM_CLKIN0, direction: INPUT, slew_rate: slow, pull_select: up,
    pull_enable: enable}
  - {pin_num: '18', peripheral: GPIOE, signal: 'GPIO, 30', pin_signal: ADC0_SE23/CMP0_IN4/PTE30/TPM0_CH3/TPM_CLKIN1/LPUART1_TX/LPTMR0_ALT1, direction: INPUT, slew_rate: slow,
    pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitTSI
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitTSI(void)
{
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t porte29_pin17_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Slow slew rate is configured */
                                                    kPORT_SlowSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as PTE29 */
                                                    kPORT_MuxAsGpio};
    /* PORTE29 (pin 17) is configured as PTE29 */
    PORT_SetPinConfig(PORTE, 29U, &porte29_pin17_config);

    const port_pin_config_t porte30_pin18_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Slow slew rate is configured */
                                                    kPORT_SlowSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as PTE30 */
                                                    kPORT_MuxAsGpio};
    /* PORTE30 (pin 18) is configured as PTE30 */
    PORT_SetPinConfig(PORTE, 30U, &porte30_pin18_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitUSB:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '5', peripheral: USB0, signal: DP, pin_signal: USB0_DP}
  - {pin_num: '6', peripheral: USB0, signal: DM, pin_signal: USB0_DM}
  - {pin_num: '27', peripheral: USB0, signal: CLKIN, pin_signal: PTA5/USB_CLKIN/TPM0_CH2, slew_rate: fast, pull_select: up, pull_enable: enable}
  - {pin_num: '7', peripheral: USB0, signal: VDD, pin_signal: USB_VDD}
  - {pin_num: '43', peripheral: USB0, signal: SOF_OUT, pin_signal: ADC0_SE14/PTC0/EXTRG_IN/USB_SOF_OUT/CMP0_OUT, slew_rate: fast, pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitUSB
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitUSB(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    const port_pin_config_t porta5_pin27_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as USB_CLKIN */
                                                   kPORT_MuxAlt2};
    /* PORTA5 (pin 27) is configured as USB_CLKIN */
    PORT_SetPinConfig(PORTA, 5U, &porta5_pin27_config);

    const port_pin_config_t portc0_pin43_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as USB_SOF_OUT */
                                                   kPORT_MuxAlt4};
    /* PORTC0 (pin 43) is configured as USB_SOF_OUT */
    PORT_SetPinConfig(PORTC, 0U, &portc0_pin43_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitACCEL_I2C:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '46', peripheral: GPIOC, signal: 'GPIO, 3', pin_signal: PTC3/LLWU_P7/SPI1_SCK/LPUART1_RX/TPM0_CH2/CLKOUT, direction: INPUT, slew_rate: fast, drive_strength: low,
    pull_select: up, pull_enable: enable}
  - {pin_num: '45', peripheral: GPIOC, signal: 'GPIO, 2', pin_signal: ADC0_SE11/PTC2/I2C1_SDA/TPM0_CH1, direction: INPUT, slew_rate: fast, pull_select: up, pull_enable: enable}
  - {pin_num: '63', peripheral: I2C1, signal: SDA, pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI1_MOSI/LPUART0_RX/I2C1_SDA/SPI1_MISO/FXIO0_D6}
  - {pin_num: '64', peripheral: I2C1, signal: SCL, pin_signal: PTD7/SPI1_MISO/LPUART0_TX/I2C1_SCL/SPI1_MOSI/FXIO0_D7}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitACCEL_I2C
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitACCEL_I2C(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    const port_pin_config_t portc2_pin45_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as PTC2 */
                                                   kPORT_MuxAsGpio};
    /* PORTC2 (pin 45) is configured as PTC2 */
    PORT_SetPinConfig(PORTC, 2U, &portc2_pin45_config);

    const port_pin_config_t portc3_pin46_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as PTC3 */
                                                   kPORT_MuxAsGpio};
    /* PORTC3 (pin 46) is configured as PTC3 */
    PORT_SetPinConfig(PORTC, 3U, &portc3_pin46_config);

    /* PORTD6 (pin 63) is configured as I2C1_SDA */
    PORT_SetPinMux(PORTD, 6U, kPORT_MuxAlt4);

    /* PORTD7 (pin 64) is configured as I2C1_SCL */
    PORT_SetPinMux(PORTD, 7U, kPORT_MuxAlt4);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitMAGNET_I2C:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '64', peripheral: I2C1, signal: SCL, pin_signal: PTD7/SPI1_MISO/LPUART0_TX/I2C1_SCL/SPI1_MOSI/FXIO0_D7, slew_rate: fast, drive_strength: low, pull_select: up,
    pull_enable: enable}
  - {pin_num: '63', peripheral: I2C1, signal: SDA, pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI1_MOSI/LPUART0_RX/I2C1_SDA/SPI1_MISO/FXIO0_D6, slew_rate: fast, drive_strength: low,
    pull_select: up, pull_enable: enable}
  - {pin_num: '45', peripheral: GPIOC, signal: 'GPIO, 2', pin_signal: ADC0_SE11/PTC2/I2C1_SDA/TPM0_CH1, direction: INPUT, slew_rate: fast, pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitMAGNET_I2C
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitMAGNET_I2C(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    const port_pin_config_t portc2_pin45_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as PTC2 */
                                                   kPORT_MuxAsGpio};
    /* PORTC2 (pin 45) is configured as PTC2 */
    PORT_SetPinConfig(PORTC, 2U, &portc2_pin45_config);

    const port_pin_config_t portd6_pin63_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as I2C1_SDA */
                                                   kPORT_MuxAlt4};
    /* PORTD6 (pin 63) is configured as I2C1_SDA */
    PORT_SetPinConfig(PORTD, 6U, &portd6_pin63_config);

    const port_pin_config_t portd7_pin64_config = {/* Internal pull-up resistor is enabled */
                                                   kPORT_PullUp,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as I2C1_SCL */
                                                   kPORT_MuxAlt4};
    /* PORTD7 (pin 64) is configured as I2C1_SCL */
    PORT_SetPinConfig(PORTD, 7U, &portd7_pin64_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitDEBUG_UART:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '23', peripheral: LPUART0, signal: RX, pin_signal: PTA1/LPUART0_RX/TPM2_CH0, slew_rate: fast, pull_select: down, pull_enable: disable}
  - {pin_num: '24', peripheral: LPUART0, signal: TX, pin_signal: PTA2/LPUART0_TX/TPM2_CH1, direction: OUTPUT, slew_rate: fast, pull_select: down, pull_enable: disable}
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
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);

    const port_pin_config_t porta1_pin23_config = {/* Internal pull-up/down resistor is disabled */
                                                   kPORT_PullDisable,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as LPUART0_RX */
                                                   kPORT_MuxAlt2};
    /* PORTA1 (pin 23) is configured as LPUART0_RX */
    PORT_SetPinConfig(PORTA, 1U, &porta1_pin23_config);

    const port_pin_config_t porta2_pin24_config = {/* Internal pull-up/down resistor is disabled */
                                                   kPORT_PullDisable,
                                                   /* Fast slew rate is configured */
                                                   kPORT_FastSlewRate,
                                                   /* Passive filter is disabled */
                                                   kPORT_PassiveFilterDisable,
                                                   /* Low drive strength is configured */
                                                   kPORT_LowDriveStrength,
                                                   /* Pin is configured as LPUART0_TX */
                                                   kPORT_MuxAlt2};
    /* PORTA2 (pin 24) is configured as LPUART0_TX */
    PORT_SetPinConfig(PORTA, 2U, &porta2_pin24_config);

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
BOARD_InitTHER:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '11', peripheral: GPIOE, signal: 'GPIO, 22', pin_signal: ADC0_DP3/ADC0_SE3/PTE22/TPM2_CH0/UART2_TX/FXIO0_D6, direction: INPUT, slew_rate: slow, pull_select: up,
    pull_enable: enable}
  - {pin_num: '12', peripheral: GPIOE, signal: 'GPIO, 23', pin_signal: ADC0_DM3/ADC0_SE7a/PTE23/TPM2_CH1/UART2_RX/FXIO0_D7, direction: INPUT, slew_rate: slow, pull_select: up,
    pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitTHER
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitTHER(void)
{
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t porte22_pin11_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Slow slew rate is configured */
                                                    kPORT_SlowSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as PTE22 */
                                                    kPORT_MuxAsGpio};
    /* PORTE22 (pin 11) is configured as PTE22 */
    PORT_SetPinConfig(PORTE, 22U, &porte22_pin11_config);

    const port_pin_config_t porte23_pin12_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Slow slew rate is configured */
                                                    kPORT_SlowSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as PTE23 */
                                                    kPORT_MuxAsGpio};
    /* PORTE23 (pin 12) is configured as PTE23 */
    PORT_SetPinConfig(PORTE, 23U, &porte23_pin12_config);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitOSC:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '32', peripheral: OSC0, signal: EXTAL0, pin_signal: EXTAL0/PTA18/LPUART1_RX/TPM_CLKIN0, slew_rate: no_init, pull_select: no_init, pull_enable: no_init}
  - {pin_num: '33', peripheral: OSC0, signal: XTAL0, pin_signal: XTAL0/PTA19/LPUART1_TX/TPM_CLKIN1/LPTMR0_ALT1, slew_rate: no_init, pull_select: no_init, pull_enable: no_init}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitOSC
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitOSC(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);

    /* PORTA18 (pin 32) is configured as EXTAL0 */
    PORT_SetPinMux(PORTA, 18U, kPORT_PinDisabledOrAnalog);

    /* PORTA19 (pin 33) is configured as XTAL0 */
    PORT_SetPinMux(PORTA, 19U, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
I2C0_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '35', peripheral: I2C0, signal: SCL, pin_signal: ADC0_SE8/PTB0/LLWU_P5/I2C0_SCL/TPM1_CH0/SPI1_MOSI/SPI1_MISO, pull_enable: no_init}
  - {pin_num: '36', peripheral: I2C0, signal: SDA, pin_signal: ADC0_SE9/PTB1/I2C0_SDA/TPM1_CH1/SPI1_MISO/SPI1_MOSI, pull_enable: no_init}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : I2C0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void I2C0_InitPins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    /* PORTB0 (pin 35) is configured as I2C0_SCL */
    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt2);

    /* PORTB1 (pin 36) is configured as I2C0_SDA */
    PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
I2C0_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '35', peripheral: ADC0, signal: 'SE, 8', pin_signal: ADC0_SE8/PTB0/LLWU_P5/I2C0_SCL/TPM1_CH0/SPI1_MOSI/SPI1_MISO}
  - {pin_num: '36', peripheral: ADC0, signal: 'SE, 9', pin_signal: ADC0_SE9/PTB1/I2C0_SDA/TPM1_CH1/SPI1_MISO/SPI1_MOSI}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : I2C0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void I2C0_DeinitPins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    /* PORTB0 (pin 35) is configured as ADC0_SE8 */
    PORT_SetPinMux(PORTB, 0U, kPORT_PinDisabledOrAnalog);

    /* PORTB1 (pin 36) is configured as ADC0_SE9 */
    PORT_SetPinMux(PORTB, 1U, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
I2C1_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '63', peripheral: I2C1, signal: SDA, pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI1_MOSI/LPUART0_RX/I2C1_SDA/SPI1_MISO/FXIO0_D6}
  - {pin_num: '64', peripheral: I2C1, signal: SCL, pin_signal: PTD7/SPI1_MISO/LPUART0_TX/I2C1_SCL/SPI1_MOSI/FXIO0_D7}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : I2C1_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void I2C1_InitPins(void)
{
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTD6 (pin 63) is configured as I2C1_SDA */
    PORT_SetPinMux(PORTD, 6U, kPORT_MuxAlt4);

    /* PORTD7 (pin 64) is configured as I2C1_SCL */
    PORT_SetPinMux(PORTD, 7U, kPORT_MuxAlt4);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
I2C1_DeinitPins:
- options: {coreID: core0, enableClock: 'false'}
- pin_list:
  - {pin_num: '63', peripheral: ADC0, signal: 'SE, 7b', pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI1_MOSI/LPUART0_RX/I2C1_SDA/SPI1_MISO/FXIO0_D6}
  - {pin_num: '64', peripheral: n/a, signal: disabled, pin_signal: PTD7/SPI1_MISO/LPUART0_TX/I2C1_SCL/SPI1_MOSI/FXIO0_D7}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : I2C1_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void I2C1_DeinitPins(void)
{

    /* PORTD6 (pin 63) is configured as ADC0_SE7b */
    PORT_SetPinMux(PORTD, 6U, kPORT_PinDisabledOrAnalog);

    /* PORTD7 (pin 64) is disabled */
    PORT_SetPinMux(PORTD, 7U, kPORT_PinDisabledOrAnalog);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
