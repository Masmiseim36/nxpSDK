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
processor: MKV46F256xxx16
package_id: MKV46F256VLL16
mcu_data: ksdk2_0
processor_version: 0.0.8
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_xbara.h"
#include "pin_mux.h"



/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '1', peripheral: UART1, signal: TX, pin_signal: ADCB_CH6f/PTE0/UART1_TX/XBAROUT10/XBARIN11}
  - {pin_num: '2', peripheral: UART1, signal: RX, pin_signal: ADCB_CH7f/PTE1/LLWU_P0/UART1_RX/XBAROUT11/XBARIN7}
  - {pin_num: '71', peripheral: XBARA, signal: 'IN, 11', pin_signal: ADCB_CH7b/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FlexPWM_A3/XBARIN11}
  - {pin_num: '72', peripheral: XBARA, signal: 'IN, 6', pin_signal: ADCB_CH6c/CMP1_IN0/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FlexPWM_B3/XBARIN6}
  - {pin_num: '78', peripheral: XBARA, signal: 'IN, 3', pin_signal: CMP2_IN4/CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/XBARIN3/UART0_RX/XBAROUT6/I2C0_SCL}
  - {pin_num: '72', peripheral: ENC0, signal: 'PHASE, A', pin_signal: ADCB_CH6c/CMP1_IN0/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FlexPWM_B3/XBARIN6}
  - {pin_num: '71', peripheral: ENC0, signal: 'PHASE, B', pin_signal: ADCB_CH7b/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FlexPWM_A3/XBARIN11}
  - {pin_num: '78', peripheral: ENC0, signal: INDEX, pin_signal: CMP2_IN4/CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/XBARIN3/UART0_RX/XBAROUT6/I2C0_SCL}
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
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);
    /* XBARA Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_XbarA);

    /* PORTC1 (pin 71) is configured as XBARIN11 */
    PORT_SetPinMux(PORTC, 1U, kPORT_MuxAlt6);

    /* PORTC2 (pin 72) is configured as XBARIN6 */
    PORT_SetPinMux(PORTC, 2U, kPORT_MuxAlt6);

    /* PORTC6 (pin 78) is configured as XBARIN3 */
    PORT_SetPinMux(PORTC, 6U, kPORT_MuxAlt4);

    /* PORTE0 (pin 1) is configured as UART1_TX */
    PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt3);

    /* PORTE1 (pin 2) is configured as UART1_RX */
    PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt3);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART1TXSRC_MASK | SIM_SOPT5_UART1RXSRC_MASK)))

                  /* UART 1 transmit data source select: UART1_TX pin. */
                  | SIM_SOPT5_UART1TXSRC(SOPT5_UART1TXSRC_UART_TX)

                  /* UART 1 receive data source select: UART1_RX pin. */
                  | SIM_SOPT5_UART1RXSRC(SOPT5_UART1RXSRC_UART_RX));
    /* XBARIN6 input pin output assigned to XBARA_IN6 input is connected
     * to XBARA_OUT44 output assigned to ENC0 quadrature waveform phase A */
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn6, kXBARA_OutputEnc0PhA);
    /* XBARIN11 input pin output assigned to XBARA_IN11 input is connected
     * to XBARA_OUT45 output assigned to ENC0 quadrature waveform phase B */
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn11, kXBARA_OutputEnc0PhB);
    /* XBARIN3 input pin output assigned to XBARA_IN3 input is connected
     * to XBARA_OUT46 output assigned to ENC0 refresh/reload */
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn3, kXBARA_OutputEnc0Index);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
