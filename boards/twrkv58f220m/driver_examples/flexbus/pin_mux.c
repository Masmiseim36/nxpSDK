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
#include "pin_mux.h"



/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '81', peripheral: UART0, signal: RX, pin_signal: HSADC0B_CH2/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/FTM1_QD_PHA/UART0_RX/RMII0_MDIO/MII0_MDIO}
  - {pin_num: '82', peripheral: UART0, signal: TX, pin_signal: HSADC0B_CH3/PTB1/I2C0_SDA/FTM1_CH1/FTM0_FLT2/EWM_IN/FTM1_QD_PHB/UART0_TX/RMII0_MDC/MII0_MDC}
  - {pin_num: '133', peripheral: FB, signal: 'AD, 0', pin_signal: HSADC1A_CH9/PTD6/LLWU_P15/SPI0_PCS3/UART0_RX/FTM0_CH6/FTM1_CH0/FTM0_FLT0/SPI1_SOUT/FB_AD0}
  - {pin_num: '132', peripheral: FB, signal: 'AD, 1', pin_signal: HSADC1A_CH8/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FLEXPWM0_B2/EWM_OUT_b/SPI1_SCK/FB_AD1}
  - {pin_num: '131', peripheral: FB, signal: 'AD, 2', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FLEXPWM0_A2/EWM_IN/SPI1_PCS0/FB_AD2}
  - {pin_num: '130', peripheral: FB, signal: 'AD, 3', pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FTM0_CH3/FLEXPWM0_B1/I2C0_SDA/FB_AD3/FLEXPWM1_B1}
  - {pin_num: '129', peripheral: FB, signal: 'AD, 4', pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FTM0_CH2/FLEXPWM0_A1/I2C0_SCL/FB_AD4/FLEXPWM1_A1}
  - {pin_num: '115', peripheral: FB, signal: 'AD, 5', pin_signal: HSADC1B_CH13/PTC10/I2C1_SCL/FTM3_CH6/FLEXPWM1_A3/FB_AD5}
  - {pin_num: '114', peripheral: FB, signal: 'AD, 6', pin_signal: HSADC1B_CH12/CMP0_IN3/PTC9/FTM3_CH5/FLEXPWM1_B2/FB_AD6}
  - {pin_num: '113', peripheral: FB, signal: 'AD, 7', pin_signal: HSADC1B_CH11/CMP0_IN2/PTC8/FTM3_CH4/FLEXPWM1_A2/FB_AD7}
  - {pin_num: '112', peripheral: FB, signal: 'AD, 8', pin_signal: CMP3_IN4/CMP0_IN1/PTC7/SPI0_SIN/XB_IN4/UART0_TX/XB_OUT7/I2C0_SDA/FB_AD8}
  - {pin_num: '111', peripheral: FB, signal: 'AD, 9', pin_signal: CMP2_IN4/CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/XB_IN3/UART0_RX/XB_OUT6/I2C0_SCL/FB_AD9}
  - {pin_num: '110', peripheral: FB, signal: 'AD, 10', pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/XB_IN2/CMP0_OUT/FTM0_CH2/FB_AD10}
  - {pin_num: '105', peripheral: FB, signal: 'AD, 12', pin_signal: HSADC1B_CH10/CMP1_IN0/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FLEXPWM0_B3/XB_IN6/FB_AD12}
  - {pin_num: '104', peripheral: FB, signal: 'AD, 13', pin_signal: HSADC0B_CH9/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FLEXPWM0_A3/XB_IN11/FB_AD13}
  - {pin_num: '103', peripheral: FB, signal: 'AD, 14', pin_signal: HSADC0B_CH8/PTC0/SPI0_PCS4/PDB0_EXTRG/FTM0_FLT1/SPI0_PCS0/FB_AD14}
  - {pin_num: '97', peripheral: FB, signal: 'AD, 15', pin_signal: PTB18/CAN0_TX/FTM2_CH0/FTM3_CH2/FLEXPWM1_A1/FTM2_QD_PHA/FB_AD15}
  - {pin_num: '96', peripheral: FB, signal: 'AD, 16', pin_signal: PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/CAN0_RX/EWM_OUT_b/FB_AD16}
  - {pin_num: '95', peripheral: FB, signal: 'AD, 17', pin_signal: PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN2/CAN0_TX/EWM_IN/XB_IN5/FB_AD17}
  - {pin_num: '92', peripheral: FB, signal: 'AD, 18', pin_signal: HSADC0B_CH7/PTB11/SPI1_SCK/UART3_TX/FTM0_FLT2/FB_AD18}
  - {pin_num: '91', peripheral: FB, signal: 'AD, 19', pin_signal: HSADC0B_CH6/PTB10/SPI1_PCS0/UART3_RX/ENET0_1588_TMR3/FTM0_FLT1/FB_AD19}
  - {pin_num: '120', peripheral: FB, signal: 'AD, 24', pin_signal: PTC15/I2C1_SDA/I2C0_SDA/FLEXPWM1_B0/FB_AD24/UART4_TX}
  - {pin_num: '119', peripheral: FB, signal: 'AD, 25', pin_signal: PTC14/I2C1_SCL/I2C0_SCL/FLEXPWM1_A0/FB_AD25/UART4_RX}
  - {pin_num: '118', peripheral: FB, signal: 'AD, 26', pin_signal: PTC13/CAN2_RX/FTM_CLKIN1/FLEXPWM1_B1/FB_AD26/UART4_CTS_b}
  - {pin_num: '117', peripheral: FB, signal: 'AD, 27', pin_signal: PTC12/CAN2_TX/FTM_CLKIN0/FLEXPWM1_A1/FTM3_FLT0/SPI2_PCS1/FB_AD27/UART4_RTS_b}
  - {pin_num: '102', peripheral: FB, signal: 'AD, 28', pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FLEXPWM0_X3/CMP3_OUT/FB_AD28}
  - {pin_num: '101', peripheral: FB, signal: 'AD, 29', pin_signal: PTB22/SPI2_SOUT/FLEXPWM0_X2/CMP2_OUT/FB_AD29}
  - {pin_num: '100', peripheral: FB, signal: 'AD, 30', pin_signal: PTB21/SPI2_SCK/FLEXPWM0_X1/CMP1_OUT/FB_AD30}
  - {pin_num: '99', peripheral: FB, signal: 'AD, 31', pin_signal: PTB20/SPI2_PCS0/FLEXPWM0_X0/CMP0_OUT/FB_AD31}
  - {pin_num: '116', peripheral: FB, signal: RW, pin_signal: HSADC1B_CH14/PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/FLEXPWM1_B3/FB_RW_b}
  - {pin_num: '98', peripheral: FB, signal: OE, pin_signal: PTB19/CAN0_RX/FTM2_CH1/FTM3_CH3/FLEXPWM1_B1/FTM2_QD_PHB/FB_OE_b}
  - {pin_num: '128', peripheral: FB, signal: CS0, pin_signal: HSADC1A_CH11/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FTM0_CH1/FLEXPWM0_B0/FB_CS0_b/FLEXPWM1_B0}
  - {pin_num: '127', peripheral: FB, signal: ALE, pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FTM0_CH0/FLEXPWM0_A0/FB_ALE/FB_CS1_b/FB_TS_b/FLEXPWM1_A0}
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
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTB0 (pin 81) is configured as UART0_RX */
    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt7);

    /* PORTB1 (pin 82) is configured as UART0_TX */
    PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt7);

    /* PORTB10 (pin 91) is configured as FB_AD19 */
    PORT_SetPinMux(PORTB, 10U, kPORT_MuxAlt8);

    /* PORTB11 (pin 92) is configured as FB_AD18 */
    PORT_SetPinMux(PORTB, 11U, kPORT_MuxAlt8);

    /* PORTB16 (pin 95) is configured as FB_AD17 */
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt8);

    /* PORTB17 (pin 96) is configured as FB_AD16 */
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt8);

    /* PORTB18 (pin 97) is configured as FB_AD15 */
    PORT_SetPinMux(PORTB, 18U, kPORT_MuxAlt8);

    /* PORTB19 (pin 98) is configured as FB_OE_b */
    PORT_SetPinMux(PORTB, 19U, kPORT_MuxAlt8);

    /* PORTB20 (pin 99) is configured as FB_AD31 */
    PORT_SetPinMux(PORTB, 20U, kPORT_MuxAlt8);

    /* PORTB21 (pin 100) is configured as FB_AD30 */
    PORT_SetPinMux(PORTB, 21U, kPORT_MuxAlt8);

    /* PORTB22 (pin 101) is configured as FB_AD29 */
    PORT_SetPinMux(PORTB, 22U, kPORT_MuxAlt8);

    /* PORTB23 (pin 102) is configured as FB_AD28 */
    PORT_SetPinMux(PORTB, 23U, kPORT_MuxAlt8);

    /* PORTC0 (pin 103) is configured as FB_AD14 */
    PORT_SetPinMux(PORTC, 0U, kPORT_MuxAlt8);

    /* PORTC1 (pin 104) is configured as FB_AD13 */
    PORT_SetPinMux(PORTC, 1U, kPORT_MuxAlt8);

    /* PORTC10 (pin 115) is configured as FB_AD5 */
    PORT_SetPinMux(PORTC, 10U, kPORT_MuxAlt8);

    /* PORTC11 (pin 116) is configured as FB_RW_b */
    PORT_SetPinMux(PORTC, 11U, kPORT_MuxAlt8);

    /* PORTC12 (pin 117) is configured as FB_AD27 */
    PORT_SetPinMux(PORTC, 12U, kPORT_MuxAlt8);

    /* PORTC13 (pin 118) is configured as FB_AD26 */
    PORT_SetPinMux(PORTC, 13U, kPORT_MuxAlt8);

    /* PORTC14 (pin 119) is configured as FB_AD25 */
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt8);

    /* PORTC15 (pin 120) is configured as FB_AD24 */
    PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt8);

    /* PORTC2 (pin 105) is configured as FB_AD12 */
    PORT_SetPinMux(PORTC, 2U, kPORT_MuxAlt8);

    /* PORTC5 (pin 110) is configured as FB_AD10 */
    PORT_SetPinMux(PORTC, 5U, kPORT_MuxAlt8);

    /* PORTC6 (pin 111) is configured as FB_AD9 */
    PORT_SetPinMux(PORTC, 6U, kPORT_MuxAlt8);

    /* PORTC7 (pin 112) is configured as FB_AD8 */
    PORT_SetPinMux(PORTC, 7U, kPORT_MuxAlt8);

    /* PORTC8 (pin 113) is configured as FB_AD7 */
    PORT_SetPinMux(PORTC, 8U, kPORT_MuxAlt8);

    /* PORTC9 (pin 114) is configured as FB_AD6 */
    PORT_SetPinMux(PORTC, 9U, kPORT_MuxAlt8);

    /* PORTD0 (pin 127) is configured as FB_ALE */
    PORT_SetPinMux(PORTD, 0U, kPORT_MuxAlt8);

    /* PORTD1 (pin 128) is configured as FB_CS0_b */
    PORT_SetPinMux(PORTD, 1U, kPORT_MuxAlt8);

    /* PORTD2 (pin 129) is configured as FB_AD4 */
    PORT_SetPinMux(PORTD, 2U, kPORT_MuxAlt8);

    /* PORTD3 (pin 130) is configured as FB_AD3 */
    PORT_SetPinMux(PORTD, 3U, kPORT_MuxAlt8);

    /* PORTD4 (pin 131) is configured as FB_AD2 */
    PORT_SetPinMux(PORTD, 4U, kPORT_MuxAlt8);

    /* PORTD5 (pin 132) is configured as FB_AD1 */
    PORT_SetPinMux(PORTD, 5U, kPORT_MuxAlt8);

    /* PORTD6 (pin 133) is configured as FB_AD0 */
    PORT_SetPinMux(PORTD, 6U, kPORT_MuxAlt8);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK)))

                  /* UART 0 transmit data source select: UART0_TX pin. */
                  | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)

                  /* UART 0 receive data source select: UART0_RX pin. */
                  | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
