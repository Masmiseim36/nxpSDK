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
processor: MKL28Z512xxx7
package_id: MKL28Z512VLL7
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
  - {pin_num: '35', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH2/PTA1/LPUART0_RX/TPM2_CH0}
  - {pin_num: '36', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH3/PTA2/LPUART0_TX/TPM2_CH1}
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
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortA);

    /* PORTA1 (pin 35) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTA, 1U, kPORT_MuxAlt2);

    /* PORTA2 (pin 36) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART0_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '35', peripheral: TSI0, signal: 'CH, 2', pin_signal: TSI0_CH2/PTA1/LPUART0_RX/TPM2_CH0}
  - {pin_num: '36', peripheral: TSI0, signal: 'CH, 3', pin_signal: TSI0_CH3/PTA2/LPUART0_TX/TPM2_CH1}
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
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortA);

    /* PORTA1 (pin 35) is configured as TSI0_CH2 */
    PORT_SetPinMux(PORTA, 1U, kPORT_PinDisabledOrAnalog);

    /* PORTA2 (pin 36) is configured as TSI0_CH3 */
    PORT_SetPinMux(PORTA, 2U, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART1_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '73', peripheral: LPUART1, signal: RX, pin_signal: CMP1_IN1/PTC3/LLWU_P7/LPSPI0_PCS1/LPUART1_RX/TPM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: '76', peripheral: LPUART1, signal: TX, pin_signal: PTC4/LLWU_P8/LPSPI0_PCS0/LPUART1_TX/TPM0_CH3/I2S0_MCLK/CMP1_OUT}
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
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC3 (pin 73) is configured as LPUART1_RX */
    PORT_SetPinMux(PORTC, 3U, kPORT_MuxAlt3);

    /* PORTC4 (pin 76) is configured as LPUART1_TX */
    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAlt3);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART1_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '76', peripheral: n/a, signal: disabled, pin_signal: PTC4/LLWU_P8/LPSPI0_PCS0/LPUART1_TX/TPM0_CH3/I2S0_MCLK/CMP1_OUT}
  - {pin_num: '73', peripheral: CMP1, signal: 'IN, 1', pin_signal: CMP1_IN1/PTC3/LLWU_P7/LPSPI0_PCS1/LPUART1_RX/TPM0_CH2/CLKOUT/I2S0_TX_BCLK}
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
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC3 (pin 73) is configured as CMP1_IN1 */
    PORT_SetPinMux(PORTC, 3U, kPORT_PinDisabledOrAnalog);

    /* PORTC4 (pin 76) is disabled */
    PORT_SetPinMux(PORTC, 4U, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART2_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '95', peripheral: LPUART2, signal: RX, pin_signal: PTD2/LLWU_P13/LPSPI0_SOUT/LPUART2_RX/TPM0_CH2/FXIO0_D2}
  - {pin_num: '96', peripheral: LPUART2, signal: TX, pin_signal: PTD3/LPSPI0_SIN/LPUART2_TX/TPM0_CH3/FXIO0_D3}
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
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTD2 (pin 95) is configured as LPUART2_RX */
    PORT_SetPinMux(PORTD, 2U, kPORT_MuxAlt3);

    /* PORTD3 (pin 96) is configured as LPUART2_TX */
    PORT_SetPinMux(PORTD, 3U, kPORT_MuxAlt3);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPUART2_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '95', peripheral: n/a, signal: disabled, pin_signal: PTD2/LLWU_P13/LPSPI0_SOUT/LPUART2_RX/TPM0_CH2/FXIO0_D2}
  - {pin_num: '96', peripheral: n/a, signal: disabled, pin_signal: PTD3/LPSPI0_SIN/LPUART2_TX/TPM0_CH3/FXIO0_D3}
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
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTD2 (pin 95) is disabled */
    PORT_SetPinMux(PORTD, 2U, kPORT_PinDisabledOrAnalog);

    /* PORTD3 (pin 96) is disabled */
    PORT_SetPinMux(PORTD, 3U, kPORT_PinDisabledOrAnalog);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
