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
processor: MK65FN2M0xxx18
package_id: MK65FN2M0VMI18
mcu_data: ksdk2_0
processor_version: 0.0.18
board: TWR-K65F180M
pin_labels:
- {pin_num: B1, pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/SDHC0_D0/TRACE_D3/I2C1_SCL/SPI1_SIN, label: 'J26[B11]/J29[7]/SDHC0_D0/SPI1_SOUT', identifier: SDHC_D0}
- {pin_num: A1, pin_signal: ADC1_SE4a/PTE0/SPI1_PCS1/UART1_TX/SDHC0_D1/TRACE_CLKOUT/I2C1_SDA/RTC_CLKOUT, label: 'J26[B22]/J29[8]/SDHC0_D1', identifier: SDHC_D1}
- {pin_num: C1, pin_signal: ADC1_SE6a/PTE2/LLWU_P1/SPI1_SCK/UART1_CTS_b/SDHC0_DCLK/TRACE_D2, label: 'J26[B7]/J29[5]/SDHC0_DCLK/SPI1_SCK', identifier: SDHC_DCLK}
- {pin_num: D1, pin_signal: ADC1_SE7a/PTE3/SPI1_SIN/UART1_RTS_b/SDHC0_CMD/TRACE_D1/SPI1_SOUT, label: 'J26[B10]/J29[3]/SDHC0_CMD/SPI1_SIN', identifier: SDHC_CMD}
- {pin_num: E1, pin_signal: PTE4/LLWU_P2/SPI1_PCS0/UART3_TX/SDHC0_D3/TRACE_D0, label: 'J26[B9]/J29[2]/SDHC0_D3/SPI1_PCS0', identifier: SDHC_D3}
- {pin_num: H1, pin_signal: USB1_DM/LLWU_P31, label: 'J15[2]/MCU_USB1_HS_DN', identifier: MCU_USB1_HS_DN_DCD;MCU_USB1_HS_DN_PHY}
- {pin_num: J1, pin_signal: USB1_DP/LLWU_P30, label: 'J15[3]/MCU_USB1_HS_DP', identifier: MCU_USB1_HS_DP_PHY;MCU_USB1_HS_DP_DCD}
- {pin_num: G1, pin_signal: USB0_DP, label: 'J26[A55]/MCU_USB0_FS_DP', identifier: MCU_USB0_FS_DP}
- {pin_num: F1, pin_signal: USB0_DM, label: 'J26[A54]/MCU_USB0_FS_DN', identifier: MCU_USB0_FS_DN}
- {pin_num: K1, pin_signal: USB1_VSS, label: GND}
- {pin_num: L1, pin_signal: ADC1_DP1, label: R124/R125/ADC1_DP1}
- {pin_num: M1, pin_signal: ADC1_DM1, label: TP19}
- {pin_num: N1, pin_signal: ADC1_DP0/ADC0_DP3, label: 'J11[9]/J26[A28]/TWRPI_ADC1', identifier: TWRPI_ADC1}
- {pin_num: A2, pin_signal: PTD14/SPI2_SIN/SDHC0_D6/FB_A22, label: 'J12[9]/J26[B44]/SPI2_SIN'}
- {pin_num: B2, pin_signal: PTD12/SPI2_SCK/FTM3_FLT0/SDHC0_D4/FB_A20, label: 'J12[12]/J26[B48]/SPI2_SCK'}
- {pin_num: C2, pin_signal: PTD11/LLWU_P25/SPI2_PCS0/SDHC0_CLKIN/LPUART0_CTS_b/FB_A19, label: 'J26[B46]/SPI2_PCS0'}
- {pin_num: D2, pin_signal: PTE5/SPI1_PCS2/UART3_RX/SDHC0_D2/FTM3_CH0, label: 'J26[A10]/J29[1]/SDHC0_D2', identifier: SDHC_D2}
- {pin_num: E2, pin_signal: PTE6/LLWU_P16/SPI1_PCS3/UART3_CTS_b/I2S0_MCLK/FTM3_CH1/USB0_SOF_OUT, label: 'J26[A21]/I2S0_MCLK'}
- {pin_num: F2, pin_signal: VREG_IN0, label: 'J23[2]/MCU_VREGIN0', identifier: MCU_VREGIN0}
- {pin_num: G2, pin_signal: VREG_OUT, label: 'J9[7]/MCU_VREGOUT', identifier: MCU_VREGOUT}
- {pin_num: K2, pin_signal: NC183, label: NC}
- {pin_num: H2, pin_signal: VREG_IN1, label: 'J23[3]/MCU_VREGIN1', identifier: MCU_VREGIN1}
- {pin_num: J2, pin_signal: USB1_VBUS/LLWU_P29, label: 'J15[1]/U12[6]/K65_USB1_VBUS', identifier: K65_USB1_VBUS}
- {pin_num: J7, pin_signal: NC184, label: NC}
- {pin_num: L4, pin_signal: TAMPER1, label: 'J4[2]'}
- {pin_num: J5, pin_signal: TAMPER7, label: 'J4[8]'}
- {pin_num: K5, pin_signal: TAMPER3, label: 'J4[4]'}
- {pin_num: L5, pin_signal: TAMPER2, label: 'J4[3]'}
- {pin_num: M5, pin_signal: TAMPER0/RTC_WAKEUP_B, label: 'J4[1]/RTC_WAKEUP', identifier: RTC_WAKEUP}
- {pin_num: J6, pin_signal: TAMPER6, label: 'J4[7]'}
- {pin_num: K6, pin_signal: TAMPER5, label: 'J4[6]'}
- {pin_num: L6, pin_signal: TAMPER4, label: 'J4[5]'}
- {pin_num: L2, pin_signal: ADC0_DM0/ADC1_DM3, label: 'J11[18]/TWRPI_ID1', identifier: TWRPI_ID1}
- {pin_num: M2, pin_signal: ADC0_DP0/ADC1_DP3, label: 'J11[17]/TWRPI_ID0', identifier: TWRPI_ID0}
- {pin_num: N2, pin_signal: ADC1_DM0/ADC0_DM3, label: 'J11[12]/J26[A29]/TWRPI_ADC2', identifier: TWRPI_ADC2}
- {pin_num: A3, pin_signal: PTD15/SPI2_PCS1/SDHC0_D7/FB_A23, label: 'J12[11]/J15[4]/J26[B47]/microUSB_ID/SPI2_PCS1', identifier: microUSB_ID}
- {pin_num: B3, pin_signal: PTD13/SPI2_SOUT/SDHC0_D5/FB_A21, label: 'J12[10]/J26[B45]/SPI2_SOUT'}
- {pin_num: J4, pin_signal: VSSA, label: GND}
- {pin_num: C3, pin_signal: VSS6, label: GND}
- {pin_num: G3, pin_signal: VSS22, label: GND}
- {pin_num: N7, pin_signal: VSS67, label: GND}
- {pin_num: G8, pin_signal: VSS17, label: GND}
- {pin_num: H8, pin_signal: VSS81, label: GND}
- {pin_num: J8, pin_signal: VSS172, label: GND}
- {pin_num: C11, pin_signal: VSS139, label: GND}
- {pin_num: M12, pin_signal: VSS95, label: GND}
- {pin_num: D3, pin_signal: PTD10/LPUART0_RTS_b/FB_A18, label: 'J26[A63]/J26[B23]/RESET_OUT'}
- {pin_num: E3, pin_signal: PTE7/UART3_RTS_b/I2S0_RXD0/FTM3_CH2, label: 'J26[A24]/I2S0_RXD0'}
- {pin_num: F3, pin_signal: PTE9/LLWU_P17/I2S0_TXD1/I2S0_RX_BCLK/LPUART0_RX/FTM3_CH4, label: 'J26[A58]/I2S0_RX_BCLK'}
- {pin_num: H3, pin_signal: ADC0_SE4a/PTE16/SPI0_PCS0/UART2_TX/FTM_CLKIN0/FTM0_FLT3/TPM_CLKIN0, label: 'J33[2]/U5[4]/UART2_TX', identifier: DEBUG_UART_TX}
- {pin_num: J3, pin_signal: VDDA, label: MCU_PWR}
- {pin_num: G5, pin_signal: VDD5, label: MCU_PWR}
- {pin_num: H5, pin_signal: VDD80, label: MCU_PWR}
- {pin_num: G6, pin_signal: VDD16, label: MCU_PWR}
- {pin_num: H6, pin_signal: VDD140, label: MCU_PWR}
- {pin_num: G7, pin_signal: VDD66, label: MCU_PWR}
- {pin_num: N12, pin_signal: VDD94, label: MCU_PWR}
- {pin_num: H7, pin_signal: VDD173, label: MCU_PWR}
- {pin_num: K3, pin_signal: VREFH, label: MCU_PWR}
- {pin_num: K4, pin_signal: VREFL, label: GND}
- {pin_num: N3, pin_signal: VREF_OUT/CMP1_IN5/CMP0_IN5/ADC1_SE18, label: TP5/VREF_OUT}
- {pin_num: L3, pin_signal: ADC0_SE16/CMP1_IN2/ADC0_SE21, label: 'J11[8]/J26[A27]/J26[A30]/TWRPI_ADC0', identifier: TWRPI_ADC0}
- {pin_num: M3, pin_signal: ADC1_SE16/CMP2_IN2/ADC0_SE22, label: 'J24[1]/J26[D30]/R67[2]/POTJ', identifier: POTJ}
- {pin_num: A4, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/SDRAM_A12/I2C0_SCL, label: 'J26[A76]/U14[60]/FB_AD4/SDRAM_A12', identifier: SDRAM_A12}
- {pin_num: B4, pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/SDRAM_A11/I2C0_SDA, label: 'J26[A77]/U14[61]/FB_AD3/SDRAM_A11', identifier: SDRAM_A11}
- {pin_num: C4, pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FB_AD1/SDRAM_A9/EWM_OUT_b/SPI1_SCK, label: 'J26[A79]/U14[63]/FB_AD1/SDRAM_A9',
  identifier: SDRAM_A9}
- {pin_num: D4, pin_signal: PTD9/I2C0_SDA/LPUART0_TX/FB_A17, label: 'J14[1]/U12[4]/K65_USB_FLGA', identifier: K65_USB_FLGA}
- {pin_num: E4, pin_signal: PTE8/I2S0_RXD1/I2S0_RX_FS/LPUART0_TX/FTM3_CH3, label: 'J26[A59]/I2S0_RX_FS'}
- {pin_num: F4, pin_signal: PTE10/LLWU_P18/I2C3_SDA/I2S0_TXD0/LPUART0_CTS_b/FTM3_CH5/USB1_ID, label: 'J26[A25]/I2S0_TXD0'}
- {pin_num: G4, pin_signal: PTE11/I2C3_SCL/I2S0_TX_FS/LPUART0_RTS_b/FTM3_CH6, label: 'J26[A23]/I2S0_TX_FS'}
- {pin_num: H4, pin_signal: PTE12/I2S0_TX_BCLK/FTM3_CH7, label: 'J26[A22]/I2S0_TX_BCLK'}
- {pin_num: M4, pin_signal: DAC0_OUT/CMP1_IN3/ADC0_SE23, label: 'J26[A32]/DAC0_OUT'}
- {pin_num: N4, pin_signal: DAC1_OUT/CMP0_IN4/CMP2_IN3/ADC1_SE23, label: 'J26[B32]/DAC1_OUT'}
- {pin_num: A5, pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FB_CS0_b, label: 'J26[A9]/J26[B64]/FB_CS0/UART2_CTS'}
- {pin_num: B5, pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FB_AD2/SDRAM_A10/EWM_IN/SPI1_PCS0, label: 'J26[A78]/U14[62]/FB_AD2/SDRAM_A10', identifier: SDRAM_A10}
- {pin_num: C5, pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/UART0_RX/FTM0_CH6/FB_AD0/FTM0_FLT0/SPI1_SOUT, label: 'J26[A80]/FB_AD0'}
- {pin_num: D5, pin_signal: PTD8/LLWU_P24/I2C0_SCL/LPUART0_RX/FB_A16, label: 'J13[2]/U12[3]/microUSB_PWR_EN', identifier: microUSB_PWR_EN}
- {pin_num: E5, pin_signal: PTD7/CMT_IRO/UART0_TX/FTM0_CH7/SDRAM_CKE/FTM0_FLT1/SPI1_SIN, label: 'U14[67]/SDRAM_CKE', identifier: SDRAM_CKE}
- {pin_num: F5, pin_signal: ADC0_SE5a/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3/TPM_CLKIN1, label: 'J34[2]/U6[2]/UART2_RX', identifier: DEBUG_UART_RX}
- {pin_num: N6, pin_signal: EXTAL32, label: 'Y1[2]/EXTAL32', identifier: EXTAL32K_CLK}
- {pin_num: N5, pin_signal: XTAL32, label: 'Y1[1]/XTAL32', identifier: XTAL32K_CLK}
- {pin_num: A6, pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b, label: 'J26[B21]/J26[B63]/UART2_RTS/FB_ALE/FB_CS1'}
- {pin_num: B6, pin_signal: PTC29/I2C3_SCL/ENET0_1588_TMR3/FB_A0/SDRAM_D0, label: 'U14[2]/SDRAM_D0', identifier: SDRAM_D0}
- {pin_num: C6, pin_signal: PTC28/I2C3_SDA/ENET0_1588_TMR2/FB_A1/SDRAM_D1, label: 'U14[4]/SDRAM_D1', identifier: SDRAM_D1}
- {pin_num: D6, pin_signal: PTC27/LPUART0_RTS_b/ENET0_1588_TMR1/FB_A2/SDRAM_D2, label: 'U14[5]/SDRAM_D2', identifier: SDRAM_D2}
- {pin_num: E6, pin_signal: PTC26/LPUART0_CTS_b/ENET0_1588_TMR0/FB_A3/SDRAM_D3, label: 'U14[7]/SDRAM_D3', identifier: SDRAM_D3}
- {pin_num: F6, pin_signal: ADC0_SE6a/PTE18/LLWU_P20/SPI0_SOUT/UART2_CTS_b/I2C0_SDA, label: 'J12[4]/J26[A8]/U15[6]/I2C0_SDA', identifier: ACCEL_I2C_SDA}
- {pin_num: M6, pin_signal: VBAT, label: 'J3[2]/K65_VBAT'}
- {pin_num: A7, pin_signal: PTC25/LPUART0_RX/FB_A4/SDRAM_D4, label: 'U14[8]/SDRAM_D4', identifier: SDRAM_D4}
- {pin_num: B7, pin_signal: PTC24/LPUART0_TX/FB_A5/SDRAM_D5, label: 'U14[10]/SDRAM_D5', identifier: SDRAM_D5}
- {pin_num: C7, pin_signal: PTC19/UART3_CTS_b/ENET0_1588_TMR3/FB_CS3_b/FB_BE7_0_BLS31_24_b/SDRAM_DQM0/FB_TA_b, label: 'U14[16]/SDRAM_DQM0', identifier: SDRAM_DQM0}
- {pin_num: D7, pin_signal: PTC18/UART3_RTS_b/ENET0_1588_TMR2/FB_TBST_b/FB_CS2_b/FB_BE15_8_BLS23_16_b/SDRAM_DQM1, label: 'U14[71]/SDRAM_DQM1', identifier: SDRAM_DQM1}
- {pin_num: E7, pin_signal: PTC17/CAN1_TX/UART3_TX/ENET0_1588_TMR1/FB_CS4_b/FB_TSIZ0/FB_BE31_24_BLS7_0_b/SDRAM_DQM3, label: 'U14[59]/SDRAM_DQM3', identifier: SDRAM_DQM3}
- {pin_num: F7, pin_signal: ADC0_SE7a/PTE19/SPI0_SIN/UART2_RTS_b/I2C0_SCL/CMP3_OUT, label: 'J12[3]/J26[A7]/U15[4]/I2C0_SCL', identifier: ACCEL_I2C_SCL}
- {pin_num: K7, pin_signal: ADC0_SE18/PTE25/LLWU_P21/CAN1_RX/UART4_RX/I2C0_SDA/EWM_IN, label: 'J26[A47]/J26[B30]/CAN1_RX/ADC0_SE18'}
- {pin_num: L7, pin_signal: ADC0_SE17/PTE24/CAN1_TX/UART4_TX/I2C0_SCL/EWM_OUT_b, label: 'J26[A48]/J26[B29]/CAN1_TX/ADC0_SE17'}
- {pin_num: M7, pin_signal: PTE28, label: 'J26[B61]/J26[B62]/U15[9]/ACCL_INT2', identifier: ACCEL_I2C_INT2}
- {pin_num: A8, pin_signal: ADC1_SE6b/PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5/SDRAM_A13, label: 'J26[A75]/U14[27]/FB_AD5/SDRAM_A13', identifier: SDRAM_A13}
- {pin_num: B8, pin_signal: PTC13/UART4_CTS_b/FTM_CLKIN1/FB_AD26/SDRAM_D26/TPM_CLKIN1, label: 'J26[B78]/U14[48]/FB_AD26/SDRAM_D26', identifier: SDRAM_D26}
- {pin_num: C8, pin_signal: PTC14/UART4_RX/FB_AD25/SDRAM_D25, label: 'J26[B79]/U14[47]/FB_AD25/SDRAM_D25', identifier: SDRAM_D25}
- {pin_num: D8, pin_signal: PTC15/UART4_TX/FB_AD24/SDRAM_D24, label: 'J26[B80]/U14[45]/FB_AD24/SDRAM_D24', identifier: SDRAM_D24}
- {pin_num: E8, pin_signal: PTC16/CAN1_RX/UART3_RX/ENET0_1588_TMR0/FB_CS5_b/FB_TSIZ1/FB_BE23_16_BLS15_8_b/SDRAM_DQM2, label: 'U14[28]/SDRAM_DQM2', identifier: SDRAM_DQM2}
- {pin_num: F8, pin_signal: TSI0_CH9/PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/SDRAM_D17/EWM_IN/TPM_CLKIN0, label: 'J26[B68]/U14[33]/FB_AD17/SDRAM_D17', identifier: SDRAM_D17}
- {pin_num: K8, pin_signal: PTE26/ENET_1588_CLKIN/UART4_CTS_b/RTC_CLKOUT/USB0_CLKIN, label: 'J26[B24]/ENET_1588_CLKIN', identifier: RMII_CLKIN;ENET_1588_CLKIN}
- {pin_num: L8, pin_signal: PTE27/UART4_RTS_b, label: 'J26[B55]/J26[B56]/U15[11]/ACCL_INT1', identifier: ACCEL_I2C_INT1}
- {pin_num: M8, pin_signal: TSI0_CH4/PTA3/UART0_RTS_b/FTM0_CH0/LPUART0_RTS_b/JTAG_TMS/SWD_DIO, label: 'J18[2]/U7[3]/JTAG_TMS/SWD_DIO', identifier: SWD_DIO}
- {pin_num: N8, pin_signal: TSI0_CH1/PTA0/UART0_CTS_b/UART0_COL_b/FTM0_CH5/LPUART0_CTS_b/JTAG_TCLK/SWD_CLK/EZP_CLK, label: 'J18[4]/U6[3]/JTAG_TCLK/SWD_CLK', identifier: SWD_CLK}
- {pin_num: A9, pin_signal: ADC1_SE7b/PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/I2S0_RXD1/FB_RW_b, label: 'J26[B57]/J26[B58]/J26[B71]/FB_RW'}
- {pin_num: B9, pin_signal: PTC12/UART4_RTS_b/FTM_CLKIN0/FB_AD27/SDRAM_D27/FTM3_FLT0/TPM_CLKIN0, label: 'J26[B77]/U14[50]/FB_AD27/SDRAM_D27', identifier: SDRAM_D27}
- {pin_num: C9, pin_signal: ADC1_SE5b/CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/SDRAM_A14/FTM2_FLT0, label: 'J26[A74]/U14[26]/FB_AD6/SDRAM_A14', identifier: SDRAM_A14}
- {pin_num: D9, pin_signal: PTB21/SPI2_SCK/FB_AD30/SDRAM_D30/CMP1_OUT, label: 'J26[B74]/U14[54]/FB_AD30/SDRAM_D30', identifier: SDRAM_D30}
- {pin_num: E9, pin_signal: PTB15/CAN1_RX/FB_A6/SDRAM_D6, label: 'U14[11]/SDRAM_D6', identifier: SDRAM_D6}
- {pin_num: F9, pin_signal: PTB9/SPI1_PCS1/UART3_CTS_b/FB_AD20/SDRAM_D20, label: 'U14[37]/SDRAM_D20', identifier: SDRAM_D20}
- {pin_num: G9, pin_signal: ADC1_SE14/PTB10/SPI1_PCS0/UART3_RX/FB_AD19/SDRAM_D19/FTM0_FLT1, label: 'J26[B70]/U14[36]/FB_AD19/SDRAM_D19', identifier: SDRAM_D19}
- {pin_num: H9, pin_signal: ADC1_SE10/PTB4/ENET0_1588_TMR2/SDRAM_CS1_b/FTM1_FLT0, label: D6/LED_1, identifier: LED_YELLOW}
- {pin_num: J9, pin_signal: PTA31/CAN0_RX/FB_A10/SDRAM_D10, label: 'J26[B41]/U14[77]/SDRAM_D10/CAN0_RX', identifier: SDRAM_D10}
- {pin_num: K9, pin_signal: ADC0_SE11/PTA8/FTM1_CH0/RMII0_MDC/MII0_MDC/FTM1_QD_PHA/TPM1_CH0/TRACE_D2, label: 'J26[A13]/RMII0_MDC', identifier: RMII0_MDC}
- {pin_num: L9, pin_signal: TSI0_CH5/PTA4/LLWU_P3/FTM0_CH1/NMI_b/EZP_CS_b, label: SW2/NMI, identifier: SW2}
- {pin_num: M9, pin_signal: TSI0_CH3/PTA2/UART0_TX/FTM0_CH7/I2C3_SCL/LPUART0_TX/JTAG_TDO/TRACE_SWO/EZP_DO, label: 'J26[A42]/UART0_TX'}
- {pin_num: N9, pin_signal: TSI0_CH2/PTA1/UART0_RX/FTM0_CH6/I2C3_SDA/LPUART0_RX/JTAG_TDI/EZP_DI, label: 'J26[A41]/UART0_RX'}
- {pin_num: A10, pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/SDRAM_A17/I2S0_MCLK, label: 'J26[A71]/U14[64]/FB_AD9/SDRAM_A17', identifier: SDRAM_A17}
- {pin_num: B10, pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB0_SOF_OUT/I2S0_RX_FS/FB_AD8/SDRAM_A16, label: 'J26[A72]/FB_AD8'}
- {pin_num: C10, pin_signal: ADC1_SE4b/CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7/SDRAM_A15, label: 'J26[A73]/U14[25]/FB_AD7/SDRAM_A15', identifier: SDRAM_A15}
- {pin_num: D10, pin_signal: PTB20/SPI2_PCS0/FB_AD31/SDRAM_D31/CMP0_OUT, label: 'J26[B73]/U14[56]/FB_AD31/SDRAM_D31', identifier: SDRAM_D31}
- {pin_num: F10, pin_signal: PTB8/UART3_RTS_b/FB_AD21/SDRAM_D21, label: 'U14[39]/SDRAM_D21', identifier: SDRAM_D21}
- {pin_num: E10, pin_signal: PTB14/CAN1_TX/FB_A7/SDRAM_D7, label: 'U14[13]/SDRAM_D7', identifier: SDRAM_D7}
- {pin_num: G10, pin_signal: ADC0_SE13/TSI0_CH8/PTB3/I2C0_SDA/UART0_CTS_b/UART0_COL_b/ENET0_1588_TMR1/SDRAM_CS0_b/FTM0_FLT0, label: 'U14[20]/SDRAM_CS0', identifier: SDRAM_CS0}
- {pin_num: H10, pin_signal: PTA30/CAN0_TX/FB_A11/SDRAM_D11, label: 'J26[B42]/U14[79]/SDRAM_D11/CAN0_TX', identifier: SDRAM_D11}
- {pin_num: J10, pin_signal: PTA26/MII0_TXD3/FB_A13/SDRAM_D13/FB_A27, label: 'U14[82]/SDRAM_D13', identifier: SDRAM_D13}
- {pin_num: K10, pin_signal: PTA9/FTM1_CH1/MII0_RXD3/FTM1_QD_PHB/TPM1_CH1/TRACE_D1, label: 'J26[A11]/J29[9]/SD_DETECT', identifier: SDHC_CD}
- {pin_num: L10, pin_signal: ADC0_SE10/PTA7/FTM0_CH4/RMII0_MDIO/MII0_MDIO/TRACE_D3, label: 'J26[A14]/RMII0_MDIO', identifier: RMII0_MDIO}
- {pin_num: M10, pin_signal: PTA6/FTM0_CH3/CLKOUT/TRACE_CLKOUT, label: 'J26[B27]/U14[68]/CLKOUT', identifier: SDRAM_CLKOUT}
- {pin_num: N10, pin_signal: PTA5/USB0_CLKIN/FTM0_CH2/RMII0_RXER/MII0_RXER/CMP2_OUT/I2S0_TX_BCLK/JTAG_TRST_b, label: 'J26[B13]/RMII0_RXER', identifier: RMII0_RXER}
- {pin_num: A11, pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/FB_AD10/SDRAM_A18/CMP0_OUT/FTM0_CH2, label: 'J26[A70]/U14[65]/FB_AD10/SDRAM_A18', identifier: SDRAM_A18}
- {pin_num: B11, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT, label: 'J26[A69]/U14[66]/FB_AD11/SDRAM_A19', identifier: SDRAM_A19}
- {pin_num: D11, pin_signal: TSI0_CH12/PTB19/CAN0_RX/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB/TPM2_CH1, label: 'J26[B72]/D8[1]/BRD_PAD1', identifier: TSI_ELECTRODE_2}
- {pin_num: E11, pin_signal: PTB13/UART3_CTS_b/FTM1_CH1/FTM0_CH5/FB_A8/SDRAM_D8/FTM1_QD_PHB/TPM1_CH1, label: 'U14[74]/SDRAM_D8', identifier: SDRAM_D8}
- {pin_num: F11, pin_signal: ADC1_SE13/PTB7/FB_AD22/SDRAM_D22, label: 'U14[40]/SDRAM_D22', identifier: SDRAM_D22}
- {pin_num: G11, pin_signal: ADC0_SE12/TSI0_CH7/PTB2/I2C0_SCL/UART0_RTS_b/ENET0_1588_TMR0/SDRAM_WE/FTM0_FLT3, label: 'U14[17]/SDRAM_WE', identifier: SDRAM_WE}
- {pin_num: H11, pin_signal: PTA29/MII0_COL/FB_A24, label: 'J26[A35]/D9/TP_LED2RR', identifier: LED_BLUE}
- {pin_num: J11, pin_signal: CMP3_IN5/PTA25/MII0_TXCLK/FB_A14/SDRAM_D14/FB_A28, label: 'U14[83]/SDRAM_D14', identifier: SDRAM_D14}
- {pin_num: K11, pin_signal: CMP3_IN4/PTA24/MII0_TXD2/FB_A15/SDRAM_D15/FB_A29, label: 'U14[85]/SDRAM_D15', identifier: SDRAM_D15}
- {pin_num: L11, pin_signal: CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/RMII0_RXD0/MII0_RXD0/I2C2_SDA/I2S0_TX_FS/FTM1_QD_PHB/TPM1_CH1, label: 'J26[A20]/RMII0_RXD0',
  identifier: RMII0_RXD0}
- {pin_num: M11, pin_signal: PTA11/LLWU_P23/FTM2_CH1/MII0_RXCLK/I2C2_SDA/FTM2_QD_PHB/TPM2_CH1, label: 'U16[9]/RTC_BYPASS', identifier: RTC_BYPASS}
- {pin_num: N11, pin_signal: PTA10/LLWU_P22/FTM2_CH0/MII0_RXD2/FTM2_QD_PHA/TPM2_CH0/TRACE_D0, label: 'J26[B52]/SW3', identifier: SW3}
- {pin_num: A12, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK, label: 'J26[B25]/J26[B28]/J26[B59]/J26[B60]'}
- {pin_num: B12, pin_signal: ADC0_SE15/TSI0_CH14/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FB_AD13/SDRAM_A21/I2S0_TXD0, label: 'J26[A67]/U14[22]/FB_AD13/SDRAM_A21',
  identifier: SDRAM_A21}
- {pin_num: C12, pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28/SDRAM_D28/CMP3_OUT, label: 'J26[B76]/U14[51]/FB_AD28/SDRAM_D28', identifier: SDRAM_D28}
- {pin_num: D12, pin_signal: TSI0_CH11/PTB18/CAN0_TX/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/SDRAM_A23/FTM2_QD_PHA/TPM2_CH0, label: 'J26[B66]/D9[1]/BRD_PAD2', identifier: TSI_ELECTRODE_1}
- {pin_num: E12, pin_signal: PTB12/UART3_RTS_b/FTM1_CH0/FTM0_CH4/FB_A9/SDRAM_D9/FTM1_QD_PHA/TPM1_CH0, label: 'U14[76]/SDRAM_D9', identifier: SDRAM_D9}
- {pin_num: F12, pin_signal: ADC1_SE12/PTB6/FB_AD23/SDRAM_D23, label: 'U14[42]/SDRAM_D23', identifier: SDRAM_D23}
- {pin_num: G12, pin_signal: ADC0_SE9/ADC1_SE9/TSI0_CH6/PTB1/I2C0_SDA/FTM1_CH1/RMII0_MDC/MII0_MDC/SDRAM_RAS_b/FTM1_QD_PHB/TPM1_CH1, label: 'U14[19]/SDRAM_RAS', identifier: SDRAM_RAS}
- {pin_num: H12, pin_signal: PTA28/MII0_TXER/FB_A25, label: 'J26[B35]/D8/TP_LED1RR', identifier: LED_GREEN}
- {pin_num: J12, pin_signal: ADC1_SE17/PTA17/SPI0_SIN/UART0_RTS_b/RMII0_TXD1/MII0_TXD1/I2S0_MCLK, label: 'J26[B19]/RMII0_TXD1', identifier: RMII0_TXD1}
- {pin_num: K12, pin_signal: CMP3_IN1/PTA15/SPI0_SCK/UART0_RX/RMII0_TXEN/MII0_TXEN/I2S0_RXD0, label: 'J26[B15]/RMII0_TXEN', identifier: RMII0_TXEN}
- {pin_num: L12, pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/I2C2_SCL/I2S0_TXD0/FTM1_QD_PHA/TPM1_CH0, label: 'J26[A19]/RMII0_RXD1', identifier: RMII0_RXD1}
- {pin_num: A13, pin_signal: ADC0_SE4b/CMP1_IN0/TSI0_CH15/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FB_AD12/SDRAM_A20/I2S0_TX_FS, label: 'J26[A68]/U14[24]/FB_AD12/SDRAM_A20',
  identifier: SDRAM_A20}
- {pin_num: B13, pin_signal: ADC0_SE14/TSI0_CH13/PTC0/SPI0_PCS4/PDB0_EXTRG/USB0_SOF_OUT/FB_AD14/SDRAM_A22/I2S0_TXD1, label: 'J26[A66]/U14[23]/FB_AD14/SDRAM_A22',
  identifier: SDRAM_A22}
- {pin_num: C13, pin_signal: PTB22/SPI2_SOUT/FB_AD29/SDRAM_D29/CMP2_OUT, label: 'J26[B75]/U14[53]/FB_AD29/SDRAM_D29', identifier: SDRAM_D29}
- {pin_num: D13, pin_signal: TSI0_CH10/PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/SDRAM_D16/EWM_OUT_b/TPM_CLKIN1, label: 'J26[B67]/U14[31]/FB_AD16/SDRAM_D16', identifier: SDRAM_D16}
- {pin_num: E13, pin_signal: ADC1_SE15/PTB11/SPI1_SCK/UART3_TX/FB_AD18/SDRAM_D18/FTM0_FLT2, label: 'J26[B69]/U14[34]/FB_AD18/SDRAM_D18', identifier: SDRAM_D18}
- {pin_num: F13, pin_signal: ADC1_SE11/PTB5/ENET0_1588_TMR3/FTM2_FLT0, label: D7/LED_2, identifier: LED_ORANGE}
- {pin_num: G13, pin_signal: ADC0_SE8/ADC1_SE8/TSI0_CH0/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/RMII0_MDIO/MII0_MDIO/SDRAM_CAS_b/FTM1_QD_PHA/TPM1_CH0, label: 'U14[18]/SDRAM_CAS',
  identifier: SDRAM_CAS}
- {pin_num: H13, pin_signal: PTA27/MII0_CRS/FB_A12/SDRAM_D12/FB_A26, label: 'U14[80]/SDRAM_D12', identifier: SDRAM_D12}
- {pin_num: J13, pin_signal: CMP3_IN2/PTA16/SPI0_SOUT/UART0_CTS_b/UART0_COL_b/RMII0_TXD0/MII0_TXD0/I2S0_RX_FS/I2S0_RXD1, label: 'J26[B20]/RMII0_TXD0', identifier: RMII0_TXD0}
- {pin_num: K13, pin_signal: PTA14/SPI0_PCS0/UART0_TX/RMII0_CRS_DV/MII0_RXDV/I2C2_SCL/I2S0_RX_BCLK/I2S0_TXD1, label: 'J26[A16]/RMII0_CRS_DV', identifier: RMII0_CRS_DV}
- {pin_num: L13, pin_signal: RESET_b, label: 'J26[A62]/J11[20]/J18[10]/U3[3]/RESET'}
- {pin_num: M13, pin_signal: XTAL0/PTA19/FTM1_FLT0/FTM_CLKIN1/LPTMR0_ALT1/TPM_CLKIN1, label: 'Y2[1]/XTAL0', identifier: XTAL0}
- {pin_num: N13, pin_signal: EXTAL0/PTA18/FTM0_FLT2/FTM_CLKIN0/TPM_CLKIN0, label: 'Y2[3]/EXTAL0', identifier: EXTAL0}
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
  - {pin_num: M9, peripheral: TPIU, signal: SWO, pin_signal: TSI0_CH3/PTA2/UART0_TX/FTM0_CH7/I2C3_SCL/LPUART0_TX/JTAG_TDO/TRACE_SWO/EZP_DO, pull_select: down, pull_enable: disable}
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

    /* PORTA2 (pin M9) is configured as TRACE_SWO */
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
BOARD_InitBUTTONsPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: L9, peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: TSI0_CH5/PTA4/LLWU_P3/FTM0_CH1/NMI_b/EZP_CS_b, direction: INPUT, gpio_interrupt: kPORT_InterruptFallingEdge,
    slew_rate: fast, open_drain: disable, pull_select: up, pull_enable: enable, passive_filter: disable}
  - {pin_num: N11, peripheral: GPIOA, signal: 'GPIO, 10', pin_signal: PTA10/LLWU_P22/FTM2_CH0/MII0_RXD2/FTM2_QD_PHA/TPM2_CH0/TRACE_D0, direction: INPUT, gpio_interrupt: kPORT_InterruptFallingEdge,
    slew_rate: fast, open_drain: disable, pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBUTTONsPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBUTTONsPins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);

    gpio_pin_config_t SW2_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTA4 (pin L9)  */
    GPIO_PinInit(BOARD_SW2_GPIO, BOARD_SW2_PIN, &SW2_config);

    gpio_pin_config_t SW3_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTA10 (pin N11)  */
    GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_PIN, &SW3_config);

    const port_pin_config_t SW3 = {/* Internal pull-up resistor is enabled */
                                   kPORT_PullUp,
                                   /* Fast slew rate is configured */
                                   kPORT_FastSlewRate,
                                   /* Passive filter is disabled */
                                   kPORT_PassiveFilterDisable,
                                   /* Open drain is disabled */
                                   kPORT_OpenDrainDisable,
                                   /* Low drive strength is configured */
                                   kPORT_LowDriveStrength,
                                   /* Pin is configured as PTA10 */
                                   kPORT_MuxAsGpio,
                                   /* Pin Control Register fields [15:0] are not locked */
                                   kPORT_UnlockRegister};
    /* PORTA10 (pin N11) is configured as PTA10 */
    PORT_SetPinConfig(BOARD_SW3_PORT, BOARD_SW3_PIN, &SW3);

    /* Interrupt configuration on PORTA10 (pin N11): Interrupt on falling edge */
    PORT_SetPinInterruptConfig(BOARD_SW3_PORT, BOARD_SW3_PIN, kPORT_InterruptFallingEdge);

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
                                   /* Pin is configured as PTA4 */
                                   kPORT_MuxAsGpio,
                                   /* Pin Control Register fields [15:0] are not locked */
                                   kPORT_UnlockRegister};
    /* PORTA4 (pin L9) is configured as PTA4 */
    PORT_SetPinConfig(BOARD_SW2_PORT, BOARD_SW2_PIN, &SW2);

    /* Interrupt configuration on PORTA4 (pin L9): Interrupt on falling edge */
    PORT_SetPinInterruptConfig(BOARD_SW2_PORT, BOARD_SW2_PIN, kPORT_InterruptFallingEdge);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDsPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: H9, peripheral: GPIOB, signal: 'GPIO, 4', pin_signal: ADC1_SE10/PTB4/ENET0_1588_TMR2/SDRAM_CS1_b/FTM1_FLT0, direction: OUTPUT, gpio_init_state: 'true',
    slew_rate: slow, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: F13, peripheral: GPIOB, signal: 'GPIO, 5', pin_signal: ADC1_SE11/PTB5/ENET0_1588_TMR3/FTM2_FLT0, direction: OUTPUT, gpio_init_state: 'true', slew_rate: slow,
    open_drain: disable, pull_select: down, pull_enable: disable}
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
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    gpio_pin_config_t LED_YELLOW_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTB4 (pin H9)  */
    GPIO_PinInit(BOARD_LED_YELLOW_GPIO, BOARD_LED_YELLOW_PIN, &LED_YELLOW_config);

    gpio_pin_config_t LED_ORANGE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTB5 (pin F13)  */
    GPIO_PinInit(BOARD_LED_ORANGE_GPIO, BOARD_LED_ORANGE_PIN, &LED_ORANGE_config);

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
                                          /* Pin is configured as PTB4 */
                                          kPORT_MuxAsGpio,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTB4 (pin H9) is configured as PTB4 */
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
                                          /* Pin is configured as PTB5 */
                                          kPORT_MuxAsGpio,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTB5 (pin F13) is configured as PTB5 */
    PORT_SetPinConfig(BOARD_LED_ORANGE_PORT, BOARD_LED_ORANGE_PIN, &LED_ORANGE);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitACCELPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: F6, peripheral: I2C0, signal: SDA, pin_signal: ADC0_SE6a/PTE18/LLWU_P20/SPI0_SOUT/UART2_CTS_b/I2C0_SDA, slew_rate: fast, open_drain: enable, pull_select: down,
    pull_enable: disable}
  - {pin_num: F7, peripheral: I2C0, signal: SCL, pin_signal: ADC0_SE7a/PTE19/SPI0_SIN/UART2_RTS_b/I2C0_SCL/CMP3_OUT, slew_rate: fast, open_drain: enable, pull_select: down,
    pull_enable: disable}
  - {pin_num: L8, peripheral: GPIOE, signal: 'GPIO, 27', pin_signal: PTE27/UART4_RTS_b, direction: INPUT, slew_rate: fast, open_drain: enable, pull_select: up, pull_enable: enable}
  - {pin_num: M7, peripheral: GPIOE, signal: 'GPIO, 28', pin_signal: PTE28, direction: INPUT, slew_rate: fast, open_drain: enable, pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitACCELPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitACCELPins(void)
{
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    gpio_pin_config_t ACCEL_I2C_INT1_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTE27 (pin L8)  */
    GPIO_PinInit(BOARD_ACCEL_I2C_INT1_GPIO, BOARD_ACCEL_I2C_INT1_PIN, &ACCEL_I2C_INT1_config);

    gpio_pin_config_t ACCEL_I2C_INT2_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTE28 (pin M7)  */
    GPIO_PinInit(BOARD_ACCEL_I2C_INT2_GPIO, BOARD_ACCEL_I2C_INT2_PIN, &ACCEL_I2C_INT2_config);

    const port_pin_config_t ACCEL_I2C_SDA = {/* Internal pull-up/down resistor is disabled */
                                             kPORT_PullDisable,
                                             /* Fast slew rate is configured */
                                             kPORT_FastSlewRate,
                                             /* Passive filter is disabled */
                                             kPORT_PassiveFilterDisable,
                                             /* Open drain is enabled */
                                             kPORT_OpenDrainEnable,
                                             /* Low drive strength is configured */
                                             kPORT_LowDriveStrength,
                                             /* Pin is configured as I2C0_SDA */
                                             kPORT_MuxAlt4,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    /* PORTE18 (pin F6) is configured as I2C0_SDA */
    PORT_SetPinConfig(BOARD_ACCEL_I2C_SDA_PORT, BOARD_ACCEL_I2C_SDA_PIN, &ACCEL_I2C_SDA);

    const port_pin_config_t ACCEL_I2C_SCL = {/* Internal pull-up/down resistor is disabled */
                                             kPORT_PullDisable,
                                             /* Fast slew rate is configured */
                                             kPORT_FastSlewRate,
                                             /* Passive filter is disabled */
                                             kPORT_PassiveFilterDisable,
                                             /* Open drain is enabled */
                                             kPORT_OpenDrainEnable,
                                             /* Low drive strength is configured */
                                             kPORT_LowDriveStrength,
                                             /* Pin is configured as I2C0_SCL */
                                             kPORT_MuxAlt4,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    /* PORTE19 (pin F7) is configured as I2C0_SCL */
    PORT_SetPinConfig(BOARD_ACCEL_I2C_SCL_PORT, BOARD_ACCEL_I2C_SCL_PIN, &ACCEL_I2C_SCL);

    const port_pin_config_t ACCEL_I2C_INT1 = {/* Internal pull-up resistor is enabled */
                                              kPORT_PullUp,
                                              /* Fast slew rate is configured */
                                              kPORT_FastSlewRate,
                                              /* Passive filter is disabled */
                                              kPORT_PassiveFilterDisable,
                                              /* Open drain is enabled */
                                              kPORT_OpenDrainEnable,
                                              /* Low drive strength is configured */
                                              kPORT_LowDriveStrength,
                                              /* Pin is configured as PTE27 */
                                              kPORT_MuxAsGpio,
                                              /* Pin Control Register fields [15:0] are not locked */
                                              kPORT_UnlockRegister};
    /* PORTE27 (pin L8) is configured as PTE27 */
    PORT_SetPinConfig(BOARD_ACCEL_I2C_INT1_PORT, BOARD_ACCEL_I2C_INT1_PIN, &ACCEL_I2C_INT1);

    const port_pin_config_t ACCEL_I2C_INT2 = {/* Internal pull-up resistor is enabled */
                                              kPORT_PullUp,
                                              /* Fast slew rate is configured */
                                              kPORT_FastSlewRate,
                                              /* Passive filter is disabled */
                                              kPORT_PassiveFilterDisable,
                                              /* Open drain is enabled */
                                              kPORT_OpenDrainEnable,
                                              /* Low drive strength is configured */
                                              kPORT_LowDriveStrength,
                                              /* Pin is configured as PTE28 */
                                              kPORT_MuxAsGpio,
                                              /* Pin Control Register fields [15:0] are not locked */
                                              kPORT_UnlockRegister};
    /* PORTE28 (pin M7) is configured as PTE28 */
    PORT_SetPinConfig(BOARD_ACCEL_I2C_INT2_PORT, BOARD_ACCEL_I2C_INT2_PIN, &ACCEL_I2C_INT2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitDEBUG_UARTPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: F5, peripheral: UART2, signal: RX, pin_signal: ADC0_SE5a/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3/TPM_CLKIN1, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: H3, peripheral: UART2, signal: TX, pin_signal: ADC0_SE4a/PTE16/SPI0_PCS0/UART2_TX/FTM_CLKIN0/FTM0_FLT3/TPM_CLKIN0, direction: OUTPUT, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
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
                                             /* Pin is configured as UART2_TX */
                                             kPORT_MuxAlt3,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    /* PORTE16 (pin H3) is configured as UART2_TX */
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
                                             /* Pin is configured as UART2_RX */
                                             kPORT_MuxAlt3,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    /* PORTE17 (pin F5) is configured as UART2_RX */
    PORT_SetPinConfig(BOARD_DEBUG_UART_RX_PORT, BOARD_DEBUG_UART_RX_PIN, &DEBUG_UART_RX);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPOTPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: M3, peripheral: ADC1, signal: 'SE, 16', pin_signal: ADC1_SE16/CMP2_IN2/ADC0_SE22}
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
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitTOUCH_PADsPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: D11, peripheral: TSI0, signal: 'CH, 12', pin_signal: TSI0_CH12/PTB19/CAN0_RX/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB/TPM2_CH1, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: D12, peripheral: TSI0, signal: 'CH, 11', pin_signal: TSI0_CH11/PTB18/CAN0_TX/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/SDRAM_A23/FTM2_QD_PHA/TPM2_CH0, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: H11, peripheral: GPIOA, signal: 'GPIO, 29', pin_signal: PTA29/MII0_COL/FB_A24, direction: OUTPUT, slew_rate: slow, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: H12, peripheral: GPIOA, signal: 'GPIO, 28', pin_signal: PTA28/MII0_TXER/FB_A25, direction: OUTPUT, slew_rate: slow, open_drain: disable, pull_select: down,
    pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitTOUCH_PADsPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitTOUCH_PADsPins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    gpio_pin_config_t LED_GREEN_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTA28 (pin H12)  */
    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, &LED_GREEN_config);

    gpio_pin_config_t LED_BLUE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTA29 (pin H11)  */
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
                                         /* Pin is configured as PTA28 */
                                         kPORT_MuxAsGpio,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTA28 (pin H12) is configured as PTA28 */
    PORT_SetPinConfig(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, &LED_GREEN);

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
                                        /* Pin is configured as PTA29 */
                                        kPORT_MuxAsGpio,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTA29 (pin H11) is configured as PTA29 */
    PORT_SetPinConfig(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, &LED_BLUE);

    const port_pin_config_t TSI_ELECTRODE_1 = {/* Internal pull-up/down resistor is disabled */
                                               kPORT_PullDisable,
                                               /* Fast slew rate is configured */
                                               kPORT_FastSlewRate,
                                               /* Passive filter is disabled */
                                               kPORT_PassiveFilterDisable,
                                               /* Open drain is disabled */
                                               kPORT_OpenDrainDisable,
                                               /* Low drive strength is configured */
                                               kPORT_LowDriveStrength,
                                               /* Pin is configured as TSI0_CH11 */
                                               kPORT_PinDisabledOrAnalog,
                                               /* Pin Control Register fields [15:0] are not locked */
                                               kPORT_UnlockRegister};
    /* PORTB18 (pin D12) is configured as TSI0_CH11 */
    PORT_SetPinConfig(BOARD_TSI_ELECTRODE_1_PORT, BOARD_TSI_ELECTRODE_1_PIN, &TSI_ELECTRODE_1);

    const port_pin_config_t TSI_ELECTRODE_2 = {/* Internal pull-up/down resistor is disabled */
                                               kPORT_PullDisable,
                                               /* Fast slew rate is configured */
                                               kPORT_FastSlewRate,
                                               /* Passive filter is disabled */
                                               kPORT_PassiveFilterDisable,
                                               /* Open drain is disabled */
                                               kPORT_OpenDrainDisable,
                                               /* Low drive strength is configured */
                                               kPORT_LowDriveStrength,
                                               /* Pin is configured as TSI0_CH12 */
                                               kPORT_PinDisabledOrAnalog,
                                               /* Pin Control Register fields [15:0] are not locked */
                                               kPORT_UnlockRegister};
    /* PORTB19 (pin D11) is configured as TSI0_CH12 */
    PORT_SetPinConfig(BOARD_TSI_ELECTRODE_2_PORT, BOARD_TSI_ELECTRODE_2_PIN, &TSI_ELECTRODE_2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitSDRAMPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: B5, peripheral: SDRAM, signal: A10, pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FB_AD2/SDRAM_A10/EWM_IN/SPI1_PCS0, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable, digital_filter: disable}
  - {pin_num: B4, peripheral: SDRAM, signal: A11, pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/SDRAM_A11/I2C0_SDA, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable, digital_filter: disable}
  - {pin_num: A4, peripheral: SDRAM, signal: A12, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/SDRAM_A12/I2C0_SCL, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable, digital_filter: disable}
  - {pin_num: A8, peripheral: SDRAM, signal: A13, pin_signal: ADC1_SE6b/PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5/SDRAM_A13, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: C9, peripheral: SDRAM, signal: A14, pin_signal: ADC1_SE5b/CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/SDRAM_A14/FTM2_FLT0, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: C10, peripheral: SDRAM, signal: A15, pin_signal: ADC1_SE4b/CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7/SDRAM_A15, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: A10, peripheral: SDRAM, signal: A17, pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/SDRAM_A17/I2S0_MCLK, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: A11, peripheral: SDRAM, signal: A18, pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/FB_AD10/SDRAM_A18/CMP0_OUT/FTM0_CH2, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: B11, peripheral: SDRAM, signal: A19, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: A13, peripheral: SDRAM, signal: A20, pin_signal: ADC0_SE4b/CMP1_IN0/TSI0_CH15/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FB_AD12/SDRAM_A20/I2S0_TX_FS, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: B12, peripheral: SDRAM, signal: A21, pin_signal: ADC0_SE15/TSI0_CH14/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FB_AD13/SDRAM_A21/I2S0_TXD0, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: B13, peripheral: SDRAM, signal: A22, pin_signal: ADC0_SE14/TSI0_CH13/PTC0/SPI0_PCS4/PDB0_EXTRG/USB0_SOF_OUT/FB_AD14/SDRAM_A22/I2S0_TXD1, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: C4, peripheral: SDRAM, signal: A9, pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FB_AD1/SDRAM_A9/EWM_OUT_b/SPI1_SCK, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, digital_filter: disable}
  - {pin_num: G13, peripheral: SDRAM, signal: CAS, pin_signal: ADC0_SE8/ADC1_SE8/TSI0_CH0/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/RMII0_MDIO/MII0_MDIO/SDRAM_CAS_b/FTM1_QD_PHA/TPM1_CH0,
    slew_rate: fast, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: E5, peripheral: SDRAM, signal: CKE, pin_signal: PTD7/CMT_IRO/UART0_TX/FTM0_CH7/SDRAM_CKE/FTM0_FLT1/SPI1_SIN, slew_rate: fast, open_drain: disable, drive_strength: low,
    pull_select: down, pull_enable: disable, digital_filter: disable}
  - {pin_num: M10, peripheral: SDRAM, signal: CLKOUT, pin_signal: PTA6/FTM0_CH3/CLKOUT/TRACE_CLKOUT, slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: G10, peripheral: SDRAM, signal: CS0, pin_signal: ADC0_SE13/TSI0_CH8/PTB3/I2C0_SDA/UART0_CTS_b/UART0_COL_b/ENET0_1588_TMR1/SDRAM_CS0_b/FTM0_FLT0, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: B6, peripheral: SDRAM, signal: D0, pin_signal: PTC29/I2C3_SCL/ENET0_1588_TMR3/FB_A0/SDRAM_D0, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: C6, peripheral: SDRAM, signal: D1, pin_signal: PTC28/I2C3_SDA/ENET0_1588_TMR2/FB_A1/SDRAM_D1, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: J9, peripheral: SDRAM, signal: D10, pin_signal: PTA31/CAN0_RX/FB_A10/SDRAM_D10, slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: H10, peripheral: SDRAM, signal: D11, pin_signal: PTA30/CAN0_TX/FB_A11/SDRAM_D11, slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: H13, peripheral: SDRAM, signal: D12, pin_signal: PTA27/MII0_CRS/FB_A12/SDRAM_D12/FB_A26, slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: J10, peripheral: SDRAM, signal: D13, pin_signal: PTA26/MII0_TXD3/FB_A13/SDRAM_D13/FB_A27, slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: J11, peripheral: SDRAM, signal: D14, pin_signal: CMP3_IN5/PTA25/MII0_TXCLK/FB_A14/SDRAM_D14/FB_A28, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: K11, peripheral: SDRAM, signal: D15, pin_signal: CMP3_IN4/PTA24/MII0_TXD2/FB_A15/SDRAM_D15/FB_A29, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: D13, peripheral: SDRAM, signal: D16, pin_signal: TSI0_CH10/PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/SDRAM_D16/EWM_OUT_b/TPM_CLKIN1, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: F8, peripheral: SDRAM, signal: D17, pin_signal: TSI0_CH9/PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/SDRAM_D17/EWM_IN/TPM_CLKIN0, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: E13, peripheral: SDRAM, signal: D18, pin_signal: ADC1_SE15/PTB11/SPI1_SCK/UART3_TX/FB_AD18/SDRAM_D18/FTM0_FLT2, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: G9, peripheral: SDRAM, signal: D19, pin_signal: ADC1_SE14/PTB10/SPI1_PCS0/UART3_RX/FB_AD19/SDRAM_D19/FTM0_FLT1, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: D6, peripheral: SDRAM, signal: D2, pin_signal: PTC27/LPUART0_RTS_b/ENET0_1588_TMR1/FB_A2/SDRAM_D2, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: F9, peripheral: SDRAM, signal: D20, pin_signal: PTB9/SPI1_PCS1/UART3_CTS_b/FB_AD20/SDRAM_D20, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: F10, peripheral: SDRAM, signal: D21, pin_signal: PTB8/UART3_RTS_b/FB_AD21/SDRAM_D21, slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: F11, peripheral: SDRAM, signal: D22, pin_signal: ADC1_SE13/PTB7/FB_AD22/SDRAM_D22, slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: F12, peripheral: SDRAM, signal: D23, pin_signal: ADC1_SE12/PTB6/FB_AD23/SDRAM_D23, slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: D8, peripheral: SDRAM, signal: D24, pin_signal: PTC15/UART4_TX/FB_AD24/SDRAM_D24, slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: C8, peripheral: SDRAM, signal: D25, pin_signal: PTC14/UART4_RX/FB_AD25/SDRAM_D25, slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: B8, peripheral: SDRAM, signal: D26, pin_signal: PTC13/UART4_CTS_b/FTM_CLKIN1/FB_AD26/SDRAM_D26/TPM_CLKIN1, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: B9, peripheral: SDRAM, signal: D27, pin_signal: PTC12/UART4_RTS_b/FTM_CLKIN0/FB_AD27/SDRAM_D27/FTM3_FLT0/TPM_CLKIN0, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: C12, peripheral: SDRAM, signal: D28, pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28/SDRAM_D28/CMP3_OUT, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: C13, peripheral: SDRAM, signal: D29, pin_signal: PTB22/SPI2_SOUT/FB_AD29/SDRAM_D29/CMP2_OUT, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: E6, peripheral: SDRAM, signal: D3, pin_signal: PTC26/LPUART0_CTS_b/ENET0_1588_TMR0/FB_A3/SDRAM_D3, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: D9, peripheral: SDRAM, signal: D30, pin_signal: PTB21/SPI2_SCK/FB_AD30/SDRAM_D30/CMP1_OUT, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: D10, peripheral: SDRAM, signal: D31, pin_signal: PTB20/SPI2_PCS0/FB_AD31/SDRAM_D31/CMP0_OUT, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: A7, peripheral: SDRAM, signal: D4, pin_signal: PTC25/LPUART0_RX/FB_A4/SDRAM_D4, slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: B7, peripheral: SDRAM, signal: D5, pin_signal: PTC24/LPUART0_TX/FB_A5/SDRAM_D5, slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: E9, peripheral: SDRAM, signal: D6, pin_signal: PTB15/CAN1_RX/FB_A6/SDRAM_D6, slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: E10, peripheral: SDRAM, signal: D7, pin_signal: PTB14/CAN1_TX/FB_A7/SDRAM_D7, slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: E11, peripheral: SDRAM, signal: D8, pin_signal: PTB13/UART3_CTS_b/FTM1_CH1/FTM0_CH5/FB_A8/SDRAM_D8/FTM1_QD_PHB/TPM1_CH1, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: E12, peripheral: SDRAM, signal: D9, pin_signal: PTB12/UART3_RTS_b/FTM1_CH0/FTM0_CH4/FB_A9/SDRAM_D9/FTM1_QD_PHA/TPM1_CH0, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: C7, peripheral: SDRAM, signal: DQM0, pin_signal: PTC19/UART3_CTS_b/ENET0_1588_TMR3/FB_CS3_b/FB_BE7_0_BLS31_24_b/SDRAM_DQM0/FB_TA_b, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: D7, peripheral: SDRAM, signal: DQM1, pin_signal: PTC18/UART3_RTS_b/ENET0_1588_TMR2/FB_TBST_b/FB_CS2_b/FB_BE15_8_BLS23_16_b/SDRAM_DQM1, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: E8, peripheral: SDRAM, signal: DQM2, pin_signal: PTC16/CAN1_RX/UART3_RX/ENET0_1588_TMR0/FB_CS5_b/FB_TSIZ1/FB_BE23_16_BLS15_8_b/SDRAM_DQM2, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: E7, peripheral: SDRAM, signal: DQM3, pin_signal: PTC17/CAN1_TX/UART3_TX/ENET0_1588_TMR1/FB_CS4_b/FB_TSIZ0/FB_BE31_24_BLS7_0_b/SDRAM_DQM3, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: G12, peripheral: SDRAM, signal: RAS, pin_signal: ADC0_SE9/ADC1_SE9/TSI0_CH6/PTB1/I2C0_SDA/FTM1_CH1/RMII0_MDC/MII0_MDC/SDRAM_RAS_b/FTM1_QD_PHB/TPM1_CH1,
    slew_rate: fast, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: G11, peripheral: SDRAM, signal: WE, pin_signal: ADC0_SE12/TSI0_CH7/PTB2/I2C0_SCL/UART0_RTS_b/ENET0_1588_TMR0/SDRAM_WE/FTM0_FLT3, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitSDRAMPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitSDRAMPins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    const port_pin_config_t SDRAM_D15 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D15 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTA24 (pin K11) is configured as SDRAM_D15 */
    PORT_SetPinConfig(BOARD_SDRAM_D15_PORT, BOARD_SDRAM_D15_PIN, &SDRAM_D15);

    const port_pin_config_t SDRAM_D14 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D14 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTA25 (pin J11) is configured as SDRAM_D14 */
    PORT_SetPinConfig(BOARD_SDRAM_D14_PORT, BOARD_SDRAM_D14_PIN, &SDRAM_D14);

    const port_pin_config_t SDRAM_D13 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D13 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTA26 (pin J10) is configured as SDRAM_D13 */
    PORT_SetPinConfig(BOARD_SDRAM_D13_PORT, BOARD_SDRAM_D13_PIN, &SDRAM_D13);

    const port_pin_config_t SDRAM_D12 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D12 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTA27 (pin H13) is configured as SDRAM_D12 */
    PORT_SetPinConfig(BOARD_SDRAM_D12_PORT, BOARD_SDRAM_D12_PIN, &SDRAM_D12);

    const port_pin_config_t SDRAM_D11 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D11 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTA30 (pin H10) is configured as SDRAM_D11 */
    PORT_SetPinConfig(BOARD_SDRAM_D11_PORT, BOARD_SDRAM_D11_PIN, &SDRAM_D11);

    const port_pin_config_t SDRAM_D10 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D10 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTA31 (pin J9) is configured as SDRAM_D10 */
    PORT_SetPinConfig(BOARD_SDRAM_D10_PORT, BOARD_SDRAM_D10_PIN, &SDRAM_D10);

    const port_pin_config_t SDRAM_CLKOUT = {/* Internal pull-up/down resistor is disabled */
                                            kPORT_PullDisable,
                                            /* Fast slew rate is configured */
                                            kPORT_FastSlewRate,
                                            /* Passive filter is disabled */
                                            kPORT_PassiveFilterDisable,
                                            /* Open drain is disabled */
                                            kPORT_OpenDrainDisable,
                                            /* Low drive strength is configured */
                                            kPORT_LowDriveStrength,
                                            /* Pin is configured as CLKOUT */
                                            kPORT_MuxAlt5,
                                            /* Pin Control Register fields [15:0] are not locked */
                                            kPORT_UnlockRegister};
    /* PORTA6 (pin M10) is configured as CLKOUT */
    PORT_SetPinConfig(BOARD_SDRAM_CLKOUT_PORT, BOARD_SDRAM_CLKOUT_PIN, &SDRAM_CLKOUT);

    const port_pin_config_t SDRAM_CAS = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_CAS_b */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTB0 (pin G13) is configured as SDRAM_CAS_b */
    PORT_SetPinConfig(BOARD_SDRAM_CAS_PORT, BOARD_SDRAM_CAS_PIN, &SDRAM_CAS);

    const port_pin_config_t SDRAM_RAS = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_RAS_b */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTB1 (pin G12) is configured as SDRAM_RAS_b */
    PORT_SetPinConfig(BOARD_SDRAM_RAS_PORT, BOARD_SDRAM_RAS_PIN, &SDRAM_RAS);

    const port_pin_config_t SDRAM_D19 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D19 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTB10 (pin G9) is configured as SDRAM_D19 */
    PORT_SetPinConfig(BOARD_SDRAM_D19_PORT, BOARD_SDRAM_D19_PIN, &SDRAM_D19);

    const port_pin_config_t SDRAM_D18 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D18 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTB11 (pin E13) is configured as SDRAM_D18 */
    PORT_SetPinConfig(BOARD_SDRAM_D18_PORT, BOARD_SDRAM_D18_PIN, &SDRAM_D18);

    const port_pin_config_t SDRAM_D9 = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as SDRAM_D9 */
                                        kPORT_MuxAlt5,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTB12 (pin E12) is configured as SDRAM_D9 */
    PORT_SetPinConfig(BOARD_SDRAM_D9_PORT, BOARD_SDRAM_D9_PIN, &SDRAM_D9);

    const port_pin_config_t SDRAM_D8 = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as SDRAM_D8 */
                                        kPORT_MuxAlt5,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTB13 (pin E11) is configured as SDRAM_D8 */
    PORT_SetPinConfig(BOARD_SDRAM_D8_PORT, BOARD_SDRAM_D8_PIN, &SDRAM_D8);

    const port_pin_config_t SDRAM_D7 = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as SDRAM_D7 */
                                        kPORT_MuxAlt5,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTB14 (pin E10) is configured as SDRAM_D7 */
    PORT_SetPinConfig(BOARD_SDRAM_D7_PORT, BOARD_SDRAM_D7_PIN, &SDRAM_D7);

    const port_pin_config_t SDRAM_D6 = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as SDRAM_D6 */
                                        kPORT_MuxAlt5,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTB15 (pin E9) is configured as SDRAM_D6 */
    PORT_SetPinConfig(BOARD_SDRAM_D6_PORT, BOARD_SDRAM_D6_PIN, &SDRAM_D6);

    const port_pin_config_t SDRAM_D17 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D17 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTB16 (pin F8) is configured as SDRAM_D17 */
    PORT_SetPinConfig(BOARD_SDRAM_D17_PORT, BOARD_SDRAM_D17_PIN, &SDRAM_D17);

    const port_pin_config_t SDRAM_D16 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D16 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTB17 (pin D13) is configured as SDRAM_D16 */
    PORT_SetPinConfig(BOARD_SDRAM_D16_PORT, BOARD_SDRAM_D16_PIN, &SDRAM_D16);

    const port_pin_config_t SDRAM_WE = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as SDRAM_WE */
                                        kPORT_MuxAlt5,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTB2 (pin G11) is configured as SDRAM_WE */
    PORT_SetPinConfig(BOARD_SDRAM_WE_PORT, BOARD_SDRAM_WE_PIN, &SDRAM_WE);

    const port_pin_config_t SDRAM_D31 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D31 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTB20 (pin D10) is configured as SDRAM_D31 */
    PORT_SetPinConfig(BOARD_SDRAM_D31_PORT, BOARD_SDRAM_D31_PIN, &SDRAM_D31);

    const port_pin_config_t SDRAM_D30 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D30 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTB21 (pin D9) is configured as SDRAM_D30 */
    PORT_SetPinConfig(BOARD_SDRAM_D30_PORT, BOARD_SDRAM_D30_PIN, &SDRAM_D30);

    const port_pin_config_t SDRAM_D29 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D29 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTB22 (pin C13) is configured as SDRAM_D29 */
    PORT_SetPinConfig(BOARD_SDRAM_D29_PORT, BOARD_SDRAM_D29_PIN, &SDRAM_D29);

    const port_pin_config_t SDRAM_D28 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D28 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTB23 (pin C12) is configured as SDRAM_D28 */
    PORT_SetPinConfig(BOARD_SDRAM_D28_PORT, BOARD_SDRAM_D28_PIN, &SDRAM_D28);

    const port_pin_config_t SDRAM_CS0 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_CS0_b */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTB3 (pin G10) is configured as SDRAM_CS0_b */
    PORT_SetPinConfig(BOARD_SDRAM_CS0_PORT, BOARD_SDRAM_CS0_PIN, &SDRAM_CS0);

    const port_pin_config_t SDRAM_D23 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D23 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTB6 (pin F12) is configured as SDRAM_D23 */
    PORT_SetPinConfig(BOARD_SDRAM_D23_PORT, BOARD_SDRAM_D23_PIN, &SDRAM_D23);

    const port_pin_config_t SDRAM_D22 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D22 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTB7 (pin F11) is configured as SDRAM_D22 */
    PORT_SetPinConfig(BOARD_SDRAM_D22_PORT, BOARD_SDRAM_D22_PIN, &SDRAM_D22);

    const port_pin_config_t SDRAM_D21 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D21 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTB8 (pin F10) is configured as SDRAM_D21 */
    PORT_SetPinConfig(BOARD_SDRAM_D21_PORT, BOARD_SDRAM_D21_PIN, &SDRAM_D21);

    const port_pin_config_t SDRAM_D20 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D20 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTB9 (pin F9) is configured as SDRAM_D20 */
    PORT_SetPinConfig(BOARD_SDRAM_D20_PORT, BOARD_SDRAM_D20_PIN, &SDRAM_D20);

    const port_pin_config_t SDRAM_A22 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_A22 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTC0 (pin B13) is configured as SDRAM_A22 */
    PORT_SetPinConfig(BOARD_SDRAM_A22_PORT, BOARD_SDRAM_A22_PIN, &SDRAM_A22);

    const port_pin_config_t SDRAM_A21 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_A21 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTC1 (pin B12) is configured as SDRAM_A21 */
    PORT_SetPinConfig(BOARD_SDRAM_A21_PORT, BOARD_SDRAM_A21_PIN, &SDRAM_A21);

    const port_pin_config_t SDRAM_A13 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_A13 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTC10 (pin A8) is configured as SDRAM_A13 */
    PORT_SetPinConfig(BOARD_SDRAM_A13_PORT, BOARD_SDRAM_A13_PIN, &SDRAM_A13);

    const port_pin_config_t SDRAM_D27 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D27 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTC12 (pin B9) is configured as SDRAM_D27 */
    PORT_SetPinConfig(BOARD_SDRAM_D27_PORT, BOARD_SDRAM_D27_PIN, &SDRAM_D27);

    const port_pin_config_t SDRAM_D26 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D26 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTC13 (pin B8) is configured as SDRAM_D26 */
    PORT_SetPinConfig(BOARD_SDRAM_D26_PORT, BOARD_SDRAM_D26_PIN, &SDRAM_D26);

    const port_pin_config_t SDRAM_D25 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D25 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTC14 (pin C8) is configured as SDRAM_D25 */
    PORT_SetPinConfig(BOARD_SDRAM_D25_PORT, BOARD_SDRAM_D25_PIN, &SDRAM_D25);

    const port_pin_config_t SDRAM_D24 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_D24 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTC15 (pin D8) is configured as SDRAM_D24 */
    PORT_SetPinConfig(BOARD_SDRAM_D24_PORT, BOARD_SDRAM_D24_PIN, &SDRAM_D24);

    const port_pin_config_t SDRAM_DQM2 = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as SDRAM_DQM2 */
                                          kPORT_MuxAlt5,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTC16 (pin E8) is configured as SDRAM_DQM2 */
    PORT_SetPinConfig(BOARD_SDRAM_DQM2_PORT, BOARD_SDRAM_DQM2_PIN, &SDRAM_DQM2);

    const port_pin_config_t SDRAM_DQM3 = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as SDRAM_DQM3 */
                                          kPORT_MuxAlt5,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTC17 (pin E7) is configured as SDRAM_DQM3 */
    PORT_SetPinConfig(BOARD_SDRAM_DQM3_PORT, BOARD_SDRAM_DQM3_PIN, &SDRAM_DQM3);

    const port_pin_config_t SDRAM_DQM1 = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as SDRAM_DQM1 */
                                          kPORT_MuxAlt5,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTC18 (pin D7) is configured as SDRAM_DQM1 */
    PORT_SetPinConfig(BOARD_SDRAM_DQM1_PORT, BOARD_SDRAM_DQM1_PIN, &SDRAM_DQM1);

    const port_pin_config_t SDRAM_DQM0 = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as SDRAM_DQM0 */
                                          kPORT_MuxAlt5,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTC19 (pin C7) is configured as SDRAM_DQM0 */
    PORT_SetPinConfig(BOARD_SDRAM_DQM0_PORT, BOARD_SDRAM_DQM0_PIN, &SDRAM_DQM0);

    const port_pin_config_t SDRAM_A20 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_A20 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTC2 (pin A13) is configured as SDRAM_A20 */
    PORT_SetPinConfig(BOARD_SDRAM_A20_PORT, BOARD_SDRAM_A20_PIN, &SDRAM_A20);

    const port_pin_config_t SDRAM_D5 = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as SDRAM_D5 */
                                        kPORT_MuxAlt5,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTC24 (pin B7) is configured as SDRAM_D5 */
    PORT_SetPinConfig(BOARD_SDRAM_D5_PORT, BOARD_SDRAM_D5_PIN, &SDRAM_D5);

    const port_pin_config_t SDRAM_D4 = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as SDRAM_D4 */
                                        kPORT_MuxAlt5,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTC25 (pin A7) is configured as SDRAM_D4 */
    PORT_SetPinConfig(BOARD_SDRAM_D4_PORT, BOARD_SDRAM_D4_PIN, &SDRAM_D4);

    const port_pin_config_t SDRAM_D3 = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as SDRAM_D3 */
                                        kPORT_MuxAlt5,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTC26 (pin E6) is configured as SDRAM_D3 */
    PORT_SetPinConfig(BOARD_SDRAM_D3_PORT, BOARD_SDRAM_D3_PIN, &SDRAM_D3);

    const port_pin_config_t SDRAM_D2 = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as SDRAM_D2 */
                                        kPORT_MuxAlt5,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTC27 (pin D6) is configured as SDRAM_D2 */
    PORT_SetPinConfig(BOARD_SDRAM_D2_PORT, BOARD_SDRAM_D2_PIN, &SDRAM_D2);

    const port_pin_config_t SDRAM_D1 = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as SDRAM_D1 */
                                        kPORT_MuxAlt5,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTC28 (pin C6) is configured as SDRAM_D1 */
    PORT_SetPinConfig(BOARD_SDRAM_D1_PORT, BOARD_SDRAM_D1_PIN, &SDRAM_D1);

    const port_pin_config_t SDRAM_D0 = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as SDRAM_D0 */
                                        kPORT_MuxAlt5,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTC29 (pin B6) is configured as SDRAM_D0 */
    PORT_SetPinConfig(BOARD_SDRAM_D0_PORT, BOARD_SDRAM_D0_PIN, &SDRAM_D0);

    const port_pin_config_t SDRAM_A19 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_A19 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTC4 (pin B11) is configured as SDRAM_A19 */
    PORT_SetPinConfig(BOARD_SDRAM_A19_PORT, BOARD_SDRAM_A19_PIN, &SDRAM_A19);

    const port_pin_config_t SDRAM_A18 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_A18 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTC5 (pin A11) is configured as SDRAM_A18 */
    PORT_SetPinConfig(BOARD_SDRAM_A18_PORT, BOARD_SDRAM_A18_PIN, &SDRAM_A18);

    const port_pin_config_t SDRAM_A17 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_A17 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTC6 (pin A10) is configured as SDRAM_A17 */
    PORT_SetPinConfig(BOARD_SDRAM_A17_PORT, BOARD_SDRAM_A17_PIN, &SDRAM_A17);

    const port_pin_config_t SDRAM_A15 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_A15 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTC8 (pin C10) is configured as SDRAM_A15 */
    PORT_SetPinConfig(BOARD_SDRAM_A15_PORT, BOARD_SDRAM_A15_PIN, &SDRAM_A15);

    const port_pin_config_t SDRAM_A14 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_A14 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTC9 (pin C9) is configured as SDRAM_A14 */
    PORT_SetPinConfig(BOARD_SDRAM_A14_PORT, BOARD_SDRAM_A14_PIN, &SDRAM_A14);
    /* Configure digital filter */
    PORT_EnablePinsDigitalFilter(
        /* Digital filter is configured on port D */
        PORTD,
        /* Digital filter is configured for PORTD0 */
          PORT_DFER_DFE_2_MASK
            /* Digital filter is configured for PORTD1 */
            | PORT_DFER_DFE_3_MASK
            /* Digital filter is configured for PORTD2 */
            | PORT_DFER_DFE_4_MASK
            /* Digital filter is configured for PORTD3 */
            | PORT_DFER_DFE_5_MASK
            /* Digital filter is configured for PORTD4 */
            | PORT_DFER_DFE_7_MASK,
        /* Disable digital filter */
        false);

    const port_pin_config_t SDRAM_A12 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_A12 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTD2 (pin A4) is configured as SDRAM_A12 */
    PORT_SetPinConfig(BOARD_SDRAM_A12_PORT, BOARD_SDRAM_A12_PIN, &SDRAM_A12);

    const port_pin_config_t SDRAM_A11 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_A11 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTD3 (pin B4) is configured as SDRAM_A11 */
    PORT_SetPinConfig(BOARD_SDRAM_A11_PORT, BOARD_SDRAM_A11_PIN, &SDRAM_A11);

    const port_pin_config_t SDRAM_A10 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_A10 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTD4 (pin B5) is configured as SDRAM_A10 */
    PORT_SetPinConfig(BOARD_SDRAM_A10_PORT, BOARD_SDRAM_A10_PIN, &SDRAM_A10);

    const port_pin_config_t SDRAM_A9 = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as SDRAM_A9 */
                                        kPORT_MuxAlt5,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTD5 (pin C4) is configured as SDRAM_A9 */
    PORT_SetPinConfig(BOARD_SDRAM_A9_PORT, BOARD_SDRAM_A9_PIN, &SDRAM_A9);

    const port_pin_config_t SDRAM_CKE = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_CKE */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTD7 (pin E5) is configured as SDRAM_CKE */
    PORT_SetPinConfig(BOARD_SDRAM_CKE_PORT, BOARD_SDRAM_CKE_PIN, &SDRAM_CKE);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitUSBPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: D5, peripheral: GPIOD, signal: 'GPIO, 8', pin_signal: PTD8/LLWU_P24/I2C0_SCL/LPUART0_RX/FB_A16, direction: OUTPUT, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable, digital_filter: disable}
  - {pin_num: A3, peripheral: GPIOD, signal: 'GPIO, 15', pin_signal: PTD15/SPI2_PCS1/SDHC0_D7/FB_A23, direction: INPUT, slew_rate: fast, open_drain: disable, pull_select: up,
    pull_enable: enable, digital_filter: disable}
  - {pin_num: J1, peripheral: USBPHY, signal: DP, pin_signal: USB1_DP/LLWU_P30, identifier: MCU_USB1_HS_DP_PHY}
  - {pin_num: H1, peripheral: USBPHY, signal: DM, pin_signal: USB1_DM/LLWU_P31, identifier: MCU_USB1_HS_DN_PHY}
  - {pin_num: J2, peripheral: USBPHY, signal: VBUS, pin_signal: USB1_VBUS/LLWU_P29}
  - {pin_num: F2, peripheral: USBHS, signal: VREGIN, pin_signal: VREG_IN0}
  - {pin_num: G2, peripheral: USBHS, signal: VREGOUT, pin_signal: VREG_OUT}
  - {pin_num: D4, peripheral: GPIOD, signal: 'GPIO, 9', pin_signal: PTD9/I2C0_SDA/LPUART0_TX/FB_A17, direction: INPUT, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable, digital_filter: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitUSBPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitUSBPins(void)
{
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    gpio_pin_config_t microUSB_PWR_EN_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTD8 (pin D5)  */
    GPIO_PinInit(BOARD_microUSB_PWR_EN_GPIO, BOARD_microUSB_PWR_EN_PIN, &microUSB_PWR_EN_config);

    gpio_pin_config_t K65_USB_FLGA_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTD9 (pin D4)  */
    GPIO_PinInit(BOARD_K65_USB_FLGA_GPIO, BOARD_K65_USB_FLGA_PIN, &K65_USB_FLGA_config);

    gpio_pin_config_t microUSB_ID_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTD15 (pin A3)  */
    GPIO_PinInit(BOARD_microUSB_ID_GPIO, BOARD_microUSB_ID_PIN, &microUSB_ID_config);
    /* Configure digital filter */
    PORT_EnablePinsDigitalFilter(
        /* Digital filter is configured on port D */
        PORTD,
        /* Digital filter is configured for PORTD0 */
          PORT_DFER_DFE_8_MASK
            /* Digital filter is configured for PORTD1 */
            | PORT_DFER_DFE_9_MASK
            /* Digital filter is configured for PORTD2 */
            | PORT_DFER_DFE_15_MASK,
        /* Disable digital filter */
        false);

    const port_pin_config_t microUSB_ID = {/* Internal pull-up resistor is enabled */
                                           kPORT_PullUp,
                                           /* Fast slew rate is configured */
                                           kPORT_FastSlewRate,
                                           /* Passive filter is disabled */
                                           kPORT_PassiveFilterDisable,
                                           /* Open drain is disabled */
                                           kPORT_OpenDrainDisable,
                                           /* Low drive strength is configured */
                                           kPORT_LowDriveStrength,
                                           /* Pin is configured as PTD15 */
                                           kPORT_MuxAsGpio,
                                           /* Pin Control Register fields [15:0] are not locked */
                                           kPORT_UnlockRegister};
    /* PORTD15 (pin A3) is configured as PTD15 */
    PORT_SetPinConfig(BOARD_microUSB_ID_PORT, BOARD_microUSB_ID_PIN, &microUSB_ID);

    const port_pin_config_t microUSB_PWR_EN = {/* Internal pull-up/down resistor is disabled */
                                               kPORT_PullDisable,
                                               /* Fast slew rate is configured */
                                               kPORT_FastSlewRate,
                                               /* Passive filter is disabled */
                                               kPORT_PassiveFilterDisable,
                                               /* Open drain is disabled */
                                               kPORT_OpenDrainDisable,
                                               /* Low drive strength is configured */
                                               kPORT_LowDriveStrength,
                                               /* Pin is configured as PTD8 */
                                               kPORT_MuxAsGpio,
                                               /* Pin Control Register fields [15:0] are not locked */
                                               kPORT_UnlockRegister};
    /* PORTD8 (pin D5) is configured as PTD8 */
    PORT_SetPinConfig(BOARD_microUSB_PWR_EN_PORT, BOARD_microUSB_PWR_EN_PIN, &microUSB_PWR_EN);

    const port_pin_config_t K65_USB_FLGA = {/* Internal pull-up/down resistor is disabled */
                                            kPORT_PullDisable,
                                            /* Fast slew rate is configured */
                                            kPORT_FastSlewRate,
                                            /* Passive filter is disabled */
                                            kPORT_PassiveFilterDisable,
                                            /* Open drain is disabled */
                                            kPORT_OpenDrainDisable,
                                            /* Low drive strength is configured */
                                            kPORT_LowDriveStrength,
                                            /* Pin is configured as PTD9 */
                                            kPORT_MuxAsGpio,
                                            /* Pin Control Register fields [15:0] are not locked */
                                            kPORT_UnlockRegister};
    /* PORTD9 (pin D4) is configured as PTD9 */
    PORT_SetPinConfig(BOARD_K65_USB_FLGA_PORT, BOARD_K65_USB_FLGA_PIN, &K65_USB_FLGA);

    SIM->USBPHYCTL = ((SIM->USBPHYCTL &
                       /* Mask bits to zero which are setting */
                       (~(SIM_USBPHYCTL_USBVREGSEL_MASK)))

                      /* Selects the default input voltage source to the USB Regulator in case both VREG_IN0 and
                       * VREG_IN1 are powered.
                       * If only one of the regulator inputs is powered, it will automatically be selected by the
                       * regulator's power mux circuitry.
                       * : VREG_IN0 will be selected if both regulator inputs are powered. */
                      | SIM_USBPHYCTL_USBVREGSEL(USBPHYCTL_USBVREGSEL_VREG_IN0));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitSDHCPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: D1, peripheral: SDHC, signal: CMD, pin_signal: ADC1_SE7a/PTE3/SPI1_SIN/UART1_RTS_b/SDHC0_CMD/TRACE_D1/SPI1_SOUT, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: B1, peripheral: SDHC, signal: 'DATA, 0', pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/SDHC0_D0/TRACE_D3/I2C1_SCL/SPI1_SIN, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: A1, peripheral: SDHC, signal: 'DATA, 1', pin_signal: ADC1_SE4a/PTE0/SPI1_PCS1/UART1_TX/SDHC0_D1/TRACE_CLKOUT/I2C1_SDA/RTC_CLKOUT, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: D2, peripheral: SDHC, signal: 'DATA, 2', pin_signal: PTE5/SPI1_PCS2/UART3_RX/SDHC0_D2/FTM3_CH0, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: E1, peripheral: SDHC, signal: 'DATA, 3', pin_signal: PTE4/LLWU_P2/SPI1_PCS0/UART3_TX/SDHC0_D3/TRACE_D0, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: C1, peripheral: SDHC, signal: DCLK, pin_signal: ADC1_SE6a/PTE2/LLWU_P1/SPI1_SCK/UART1_CTS_b/SDHC0_DCLK/TRACE_D2, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: K10, peripheral: GPIOA, signal: 'GPIO, 9', pin_signal: PTA9/FTM1_CH1/MII0_RXD3/FTM1_QD_PHB/TPM1_CH1/TRACE_D1, direction: INPUT, slew_rate: fast, open_drain: disable,
    pull_select: up, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitSDHCPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitSDHCPins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    gpio_pin_config_t SDHC_CD_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTA9 (pin K10)  */
    GPIO_PinInit(BOARD_SDHC_CD_GPIO, BOARD_SDHC_CD_PIN, &SDHC_CD_config);

    const port_pin_config_t SDHC_CD = {/* Internal pull-up resistor is enabled */
                                       kPORT_PullUp,
                                       /* Fast slew rate is configured */
                                       kPORT_FastSlewRate,
                                       /* Passive filter is disabled */
                                       kPORT_PassiveFilterDisable,
                                       /* Open drain is disabled */
                                       kPORT_OpenDrainDisable,
                                       /* Low drive strength is configured */
                                       kPORT_LowDriveStrength,
                                       /* Pin is configured as PTA9 */
                                       kPORT_MuxAsGpio,
                                       /* Pin Control Register fields [15:0] are not locked */
                                       kPORT_UnlockRegister};
    /* PORTA9 (pin K10) is configured as PTA9 */
    PORT_SetPinConfig(BOARD_SDHC_CD_PORT, BOARD_SDHC_CD_PIN, &SDHC_CD);

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
    /* PORTE0 (pin A1) is configured as SDHC0_D1 */
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
    /* PORTE1 (pin B1) is configured as SDHC0_D0 */
    PORT_SetPinConfig(BOARD_SDHC_D0_PORT, BOARD_SDHC_D0_PIN, &SDHC_D0);

    const port_pin_config_t SDHC_DCLK = {/* Internal pull-up/down resistor is disabled */
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
    /* PORTE2 (pin C1) is configured as SDHC0_DCLK */
    PORT_SetPinConfig(BOARD_SDHC_DCLK_PORT, BOARD_SDHC_DCLK_PIN, &SDHC_DCLK);

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
    /* PORTE3 (pin D1) is configured as SDHC0_CMD */
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
    /* PORTE4 (pin E1) is configured as SDHC0_D3 */
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
    /* PORTE5 (pin D2) is configured as SDHC0_D2 */
    PORT_SetPinConfig(BOARD_SDHC_D2_PORT, BOARD_SDHC_D2_PIN, &SDHC_D2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitENETPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: K8, peripheral: ENET, signal: CLKIN_1588, pin_signal: PTE26/ENET_1588_CLKIN/UART4_CTS_b/RTC_CLKOUT/USB0_CLKIN, identifier: ENET_1588_CLKIN}
  - {pin_num: K8, peripheral: ENET, signal: RMII_CLKIN, pin_signal: PTE26/ENET_1588_CLKIN/UART4_CTS_b/RTC_CLKOUT/USB0_CLKIN, identifier: RMII_CLKIN}
  - {pin_num: K9, peripheral: ENET, signal: RMII_MDC, pin_signal: ADC0_SE11/PTA8/FTM1_CH0/RMII0_MDC/MII0_MDC/FTM1_QD_PHA/TPM1_CH0/TRACE_D2}
  - {pin_num: L10, peripheral: ENET, signal: RMII_MDIO, pin_signal: ADC0_SE10/PTA7/FTM0_CH4/RMII0_MDIO/MII0_MDIO/TRACE_D3}
  - {pin_num: N10, peripheral: ENET, signal: RMII_RXER, pin_signal: PTA5/USB0_CLKIN/FTM0_CH2/RMII0_RXER/MII0_RXER/CMP2_OUT/I2S0_TX_BCLK/JTAG_TRST_b}
  - {pin_num: L11, peripheral: ENET, signal: RMII_RXD0, pin_signal: CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/RMII0_RXD0/MII0_RXD0/I2C2_SDA/I2S0_TX_FS/FTM1_QD_PHB/TPM1_CH1}
  - {pin_num: J12, peripheral: ENET, signal: RMII_TXD1, pin_signal: ADC1_SE17/PTA17/SPI0_SIN/UART0_RTS_b/RMII0_TXD1/MII0_TXD1/I2S0_MCLK}
  - {pin_num: K12, peripheral: ENET, signal: RMII_TXEN, pin_signal: CMP3_IN1/PTA15/SPI0_SCK/UART0_RX/RMII0_TXEN/MII0_TXEN/I2S0_RXD0}
  - {pin_num: L12, peripheral: ENET, signal: RMII_RXD1, pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/I2C2_SCL/I2S0_TXD0/FTM1_QD_PHA/TPM1_CH0}
  - {pin_num: J13, peripheral: ENET, signal: RMII_TXD0, pin_signal: CMP3_IN2/PTA16/SPI0_SOUT/UART0_CTS_b/UART0_COL_b/RMII0_TXD0/MII0_TXD0/I2S0_RX_FS/I2S0_RXD1}
  - {pin_num: K13, peripheral: ENET, signal: RMII_CRS_DV, pin_signal: PTA14/SPI0_PCS0/UART0_TX/RMII0_CRS_DV/MII0_RXDV/I2C2_SCL/I2S0_RX_BCLK/I2S0_TXD1}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitENETPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitENETPins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTA12 (pin L12) is configured as RMII0_RXD1 */
    PORT_SetPinMux(BOARD_RMII0_RXD1_PORT, BOARD_RMII0_RXD1_PIN, kPORT_MuxAlt4);

    /* PORTA13 (pin L11) is configured as RMII0_RXD0 */
    PORT_SetPinMux(BOARD_RMII0_RXD0_PORT, BOARD_RMII0_RXD0_PIN, kPORT_MuxAlt4);

    /* PORTA14 (pin K13) is configured as RMII0_CRS_DV */
    PORT_SetPinMux(BOARD_RMII0_CRS_DV_PORT, BOARD_RMII0_CRS_DV_PIN, kPORT_MuxAlt4);

    /* PORTA15 (pin K12) is configured as RMII0_TXEN */
    PORT_SetPinMux(BOARD_RMII0_TXEN_PORT, BOARD_RMII0_TXEN_PIN, kPORT_MuxAlt4);

    /* PORTA16 (pin J13) is configured as RMII0_TXD0 */
    PORT_SetPinMux(BOARD_RMII0_TXD0_PORT, BOARD_RMII0_TXD0_PIN, kPORT_MuxAlt4);

    /* PORTA17 (pin J12) is configured as RMII0_TXD1 */
    PORT_SetPinMux(BOARD_RMII0_TXD1_PORT, BOARD_RMII0_TXD1_PIN, kPORT_MuxAlt4);

    /* PORTA5 (pin N10) is configured as RMII0_RXER */
    PORT_SetPinMux(BOARD_RMII0_RXER_PORT, BOARD_RMII0_RXER_PIN, kPORT_MuxAlt4);

    /* PORTA7 (pin L10) is configured as RMII0_MDIO */
    PORT_SetPinMux(BOARD_RMII0_MDIO_PORT, BOARD_RMII0_MDIO_PIN, kPORT_MuxAlt5);

    /* PORTA8 (pin K9) is configured as RMII0_MDC */
    PORT_SetPinMux(BOARD_RMII0_MDC_PORT, BOARD_RMII0_MDC_PIN, kPORT_MuxAlt5);

    /* PORTE26 (pin K8) is configured as ENET_1588_CLKIN */
    PORT_SetPinMux(BOARD_ENET_1588_CLKIN_PORT, BOARD_ENET_1588_CLKIN_PIN, kPORT_MuxAlt2);

    SIM->SOPT2 = ((SIM->SOPT2 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT2_RMIISRC_MASK)))

                  /* RMII clock source select: External bypass clock (ENET_1588_CLKIN). */
                  | SIM_SOPT2_RMIISRC(SOPT2_RMIISRC_ENET));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitOSCsPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: N5, peripheral: RTC, signal: XTAL32, pin_signal: XTAL32}
  - {pin_num: N6, peripheral: RTC, signal: EXTAL32, pin_signal: EXTAL32}
  - {pin_num: M13, peripheral: OSC, signal: XTAL0, pin_signal: XTAL0/PTA19/FTM1_FLT0/FTM_CLKIN1/LPTMR0_ALT1/TPM_CLKIN1, slew_rate: no_init, open_drain: no_init, pull_select: no_init,
    pull_enable: no_init}
  - {pin_num: N13, peripheral: OSC, signal: EXTAL0, pin_signal: EXTAL0/PTA18/FTM0_FLT2/FTM_CLKIN0/TPM_CLKIN0, slew_rate: no_init, open_drain: no_init, pull_select: no_init,
    pull_enable: no_init}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitOSCsPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitOSCsPins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);

    /* PORTA18 (pin N13) is configured as EXTAL0 */
    PORT_SetPinMux(BOARD_EXTAL0_PORT, BOARD_EXTAL0_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTA19 (pin M13) is configured as XTAL0 */
    PORT_SetPinMux(BOARD_XTAL0_PORT, BOARD_XTAL0_PIN, kPORT_PinDisabledOrAnalog);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
