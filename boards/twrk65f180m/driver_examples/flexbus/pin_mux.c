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
  - {pin_num: A4, peripheral: FB, signal: 'AD, 4', pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/SDRAM_A12/I2C0_SCL}
  - {pin_num: B4, peripheral: FB, signal: 'AD, 3', pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/SDRAM_A11/I2C0_SDA}
  - {pin_num: B5, peripheral: FB, signal: 'AD, 2', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FB_AD2/SDRAM_A10/EWM_IN/SPI1_PCS0}
  - {pin_num: C4, peripheral: FB, signal: 'AD, 1', pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FB_AD1/SDRAM_A9/EWM_OUT_b/SPI1_SCK}
  - {pin_num: C5, peripheral: FB, signal: 'AD, 0', pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/UART0_RX/FTM0_CH6/FB_AD0/FTM0_FLT0/SPI1_SOUT}
  - {pin_num: A11, peripheral: FB, signal: 'AD, 10', pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/FB_AD10/SDRAM_A18/CMP0_OUT/FTM0_CH2}
  - {pin_num: A10, peripheral: FB, signal: 'AD, 9', pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/SDRAM_A17/I2S0_MCLK}
  - {pin_num: B10, peripheral: FB, signal: 'AD, 8', pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB0_SOF_OUT/I2S0_RX_FS/FB_AD8/SDRAM_A16}
  - {pin_num: C10, peripheral: FB, signal: 'AD, 7', pin_signal: ADC1_SE4b/CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7/SDRAM_A15}
  - {pin_num: C9, peripheral: FB, signal: 'AD, 6', pin_signal: ADC1_SE5b/CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/SDRAM_A14/FTM2_FLT0}
  - {pin_num: A8, peripheral: FB, signal: 'AD, 5', pin_signal: ADC1_SE6b/PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5/SDRAM_A13}
  - {pin_num: B13, peripheral: FB, signal: 'AD, 14', pin_signal: ADC0_SE14/TSI0_CH13/PTC0/SPI0_PCS4/PDB0_EXTRG/USB0_SOF_OUT/FB_AD14/SDRAM_A22/I2S0_TXD1}
  - {pin_num: B12, peripheral: FB, signal: 'AD, 13', pin_signal: ADC0_SE15/TSI0_CH14/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FB_AD13/SDRAM_A21/I2S0_TXD0}
  - {pin_num: A13, peripheral: FB, signal: 'AD, 12', pin_signal: ADC0_SE4b/CMP1_IN0/TSI0_CH15/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FB_AD12/SDRAM_A20/I2S0_TX_FS}
  - {pin_num: G9, peripheral: FB, signal: 'AD, 19', pin_signal: ADC1_SE14/PTB10/SPI1_PCS0/UART3_RX/FB_AD19/SDRAM_D19/FTM0_FLT1}
  - {pin_num: E13, peripheral: FB, signal: 'AD, 18', pin_signal: ADC1_SE15/PTB11/SPI1_SCK/UART3_TX/FB_AD18/SDRAM_D18/FTM0_FLT2}
  - {pin_num: F8, peripheral: FB, signal: 'AD, 17', pin_signal: TSI0_CH9/PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/SDRAM_D17/EWM_IN/TPM_CLKIN0}
  - {pin_num: D13, peripheral: FB, signal: 'AD, 16', pin_signal: TSI0_CH10/PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/SDRAM_D16/EWM_OUT_b/TPM_CLKIN1}
  - {pin_num: D12, peripheral: FB, signal: 'AD, 15', pin_signal: TSI0_CH11/PTB18/CAN0_TX/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/SDRAM_A23/FTM2_QD_PHA/TPM2_CH0}
  - {pin_num: D10, peripheral: FB, signal: 'AD, 31', pin_signal: PTB20/SPI2_PCS0/FB_AD31/SDRAM_D31/CMP0_OUT}
  - {pin_num: D9, peripheral: FB, signal: 'AD, 30', pin_signal: PTB21/SPI2_SCK/FB_AD30/SDRAM_D30/CMP1_OUT}
  - {pin_num: C13, peripheral: FB, signal: 'AD, 29', pin_signal: PTB22/SPI2_SOUT/FB_AD29/SDRAM_D29/CMP2_OUT}
  - {pin_num: C12, peripheral: FB, signal: 'AD, 28', pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28/SDRAM_D28/CMP3_OUT}
  - {pin_num: B9, peripheral: FB, signal: 'AD, 27', pin_signal: PTC12/UART4_RTS_b/FTM_CLKIN0/FB_AD27/SDRAM_D27/FTM3_FLT0/TPM_CLKIN0}
  - {pin_num: B8, peripheral: FB, signal: 'AD, 26', pin_signal: PTC13/UART4_CTS_b/FTM_CLKIN1/FB_AD26/SDRAM_D26/TPM_CLKIN1}
  - {pin_num: C8, peripheral: FB, signal: 'AD, 25', pin_signal: PTC14/UART4_RX/FB_AD25/SDRAM_D25}
  - {pin_num: D8, peripheral: FB, signal: 'AD, 24', pin_signal: PTC15/UART4_TX/FB_AD24/SDRAM_D24}
  - {pin_num: A9, peripheral: FB, signal: RW, pin_signal: ADC1_SE7b/PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/I2S0_RXD1/FB_RW_b}
  - {pin_num: D11, peripheral: FB, signal: OE, pin_signal: TSI0_CH12/PTB19/CAN0_RX/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB/TPM2_CH1}
  - {pin_num: A5, peripheral: FB, signal: CS0, pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FB_CS0_b}
  - {pin_num: A6, peripheral: FB, signal: CS1_ALE_TS, pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b}
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

    /* PORTB10 (pin G9) is configured as FB_AD19 */
    PORT_SetPinMux(PORTB, 10U, kPORT_MuxAlt5);

    /* PORTB11 (pin E13) is configured as FB_AD18 */
    PORT_SetPinMux(PORTB, 11U, kPORT_MuxAlt5);

    /* PORTB16 (pin F8) is configured as FB_AD17 */
    PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt5);

    /* PORTB17 (pin D13) is configured as FB_AD16 */
    PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt5);

    /* PORTB18 (pin D12) is configured as FB_AD15 */
    PORT_SetPinMux(PORTB, 18U, kPORT_MuxAlt5);

    /* PORTB19 (pin D11) is configured as FB_OE_b */
    PORT_SetPinMux(PORTB, 19U, kPORT_MuxAlt5);

    /* PORTB20 (pin D10) is configured as FB_AD31 */
    PORT_SetPinMux(PORTB, 20U, kPORT_MuxAlt5);

    /* PORTB21 (pin D9) is configured as FB_AD30 */
    PORT_SetPinMux(PORTB, 21U, kPORT_MuxAlt5);

    /* PORTB22 (pin C13) is configured as FB_AD29 */
    PORT_SetPinMux(PORTB, 22U, kPORT_MuxAlt5);

    /* PORTB23 (pin C12) is configured as FB_AD28 */
    PORT_SetPinMux(PORTB, 23U, kPORT_MuxAlt5);

    /* PORTC0 (pin B13) is configured as FB_AD14 */
    PORT_SetPinMux(PORTC, 0U, kPORT_MuxAlt5);

    /* PORTC1 (pin B12) is configured as FB_AD13 */
    PORT_SetPinMux(PORTC, 1U, kPORT_MuxAlt5);

    /* PORTC10 (pin A8) is configured as FB_AD5 */
    PORT_SetPinMux(PORTC, 10U, kPORT_MuxAlt5);

    /* PORTC11 (pin A9) is configured as FB_RW_b */
    PORT_SetPinMux(PORTC, 11U, kPORT_MuxAlt5);

    /* PORTC12 (pin B9) is configured as FB_AD27 */
    PORT_SetPinMux(PORTC, 12U, kPORT_MuxAlt5);

    /* PORTC13 (pin B8) is configured as FB_AD26 */
    PORT_SetPinMux(PORTC, 13U, kPORT_MuxAlt5);

    /* PORTC14 (pin C8) is configured as FB_AD25 */
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt5);

    /* PORTC15 (pin D8) is configured as FB_AD24 */
    PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt5);

    /* PORTC2 (pin A13) is configured as FB_AD12 */
    PORT_SetPinMux(PORTC, 2U, kPORT_MuxAlt5);

    /* PORTC5 (pin A11) is configured as FB_AD10 */
    PORT_SetPinMux(PORTC, 5U, kPORT_MuxAlt5);

    /* PORTC6 (pin A10) is configured as FB_AD9 */
    PORT_SetPinMux(PORTC, 6U, kPORT_MuxAlt5);

    /* PORTC7 (pin B10) is configured as FB_AD8 */
    PORT_SetPinMux(PORTC, 7U, kPORT_MuxAlt5);

    /* PORTC8 (pin C10) is configured as FB_AD7 */
    PORT_SetPinMux(PORTC, 8U, kPORT_MuxAlt5);

    /* PORTC9 (pin C9) is configured as FB_AD6 */
    PORT_SetPinMux(PORTC, 9U, kPORT_MuxAlt5);

    /* PORTD0 (pin A6) is configured as FB_CS1_b */
    PORT_SetPinMux(PORTD, 0U, kPORT_MuxAlt5);

    /* PORTD1 (pin A5) is configured as FB_CS0_b */
    PORT_SetPinMux(PORTD, 1U, kPORT_MuxAlt5);

    /* PORTD2 (pin A4) is configured as FB_AD4 */
    PORT_SetPinMux(PORTD, 2U, kPORT_MuxAlt5);

    /* PORTD3 (pin B4) is configured as FB_AD3 */
    PORT_SetPinMux(PORTD, 3U, kPORT_MuxAlt5);

    /* PORTD4 (pin B5) is configured as FB_AD2 */
    PORT_SetPinMux(PORTD, 4U, kPORT_MuxAlt5);

    /* PORTD5 (pin C4) is configured as FB_AD1 */
    PORT_SetPinMux(PORTD, 5U, kPORT_MuxAlt5);

    /* PORTD6 (pin C5) is configured as FB_AD0 */
    PORT_SetPinMux(PORTD, 6U, kPORT_MuxAlt5);

    /* PORTE16 (pin H3) is configured as UART2_TX */
    PORT_SetPinMux(PORTE, 16U, kPORT_MuxAlt3);

    /* PORTE17 (pin F5) is configured as UART2_RX */
    PORT_SetPinMux(PORTE, 17U, kPORT_MuxAlt3);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
