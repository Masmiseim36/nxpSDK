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
  - {pin_num: '81', peripheral: UART0, signal: RX, pin_signal: HSADC0B_CH2/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/FTM1_QD_PHA/UART0_RX/RMII0_MDIO/MII0_MDIO}
  - {pin_num: '82', peripheral: UART0, signal: TX, pin_signal: HSADC0B_CH3/PTB1/I2C0_SDA/FTM1_CH1/FTM0_FLT2/EWM_IN/FTM1_QD_PHB/UART0_TX/RMII0_MDC/MII0_MDC}
  - {pin_num: '41', peripheral: XBARA, signal: 'IN, 2', pin_signal: PTE22/FTM2_CH0/XB_IN2/FTM2_QD_PHA}
  - {pin_num: '42', peripheral: XBARA, signal: 'IN, 3', pin_signal: PTE23/FTM2_CH1/XB_IN3/FTM2_QD_PHB}
  - {pin_num: '26', peripheral: XBARA, signal: 'IN, 9', pin_signal: HSADC0A_CH9/HSADC1A_CH7/PTE21/XB_IN9/FTM1_CH1/UART0_RX/FTM1_QD_PHB}
  - {pin_num: '41', peripheral: ENC, signal: 'PHASE, A', pin_signal: PTE22/FTM2_CH0/XB_IN2/FTM2_QD_PHA}
  - {pin_num: '42', peripheral: ENC, signal: 'PHASE, B', pin_signal: PTE23/FTM2_CH1/XB_IN3/FTM2_QD_PHB}
  - {pin_num: '26', peripheral: ENC, signal: INDEX, pin_signal: HSADC0A_CH9/HSADC1A_CH7/PTE21/XB_IN9/FTM1_CH1/UART0_RX/FTM1_QD_PHB}
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
    /* XBARA Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_XbarA);

    /* PORTB0 (pin 81) is configured as UART0_RX */
    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt7);

    /* PORTB1 (pin 82) is configured as UART0_TX */
    PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt7);

    /* PORTE21 (pin 26) is configured as XB_IN9 */
    PORT_SetPinMux(PORTE, 21U, kPORT_MuxAlt2);

    /* PORTE22 (pin 41) is configured as XB_IN2 */
    PORT_SetPinMux(PORTE, 22U, kPORT_MuxAlt4);

    /* PORTE23 (pin 42) is configured as XB_IN3 */
    PORT_SetPinMux(PORTE, 23U, kPORT_MuxAlt4);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK)))

                  /* UART 0 transmit data source select: UART0_TX pin. */
                  | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)

                  /* UART 0 receive data source select: UART0_RX pin. */
                  | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX));
    /* XB_IN2 input pin output assigned to XBARA_IN2 input is connected
     * to XBARA_OUT44 output assigned to ENC quadrature waveform phase A */
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn2, kXBARA_OutputEncPhA);
    /* XB_IN3 input pin output assigned to XBARA_IN3 input is connected
     * to XBARA_OUT45 output assigned to ENC quadrature waveform phase B */
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn3, kXBARA_OutputEncPhB);
    /* XB_IN9 input pin output assigned to XBARA_IN9 input is connected
     * to XBARA_OUT46 output assigned to ENC refresh/reload */
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn9, kXBARA_OutputEncIndex);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
