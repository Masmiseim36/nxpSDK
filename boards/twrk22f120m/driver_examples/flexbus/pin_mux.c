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
processor: MK22FN512xxx12
package_id: MK22FN512VDC12
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
  - {pin_num: E4, peripheral: UART1, signal: TX, pin_signal: ADC1_SE4a/PTE0/CLKOUT32K/SPI1_PCS1/UART1_TX/I2C1_SDA/RTC_CLKOUT}
  - {pin_num: E3, peripheral: UART1, signal: RX, pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/I2C1_SCL/SPI1_SIN}
  - {pin_num: B2, peripheral: FB, signal: 'AD, 0', pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/UART0_RX/FTM0_CH6/FB_AD0/FTM0_FLT0/SPI1_SOUT}
  - {pin_num: A2, peripheral: FB, signal: 'AD, 1', pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/UART0_CTS_b/FTM0_CH5/FB_AD1/EWM_OUT_b/SPI1_SCK}
  - {pin_num: A3, peripheral: FB, signal: 'AD, 2', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FB_AD2/EWM_IN/SPI1_PCS0}
  - {pin_num: B3, peripheral: FB, signal: 'AD, 3', pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/LPUART0_TX/I2C0_SDA}
  - {pin_num: C3, peripheral: FB, signal: 'AD, 4', pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/LPUART0_RX/I2C0_SCL}
  - {pin_num: C6, peripheral: FB, signal: 'AD, 5', pin_signal: ADC1_SE6b/PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5}
  - {pin_num: D6, peripheral: FB, signal: 'AD, 6', pin_signal: ADC1_SE5b/CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/FTM2_FLT0}
  - {pin_num: A7, peripheral: FB, signal: 'AD, 7', pin_signal: ADC1_SE4b/CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7}
  - {pin_num: B7, peripheral: FB, signal: 'AD, 8', pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB_SOF_OUT/I2S0_RX_FS/FB_AD8}
  - {pin_num: C7, peripheral: FB, signal: 'AD, 9', pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/I2S0_MCLK}
  - {pin_num: D7, peripheral: FB, signal: 'AD, 10', pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/FB_AD10/CMP0_OUT/FTM0_CH2}
  - {pin_num: C8, peripheral: FB, signal: 'AD, 12', pin_signal: ADC0_SE4b/CMP1_IN0/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FB_AD12/I2S0_TX_FS/LPUART0_CTS_b}
  - {pin_num: D8, peripheral: FB, signal: 'AD, 13', pin_signal: ADC0_SE15/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FB_AD13/I2S0_TXD0/LPUART0_RTS_b}
  - {pin_num: B9, peripheral: FB, signal: 'AD, 14', pin_signal: ADC0_SE14/PTC0/SPI0_PCS4/PDB0_EXTRG/USB_SOF_OUT/FB_AD14}
  - {pin_num: D9, peripheral: FB, signal: 'AD, 15', pin_signal: PTB18/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/FTM2_QD_PHA}
  - {pin_num: E9, peripheral: FB, signal: 'AD, 16', pin_signal: PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/EWM_OUT_b}
  - {pin_num: B10, peripheral: FB, signal: 'AD, 17', pin_signal: PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/EWM_IN}
  - {pin_num: C10, peripheral: FB, signal: 'AD, 18', pin_signal: ADC1_SE15/PTB11/SPI1_SCK/LPUART0_TX/FB_AD18/FTM0_FLT2}
  - {pin_num: D10, peripheral: FB, signal: 'AD, 19', pin_signal: ADC1_SE14/PTB10/SPI1_PCS0/LPUART0_RX/FB_AD19/FTM0_FLT1}
  - {pin_num: B5, peripheral: FB, signal: 'AD, 24', pin_signal: PTC15/FB_AD24}
  - {pin_num: A5, peripheral: FB, signal: 'AD, 25', pin_signal: PTC14/FB_AD25}
  - {pin_num: A6, peripheral: FB, signal: 'AD, 26', pin_signal: PTC13/FB_AD26}
  - {pin_num: B6, peripheral: FB, signal: 'AD, 27', pin_signal: PTC12/FB_AD27/FTM3_FLT0}
  - {pin_num: E8, peripheral: FB, signal: 'AD, 28', pin_signal: PTB23/SPI0_PCS5/FB_AD28}
  - {pin_num: F8, peripheral: FB, signal: 'AD, 29', pin_signal: PTB22/FB_AD29}
  - {pin_num: F9, peripheral: FB, signal: 'AD, 30', pin_signal: PTB21/FB_AD30/CMP1_OUT}
  - {pin_num: F10, peripheral: FB, signal: 'AD, 31', pin_signal: PTB20/FB_AD31/CMP0_OUT}
  - {pin_num: C5, peripheral: FB, signal: RW, pin_signal: ADC1_SE7b/PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/FB_RW_b}
  - {pin_num: C9, peripheral: FB, signal: OE, pin_signal: PTB19/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB}
  - {pin_num: D3, peripheral: FB, signal: CS0, pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FB_CS0_b/LPUART0_CTS_b}
  - {pin_num: D4, peripheral: FB, signal: CS1_ALE_TS, pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b/LPUART0_RTS_b}
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
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTB10 (pin D10) is configured as FB_AD19 */
    PORT_SetPinMux(PORTB, 10U, kPORT_MuxAlt5);

    /* PORTB11 (pin C10) is configured as FB_AD18 */
    PORT_SetPinMux(PORTB, 11U, kPORT_MuxAlt5);

    /* PORTB16 (pin B10) is configured as FB_AD17 */
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt5);

    /* PORTB17 (pin E9) is configured as FB_AD16 */
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt5);

    /* PORTB18 (pin D9) is configured as FB_AD15 */
    PORT_SetPinMux(PORTB, 18U, kPORT_MuxAlt5);

    /* PORTB19 (pin C9) is configured as FB_OE_b */
    PORT_SetPinMux(PORTB, 19U, kPORT_MuxAlt5);

    /* PORTB20 (pin F10) is configured as FB_AD31 */
    PORT_SetPinMux(PORTB, 20U, kPORT_MuxAlt5);

    /* PORTB21 (pin F9) is configured as FB_AD30 */
    PORT_SetPinMux(PORTB, 21U, kPORT_MuxAlt5);

    /* PORTB22 (pin F8) is configured as FB_AD29 */
    PORT_SetPinMux(PORTB, 22U, kPORT_MuxAlt5);

    /* PORTB23 (pin E8) is configured as FB_AD28 */
    PORT_SetPinMux(PORTB, 23U, kPORT_MuxAlt5);

    /* PORTC0 (pin B9) is configured as FB_AD14 */
    PORT_SetPinMux(PORTC, 0U, kPORT_MuxAlt5);

    /* PORTC1 (pin D8) is configured as FB_AD13 */
    PORT_SetPinMux(PORTC, 1U, kPORT_MuxAlt5);

    /* PORTC10 (pin C6) is configured as FB_AD5 */
    PORT_SetPinMux(PORTC, 10U, kPORT_MuxAlt5);

    /* PORTC11 (pin C5) is configured as FB_RW_b */
    PORT_SetPinMux(PORTC, 11U, kPORT_MuxAlt5);

    /* PORTC12 (pin B6) is configured as FB_AD27 */
    PORT_SetPinMux(PORTC, 12U, kPORT_MuxAlt5);

    /* PORTC13 (pin A6) is configured as FB_AD26 */
    PORT_SetPinMux(PORTC, 13U, kPORT_MuxAlt5);

    /* PORTC14 (pin A5) is configured as FB_AD25 */
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt5);

    /* PORTC15 (pin B5) is configured as FB_AD24 */
    PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt5);

    /* PORTC2 (pin C8) is configured as FB_AD12 */
    PORT_SetPinMux(PORTC, 2U, kPORT_MuxAlt5);

    /* PORTC5 (pin D7) is configured as FB_AD10 */
    PORT_SetPinMux(PORTC, 5U, kPORT_MuxAlt5);

    /* PORTC6 (pin C7) is configured as FB_AD9 */
    PORT_SetPinMux(PORTC, 6U, kPORT_MuxAlt5);

    /* PORTC7 (pin B7) is configured as FB_AD8 */
    PORT_SetPinMux(PORTC, 7U, kPORT_MuxAlt5);

    /* PORTC8 (pin A7) is configured as FB_AD7 */
    PORT_SetPinMux(PORTC, 8U, kPORT_MuxAlt5);

    /* PORTC9 (pin D6) is configured as FB_AD6 */
    PORT_SetPinMux(PORTC, 9U, kPORT_MuxAlt5);

    /* PORTD0 (pin D4) is configured as FB_CS1_b */
    PORT_SetPinMux(PORTD, 0U, kPORT_MuxAlt5);

    /* PORTD1 (pin D3) is configured as FB_CS0_b */
    PORT_SetPinMux(PORTD, 1U, kPORT_MuxAlt5);

    /* PORTD2 (pin C3) is configured as FB_AD4 */
    PORT_SetPinMux(PORTD, 2U, kPORT_MuxAlt5);

    /* PORTD3 (pin B3) is configured as FB_AD3 */
    PORT_SetPinMux(PORTD, 3U, kPORT_MuxAlt5);

    /* PORTD4 (pin A3) is configured as FB_AD2 */
    PORT_SetPinMux(PORTD, 4U, kPORT_MuxAlt5);

    /* PORTD5 (pin A2) is configured as FB_AD1 */
    PORT_SetPinMux(PORTD, 5U, kPORT_MuxAlt5);

    /* PORTD6 (pin B2) is configured as FB_AD0 */
    PORT_SetPinMux(PORTD, 6U, kPORT_MuxAlt5);

    /* PORTE0 (pin E4) is configured as UART1_TX */
    PORT_SetPinMux(PORTE, 0U, kPORT_MuxAlt3);

    /* PORTE1 (pin E3) is configured as UART1_RX */
    PORT_SetPinMux(PORTE, 1U, kPORT_MuxAlt3);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART1TXSRC_MASK)))

                  /* UART 1 transmit data source select: UART1_TX pin. */
                  | SIM_SOPT5_UART1TXSRC(SOPT5_UART1TXSRC_UART_TX));
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
