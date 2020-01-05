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
  - {pin_num: '73', peripheral: LPI2C0, signal: SDA, pin_signal: ADC1_SE0/PTA2/FTM3_CH0/LPI2C0_SDA/EWM_OUT_b/LPUART0_RX}
  - {pin_num: '72', peripheral: LPI2C0, signal: SCL, pin_signal: ADC1_SE1/PTA3/FTM3_CH1/LPI2C0_SCL/EWM_IN/LPUART0_TX}
  - {pin_num: '42', peripheral: LPI2C1, signal: SDA, pin_signal: PTD8/LPI2C1_SDA/FTM2_FLT2/FTM1_CH4}
  - {pin_num: '41', peripheral: LPI2C1, signal: SCL, pin_signal: ACMP1_IN5/PTD9/LPI2C1_SCL/FTM2_FLT3/FTM1_CH5}
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
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Clock Control: 0x01u */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTA2 (pin 73) is configured as LPI2C0_SDA */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt3);

    /* PORTA3 (pin 72) is configured as LPI2C0_SCL */
    PORT_SetPinMux(PORTA, 3U, kPORT_MuxAlt3);

    /* PORTB0 (pin 54) is configured as LPUART0_RX */
    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt2);

    /* PORTB1 (pin 53) is configured as LPUART0_TX */
    PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt2);

    /* PORTD8 (pin 42) is configured as LPI2C1_SDA */
    PORT_SetPinMux(PORTD, 8U, kPORT_MuxAlt2);

    /* PORTD9 (pin 41) is configured as LPI2C1_SCL */
    PORT_SetPinMux(PORTD, 9U, kPORT_MuxAlt2);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
