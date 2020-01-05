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
processor: MK65FN2M0xxx18
package_id: MK65FN2M0VMI18
mcu_data: ksdk2_0
processor_version: 0.0.10
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
  - {pin_num: H3, peripheral: UART2, signal: TX, pin_signal: ADC0_SE4a/PTE16/SPI0_PCS0/UART2_TX/FTM_CLKIN0/FTM0_FLT3/TPM_CLKIN0}
  - {pin_num: F5, peripheral: UART2, signal: RX, pin_signal: ADC0_SE5a/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3/TPM_CLKIN1}
  - {pin_num: B5, peripheral: SDRAM, signal: A10, pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FB_AD2/SDRAM_A10/EWM_IN/SPI1_PCS0}
  - {pin_num: B4, peripheral: SDRAM, signal: A11, pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/SDRAM_A11/I2C0_SDA}
  - {pin_num: A4, peripheral: SDRAM, signal: A12, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/SDRAM_A12/I2C0_SCL}
  - {pin_num: A8, peripheral: SDRAM, signal: A13, pin_signal: ADC1_SE6b/PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5/SDRAM_A13}
  - {pin_num: C9, peripheral: SDRAM, signal: A14, pin_signal: ADC1_SE5b/CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/SDRAM_A14/FTM2_FLT0}
  - {pin_num: C10, peripheral: SDRAM, signal: A15, pin_signal: ADC1_SE4b/CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7/SDRAM_A15}
  - {pin_num: B10, peripheral: SDRAM, signal: A16, pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB0_SOF_OUT/I2S0_RX_FS/FB_AD8/SDRAM_A16}
  - {pin_num: A10, peripheral: SDRAM, signal: A17, pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/SDRAM_A17/I2S0_MCLK}
  - {pin_num: A11, peripheral: SDRAM, signal: A18, pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/FB_AD10/SDRAM_A18/CMP0_OUT/FTM0_CH2}
  - {pin_num: B11, peripheral: SDRAM, signal: A19, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT}
  - {pin_num: A13, peripheral: SDRAM, signal: A20, pin_signal: ADC0_SE4b/CMP1_IN0/TSI0_CH15/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FB_AD12/SDRAM_A20/I2S0_TX_FS}
  - {pin_num: B12, peripheral: SDRAM, signal: A21, pin_signal: ADC0_SE15/TSI0_CH14/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FB_AD13/SDRAM_A21/I2S0_TXD0}
  - {pin_num: B13, peripheral: SDRAM, signal: A22, pin_signal: ADC0_SE14/TSI0_CH13/PTC0/SPI0_PCS4/PDB0_EXTRG/USB0_SOF_OUT/FB_AD14/SDRAM_A22/I2S0_TXD1}
  - {pin_num: D12, peripheral: SDRAM, signal: A23, pin_signal: TSI0_CH11/PTB18/CAN0_TX/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/SDRAM_A23/FTM2_QD_PHA/TPM2_CH0}
  - {pin_num: C4, peripheral: SDRAM, signal: A9, pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FB_AD1/SDRAM_A9/EWM_OUT_b/SPI1_SCK}
  - {pin_num: G13, peripheral: SDRAM, signal: CAS, pin_signal: ADC0_SE8/ADC1_SE8/TSI0_CH0/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/RMII0_MDIO/MII0_MDIO/SDRAM_CAS_b/FTM1_QD_PHA/TPM1_CH0}
  - {pin_num: E5, peripheral: SDRAM, signal: CKE, pin_signal: PTD7/CMT_IRO/UART0_TX/FTM0_CH7/SDRAM_CKE/FTM0_FLT1/SPI1_SIN}
  - {pin_num: G10, peripheral: SDRAM, signal: CS0, pin_signal: ADC0_SE13/TSI0_CH8/PTB3/I2C0_SDA/UART0_CTS_b/UART0_COL_b/ENET0_1588_TMR1/SDRAM_CS0_b/FTM0_FLT0}
  - {pin_num: H9, peripheral: SDRAM, signal: CS1, pin_signal: ADC1_SE10/PTB4/ENET0_1588_TMR2/SDRAM_CS1_b/FTM1_FLT0}
  - {pin_num: B6, peripheral: SDRAM, signal: D0, pin_signal: PTC29/I2C3_SCL/ENET0_1588_TMR3/FB_A0/SDRAM_D0}
  - {pin_num: C6, peripheral: SDRAM, signal: D1, pin_signal: PTC28/I2C3_SDA/ENET0_1588_TMR2/FB_A1/SDRAM_D1}
  - {pin_num: J9, peripheral: SDRAM, signal: D10, pin_signal: PTA31/CAN0_RX/FB_A10/SDRAM_D10}
  - {pin_num: H10, peripheral: SDRAM, signal: D11, pin_signal: PTA30/CAN0_TX/FB_A11/SDRAM_D11}
  - {pin_num: H13, peripheral: SDRAM, signal: D12, pin_signal: PTA27/MII0_CRS/FB_A12/SDRAM_D12/FB_A26}
  - {pin_num: J10, peripheral: SDRAM, signal: D13, pin_signal: PTA26/MII0_TXD3/FB_A13/SDRAM_D13/FB_A27}
  - {pin_num: J11, peripheral: SDRAM, signal: D14, pin_signal: CMP3_IN5/PTA25/MII0_TXCLK/FB_A14/SDRAM_D14/FB_A28}
  - {pin_num: K11, peripheral: SDRAM, signal: D15, pin_signal: CMP3_IN4/PTA24/MII0_TXD2/FB_A15/SDRAM_D15/FB_A29}
  - {pin_num: D13, peripheral: SDRAM, signal: D16, pin_signal: TSI0_CH10/PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/SDRAM_D16/EWM_OUT_b/TPM_CLKIN1}
  - {pin_num: F8, peripheral: SDRAM, signal: D17, pin_signal: TSI0_CH9/PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/SDRAM_D17/EWM_IN/TPM_CLKIN0}
  - {pin_num: E13, peripheral: SDRAM, signal: D18, pin_signal: ADC1_SE15/PTB11/SPI1_SCK/UART3_TX/FB_AD18/SDRAM_D18/FTM0_FLT2}
  - {pin_num: G9, peripheral: SDRAM, signal: D19, pin_signal: ADC1_SE14/PTB10/SPI1_PCS0/UART3_RX/FB_AD19/SDRAM_D19/FTM0_FLT1}
  - {pin_num: D6, peripheral: SDRAM, signal: D2, pin_signal: PTC27/LPUART0_RTS_b/ENET0_1588_TMR1/FB_A2/SDRAM_D2}
  - {pin_num: F9, peripheral: SDRAM, signal: D20, pin_signal: PTB9/SPI1_PCS1/UART3_CTS_b/FB_AD20/SDRAM_D20}
  - {pin_num: F10, peripheral: SDRAM, signal: D21, pin_signal: PTB8/UART3_RTS_b/FB_AD21/SDRAM_D21}
  - {pin_num: F11, peripheral: SDRAM, signal: D22, pin_signal: ADC1_SE13/PTB7/FB_AD22/SDRAM_D22}
  - {pin_num: F12, peripheral: SDRAM, signal: D23, pin_signal: ADC1_SE12/PTB6/FB_AD23/SDRAM_D23}
  - {pin_num: D8, peripheral: SDRAM, signal: D24, pin_signal: PTC15/UART4_TX/FB_AD24/SDRAM_D24}
  - {pin_num: C8, peripheral: SDRAM, signal: D25, pin_signal: PTC14/UART4_RX/FB_AD25/SDRAM_D25}
  - {pin_num: B8, peripheral: SDRAM, signal: D26, pin_signal: PTC13/UART4_CTS_b/FTM_CLKIN1/FB_AD26/SDRAM_D26/TPM_CLKIN1}
  - {pin_num: B9, peripheral: SDRAM, signal: D27, pin_signal: PTC12/UART4_RTS_b/FTM_CLKIN0/FB_AD27/SDRAM_D27/FTM3_FLT0/TPM_CLKIN0}
  - {pin_num: C12, peripheral: SDRAM, signal: D28, pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28/SDRAM_D28/CMP3_OUT}
  - {pin_num: C13, peripheral: SDRAM, signal: D29, pin_signal: PTB22/SPI2_SOUT/FB_AD29/SDRAM_D29/CMP2_OUT}
  - {pin_num: E6, peripheral: SDRAM, signal: D3, pin_signal: PTC26/LPUART0_CTS_b/ENET0_1588_TMR0/FB_A3/SDRAM_D3}
  - {pin_num: D9, peripheral: SDRAM, signal: D30, pin_signal: PTB21/SPI2_SCK/FB_AD30/SDRAM_D30/CMP1_OUT}
  - {pin_num: D10, peripheral: SDRAM, signal: D31, pin_signal: PTB20/SPI2_PCS0/FB_AD31/SDRAM_D31/CMP0_OUT}
  - {pin_num: A7, peripheral: SDRAM, signal: D4, pin_signal: PTC25/LPUART0_RX/FB_A4/SDRAM_D4}
  - {pin_num: B7, peripheral: SDRAM, signal: D5, pin_signal: PTC24/LPUART0_TX/FB_A5/SDRAM_D5}
  - {pin_num: E9, peripheral: SDRAM, signal: D6, pin_signal: PTB15/CAN1_RX/FB_A6/SDRAM_D6}
  - {pin_num: E10, peripheral: SDRAM, signal: D7, pin_signal: PTB14/CAN1_TX/FB_A7/SDRAM_D7}
  - {pin_num: E11, peripheral: SDRAM, signal: D8, pin_signal: PTB13/UART3_CTS_b/FTM1_CH1/FTM0_CH5/FB_A8/SDRAM_D8/FTM1_QD_PHB/TPM1_CH1}
  - {pin_num: E12, peripheral: SDRAM, signal: D9, pin_signal: PTB12/UART3_RTS_b/FTM1_CH0/FTM0_CH4/FB_A9/SDRAM_D9/FTM1_QD_PHA/TPM1_CH0}
  - {pin_num: C7, peripheral: SDRAM, signal: DQM0, pin_signal: PTC19/UART3_CTS_b/ENET0_1588_TMR3/FB_CS3_b/FB_BE7_0_BLS31_24_b/SDRAM_DQM0/FB_TA_b}
  - {pin_num: D7, peripheral: SDRAM, signal: DQM1, pin_signal: PTC18/UART3_RTS_b/ENET0_1588_TMR2/FB_TBST_b/FB_CS2_b/FB_BE15_8_BLS23_16_b/SDRAM_DQM1}
  - {pin_num: E8, peripheral: SDRAM, signal: DQM2, pin_signal: PTC16/CAN1_RX/UART3_RX/ENET0_1588_TMR0/FB_CS5_b/FB_TSIZ1/FB_BE23_16_BLS15_8_b/SDRAM_DQM2}
  - {pin_num: E7, peripheral: SDRAM, signal: DQM3, pin_signal: PTC17/CAN1_TX/UART3_TX/ENET0_1588_TMR1/FB_CS4_b/FB_TSIZ0/FB_BE31_24_BLS7_0_b/SDRAM_DQM3}
  - {pin_num: G12, peripheral: SDRAM, signal: RAS, pin_signal: ADC0_SE9/ADC1_SE9/TSI0_CH6/PTB1/I2C0_SDA/FTM1_CH1/RMII0_MDC/MII0_MDC/SDRAM_RAS_b/FTM1_QD_PHB/TPM1_CH1}
  - {pin_num: G11, peripheral: SDRAM, signal: WE, pin_signal: ADC0_SE12/TSI0_CH7/PTB2/I2C0_SCL/UART0_RTS_b/ENET0_1588_TMR0/SDRAM_WE/FTM0_FLT3}
  - {pin_num: M10, peripheral: SDRAM, signal: CLKOUT, pin_signal: PTA6/FTM0_CH3/CLKOUT/TRACE_CLKOUT}
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
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTA24 (pin K11) is configured as SDRAM_D15 */
    PORT_SetPinMux(PORTA, 24U, kPORT_MuxAlt5);

    /* PORTA25 (pin J11) is configured as SDRAM_D14 */
    PORT_SetPinMux(PORTA, 25U, kPORT_MuxAlt5);

    /* PORTA26 (pin J10) is configured as SDRAM_D13 */
    PORT_SetPinMux(PORTA, 26U, kPORT_MuxAlt5);

    /* PORTA27 (pin H13) is configured as SDRAM_D12 */
    PORT_SetPinMux(PORTA, 27U, kPORT_MuxAlt5);

    /* PORTA30 (pin H10) is configured as SDRAM_D11 */
    PORT_SetPinMux(PORTA, 30U, kPORT_MuxAlt5);

    /* PORTA31 (pin J9) is configured as SDRAM_D10 */
    PORT_SetPinMux(PORTA, 31U, kPORT_MuxAlt5);

    /* PORTA6 (pin M10) is configured as CLKOUT */
    PORT_SetPinMux(PORTA, 6U, kPORT_MuxAlt5);

    /* PORTB0 (pin G13) is configured as SDRAM_CAS_b */
    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt5);

    /* PORTB1 (pin G12) is configured as SDRAM_RAS_b */
    PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt5);

    /* PORTB10 (pin G9) is configured as SDRAM_D19 */
    PORT_SetPinMux(PORTB, 10U, kPORT_MuxAlt5);

    /* PORTB11 (pin E13) is configured as SDRAM_D18 */
    PORT_SetPinMux(PORTB, 11U, kPORT_MuxAlt5);

    /* PORTB12 (pin E12) is configured as SDRAM_D9 */
    PORT_SetPinMux(PORTB, 12U, kPORT_MuxAlt5);

    /* PORTB13 (pin E11) is configured as SDRAM_D8 */
    PORT_SetPinMux(PORTB, 13U, kPORT_MuxAlt5);

    /* PORTB14 (pin E10) is configured as SDRAM_D7 */
    PORT_SetPinMux(PORTB, 14U, kPORT_MuxAlt5);

    /* PORTB15 (pin E9) is configured as SDRAM_D6 */
    PORT_SetPinMux(PORTB, 15U, kPORT_MuxAlt5);

    /* PORTB16 (pin F8) is configured as SDRAM_D17 */
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt5);

    /* PORTB17 (pin D13) is configured as SDRAM_D16 */
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt5);

    /* PORTB18 (pin D12) is configured as SDRAM_A23 */
    PORT_SetPinMux(PORTB, 18U, kPORT_MuxAlt5);

    /* PORTB2 (pin G11) is configured as SDRAM_WE */
    PORT_SetPinMux(PORTB, 2U, kPORT_MuxAlt5);

    /* PORTB20 (pin D10) is configured as SDRAM_D31 */
    PORT_SetPinMux(PORTB, 20U, kPORT_MuxAlt5);

    /* PORTB21 (pin D9) is configured as SDRAM_D30 */
    PORT_SetPinMux(PORTB, 21U, kPORT_MuxAlt5);

    /* PORTB22 (pin C13) is configured as SDRAM_D29 */
    PORT_SetPinMux(PORTB, 22U, kPORT_MuxAlt5);

    /* PORTB23 (pin C12) is configured as SDRAM_D28 */
    PORT_SetPinMux(PORTB, 23U, kPORT_MuxAlt5);

    /* PORTB3 (pin G10) is configured as SDRAM_CS0_b */
    PORT_SetPinMux(PORTB, 3U, kPORT_MuxAlt5);

    /* PORTB4 (pin H9) is configured as SDRAM_CS1_b */
    PORT_SetPinMux(PORTB, 4U, kPORT_MuxAlt5);

    /* PORTB6 (pin F12) is configured as SDRAM_D23 */
    PORT_SetPinMux(PORTB, 6U, kPORT_MuxAlt5);

    /* PORTB7 (pin F11) is configured as SDRAM_D22 */
    PORT_SetPinMux(PORTB, 7U, kPORT_MuxAlt5);

    /* PORTB8 (pin F10) is configured as SDRAM_D21 */
    PORT_SetPinMux(PORTB, 8U, kPORT_MuxAlt5);

    /* PORTB9 (pin F9) is configured as SDRAM_D20 */
    PORT_SetPinMux(PORTB, 9U, kPORT_MuxAlt5);

    /* PORTC0 (pin B13) is configured as SDRAM_A22 */
    PORT_SetPinMux(PORTC, 0U, kPORT_MuxAlt5);

    /* PORTC1 (pin B12) is configured as SDRAM_A21 */
    PORT_SetPinMux(PORTC, 1U, kPORT_MuxAlt5);

    /* PORTC10 (pin A8) is configured as SDRAM_A13 */
    PORT_SetPinMux(PORTC, 10U, kPORT_MuxAlt5);

    /* PORTC12 (pin B9) is configured as SDRAM_D27 */
    PORT_SetPinMux(PORTC, 12U, kPORT_MuxAlt5);

    /* PORTC13 (pin B8) is configured as SDRAM_D26 */
    PORT_SetPinMux(PORTC, 13U, kPORT_MuxAlt5);

    /* PORTC14 (pin C8) is configured as SDRAM_D25 */
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt5);

    /* PORTC15 (pin D8) is configured as SDRAM_D24 */
    PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt5);

    /* PORTC16 (pin E8) is configured as SDRAM_DQM2 */
    PORT_SetPinMux(PORTC, 16U, kPORT_MuxAlt5);

    /* PORTC17 (pin E7) is configured as SDRAM_DQM3 */
    PORT_SetPinMux(PORTC, 17U, kPORT_MuxAlt5);

    /* PORTC18 (pin D7) is configured as SDRAM_DQM1 */
    PORT_SetPinMux(PORTC, 18U, kPORT_MuxAlt5);

    /* PORTC19 (pin C7) is configured as SDRAM_DQM0 */
    PORT_SetPinMux(PORTC, 19U, kPORT_MuxAlt5);

    /* PORTC2 (pin A13) is configured as SDRAM_A20 */
    PORT_SetPinMux(PORTC, 2U, kPORT_MuxAlt5);

    /* PORTC24 (pin B7) is configured as SDRAM_D5 */
    PORT_SetPinMux(PORTC, 24U, kPORT_MuxAlt5);

    /* PORTC25 (pin A7) is configured as SDRAM_D4 */
    PORT_SetPinMux(PORTC, 25U, kPORT_MuxAlt5);

    /* PORTC26 (pin E6) is configured as SDRAM_D3 */
    PORT_SetPinMux(PORTC, 26U, kPORT_MuxAlt5);

    /* PORTC27 (pin D6) is configured as SDRAM_D2 */
    PORT_SetPinMux(PORTC, 27U, kPORT_MuxAlt5);

    /* PORTC28 (pin C6) is configured as SDRAM_D1 */
    PORT_SetPinMux(PORTC, 28U, kPORT_MuxAlt5);

    /* PORTC29 (pin B6) is configured as SDRAM_D0 */
    PORT_SetPinMux(PORTC, 29U, kPORT_MuxAlt5);

    /* PORTC4 (pin B11) is configured as SDRAM_A19 */
    PORT_SetPinMux(PORTC, 4U, kPORT_MuxAlt5);

    /* PORTC5 (pin A11) is configured as SDRAM_A18 */
    PORT_SetPinMux(PORTC, 5U, kPORT_MuxAlt5);

    /* PORTC6 (pin A10) is configured as SDRAM_A17 */
    PORT_SetPinMux(PORTC, 6U, kPORT_MuxAlt5);

    /* PORTC7 (pin B10) is configured as SDRAM_A16 */
    PORT_SetPinMux(PORTC, 7U, kPORT_MuxAlt5);

    /* PORTC8 (pin C10) is configured as SDRAM_A15 */
    PORT_SetPinMux(PORTC, 8U, kPORT_MuxAlt5);

    /* PORTC9 (pin C9) is configured as SDRAM_A14 */
    PORT_SetPinMux(PORTC, 9U, kPORT_MuxAlt5);

    /* PORTD2 (pin A4) is configured as SDRAM_A12 */
    PORT_SetPinMux(PORTD, 2U, kPORT_MuxAlt5);

    /* PORTD3 (pin B4) is configured as SDRAM_A11 */
    PORT_SetPinMux(PORTD, 3U, kPORT_MuxAlt5);

    /* PORTD4 (pin B5) is configured as SDRAM_A10 */
    PORT_SetPinMux(PORTD, 4U, kPORT_MuxAlt5);

    /* PORTD5 (pin C4) is configured as SDRAM_A9 */
    PORT_SetPinMux(PORTD, 5U, kPORT_MuxAlt5);

    /* PORTD7 (pin E5) is configured as SDRAM_CKE */
    PORT_SetPinMux(PORTD, 7U, kPORT_MuxAlt5);

    /* PORTE16 (pin H3) is configured as UART2_TX */
    PORT_SetPinMux(PORTE, 16U, kPORT_MuxAlt3);

    /* PORTE17 (pin F5) is configured as UART2_RX */
    PORT_SetPinMux(PORTE, 17U, kPORT_MuxAlt3);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
