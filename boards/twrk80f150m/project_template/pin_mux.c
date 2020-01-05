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
processor: MK80FN256xxx15
package_id: MK80FN256VDC15
mcu_data: ksdk2_0
processor_version: 0.0.18
board: TWR-K80F150M
pin_labels:
- {pin_num: J4, pin_signal: NC147, label: NC}
- {pin_num: H6, pin_signal: NC41, label: NC}
- {pin_num: J6, pin_signal: NC146, label: NC}
- {pin_num: A11, pin_signal: NC145, label: NC}
- {pin_num: H3, pin_signal: VSS23, label: GND}
- {pin_num: F6, pin_signal: VSSA, label: GND}
- {pin_num: L6, pin_signal: VSS93, label: GND}
- {pin_num: F7, pin_signal: VSS5, label: GND}
- {pin_num: G7, pin_signal: VSS71, label: GND}
- {pin_num: K10, pin_signal: VSS57, label: GND}
- {pin_num: E5, pin_signal: VDDIO_E6, label: VDDIO_E}
- {pin_num: E6, pin_signal: VDD70, label: 'J9[2]/MCU_PWR'}
- {pin_num: E7, pin_signal: VDD94, label: 'J9[2]/MCU_PWR'}
- {pin_num: L10, pin_signal: VDD56, label: 'J9[2]/MCU_PWR'}
- {pin_num: F5, pin_signal: VDDA, label: 'J10[1]/MCU_PWR'}
- {pin_num: A1, pin_signal: PTD7/CMT_IRO/LPUART0_TX/FTM0_CH7/SDRAM_CKE/FTM0_FLT1/SPI1_SIN, label: 'J13[A56]/U2[37]/USBID/SDRAM_CKE', identifier: SDRAM_CKE;USB_ID}
- {pin_num: B1, pin_signal: PTE0/SPI1_PCS1/LPUART1_TX/SDHC0_D1/QSPI0A_DATA3/I2C1_SDA/RTC_CLKOUT, label: 'U23[7]/QSPI0A_DATA3', identifier: QSPI0A_DATA3}
- {pin_num: C2, pin_signal: PTE1/LLWU_P0/SPI1_SCK/LPUART1_RX/SDHC0_D0/QSPI0A_SCLK/I2C1_SCL/SPI1_SIN, label: 'U23[6]/QSPI0A_SCLK', identifier: QSPI0A_SCLK}
- {pin_num: C1, pin_signal: PTE2/LLWU_P1/SPI1_SOUT/LPUART1_CTS_b/SDHC0_DCLK/QSPI0A_DATA0/SPI1_SCK, label: 'U23[5]/QSPI0A_DATA0', identifier: QSPI0A_DATA0}
- {pin_num: D2, pin_signal: PTE3/SPI1_PCS2/LPUART1_RTS_b/SDHC0_CMD/QSPI0A_DATA2/SPI1_SOUT, label: 'U23[3]/QSPI0A_DATA2', identifier: QSPI0A_DATA2}
- {pin_num: D1, pin_signal: PTE4/LLWU_P2/SPI1_SIN/LPUART3_TX/SDHC0_D3/QSPI0A_DATA1, label: 'U23[2]/QSPI0A_DATA1', identifier: QSPI0A_DATA1}
- {pin_num: E2, pin_signal: PTE5/SPI1_PCS0/LPUART3_RX/SDHC0_D2/QSPI0A_SS0_B/FTM3_CH0/USB0_SOF_OUT, label: 'U23[1]/QSPI0A_SS0', identifier: QSPI0A_SS0}
- {pin_num: E1, pin_signal: PTE6/LLWU_P16/SPI1_PCS3/LPUART3_CTS_b/I2S0_MCLK/QSPI0B_DATA3/FTM3_CH1/SDHC0_D4, label: 'U22[7]/QSPI0B_DATA3', identifier: QSPI0B_DATA3}
- {pin_num: F3, pin_signal: PTE7/SPI2_SCK/LPUART3_RTS_b/I2S0_RXD0/QSPI0B_SCLK/FTM3_CH2/QSPI0A_SS1_B, label: 'U22[6]/QSPI0B_SCLK', identifier: QSPI0B_SCLK}
- {pin_num: F2, pin_signal: PTE8/I2S0_RXD1/SPI2_SOUT/I2S0_RX_FS/QSPI0B_DATA0/FTM3_CH3/SDHC0_D5, label: 'U22[5]/QSPI0B_DATA0', identifier: QSPI0B_DATA0}
- {pin_num: F1, pin_signal: PTE9/LLWU_P17/I2S0_TXD1/SPI2_PCS1/I2S0_RX_BCLK/QSPI0B_DATA2/FTM3_CH4/SDHC0_D6, label: 'U22[3]/QSPI0B_DATA2', identifier: QSPI0B_DATA2}
- {pin_num: G2, pin_signal: PTE10/LLWU_P18/I2C3_SDA/SPI2_SIN/I2S0_TXD0/QSPI0B_DATA1/FTM3_CH5/SDHC0_D7, label: 'U22[2]/QSPI0B_DATA1', identifier: QSPI0B_DATA1}
- {pin_num: G1, pin_signal: PTE11/I2C3_SCL/SPI2_PCS0/I2S0_TX_FS/QSPI0B_SS0_B/FTM3_CH6/QSPI0A_DQS, label: 'U22[1]/QSPI0B_SS0', identifier: QSPI0B_SS0}
- {pin_num: L11, pin_signal: EXTAL0/PTA18/FTM0_FLT2/FTM_CLKIN0/TPM_CLKIN0, label: 'Y1[3]/EXTAL0', identifier: EXTAL0}
- {pin_num: K11, pin_signal: XTAL0/PTA19/FTM1_FLT0/FTM_CLKIN1/LPTMR0_ALT1/LPTMR1_ALT1/TPM_CLKIN1, label: 'Y1[1]/XTAL0', identifier: XTAL0}
- {pin_num: K7, pin_signal: PTA5/USB0_CLKIN/FTM0_CH2/FXIO0_D15/I2S0_TX_BCLK/JTAG_TRST_b, label: 'J13[A22]/I2S0_TX_BCLK'}
- {pin_num: J9, pin_signal: PTA10/LLWU_P22/I2C2_SDA/FTM2_CH0/EMVSIM1_VCCEN/FXIO0_D16/FTM2_QD_PHA/TPM2_CH0/TRACE_D0, label: 'J7[17]/J13[C8]/I2C2_SDA/FXIO0_D16'}
- {pin_num: H7, pin_signal: PTA11/LLWU_P23/I2C2_SCL/FTM2_CH1/FXIO0_D17/FTM2_QD_PHB/TPM2_CH1, label: 'J7[18]/J13[C7]/I2C2_SCL/FTM2_CH1//FXIO0_D17'}
- {pin_num: K8, pin_signal: PTA12/FTM1_CH0/TRACE_CLKOUT/FXIO0_D18/I2S0_TXD0/FTM1_QD_PHA/TPM1_CH0, label: 'J7[19]/J13[A25]/J13[B62]/I2S0_TXD0/FXIO0_D18'}
- {pin_num: L8, pin_signal: PTA13/LLWU_P4/FTM1_CH1/TRACE_D3/FXIO0_D19/I2S0_TX_FS/FTM1_QD_PHB/TPM1_CH1, label: 'J7[20]/J13[A23]/J13[B61]/LLWU_P4/I2S0_TX_FS/FXIO0_D19'}
- {pin_num: K9, pin_signal: PTA14/SPI0_PCS0/LPUART0_TX/TRACE_D2/FXIO0_D20/I2S0_RX_BCLK/I2S0_TXD1, label: 'J13[A42]/J13[A58]/J13[B60]/I2S0_RX_BCLK/LPUART0_TX'}
- {pin_num: L9, pin_signal: PTA15/SPI0_SCK/LPUART0_RX/TRACE_D1/FXIO0_D21/I2S0_RXD0, label: 'J13[A24]/J13[A41]/I2S0_RXD0/LPUART0_RX'}
- {pin_num: J10, pin_signal: PTA16/SPI0_SOUT/LPUART0_CTS_b/TRACE_D0/FXIO0_D22/I2S0_RX_FS/I2S0_RXD1, label: 'J13[A59]/I2S0_RX_FS'}
- {pin_num: H10, pin_signal: PTA17/SPI0_SIN/LPUART0_RTS_b/FXIO0_D23/I2S0_MCLK, label: 'J13[A21]/I2S0_MCLK'}
- {pin_num: H5, pin_signal: PTA20/I2C0_SCL/LPUART4_TX/FTM_CLKIN1/FXIO0_D8/EWM_OUT_b/TPM_CLKIN1, label: 'J7[9]/J13[C40]/FXIO0_D8/EWM_OUT'}
- {pin_num: J5, pin_signal: PTA21/LLWU_P21/I2C0_SDA/LPUART4_RX/FXIO0_D9/EWM_IN, label: 'J7[10]/J13[C39]/SW3/FXIO0_D9/LLWU_P21/EWM_IN', identifier: SW3}
- {pin_num: H11, pin_signal: PTA29/FB_A24, label: 'Q1[3]/USB_PWROUT', identifier: USB_PWR_STAT}
- {pin_num: L7, pin_signal: TSI0_CH1/PTA0/LPUART0_CTS_b/FTM0_CH5/FXIO0_D10/EMVSIM0_CLK/JTAG_TCLK/SWD_CLK, label: 'J7[11]/J11[4]/U13[4]/JTAG_TCLK/SWD_CLKCH5/FXIO0_D10'}
- {pin_num: H8, pin_signal: TSI0_CH2/PTA1/LPUART0_RX/FTM0_CH6/I2C3_SDA/FXIO0_D11/EMVSIM0_IO/JTAG_TDI, label: 'J7[12]/J11[8]/JTAG_TDI/FXIO0_D11'}
- {pin_num: J7, pin_signal: TSI0_CH3/PTA2/LPUART0_TX/FTM0_CH7/I2C3_SCL/FXIO0_D12/EMVSIM0_PD/JTAG_TDO/TRACE_SWO, label: 'J7[13]/J11[6]/JTAG_TDO/TRACE_SWO/FXIO0_D12'}
- {pin_num: H9, pin_signal: TSI0_CH4/PTA3/LPUART0_RTS_b/FTM0_CH0/FXIO0_D13/EMVSIM0_RST/JTAG_TMS/SWD_DIO, label: 'J11[2]/U11[4]/U25[2]/JTAG_TMS/SWD_DIO'}
- {pin_num: J8, pin_signal: TSI0_CH5/PTA4/LLWU_P3/FTM0_CH1/FXIO0_D14/EMVSIM0_VCCEN/NMI_b, label: 'J12[4]/SW2/NMI/TSI0_CH5', identifier: SW2}
- {pin_num: L4, pin_signal: XTAL32, label: 'Y2[1]/XTAL32', identifier: XTAL32K}
- {pin_num: L5, pin_signal: EXTAL32, label: 'Y2[2]/EXTAL32', identifier: EXTAL32K}
- {pin_num: G11, pin_signal: ADC0_SE8/TSI0_CH0/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/SDRAM_CAS_b/FTM1_QD_PHA/TPM1_CH0/FXIO0_D0, label: 'J7[1]/J12[5]/U2[17]/J13[A38]/FXIO0_D0/FTM1_CH0/SDRAM_CAS/TSI0_CH0',
  identifier: SDRAM_CAS}
- {pin_num: G10, pin_signal: ADC0_SE9/TSI0_CH6/PTB1/I2C0_SDA/FTM1_CH1/SDRAM_RAS_b/FTM1_QD_PHB/TPM1_CH1/FXIO0_D1, label: 'J7[2]/J12[7]/U2[18]/J13[A28]/J13[A37]/FXIO0_D1/FTM1_CH1/SDRAM_RAS/TSI0_CH6',
  identifier: SDRAM_RAS}
- {pin_num: G9, pin_signal: ADC0_SE12/TSI0_CH7/PTB2/I2C0_SCL/LPUART0_RTS_b/SDRAM_WE/FTM0_FLT3/FXIO0_D2, label: 'J7[3]/J12[8]/U2[16]/FXIO0_D2/SDRAM_WE/TSI0_CH7', identifier: SDRAM_WE}
- {pin_num: G8, pin_signal: ADC0_SE13/TSI0_CH8/PTB3/I2C0_SDA/LPUART0_CTS_b/SDRAM_CS0_b/FTM0_FLT0/FXIO0_D3, label: 'J7[4]/J12[9]/U2[19]/FXIO0_D3/SDRAM_CS0/TSI0_CH8',
  identifier: SDRAM_CS0}
- {pin_num: B11, pin_signal: PTB4/EMVSIM1_IO/SDRAM_CS1_b/FTM1_FLT0, label: 'J13[C61]/J35[C7]/EMVSIM1_IO', identifier: SMARTCARD_IO}
- {pin_num: C11, pin_signal: PTB5/EMVSIM1_CLK/FTM2_FLT0, label: 'J13[B59]/J13[C58]/J35[C3]/EMVSIM1_CLK', identifier: SMARTCARD_CLK}
- {pin_num: F11, pin_signal: PTB6/EMVSIM1_VCCEN/FB_AD23/SDRAM_D23, label: 'J13[C25]/U7[B2]/U2[13]/EMVSIM1_VCCEN/SDRAM_D23', identifier: SMARTCARD_VCCEN;SDRAM_D23}
- {pin_num: E11, pin_signal: PTB7/EMVSIM1_PD/FB_AD22/SDRAM_D22, label: 'J13[C22]/J35[SW1]/U2[11]/EMVSIM1_PD/SDRAM_D22', identifier: SMARTCARD_PD;SDRAM_D22}
- {pin_num: D11, pin_signal: PTB8/EMVSIM1_RST/LPUART3_RTS_b/FB_AD21/SDRAM_D21, label: 'J13[C59]/J35[C2]/U2[10]/EMVSIM1_RST/SDRAM_D21', identifier: SMARTCARD_RST;SDRAM_D21}
- {pin_num: E10, pin_signal: PTB9/SPI1_PCS1/LPUART3_CTS_b/FB_AD20/SDRAM_D20, label: 'J13[B58]/U2[8]/SDRAM_D20', identifier: SDRAM_D20}
- {pin_num: D10, pin_signal: PTB10/SPI1_PCS0/LPUART3_RX/I2C2_SCL/FB_AD19/SDRAM_D19/FTM0_FLT1/FXIO0_D4, label: 'J7[5]/J13[B57]/J13[B70]/U2[7]/FBa_AD19/SDRAM_D19/FXIO0_D4',
  identifier: SDRAM_D19}
- {pin_num: C10, pin_signal: PTB11/SPI1_SCK/LPUART3_TX/I2C2_SDA/FB_AD18/SDRAM_D18/FTM0_FLT2/FXIO0_D5, label: 'J7[6]/J13[B69]/U2[5]/FBa_AD18/SDRAM_D18/FXIO0_D5', identifier: SDRAM_D18}
- {pin_num: B10, pin_signal: TSI0_CH9/PTB16/SPI1_SOUT/LPUART0_RX/FTM_CLKIN0/FB_AD17/SDRAM_D17/EWM_IN/TPM_CLKIN0, label: 'J12[10]/J13[B68]/U2[4]/D4[1]/TSI0_CH9/FBa_AD17/SDRAM_D17/BRD_PAD2',
  identifier: TSI_ELECTRODE_1;SDRAM_D17}
- {pin_num: E9, pin_signal: TSI0_CH10/PTB17/SPI1_SIN/LPUART0_TX/FTM_CLKIN1/FB_AD16/SDRAM_D16/EWM_OUT_b/TPM_CLKIN1, label: 'J12[11]/J13[B67]/U2[2]/D13[1]/TSI0_CH10/FBa_AD16/SDRAM_D16/BRD_PAD1',
  identifier: TSI_ELECTRODE_2;SDRAM_D16}
- {pin_num: D9, pin_signal: TSI0_CH11/PTB18/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/SDRAM_A23/FTM2_QD_PHA/TPM2_CH0/FXIO0_D6, label: 'J7[7]/J12[12]/J13[A34]/J13[B66]/TSI0_CH11/FBa_AD15/SDRAM_A23/FXIO0_D6/FTM2_CH0',
  identifier: SDRAM_A23}
- {pin_num: C9, pin_signal: TSI0_CH12/PTB19/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB/TPM2_CH1/FXIO0_D7, label: 'J7[8]/J12[13]/J13[A33]/J13[B72]/TSI0_CH12/FBa_OE/FXIO0_D7/FTM2_CH1'}
- {pin_num: F10, pin_signal: PTB20/SPI2_PCS0/FB_AD31/SDRAM_D31/CMP0_OUT/FXIO0_D8, label: 'J13[B73]/U2[53]/FBa_AD31/SDRAM_D31', identifier: SDRAM_D31}
- {pin_num: F9, pin_signal: PTB21/SPI2_SCK/FB_AD30/SDRAM_D30/CMP1_OUT/FXIO0_D9, label: 'J13[B74]/U2[51]/FBa_AD30/SDRAM_D30', identifier: SDRAM_D30}
- {pin_num: F8, pin_signal: PTB22/SPI2_SOUT/FB_AD29/SDRAM_D29/FXIO0_D10, label: 'J13[B75]/U2[50]/FBa_AD29/SDRAM_D29', identifier: SDRAM_D29}
- {pin_num: E8, pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28/SDRAM_D28/FXIO0_D11, label: 'J13[B76]/U2[48]/FBa_AD28/SDRAM_D28', identifier: SDRAM_D28}
- {pin_num: D8, pin_signal: ADC0_SE15/TSI0_CH14/PTC1/LLWU_P6/SPI0_PCS3/LPUART1_RTS_b/FTM0_CH0/FB_AD13/SDRAM_A21/I2S0_TXD0/FXIO0_D13, label: 'J7[14]/J12[15]/J13[A40]/J13[A67]/J13[B21]/U2[20]/FBa_AD13/SDRAM_A21/FXIO0_D13/TSI0_CH14',
  identifier: SDRAM_A21}
- {pin_num: B9, pin_signal: ADC0_SE14/TSI0_CH13/PTC0/SPI0_PCS4/PDB0_EXTRG/USB0_SOF_OUT/FB_AD14/SDRAM_A22/I2S0_TXD1/FXIO0_D12, label: 'J12[14]/J13[A66]/U2[21]/FBa_AD14/SDRAM_A22/TSI0_CH13',
  identifier: SDRAM_A22}
- {pin_num: C8, pin_signal: ADC0_SE4b/CMP1_IN0/TSI0_CH15/PTC2/SPI0_PCS2/LPUART1_CTS_b/FTM0_CH1/FB_AD12/SDRAM_A20/I2S0_TX_FS, label: 'J12[16]/J13[A9]/J13[A39]/J13[A68]/U2[35]/FBa_AD12/SDRAM_A20/TSI0_CH15',
  identifier: SDRAM_A20}
- {pin_num: C7, pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/SDRAM_A17/I2S0_MCLK/FXIO0_D14, label: 'J7[15]/J13[A71]/J13[B45]/U2[33]/SPI0_SOUT/FXIO0_D14/FBa_AD9/SDRAM_A17',
  identifier: SDRAM_A17}
- {pin_num: B7, pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB0_SOF_OUT/I2S0_RX_FS/FB_AD8/SDRAM_A16/FXIO0_D15, label: 'J7[16]/J13[A72]/J13[B44]/U2[23]/SPI0_SIN/FBa_AD8/SDRAM_A16/FXIO0_D15',
  identifier: SDRAM_A16}
- {pin_num: A7, pin_signal: CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7/SDRAM_A15/FXIO0_D16, label: 'J13[A73]/U2[24]/FBa_AD7/SDRAM_A15', identifier: SDRAM_A15}
- {pin_num: D6, pin_signal: CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/SDRAM_A14/FTM2_FLT0/FXIO0_D17, label: 'J13[A74]/U2[25]/FBa_AD6/SDRAM_A14', identifier: SDRAM_A14}
- {pin_num: B8, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK, label: 'J6[2]/LPUART1_RX/CLKOUT', identifier: SDRAM_CLK;DEBUG_UART_RX}
- {pin_num: A8, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT, label: 'J8[2]/J13[A69]/U2[22]/LPUART1_TX/FBa_AD11/SDRAM_A19', identifier: SDRAM_A19;DEBUG_UART_TX}
- {pin_num: D7, pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/LPTMR1_ALT2/I2S0_RXD0/FB_AD10/SDRAM_A18/CMP0_OUT/FTM0_CH2, label: 'J13[A70]/U2[34]/FBa_AD10/SDRAM_A18',
  identifier: SDRAM_A18}
- {pin_num: C6, pin_signal: PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5/SDRAM_A13/FXIO0_D18, label: 'J13[A75]/J13[B50]/U2[26]/I2C1_SCL/FBa_AD5/SDRAM_A13', identifier: SDRAM_A13}
- {pin_num: C5, pin_signal: PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/I2S0_RXD1/FB_RW_b/FXIO0_D19, label: 'J13[B51]/J13[B71]/I2C1_SDA/FBa_RW'}
- {pin_num: B6, pin_signal: PTC12/LPUART4_RTS_b/FTM_CLKIN0/FB_AD27/SDRAM_D27/FTM3_FLT0/TPM_CLKIN0, label: 'J13[B77]/U2[47]/FBa_AD27/SDRAM_D27', identifier: SDRAM_D27}
- {pin_num: A6, pin_signal: PTC13/LPUART4_CTS_b/FTM_CLKIN1/FB_AD26/SDRAM_D26/TPM_CLKIN1, label: 'J13[B78]/U2[45]/FBa_AD26/SDRAM_D26', identifier: SDRAM_D26}
- {pin_num: A5, pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20, label: 'J13[B79]/U2[44]/FBa_AD25/SDRAM_D25', identifier: SDRAM_D25}
- {pin_num: B5, pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21, label: 'J13[B80]/U2[42]/FBa_AD24/SDRAM_D24', identifier: SDRAM_D24}
- {pin_num: D5, pin_signal: PTC16/LPUART3_RX/FB_CS5_b/FB_TSIZ1/FB_BE23_16_BLS15_8_b/SDRAM_DQM2, label: 'U2[15]/SDRAM_DQM2', identifier: SDRAM_DQM2}
- {pin_num: C4, pin_signal: PTC17/LPUART3_TX/FB_CS4_b/FB_TSIZ0/FB_BE31_24_BLS7_0_b/SDRAM_DQM3, label: 'J1[8]/U2[39]/SDRAM_DQM3', identifier: SDRAM_DQM3}
- {pin_num: B4, pin_signal: PTC18/LPUART3_RTS_b/FB_TBST_b/FB_CS2_b/FB_BE15_8_BLS23_16_b/SDRAM_DQM1, label: 'J18[2]/U5[4]/K80_USB_FLGA', identifier: USB_FLAG}
- {pin_num: A4, pin_signal: PTC19/LPUART3_CTS_b/FB_CS3_b/FB_BE7_0_BLS31_24_b/SDRAM_DQM0/FB_TA_b, label: 'J28[2]/U5[3]/K80_USB_EN', identifier: USB_PWR_EN}
- {pin_num: D4, pin_signal: PTD0/LLWU_P12/SPI0_PCS0/LPUART2_RTS_b/FTM3_CH0/FB_ALE/FB_CS1_b/FB_TS_b/FXIO0_D22, label: 'J13[B48]/J13[B63]/SPI0_PCS0/FBa_ALE/FBa_CS1'}
- {pin_num: D3, pin_signal: ADC0_SE5b/PTD1/SPI0_SCK/LPUART2_CTS_b/FTM3_CH1/FB_CS0_b/FXIO0_D23, label: 'J13[B46]/J13[B64]/SPI0_SCK/FBa_CS0'}
- {pin_num: C3, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/LPUART2_RX/FTM3_CH2/FB_AD4/SDRAM_A12/I2C0_SCL, label: 'J13[A76]/J13[B35]/U2[29]/FBa_AD4/SDRAM_A12', identifier: SDRAM_A12}
- {pin_num: B3, pin_signal: PTD3/SPI0_SIN/LPUART2_TX/FTM3_CH3/FB_AD3/SDRAM_A11/I2C0_SDA, label: 'J13[A77]/J13[B52]/U2[30]/FBa_AD3/SDRAM_A11', identifier: SDRAM_A11}
- {pin_num: A3, pin_signal: PTD4/LLWU_P14/SPI0_PCS1/LPUART0_RTS_b/FTM0_CH4/FB_AD2/SDRAM_A10/EWM_IN/SPI1_PCS0, label: 'J13[A78]/J13[B47]/U2[31]/FBa_AD2/SDRAM_A10',
  identifier: SDRAM_A10}
- {pin_num: A2, pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/LPUART0_CTS_b/FTM0_CH5/FB_AD1/SDRAM_A9/EWM_OUT_b/SPI1_SCK, label: 'J13[A27]/J13[A79]/U2[32]/FBa_AD1/SDRAM_A9',
  identifier: SDRAM_A9}
- {pin_num: B2, pin_signal: ADC0_SE7b/PTD6/LLWU_P15/SPI0_PCS3/LPUART0_RX/FTM0_CH6/FB_AD0/FTM0_FLT0/SPI1_SOUT, label: 'J13[A11]/J13[A80]/J13[B52]/U2[30]/FBa_AD0'}
- {pin_num: A10, pin_signal: PTD8/LLWU_P24/I2C0_SCL/FB_A16/FXIO0_D24, label: 'J13[A7]/U1[11]/U3[8]/U26[4]/I2C0_SCL', identifier: ACCEL_SCL;GYRO_SCL;PRESSURE_SCL}
- {pin_num: A9, pin_signal: PTD9/I2C0_SDA/FB_A17/FXIO0_D25, label: 'J13[A8]/U1[12]/U3[7]/U26[6]/I2C0_SDA', identifier: ACCEL_SDA;GYRO_SDA;PRESSURE_SDA}
- {pin_num: E3, pin_signal: PTD11/LLWU_P25/SPI2_PCS0/FB_A19/FXIO0_D27, label: D1/LED_J_PTD11, identifier: LED_RED}
- {pin_num: E4, pin_signal: PTD10/FB_A18/FXIO0_D26, label: 'Q4[1]/CHG_EN'}
- {pin_num: F4, pin_signal: PTD12/SPI2_SCK/FTM3_FLT0/FB_A20/FXIO0_D28, label: D2/LED_J_PTD12, identifier: LED_GREEN}
- {pin_num: G3, pin_signal: PTD13/SPI2_SOUT/FB_A21/FXIO0_D29, label: D3/LED_J_PTD13, identifier: LED_BLUE}
- {pin_num: G4, pin_signal: PTD14/SPI2_SIN/FB_A22/FXIO0_D30, label: D13/TP_LED1RR, identifier: LED_TSI_PAD2}
- {pin_num: H4, pin_signal: PTD15/SPI2_PCS1/FB_A23/FXIO0_D31, label: D14/TP_LED2RR, identifier: LED_TSI_PAD1}
- {pin_num: K1, pin_signal: ADC0_DM0, label: 'J12[18]/TWRPI_ID1'}
- {pin_num: L1, pin_signal: ADC0_DM1, label: 'J13[A29]'}
- {pin_num: K3, pin_signal: ADC0_DM3, label: 'R44[2]/POT_5K/ADC0_DM3', identifier: ADC_POT}
- {pin_num: K2, pin_signal: ADC0_DP0, label: 'J12[17]/TWRPI_ID0'}
- {pin_num: L2, pin_signal: ADC0_DP1, label: 'J13[A30]'}
- {pin_num: J3, pin_signal: ADC0_DP3, label: R272/R276/BAT_OUT/ADC0_DP3}
- {pin_num: K4, pin_signal: DAC0_OUT/CMP1_IN3/ADC0_SE23, label: 'J13[A32]'}
- {pin_num: H1, pin_signal: USB0_DM, label: 'U9[2]/K80_USB0_DM', identifier: USB_DM}
- {pin_num: H2, pin_signal: USB0_DP, label: 'U9[1]/K80_USB0_DP', identifier: USB_DP}
- {pin_num: J1, pin_signal: VOUT33, label: 'J25[2]/P3V3_K80_VOUT', identifier: USB_VOUT}
- {pin_num: J2, pin_signal: VREGIN, label: 'J22[2]/5V0_VREGIN_K80', identifier: USB_VREGIN}
- {pin_num: L3, pin_signal: VREF_OUT/CMP1_IN5/CMP0_IN5/ADC0_SE22, label: TP20/VREF_OUT}
- {pin_num: G5, pin_signal: VREFH, label: VREFH}
- {pin_num: G6, pin_signal: VREFL, label: GND}
- {pin_num: K5, pin_signal: RTC_WAKEUP_B, label: RTC_WAKEUP}
- {pin_num: K6, pin_signal: VBAT, label: 'J3[2]/VBAT'}
- {pin_num: J11, pin_signal: RESET_b, label: 'J11[10]/J12[20]/J13[A62]/U24[7]/RESET'}
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
  - {pin_num: J7, peripheral: TPIU, signal: SWO, pin_signal: TSI0_CH3/PTA2/LPUART0_TX/FTM0_CH7/I2C3_SCL/FXIO0_D12/EMVSIM0_PD/JTAG_TDO/TRACE_SWO, pull_select: down,
    pull_enable: disable}
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
BOARD_InitBUTTONsPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: J8, peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: TSI0_CH5/PTA4/LLWU_P3/FTM0_CH1/FXIO0_D14/EMVSIM0_VCCEN/NMI_b, direction: INPUT, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable, passive_filter: disable}
  - {pin_num: J5, peripheral: GPIOA, signal: 'GPIO, 21', pin_signal: PTA21/LLWU_P21/I2C0_SDA/LPUART4_RX/FXIO0_D9/EWM_IN, direction: INPUT, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
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
    /* Initialize GPIO functionality on pin PTA4 (pin J8)  */
    GPIO_PinInit(BOARD_SW2_GPIO, BOARD_SW2_PIN, &SW2_config);

    gpio_pin_config_t SW3_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTA21 (pin J5)  */
    GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_PIN, &SW3_config);

    const port_pin_config_t SW3 = {/* Internal pull-up/down resistor is disabled */
                                   kPORT_PullDisable,
                                   /* Fast slew rate is configured */
                                   kPORT_FastSlewRate,
                                   /* Passive filter is disabled */
                                   kPORT_PassiveFilterDisable,
                                   /* Open drain is disabled */
                                   kPORT_OpenDrainDisable,
                                   /* Low drive strength is configured */
                                   kPORT_LowDriveStrength,
                                   /* Pin is configured as PTA21 */
                                   kPORT_MuxAsGpio,
                                   /* Pin Control Register fields [15:0] are not locked */
                                   kPORT_UnlockRegister};
    /* PORTA21 (pin J5) is configured as PTA21 */
    PORT_SetPinConfig(BOARD_SW3_PORT, BOARD_SW3_PIN, &SW3);

    const port_pin_config_t SW2 = {/* Internal pull-up/down resistor is disabled */
                                   kPORT_PullDisable,
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
    /* PORTA4 (pin J8) is configured as PTA4 */
    PORT_SetPinConfig(BOARD_SW2_PORT, BOARD_SW2_PIN, &SW2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDsPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: F4, peripheral: GPIOD, signal: 'GPIO, 12', pin_signal: PTD12/SPI2_SCK/FTM3_FLT0/FB_A20/FXIO0_D28, direction: OUTPUT, gpio_init_state: 'true', slew_rate: slow,
    open_drain: disable, pull_select: down, pull_enable: disable, digital_filter: disable}
  - {pin_num: G3, peripheral: GPIOD, signal: 'GPIO, 13', pin_signal: PTD13/SPI2_SOUT/FB_A21/FXIO0_D29, direction: OUTPUT, gpio_init_state: 'true', slew_rate: slow,
    open_drain: disable, pull_select: down, pull_enable: disable, digital_filter: disable}
  - {pin_num: E3, peripheral: GPIOD, signal: 'GPIO, 11', pin_signal: PTD11/LLWU_P25/SPI2_PCS0/FB_A19/FXIO0_D27, direction: OUTPUT, gpio_init_state: 'true', slew_rate: slow,
    open_drain: disable, pull_select: down, pull_enable: disable, digital_filter: disable}
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

    gpio_pin_config_t LED_RED_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTD11 (pin E3)  */
    GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, &LED_RED_config);

    gpio_pin_config_t LED_GREEN_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTD12 (pin F4)  */
    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, &LED_GREEN_config);

    gpio_pin_config_t LED_BLUE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTD13 (pin G3)  */
    GPIO_PinInit(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, &LED_BLUE_config);
    /* Configure digital filter */
    PORT_EnablePinsDigitalFilter(
        /* Digital filter is configured on port D */
        PORTD,
        /* Digital filter is configured for PORTD0 */
          PORT_DFER_DFE_11_MASK
            /* Digital filter is configured for PORTD1 */
            | PORT_DFER_DFE_12_MASK
            /* Digital filter is configured for PORTD2 */
            | PORT_DFER_DFE_13_MASK,
        /* Disable digital filter */
        false);

    const port_pin_config_t LED_RED = {/* Internal pull-up/down resistor is disabled */
                                       kPORT_PullDisable,
                                       /* Slow slew rate is configured */
                                       kPORT_SlowSlewRate,
                                       /* Passive filter is disabled */
                                       kPORT_PassiveFilterDisable,
                                       /* Open drain is disabled */
                                       kPORT_OpenDrainDisable,
                                       /* Low drive strength is configured */
                                       kPORT_LowDriveStrength,
                                       /* Pin is configured as PTD11 */
                                       kPORT_MuxAsGpio,
                                       /* Pin Control Register fields [15:0] are not locked */
                                       kPORT_UnlockRegister};
    /* PORTD11 (pin E3) is configured as PTD11 */
    PORT_SetPinConfig(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, &LED_RED);

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
                                         /* Pin is configured as PTD12 */
                                         kPORT_MuxAsGpio,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTD12 (pin F4) is configured as PTD12 */
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
                                        /* Pin is configured as PTD13 */
                                        kPORT_MuxAsGpio,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTD13 (pin G3) is configured as PTD13 */
    PORT_SetPinConfig(BOARD_LED_BLUE_PORT, BOARD_LED_BLUE_PIN, &LED_BLUE);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitQSPI0APins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: B1, peripheral: QuadSPI0, signal: 'DATA3, A', pin_signal: PTE0/SPI1_PCS1/LPUART1_TX/SDHC0_D1/QSPI0A_DATA3/I2C1_SDA/RTC_CLKOUT, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: C2, peripheral: QuadSPI0, signal: 'SCLK, A', pin_signal: PTE1/LLWU_P0/SPI1_SCK/LPUART1_RX/SDHC0_D0/QSPI0A_SCLK/I2C1_SCL/SPI1_SIN, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: C1, peripheral: QuadSPI0, signal: 'DATA0, A', pin_signal: PTE2/LLWU_P1/SPI1_SOUT/LPUART1_CTS_b/SDHC0_DCLK/QSPI0A_DATA0/SPI1_SCK, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: D2, peripheral: QuadSPI0, signal: 'DATA2, A', pin_signal: PTE3/SPI1_PCS2/LPUART1_RTS_b/SDHC0_CMD/QSPI0A_DATA2/SPI1_SOUT, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: D1, peripheral: QuadSPI0, signal: 'DATA1, A', pin_signal: PTE4/LLWU_P2/SPI1_SIN/LPUART3_TX/SDHC0_D3/QSPI0A_DATA1, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: E2, peripheral: QuadSPI0, signal: 'SS0, A', pin_signal: PTE5/SPI1_PCS0/LPUART3_RX/SDHC0_D2/QSPI0A_SS0_B/FTM3_CH0/USB0_SOF_OUT, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitQSPI0APins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitQSPI0APins(void)
{
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t QSPI0A_DATA3 = {/* Internal pull-up/down resistor is disabled */
                                            kPORT_PullDisable,
                                            /* Fast slew rate is configured */
                                            kPORT_FastSlewRate,
                                            /* Passive filter is disabled */
                                            kPORT_PassiveFilterDisable,
                                            /* Open drain is disabled */
                                            kPORT_OpenDrainDisable,
                                            /* Low drive strength is configured */
                                            kPORT_LowDriveStrength,
                                            /* Pin is configured as QSPI0A_DATA3 */
                                            kPORT_MuxAlt5,
                                            /* Pin Control Register fields [15:0] are not locked */
                                            kPORT_UnlockRegister};
    /* PORTE0 (pin B1) is configured as QSPI0A_DATA3 */
    PORT_SetPinConfig(BOARD_QSPI0A_DATA3_PORT, BOARD_QSPI0A_DATA3_PIN, &QSPI0A_DATA3);

    const port_pin_config_t QSPI0A_SCLK = {/* Internal pull-up/down resistor is disabled */
                                           kPORT_PullDisable,
                                           /* Fast slew rate is configured */
                                           kPORT_FastSlewRate,
                                           /* Passive filter is disabled */
                                           kPORT_PassiveFilterDisable,
                                           /* Open drain is disabled */
                                           kPORT_OpenDrainDisable,
                                           /* Low drive strength is configured */
                                           kPORT_LowDriveStrength,
                                           /* Pin is configured as QSPI0A_SCLK */
                                           kPORT_MuxAlt5,
                                           /* Pin Control Register fields [15:0] are not locked */
                                           kPORT_UnlockRegister};
    /* PORTE1 (pin C2) is configured as QSPI0A_SCLK */
    PORT_SetPinConfig(BOARD_QSPI0A_SCLK_PORT, BOARD_QSPI0A_SCLK_PIN, &QSPI0A_SCLK);

    const port_pin_config_t QSPI0A_DATA0 = {/* Internal pull-up/down resistor is disabled */
                                            kPORT_PullDisable,
                                            /* Fast slew rate is configured */
                                            kPORT_FastSlewRate,
                                            /* Passive filter is disabled */
                                            kPORT_PassiveFilterDisable,
                                            /* Open drain is disabled */
                                            kPORT_OpenDrainDisable,
                                            /* Low drive strength is configured */
                                            kPORT_LowDriveStrength,
                                            /* Pin is configured as QSPI0A_DATA0 */
                                            kPORT_MuxAlt5,
                                            /* Pin Control Register fields [15:0] are not locked */
                                            kPORT_UnlockRegister};
    /* PORTE2 (pin C1) is configured as QSPI0A_DATA0 */
    PORT_SetPinConfig(BOARD_QSPI0A_DATA0_PORT, BOARD_QSPI0A_DATA0_PIN, &QSPI0A_DATA0);

    const port_pin_config_t QSPI0A_DATA2 = {/* Internal pull-up/down resistor is disabled */
                                            kPORT_PullDisable,
                                            /* Fast slew rate is configured */
                                            kPORT_FastSlewRate,
                                            /* Passive filter is disabled */
                                            kPORT_PassiveFilterDisable,
                                            /* Open drain is disabled */
                                            kPORT_OpenDrainDisable,
                                            /* Low drive strength is configured */
                                            kPORT_LowDriveStrength,
                                            /* Pin is configured as QSPI0A_DATA2 */
                                            kPORT_MuxAlt5,
                                            /* Pin Control Register fields [15:0] are not locked */
                                            kPORT_UnlockRegister};
    /* PORTE3 (pin D2) is configured as QSPI0A_DATA2 */
    PORT_SetPinConfig(BOARD_QSPI0A_DATA2_PORT, BOARD_QSPI0A_DATA2_PIN, &QSPI0A_DATA2);

    const port_pin_config_t QSPI0A_DATA1 = {/* Internal pull-up/down resistor is disabled */
                                            kPORT_PullDisable,
                                            /* Fast slew rate is configured */
                                            kPORT_FastSlewRate,
                                            /* Passive filter is disabled */
                                            kPORT_PassiveFilterDisable,
                                            /* Open drain is disabled */
                                            kPORT_OpenDrainDisable,
                                            /* Low drive strength is configured */
                                            kPORT_LowDriveStrength,
                                            /* Pin is configured as QSPI0A_DATA1 */
                                            kPORT_MuxAlt5,
                                            /* Pin Control Register fields [15:0] are not locked */
                                            kPORT_UnlockRegister};
    /* PORTE4 (pin D1) is configured as QSPI0A_DATA1 */
    PORT_SetPinConfig(BOARD_QSPI0A_DATA1_PORT, BOARD_QSPI0A_DATA1_PIN, &QSPI0A_DATA1);

    const port_pin_config_t QSPI0A_SS0 = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as QSPI0A_SS0_B */
                                          kPORT_MuxAlt5,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTE5 (pin E2) is configured as QSPI0A_SS0_B */
    PORT_SetPinConfig(BOARD_QSPI0A_SS0_PORT, BOARD_QSPI0A_SS0_PIN, &QSPI0A_SS0);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitQSPI0BPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: E1, peripheral: QuadSPI0, signal: 'DATA3, B', pin_signal: PTE6/LLWU_P16/SPI1_PCS3/LPUART3_CTS_b/I2S0_MCLK/QSPI0B_DATA3/FTM3_CH1/SDHC0_D4, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: F3, peripheral: QuadSPI0, signal: 'SCLK, B', pin_signal: PTE7/SPI2_SCK/LPUART3_RTS_b/I2S0_RXD0/QSPI0B_SCLK/FTM3_CH2/QSPI0A_SS1_B, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: F2, peripheral: QuadSPI0, signal: 'DATA0, B', pin_signal: PTE8/I2S0_RXD1/SPI2_SOUT/I2S0_RX_FS/QSPI0B_DATA0/FTM3_CH3/SDHC0_D5, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: F1, peripheral: QuadSPI0, signal: 'DATA2, B', pin_signal: PTE9/LLWU_P17/I2S0_TXD1/SPI2_PCS1/I2S0_RX_BCLK/QSPI0B_DATA2/FTM3_CH4/SDHC0_D6, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: G2, peripheral: QuadSPI0, signal: 'DATA1, B', pin_signal: PTE10/LLWU_P18/I2C3_SDA/SPI2_SIN/I2S0_TXD0/QSPI0B_DATA1/FTM3_CH5/SDHC0_D7, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: G1, peripheral: QuadSPI0, signal: 'SS0, B', pin_signal: PTE11/I2C3_SCL/SPI2_PCS0/I2S0_TX_FS/QSPI0B_SS0_B/FTM3_CH6/QSPI0A_DQS, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitQSPI0BPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitQSPI0BPins(void)
{
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    const port_pin_config_t QSPI0B_DATA1 = {/* Internal pull-up/down resistor is disabled */
                                            kPORT_PullDisable,
                                            /* Fast slew rate is configured */
                                            kPORT_FastSlewRate,
                                            /* Passive filter is disabled */
                                            kPORT_PassiveFilterDisable,
                                            /* Open drain is disabled */
                                            kPORT_OpenDrainDisable,
                                            /* Low drive strength is configured */
                                            kPORT_LowDriveStrength,
                                            /* Pin is configured as QSPI0B_DATA1 */
                                            kPORT_MuxAlt5,
                                            /* Pin Control Register fields [15:0] are not locked */
                                            kPORT_UnlockRegister};
    /* PORTE10 (pin G2) is configured as QSPI0B_DATA1 */
    PORT_SetPinConfig(BOARD_QSPI0B_DATA1_PORT, BOARD_QSPI0B_DATA1_PIN, &QSPI0B_DATA1);

    const port_pin_config_t QSPI0B_SS0 = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as QSPI0B_SS0_B */
                                          kPORT_MuxAlt5,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTE11 (pin G1) is configured as QSPI0B_SS0_B */
    PORT_SetPinConfig(BOARD_QSPI0B_SS0_PORT, BOARD_QSPI0B_SS0_PIN, &QSPI0B_SS0);

    const port_pin_config_t QSPI0B_DATA3 = {/* Internal pull-up/down resistor is disabled */
                                            kPORT_PullDisable,
                                            /* Fast slew rate is configured */
                                            kPORT_FastSlewRate,
                                            /* Passive filter is disabled */
                                            kPORT_PassiveFilterDisable,
                                            /* Open drain is disabled */
                                            kPORT_OpenDrainDisable,
                                            /* Low drive strength is configured */
                                            kPORT_LowDriveStrength,
                                            /* Pin is configured as QSPI0B_DATA3 */
                                            kPORT_MuxAlt5,
                                            /* Pin Control Register fields [15:0] are not locked */
                                            kPORT_UnlockRegister};
    /* PORTE6 (pin E1) is configured as QSPI0B_DATA3 */
    PORT_SetPinConfig(BOARD_QSPI0B_DATA3_PORT, BOARD_QSPI0B_DATA3_PIN, &QSPI0B_DATA3);

    const port_pin_config_t QSPI0B_SCLK = {/* Internal pull-up/down resistor is disabled */
                                           kPORT_PullDisable,
                                           /* Fast slew rate is configured */
                                           kPORT_FastSlewRate,
                                           /* Passive filter is disabled */
                                           kPORT_PassiveFilterDisable,
                                           /* Open drain is disabled */
                                           kPORT_OpenDrainDisable,
                                           /* Low drive strength is configured */
                                           kPORT_LowDriveStrength,
                                           /* Pin is configured as QSPI0B_SCLK */
                                           kPORT_MuxAlt5,
                                           /* Pin Control Register fields [15:0] are not locked */
                                           kPORT_UnlockRegister};
    /* PORTE7 (pin F3) is configured as QSPI0B_SCLK */
    PORT_SetPinConfig(BOARD_QSPI0B_SCLK_PORT, BOARD_QSPI0B_SCLK_PIN, &QSPI0B_SCLK);

    const port_pin_config_t QSPI0B_DATA0 = {/* Internal pull-up/down resistor is disabled */
                                            kPORT_PullDisable,
                                            /* Fast slew rate is configured */
                                            kPORT_FastSlewRate,
                                            /* Passive filter is disabled */
                                            kPORT_PassiveFilterDisable,
                                            /* Open drain is disabled */
                                            kPORT_OpenDrainDisable,
                                            /* Low drive strength is configured */
                                            kPORT_LowDriveStrength,
                                            /* Pin is configured as QSPI0B_DATA0 */
                                            kPORT_MuxAlt5,
                                            /* Pin Control Register fields [15:0] are not locked */
                                            kPORT_UnlockRegister};
    /* PORTE8 (pin F2) is configured as QSPI0B_DATA0 */
    PORT_SetPinConfig(BOARD_QSPI0B_DATA0_PORT, BOARD_QSPI0B_DATA0_PIN, &QSPI0B_DATA0);

    const port_pin_config_t QSPI0B_DATA2 = {/* Internal pull-up/down resistor is disabled */
                                            kPORT_PullDisable,
                                            /* Fast slew rate is configured */
                                            kPORT_FastSlewRate,
                                            /* Passive filter is disabled */
                                            kPORT_PassiveFilterDisable,
                                            /* Open drain is disabled */
                                            kPORT_OpenDrainDisable,
                                            /* Low drive strength is configured */
                                            kPORT_LowDriveStrength,
                                            /* Pin is configured as QSPI0B_DATA2 */
                                            kPORT_MuxAlt5,
                                            /* Pin Control Register fields [15:0] are not locked */
                                            kPORT_UnlockRegister};
    /* PORTE9 (pin F1) is configured as QSPI0B_DATA2 */
    PORT_SetPinConfig(BOARD_QSPI0B_DATA2_PORT, BOARD_QSPI0B_DATA2_PIN, &QSPI0B_DATA2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitSDRAMPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: A3, peripheral: SDRAM, signal: A10, pin_signal: PTD4/LLWU_P14/SPI0_PCS1/LPUART0_RTS_b/FTM0_CH4/FB_AD2/SDRAM_A10/EWM_IN/SPI1_PCS0, slew_rate: fast, open_drain: disable,
    drive_strength: low, pull_select: down, pull_enable: disable, digital_filter: disable}
  - {pin_num: B3, peripheral: SDRAM, signal: A11, pin_signal: PTD3/SPI0_SIN/LPUART2_TX/FTM3_CH3/FB_AD3/SDRAM_A11/I2C0_SDA, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable, digital_filter: disable}
  - {pin_num: C3, peripheral: SDRAM, signal: A12, pin_signal: PTD2/LLWU_P13/SPI0_SOUT/LPUART2_RX/FTM3_CH2/FB_AD4/SDRAM_A12/I2C0_SCL, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable, digital_filter: disable}
  - {pin_num: C6, peripheral: SDRAM, signal: A13, pin_signal: PTC10/I2C1_SCL/FTM3_CH6/I2S0_RX_FS/FB_AD5/SDRAM_A13/FXIO0_D18, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: D6, peripheral: SDRAM, signal: A14, pin_signal: CMP0_IN3/PTC9/FTM3_CH5/I2S0_RX_BCLK/FB_AD6/SDRAM_A14/FTM2_FLT0/FXIO0_D17, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: A7, peripheral: SDRAM, signal: A15, pin_signal: CMP0_IN2/PTC8/FTM3_CH4/I2S0_MCLK/FB_AD7/SDRAM_A15/FXIO0_D16, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: B7, peripheral: SDRAM, signal: A16, pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB0_SOF_OUT/I2S0_RX_FS/FB_AD8/SDRAM_A16/FXIO0_D15, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: C7, peripheral: SDRAM, signal: A17, pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/SDRAM_A17/I2S0_MCLK/FXIO0_D14, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: D7, peripheral: SDRAM, signal: A18, pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/LPTMR1_ALT2/I2S0_RXD0/FB_AD10/SDRAM_A18/CMP0_OUT/FTM0_CH2, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: A8, peripheral: SDRAM, signal: A19, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT, identifier: SDRAM_A19, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: C8, peripheral: SDRAM, signal: A20, pin_signal: ADC0_SE4b/CMP1_IN0/TSI0_CH15/PTC2/SPI0_PCS2/LPUART1_CTS_b/FTM0_CH1/FB_AD12/SDRAM_A20/I2S0_TX_FS, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: D8, peripheral: SDRAM, signal: A21, pin_signal: ADC0_SE15/TSI0_CH14/PTC1/LLWU_P6/SPI0_PCS3/LPUART1_RTS_b/FTM0_CH0/FB_AD13/SDRAM_A21/I2S0_TXD0/FXIO0_D13,
    slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: B9, peripheral: SDRAM, signal: A22, pin_signal: ADC0_SE14/TSI0_CH13/PTC0/SPI0_PCS4/PDB0_EXTRG/USB0_SOF_OUT/FB_AD14/SDRAM_A22/I2S0_TXD1/FXIO0_D12, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: D9, peripheral: SDRAM, signal: A23, pin_signal: TSI0_CH11/PTB18/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/SDRAM_A23/FTM2_QD_PHA/TPM2_CH0/FXIO0_D6, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: A2, peripheral: SDRAM, signal: A9, pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/LPUART0_CTS_b/FTM0_CH5/FB_AD1/SDRAM_A9/EWM_OUT_b/SPI1_SCK, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, digital_filter: disable}
  - {pin_num: G11, peripheral: SDRAM, signal: CAS, pin_signal: ADC0_SE8/TSI0_CH0/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/SDRAM_CAS_b/FTM1_QD_PHA/TPM1_CH0/FXIO0_D0, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: A1, peripheral: SDRAM, signal: CKE, pin_signal: PTD7/CMT_IRO/LPUART0_TX/FTM0_CH7/SDRAM_CKE/FTM0_FLT1/SPI1_SIN, identifier: SDRAM_CKE, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable, digital_filter: disable}
  - {pin_num: B8, peripheral: SDRAM, signal: CLKOUT, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK, identifier: SDRAM_CLK, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: G8, peripheral: SDRAM, signal: CS0, pin_signal: ADC0_SE13/TSI0_CH8/PTB3/I2C0_SDA/LPUART0_CTS_b/SDRAM_CS0_b/FTM0_FLT0/FXIO0_D3, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: E9, peripheral: SDRAM, signal: D16, pin_signal: TSI0_CH10/PTB17/SPI1_SIN/LPUART0_TX/FTM_CLKIN1/FB_AD16/SDRAM_D16/EWM_OUT_b/TPM_CLKIN1, identifier: SDRAM_D16,
    slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: B10, peripheral: SDRAM, signal: D17, pin_signal: TSI0_CH9/PTB16/SPI1_SOUT/LPUART0_RX/FTM_CLKIN0/FB_AD17/SDRAM_D17/EWM_IN/TPM_CLKIN0, identifier: SDRAM_D17,
    slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: C10, peripheral: SDRAM, signal: D18, pin_signal: PTB11/SPI1_SCK/LPUART3_TX/I2C2_SDA/FB_AD18/SDRAM_D18/FTM0_FLT2/FXIO0_D5, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: D10, peripheral: SDRAM, signal: D19, pin_signal: PTB10/SPI1_PCS0/LPUART3_RX/I2C2_SCL/FB_AD19/SDRAM_D19/FTM0_FLT1/FXIO0_D4, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: E10, peripheral: SDRAM, signal: D20, pin_signal: PTB9/SPI1_PCS1/LPUART3_CTS_b/FB_AD20/SDRAM_D20, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: D11, peripheral: SDRAM, signal: D21, pin_signal: PTB8/EMVSIM1_RST/LPUART3_RTS_b/FB_AD21/SDRAM_D21, identifier: SDRAM_D21, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: E11, peripheral: SDRAM, signal: D22, pin_signal: PTB7/EMVSIM1_PD/FB_AD22/SDRAM_D22, identifier: SDRAM_D22, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: F11, peripheral: SDRAM, signal: D23, pin_signal: PTB6/EMVSIM1_VCCEN/FB_AD23/SDRAM_D23, identifier: SDRAM_D23, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: B5, peripheral: SDRAM, signal: D24, pin_signal: PTC15/LPUART4_TX/FB_AD24/SDRAM_D24/FXIO0_D21, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: A5, peripheral: SDRAM, signal: D25, pin_signal: PTC14/LPUART4_RX/FB_AD25/SDRAM_D25/FXIO0_D20, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: A6, peripheral: SDRAM, signal: D26, pin_signal: PTC13/LPUART4_CTS_b/FTM_CLKIN1/FB_AD26/SDRAM_D26/TPM_CLKIN1, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: B6, peripheral: SDRAM, signal: D27, pin_signal: PTC12/LPUART4_RTS_b/FTM_CLKIN0/FB_AD27/SDRAM_D27/FTM3_FLT0/TPM_CLKIN0, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: E8, peripheral: SDRAM, signal: D28, pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28/SDRAM_D28/FXIO0_D11, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: F8, peripheral: SDRAM, signal: D29, pin_signal: PTB22/SPI2_SOUT/FB_AD29/SDRAM_D29/FXIO0_D10, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: F9, peripheral: SDRAM, signal: D30, pin_signal: PTB21/SPI2_SCK/FB_AD30/SDRAM_D30/CMP1_OUT/FXIO0_D9, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: F10, peripheral: SDRAM, signal: D31, pin_signal: PTB20/SPI2_PCS0/FB_AD31/SDRAM_D31/CMP0_OUT/FXIO0_D8, slew_rate: fast, open_drain: disable, pull_select: down,
    pull_enable: disable}
  - {pin_num: D5, peripheral: SDRAM, signal: DQM2, pin_signal: PTC16/LPUART3_RX/FB_CS5_b/FB_TSIZ1/FB_BE23_16_BLS15_8_b/SDRAM_DQM2, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: C4, peripheral: SDRAM, signal: DQM3, pin_signal: PTC17/LPUART3_TX/FB_CS4_b/FB_TSIZ0/FB_BE31_24_BLS7_0_b/SDRAM_DQM3, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: G10, peripheral: SDRAM, signal: RAS, pin_signal: ADC0_SE9/TSI0_CH6/PTB1/I2C0_SDA/FTM1_CH1/SDRAM_RAS_b/FTM1_QD_PHB/TPM1_CH1/FXIO0_D1, slew_rate: fast,
    open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: G9, peripheral: SDRAM, signal: WE, pin_signal: ADC0_SE12/TSI0_CH7/PTB2/I2C0_SCL/LPUART0_RTS_b/SDRAM_WE/FTM0_FLT3/FXIO0_D2, slew_rate: fast, open_drain: disable,
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
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

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
    /* PORTB0 (pin G11) is configured as SDRAM_CAS_b */
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
    /* PORTB1 (pin G10) is configured as SDRAM_RAS_b */
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
    /* PORTB10 (pin D10) is configured as SDRAM_D19 */
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
    /* PORTB11 (pin C10) is configured as SDRAM_D18 */
    PORT_SetPinConfig(BOARD_SDRAM_D18_PORT, BOARD_SDRAM_D18_PIN, &SDRAM_D18);

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
    /* PORTB16 (pin B10) is configured as SDRAM_D17 */
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
    /* PORTB17 (pin E9) is configured as SDRAM_D16 */
    PORT_SetPinConfig(BOARD_SDRAM_D16_PORT, BOARD_SDRAM_D16_PIN, &SDRAM_D16);

    const port_pin_config_t SDRAM_A23 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_A23 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTB18 (pin D9) is configured as SDRAM_A23 */
    PORT_SetPinConfig(BOARD_SDRAM_A23_PORT, BOARD_SDRAM_A23_PIN, &SDRAM_A23);

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
    /* PORTB2 (pin G9) is configured as SDRAM_WE */
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
    /* PORTB20 (pin F10) is configured as SDRAM_D31 */
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
    /* PORTB21 (pin F9) is configured as SDRAM_D30 */
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
    /* PORTB22 (pin F8) is configured as SDRAM_D29 */
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
    /* PORTB23 (pin E8) is configured as SDRAM_D28 */
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
    /* PORTB3 (pin G8) is configured as SDRAM_CS0_b */
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
    /* PORTB6 (pin F11) is configured as SDRAM_D23 */
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
    /* PORTB7 (pin E11) is configured as SDRAM_D22 */
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
    /* PORTB8 (pin D11) is configured as SDRAM_D21 */
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
    /* PORTB9 (pin E10) is configured as SDRAM_D20 */
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
    /* PORTC0 (pin B9) is configured as SDRAM_A22 */
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
    /* PORTC1 (pin D8) is configured as SDRAM_A21 */
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
    /* PORTC10 (pin C6) is configured as SDRAM_A13 */
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
    /* PORTC12 (pin B6) is configured as SDRAM_D27 */
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
    /* PORTC13 (pin A6) is configured as SDRAM_D26 */
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
    /* PORTC14 (pin A5) is configured as SDRAM_D25 */
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
    /* PORTC15 (pin B5) is configured as SDRAM_D24 */
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
    /* PORTC16 (pin D5) is configured as SDRAM_DQM2 */
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
    /* PORTC17 (pin C4) is configured as SDRAM_DQM3 */
    PORT_SetPinConfig(BOARD_SDRAM_DQM3_PORT, BOARD_SDRAM_DQM3_PIN, &SDRAM_DQM3);

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
    /* PORTC2 (pin C8) is configured as SDRAM_A20 */
    PORT_SetPinConfig(BOARD_SDRAM_A20_PORT, BOARD_SDRAM_A20_PIN, &SDRAM_A20);

    const port_pin_config_t SDRAM_CLK = {/* Internal pull-up/down resistor is disabled */
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
    /* PORTC3 (pin B8) is configured as CLKOUT */
    PORT_SetPinConfig(BOARD_SDRAM_CLK_PORT, BOARD_SDRAM_CLK_PIN, &SDRAM_CLK);

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
    /* PORTC4 (pin A8) is configured as SDRAM_A19 */
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
    /* PORTC5 (pin D7) is configured as SDRAM_A18 */
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
    /* PORTC6 (pin C7) is configured as SDRAM_A17 */
    PORT_SetPinConfig(BOARD_SDRAM_A17_PORT, BOARD_SDRAM_A17_PIN, &SDRAM_A17);

    const port_pin_config_t SDRAM_A16 = {/* Internal pull-up/down resistor is disabled */
                                         kPORT_PullDisable,
                                         /* Fast slew rate is configured */
                                         kPORT_FastSlewRate,
                                         /* Passive filter is disabled */
                                         kPORT_PassiveFilterDisable,
                                         /* Open drain is disabled */
                                         kPORT_OpenDrainDisable,
                                         /* Low drive strength is configured */
                                         kPORT_LowDriveStrength,
                                         /* Pin is configured as SDRAM_A16 */
                                         kPORT_MuxAlt5,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTC7 (pin B7) is configured as SDRAM_A16 */
    PORT_SetPinConfig(BOARD_SDRAM_A16_PORT, BOARD_SDRAM_A16_PIN, &SDRAM_A16);

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
    /* PORTC8 (pin A7) is configured as SDRAM_A15 */
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
    /* PORTC9 (pin D6) is configured as SDRAM_A14 */
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
    /* PORTD2 (pin C3) is configured as SDRAM_A12 */
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
    /* PORTD3 (pin B3) is configured as SDRAM_A11 */
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
    /* PORTD4 (pin A3) is configured as SDRAM_A10 */
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
    /* PORTD5 (pin A2) is configured as SDRAM_A9 */
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
    /* PORTD7 (pin A1) is configured as SDRAM_CKE */
    PORT_SetPinConfig(BOARD_SDRAM_CKE_PORT, BOARD_SDRAM_CKE_PIN, &SDRAM_CKE);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitEMVSIMPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: C11, peripheral: EMVSIM1, signal: CLK, pin_signal: PTB5/EMVSIM1_CLK/FTM2_FLT0, slew_rate: fast, open_drain: disable, pull_select: up, pull_enable: enable}
  - {pin_num: B11, peripheral: EMVSIM1, signal: IO, pin_signal: PTB4/EMVSIM1_IO/SDRAM_CS1_b/FTM1_FLT0, slew_rate: fast, open_drain: enable, pull_select: down, pull_enable: disable}
  - {pin_num: E11, peripheral: EMVSIM1, signal: PD, pin_signal: PTB7/EMVSIM1_PD/FB_AD22/SDRAM_D22, identifier: SMARTCARD_PD, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: D11, peripheral: EMVSIM1, signal: RST, pin_signal: PTB8/EMVSIM1_RST/LPUART3_RTS_b/FB_AD21/SDRAM_D21, identifier: SMARTCARD_RST, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
  - {pin_num: F11, peripheral: EMVSIM1, signal: VCCEN, pin_signal: PTB6/EMVSIM1_VCCEN/FB_AD23/SDRAM_D23, identifier: SMARTCARD_VCCEN, slew_rate: fast, open_drain: disable,
    pull_select: down, pull_enable: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitEMVSIMPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitEMVSIMPins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    const port_pin_config_t SMARTCARD_IO = {/* Internal pull-up/down resistor is disabled */
                                            kPORT_PullDisable,
                                            /* Fast slew rate is configured */
                                            kPORT_FastSlewRate,
                                            /* Passive filter is disabled */
                                            kPORT_PassiveFilterDisable,
                                            /* Open drain is enabled */
                                            kPORT_OpenDrainEnable,
                                            /* Low drive strength is configured */
                                            kPORT_LowDriveStrength,
                                            /* Pin is configured as EMVSIM1_IO */
                                            kPORT_MuxAlt2,
                                            /* Pin Control Register fields [15:0] are not locked */
                                            kPORT_UnlockRegister};
    /* PORTB4 (pin B11) is configured as EMVSIM1_IO */
    PORT_SetPinConfig(BOARD_SMARTCARD_IO_PORT, BOARD_SMARTCARD_IO_PIN, &SMARTCARD_IO);

    const port_pin_config_t SMARTCARD_CLK = {/* Internal pull-up resistor is enabled */
                                             kPORT_PullUp,
                                             /* Fast slew rate is configured */
                                             kPORT_FastSlewRate,
                                             /* Passive filter is disabled */
                                             kPORT_PassiveFilterDisable,
                                             /* Open drain is disabled */
                                             kPORT_OpenDrainDisable,
                                             /* Low drive strength is configured */
                                             kPORT_LowDriveStrength,
                                             /* Pin is configured as EMVSIM1_CLK */
                                             kPORT_MuxAlt2,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    /* PORTB5 (pin C11) is configured as EMVSIM1_CLK */
    PORT_SetPinConfig(BOARD_SMARTCARD_CLK_PORT, BOARD_SMARTCARD_CLK_PIN, &SMARTCARD_CLK);

    const port_pin_config_t SMARTCARD_VCCEN = {/* Internal pull-up/down resistor is disabled */
                                               kPORT_PullDisable,
                                               /* Fast slew rate is configured */
                                               kPORT_FastSlewRate,
                                               /* Passive filter is disabled */
                                               kPORT_PassiveFilterDisable,
                                               /* Open drain is disabled */
                                               kPORT_OpenDrainDisable,
                                               /* Low drive strength is configured */
                                               kPORT_LowDriveStrength,
                                               /* Pin is configured as EMVSIM1_VCCEN */
                                               kPORT_MuxAlt2,
                                               /* Pin Control Register fields [15:0] are not locked */
                                               kPORT_UnlockRegister};
    /* PORTB6 (pin F11) is configured as EMVSIM1_VCCEN */
    PORT_SetPinConfig(BOARD_SMARTCARD_VCCEN_PORT, BOARD_SMARTCARD_VCCEN_PIN, &SMARTCARD_VCCEN);

    const port_pin_config_t SMARTCARD_PD = {/* Internal pull-up/down resistor is disabled */
                                            kPORT_PullDisable,
                                            /* Fast slew rate is configured */
                                            kPORT_FastSlewRate,
                                            /* Passive filter is disabled */
                                            kPORT_PassiveFilterDisable,
                                            /* Open drain is disabled */
                                            kPORT_OpenDrainDisable,
                                            /* Low drive strength is configured */
                                            kPORT_LowDriveStrength,
                                            /* Pin is configured as EMVSIM1_PD */
                                            kPORT_MuxAlt2,
                                            /* Pin Control Register fields [15:0] are not locked */
                                            kPORT_UnlockRegister};
    /* PORTB7 (pin E11) is configured as EMVSIM1_PD */
    PORT_SetPinConfig(BOARD_SMARTCARD_PD_PORT, BOARD_SMARTCARD_PD_PIN, &SMARTCARD_PD);

    const port_pin_config_t SMARTCARD_RST = {/* Internal pull-up/down resistor is disabled */
                                             kPORT_PullDisable,
                                             /* Fast slew rate is configured */
                                             kPORT_FastSlewRate,
                                             /* Passive filter is disabled */
                                             kPORT_PassiveFilterDisable,
                                             /* Open drain is disabled */
                                             kPORT_OpenDrainDisable,
                                             /* Low drive strength is configured */
                                             kPORT_LowDriveStrength,
                                             /* Pin is configured as EMVSIM1_RST */
                                             kPORT_MuxAlt2,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    /* PORTB8 (pin D11) is configured as EMVSIM1_RST */
    PORT_SetPinConfig(BOARD_SMARTCARD_RST_PORT, BOARD_SMARTCARD_RST_PIN, &SMARTCARD_RST);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitTOUCH_PADsPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: E9, peripheral: TSI0, signal: 'CH, 10', pin_signal: TSI0_CH10/PTB17/SPI1_SIN/LPUART0_TX/FTM_CLKIN1/FB_AD16/SDRAM_D16/EWM_OUT_b/TPM_CLKIN1, identifier: TSI_ELECTRODE_2,
    slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: B10, peripheral: TSI0, signal: 'CH, 9', pin_signal: TSI0_CH9/PTB16/SPI1_SOUT/LPUART0_RX/FTM_CLKIN0/FB_AD17/SDRAM_D17/EWM_IN/TPM_CLKIN0, identifier: TSI_ELECTRODE_1,
    slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: G4, peripheral: GPIOD, signal: 'GPIO, 14', pin_signal: PTD14/SPI2_SIN/FB_A22/FXIO0_D30, direction: OUTPUT, gpio_init_state: 'true', slew_rate: slow,
    open_drain: disable, pull_select: down, pull_enable: disable, digital_filter: disable}
  - {pin_num: H4, peripheral: GPIOD, signal: 'GPIO, 15', pin_signal: PTD15/SPI2_PCS1/FB_A23/FXIO0_D31, direction: OUTPUT, gpio_init_state: 'true', slew_rate: slow,
    open_drain: disable, pull_select: down, pull_enable: disable, digital_filter: disable}
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
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    gpio_pin_config_t LED_TSI_PAD2_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTD14 (pin G4)  */
    GPIO_PinInit(BOARD_LED_TSI_PAD2_GPIO, BOARD_LED_TSI_PAD2_PIN, &LED_TSI_PAD2_config);

    gpio_pin_config_t LED_TSI_PAD1_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTD15 (pin H4)  */
    GPIO_PinInit(BOARD_LED_TSI_PAD1_GPIO, BOARD_LED_TSI_PAD1_PIN, &LED_TSI_PAD1_config);

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
                                               /* Pin is configured as TSI0_CH9 */
                                               kPORT_PinDisabledOrAnalog,
                                               /* Pin Control Register fields [15:0] are not locked */
                                               kPORT_UnlockRegister};
    /* PORTB16 (pin B10) is configured as TSI0_CH9 */
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
                                               /* Pin is configured as TSI0_CH10 */
                                               kPORT_PinDisabledOrAnalog,
                                               /* Pin Control Register fields [15:0] are not locked */
                                               kPORT_UnlockRegister};
    /* PORTB17 (pin E9) is configured as TSI0_CH10 */
    PORT_SetPinConfig(BOARD_TSI_ELECTRODE_2_PORT, BOARD_TSI_ELECTRODE_2_PIN, &TSI_ELECTRODE_2);
    /* Configure digital filter */
    PORT_EnablePinsDigitalFilter(
        /* Digital filter is configured on port D */
        PORTD,
        /* Digital filter is configured for PORTD0 */
          PORT_DFER_DFE_14_MASK
            /* Digital filter is configured for PORTD1 */
            | PORT_DFER_DFE_15_MASK,
        /* Disable digital filter */
        false);

    const port_pin_config_t LED_TSI_PAD2 = {/* Internal pull-up/down resistor is disabled */
                                            kPORT_PullDisable,
                                            /* Slow slew rate is configured */
                                            kPORT_SlowSlewRate,
                                            /* Passive filter is disabled */
                                            kPORT_PassiveFilterDisable,
                                            /* Open drain is disabled */
                                            kPORT_OpenDrainDisable,
                                            /* Low drive strength is configured */
                                            kPORT_LowDriveStrength,
                                            /* Pin is configured as PTD14 */
                                            kPORT_MuxAsGpio,
                                            /* Pin Control Register fields [15:0] are not locked */
                                            kPORT_UnlockRegister};
    /* PORTD14 (pin G4) is configured as PTD14 */
    PORT_SetPinConfig(BOARD_LED_TSI_PAD2_PORT, BOARD_LED_TSI_PAD2_PIN, &LED_TSI_PAD2);

    const port_pin_config_t LED_TSI_PAD1 = {/* Internal pull-up/down resistor is disabled */
                                            kPORT_PullDisable,
                                            /* Slow slew rate is configured */
                                            kPORT_SlowSlewRate,
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
    /* PORTD15 (pin H4) is configured as PTD15 */
    PORT_SetPinConfig(BOARD_LED_TSI_PAD1_PORT, BOARD_LED_TSI_PAD1_PIN, &LED_TSI_PAD1);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPOTPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: K3, peripheral: ADC0, signal: 'SE, 20', pin_signal: ADC0_DM3}
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
BOARD_InitACCELPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: A9, peripheral: I2C0, signal: SDA, pin_signal: PTD9/I2C0_SDA/FB_A17/FXIO0_D25, identifier: ACCEL_SDA, slew_rate: fast, open_drain: enable, pull_select: down,
    pull_enable: disable, digital_filter: disable}
  - {pin_num: A10, peripheral: I2C0, signal: SCL, pin_signal: PTD8/LLWU_P24/I2C0_SCL/FB_A16/FXIO0_D24, identifier: ACCEL_SCL, slew_rate: fast, open_drain: enable,
    pull_select: down, pull_enable: disable, digital_filter: disable}
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
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Configure digital filter */
    PORT_EnablePinsDigitalFilter(
        /* Digital filter is configured on port D */
        PORTD,
        /* Digital filter is configured for PORTD0 */
          PORT_DFER_DFE_8_MASK
            /* Digital filter is configured for PORTD1 */
            | PORT_DFER_DFE_9_MASK,
        /* Disable digital filter */
        false);

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
                                         /* Pin is configured as I2C0_SCL */
                                         kPORT_MuxAlt2,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTD8 (pin A10) is configured as I2C0_SCL */
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
                                         /* Pin is configured as I2C0_SDA */
                                         kPORT_MuxAlt2,
                                         /* Pin Control Register fields [15:0] are not locked */
                                         kPORT_UnlockRegister};
    /* PORTD9 (pin A9) is configured as I2C0_SDA */
    PORT_SetPinConfig(BOARD_ACCEL_SDA_PORT, BOARD_ACCEL_SDA_PIN, &ACCEL_SDA);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitGYROPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: A10, peripheral: I2C0, signal: SCL, pin_signal: PTD8/LLWU_P24/I2C0_SCL/FB_A16/FXIO0_D24, identifier: GYRO_SCL, slew_rate: fast, open_drain: enable,
    pull_select: down, pull_enable: disable, digital_filter: disable}
  - {pin_num: A9, peripheral: I2C0, signal: SDA, pin_signal: PTD9/I2C0_SDA/FB_A17/FXIO0_D25, identifier: GYRO_SDA, slew_rate: fast, open_drain: enable, pull_select: down,
    pull_enable: disable, digital_filter: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitGYROPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitGYROPins(void)
{
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Configure digital filter */
    PORT_EnablePinsDigitalFilter(
        /* Digital filter is configured on port D */
        PORTD,
        /* Digital filter is configured for PORTD0 */
          PORT_DFER_DFE_8_MASK
            /* Digital filter is configured for PORTD1 */
            | PORT_DFER_DFE_9_MASK,
        /* Disable digital filter */
        false);

    const port_pin_config_t GYRO_SCL = {/* Internal pull-up/down resistor is disabled */
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
                                        kPORT_MuxAlt2,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTD8 (pin A10) is configured as I2C0_SCL */
    PORT_SetPinConfig(BOARD_GYRO_SCL_PORT, BOARD_GYRO_SCL_PIN, &GYRO_SCL);

    const port_pin_config_t GYRO_SDA = {/* Internal pull-up/down resistor is disabled */
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
                                        kPORT_MuxAlt2,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTD9 (pin A9) is configured as I2C0_SDA */
    PORT_SetPinConfig(BOARD_GYRO_SDA_PORT, BOARD_GYRO_SDA_PIN, &GYRO_SDA);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPRESSUREPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: A10, peripheral: I2C0, signal: SCL, pin_signal: PTD8/LLWU_P24/I2C0_SCL/FB_A16/FXIO0_D24, identifier: PRESSURE_SCL, slew_rate: fast, open_drain: enable,
    pull_select: down, pull_enable: disable, digital_filter: disable}
  - {pin_num: A9, peripheral: I2C0, signal: SDA, pin_signal: PTD9/I2C0_SDA/FB_A17/FXIO0_D25, identifier: PRESSURE_SDA, slew_rate: fast, open_drain: enable, pull_select: down,
    pull_enable: disable, digital_filter: disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPRESSUREPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPRESSUREPins(void)
{
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Configure digital filter */
    PORT_EnablePinsDigitalFilter(
        /* Digital filter is configured on port D */
        PORTD,
        /* Digital filter is configured for PORTD0 */
          PORT_DFER_DFE_8_MASK
            /* Digital filter is configured for PORTD1 */
            | PORT_DFER_DFE_9_MASK,
        /* Disable digital filter */
        false);

    const port_pin_config_t PRESSURE_SCL = {/* Internal pull-up/down resistor is disabled */
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
                                            kPORT_MuxAlt2,
                                            /* Pin Control Register fields [15:0] are not locked */
                                            kPORT_UnlockRegister};
    /* PORTD8 (pin A10) is configured as I2C0_SCL */
    PORT_SetPinConfig(BOARD_PRESSURE_SCL_PORT, BOARD_PRESSURE_SCL_PIN, &PRESSURE_SCL);

    const port_pin_config_t PRESSURE_SDA = {/* Internal pull-up/down resistor is disabled */
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
                                            kPORT_MuxAlt2,
                                            /* Pin Control Register fields [15:0] are not locked */
                                            kPORT_UnlockRegister};
    /* PORTD9 (pin A9) is configured as I2C0_SDA */
    PORT_SetPinConfig(BOARD_PRESSURE_SDA_PORT, BOARD_PRESSURE_SDA_PIN, &PRESSURE_SDA);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitUSBPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: H1, peripheral: USB0, signal: DM, pin_signal: USB0_DM}
  - {pin_num: H2, peripheral: USB0, signal: DP, pin_signal: USB0_DP}
  - {pin_num: J2, peripheral: USB0, signal: VREGIN, pin_signal: VREGIN}
  - {pin_num: J1, peripheral: USB0, signal: VOUT33, pin_signal: VOUT33}
  - {pin_num: A4, peripheral: GPIOC, signal: 'GPIO, 19', pin_signal: PTC19/LPUART3_CTS_b/FB_CS3_b/FB_BE7_0_BLS31_24_b/SDRAM_DQM0/FB_TA_b, direction: OUTPUT, gpio_init_state: 'false',
    slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: B4, peripheral: GPIOC, signal: 'GPIO, 18', pin_signal: PTC18/LPUART3_RTS_b/FB_TBST_b/FB_CS2_b/FB_BE15_8_BLS23_16_b/SDRAM_DQM1, direction: INPUT, slew_rate: fast,
    open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: H11, peripheral: GPIOA, signal: 'GPIO, 29', pin_signal: PTA29/FB_A24, direction: INPUT, slew_rate: fast, open_drain: disable, pull_select: down, pull_enable: disable}
  - {pin_num: A1, peripheral: GPIOD, signal: 'GPIO, 7', pin_signal: PTD7/CMT_IRO/LPUART0_TX/FTM0_CH7/SDRAM_CKE/FTM0_FLT1/SPI1_SIN, identifier: USB_ID, direction: INPUT,
    slew_rate: fast, open_drain: disable, drive_strength: low, pull_select: up, pull_enable: enable, digital_filter: disable}
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
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    gpio_pin_config_t USB_PWR_STAT_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTA29 (pin H11)  */
    GPIO_PinInit(BOARD_USB_PWR_STAT_GPIO, BOARD_USB_PWR_STAT_PIN, &USB_PWR_STAT_config);

    gpio_pin_config_t USB_FLAG_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC18 (pin B4)  */
    GPIO_PinInit(BOARD_USB_FLAG_GPIO, BOARD_USB_FLAG_PIN, &USB_FLAG_config);

    gpio_pin_config_t USB_PWR_EN_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC19 (pin A4)  */
    GPIO_PinInit(BOARD_USB_PWR_EN_GPIO, BOARD_USB_PWR_EN_PIN, &USB_PWR_EN_config);

    gpio_pin_config_t USB_ID_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTD7 (pin A1)  */
    GPIO_PinInit(BOARD_USB_ID_GPIO, BOARD_USB_ID_PIN, &USB_ID_config);

    const port_pin_config_t USB_PWR_STAT = {/* Internal pull-up/down resistor is disabled */
                                            kPORT_PullDisable,
                                            /* Fast slew rate is configured */
                                            kPORT_FastSlewRate,
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
    PORT_SetPinConfig(BOARD_USB_PWR_STAT_PORT, BOARD_USB_PWR_STAT_PIN, &USB_PWR_STAT);

    const port_pin_config_t USB_FLAG = {/* Internal pull-up/down resistor is disabled */
                                        kPORT_PullDisable,
                                        /* Fast slew rate is configured */
                                        kPORT_FastSlewRate,
                                        /* Passive filter is disabled */
                                        kPORT_PassiveFilterDisable,
                                        /* Open drain is disabled */
                                        kPORT_OpenDrainDisable,
                                        /* Low drive strength is configured */
                                        kPORT_LowDriveStrength,
                                        /* Pin is configured as PTC18 */
                                        kPORT_MuxAsGpio,
                                        /* Pin Control Register fields [15:0] are not locked */
                                        kPORT_UnlockRegister};
    /* PORTC18 (pin B4) is configured as PTC18 */
    PORT_SetPinConfig(BOARD_USB_FLAG_PORT, BOARD_USB_FLAG_PIN, &USB_FLAG);

    const port_pin_config_t USB_PWR_EN = {/* Internal pull-up/down resistor is disabled */
                                          kPORT_PullDisable,
                                          /* Fast slew rate is configured */
                                          kPORT_FastSlewRate,
                                          /* Passive filter is disabled */
                                          kPORT_PassiveFilterDisable,
                                          /* Open drain is disabled */
                                          kPORT_OpenDrainDisable,
                                          /* Low drive strength is configured */
                                          kPORT_LowDriveStrength,
                                          /* Pin is configured as PTC19 */
                                          kPORT_MuxAsGpio,
                                          /* Pin Control Register fields [15:0] are not locked */
                                          kPORT_UnlockRegister};
    /* PORTC19 (pin A4) is configured as PTC19 */
    PORT_SetPinConfig(BOARD_USB_PWR_EN_PORT, BOARD_USB_PWR_EN_PIN, &USB_PWR_EN);
    /* Configure digital filter */
    PORT_EnablePinsDigitalFilter(
        /* Digital filter is configured on port D */
        PORTD,
        /* Digital filter is configured for PORTD0 */
        PORT_DFER_DFE_7_MASK,
        /* Disable digital filter */
        false);

    const port_pin_config_t USB_ID = {/* Internal pull-up resistor is enabled */
                                      kPORT_PullUp,
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
    PORT_SetPinConfig(BOARD_USB_ID_PORT, BOARD_USB_ID_PIN, &USB_ID);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitDEBUG_UARTPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: B8, peripheral: LPUART1, signal: RX, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/LPUART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK, identifier: DEBUG_UART_RX,
    slew_rate: fast, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
  - {pin_num: A8, peripheral: LPUART1, signal: TX, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/LPUART1_TX/FTM0_CH3/FB_AD11/SDRAM_A19/CMP1_OUT, identifier: DEBUG_UART_TX, direction: OUTPUT,
    slew_rate: fast, open_drain: disable, drive_strength: low, pull_select: down, pull_enable: disable}
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
                                             /* Pin is configured as LPUART1_RX */
                                             kPORT_MuxAlt3,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    /* PORTC3 (pin B8) is configured as LPUART1_RX */
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
                                             /* Pin is configured as LPUART1_TX */
                                             kPORT_MuxAlt3,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    /* PORTC4 (pin A8) is configured as LPUART1_TX */
    PORT_SetPinConfig(BOARD_DEBUG_UART_TX_PORT, BOARD_DEBUG_UART_TX_PIN, &DEBUG_UART_TX);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_LPUART1TXSRC_MASK | SIM_SOPT5_LPUART1RXSRC_MASK)))

                  /* LPUART1 transmit data source select: LPUART1_TX pin. */
                  | SIM_SOPT5_LPUART1TXSRC(SOPT5_LPUART1TXSRC_LPUART_TX)

                  /* LPUART1 receive data source select: LPUART1_RX pin. */
                  | SIM_SOPT5_LPUART1RXSRC(SOPT5_LPUART1RXSRC_LPUART_RX));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitOSCsPins:
- options: {prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: L4, peripheral: RTC, signal: XTAL32, pin_signal: XTAL32}
  - {pin_num: L5, peripheral: RTC, signal: EXTAL32, pin_signal: EXTAL32}
  - {pin_num: K11, peripheral: OSC, signal: XTAL0, pin_signal: XTAL0/PTA19/FTM1_FLT0/FTM_CLKIN1/LPTMR0_ALT1/LPTMR1_ALT1/TPM_CLKIN1, slew_rate: no_init, open_drain: no_init,
    pull_select: no_init, pull_enable: no_init}
  - {pin_num: L11, peripheral: OSC, signal: EXTAL0, pin_signal: EXTAL0/PTA18/FTM0_FLT2/FTM_CLKIN0/TPM_CLKIN0, slew_rate: no_init, open_drain: no_init, pull_select: no_init,
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

    /* PORTA18 (pin L11) is configured as EXTAL0 */
    PORT_SetPinMux(BOARD_EXTAL0_PORT, BOARD_EXTAL0_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTA19 (pin K11) is configured as XTAL0 */
    PORT_SetPinMux(BOARD_XTAL0_PORT, BOARD_XTAL0_PIN, kPORT_PinDisabledOrAnalog);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
