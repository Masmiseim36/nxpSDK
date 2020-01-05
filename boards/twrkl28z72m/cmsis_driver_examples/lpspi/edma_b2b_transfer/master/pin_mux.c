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
BOARD_InitPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '35', peripheral: LPUART0, signal: RX, pin_signal: TSI0_CH2/PTA1/LPUART0_RX/TPM2_CH0}
  - {pin_num: '36', peripheral: LPUART0, signal: TX, pin_signal: TSI0_CH3/PTA2/LPUART0_TX/TPM2_CH1}
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
LPSPI0_InitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '93', peripheral: LPSPI0, signal: PCS0, pin_signal: PTD0/LLWU_P12/LPSPI0_PCS0/LPUART2_RTS_b/TPM0_CH0/FXIO0_D0}
  - {pin_num: '94', peripheral: LPSPI0, signal: SCK, pin_signal: ADC0_SE5b/PTD1/LPSPI0_SCK/LPUART2_CTS_b/TPM0_CH1/FXIO0_D1}
  - {pin_num: '95', peripheral: LPSPI0, signal: SOUT, pin_signal: PTD2/LLWU_P13/LPSPI0_SOUT/LPUART2_RX/TPM0_CH2/FXIO0_D2}
  - {pin_num: '96', peripheral: LPSPI0, signal: SIN, pin_signal: PTD3/LPSPI0_SIN/LPUART2_TX/TPM0_CH3/FXIO0_D3}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPSPI0_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPSPI0_InitPins(void)
{
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTD0 (pin 93) is configured as LPSPI0_PCS0 */
    PORT_SetPinMux(PORTD, 0U, kPORT_MuxAlt2);

    /* PORTD1 (pin 94) is configured as LPSPI0_SCK */
    PORT_SetPinMux(PORTD, 1U, kPORT_MuxAlt2);

    /* PORTD2 (pin 95) is configured as LPSPI0_SOUT */
    PORT_SetPinMux(PORTD, 2U, kPORT_MuxAlt2);

    /* PORTD3 (pin 96) is configured as LPSPI0_SIN */
    PORT_SetPinMux(PORTD, 3U, kPORT_MuxAlt2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
LPSPI0_DeinitPins:
- options: {coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '93', peripheral: n/a, signal: disabled, pin_signal: PTD0/LLWU_P12/LPSPI0_PCS0/LPUART2_RTS_b/TPM0_CH0/FXIO0_D0}
  - {pin_num: '94', peripheral: ADC0, signal: 'SE, 5b', pin_signal: ADC0_SE5b/PTD1/LPSPI0_SCK/LPUART2_CTS_b/TPM0_CH1/FXIO0_D1}
  - {pin_num: '95', peripheral: n/a, signal: disabled, pin_signal: PTD2/LLWU_P13/LPSPI0_SOUT/LPUART2_RX/TPM0_CH2/FXIO0_D2}
  - {pin_num: '96', peripheral: n/a, signal: disabled, pin_signal: PTD3/LPSPI0_SIN/LPUART2_TX/TPM0_CH3/FXIO0_D3}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : LPSPI0_DeinitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void LPSPI0_DeinitPins(void)
{
    /* Clock Gate Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTD0 (pin 93) is disabled */
    PORT_SetPinMux(PORTD, 0U, kPORT_PinDisabledOrAnalog);

    /* PORTD1 (pin 94) is configured as ADC0_SE5b */
    PORT_SetPinMux(PORTD, 1U, kPORT_PinDisabledOrAnalog);

    /* PORTD2 (pin 95) is disabled */
    PORT_SetPinMux(PORTD, 2U, kPORT_PinDisabledOrAnalog);

    /* PORTD3 (pin 96) is disabled */
    PORT_SetPinMux(PORTD, 3U, kPORT_PinDisabledOrAnalog);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
