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
processor: MK64FN1M0xxx12
package_id: MK64FN1M0VMD12
mcu_data: ksdk2_0
processor_version: 4.0.0
board: TWR-K64F120M
pin_labels:
- {pin_num: L6, pin_signal: VBAT, label: VBAT}
- {pin_num: M6, pin_signal: EXTAL32, label: 'Y2[1]/EXTAL32_RTC', identifier: EXTAL_32K}
- {pin_num: M7, pin_signal: XTAL32, label: 'Y2[2]/XTAL32_RTC', identifier: XTAL_32K}
- {pin_num: G6, pin_signal: VREFL, label: GND, identifier: GND}
- {pin_num: G5, pin_signal: VREFH, label: VREFH, identifier: VREFH}
- {pin_num: G1, pin_signal: VOUT33, label: VOUT33_K64, identifier: VOUT33_K64}
- {pin_num: H1, pin_signal: USB0_DP, label: 'J17[3]/K64_USB0_DP', identifier: USB_DP}
- {pin_num: H2, pin_signal: USB0_DM, label: 'J17[2]/K64_USB0_DM', identifier: USB_DM}
- {pin_num: L5, pin_signal: RTC_WAKEUP_B, label: 'J21[1]/R69/TAMPER0', identifier: TAMPER0}
- {pin_num: H5, pin_signal: VDDA, label: VDDA}
- {pin_num: E5, pin_signal: VDD5, label: MCU_PWR}
- {pin_num: F5, pin_signal: VDD110, label: MCU_PWR}
- {pin_num: E6, pin_signal: VDD16, label: MCU_PWR}
- {pin_num: E7, pin_signal: VDD63, label: MCU_PWR}
- {pin_num: E8, pin_signal: VDD80, label: MCU_PWR}
- {pin_num: F8, pin_signal: VDD153, label: MCU_PWR}
- {pin_num: M11, pin_signal: XTAL0/PTA19/FTM1_FLT0/FTM_CLKIN1/LPTMR0_ALT1, label: GND}
- {pin_num: L12, pin_signal: RESET_b, label: 'J1[1]/J5[10]/J4[20]/J16[3]/J11[A62]/TWRPI_RESET_B/RESET'}
- {pin_num: C10, pin_signal: NC167, label: NC}
- {pin_num: B10, pin_signal: NC166, label: NC}
- {pin_num: A10, pin_signal: NC165, label: NC}
- {pin_num: M5, pin_signal: NC164, label: NC}
- {pin_num: H3, pin_signal: VSS18, label: GND}
- {pin_num: F6, pin_signal: VSS6, label: GND}
- {pin_num: F7, pin_signal: VSS17, label: GND}
- {pin_num: H6, pin_signal: VSSA, label: GND}
- {pin_num: G7, pin_signal: VSS64, label: GND}
- {pin_num: H7, pin_signal: VSS109, label: GND}
- {pin_num: G8, pin_signal: VSS81, label: GND}
- {pin_num: H8, pin_signal: VSS123, label: GND}
- {pin_num: M10, pin_signal: VSS152, label: GND}
- {pin_num: L7, pin_signal: PTA4/LLWU_P3/FTM0_CH1/NMI_b/EZP_CS_b, label: SW3/NMI_B, identifier: SW3}
- {pin_num: C8, pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/I2S0_MCLK, label: 'J11[A21]/J11[A58]/J11[A71]/SW1', identifier: SW1;I2S0_MCLK;FB_AD9}
- {pin_num: A6, pin_signal: PTC16/UART3_RX/ENET0_1588_TMR0/FB_CS5_b/FB_TSIZ1/FB_BE23_16_BLS15_8_b, label: 'J3[17]/J12[7]/TWRPI_GPIO2/ENET0_1588_TMR0', identifier: TWRPI_GPIO2}
- {pin_num: D5, pin_signal: PTC17/UART3_TX/ENET0_1588_TMR1/FB_CS4_b/FB_TSIZ0/FB_BE31_24_BLS7_0_b, label: 'J3[16]/J12[5]/TWRPI_GPIO6/ENET0_1588_TMR1', identifier: TWRPI_GPIO6}
- {pin_num: C5, pin_signal: PTC18/UART3_RTS_b/ENET0_1588_TMR2/FB_TBST_b/FB_CS2_b/FB_BE15_8_BLS23_16_b, label: 'J3[19]/J12[3]/TWRPI_GPIO4/ENET0_1588_TMR2', identifier: TWRPI_GPIO4}
- {pin_num: B5, pin_signal: PTC19/UART3_CTS_b/ENET0_1588_TMR3/FB_CS3_b/FB_BE7_0_BLS31_24_b/FB_TA_b, label: 'J3/J12[1]/TWRPI_GPIO5/ENET0_1588_TMR3', identifier: TWRPI_GPIO5}
- {pin_num: M12, pin_signal: EXTAL0/PTA18/FTM0_FLT2/FTM_CLKIN0, label: 'J11[B24]/Y1[3]/EXTAL_K64_MAIN', identifier: XTAL0}
- {pin_num: K7, pin_signal: PTA3/UART0_RTS_b/FTM0_CH0/JTAG_TMS/SWD_DIO, label: 'J5[2]/JTAG_TMS/SWD_DIO'}
- {pin_num: J5, pin_signal: PTA0/UART0_CTS_b/UART0_COL_b/FTM0_CH5/JTAG_TCLK/SWD_CLK/EZP_CLK, label: 'J5[4]/JTAG_TCLK/SWD_CLK/EZP_CLK'}
- {pin_num: K6, pin_signal: PTA2/UART0_TX/FTM0_CH7/JTAG_TDO/TRACE_SWO/EZP_DO, label: 'J5[6]/J11[B37]/JTAG_TDO/TRACE_SWO/EZP_DO'}
- {pin_num: J6, pin_signal: PTA1/UART0_RX/FTM0_CH6/JTAG_TDI/EZP_DI, label: 'J5[8]/JTAG_TDI/EZP_DI'}
- {pin_num: D3, pin_signal: ADC1_SE4a/PTE0/SPI1_PCS1/UART1_TX/SDHC0_D1/TRACE_CLKOUT/I2C1_SDA/RTC_CLKOUT, label: 'J5[12]/J11[B22]/P500[8]/SDHC0_D1/TRACE_CLKOUT', identifier: SDHC_D1}
- {pin_num: E3, pin_signal: PTE4/LLWU_P2/SPI1_PCS0/UART3_TX/SDHC0_D3/TRACE_D0, label: 'J5[14]/P500[1]/SDHC0_D3/TRACE_D0', identifier: SDHC_D3}
- {pin_num: E4, pin_signal: ADC0_DM2/ADC1_SE7a/PTE3/SPI1_SIN/UART1_RTS_b/SDHC0_CMD/TRACE_D1/SPI1_SOUT, label: 'J5[16]/P500[2]/SDHC0_CMD/TRACE_D1', identifier: SDHC_CMD}
- {pin_num: D1, pin_signal: ADC0_DP2/ADC1_SE6a/PTE2/LLWU_P1/SPI1_SCK/UART1_CTS_b/SDHC0_DCLK/TRACE_D2, label: 'J5[18]/P500[5]/SDHC0_DCLK/TRACE_D2', identifier: SDHC_CLK}
- {pin_num: D2, pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/SDHC0_D0/TRACE_D3/I2C1_SCL/SPI1_SIN, label: 'J5[20]/P500[7]/SDHC0_D0/TRACE_D3', identifier: SDHC_D0}
- {pin_num: M4, pin_signal: ADC0_SE17/PTE24/UART4_TX/I2C0_SCL/EWM_OUT_b, label: 'J36[2]/UART4_TX', identifier: UART4_TX}
- {pin_num: K5, pin_signal: ADC0_SE18/PTE25/UART4_RX/I2C0_SDA/EWM_IN, label: 'J27[2]/UART4_RX', identifier: UART4_RX}
- {pin_num: J4, pin_signal: PTE27/UART4_RTS_b, label: 'J34[2]/SDHC_WP_B/UART4_RTS_B', identifier: UART4_RTS}
- {pin_num: H4, pin_signal: PTE28, label: 'J11[B23]/J35[2]', identifier: GPOI3_ELEV}
- {pin_num: K4, pin_signal: PTE26/ENET_1588_CLKIN/UART4_CTS_b/RTC_CLKOUT/USB_CLKIN, label: 'J11[B25]/J31[2]/CLKOUT32K/RTC_CLKOUT/ENET_1588_CLKIN', identifier: RTC_CLKOUT}
- {pin_num: G2, pin_signal: VREGIN, label: 'J19[2]/VREGIN_K64', identifier: VREGIN_K64}
- {pin_num: J7, pin_signal: PTA6/FTM0_CH3/CLKOUT/TRACE_CLKOUT, label: 'J8[2]/U6[11]/ACCEL_INT1', identifier: ACCEL_INT1}
- {pin_num: K8, pin_signal: ADC0_SE11/PTA8/FTM1_CH0/FTM1_QD_PHA/TRACE_D2, label: 'J7[2]/U6[9]/ACCEL_INT2', identifier: ACCEL_INT2}
- {pin_num: C7, pin_signal: ADC1_SE6b/PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5, label: 'J3[3]/J11[A75]/U6[4]/I2C1_SCL', identifier: ACCEL_SCL;FB_AD5}
- {pin_num: B7, pin_signal: ADC1_SE7b/PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/I2S0_RXD1/FB_RW_b, label: 'J3[4]/J11[A60]/J11[B71]/U6[6]/I2C1_SDA', identifier: ACCEL_SDA;FB_RW_B}
- {pin_num: E2, pin_signal: PTE5/SPI1_PCS2/UART3_RX/SDHC0_D2/FTM3_CH0, label: 'P500[9]/SDHC0_D2', identifier: SDHC_D2}
- {pin_num: D10, pin_signal: PTB20/SPI2_PCS0/FB_AD31/CMP0_OUT, label: 'J11[B73]/P500[10]/SD_CARD_DETECT', identifier: SD_CARD_DETECT;FB_AD31}
- {pin_num: E1, pin_signal: PTE6/SPI1_PCS3/UART3_CTS_b/I2S0_MCLK/FTM3_CH1/USB_SOF_OUT, label: 'J30[1]/D5/LED1', identifier: LED_GREEN}
- {pin_num: F4, pin_signal: PTE7/UART3_RTS_b/I2S0_RXD0/FTM3_CH2, label: 'J30[3]/D6/LED2', identifier: LED_YELLOW}
- {pin_num: F3, pin_signal: PTE8/I2S0_RXD1/UART5_TX/I2S0_RX_FS/FTM3_CH3, label: 'J11[A44]/J30[5]/D7/LED3', identifier: LED_ORANGE}
- {pin_num: F2, pin_signal: PTE9/I2S0_TXD1/UART5_RX/I2S0_RX_BCLK/FTM3_CH4, label: 'J11[A43]/J30[7]/D9/LED4', identifier: LED_BLUE}
- {pin_num: A8, pin_signal: ADC1_SE4b/CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7, label: 'J23[2]/J11[A73]/U505[2]/K64_USB_FLGA', identifier: USB_FLGA;FB_AD7}
- {pin_num: D7, pin_signal: ADC1_SE5b/CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/FTM2_FLT0, label: 'J26[1]/J11[A74]/U505[1]/K64_USB_ENABLE', identifier: USB_ENABLE;FB_AD6}
- {pin_num: G3, pin_signal: PTE12/I2S0_TX_BCLK/FTM3_CH7, label: 'J25[2]/K64_USB_ID', identifier: USB_ID}
- {pin_num: M3, pin_signal: VREF_OUT/CMP1_IN5/CMP0_IN5/ADC1_SE18, label: 'J24[2]/POT', identifier: POT}
- {pin_num: J3, pin_signal: ADC0_SE16/CMP1_IN2/ADC0_SE21, label: 'J4[9]/TWRPI_ADC1', identifier: TWRPI_ADC1}
- {pin_num: L1, pin_signal: ADC0_DP0/ADC1_DP3, label: 'J4[17]/TWRPI_ID0', identifier: TWRPI_ID0}
- {pin_num: L2, pin_signal: ADC0_DM0/ADC1_DM3, label: 'J4[18]/TWRPI_ID1', identifier: TWRPI_ID1}
- {pin_num: M1, pin_signal: ADC1_DP0/ADC0_DP3, label: 'J4[12]/TWRPI_ADC2', identifier: TWRPI_ADC2}
- {pin_num: K3, pin_signal: ADC1_SE16/CMP2_IN2/ADC0_SE22, label: 'J4[8]/TWRPI_ADC0', identifier: TWRPI_ADC0}
- {pin_num: E9, pin_signal: PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/EWM_OUT_b, label: 'J3[9]/J11[B11]/J11[B67]/SPI1_SIN', identifier: SPI1_SIN;FB_AD16}
- {pin_num: E12, pin_signal: ADC1_SE14/PTB10/SPI1_PCS0/UART3_RX/FB_AD19/FTM0_FLT1, label: 'J3[11]/J11[B9]/J11[B70]/SPI1_PCS0', identifier: SPI1_PCS0;FB_AD19}
- {pin_num: G11, pin_signal: ADC0_SE13/PTB3/I2C0_SDA/UART0_CTS_b/UART0_COL_b/ENET0_1588_TMR1/FTM0_FLT0, label: 'J3[15]/TWRPI_GPIO0', identifier: TWRPI_GPIO0}
- {pin_num: E10, pin_signal: PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/EWM_IN, label: 'J3[10]/J11[B10]/J11[B68]/SPI1_SOUT', identifier: SPI1_SOUT;FB_AD17}
- {pin_num: E11, pin_signal: ADC1_SE15/PTB11/SPI1_SCK/UART3_TX/FB_AD18/FTM0_FLT2, label: 'J3[12]/J11[B7]/J11[B34]/J11[B69]', identifier: SPI1_SCK;FB_AD18}
- {pin_num: B12, pin_signal: ADC0_SE14/PTC0/SPI0_PCS4/PDB0_EXTRG/USB_SOF_OUT/FB_AD14/I2S0_TXD1, label: 'J3[16]/J11[A61]/J11[A66]/TWRPI_GPIO1', identifier: TWRPI_GPIO1;FB_AD14}
- {pin_num: A11, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK, label: 'J10[2]/J11[A38]/J11[A64]/UART1_RX', identifier: DEBUG_UART_RX}
- {pin_num: A9, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/FB_AD11/CMP1_OUT, label: 'J11[A37]/J15[2]/J11[A69]/UART1_TX', identifier: DEBUG_UART_TX}
- {pin_num: F9, pin_signal: PTB9/SPI1_PCS1/UART3_CTS_b/FB_AD20, label: 'J11[A35]/J11[B8]', identifier: SPI1_PCS1}
- {pin_num: H11, pin_signal: PTA29/MII0_COL/FB_A24, label: 'J11[B12]'}
- {pin_num: M8, pin_signal: PTA5/USB_CLKIN/FTM0_CH2/RMII0_RXER/MII0_RXER/CMP2_OUT/I2S0_TX_BCLK/JTAG_TRST_b, label: 'J11[B13]'}
- {pin_num: J12, pin_signal: PTA25/MII0_TXCLK/FB_A28, label: 'J11[B14]'}
- {pin_num: L11, pin_signal: PTA15/SPI0_SCK/UART0_RX/RMII0_TXEN/MII0_TXEN/I2S0_RXD0, label: 'J11[B15]'}
- {pin_num: H12, pin_signal: PTA28/MII0_TXER/FB_A25, label: 'J11[B16]'}
- {pin_num: J11, pin_signal: PTA26/MII0_TXD3/FB_A27, label: 'J11[B17]'}
- {pin_num: K12, pin_signal: PTA24/MII0_TXD2/FB_A29, label: 'J11[B18]', identifier: MII0_TXD2}
- {pin_num: K11, pin_signal: ADC1_SE17/PTA17/SPI0_SIN/UART0_RTS_b/RMII0_TXD1/MII0_TXD1/I2S0_MCLK, label: 'J11[B19]'}
- {pin_num: K10, pin_signal: PTA16/SPI0_SOUT/UART0_CTS_b/UART0_COL_b/RMII0_TXD0/MII0_TXD0/I2S0_RX_FS/I2S0_RXD1, label: 'J11[B20]'}
- {pin_num: G4, pin_signal: PTE11/UART5_RTS_b/I2S0_TX_FS/FTM3_CH6, label: 'J11[B21]'}
- {pin_num: G10, pin_signal: ADC1_SE10/PTB4/ENET0_1588_TMR2/FTM1_FLT0, label: 'J11[B27]', identifier: ADC1_SE10}
- {pin_num: G9, pin_signal: ADC1_SE11/PTB5/ENET0_1588_TMR3/FTM2_FLT0, label: 'J11[B28]', identifier: ADC1_SE11}
- {pin_num: F12, pin_signal: ADC1_SE12/PTB6/FB_AD23, label: 'J11[B29]', identifier: ADC1_SE12}
- {pin_num: F11, pin_signal: ADC1_SE13/PTB7/FB_AD22, label: 'J1[1]/J11[B30]', identifier: ADC1_SE13}
- {pin_num: L4, pin_signal: DAC1_OUT/CMP0_IN4/CMP2_IN3/ADC1_SE23, label: 'J11[B32]', identifier: DAC1_OUT}
- {pin_num: G12, pin_signal: ADC0_SE12/PTB2/I2C0_SCL/UART0_RTS_b/ENET0_1588_TMR0/FTM0_FLT3, label: 'J11[B33]', identifier: TMR3}
- {pin_num: F10, pin_signal: PTB8/UART3_RTS_b/FB_AD21, label: 'J11[B35]', identifier: GPIO4_ELEV}
- {pin_num: A2, pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/UART0_RX/FTM0_CH6/FB_AD0/FTM0_FLT0/SPI1_SOUT, label: 'J11[A34]/J11[A80]/J11[B38]', identifier: FTM0_CH6;FB_AD0}
- {pin_num: A3, pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FB_AD1/EWM_OUT_b/SPI1_SCK, label: 'J11[A79]/J11[B39]', identifier: FTM0_CH5;FB_AD1}
- {pin_num: J8, pin_signal: ADC0_SE10/PTA7/FTM0_CH4/TRACE_D3, label: 'J11[B40]', identifier: FTM0_CH4}
- {pin_num: D11, pin_signal: PTB19/CAN0_RX/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB, label: 'J11[A23]/J11[B41]/J11[B72]', identifier: CAN0_RX;I2S0_TX_FS;FB_OE_B}
- {pin_num: D12, pin_signal: PTB18/CAN0_TX/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/FTM2_QD_PHA, label: 'J11[A22]/J11[B42]/J11[B66]', identifier: CAN0_TX;I2S_TX_BCLK;FB_AD15}
- {pin_num: B4, pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/I2C0_SDA, label: 'J11[A77]/J11[B44]', identifier: SPI0_SIN;FB_AD3}
- {pin_num: C4, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/I2C0_SCL, label: 'J11[A76]/J11[B45]', identifier: SPI0_SOUT;FB_AD4}
- {pin_num: A5, pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b, label: 'J11[B46]/J11[B63]', identifier: SPI0_PCS0;FB_ALE_B}
- {pin_num: A4, pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FB_AD2/EWM_IN/SPI1_PCS0, label: 'J11[A78]/J11[B47]', identifier: SPI0_PCS1;FB_AD2}
- {pin_num: D4, pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FB_CS0_b, label: 'J11[B48]/J11[B64]', identifier: SPI0_SCK;FB_CS0_B}
- {pin_num: C9, pin_signal: PTD8/I2C0_SCL/UART5_RX/FB_A16, label: 'J11[A7]/J11[B50]', identifier: I2C0_SCL}
- {pin_num: B9, pin_signal: PTD9/I2C0_SDA/UART5_TX/FB_A17, label: 'J11[A8]/J11[B51]', identifier: I2C0_SDA}
- {pin_num: A1, pin_signal: PTD7/CMT_IRO/UART0_TX/FTM0_CH7/FTM0_FLT1/SPI1_SIN, label: 'J11[A33]/J11[B52]', identifier: SDHC_DETECT}
- {pin_num: B1, pin_signal: PTD12/SPI2_SCK/FTM3_FLT0/SDHC0_D4/FB_A20, label: 'J11[B55]/J11[B56]', identifier: IRQ_G_H}
- {pin_num: C3, pin_signal: PTD13/SPI2_SOUT/SDHC0_D5/FB_A21, label: 'J11[B57]/J11[B58]', identifier: IRQ_E_F}
- {pin_num: C2, pin_signal: PTD14/SPI2_SIN/SDHC0_D6/FB_A22, label: 'J11[B59]/J11[B60]', identifier: IRQ_C_D}
- {pin_num: C1, pin_signal: PTD15/SPI2_PCS1/SDHC0_D7/FB_A23, label: 'J11[B61]/J11[B62]', identifier: IRQ_A_B}
- {pin_num: D9, pin_signal: PTB21/SPI2_SCK/FB_AD30/CMP1_OUT, label: 'J11[B74]', identifier: FB_AD30}
- {pin_num: C12, pin_signal: PTB22/SPI2_SOUT/FB_AD29/CMP2_OUT, label: 'J11[B75]', identifier: FB_AD29}
- {pin_num: C11, pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28, label: 'J11[B76]', identifier: FB_AD28}
- {pin_num: A7, pin_signal: PTC12/UART4_RTS_b/FB_AD27/FTM3_FLT0, label: 'J11[A10]/J11[B77]', identifier: GPIO8_ELEV;FB_AD27}
- {pin_num: D6, pin_signal: PTC13/UART4_CTS_b/FB_AD26, label: 'J11[A11]/J11[B78]', identifier: GPIO7_ELEV;FB_AD26}
- {pin_num: C6, pin_signal: PTC14/UART4_RX/FB_AD25, label: 'J11[A50]/J11[B79]', identifier: FB_AD25}
- {pin_num: B6, pin_signal: PTC15/UART4_TX/FB_AD24, label: 'J11[A50]/J11[B80]', identifier: FB_AD24}
- {pin_num: F1, pin_signal: PTE10/UART5_CTS_b/I2S0_TXD0/FTM3_CH5, label: 'J11[A9]'}
- {pin_num: J10, pin_signal: PTA27/MII0_CRS/FB_A26, label: 'J11[A12]', identifier: ETH_CRS}
- {pin_num: H9, pin_signal: ADC0_SE9/ADC1_SE9/PTB1/I2C0_SDA/FTM1_CH1/RMII0_MDC/MII0_MDC/FTM1_QD_PHB, label: 'J11[A13]', identifier: ETH_MDC_1}
- {pin_num: H10, pin_signal: ADC0_SE8/ADC1_SE8/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/RMII0_MDIO/MII0_MDIO/FTM1_QD_PHA, label: 'J11[A14]', identifier: ETH_MDIO_1}
- {pin_num: L9, pin_signal: PTA11/FTM2_CH1/MII0_RXCLK/I2C2_SDA/FTM2_QD_PHB, label: 'J11[A15]', identifier: ETH_RXCLK_1}
- {pin_num: L10, pin_signal: PTA14/SPI0_PCS0/UART0_TX/RMII0_CRS_DV/MII0_RXDV/I2C2_SCL/I2S0_RX_BCLK/I2S0_TXD1, label: 'J11[A16]', identifier: ETH_RXDV_1}
- {pin_num: L8, pin_signal: PTA9/FTM1_CH1/MII0_RXD3/FTM1_QD_PHB/TRACE_D1, label: 'J11[A17]', identifier: ETH_RXD3}
- {pin_num: M9, pin_signal: PTA10/FTM2_CH0/MII0_RXD2/FTM2_QD_PHA/TRACE_D0, label: 'J11[A18]', identifier: ETH_RXD2}
- {pin_num: K9, pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/I2C2_SCL/I2S0_TXD0/FTM1_QD_PHA, label: 'J11[A19]', identifier: ETH_RXD1_1}
- {pin_num: J9, pin_signal: CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/RMII0_RXD0/MII0_RXD0/I2C2_SDA/I2S0_TX_FS/FTM1_QD_PHB, label: 'J11[A20]', identifier: ETH_RXD0_1}
- {pin_num: D8, pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/FB_AD10/CMP0_OUT/FTM0_CH2, label: 'J11[A24]/J11[A70]', identifier: FB_AD10}
- {pin_num: B11, pin_signal: ADC0_SE15/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FB_AD13/I2S0_TXD0, label: 'J11[A25]/J11[A40]/J11[A67]', identifier: FTM0_CH0;I2S0_TXD0;FB_AD13}
- {pin_num: J1, pin_signal: ADC0_DP1, label: 'J11[A27]', identifier: ADC0_DP1}
- {pin_num: J2, pin_signal: ADC0_DM1, label: 'J11[A28]', identifier: ADC0_DM1}
- {pin_num: K1, pin_signal: ADC1_DP1, label: 'J11[A29]', identifier: ADC1_DP1}
- {pin_num: K2, pin_signal: ADC1_DM1, label: 'J11[A30]', identifier: ADC1_DM1}
- {pin_num: L3, pin_signal: DAC0_OUT/CMP1_IN3/ADC0_SE23, label: 'J11[A32]', identifier: DAC0_OUT}
- {pin_num: A12, pin_signal: ADC0_SE4b/CMP1_IN0/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FB_AD12/I2S0_TX_FS, label: 'J11[A39]/J11[A68]', identifier: FTM0_CH1;FB_AD12}
- {pin_num: B3, pin_signal: PTD10/UART5_RTS_b/FB_A18, label: 'J11[A52]'}
- {pin_num: B2, pin_signal: PTD11/SPI2_PCS0/UART5_CTS_b/SDHC0_CLKIN/FB_A19, label: 'J11[A53]'}
- {pin_num: B8, pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB_SOF_OUT/I2S0_RX_FS/FB_AD8, label: 'J11[A59]/J11[A72]', identifier: FB_AD8}
- {pin_num: M2, pin_signal: ADC1_DM0/ADC0_DM3, label: 'J11[D30]', identifier: ADC0_DM3}
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
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: K6, peripheral: TPIU, signal: SWO, pin_signal: PTA2/UART0_TX/FTM0_CH7/JTAG_TDO/TRACE_SWO/EZP_DO, drive_strength: low, pull_select: down, pull_enable: disable}
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

    /* PORTA2 (pin K6) is configured as TRACE_SWO */
    PORT_SetPinMux(PORTA, 2U, kPORT_MuxAlt7);

    PORTA->PCR[2] = ((PORTA->PCR[2] &
                      /* Mask bits to zero which are setting */
                      (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_ISF_MASK)))

                     /* Pull Select: Internal pulldown resistor is enabled on the corresponding pin, if the
                      * corresponding PE field is set. */
                     | PORT_PCR_PS(kPORT_PullDown)

                     /* Pull Enable: Internal pullup or pulldown resistor is not enabled on the corresponding pin. */
                     | PORT_PCR_PE(kPORT_PullDisable)

                     /* Drive Strength Enable: Low drive strength is configured on the corresponding pin, if pin
                      * is configured as a digital output. */
                     | PORT_PCR_DSE(kPORT_LowDriveStrength));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitButtonsPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: C8, peripheral: GPIOC, signal: 'GPIO, 6', pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/I2S0_MCLK, identifier: SW1, direction: INPUT,
    gpio_interrupt: kPORT_InterruptFallingEdge, slew_rate: slow, open_drain: disable, drive_strength: low, pull_select: up, pull_enable: enable, passive_filter: enable}
  - {pin_num: L7, peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: PTA4/LLWU_P3/FTM0_CH1/NMI_b/EZP_CS_b, direction: INPUT, gpio_interrupt: kPORT_InterruptFallingEdge,
    slew_rate: slow, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: enable}
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
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    gpio_pin_config_t SW3_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTA4 (pin L7)  */
    GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_PIN, &SW3_config);

    gpio_pin_config_t SW1_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC6 (pin C8)  */
    GPIO_PinInit(BOARD_SW1_GPIO, BOARD_SW1_PIN, &SW1_config);

    const port_pin_config_t SW3 = {/* Internal pull-up/down resistor is disabled */
                                   kPORT_PullDisable,
                                   /* Slow slew rate is configured */
                                   kPORT_SlowSlewRate,
                                   /* Passive filter is enabled */
                                   kPORT_PassiveFilterEnable,
                                   /* Open drain is disabled */
                                   kPORT_OpenDrainDisable,
                                   /* Low drive strength is configured */
                                   kPORT_LowDriveStrength,
                                   /* Pin is configured as PTA4 */
                                   kPORT_MuxAsGpio,
                                   /* Pin Control Register fields [15:0] are not locked */
                                   kPORT_UnlockRegister};
    /* PORTA4 (pin L7) is configured as PTA4 */
    PORT_SetPinConfig(BOARD_SW3_PORT, BOARD_SW3_PIN, &SW3);

    /* Interrupt configuration on PORTA4 (pin L7): Interrupt on falling edge */
    PORT_SetPinInterruptConfig(BOARD_SW3_PORT, BOARD_SW3_PIN, kPORT_InterruptFallingEdge);

    const port_pin_config_t SW1 = {/* Internal pull-up resistor is enabled */
                                   kPORT_PullUp,
                                   /* Slow slew rate is configured */
                                   kPORT_SlowSlewRate,
                                   /* Passive filter is enabled */
                                   kPORT_PassiveFilterEnable,
                                   /* Open drain is disabled */
                                   kPORT_OpenDrainDisable,
                                   /* Low drive strength is configured */
                                   kPORT_LowDriveStrength,
                                   /* Pin is configured as PTC6 */
                                   kPORT_MuxAsGpio,
                                   /* Pin Control Register fields [15:0] are not locked */
                                   kPORT_UnlockRegister};
    /* PORTC6 (pin C8) is configured as PTC6 */
    PORT_SetPinConfig(BOARD_SW1_PORT, BOARD_SW1_PIN, &SW1);

    /* Interrupt configuration on PORTC6 (pin C8): Interrupt on falling edge */
    PORT_SetPinInterruptConfig(BOARD_SW1_PORT, BOARD_SW1_PIN, kPORT_InterruptFallingEdge);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDsPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: E1, peripheral: GPIOE, signal: 'GPIO, 6', pin_signal: PTE6/SPI1_PCS3/UART3_CTS_b/I2S0_MCLK/FTM3_CH1/USB_SOF_OUT, direction: OUTPUT, gpio_init_state: 'true',
    slew_rate: slow, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: F4, peripheral: GPIOE, signal: 'GPIO, 7', pin_signal: PTE7/UART3_RTS_b/I2S0_RXD0/FTM3_CH2, direction: OUTPUT, gpio_init_state: 'true', slew_rate: slow,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: F3, peripheral: GPIOE, signal: 'GPIO, 8', pin_signal: PTE8/I2S0_RXD1/UART5_TX/I2S0_RX_FS/FTM3_CH3, direction: OUTPUT, gpio_init_state: 'true', slew_rate: slow,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: F2, peripheral: GPIOE, signal: 'GPIO, 9', pin_signal: PTE9/I2S0_TXD1/UART5_RX/I2S0_RX_BCLK/FTM3_CH4, direction: OUTPUT, gpio_init_state: 'true', slew_rate: slow,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
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
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    gpio_pin_config_t LED_GREEN_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTE6 (pin E1)  */
    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, &LED_GREEN_config);

    gpio_pin_config_t LED_YELLOW_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTE7 (pin F4)  */
    GPIO_PinInit(BOARD_LED_YELLOW_GPIO, BOARD_LED_YELLOW_PIN, &LED_YELLOW_config);

    gpio_pin_config_t LED_ORANGE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTE8 (pin F3)  */
    GPIO_PinInit(BOARD_LED_ORANGE_GPIO, BOARD_LED_ORANGE_PIN, &LED_ORANGE_config);

    gpio_pin_config_t LED_BLUE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTE9 (pin F2)  */
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
                                         /* Pin is configured as PTE6 */
                                         kPORT_MuxAsGpio,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTE6 (pin E1) is configured as PTE6 */
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
                                          /* Pin is configured as PTE7 */
                                          kPORT_MuxAsGpio,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTE7 (pin F4) is configured as PTE7 */
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
                                          /* Pin is configured as PTE8 */
                                          kPORT_MuxAsGpio,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTE8 (pin F3) is configured as PTE8 */
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
                                        /* Pin is configured as PTE9 */
                                        kPORT_MuxAsGpio,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTE9 (pin F2) is configured as PTE9 */
    PORT_SetPinConfig(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, &LED_BLUE);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitDebug_UARTPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: A11, peripheral: UART1, signal: RX, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: A9, peripheral: UART1, signal: TX, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/FB_AD11/CMP1_OUT, direction: OUTPUT, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitDebug_UARTPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitDebug_UARTPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

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
    /* PORTC3 (pin A11) is configured as UART1_RX */
    PORT_SetPinConfig(BOARD_DEBUG_UART_RX_PORT, BOARD_DEBUG_UART_RX_PIN, &DEBUG_UART_RX);

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
    /* PORTC4 (pin A9) is configured as UART1_TX */
    PORT_SetPinConfig(BOARD_DEBUG_UART_TX_PORT, BOARD_DEBUG_UART_TX_PIN, &DEBUG_UART_TX);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART1TXSRC_MASK)))

                  /* UART 1 transmit data source select: UART1_TX pin. */
                  | SIM_SOPT5_UART1TXSRC(SOPT5_UART1TXSRC_UART_TX));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitOSCPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: M12, peripheral: OSC, signal: EXTAL0, pin_signal: EXTAL0/PTA18/FTM0_FLT2/FTM_CLKIN0, slew_rate: no_init, open_drain: no_init, drive_strength: no_init,
    pull_select: no_init, pull_enable: no_init, passive_filter: no_init}
  - {pin_num: M6, peripheral: RTC, signal: EXTAL32, pin_signal: EXTAL32}
  - {pin_num: M7, peripheral: RTC, signal: XTAL32, pin_signal: XTAL32}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitOSCPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitOSCPins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);

    /* PORTA18 (pin M12) is configured as EXTAL0 */
    PORT_SetPinMux(BOARD_XTAL0_PORT, BOARD_XTAL0_PIN, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitUSB_otgPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: H2, peripheral: USB0, signal: DM, pin_signal: USB0_DM}
  - {pin_num: H1, peripheral: USB0, signal: DP, pin_signal: USB0_DP}
  - {pin_num: D7, peripheral: GPIOC, signal: 'GPIO, 9', pin_signal: ADC1_SE5b/CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/FTM2_FLT0, identifier: USB_ENABLE, direction: OUTPUT,
    gpio_init_state: 'false', slew_rate: slow, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: A8, peripheral: GPIOC, signal: 'GPIO, 8', pin_signal: ADC1_SE4b/CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7, identifier: USB_FLGA, direction: INPUT, slew_rate: fast,
    open_drain: enable, drive_strength: low, pull_select: up, pull_enable: enable, passive_filter: disable}
  - {pin_num: G3, peripheral: GPIOE, signal: 'GPIO, 12', pin_signal: PTE12/I2S0_TX_BCLK/FTM3_CH7, direction: INPUT, slew_rate: slow, open_drain: disable, drive_strength: low,
    pull_select: up, pull_enable: enable, passive_filter: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitUSB_otgPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitUSB_otgPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    gpio_pin_config_t USB_FLGA_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC8 (pin A8)  */
    GPIO_PinInit(BOARD_USB_FLGA_GPIO, BOARD_USB_FLGA_PIN, &USB_FLGA_config);

    gpio_pin_config_t USB_ENABLE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC9 (pin D7)  */
    GPIO_PinInit(BOARD_USB_ENABLE_GPIO, BOARD_USB_ENABLE_PIN, &USB_ENABLE_config);

    gpio_pin_config_t USB_ID_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTE12 (pin G3)  */
    GPIO_PinInit(BOARD_USB_ID_GPIO, BOARD_USB_ID_PIN, &USB_ID_config);

    const port_pin_config_t USB_FLGA = {/* Internal pull-up resistor is enabled */
                                        kPORT_PullUp,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is enabled */
                                        kPORT_OpenDrainEnable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as PTC8 */
                                        kPORT_MuxAsGpio,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTC8 (pin A8) is configured as PTC8 */
    PORT_SetPinConfig(BOARD_USB_FLGA_PORT, BOARD_USB_FLGA_PIN, &USB_FLGA);

    const port_pin_config_t USB_ENABLE = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Slow slew rate is configured */
                                          kPORT_SlowSlewRate,
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
    /* PORTC9 (pin D7) is configured as PTC9 */
    PORT_SetPinConfig(BOARD_USB_ENABLE_PORT, BOARD_USB_ENABLE_PIN, &USB_ENABLE);

    const port_pin_config_t USB_ID = {/* Internal pull-up resistor is enabled */
                                      kPORT_PullUp,
                                      /* Slow slew rate is configured */
                                      kPORT_SlowSlewRate,
                                      /* Passive filter is disabled */
                                      kPORT_PassiveFilterDisable,
                                      /* Open drain is disabled */
                                      kPORT_OpenDrainDisable,
                                      /* Low drive strength is configured */
                                      kPORT_LowDriveStrength,
                                      /* Pin is configured as PTE12 */
                                      kPORT_MuxAsGpio,
                                      /* Pin Control Register fields [15:0] are not locked */
                                      kPORT_UnlockRegister};
    /* PORTE12 (pin G3) is configured as PTE12 */
    PORT_SetPinConfig(BOARD_USB_ID_PORT, BOARD_USB_ID_PIN, &USB_ID);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitAccelerometerPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: C7, peripheral: I2C1, signal: SCL, pin_signal: ADC1_SE6b/PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5, identifier: ACCEL_SCL, slew_rate: fast, open_drain: enable,
    drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: B7, peripheral: I2C1, signal: SDA, pin_signal: ADC1_SE7b/PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/I2S0_RXD1/FB_RW_b, identifier: ACCEL_SDA, slew_rate: fast,
    open_drain: enable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: J7, peripheral: GPIOA, signal: 'GPIO, 6', pin_signal: PTA6/FTM0_CH3/CLKOUT/TRACE_CLKOUT, direction: INPUT, slew_rate: fast, open_drain: enable, drive_strength: low,
    pull_select: up, pull_enable: enable, passive_filter: disable}
  - {pin_num: K8, peripheral: GPIOA, signal: 'GPIO, 8', pin_signal: ADC0_SE11/PTA8/FTM1_CH0/FTM1_QD_PHA/TRACE_D2, direction: INPUT, slew_rate: fast, open_drain: enable,
    drive_strength: low, pull_select: up, pull_enable: enable, passive_filter: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitAccelerometerPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitAccelerometerPins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    gpio_pin_config_t ACCEL_INT1_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTA6 (pin J7)  */
    GPIO_PinInit(BOARD_ACCEL_INT1_GPIO, BOARD_ACCEL_INT1_PIN, &ACCEL_INT1_config);

    gpio_pin_config_t ACCEL_INT2_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTA8 (pin K8)  */
    GPIO_PinInit(BOARD_ACCEL_INT2_GPIO, BOARD_ACCEL_INT2_PIN, &ACCEL_INT2_config);

    const port_pin_config_t ACCEL_INT1 = {/* Internal pull-up resistor is enabled */
                                          kPORT_PullUp,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is enabled */
                                          kPORT_OpenDrainEnable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as PTA6 */
                                          kPORT_MuxAsGpio,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTA6 (pin J7) is configured as PTA6 */
    PORT_SetPinConfig(BOARD_ACCEL_INT1_PORT, BOARD_ACCEL_INT1_PIN, &ACCEL_INT1);

    const port_pin_config_t ACCEL_INT2 = {/* Internal pull-up resistor is enabled */
                                          kPORT_PullUp,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is enabled */
                                          kPORT_OpenDrainEnable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as PTA8 */
                                          kPORT_MuxAsGpio,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTA8 (pin K8) is configured as PTA8 */
    PORT_SetPinConfig(BOARD_ACCEL_INT2_PORT, BOARD_ACCEL_INT2_PIN, &ACCEL_INT2);

    const port_pin_config_t ACCEL_SCL = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
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
    /* PORTC10 (pin C7) is configured as I2C1_SCL */
    PORT_SetPinConfig(BOARD_ACCEL_SCL_PORT, BOARD_ACCEL_SCL_PIN, &ACCEL_SCL);

    const port_pin_config_t ACCEL_SDA = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
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
    /* PORTC11 (pin B7) is configured as I2C1_SDA */
    PORT_SetPinConfig(BOARD_ACCEL_SDA_PORT, BOARD_ACCEL_SDA_PIN, &ACCEL_SDA);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitSD_CardPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: D2, peripheral: SDHC, signal: 'DATA, 0', pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/SDHC0_D0/TRACE_D3/I2C1_SCL/SPI1_SIN, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: D3, peripheral: SDHC, signal: 'DATA, 1', pin_signal: ADC1_SE4a/PTE0/SPI1_PCS1/UART1_TX/SDHC0_D1/TRACE_CLKOUT/I2C1_SDA/RTC_CLKOUT, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: E2, peripheral: SDHC, signal: 'DATA, 2', pin_signal: PTE5/SPI1_PCS2/UART3_RX/SDHC0_D2/FTM3_CH0, slew_rate: fast, open_drain: disable, drive_strength: low,
    pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: E3, peripheral: SDHC, signal: 'DATA, 3', pin_signal: PTE4/LLWU_P2/SPI1_PCS0/UART3_TX/SDHC0_D3/TRACE_D0, slew_rate: fast, open_drain: disable, drive_strength: low,
    pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: E4, peripheral: SDHC, signal: CMD, pin_signal: ADC0_DM2/ADC1_SE7a/PTE3/SPI1_SIN/UART1_RTS_b/SDHC0_CMD/TRACE_D1/SPI1_SOUT, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: D1, peripheral: SDHC, signal: DCLK, pin_signal: ADC0_DP2/ADC1_SE6a/PTE2/LLWU_P1/SPI1_SCK/UART1_CTS_b/SDHC0_DCLK/TRACE_D2, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: A1, peripheral: GPIOD, signal: 'GPIO, 7', pin_signal: PTD7/CMT_IRO/UART0_TX/FTM0_CH7/FTM0_FLT1/SPI1_SIN, direction: INPUT, slew_rate: slow, open_drain: disable,
    drive_strength: low, pull_select: up, pull_enable: enable, passive_filter: disable, digital_filter: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitSD_CardPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitSD_CardPins(void)
{
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    gpio_pin_config_t SDHC_DETECT_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTD7 (pin A1)  */
    GPIO_PinInit(BOARD_SDHC_DETECT_GPIO, BOARD_SDHC_DETECT_PIN, &SDHC_DETECT_config);
    /* Configure digital filter */
    PORT_EnablePinsDigitalFilter(
        /* Digital filter is configured on port D */
        PORTD,
        /* Digital filter is configured for PORTD0 */
        PORT_DFER_DFE_7_MASK,
        /* Disable digital filter */
        false);

    const port_pin_config_t SDHC_DETECT = {/* Internal pull-up resistor is enabled */
                                           kPORT_PullUp,
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
    PORT_SetPinConfig(BOARD_SDHC_DETECT_PORT, BOARD_SDHC_DETECT_PIN, &SDHC_DETECT);

    const port_pin_config_t SDHC_D1 = {/* Internal pull-up/down resistor is disabled */
                                       kPORT_PullDisable,
                                       /* Fast slew rate is configured */
                                       kPORT_FastSlewRate,
                                       /* Passive filter is disabled */
                                       kPORT_PassiveFilterDisable,
                                       /* Open drain is disabled */
                                       kPORT_OpenDrainDisable,
                                       /* Low drive strength is configured */
                                       kPORT_LowDriveStrength,
                                       /* Pin is configured as SDHC0_D1 */
                                       kPORT_MuxAlt4,
                                       /* Pin Control Register fields [15:0] are not locked */
                                       kPORT_UnlockRegister};
    /* PORTE0 (pin D3) is configured as SDHC0_D1 */
    PORT_SetPinConfig(BOARD_SDHC_D1_PORT, BOARD_SDHC_D1_PIN, &SDHC_D1);

    const port_pin_config_t SDHC_D0 = {/* Internal pull-up/down resistor is disabled */
                                       kPORT_PullDisable,
                                       /* Fast slew rate is configured */
                                       kPORT_FastSlewRate,
                                       /* Passive filter is disabled */
                                       kPORT_PassiveFilterDisable,
                                       /* Open drain is disabled */
                                       kPORT_OpenDrainDisable,
                                       /* Low drive strength is configured */
                                       kPORT_LowDriveStrength,
                                       /* Pin is configured as SDHC0_D0 */
                                       kPORT_MuxAlt4,
                                       /* Pin Control Register fields [15:0] are not locked */
                                       kPORT_UnlockRegister};
    /* PORTE1 (pin D2) is configured as SDHC0_D0 */
    PORT_SetPinConfig(BOARD_SDHC_D0_PORT, BOARD_SDHC_D0_PIN, &SDHC_D0);

    const port_pin_config_t SDHC_CLK = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as SDHC0_DCLK */
                                        kPORT_MuxAlt4,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTE2 (pin D1) is configured as SDHC0_DCLK */
    PORT_SetPinConfig(BOARD_SDHC_CLK_PORT, BOARD_SDHC_CLK_PIN, &SDHC_CLK);

    const port_pin_config_t SDHC_CMD = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as SDHC0_CMD */
                                        kPORT_MuxAlt4,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTE3 (pin E4) is configured as SDHC0_CMD */
    PORT_SetPinConfig(BOARD_SDHC_CMD_PORT, BOARD_SDHC_CMD_PIN, &SDHC_CMD);

    const port_pin_config_t SDHC_D3 = {/* Internal pull-up/down resistor is disabled */
                                       kPORT_PullDisable,
                                       /* Fast slew rate is configured */
                                       kPORT_FastSlewRate,
                                       /* Passive filter is disabled */
                                       kPORT_PassiveFilterDisable,
                                       /* Open drain is disabled */
                                       kPORT_OpenDrainDisable,
                                       /* Low drive strength is configured */
                                       kPORT_LowDriveStrength,
                                       /* Pin is configured as SDHC0_D3 */
                                       kPORT_MuxAlt4,
                                       /* Pin Control Register fields [15:0] are not locked */
                                       kPORT_UnlockRegister};
    /* PORTE4 (pin E3) is configured as SDHC0_D3 */
    PORT_SetPinConfig(BOARD_SDHC_D3_PORT, BOARD_SDHC_D3_PIN, &SDHC_D3);

    const port_pin_config_t SDHC_D2 = {/* Internal pull-up/down resistor is disabled */
                                       kPORT_PullDisable,
                                       /* Fast slew rate is configured */
                                       kPORT_FastSlewRate,
                                       /* Passive filter is disabled */
                                       kPORT_PassiveFilterDisable,
                                       /* Open drain is disabled */
                                       kPORT_OpenDrainDisable,
                                       /* Low drive strength is configured */
                                       kPORT_LowDriveStrength,
                                       /* Pin is configured as SDHC0_D2 */
                                       kPORT_MuxAlt4,
                                       /* Pin Control Register fields [15:0] are not locked */
                                       kPORT_UnlockRegister};
    /* PORTE5 (pin E2) is configured as SDHC0_D2 */
    PORT_SetPinConfig(BOARD_SDHC_D2_PORT, BOARD_SDHC_D2_PIN, &SDHC_D2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPotentiometerPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: M3, peripheral: ADC1, signal: 'SE, 18', pin_signal: VREF_OUT/CMP1_IN5/CMP0_IN5/ADC1_SE18}
  - {pin_num: G5, peripheral: ADC1, signal: VREFH, pin_signal: VREFH}
  - {pin_num: G6, peripheral: ADC1, signal: VREFL, pin_signal: VREFL}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPotentiometerPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPotentiometerPins(void)
{
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
