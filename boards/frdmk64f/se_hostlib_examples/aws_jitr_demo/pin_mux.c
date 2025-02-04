/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v4.0
processor: MK64FN1M0xxx12
package_id: MK64FN1M0VLL12
mcu_data: ksdk2_0
processor_version: 3.0.1
board: FRDM-K64F
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '54', peripheral: ENET, signal: RMII_MDC, pin_signal: ADC0_SE9/ADC1_SE9/PTB1/I2C0_SDA/FTM1_CH1/RMII0_MDC/MII0_MDC/FTM1_QD_PHB, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: '53', peripheral: ENET, signal: RMII_MDIO, pin_signal: ADC0_SE8/ADC1_SE8/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/RMII0_MDIO/MII0_MDIO/FTM1_QD_PHA, slew_rate: fast,
    open_drain: enable, drive_strength: low, pull_select: up, pull_enable: enable, passive_filter: disable}
  - {pin_num: '43', peripheral: ENET, signal: RMII_RXD0, pin_signal: CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/RMII0_RXD0/MII0_RXD0/I2C2_SDA/I2S0_TX_FS/FTM1_QD_PHB,
    slew_rate: fast, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: '42', peripheral: ENET, signal: RMII_RXD1, pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/I2C2_SCL/I2S0_TXD0/FTM1_QD_PHA, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: '39', peripheral: ENET, signal: RMII_RXER, pin_signal: PTA5/USB_CLKIN/FTM0_CH2/RMII0_RXER/MII0_RXER/CMP2_OUT/I2S0_TX_BCLK/JTAG_TRST_b, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: '46', peripheral: ENET, signal: RMII_TXD0, pin_signal: PTA16/SPI0_SOUT/UART0_CTS_b/UART0_COL_b/RMII0_TXD0/MII0_TXD0/I2S0_RX_FS/I2S0_RXD1, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: '47', peripheral: ENET, signal: RMII_TXD1, pin_signal: ADC1_SE17/PTA17/SPI0_SIN/UART0_RTS_b/RMII0_TXD1/MII0_TXD1/I2S0_MCLK, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: '45', peripheral: ENET, signal: RMII_TXEN, pin_signal: PTA15/SPI0_SCK/UART0_RX/RMII0_TXEN/MII0_TXEN/I2S0_RXD0, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: '44', peripheral: ENET, signal: RMII_CRS_DV, pin_signal: PTA14/SPI0_PCS0/UART0_TX/RMII0_CRS_DV/MII0_RXDV/I2C2_SCL/I2S0_RX_BCLK/I2S0_TXD1, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: '90', peripheral: ENET, signal: 'TMR_1588, 0', pin_signal: PTC16/UART3_RX/ENET0_1588_TMR0/FB_CS5_b/FB_TSIZ1/FB_BE23_16_BLS15_8_b}
  - {pin_num: '91', peripheral: ENET, signal: 'TMR_1588, 1', pin_signal: PTC17/UART3_TX/ENET0_1588_TMR1/FB_CS4_b/FB_TSIZ0/FB_BE31_24_BLS7_0_b}
  - {pin_num: '92', peripheral: ENET, signal: 'TMR_1588, 2', pin_signal: PTC18/UART3_RTS_b/ENET0_1588_TMR2/FB_TBST_b/FB_CS2_b/FB_BE15_8_BLS23_16_b}
  - {pin_num: '62', peripheral: UART0, signal: RX, pin_signal: PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/EWM_IN}
  - {pin_num: '63', peripheral: UART0, signal: TX, pin_signal: PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/EWM_OUT_b}
  - {pin_num: '31', peripheral: I2C0, signal: SCL, pin_signal: ADC0_SE17/PTE24/UART4_TX/I2C0_SCL/EWM_OUT_b, identifier: '', open_drain: enable, pull_select: up, pull_enable: enable}
  - {pin_num: '32', peripheral: I2C0, signal: SDA, pin_signal: ADC0_SE18/PTE25/UART4_RX/I2C0_SDA/EWM_IN, identifier: '', open_drain: enable, pull_select: up, pull_enable: enable}
  - {pin_num: '68', peripheral: GPIOB, signal: 'GPIO, 22', pin_signal: PTB22/SPI2_SOUT/FB_AD29/CMP2_OUT, direction: OUTPUT}
  - {pin_num: '33', peripheral: GPIOE, signal: 'GPIO, 26', pin_signal: PTE26/ENET_1588_CLKIN/UART4_CTS_b/RTC_CLKOUT/USB_CLKIN, direction: OUTPUT}
  - {pin_num: '67', peripheral: GPIOB, signal: 'GPIO, 21', pin_signal: PTB21/SPI2_SCK/FB_AD30/CMP1_OUT, direction: OUTPUT}
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
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t RMII0_RXD1 = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as RMII0_RXD1 */
                                          kPORT_MuxAlt4,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTA12 (pin 42) is configured as RMII0_RXD1 */
    PORT_SetPinConfig(BOARD_RMII0_RXD1_PORT, BOARD_RMII0_RXD1_PIN, &RMII0_RXD1);

    const port_pin_config_t RMII0_RXD0 = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as RMII0_RXD0 */
                                          kPORT_MuxAlt4,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTA13 (pin 43) is configured as RMII0_RXD0 */
    PORT_SetPinConfig(BOARD_RMII0_RXD0_PORT, BOARD_RMII0_RXD0_PIN, &RMII0_RXD0);

    const port_pin_config_t RMII0_CRS_DV = {/* Internal pull-up/down resistor is disabled */
                                            kPORT_PullDisable,
                                            /* Fast slew rate is configured */
                                            kPORT_FastSlewRate,
                                            /* Passive filter is disabled */
                                            kPORT_PassiveFilterDisable,
                                            /* Open drain is disabled */
                                            kPORT_OpenDrainDisable,
                                            /* Low drive strength is configured */
                                            kPORT_LowDriveStrength,
                                            /* Pin is configured as RMII0_CRS_DV */
                                            kPORT_MuxAlt4,
                                            /* Pin Control Register fields [15:0] are not locked */
                                            kPORT_UnlockRegister};
    /* PORTA14 (pin 44) is configured as RMII0_CRS_DV */
    PORT_SetPinConfig(BOARD_RMII0_CRS_DV_PORT, BOARD_RMII0_CRS_DV_PIN, &RMII0_CRS_DV);

    const port_pin_config_t RMII0_TXEN = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as RMII0_TXEN */
                                          kPORT_MuxAlt4,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTA15 (pin 45) is configured as RMII0_TXEN */
    PORT_SetPinConfig(BOARD_RMII0_TXEN_PORT, BOARD_RMII0_TXEN_PIN, &RMII0_TXEN);

    const port_pin_config_t RMII0_TXD0 = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as RMII0_TXD0 */
                                          kPORT_MuxAlt4,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTA16 (pin 46) is configured as RMII0_TXD0 */
    PORT_SetPinConfig(BOARD_RMII0_TXD0_PORT, BOARD_RMII0_TXD0_PIN, &RMII0_TXD0);

    const port_pin_config_t RMII0_TXD1 = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as RMII0_TXD1 */
                                          kPORT_MuxAlt4,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTA17 (pin 47) is configured as RMII0_TXD1 */
    PORT_SetPinConfig(BOARD_RMII0_TXD1_PORT, BOARD_RMII0_TXD1_PIN, &RMII0_TXD1);

    const port_pin_config_t RMII0_RXER = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as RMII0_RXER */
                                          kPORT_MuxAlt4,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTA5 (pin 39) is configured as RMII0_RXER */
    PORT_SetPinConfig(BOARD_RMII0_RXER_PORT, BOARD_RMII0_RXER_PIN, &RMII0_RXER);

    const port_pin_config_t RMII0_MDIO = {/* Internal pull-up resistor is enabled */
                                          kPORT_PullUp,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is enabled */
                                          kPORT_OpenDrainEnable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as RMII0_MDIO */
                                          kPORT_MuxAlt4,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTB0 (pin 53) is configured as RMII0_MDIO */
    PORT_SetPinConfig(BOARD_RMII0_MDIO_PORT, BOARD_RMII0_MDIO_PIN, &RMII0_MDIO);

    const port_pin_config_t RMII0_MDC = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as RMII0_MDC */
                                         kPORT_MuxAlt4,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTB1 (pin 54) is configured as RMII0_MDC */
    PORT_SetPinConfig(BOARD_RMII0_MDC_PORT, BOARD_RMII0_MDC_PIN, &RMII0_MDC);

    /* PORTB16 (pin 62) is configured as UART0_RX */
    PORT_SetPinMux(BOARD_DEBUG_UART_RX_PORT, BOARD_DEBUG_UART_RX_PIN, kPORT_MuxAlt3);

    /* PORTB17 (pin 63) is configured as UART0_TX */
    PORT_SetPinMux(BOARD_DEBUG_UART_TX_PORT, BOARD_DEBUG_UART_TX_PIN, kPORT_MuxAlt3);

    /* PORTC16 (pin 90) is configured as ENET0_1588_TMR0 */
    PORT_SetPinMux(BOARD_TMR_1588_0_PORT, BOARD_TMR_1588_0_PIN, kPORT_MuxAlt4);

    /* PORTC17 (pin 91) is configured as ENET0_1588_TMR1 */
    PORT_SetPinMux(BOARD_TMR_1588_1_PORT, BOARD_TMR_1588_1_PIN, kPORT_MuxAlt4);

    /* PORTC18 (pin 92) is configured as ENET0_1588_TMR2 */
    PORT_SetPinMux(PORTC, 18U, kPORT_MuxAlt4);

    /* PORTE24 (pin 31) is configured as I2C0_SCL */
    PORT_SetPinMux(PORTE, 24U, kPORT_MuxAlt5);

    PORTE->PCR[24] = ((PORTE->PCR[24] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK)))

                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | (uint32_t)(kPORT_PullUp)

                      /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is
                       * configured as a digital output. */
                      | PORT_PCR_ODE(kPORT_OpenDrainEnable));

    /* PORTE25 (pin 32) is configured as I2C0_SDA */
    PORT_SetPinMux(PORTE, 25U, kPORT_MuxAlt5);

    PORTE->PCR[25] = ((PORTE->PCR[25] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ODE_MASK | PORT_PCR_ISF_MASK)))

                      /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | (uint32_t)(kPORT_PullUp)

                      /* Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is
                       * configured as a digital output. */
                      | PORT_PCR_ODE(kPORT_OpenDrainEnable));

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART0TXSRC_MASK)))

                  /* UART 0 transmit data source select: UART0_TX pin. */
                  | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX));

    /* PORTB21 (pin 67) is configured as PTB21 */
    PORT_SetPinMux(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, kPORT_MuxAsGpio);

    /* PORTB22 (pin 68) is configured as PTB22 */
    PORT_SetPinMux(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, kPORT_MuxAsGpio);

    /* PORTE26 (pin 33) is configured as PTE26 */
    PORT_SetPinMux(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, kPORT_MuxAsGpio);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
