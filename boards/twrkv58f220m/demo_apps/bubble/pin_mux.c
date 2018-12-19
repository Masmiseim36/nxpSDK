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
processor: MKV58F1M0xxx24
package_id: MKV58F1M0VLQ24
mcu_data: ksdk2_0
processor_version: 0.0.8
board: TWR-KV58F220M
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
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '81', peripheral: UART0, signal: RX, pin_signal: HSADC0B_CH2/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/FTM1_QD_PHA/UART0_RX/RMII0_MDIO/MII0_MDIO}
  - {pin_num: '82', peripheral: UART0, signal: TX, pin_signal: HSADC0B_CH3/PTB1/I2C0_SDA/FTM1_CH1/FTM0_FLT2/EWM_IN/FTM1_QD_PHB/UART0_TX/RMII0_MDC/MII0_MDC}
  - {pin_num: '14', peripheral: FTM3, signal: 'CH, 6', pin_signal: HSADC1A_CH6/ADC0_SE3/ADC0_DP3/PTE11/UART5_RTS_b/FLEXPWM1_A3/FTM3_CH6, direction: OUTPUT}
  - {pin_num: '15', peripheral: FTM3, signal: 'CH, 7', pin_signal: HSADC1B_CH6/ADC0_SE11/ADC0_DM3/PTE12/FLEXPWM1_B3/FTM3_CH7, direction: OUTPUT}
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
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTB0 (pin 81) is configured as UART0_RX */
    PORT_SetPinMux(BOARD_DEBUG_UART_RX_PORT, BOARD_DEBUG_UART_RX_PIN, kPORT_MuxAlt7);

    /* PORTB1 (pin 82) is configured as UART0_TX */
    PORT_SetPinMux(BOARD_DEBUG_UART_TX_PORT, BOARD_DEBUG_UART_TX_PIN, kPORT_MuxAlt7);

    /* PORTE11 (pin 14) is configured as FTM3_CH6 */
    PORT_SetPinMux(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, kPORT_MuxAlt6);

    /* PORTE12 (pin 15) is configured as FTM3_CH7 */
    PORT_SetPinMux(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, kPORT_MuxAlt6);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK)))

                  /* UART 0 transmit data source select: UART0_TX pin. */
                  | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)

                  /* UART 0 receive data source select: UART0_RX pin. */
                  | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX));

    SIM->SOPT8 = ((SIM->SOPT8 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT8_FTM3OCH6SRC_MASK | SIM_SOPT8_FTM3OCH7SRC_MASK)))

                  /* FTM3 channel 6 output source: FTM3_CH6 pin is output of FTM3 channel 6 output. */
                  | SIM_SOPT8_FTM3OCH6SRC(SOPT8_FTM3OCH6SRC_FTM)

                  /* FTM3 channel 7 output source: FTM3_CH7 pin is output of FTM3 channel 7 output. */
                  | SIM_SOPT8_FTM3OCH7SRC(SOPT8_FTM3OCH7SRC_FTM));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_I2C_ConfigurePins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '137', peripheral: I2C1, signal: SCL, pin_signal: PTD8/LLWU_P24/I2C1_SCL/UART5_RX/FLEXPWM0_A3/FB_A16, slew_rate: fast, open_drain: enable, pull_select: up,
    pull_enable: enable, digital_filter: disable}
  - {pin_num: '138', peripheral: I2C1, signal: SDA, pin_signal: PTD9/I2C1_SDA/UART5_TX/FLEXPWM0_B3/FB_A17, slew_rate: fast, open_drain: enable, pull_select: up, pull_enable: enable,
    digital_filter: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_I2C_ConfigurePins
 *
 * END ****************************************************************************************************************/
void BOARD_I2C_ConfigurePins(void)
{
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Configure digital filter */
    PORT_EnablePinsDigitalFilter(
        /* Digital filter is configured on port D */
        PORTD,
        /* Digital filter is configured for PORTD0 */
        PORT_DFER_DFE_8_MASK
            /* Digital filter is configured for PORTD1 */
            | PORT_DFER_DFE_9_MASK,
        /* Disable digital filter */
        false);

    const port_pin_config_t I2C_SCL = {/* Internal pull-up resistor is enabled */
                                       kPORT_PullUp,
                                       /* Fast slew rate is configured */
                                       kPORT_FastSlewRate,
                                       /* Passive filter is disabled */
                                       kPORT_PassiveFilterDisable,
                                       /* Open drain is enabled */
                                       kPORT_OpenDrainEnable,
                                       /* Low drive strength is configured */
                                       kPORT_LowDriveStrength,
                                       /* Pin is configured as I2C1_SCL */
                                       kPORT_MuxAlt2,
                                       /* Pin Control Register fields [15:0] are not locked */
                                       kPORT_UnlockRegister};
    /* PORTD8 (pin 137) is configured as I2C1_SCL */
    PORT_SetPinConfig(BOARD_I2C_CONFIGUREPINS_I2C_SCL_PORT, BOARD_I2C_CONFIGUREPINS_I2C_SCL_PIN, &I2C_SCL);

    const port_pin_config_t I2C_SDA = {/* Internal pull-up resistor is enabled */
                                       kPORT_PullUp,
                                       /* Fast slew rate is configured */
                                       kPORT_FastSlewRate,
                                       /* Passive filter is disabled */
                                       kPORT_PassiveFilterDisable,
                                       /* Open drain is enabled */
                                       kPORT_OpenDrainEnable,
                                       /* Low drive strength is configured */
                                       kPORT_LowDriveStrength,
                                       /* Pin is configured as I2C1_SDA */
                                       kPORT_MuxAlt2,
                                       /* Pin Control Register fields [15:0] are not locked */
                                       kPORT_UnlockRegister};
    /* PORTD9 (pin 138) is configured as I2C1_SDA */
    PORT_SetPinConfig(BOARD_I2C_CONFIGUREPINS_I2C_SDA_PORT, BOARD_I2C_CONFIGUREPINS_I2C_SDA_PIN, &I2C_SDA);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
