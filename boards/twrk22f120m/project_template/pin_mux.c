/*
 * Copyright 2018-2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v5.0
processor: MK22FN512xxx12
package_id: MK22FN512VDC12
mcu_data: ksdk2_0
processor_version: 0.0.18
board: TWR-K22F120M
pin_labels:
- {pin_num: J6, pin_signal: PTA0/UART0_CTS_b/FTM0_CH5/JTAG_TCLK/SWD_CLK/EZP_CLK, label: 'J31[4]/J20[4]/JTAG_TCLK/SWD_CLK_TGTMCU', identifier: SWD_CLK}
- {pin_num: L11, pin_signal: EXTAL0/PTA18/FTM0_FLT2/FTM_CLKIN0, label: 'X51[1]/8MHz_EXTAL', identifier: EXTAL0}
- {pin_num: K11, pin_signal: XTAL0/PTA19/FTM1_FLT0/FTM_CLKIN1/LPTMR0_ALT1, label: 'X51[3]/8MHz_XTAL', identifier: XTAL0}
- {pin_num: H8, pin_signal: PTA1/UART0_RX/FTM0_CH6/JTAG_TDI/EZP_DI, label: 'J31[8]/J20[8]/JTAG_TDI/SWD_DIO_TGTMCU', identifier: SWD_DIO}
- {pin_num: J7, pin_signal: PTA2/UART0_TX/FTM0_CH7/JTAG_TDO/TRACE_SWO/EZP_DO, label: 'J31[6]/J20[6]/JTAG_TDO'}
- {pin_num: H9, pin_signal: PTA3/UART0_RTS_b/FTM0_CH0/JTAG_TMS/SWD_DIO, label: 'J31[2]/J20[2]/JTAG_TMS'}
- {pin_num: J8, pin_signal: PTA4/LLWU_P3/FTM0_CH1/NMI_b/EZP_CS_b, label: 'J31[9]'}
- {pin_num: K7, pin_signal: PTA5/USB_CLKIN/FTM0_CH2/I2S0_TX_BCLK/JTAG_TRST_b, label: 'J24[B23]'}
- {pin_num: J9, pin_signal: PTA10/FTM2_CH0/FTM2_QD_PHA, label: 'J1[13]'}
- {pin_num: J4, pin_signal: PTA11/FTM2_CH1/FTM2_QD_PHB, label: 'J1[15]'}
- {pin_num: K8, pin_signal: PTA12/FTM1_CH0/I2S0_TXD0/FTM1_QD_PHA, label: 'J24[A34]'}
- {pin_num: L8, pin_signal: PTA13/LLWU_P4/FTM1_CH1/I2S0_TX_FS/FTM1_QD_PHB, label: 'J24[A33]'}
- {pin_num: K9, pin_signal: PTA14/SPI0_PCS0/UART0_TX/I2S0_RX_BCLK, label: 'J14[1]/J24[A42]/J24[B35]/UART0_TX'}
- {pin_num: L9, pin_signal: PTA15/SPI0_SCK/UART0_RX/I2S0_RXD0, label: 'J24[A35]/J24[A41]/UART0_RX'}
- {pin_num: J10, pin_signal: PTA16/SPI0_SOUT/UART0_CTS_b/I2S0_RX_FS, label: 'J24[B59]'}
- {pin_num: H10, pin_signal: ADC1_SE17/PTA17/SPI0_SIN/UART0_RTS_b/I2S0_MCLK, label: 'J14[3]/J24[A11]'}
- {pin_num: H11, pin_signal: PTA29/FB_A24, label: 'J24[A51]'}
- {pin_num: G11, pin_signal: ADC0_SE8/ADC1_SE8/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/FTM1_QD_PHA, label: 'J6[8]/J24[B27]/U1[11]/INT1', identifier: ACCEL_INT1}
- {pin_num: G10, pin_signal: ADC0_SE9/ADC1_SE9/PTB1/I2C0_SDA/FTM1_CH1/FTM1_QD_PHB, label: 'J6[9]/J24[B28]/U1[9]/INT2', identifier: ACCEL_INT2}
- {pin_num: G9, pin_signal: ADC0_SE12/PTB2/I2C0_SCL/UART0_RTS_b/FTM0_FLT3, label: 'J18[1]/J24[B29]/POT_5K', identifier: POT_5K}
- {pin_num: G8, pin_signal: ADC0_SE13/PTB3/I2C0_SDA/UART0_CTS_b/FTM0_FLT0, label: 'J5[15]/J24[B30]'}
- {pin_num: F11, pin_signal: ADC1_SE12/PTB6/FB_AD23, label: 'J24[B58]'}
- {pin_num: D11, pin_signal: PTB8/LPUART0_RTS_b/FB_AD21, label: 'J24[B57]'}
- {pin_num: E11, pin_signal: ADC1_SE13/PTB7/FB_AD22, label: 'J1[3]'}
- {pin_num: E10, pin_signal: PTB9/SPI1_PCS1/LPUART0_CTS_b/FB_AD20, label: 'J24[B8]'}
- {pin_num: D10, pin_signal: ADC1_SE14/PTB10/SPI1_PCS0/LPUART0_RX/FB_AD19/FTM0_FLT1, label: 'J5[11]/J24[B11]/J24[B70]'}
- {pin_num: C10, pin_signal: ADC1_SE15/PTB11/SPI1_SCK/LPUART0_TX/FB_AD18/FTM0_FLT2, label: 'J5[12]/J24[B7]/J24[B69]'}
- {pin_num: B10, pin_signal: PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/EWM_IN, label: 'J5[14]/J24[B10]/J24[B68]'}
- {pin_num: E9, pin_signal: PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/EWM_OUT_b, label: 'J5[9]/J24[B9]/J24[B67]'}
- {pin_num: D9, pin_signal: PTB18/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/FTM2_QD_PHA, label: 'J24[A22]/J24[B66]'}
- {pin_num: C9, pin_signal: PTB19/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB, label: 'J24[A23]/J24[B72]'}
- {pin_num: F10, pin_signal: PTB20/FB_AD31/CMP0_OUT, label: 'J24[B73]'}
- {pin_num: F9, pin_signal: PTB21/FB_AD30/CMP1_OUT, label: 'J24[B74]'}
- {pin_num: F8, pin_signal: PTB22/FB_AD29, label: 'J24[B75]'}
- {pin_num: E8, pin_signal: PTB23/SPI0_PCS5/FB_AD28, label: 'J24[B76]'}
- {pin_num: B9, pin_signal: ADC0_SE14/PTC0/SPI0_PCS4/PDB0_EXTRG/USB_SOF_OUT/FB_AD14, label: 'J5[16]/J24[A61]/J24[A66]'}
- {pin_num: D8, pin_signal: ADC0_SE15/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FB_AD13/I2S0_TXD0/LPUART0_RTS_b, label: 'J24[A25]/J24[40]/J24[A67]'}
- {pin_num: D7, pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/FB_AD10/CMP0_OUT/FTM0_CH2, label: 'J24[A24]/J24[A70]/J24[B62]'}
- {pin_num: C7, pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/I2S0_MCLK, label: 'J24[A58]/J24[A71]/J24[B61]', identifier: SW1}
- {pin_num: B7, pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB_SOF_OUT/I2S0_RX_FS/FB_AD8, label: 'J24[A59]/J24[A72]', identifier: SW2}
- {pin_num: A7, pin_signal: ADC1_SE4b/CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7, label: 'J28[2]/J24[A21]/J24[A73]/U61[2]', identifier: K22_USB_FLGA_B}
- {pin_num: C5, pin_signal: ADC1_SE7b/PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/FB_RW_b, label: 'J5[4]/J7[1]/J24[A60]/J24[B51]/J24[B71]/U1[6]/I2C1_SDA', identifier: ACCEL_SDA}
- {pin_num: D5, pin_signal: PTC16/LPUART0_RX/FB_CS5_b/FB_TSIZ1/FB_BE23_16_BLS15_8_b, label: 'J5[17]/LPUART0_RX'}
- {pin_num: C4, pin_signal: PTC17/LPUART0_TX/FB_CS4_b/FB_TSIZ0/FB_BE31_24_BLS7_0_b, label: 'J5[18]/LPUART0_TX'}
- {pin_num: B4, pin_signal: PTC18/LPUART0_RTS_b/FB_TBST_b/FB_CS2_b/FB_BE15_8_BLS23_16_b, label: 'J5[20]/LPUART0_RTS'}
- {pin_num: A4, pin_signal: PTC19/LPUART0_CTS_b/FB_CS3_b/FB_BE7_0_BLS31_24_b/FB_TA_b, label: 'J5[]19]/LPUART0_CTS'}
- {pin_num: C6, pin_signal: ADC1_SE6b/PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5, label: 'J5[3]/J24[A75]/J24[B50]/J24[B52/J9[1]/U1[4]/I2C1_SCL', identifier: ACCEL_SCL}
- {pin_num: C8, pin_signal: ADC0_SE4b/CMP1_IN0/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FB_AD12/I2S0_TX_FS/LPUART0_CTS_b, label: 'J24[A39]/J24[A68]'}
- {pin_num: B8, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK/LPUART0_RX, label: 'J24[A38]/J24[A64]/CLKOUT'}
- {pin_num: A8, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/FB_AD11/CMP1_OUT/LPUART0_TX, label: 'J24[A37]/J24[A69]'}
- {pin_num: D6, pin_signal: ADC1_SE5b/CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/FTM2_FLT0, label: 'J26[2]/J24[A74]/U61[1]/K22_USB_ENA', identifier: K22_USB_ENA}
- {pin_num: B5, pin_signal: PTC15/FB_AD24, label: 'J24[B80]'}
- {pin_num: A5, pin_signal: PTC14/FB_AD25, label: 'J24[B79]'}
- {pin_num: A6, pin_signal: PTC13/FB_AD26, label: 'J24[B78]'}
- {pin_num: B6, pin_signal: PTC12/FB_AD27/FTM3_FLT0, label: 'J24[B77]'}
- {pin_num: B3, pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/LPUART0_TX/I2C0_SDA, label: 'J24[B44]/J24[A77]'}
- {pin_num: C3, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/LPUART0_RX/I2C0_SCL, label: 'J24[B45]/J24[A76]'}
- {pin_num: D4, pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b/LPUART0_RTS_b, label: 'J24[B46]/J24[B63]'}
- {pin_num: A3, pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FB_AD2/EWM_IN/SPI1_PCS0, label: 'J16[1]/J24[B47]/J24[A78]/LED_J_PTD4', identifier: LED_GREEN}
- {pin_num: D3, pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FB_CS0_b/LPUART0_CTS_b, label: 'J24[B48]/J24[B64]'}
- {pin_num: B2, pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/UART0_RX/FTM0_CH6/FB_AD0/FTM0_FLT0/SPI1_SOUT, label: 'J16[5]/J24[A80]/J24[B38]/LED_J_PTD6', identifier: LED_ORANGE}
- {pin_num: A2, pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/UART0_CTS_b/FTM0_CH5/FB_AD1/EWM_OUT_b/SPI1_SCK, label: 'J16[3]/J24[A79]/LED_J_PTD5', identifier: LED_YELLOW}
- {pin_num: A1, pin_signal: PTD7/UART0_TX/FTM0_CH7/FTM0_FLT1/SPI1_SIN, label: 'J16[7]/J24[A56]/J32[4]/J34[1]/USB_K22_ID/LED_J_PTD7', identifier: LED_BLUE;USB_K22_ID}
- {pin_num: A9, pin_signal: PTD9/I2C0_SDA/LPUART0_TX/FB_A17, label: 'J24[A52]'}
- {pin_num: B1, pin_signal: PTD10/LPUART0_RTS_b/FB_A18, label: 'J24[A53]'}
- {pin_num: E1, pin_signal: PTD15/FB_A23, label: 'J1[A50]'}
- {pin_num: A10, pin_signal: PTD8/I2C0_SCL/LPUART0_RX/FB_A16, label: 'J1[9]'}
- {pin_num: C2, pin_signal: PTD11/LPUART0_CTS_b/FB_A19, label: 'J1[17]'}
- {pin_num: C1, pin_signal: PTD12/FTM3_FLT0/FB_A20, label: 'J1[19]'}
- {pin_num: D2, pin_signal: PTD13/FB_A21, label: 'J1[20]'}
- {pin_num: D1, pin_signal: PTD14/FB_A22, label: 'J1[18]'}
- {pin_num: E4, pin_signal: ADC1_SE4a/PTE0/CLKOUT32K/SPI1_PCS1/UART1_TX/I2C1_SDA/RTC_CLKOUT, label: UART1_TX_PKYK, identifier: DEBUG_UART_TX;RTC_CLKOUT}
- {pin_num: E3, pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/I2C1_SCL/SPI1_SIN, label: UART1_RX_PKYK, identifier: DEBUG_UART_RX}
- {pin_num: E2, pin_signal: ADC1_SE6a/PTE2/LLWU_P1/SPI1_SCK/UART1_CTS_b, label: 'J24[A9]/UART1_CTS'}
- {pin_num: F4, pin_signal: ADC1_SE7a/PTE3/SPI1_SIN/UART1_RTS_b/SPI1_SOUT, label: 'J24[B21]/UART1_RTS'}
- {pin_num: J5, pin_signal: ADC0_SE18/PTE25/I2C0_SDA/EWM_IN, label: 'J24[A8]/J24[B22]/I2C0_SDA'}
- {pin_num: H5, pin_signal: ADC0_SE17/PTE24/I2C0_SCL/EWM_OUT_b, label: 'J24[A7]/J24[B60]/I2C0_SCL'}
- {pin_num: H7, pin_signal: PTE4/LLWU_P2/SPI1_PCS0/LPUART0_TX, label: 'J1[7]'}
- {pin_num: G4, pin_signal: PTE5/SPI1_PCS2/LPUART0_RX/FTM3_CH0, label: 'J24[B56]'}
- {pin_num: F3, pin_signal: PTE6/SPI1_PCS3/LPUART0_CTS_b/I2S0_MCLK/FTM3_CH1/USB_SOF_OUT, label: 'J24[B55]'}
- {pin_num: H6, pin_signal: PTE26/CLKOUT32K/RTC_CLKOUT/USB_CLKIN, label: 'J24[B25]/CLKOUT32K/RTC_CLKOUT', identifier: RTC_CLKOUT;USB_CLKIN}
- {pin_num: L5, pin_signal: EXTAL32, label: 'X50[2]/EXTAL32', identifier: EXTAL32}
- {pin_num: L4, pin_signal: XTAL32, label: 'X50[1]/XTAL32', identifier: XTAL32}
- {pin_num: F1, pin_signal: USB0_DP, label: 'J32[2]/U8[3]/USB0_R_DP', identifier: USB0_DP}
- {pin_num: F2, pin_signal: USB0_DM, label: 'J32[3]/U8[5]/USB0_R_DN', identifier: USB0_DN}
- {pin_num: G2, pin_signal: VREGIN, label: 'J35[2]/VREGIN_K22'}
- {pin_num: J11, pin_signal: RESET_b, label: 'J6[20]/J31[10]/RST_TGTMCU'}
- {pin_num: G1, pin_signal: VOUT33, label: P3V3_VOUT}
- {pin_num: H2, pin_signal: ADC0_DM1, label: 'J1[4]/ADC0_DM1'}
- {pin_num: J2, pin_signal: ADC1_DM1/ADC0_DM2, label: 'J1[10]/ADC1_DM1'}
- {pin_num: K2, pin_signal: ADC0_DM0/ADC1_DM3, label: 'J6[18]/J24[A28]/ADC0_DM0'}
- {pin_num: L2, pin_signal: ADC1_DM0/ADC0_DM3, label: 'J24[A30]/ADC1_DM0/ADC0_DM3'}
- {pin_num: G3, pin_signal: VSS2, label: GND}
- {pin_num: H3, pin_signal: ADC0_SE16/CMP1_IN2/ADC0_SE21, label: 'J1[14]/ADC0_SE21/ADC0_SE16'}
- {pin_num: J3, pin_signal: ADC1_SE16/ADC0_SE22, label: 'J1[16]/ADC1_SE16'}
- {pin_num: K3, pin_signal: NC1, label: NC}
- {pin_num: H1, pin_signal: ADC0_DP1, label: 'J1[2]/ADC0_DP1'}
- {pin_num: J1, pin_signal: ADC1_DP1/ADC0_DP2, label: 'J1[8]/ADC1_DP1'}
- {pin_num: K1, pin_signal: ADC0_DP0/ADC1_DP3, label: 'J6[17]/J24[A27]/ADC0_DP0'}
- {pin_num: L1, pin_signal: ADC1_DP0/ADC0_DP3, label: 'J6[12]/J24[A29]/ADC0_DP3'}
- {pin_num: K5, pin_signal: DAC0_OUT/CMP1_IN3/ADC0_SE23, label: 'J24[A32]/ADC0_SE23/DAC0_OUT'}
- {pin_num: H4, pin_signal: NC2, label: NC}
- {pin_num: K4, pin_signal: DAC1_OUT/CMP0_IN4/ADC1_SE23, label: 'J24[B32]/DAC1_OUT'}
- {pin_num: L3, pin_signal: VREF_OUT/CMP1_IN5/CMP0_IN5/ADC1_SE18, label: ADC1_SE18}
- {pin_num: E7, pin_signal: VDD3, label: MCU_VDD}
- {pin_num: E6, pin_signal: VDD0, label: MCU_VDD}
- {pin_num: E5, pin_signal: VDD1, label: MCU_VDD}
- {pin_num: L10, pin_signal: VDD2, label: MCU_VDD}
- {pin_num: F5, pin_signal: VDDA, label: VDDA}
- {pin_num: G5, pin_signal: VREFH, label: VREFH}
- {pin_num: F6, pin_signal: VSSA, label: GND}
- {pin_num: G6, pin_signal: VREFL, label: VREFL}
- {pin_num: K6, pin_signal: VBAT, label: VBAT}
- {pin_num: F7, pin_signal: VSS4, label: GND}
- {pin_num: G7, pin_signal: VSS0, label: GND}
- {pin_num: L6, pin_signal: VSS1, label: GND}
- {pin_num: K10, pin_signal: VSS3, label: GND}
- {pin_num: L7, pin_signal: RTC_WAKEUP_B, label: RTC_WAKEUP_B}
- {pin_num: A11, pin_signal: NC0, label: NC}
- {pin_num: B11, pin_signal: NC3, label: NC}
- {pin_num: C11, pin_signal: NC4, label: NC}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
    BOARD_InitDEBUG_UARTPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: J7, peripheral: TPIU, signal: SWO, pin_signal: PTA2/UART0_TX/FTM0_CH7/JTAG_TDO/TRACE_SWO/EZP_DO, pull_select: down, pull_enable: disable}
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

    /* PORTA2 (pin J7) is configured as TRACE_SWO */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt7);

    PORTA->PCR[2] = ((PORTA->PCR[2] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                     /* Pull Select: Internal pulldown resistor is enabled on the corresponding pin, if the
                      * corresponding PE field is set. */
                     | PORT_PCR_PS(kPORT_PullDown)

                     /* Pull Enable: Internal pullup or pulldown resistor is not enabled on the corresponding pin. */
                     | PORT_PCR_PE(kPORT_PullDisable));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDsPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: A2, peripheral: GPIOD, signal: 'GPIO, 5', pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/UART0_CTS_b/FTM0_CH5/FB_AD1/EWM_OUT_b/SPI1_SCK, direction: OUTPUT, gpio_init_state: 'true',
    slew_rate: slow, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: B2, peripheral: GPIOD, signal: 'GPIO, 6', pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/UART0_RX/FTM0_CH6/FB_AD0/FTM0_FLT0/SPI1_SOUT, direction: OUTPUT,
    gpio_init_state: 'true', slew_rate: slow, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: A3, peripheral: GPIOD, signal: 'GPIO, 4', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FB_AD2/EWM_IN/SPI1_PCS0, direction: OUTPUT, gpio_init_state: 'true',
    slew_rate: slow, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: A1, peripheral: GPIOD, signal: 'GPIO, 7', pin_signal: PTD7/UART0_TX/FTM0_CH7/FTM0_FLT1/SPI1_SIN, identifier: LED_BLUE, direction: OUTPUT, gpio_init_state: 'true',
    slew_rate: slow, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitLEDsPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitLEDsPins(void)
{
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    gpio_pin_config_t LED_GREEN_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTD4 (pin A3)  */
    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, &LED_GREEN_config);

    gpio_pin_config_t LED_YELLOW_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTD5 (pin A2)  */
    GPIO_PinInit(BOARD_LED_YELLOW_GPIO, BOARD_LED_YELLOW_PIN, &LED_YELLOW_config);

    gpio_pin_config_t LED_ORANGE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTD6 (pin B2)  */
    GPIO_PinInit(BOARD_LED_ORANGE_GPIO, BOARD_LED_ORANGE_PIN, &LED_ORANGE_config);

    gpio_pin_config_t LED_BLUE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTD7 (pin A1)  */
    GPIO_PinInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, &LED_BLUE_config);

    const port_pin_config_t LED_GREEN = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Slow slew rate is configured */
                                         kPORT_SlowSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as PTD4 */
                                         kPORT_MuxAsGpio,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTD4 (pin A3) is configured as PTD4 */
    PORT_SetPinConfig(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, &LED_GREEN);

    const port_pin_config_t LED_YELLOW = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Slow slew rate is configured */
                                          kPORT_SlowSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as PTD5 */
                                          kPORT_MuxAsGpio,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTD5 (pin A2) is configured as PTD5 */
    PORT_SetPinConfig(BOARD_LED_YELLOW_PORT, BOARD_LED_YELLOW_PIN, &LED_YELLOW);

    const port_pin_config_t LED_ORANGE = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Slow slew rate is configured */
                                          kPORT_SlowSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as PTD6 */
                                          kPORT_MuxAsGpio,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTD6 (pin B2) is configured as PTD6 */
    PORT_SetPinConfig(BOARD_LED_ORANGE_PORT, BOARD_LED_ORANGE_PIN, &LED_ORANGE);

    const port_pin_config_t LED_BLUE = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Slow slew rate is configured */
                                        kPORT_SlowSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as PTD7 */
                                        kPORT_MuxAsGpio,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTD7 (pin A1) is configured as PTD7 */
    PORT_SetPinConfig(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, &LED_BLUE);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitButtonsPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: C7, peripheral: GPIOC, signal: 'GPIO, 6', pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/I2S0_MCLK, direction: INPUT, slew_rate: fast,
    open_drain: disable, pull_select: up, pull_enable: enable}
  - {pin_num: B7, peripheral: GPIOC, signal: 'GPIO, 7', pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB_SOF_OUT/I2S0_RX_FS/FB_AD8, direction: INPUT, slew_rate: fast, open_drain: disable,
    pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitButtonsPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitButtonsPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    gpio_pin_config_t SW1_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC6 (pin C7)  */
    GPIO_PinInit(BOARD_SW1_GPIO, BOARD_SW1_PIN, &SW1_config);

    gpio_pin_config_t SW2_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC7 (pin B7)  */
    GPIO_PinInit(BOARD_SW2_GPIO, BOARD_SW2_PIN, &SW2_config);

    const port_pin_config_t SW1 = {/* Internal pull-up resistor is enabled */
                                   kPORT_PullUp,
                                   /* Fast slew rate is configured */
                                   kPORT_FastSlewRate,
                                   /* Passive filter is disabled */
                                   kPORT_PassiveFilterDisable,
                                   /* Open drain is disabled */
                                   kPORT_OpenDrainDisable,
                                   /* Low drive strength is configured */
                                   kPORT_LowDriveStrength,
                                   /* Pin is configured as PTC6 */
                                   kPORT_MuxAsGpio,
                                   /* Pin Control Register fields [15:0] are not locked */
                                   kPORT_UnlockRegister};
    /* PORTC6 (pin C7) is configured as PTC6 */
    PORT_SetPinConfig(BOARD_SW1_PORT, BOARD_SW1_PIN, &SW1);

    const port_pin_config_t SW2 = {/* Internal pull-up resistor is enabled */
                                   kPORT_PullUp,
                                   /* Fast slew rate is configured */
                                   kPORT_FastSlewRate,
                                   /* Passive filter is disabled */
                                   kPORT_PassiveFilterDisable,
                                   /* Open drain is disabled */
                                   kPORT_OpenDrainDisable,
                                   /* Low drive strength is configured */
                                   kPORT_LowDriveStrength,
                                   /* Pin is configured as PTC7 */
                                   kPORT_MuxAsGpio,
                                   /* Pin Control Register fields [15:0] are not locked */
                                   kPORT_UnlockRegister};
    /* PORTC7 (pin B7) is configured as PTC7 */
    PORT_SetPinConfig(BOARD_SW2_PORT, BOARD_SW2_PIN, &SW2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitDEBUG_UARTPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: E3, peripheral: UART1, signal: RX, pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/I2C1_SCL/SPI1_SIN, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: E4, peripheral: UART1, signal: TX, pin_signal: ADC1_SE4a/PTE0/CLKOUT32K/SPI1_PCS1/UART1_TX/I2C1_SDA/RTC_CLKOUT, identifier: DEBUG_UART_TX, direction: OUTPUT,
    slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitDEBUG_UARTPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitDEBUG_UARTPins(void)
{
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t DEBUG_UART_TX = {/* Internal pull-up/down resistor is disabled */
                                             kPORT_PullDisable,
                                             /* Fast slew rate is configured */
                                             kPORT_FastSlewRate,
                                             /* Passive filter is disabled */
                                             kPORT_PassiveFilterDisable,
                                             /* Open drain is disabled */
                                             kPORT_OpenDrainDisable,
                                             /* Low drive strength is configured */
                                             kPORT_LowDriveStrength,
                                             /* Pin is configured as UART1_TX */
                                             kPORT_MuxAlt3,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    /* PORTE0 (pin E4) is configured as UART1_TX */
    PORT_SetPinConfig(BOARD_DEBUG_UART_TX_PORT, BOARD_DEBUG_UART_TX_PIN, &DEBUG_UART_TX);

    const port_pin_config_t DEBUG_UART_RX = {/* Internal pull-up/down resistor is disabled */
                                             kPORT_PullDisable,
                                             /* Fast slew rate is configured */
                                             kPORT_FastSlewRate,
                                             /* Passive filter is disabled */
                                             kPORT_PassiveFilterDisable,
                                             /* Open drain is disabled */
                                             kPORT_OpenDrainDisable,
                                             /* Low drive strength is configured */
                                             kPORT_LowDriveStrength,
                                             /* Pin is configured as UART1_RX */
                                             kPORT_MuxAlt3,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    /* PORTE1 (pin E3) is configured as UART1_RX */
    PORT_SetPinConfig(BOARD_DEBUG_UART_RX_PORT, BOARD_DEBUG_UART_RX_PIN, &DEBUG_UART_RX);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART1TXSRC_MASK)))

                  /* UART 1 transmit data source select: UART1_TX pin. */
                  | SIM_SOPT5_UART1TXSRC(SOPT5_UART1TXSRC_UART_TX));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitAccelPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: C5, peripheral: I2C1, signal: SDA, pin_signal: ADC1_SE7b/PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/FB_RW_b, slew_rate: fast, open_drain: enable, pull_select: up,
    pull_enable: enable}
  - {pin_num: C6, peripheral: I2C1, signal: SCL, pin_signal: ADC1_SE6b/PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5, slew_rate: fast, open_drain: enable, pull_select: up,
    pull_enable: enable}
  - {pin_num: G11, peripheral: GPIOB, signal: 'GPIO, 0', pin_signal: ADC0_SE8/ADC1_SE8/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/FTM1_QD_PHA, direction: INPUT, slew_rate: fast,
    open_drain: enable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: G10, peripheral: GPIOB, signal: 'GPIO, 1', pin_signal: ADC0_SE9/ADC1_SE9/PTB1/I2C0_SDA/FTM1_CH1/FTM1_QD_PHB, direction: INPUT, slew_rate: fast, open_drain: enable,
    drive_strength: low, pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitAccelPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitAccelPins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    gpio_pin_config_t ACCEL_INT1_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTB0 (pin G11)  */
    GPIO_PinInit(BOARD_ACCEL_INT1_GPIO, BOARD_ACCEL_INT1_PIN, &ACCEL_INT1_config);

    gpio_pin_config_t ACCEL_INT2_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTB1 (pin G10)  */
    GPIO_PinInit(BOARD_ACCEL_INT2_GPIO, BOARD_ACCEL_INT2_PIN, &ACCEL_INT2_config);

    const port_pin_config_t ACCEL_INT1 = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is enabled */
                                          kPORT_OpenDrainEnable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as PTB0 */
                                          kPORT_MuxAsGpio,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTB0 (pin G11) is configured as PTB0 */
    PORT_SetPinConfig(BOARD_ACCEL_INT1_PORT, BOARD_ACCEL_INT1_PIN, &ACCEL_INT1);

    const port_pin_config_t ACCEL_INT2 = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is enabled */
                                          kPORT_OpenDrainEnable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as PTB1 */
                                          kPORT_MuxAsGpio,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTB1 (pin G10) is configured as PTB1 */
    PORT_SetPinConfig(BOARD_ACCEL_INT2_PORT, BOARD_ACCEL_INT2_PIN, &ACCEL_INT2);

    const port_pin_config_t ACCEL_SCL = {/* Internal pull-up resistor is enabled */
                                         kPORT_PullUp,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is enabled */
                                         kPORT_OpenDrainEnable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as I2C1_SCL */
                                         kPORT_MuxAlt2,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTC10 (pin C6) is configured as I2C1_SCL */
    PORT_SetPinConfig(BOARD_ACCEL_SCL_PORT, BOARD_ACCEL_SCL_PIN, &ACCEL_SCL);

    const port_pin_config_t ACCEL_SDA = {/* Internal pull-up resistor is enabled */
                                         kPORT_PullUp,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is enabled */
                                         kPORT_OpenDrainEnable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as I2C1_SDA */
                                         kPORT_MuxAlt2,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTC11 (pin C5) is configured as I2C1_SDA */
    PORT_SetPinConfig(BOARD_ACCEL_SDA_PORT, BOARD_ACCEL_SDA_PIN, &ACCEL_SDA);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitOSC0Pins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: L11, peripheral: OSC, signal: EXTAL0, pin_signal: EXTAL0/PTA18/FTM0_FLT2/FTM_CLKIN0, slew_rate: no_init, open_drain: no_init, pull_select: no_init,
    pull_enable: no_init}
  - {pin_num: K11, peripheral: OSC, signal: XTAL0, pin_signal: XTAL0/PTA19/FTM1_FLT0/FTM_CLKIN1/LPTMR0_ALT1, slew_rate: no_init, open_drain: no_init, pull_select: no_init,
    pull_enable: no_init}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitOSC0Pins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitOSC0Pins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);

    /* PORTA18 (pin L11) is configured as EXTAL0 */
    PORT_SetPinMux(BOARD_EXTAL0_PORT, BOARD_EXTAL0_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTA19 (pin K11) is configured as XTAL0 */
    PORT_SetPinMux(BOARD_XTAL0_PORT, BOARD_XTAL0_PIN, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitUSBHostPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: F1, peripheral: USB0, signal: DP, pin_signal: USB0_DP}
  - {pin_num: F2, peripheral: USB0, signal: DM, pin_signal: USB0_DM}
  - {pin_num: G2, peripheral: USB0, signal: VREGIN, pin_signal: VREGIN}
  - {pin_num: G1, peripheral: USB0, signal: VOUT33, pin_signal: VOUT33}
  - {pin_num: D6, peripheral: GPIOC, signal: 'GPIO, 9', pin_signal: ADC1_SE5b/CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/FTM2_FLT0, direction: OUTPUT, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: A1, peripheral: GPIOD, signal: 'GPIO, 7', pin_signal: PTD7/UART0_TX/FTM0_CH7/FTM0_FLT1/SPI1_SIN, identifier: USB_K22_ID, direction: OUTPUT, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitUSBHostPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitUSBHostPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    gpio_pin_config_t K22_USB_ENA_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC9 (pin D6)  */
    GPIO_PinInit(BOARD_K22_USB_ENA_GPIO, BOARD_K22_USB_ENA_PIN, &K22_USB_ENA_config);

    gpio_pin_config_t USB_K22_ID_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTD7 (pin A1)  */
    GPIO_PinInit(BOARD_USB_K22_ID_GPIO, BOARD_USB_K22_ID_PIN, &USB_K22_ID_config);

    const port_pin_config_t K22_USB_ENA = {/* Internal pull-up/down resistor is disabled */
                                           kPORT_PullDisable,
                                           /* Fast slew rate is configured */
                                           kPORT_FastSlewRate,
                                           /* Passive filter is disabled */
                                           kPORT_PassiveFilterDisable,
                                           /* Open drain is disabled */
                                           kPORT_OpenDrainDisable,
                                           /* Low drive strength is configured */
                                           kPORT_LowDriveStrength,
                                           /* Pin is configured as PTC9 */
                                           kPORT_MuxAsGpio,
                                           /* Pin Control Register fields [15:0] are not locked */
                                           kPORT_UnlockRegister};
    /* PORTC9 (pin D6) is configured as PTC9 */
    PORT_SetPinConfig(BOARD_K22_USB_ENA_PORT, BOARD_K22_USB_ENA_PIN, &K22_USB_ENA);

    const port_pin_config_t USB_K22_ID = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as PTD7 */
                                          kPORT_MuxAsGpio,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTD7 (pin A1) is configured as PTD7 */
    PORT_SetPinConfig(BOARD_USB_K22_ID_PORT, BOARD_USB_K22_ID_PIN, &USB_K22_ID);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPOTPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: G9, peripheral: ADC0, signal: 'SE, 12', pin_signal: ADC0_SE12/PTB2/I2C0_SCL/UART0_RTS_b/FTM0_FLT3, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPOTPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPOTPins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    const port_pin_config_t POT_5K = {/* Internal pull-up/down resistor is disabled */
                                      kPORT_PullDisable,
                                      /* Fast slew rate is configured */
                                      kPORT_FastSlewRate,
                                      /* Passive filter is disabled */
                                      kPORT_PassiveFilterDisable,
                                      /* Open drain is disabled */
                                      kPORT_OpenDrainDisable,
                                      /* Low drive strength is configured */
                                      kPORT_LowDriveStrength,
                                      /* Pin is configured as ADC0_SE12 */
                                      kPORT_PinDisabledOrAnalog,
                                      /* Pin Control Register fields [15:0] are not locked */
                                      kPORT_UnlockRegister};
    /* PORTB2 (pin G9) is configured as ADC0_SE12 */
    PORT_SetPinConfig(BOARD_POT_5K_PORT, BOARD_POT_5K_PIN, &POT_5K);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
