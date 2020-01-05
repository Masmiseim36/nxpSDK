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
processor: MKE18F512xxx16
package_id: MKE18F512VLL16
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
LPUART0_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '54', peripheral: LPUART0, signal: RX, pin_signal: ADC0_SE4/PTB0/LPUART0_RX/LPSPI0_PCS0/LPTMR0_ALT3/PWT_IN3}
  - {pin_num: '53', peripheral: LPUART0, signal: TX, pin_signal: ADC0_SE5/PTB1/LPUART0_TX/LPSPI0_SOUT/TCLK0}
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
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortB);

    /* PORTB0 (pin 54) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt2);

    /* PORTB1 (pin 53) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART0_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '54', peripheral: ADC0, signal: 'SE, 4', pin_signal: ADC0_SE4/PTB0/LPUART0_RX/LPSPI0_PCS0/LPTMR0_ALT3/PWT_IN3}
  - {pin_num: '53', peripheral: ADC0, signal: 'SE, 5', pin_signal: ADC0_SE5/PTB1/LPUART0_TX/LPSPI0_SOUT/TCLK0}
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
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortB);

    /* PORTB0 (pin 54) is configured as ADC0_SE4 */
    PORT_SetPinMux(PORTB, 0U, kPORT_PinDisabledOrAnalog);

    /* PORTB1 (pin 53) is configured as ADC0_SE5 */
    PORT_SetPinMux(PORTB, 1U, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART1_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '81', peripheral: LPUART1, signal: RX, pin_signal: ADC1_SE4/PTC6/LPUART1_RX/CAN1_RX/FTM3_CH2}
  - {pin_num: '80', peripheral: LPUART1, signal: TX, pin_signal: ADC1_SE5/PTC7/LPUART1_TX/CAN1_TX/FTM3_CH3}
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
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC6 (pin 81) is configured as LPUART1_RX */
    PORT_SetPinMux(PORTC, 6U, kPORT_MuxAlt2);

    /* PORTC7 (pin 80) is configured as LPUART1_TX */
    PORT_SetPinMux(PORTC, 7U, kPORT_MuxAlt2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART1_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '81', peripheral: ADC1, signal: 'SE, 4', pin_signal: ADC1_SE4/PTC6/LPUART1_RX/CAN1_RX/FTM3_CH2}
  - {pin_num: '80', peripheral: ADC1, signal: 'SE, 5', pin_signal: ADC1_SE5/PTC7/LPUART1_TX/CAN1_TX/FTM3_CH3}
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
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC6 (pin 81) is configured as ADC1_SE4 */
    PORT_SetPinMux(PORTC, 6U, kPORT_PinDisabledOrAnalog);

    /* PORTC7 (pin 80) is configured as ADC1_SE5 */
    PORT_SetPinMux(PORTC, 7U, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART2_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '32', peripheral: LPUART2, signal: RX, pin_signal: PTD6/LPUART2_RX/FTM2_FLT2}
  - {pin_num: '31', peripheral: LPUART2, signal: TX, pin_signal: PTD7/LPUART2_TX/FTM2_FLT3}
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
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTD6 (pin 32) is configured as LPUART2_RX */
    PORT_SetPinMux(PORTD, 6U, kPORT_MuxAlt2);

    /* PORTD7 (pin 31) is configured as LPUART2_TX */
    PORT_SetPinMux(PORTD, 7U, kPORT_MuxAlt2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART2_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '32', peripheral: n/a, signal: disabled, pin_signal: PTD6/LPUART2_RX/FTM2_FLT2}
  - {pin_num: '31', peripheral: n/a, signal: disabled, pin_signal: PTD7/LPUART2_TX/FTM2_FLT3}
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
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTD6 (pin 32) is disabled */
    PORT_SetPinMux(PORTD, 6U, kPORT_PinDisabledOrAnalog);

    /* PORTD7 (pin 31) is disabled */
    PORT_SetPinMux(PORTD, 7U, kPORT_PinDisabledOrAnalog);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
