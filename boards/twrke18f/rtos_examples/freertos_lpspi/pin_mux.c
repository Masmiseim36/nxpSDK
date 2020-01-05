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
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '54', peripheral: LPUART0, signal: RX, pin_signal: ADC0_SE4/PTB0/LPUART0_RX/LPSPI0_PCS0/LPTMR0_ALT3/PWT_IN3}
  - {pin_num: '53', peripheral: LPUART0, signal: TX, pin_signal: ADC0_SE5/PTB1/LPUART0_TX/LPSPI0_SOUT/TCLK0}
  - {pin_num: '4', peripheral: LPSPI1, signal: SCK, pin_signal: ADC2_SE0/PTD0/FTM0_CH2/LPSPI1_SCK/FTM2_CH0/FXIO_D0/TRGMUX_OUT1}
  - {pin_num: '3', peripheral: LPSPI1, signal: SIN, pin_signal: ADC2_SE1/PTD1/FTM0_CH3/LPSPI1_SIN/FTM2_CH1/FXIO_D1/TRGMUX_OUT2}
  - {pin_num: '71', peripheral: LPSPI1, signal: SOUT, pin_signal: ADC1_SE2/PTD2/FTM3_CH4/LPSPI1_SOUT/FXIO_D4/TRGMUX_IN5}
  - {pin_num: '70', peripheral: LPSPI1, signal: PCS0, pin_signal: ADC1_SE3/PTD3/FTM3_CH5/LPSPI1_PCS0/FXIO_D5/TRGMUX_IN4/NMI_b}
  - {pin_num: '94', peripheral: LPSPI0, signal: SCK, pin_signal: ADC2_SE7/PTE0/LPSPI0_SCK/TCLK1/LPI2C1_SDA/FTM1_FLT2}
  - {pin_num: '93', peripheral: LPSPI0, signal: SIN, pin_signal: ADC2_SE6/PTE1/LPSPI0_SIN/LPI2C0_HREQ/LPI2C1_SCL/FTM1_FLT1}
  - {pin_num: '85', peripheral: LPSPI0, signal: SOUT, pin_signal: ADC1_SE10/PTE2/LPSPI0_SOUT/LPTMR0_ALT3/FTM3_CH6/PWT_IN3/LPUART1_CTS}
  - {pin_num: '84', peripheral: LPSPI0, signal: PCS2, pin_signal: ADC1_SE11/ACMP0_IN6/PTE6/LPSPI0_PCS2/FTM3_CH7/LPUART1_RTS}
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
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTB0 (pin 54) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt2);

    /* PORTB1 (pin 53) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt2);

    /* PORTD0 (pin 4) is configured as LPSPI1_SCK */
    PORT_SetPinMux(PORTD, 0U, kPORT_MuxAlt3);

    /* PORTD1 (pin 3) is configured as LPSPI1_SIN */
    PORT_SetPinMux(PORTD, 1U, kPORT_MuxAlt3);

    /* PORTD2 (pin 71) is configured as LPSPI1_SOUT */
    PORT_SetPinMux(PORTD, 2U, kPORT_MuxAlt3);

    /* PORTD3 (pin 70) is configured as LPSPI1_PCS0 */
    PORT_SetPinMux(PORTD, 3U, kPORT_MuxAlt3);

    /* PORTE0 (pin 94) is configured as LPSPI0_SCK */
    PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt2);

    /* PORTE1 (pin 93) is configured as LPSPI0_SIN */
    PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt2);

    /* PORTE2 (pin 85) is configured as LPSPI0_SOUT */
    PORT_SetPinMux(PORTE, 2U, kPORT_MuxAlt2);

    /* PORTE6 (pin 84) is configured as LPSPI0_PCS2 */
    PORT_SetPinMux(PORTE, 6U, kPORT_MuxAlt2);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
