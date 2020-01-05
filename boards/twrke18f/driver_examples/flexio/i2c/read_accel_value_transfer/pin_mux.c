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
  - {pin_num: '4', peripheral: FLEXIO, signal: 'D, 0', pin_signal: ADC2_SE0/PTD0/FTM0_CH2/LPSPI1_SCK/FTM2_CH0/FXIO_D0/TRGMUX_OUT1}
  - {pin_num: '3', peripheral: FLEXIO, signal: 'D, 1', pin_signal: ADC2_SE1/PTD1/FTM0_CH3/LPSPI1_SIN/FTM2_CH1/FXIO_D1/TRGMUX_OUT2}
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

    /* PORTB0 (pin 54) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt2);

    /* PORTB1 (pin 53) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt2);

    /* PORTD0 (pin 4) is configured as FXIO_D0 */
    PORT_SetPinMux(PORTD, 0U, kPORT_MuxAlt6);

    /* PORTD1 (pin 3) is configured as FXIO_D1 */
    PORT_SetPinMux(PORTD, 1U, kPORT_MuxAlt6);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
