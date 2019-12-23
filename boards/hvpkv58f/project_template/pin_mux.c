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
processor: MKV58F1M0xxx24
package_id: MKV58F1M0VLQ24
mcu_data: ksdk2_0
processor_version: 0.0.16
board: HVP-KV58F
pin_labels:
- {pin_num: '1', pin_signal: HSADC0B_CH16/HSADC1A_CH0/PTE0/SPI1_PCS1/UART1_TX/XB_OUT10/XB_IN11/I2C1_SDA/TRACE_CLKOUT, label: 'MB_J9[5]/MB_J11[B16]/MB_TP19/MB_ENC_Index/J4[B16]/TM2',
  identifier: ENC_INDEX}
- {pin_num: '2', pin_signal: HSADC0B_CH17/HSADC1A_CH1/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/XB_OUT11/XB_IN7/I2C1_SCL/TRACE_D3, label: 'MB_J11[A25]/MB_U12[11]/MB_TP28/MB_FAULT/MB_FAULT_1/MB_PFC_overcurrent/J4[A25]/FAULT_1',
  identifier: FAULT_1}
- {pin_num: '3', pin_signal: HSADC0B_CH10/HSADC1B_CH0/PTE2/LLWU_P1/SPI1_SCK/UART1_CTS_b/TRACE_D2, label: TP24}
- {pin_num: '4', pin_signal: HSADC0B_CH11/HSADC1B_CH1/PTE3/SPI1_SIN/UART1_RTS_b/TRACE_D1, label: 'MB_J11[B6]/MB_TP44/MB_BEMF_sense_A/MB_Phase_A/J4[B6]/BEMF_A', identifier: BEMF_A}
- {pin_num: '7', pin_signal: HSADC1A_CH4/ADC0_SE2/ADC0_DP2/PTE4/LLWU_P2/SPI1_PCS0/UART3_TX/TRACE_D0, label: 'J9[1]/M2_I_phA/CON1 - M2_I_phA', identifier: M2_I_PH_A}
- {pin_num: '8', pin_signal: HSADC1A_CH5/ADC0_SE10/ADC0_DM2/PTE5/SPI1_PCS2/UART3_RX/FLEXPWM1_A0/FTM3_CH0, label: 'J9[15]/M2_PWM0', identifier: M2_PWM0}
- {pin_num: '9', pin_signal: HSADC1B_CH7/ADC0_SE4a/PTE6/LLWU_P16/SPI1_PCS3/UART3_CTS_b/FLEXPWM1_B0/FTM3_CH1, label: 'J9[17]/M2_PWM1', identifier: M2_PWM1}
- {pin_num: '10', pin_signal: PTE7/UART3_RTS_b/FLEXPWM1_A1/FTM3_CH2, label: 'J9[19]/M2_PWM2', identifier: M2_PWM2}
- {pin_num: '11', pin_signal: PTE8/UART5_TX/FLEXPWM1_B1/FTM3_CH3, label: 'J9[16]/M2_PWM3', identifier: M2_PWM3}
- {pin_num: '12', pin_signal: PTE9/LLWU_P17/UART5_RX/FLEXPWM1_A2/FTM3_CH4, label: 'J9[18]/M2_PWM4', identifier: M2_PWM4}
- {pin_num: '13', pin_signal: PTE10/LLWU_P18/UART5_CTS_b/FLEXPWM1_B2/FTM3_CH5, label: 'J9[20]/M2_PWM5', identifier: M2_PWM5}
- {pin_num: '14', pin_signal: HSADC1A_CH6/ADC0_SE3/ADC0_DP3/PTE11/UART5_RTS_b/FLEXPWM1_A3/FTM3_CH6, label: NC}
- {pin_num: '15', pin_signal: HSADC1B_CH6/ADC0_SE11/ADC0_DM3/PTE12/FLEXPWM1_B3/FTM3_CH7, label: NC}
- {pin_num: '40', pin_signal: PTE13, label: NC}
- {pin_num: '18', pin_signal: HSADC0A_CH0/ADC0_SE1/ADC0_DP1/PTE16/SPI0_PCS0/UART2_TX/FTM_CLKIN0/FTM0_FLT3, label: NC}
- {pin_num: '19', pin_signal: HSADC0A_CH1/ADC0_SE9/ADC0_DM1/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3, label: 'MB_J11[B4]/MB_TP31/MB_V_sense_DCB/MB_DCB_Pos/J4[B4]/U_dcb',
  identifier: U_DCB}
- {pin_num: '20', pin_signal: HSADC0B_CH0/ADC0_SE5a/PTE18/LLWU_P20/SPI0_SOUT/UART2_CTS_b/I2C0_SDA, label: 'MB_J11[B5]/MB_TP33/''MB_V_sense_DCB/2''/J4[B5]/''U_dcb/2''',
  identifier: U_DCB_HALF}
- {pin_num: '21', pin_signal: HSADC0B_CH1/ADC0_SE6a/PTE19/SPI0_SIN/UART2_RTS_b/I2C0_SCL/CMP3_OUT, label: 'MB_J11[B9]/MB_U12[2]/MB_TP38/MB_IPM_temp/J4[B9]/Temp_IPM',
  identifier: TEMP_IPM}
- {pin_num: '25', pin_signal: HSADC0A_CH8/ADC0_SE5b/PTE20/FTM1_CH0/UART0_TX/FTM1_QD_PHA, label: 'MB_J1[1]/MB_J2[3]/MB_J11[A31]/MB_TxD_1/MB_TxD_EXT/J4[A31]/TxD1',
  identifier: TXD_1}
- {pin_num: '26', pin_signal: HSADC0A_CH9/HSADC1A_CH7/PTE21/XB_IN9/FTM1_CH1/UART0_RX/FTM1_QD_PHB, label: 'MB_J2[4]/MB_J3[4]/MB_J11[A32]/MB_RxD_1/MB_RxD_EXT/J4[A32]/RxD1',
  identifier: RXD_1}
- {pin_num: '41', pin_signal: PTE22/FTM2_CH0/XB_IN2/FTM2_QD_PHA, label: 'J9[13]/M2_FAULT1', identifier: M2_FAULT1}
- {pin_num: '42', pin_signal: PTE23/FTM2_CH1/XB_IN3/FTM2_QD_PHB, label: NC}
- {pin_num: '45', pin_signal: HSADC0B_CH4/HSADC1B_CH4/PTE24/CAN1_TX/FTM0_CH0/XB_IN2/I2C0_SCL/EWM_OUT_b/XB_OUT4/UART4_TX, label: 'MB_J11[B7]/MB_TP43/MB_BEMF_sense_B/MB_Phase_B/U1A[64]/J4[B7]/BEMF_B',
  identifier: BEMF_B}
- {pin_num: '46', pin_signal: HSADC0B_CH5/HSADC1B_CH5/PTE25/LLWU_P21/CAN1_RX/FTM0_CH1/XB_IN3/I2C0_SDA/EWM_IN/XB_OUT5/UART4_RX, label: 'MB_J11[B8]/MB_TP41/MB_BEMF_sense_C/MB_Phase_C/U1A[66]/J4[B8]/BEMF_C',
  identifier: BEMF_C}
- {pin_num: '47', pin_signal: PTE26/ENET_1588_CLKIN/FTM0_CH4/UART4_CTS_b, label: NC}
- {pin_num: '48', pin_signal: PTE27/CAN2_TX/UART4_RTS_b, label: NC}
- {pin_num: '49', pin_signal: PTE28/CAN2_RX, label: NC}
- {pin_num: '37', pin_signal: HSADC0A_CH4/CMP1_IN5/CMP0_IN5/PTE29/FTM0_CH2/FTM_CLKIN0, label: 'MB_J11[B3]/MB_U8B[7]/MB_TP34/MB_I_sense_DCB/J4[B3]/I_dcb', identifier: I_DCB}
- {pin_num: '38', pin_signal: DAC0_OUT/CMP1_IN3/HSADC0A_CH5/PTE30/FTM0_CH3/FTM_CLKIN1, label: 'MB_J11[A6]/MB_TP16/MB_TACHO/J4[A6]/TACHO', identifier: TACHO}
- {pin_num: '35', pin_signal: ADC0_SE0/ADC0_DP0/CMP2_IN5, label: NC}
- {pin_num: '36', pin_signal: ADC0_SE8/ADC0_DM0/CMP1_IN2, label: NC}
- {pin_num: '27', pin_signal: HSADC0A_CH2/HSADC1A_CH2, label: 'MB_J11[A3]/MB_U11A[1]/MB_TP45/MB_I_sense_A/J4[A3]/I_phA', identifier: I_PH_A}
- {pin_num: '28', pin_signal: HSADC0A_CH3/HSADC1A_CH3, label: 'MB_J11[A4]/MB_U11B[7]/MB_TP40/MB_I_sense_B/J4[A4]/I_phB', identifier: I_PH_B}
- {pin_num: '29', pin_signal: HSADC0A_CH10/HSADC1B_CH2, label: 'MB_J11[A5]/MB_U8A[1]/MB_TP37/MB_I_sense_C/J4[A5]/I_phC', identifier: I_PH_C}
- {pin_num: '23', pin_signal: HSADC0A_CH6/ADC0_SE7a, label: NC}
- {pin_num: '39', pin_signal: HSADC0A_CH12/CMP0_IN4/CMP2_IN3, label: 'MB_J11[B6]/MB_TP44/MB_BEMF_sense_A/MB_Phase_A/J4[B6]/BEMF_A', identifier: BEMF_A_CMP_IN}
- {pin_num: '24', pin_signal: HSADC0A_CH7/ADC0_SE4b, label: 'J9[7]/M2_U_dcb/CON7 - M2_U_dcb', identifier: M2_U_DCB}
- {pin_num: '30', pin_signal: HSADC0A_CH11/HSADC1B_CH3, label: 'MB_J11[A7]/MB_TP18/MB_Vin/J4[A7]/V_in', identifier: V_IN}
- {pin_num: '50', pin_signal: PTA0/UART0_CTS_b/UART0_COL_b/FTM0_CH5/XB_IN4/EWM_IN/JTAG_TCLK/SWD_CLK, label: 'U4[11]/U5[13]/J3[4]/SWCLK/SWDCLK_ISOLATED', identifier: SWCLK}
- {pin_num: '51', pin_signal: PTA1/UART0_RX/FTM0_CH6/CMP0_OUT/FTM2_QD_PHA/FTM1_CH1/JTAG_TDI, label: 'U1A[99]/cpm0_out', identifier: BEMF_A_CMP_OUT}
- {pin_num: '52', pin_signal: PTA2/UART0_TX/FTM0_CH7/CMP1_OUT/FTM2_QD_PHB/FTM1_CH0/JTAG_TDO/TRACE_SWO, label: 'U5[12]/J3[6]/SWO/SWO_ISOLATED', identifier: SWO}
- {pin_num: '53', pin_signal: PTA3/UART0_RTS_b/FTM0_CH0/XB_IN9/EWM_OUT_b/FLEXPWM0_A0/JTAG_TMS/SWD_DIO, label: 'U4[3]/U5[14]/J3[2]/SWDIO/SWDIO_ISOLATED', identifier: SWDIO}
- {pin_num: '54', pin_signal: PTA4/LLWU_P3/FTM0_CH1/XB_IN10/FTM0_FLT3/FLEXPWM0_B0/NMI_b, label: nmi}
- {pin_num: '55', pin_signal: PTA5/FTM0_CH2/RMII0_RXER/MII0_RXER/CMP2_OUT/JTAG_TRST_b, label: 'U1A[100]/cmp2_out', identifier: BEMF_B_CMP_OUT}
- {pin_num: '58', pin_signal: PTA6/FTM0_CH3/CLKOUT/TRACE_CLKOUT, label: NC}
- {pin_num: '59', pin_signal: HSADC1B_CH8/PTA7/FTM0_CH4/RMII0_MDIO/MII0_MDIO/TRACE_D3, label: NC}
- {pin_num: '60', pin_signal: HSADC1B_CH9/PTA8/FTM1_CH0/RMII0_MDC/MII0_MDC/TRACE_D2, label: NC}
- {pin_num: '61', pin_signal: PTA9/FTM1_CH1/MII0_RXD3/TRACE_D1, label: NC}
- {pin_num: '62', pin_signal: PTA10/LLWU_P22/FTM2_CH0/MII0_RXD2/FTM2_QD_PHA/TRACE_D0, label: NC}
- {pin_num: '63', pin_signal: PTA11/LLWU_P23/FTM2_CH1/MII0_RXCLK/FTM2_QD_PHB/I2C0_SDA, label: NC}
- {pin_num: '64', pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/FTM1_QD_PHA/I2C0_SCL, label: 'MB_J11[B7]/MB_TP43/MB_BEMF_sense_B/MB_Phase_B/U1B[45]/J4[B7]/BEMF_B',
  identifier: BEMF_B_CMP_IN}
- {pin_num: '65', pin_signal: CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/RMII0_RXD0/MII0_RXD0/FTM1_QD_PHB/I2C1_SDA, label: 'MB_J11[A26]/MB_U9[2]/MB_TP35/MB_FAULT_2/MB_Over-voltage_FAULT/J4[A26]/FAULT_2',
  identifier: FAULT_2}
- {pin_num: '66', pin_signal: CMP3_IN0/PTA14/SPI0_PCS0/UART0_TX/CAN2_TX/RMII0_CRS_DV/MII0_RXDV/I2C1_SCL, label: 'MB_J11[B8]/MB_TP41/MB_BEMF_sense_C/MB_Phase_C/U1B[46]/J4[B8]/BEMF_C',
  identifier: BEMF_C_CMP_IN}
- {pin_num: '67', pin_signal: CMP3_IN1/PTA15/SPI0_SCK/UART0_RX/CAN2_RX/RMII0_TXEN/MII0_TXEN, label: 'J4[B23]/TP20/SCLK'}
- {pin_num: '68', pin_signal: CMP3_IN2/PTA16/SPI0_SOUT/UART0_CTS_b/UART0_COL_b/RMII0_TXD0/MII0_TXD0, label: TP19/MISO}
- {pin_num: '69', pin_signal: HSADC0A_CH15/PTA17/SPI0_SIN/UART0_RTS_b/RMII0_TXD1/MII0_TXD1, label: TP21/MOSI}
- {pin_num: '72', pin_signal: EXTAL0/PTA18/XB_IN7/FTM0_FLT2/FTM_CLKIN0/XB_OUT8/FTM3_CH2, label: 'Y2[1]/GPIOA18/EXTAL_J', identifier: EXTAL0}
- {pin_num: '73', pin_signal: XTAL0/PTA19/XB_IN8/FTM1_FLT0/FTM_CLKIN1/XB_OUT9/LPTMR0_ALT1, label: 'Y2[3]/GPIOA19/XTAL_J', identifier: XTAL0}
- {pin_num: '75', pin_signal: PTA24/XB_IN4/MII0_TXD2/FB_A29, label: NC}
- {pin_num: '76', pin_signal: PTA25/XB_IN5/MII0_TXCLK/FB_A28, label: NC}
- {pin_num: '77', pin_signal: PTA26/MII0_TXD3/FB_A27, label: NC}
- {pin_num: '78', pin_signal: PTA27/MII0_CRS/FB_A26, label: NC}
- {pin_num: '79', pin_signal: PTA28/MII0_TXER/FB_A25, label: NC}
- {pin_num: '80', pin_signal: PTA29/MII0_COL/FB_A24, label: NC}
- {pin_num: '74', pin_signal: RESET_b, label: 'U2[21]/U5[11]/J3[10]/SW1/''/RESET''/RESET_ISOLATED', identifier: RESET}
- {pin_num: '81', pin_signal: HSADC0B_CH2/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/FTM1_QD_PHA/UART0_RX/RMII0_MDIO/MII0_MDIO, label: 'J9[2]/M2_BEMF_A/CON2 - M2_BEMF_A', identifier: M2_BEMF_A}
- {pin_num: '82', pin_signal: HSADC0B_CH3/PTB1/I2C0_SDA/FTM1_CH1/FTM0_FLT2/EWM_IN/FTM1_QD_PHB/UART0_TX/RMII0_MDC/MII0_MDC, label: 'J9[4]/M2_BEMF_B/CON4 - M2_BEMF_B',
  identifier: M2_BEMF_B}
- {pin_num: '83', pin_signal: HSADC0A_CH14/CMP2_IN2/PTB2/I2C0_SCL/UART0_RTS_b/FTM0_FLT1/ENET0_1588_TMR0/FTM0_FLT3, label: 'J9[8]/M2_I_dcb/CON8 - M2_I_dcb', identifier: M2_I_DCB}
- {pin_num: '84', pin_signal: HSADC0B_CH15/CMP3_IN5/PTB3/I2C0_SDA/UART0_CTS_b/UART0_COL_b/ENET0_1588_TMR1/FTM0_FLT0, label: 'J9[6]/M2_BEMF_C/CON6 - M2_BEMF_C', identifier: M2_BEMF_C}
- {pin_num: '85', pin_signal: ADC0_SE6b/PTB4/FLEXPWM1_X0/ENET0_1588_TMR2/FTM1_FLT0, label: USER_LED_2, identifier: LED_RED}
- {pin_num: '86', pin_signal: ADC0_SE7b/PTB5/FLEXPWM1_X1/ENET0_1588_TMR3/FTM2_FLT0, label: NC, identifier: NC}
- {pin_num: '87', pin_signal: HSADC1A_CH12/PTB6/CAN2_TX/FLEXPWM1_X2/FB_AD23, label: 'J9[3]/CON3 - M2_I_phB', identifier: M2_I_PH_B}
- {pin_num: '88', pin_signal: HSADC1A_CH13/PTB7/CAN2_RX/FLEXPWM1_X3/FB_AD22, label: 'J9[5]/CON5 - M2_I_phC', identifier: M2_I_PH_C}
- {pin_num: '89', pin_signal: PTB8/UART3_RTS_b/FB_AD21, label: NC}
- {pin_num: '90', pin_signal: PTB9/SPI1_PCS1/UART3_CTS_b/ENET0_1588_TMR2/FB_AD20, label: NC}
- {pin_num: '91', pin_signal: HSADC0B_CH6/PTB10/SPI1_PCS0/UART3_RX/ENET0_1588_TMR3/FTM0_FLT1/FB_AD19, label: 'MB_J11[A30]/MB_TP22/J4[A30]'}
- {pin_num: '92', pin_signal: HSADC0B_CH7/PTB11/SPI1_SCK/UART3_TX/FTM0_FLT2/FB_AD18, label: 'MB_J11[A29]/MB_TP24/J4[A29]'}
- {pin_num: '95', pin_signal: PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN2/CAN0_TX/EWM_IN/XB_IN5/FB_AD17, label: 'MB_J9[3]/MB_J11[B14]/MB_TP13/MB_ENC_PhaseA/J4[B14]/TM0',
  identifier: ENC_PHASE_A}
- {pin_num: '96', pin_signal: PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/CAN0_RX/EWM_OUT_b/FB_AD16, label: NC}
- {pin_num: '97', pin_signal: PTB18/CAN0_TX/FTM2_CH0/FTM3_CH2/FLEXPWM1_A1/FTM2_QD_PHA/FB_AD15, label: NC}
- {pin_num: '98', pin_signal: PTB19/CAN0_RX/FTM2_CH1/FTM3_CH3/FLEXPWM1_B1/FTM2_QD_PHB/FB_OE_b, label: 'MB_J11[B20]/MB_U9[4]/MB_USER_LED/J4[B20]/USER_LED', identifier: LED_GREEN}
- {pin_num: '99', pin_signal: PTB20/SPI2_PCS0/FLEXPWM0_X0/CMP0_OUT/FB_AD31, label: 'U1B[51]/cmp0_out', identifier: BEMF_A_CAPTURE}
- {pin_num: '100', pin_signal: PTB21/SPI2_SCK/FLEXPWM0_X1/CMP1_OUT/FB_AD30, label: 'U1B[55]/cmp2_out', identifier: BEMF_B_CAPTURE}
- {pin_num: '103', pin_signal: HSADC0B_CH8/PTC0/SPI0_PCS4/PDB0_EXTRG/FTM0_FLT1/SPI0_PCS0/FB_AD14, label: 'J4[B24]/TP18/''/SS'''}
- {pin_num: '104', pin_signal: HSADC0B_CH9/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FLEXPWM0_A3/XB_IN11/FB_AD13, label: 'MB_J11[A22]/MB_U14[4]/MB_TP15/MB_PWM_PFC2/J4[A22]/PWM8',
  identifier: PWM_PFC2}
- {pin_num: '105', pin_signal: HSADC1B_CH10/CMP1_IN0/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FLEXPWM0_B3/XB_IN6/FB_AD12, label: 'MB_J11[A23]/MB_U14[2]/MB_TP12/MB_PWM_PFC1/J4[A23]/PWM9',
  identifier: PWM_PFC1}
- {pin_num: '106', pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/FTM3_FLT0, label: 'U2[25]/U8[12]/RxD_sda/UART_RX_TGTMCU', identifier: UART_RX_TGTMCU}
- {pin_num: '114', pin_signal: HSADC1B_CH12/CMP0_IN3/PTC9/FTM3_CH5/FLEXPWM1_B2/FB_AD6, label: 'MB_J11[A9]/MB_U2A[1]/MB_TP5/MB_Ipfc2/J4[A9]/I_pfc2', identifier: I_PFC2}
- {pin_num: '113', pin_signal: HSADC1B_CH11/CMP0_IN2/PTC8/FTM3_CH4/FLEXPWM1_A2/FB_AD7, label: 'MB_J11[A8]/MB_U2B[7]/MB_TP14/MB_Ipfc1/J4[A8]/I_pfc1', identifier: I_PFC1}
- {pin_num: '115', pin_signal: HSADC1B_CH13/PTC10/I2C1_SCL/FTM3_CH6/FLEXPWM1_A3/FB_AD5, label: NC}
- {pin_num: '116', pin_signal: HSADC1B_CH14/PTC11/LLWU_P11/I2C1_SDA/FTM3_CH7/FLEXPWM1_B3/FB_RW_b, label: NC}
- {pin_num: '117', pin_signal: PTC12/CAN2_TX/FTM_CLKIN0/FLEXPWM1_A1/FTM3_FLT0/SPI2_PCS1/FB_AD27/UART4_RTS_b, label: NC}
- {pin_num: '112', pin_signal: CMP3_IN4/CMP0_IN1/PTC7/SPI0_SIN/XB_IN4/UART0_TX/XB_OUT7/I2C0_SDA/FB_AD8, label: 'MB_J11[B22]/MB_PFC_zc_1/J4[B22]/MB_TP_26', identifier: PFC_ZC_1}
- {pin_num: '111', pin_signal: CMP2_IN4/CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/XB_IN3/UART0_RX/XB_OUT6/I2C0_SCL/FB_AD9, label: 'MB_J11[B21]/MB_PFC_zc_2/J4[B21]/MB_TP_27',
  identifier: PFC_ZC_2}
- {pin_num: '110', pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/XB_IN2/CMP0_OUT/FTM0_CH2/FB_AD10, label: 'MB_J9[4]/MB_J11[B15]/MB_TP17/MB_ENC_PhaseB/J4[B15]/TM1',
  identifier: ENC_PHASE_B}
- {pin_num: '109', pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/CMP1_OUT/FB_AD11, label: 'U2[24]/U8[13]/TxD_sda/UART_TX_TGTMCU', identifier: UART_TX_TGTMCU}
- {pin_num: '118', pin_signal: PTC13/CAN2_RX/FTM_CLKIN1/FLEXPWM1_B1/FB_AD26/UART4_CTS_b, label: 'MB_J11[B28]/MB_Q3[1]/MB_Relay/J4[B28]/Relay', identifier: RELAY}
- {pin_num: '119', pin_signal: PTC14/I2C1_SCL/I2C0_SCL/FLEXPWM1_A0/FB_AD25/UART4_RX, label: 'MB_J11[A28]/J4[A28]/TP23/I2C_SCL'}
- {pin_num: '120', pin_signal: PTC15/I2C1_SDA/I2C0_SDA/FLEXPWM1_B0/FB_AD24/UART4_TX, label: 'MB_J11[A27]/J4[A27]/TP22/I2C_SDA'}
- {pin_num: '123', pin_signal: PTC16/CAN1_RX/UART3_RX/ENET0_1588_TMR0/FLEXPWM1_A2/FB_CS5_b/FB_TSIZ1/FB_BE23_16_b, label: 'MB_J11[B25]/MB_U9[2]/MB_TP_35/MB_MCU_BRAKE/J4[B25]/MCU_BRAKE',
  identifier: MCU_BRAKE}
- {pin_num: '124', pin_signal: PTC17/CAN1_TX/UART3_TX/ENET0_1588_TMR1/FLEXPWM1_B2/FB_CS4_b/FB_TSIZ0/FB_BE31_24_b, label: NC}
- {pin_num: '125', pin_signal: PTC18/UART3_RTS_b/ENET0_1588_TMR2/FLEXPWM1_A3/FB_TBST_b/FB_CS2_b/FB_BE15_8_b, label: NC}
- {pin_num: '126', pin_signal: PTC19/UART3_CTS_b/ENET0_1588_TMR3/FLEXPWM1_B3/FB_CS3_b/FB_BE7_0_b/FB_TA_b, label: NC}
- {pin_num: '127', pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FTM0_CH0/FLEXPWM0_A0/FB_ALE/FB_CS1_b/FB_TS_b/FLEXPWM1_A0, label: 'MB_J11[A14]/MB_U12[18]/MB_PWM_AT/J4[A14]/PWM0',
  identifier: PWM_AT}
- {pin_num: '128', pin_signal: HSADC1A_CH11/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FTM0_CH1/FLEXPWM0_B0/FB_CS0_b/FLEXPWM1_B0, label: 'MB_J11[A15]/MB_U12[12]/MB_PWM_AB/J4[A15]/PWM1',
  identifier: PWM_AB}
- {pin_num: '129', pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FTM0_CH2/FLEXPWM0_A1/I2C0_SCL/FB_AD4/FLEXPWM1_A1, label: 'MB_J11[A16]/MB_U12[19]/MB_PWM_BT/J4[A16]/PWM2',
  identifier: PWM_BT}
- {pin_num: '130', pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FTM0_CH3/FLEXPWM0_B1/I2C0_SDA/FB_AD3/FLEXPWM1_B1, label: 'MB_J11[A17]/MB_U12[13]/MB_PWM_BB/J4[A17]/PWM3',
  identifier: PWM_BB}
- {pin_num: '131', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FLEXPWM0_A2/EWM_IN/SPI1_PCS0/FB_AD2, label: 'MB_J11[A18]/MB_U12[20]/MB_PWM_CT/J4[A18]/PWM4',
  identifier: PWM_CT}
- {pin_num: '132', pin_signal: HSADC1A_CH8/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FLEXPWM0_B2/EWM_OUT_b/SPI1_SCK/FB_AD1, label: 'MB_J11[A19]/MB_U12[14]/MB_PWM_CB/J4[A19]/PWM5',
  identifier: PWM_CB}
- {pin_num: '133', pin_signal: HSADC1A_CH9/PTD6/LLWU_P15/SPI0_PCS3/UART0_RX/FTM0_CH6/FTM1_CH0/FTM0_FLT0/SPI1_SOUT/FB_AD0, label: 'MB_J3[2]/MB_J11[B27]/MB_U1[12]/MB_U3[26]/MB_TP2/MB_TP10/MB_RxD/J4[B27]/RxD',
  identifier: RXD}
- {pin_num: '136', pin_signal: PTD7/UART0_TX/FTM0_CH7/FTM1_CH1/FTM0_FLT1/SPI1_SIN, label: 'MB_J1[3]/MB_J11[B26]/MB_U1[13]/MB_U3[25]/MB_TP3/MB_TP7/MB_TxD/J4[B26]/TxD',
  identifier: TXD}
- {pin_num: '137', pin_signal: PTD8/LLWU_P24/I2C1_SCL/UART5_RX/FLEXPWM0_A3/FB_A16, label: NC}
- {pin_num: '138', pin_signal: PTD9/I2C1_SDA/UART5_TX/FLEXPWM0_B3/FB_A17, label: NC}
- {pin_num: '139', pin_signal: PTD10/UART5_RTS_b/FLEXPWM0_A2/FB_A18, label: NC}
- {pin_num: '140', pin_signal: PTD11/LLWU_P25/SPI2_PCS0/UART5_CTS_b/FLEXPWM0_B2/FB_A19, label: NC}
- {pin_num: '141', pin_signal: PTD12/SPI2_SCK/FTM3_FLT0/XB_IN5/XB_OUT5/FLEXPWM0_A1/FB_A20, label: 'MB_J11[B17]/J4[B17]/MB_TP_29'}
- {pin_num: '143', pin_signal: PTD14/SPI2_SIN/XB_IN11/XB_OUT11/FLEXPWM0_A0/FB_A22, label: 'J9[14]/M2_GPIO', identifier: M2_GPIO}
- {pin_num: '144', pin_signal: PTD15/SPI2_PCS1/FLEXPWM0_B0/FB_A23, label: NC}
- {pin_num: '142', pin_signal: PTD13/SPI2_SOUT/XB_IN7/XB_OUT7/FLEXPWM0_B1/FB_A21, label: NC}
- {pin_num: '5', pin_signal: VDD5, label: 3.3V}
- {pin_num: '16', pin_signal: VDD16, label: 3.3V}
- {pin_num: '43', pin_signal: VDD43, label: 3.3V}
- {pin_num: '56', pin_signal: VDD56, label: 3.3V}
- {pin_num: '70', pin_signal: VDD70, label: 3.3V}
- {pin_num: '94', pin_signal: VDD94, label: 3.3V}
- {pin_num: '108', pin_signal: VDD108, label: 3.3V}
- {pin_num: '122', pin_signal: VDD122, label: 3.3V}
- {pin_num: '135', pin_signal: VDD135, label: 3.3V}
- {pin_num: '31', pin_signal: VDDA, label: 3.3VA}
- {pin_num: '32', pin_signal: VREFH, label: 3.3VA}
- {pin_num: '33', pin_signal: VREFL, label: GNDA}
- {pin_num: '34', pin_signal: VSSA, label: GNDA}
- {pin_num: '6', pin_signal: VSS6, label: GND}
- {pin_num: '17', pin_signal: VSS17, label: GND}
- {pin_num: '22', pin_signal: VSS22, label: GND}
- {pin_num: '44', pin_signal: VSS44, label: GND}
- {pin_num: '57', pin_signal: VSS57, label: GND}
- {pin_num: '71', pin_signal: VSS71, label: GND}
- {pin_num: '93', pin_signal: VSS93, label: GND}
- {pin_num: '134', pin_signal: VSS134, label: GND}
- {pin_num: '121', pin_signal: VSS121, label: GND}
- {pin_num: '107', pin_signal: VSS107, label: GND}
- {pin_num: '101', pin_signal: PTB22/SPI2_SOUT/FLEXPWM0_X2/CMP2_OUT/FB_AD29, label: 'U1A[102]/cmp3_out', identifier: BEMF_C_CAPTURE}
- {pin_num: '102', pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FLEXPWM0_X3/CMP3_OUT/FB_AD28, label: 'U1A[101]/cmp3_out', identifier: BEMF_C_CMP_OUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_xbara.h"
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
MB_InitMC_PWMPins:
- options: {callFromInitBoot: 'false', prefix: MB_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '127', peripheral: PWM0, signal: 'A, 0', pin_signal: PTD0/LLWU_P12/SPI0_PCS0/UART2_RTS_b/FTM3_CH0/FTM0_CH0/FLEXPWM0_A0/FB_ALE/FB_CS1_b/FB_TS_b/FLEXPWM1_A0,
    direction: OUTPUT}
  - {pin_num: '128', peripheral: PWM0, signal: 'B, 0', pin_signal: HSADC1A_CH11/PTD1/SPI0_SCK/UART2_CTS_b/FTM3_CH1/FTM0_CH1/FLEXPWM0_B0/FB_CS0_b/FLEXPWM1_B0, direction: OUTPUT}
  - {pin_num: '129', peripheral: PWM0, signal: 'A, 1', pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FTM0_CH2/FLEXPWM0_A1/I2C0_SCL/FB_AD4/FLEXPWM1_A1, direction: OUTPUT}
  - {pin_num: '130', peripheral: PWM0, signal: 'B, 1', pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FTM0_CH3/FLEXPWM0_B1/I2C0_SDA/FB_AD3/FLEXPWM1_B1, direction: OUTPUT}
  - {pin_num: '131', peripheral: PWM0, signal: 'A, 2', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FLEXPWM0_A2/EWM_IN/SPI1_PCS0/FB_AD2, direction: OUTPUT}
  - {pin_num: '132', peripheral: PWM0, signal: 'B, 2', pin_signal: HSADC1A_CH8/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FLEXPWM0_B2/EWM_OUT_b/SPI1_SCK/FB_AD1,
    direction: OUTPUT}
  - {pin_num: '2', peripheral: PWM0, signal: 'FAULT, 0', pin_signal: HSADC0B_CH17/HSADC1A_CH1/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/XB_OUT11/XB_IN7/I2C1_SCL/TRACE_D3}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : MB_InitMC_PWMPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void MB_InitMC_PWMPins(void)
{
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);
    /* XBARA Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_XbarA);

    /* PORTD0 (pin 127) is configured as FLEXPWM0_A0 */
    PORT_SetPinMux(MB_PWM_AT_PORT, MB_PWM_AT_PIN, kPORT_MuxAlt6);

    /* PORTD1 (pin 128) is configured as FLEXPWM0_B0 */
    PORT_SetPinMux(MB_PWM_AB_PORT, MB_PWM_AB_PIN, kPORT_MuxAlt6);

    /* PORTD2 (pin 129) is configured as FLEXPWM0_A1 */
    PORT_SetPinMux(MB_PWM_BT_PORT, MB_PWM_BT_PIN, kPORT_MuxAlt6);

    /* PORTD3 (pin 130) is configured as FLEXPWM0_B1 */
    PORT_SetPinMux(MB_PWM_BB_PORT, MB_PWM_BB_PIN, kPORT_MuxAlt6);

    /* PORTD4 (pin 131) is configured as FLEXPWM0_A2 */
    PORT_SetPinMux(MB_PWM_CT_PORT, MB_PWM_CT_PIN, kPORT_MuxAlt5);

    /* PORTD5 (pin 132) is configured as FLEXPWM0_B2 */
    PORT_SetPinMux(MB_PWM_CB_PORT, MB_PWM_CB_PIN, kPORT_MuxAlt5);

    /* PORTE1 (pin 2) is configured as XB_IN7 */
    PORT_SetPinMux(MB_FAULT_1_PORT, MB_FAULT_1_PIN, kPORT_MuxAlt5);
    /* XB_IN7 input pin output assigned to XBARA_IN7 input is connected
     * to XBARA_OUT29 output assigned to PWM0 and PWM1 fault 0 */
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn7, kXBARA_OutputPwm0Fault0);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
MB_InitPFCPins:
- options: {callFromInitBoot: 'false', prefix: MB_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '111', peripheral: CMP0, signal: 'IN, 0', pin_signal: CMP2_IN4/CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/XB_IN3/UART0_RX/XB_OUT6/I2C0_SCL/FB_AD9}
  - {pin_num: '112', peripheral: CMP0, signal: 'IN, 1', pin_signal: CMP3_IN4/CMP0_IN1/PTC7/SPI0_SIN/XB_IN4/UART0_TX/XB_OUT7/I2C0_SDA/FB_AD8}
  - {pin_num: '118', peripheral: GPIOC, signal: 'GPIO, 13', pin_signal: PTC13/CAN2_RX/FTM_CLKIN1/FLEXPWM1_B1/FB_AD26/UART4_CTS_b, direction: OUTPUT, gpio_init_state: 'false'}
  - {pin_num: '104', peripheral: PWM0, signal: 'A, 3', pin_signal: HSADC0B_CH9/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FLEXPWM0_A3/XB_IN11/FB_AD13, direction: OUTPUT}
  - {pin_num: '105', peripheral: PWM0, signal: 'B, 3', pin_signal: HSADC1B_CH10/CMP1_IN0/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FLEXPWM0_B3/XB_IN6/FB_AD12, direction: OUTPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : MB_InitPFCPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void MB_InitPFCPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    gpio_pin_config_t RELAY_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC13 (pin 118)  */
    GPIO_PinInit(MB_RELAY_GPIO, MB_RELAY_PIN, &RELAY_config);

    /* PORTC1 (pin 104) is configured as FLEXPWM0_A3 */
    PORT_SetPinMux(MB_PWM_PFC2_PORT, MB_PWM_PFC2_PIN, kPORT_MuxAlt5);

    /* PORTC13 (pin 118) is configured as PTC13 */
    PORT_SetPinMux(MB_RELAY_PORT, MB_RELAY_PIN, kPORT_MuxAsGpio);

    /* PORTC2 (pin 105) is configured as FLEXPWM0_B3 */
    PORT_SetPinMux(MB_PWM_PFC1_PORT, MB_PWM_PFC1_PIN, kPORT_MuxAlt5);

    /* PORTC6 (pin 111) is configured as CMP0_IN0 */
    PORT_SetPinMux(MB_PFC_ZC_2_PORT, MB_PFC_ZC_2_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTC7 (pin 112) is configured as CMP0_IN1 */
    PORT_SetPinMux(MB_PFC_ZC_1_PORT, MB_PFC_ZC_1_PIN, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
MB_InitANA_SENSPins:
- options: {callFromInitBoot: 'false', prefix: MB_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '37', peripheral: HSADC0, signal: 'ADCA, CH4', pin_signal: HSADC0A_CH4/CMP1_IN5/CMP0_IN5/PTE29/FTM0_CH2/FTM_CLKIN0}
  - {pin_num: '19', peripheral: HSADC0, signal: 'ADCA, CH1', pin_signal: HSADC0A_CH1/ADC0_SE9/ADC0_DM1/PTE17/LLWU_P19/SPI0_SCK/UART2_RX/FTM_CLKIN1/LPTMR0_ALT3}
  - {pin_num: '20', peripheral: HSADC0, signal: 'ADCB, CH0', pin_signal: HSADC0B_CH0/ADC0_SE5a/PTE18/LLWU_P20/SPI0_SOUT/UART2_CTS_b/I2C0_SDA}
  - {pin_num: '21', peripheral: HSADC0, signal: 'ADCB, CH1', pin_signal: HSADC0B_CH1/ADC0_SE6a/PTE19/SPI0_SIN/UART2_RTS_b/I2C0_SCL/CMP3_OUT}
  - {pin_num: '27', peripheral: HSADC0, signal: 'ADCA, CH2', pin_signal: HSADC0A_CH2/HSADC1A_CH2}
  - {pin_num: '28', peripheral: HSADC0, signal: 'ADCA, CH3', pin_signal: HSADC0A_CH3/HSADC1A_CH3}
  - {pin_num: '29', peripheral: HSADC0, signal: 'ADCA, CH10', pin_signal: HSADC0A_CH10/HSADC1B_CH2}
  - {pin_num: '38', peripheral: HSADC0, signal: 'ADCA, CH5', pin_signal: DAC0_OUT/CMP1_IN3/HSADC0A_CH5/PTE30/FTM0_CH3/FTM_CLKIN1}
  - {pin_num: '30', peripheral: HSADC0, signal: 'ADCA, CH11', pin_signal: HSADC0A_CH11/HSADC1B_CH3}
  - {pin_num: '113', peripheral: HSADC1, signal: 'ADCB, CH11', pin_signal: HSADC1B_CH11/CMP0_IN2/PTC8/FTM3_CH4/FLEXPWM1_A2/FB_AD7}
  - {pin_num: '114', peripheral: HSADC1, signal: 'ADCB, CH12', pin_signal: HSADC1B_CH12/CMP0_IN3/PTC9/FTM3_CH5/FLEXPWM1_B2/FB_AD6}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : MB_InitANA_SENSPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void MB_InitANA_SENSPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTC8 (pin 113) is configured as HSADC1B_CH11 */
    PORT_SetPinMux(MB_I_PFC1_PORT, MB_I_PFC1_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTC9 (pin 114) is configured as HSADC1B_CH12 */
    PORT_SetPinMux(MB_I_PFC2_PORT, MB_I_PFC2_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTE17 (pin 19) is configured as HSADC0A_CH1 */
    PORT_SetPinMux(MB_U_DCB_PORT, MB_U_DCB_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTE18 (pin 20) is configured as HSADC0B_CH0 */
    PORT_SetPinMux(MB_U_DCB_HALF_PORT, MB_U_DCB_HALF_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTE19 (pin 21) is configured as HSADC0B_CH1 */
    PORT_SetPinMux(MB_TEMP_IPM_PORT, MB_TEMP_IPM_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTE29 (pin 37) is configured as HSADC0A_CH4 */
    PORT_SetPinMux(MB_I_DCB_PORT, MB_I_DCB_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTE30 (pin 38) is configured as HSADC0A_CH5 */
    PORT_SetPinMux(MB_TACHO_PORT, MB_TACHO_PIN, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
MB_InitBEMFPins:
- options: {callFromInitBoot: 'false', prefix: MB_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '4', peripheral: HSADC0, signal: 'ADCB, CH11', pin_signal: HSADC0B_CH11/HSADC1B_CH1/PTE3/SPI1_SIN/UART1_RTS_b/TRACE_D1}
  - {pin_num: '39', peripheral: CMP0, signal: 'IN, 4', pin_signal: HSADC0A_CH12/CMP0_IN4/CMP2_IN3}
  - {pin_num: '51', peripheral: CMP0, signal: OUT, pin_signal: PTA1/UART0_RX/FTM0_CH6/CMP0_OUT/FTM2_QD_PHA/FTM1_CH1/JTAG_TDI}
  - {pin_num: '99', peripheral: PWM0, signal: 'X, 0', pin_signal: PTB20/SPI2_PCS0/FLEXPWM0_X0/CMP0_OUT/FB_AD31, direction: INPUT}
  - {pin_num: '45', peripheral: HSADC0, signal: 'ADCB, CH4', pin_signal: HSADC0B_CH4/HSADC1B_CH4/PTE24/CAN1_TX/FTM0_CH0/XB_IN2/I2C0_SCL/EWM_OUT_b/XB_OUT4/UART4_TX}
  - {pin_num: '64', peripheral: CMP2, signal: 'IN, 0', pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/FTM1_QD_PHA/I2C0_SCL}
  - {pin_num: '55', peripheral: CMP2, signal: OUT, pin_signal: PTA5/FTM0_CH2/RMII0_RXER/MII0_RXER/CMP2_OUT/JTAG_TRST_b}
  - {pin_num: '100', peripheral: PWM0, signal: 'X, 1', pin_signal: PTB21/SPI2_SCK/FLEXPWM0_X1/CMP1_OUT/FB_AD30, direction: INPUT}
  - {pin_num: '46', peripheral: HSADC0, signal: 'ADCB, CH5', pin_signal: HSADC0B_CH5/HSADC1B_CH5/PTE25/LLWU_P21/CAN1_RX/FTM0_CH1/XB_IN3/I2C0_SDA/EWM_IN/XB_OUT5/UART4_RX}
  - {pin_num: '66', peripheral: CMP3, signal: 'IN, 0', pin_signal: CMP3_IN0/PTA14/SPI0_PCS0/UART0_TX/CAN2_TX/RMII0_CRS_DV/MII0_RXDV/I2C1_SCL}
  - {pin_num: '102', peripheral: CMP3, signal: OUT, pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FLEXPWM0_X3/CMP3_OUT/FB_AD28}
  - {pin_num: '101', peripheral: PWM0, signal: 'X, 2', pin_signal: PTB22/SPI2_SOUT/FLEXPWM0_X2/CMP2_OUT/FB_AD29, direction: INPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : MB_InitBEMFPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void MB_InitBEMFPins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTA1 (pin 51) is configured as CMP0_OUT */
    PORT_SetPinMux(MB_BEMF_A_CMP_OUT_PORT, MB_BEMF_A_CMP_OUT_PIN, kPORT_MuxAlt4);

    /* PORTA12 (pin 64) is configured as CMP2_IN0 */
    PORT_SetPinMux(MB_BEMF_B_CMP_IN_PORT, MB_BEMF_B_CMP_IN_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTA14 (pin 66) is configured as CMP3_IN0 */
    PORT_SetPinMux(MB_BEMF_C_CMP_IN_PORT, MB_BEMF_C_CMP_IN_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTA5 (pin 55) is configured as CMP2_OUT */
    PORT_SetPinMux(MB_BEMF_B_CMP_OUT_PORT, MB_BEMF_B_CMP_OUT_PIN, kPORT_MuxAlt5);

    /* PORTB20 (pin 99) is configured as FLEXPWM0_X0 */
    PORT_SetPinMux(MB_BEMF_A_CAPTURE_PORT, MB_BEMF_A_CAPTURE_PIN, kPORT_MuxAlt5);

    /* PORTB21 (pin 100) is configured as FLEXPWM0_X1 */
    PORT_SetPinMux(MB_BEMF_B_CAPTURE_PORT, MB_BEMF_B_CAPTURE_PIN, kPORT_MuxAlt5);

    /* PORTB22 (pin 101) is configured as FLEXPWM0_X2 */
    PORT_SetPinMux(MB_BEMF_C_CAPTURE_PORT, MB_BEMF_C_CAPTURE_PIN, kPORT_MuxAlt5);

    /* PORTB23 (pin 102) is configured as CMP3_OUT */
    PORT_SetPinMux(MB_BEMF_C_CMP_OUT_PORT, MB_BEMF_C_CMP_OUT_PIN, kPORT_MuxAlt6);

    /* PORTE24 (pin 45) is configured as HSADC0B_CH4 */
    PORT_SetPinMux(MB_BEMF_B_PORT, MB_BEMF_B_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTE25 (pin 46) is configured as HSADC0B_CH5 */
    PORT_SetPinMux(MB_BEMF_C_PORT, MB_BEMF_C_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTE3 (pin 4) is configured as HSADC0B_CH11 */
    PORT_SetPinMux(MB_BEMF_A_PORT, MB_BEMF_A_PIN, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
MB_InitENCPins:
- options: {callFromInitBoot: 'false', prefix: MB_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '95', peripheral: ENC, signal: 'PHASE, A', pin_signal: PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN2/CAN0_TX/EWM_IN/XB_IN5/FB_AD17}
  - {pin_num: '110', peripheral: ENC, signal: 'PHASE, B', pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/XB_IN2/CMP0_OUT/FTM0_CH2/FB_AD10}
  - {pin_num: '1', peripheral: ENC, signal: INDEX, pin_signal: HSADC0B_CH16/HSADC1A_CH0/PTE0/SPI1_PCS1/UART1_TX/XB_OUT10/XB_IN11/I2C1_SDA/TRACE_CLKOUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : MB_InitENCPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void MB_InitENCPins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);
    /* XBARA Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_XbarA);

    /* PORTB16 (pin 95) is configured as XB_IN5 */
    PORT_SetPinMux(MB_ENC_PHASE_A_PORT, MB_ENC_PHASE_A_PIN, kPORT_MuxAlt7);

    /* PORTC5 (pin 110) is configured as XB_IN2 */
    PORT_SetPinMux(MB_ENC_PHASE_B_PORT, MB_ENC_PHASE_B_PIN, kPORT_MuxAlt4);

    /* PORTE0 (pin 1) is configured as XB_IN11 */
    PORT_SetPinMux(MB_ENC_INDEX_PORT, MB_ENC_INDEX_PIN, kPORT_MuxAlt5);
    /* XB_IN5 input pin output assigned to XBARA_IN5 input is connected
     * to XBARA_OUT44 output assigned to ENC quadrature waveform phase A */
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn5, kXBARA_OutputEncPhA);
    /* XB_IN2 input pin output assigned to XBARA_IN2 input is connected
     * to XBARA_OUT45 output assigned to ENC quadrature waveform phase B */
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn2, kXBARA_OutputEncPhB);
    /* XB_IN11 input pin output assigned to XBARA_IN11 input is connected
     * to XBARA_OUT46 output assigned to ENC refresh/reload */
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn11, kXBARA_OutputEncIndex);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
MB_InitUSB_UARTPins:
- options: {callFromInitBoot: 'false', prefix: MB_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '136', peripheral: UART0, signal: TX, pin_signal: PTD7/UART0_TX/FTM0_CH7/FTM1_CH1/FTM0_FLT1/SPI1_SIN, direction: OUTPUT}
  - {pin_num: '133', peripheral: UART0, signal: RX, pin_signal: HSADC1A_CH9/PTD6/LLWU_P15/SPI0_PCS3/UART0_RX/FTM0_CH6/FTM1_CH0/FTM0_FLT0/SPI1_SOUT/FB_AD0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : MB_InitUSB_UARTPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void MB_InitUSB_UARTPins(void)
{
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTD6 (pin 133) is configured as UART0_RX */
    PORT_SetPinMux(MB_RXD_PORT, MB_RXD_PIN, kPORT_MuxAlt3);

    /* PORTD7 (pin 136) is configured as UART0_TX */
    PORT_SetPinMux(MB_TXD_PORT, MB_TXD_PIN, kPORT_MuxAlt3);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK)))

                  /* UART 0 transmit data source select: UART0_TX pin. */
                  | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)

                  /* UART 0 receive data source select: UART0_RX pin. */
                  | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
MB_InitBRAKEPins:
- options: {callFromInitBoot: 'false', prefix: MB_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '123', peripheral: GPIOC, signal: 'GPIO, 16', pin_signal: PTC16/CAN1_RX/UART3_RX/ENET0_1588_TMR0/FLEXPWM1_A2/FB_CS5_b/FB_TSIZ1/FB_BE23_16_b, direction: OUTPUT,
    gpio_init_state: 'false'}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : MB_InitBRAKEPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void MB_InitBRAKEPins(void)
{
    /* Port C Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    gpio_pin_config_t MCU_BRAKE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTC16 (pin 123)  */
    GPIO_PinInit(MB_MCU_BRAKE_GPIO, MB_MCU_BRAKE_PIN, &MCU_BRAKE_config);

    /* PORTC16 (pin 123) is configured as PTC16 */
    PORT_SetPinMux(MB_MCU_BRAKE_PORT, MB_MCU_BRAKE_PIN, kPORT_MuxAsGpio);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
MB_InitEXT_UARTPins:
- options: {callFromInitBoot: 'false', prefix: MB_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '25', peripheral: UART0, signal: TX, pin_signal: HSADC0A_CH8/ADC0_SE5b/PTE20/FTM1_CH0/UART0_TX/FTM1_QD_PHA, direction: OUTPUT}
  - {pin_num: '26', peripheral: UART0, signal: RX, pin_signal: HSADC0A_CH9/HSADC1A_CH7/PTE21/XB_IN9/FTM1_CH1/UART0_RX/FTM1_QD_PHB}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : MB_InitEXT_UARTPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void MB_InitEXT_UARTPins(void)
{
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTE20 (pin 25) is configured as UART0_TX */
    PORT_SetPinMux(MB_TXD_1_PORT, MB_TXD_1_PIN, kPORT_MuxAlt4);

    /* PORTE21 (pin 26) is configured as UART0_RX */
    PORT_SetPinMux(MB_RXD_1_PORT, MB_RXD_1_PIN, kPORT_MuxAlt4);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK)))

                  /* UART 0 transmit data source select: UART0_TX pin. */
                  | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)

                  /* UART 0 receive data source select: UART0_RX pin. */
                  | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
MB_InitMISCPins:
- options: {callFromInitBoot: 'false', prefix: MB_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '65', peripheral: GPIOA, signal: 'GPIO, 13', pin_signal: CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/RMII0_RXD0/MII0_RXD0/FTM1_QD_PHB/I2C1_SDA, direction: INPUT}
  - {peripheral: HSADC0, signal: 'TRG, A', pin_signal: PWM0_CH3_TRG0}
  - {peripheral: HSADC1, signal: 'TRG, A', pin_signal: PWM0_CH3_TRG0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : MB_InitMISCPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void MB_InitMISCPins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* XBARA Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_XbarA);

    gpio_pin_config_t FAULT_2_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTA13 (pin 65)  */
    GPIO_PinInit(MB_FAULT_2_GPIO, MB_FAULT_2_PIN, &FAULT_2_config);

    /* PORTA13 (pin 65) is configured as PTA13 */
    PORT_SetPinMux(MB_FAULT_2_PORT, MB_FAULT_2_PIN, kPORT_MuxAsGpio);

    SIM->SOPT7 = ((SIM->SOPT7 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT7_HSADC0AALTTRGEN_MASK | SIM_SOPT7_HSADC1ATRGSEL_MASK | SIM_SOPT7_HSADC1AALTTRGEN_MASK)))

                  /* HSADC0A alternate trigger enable: XBARA output 12. */
                  | SIM_SOPT7_HSADC0AALTTRGEN(SOPT7_HSADC0AALTTRGEN_XBAR)

                  /* HSADC1A trigger select: XBARA output 41. */
                  | SIM_SOPT7_HSADC1ATRGSEL(SOPT7_HSADC1ATRGSEL_XBAR)

                  /* HSADC1A alternate trigger enable: 0x02u. */
                  | SIM_SOPT7_HSADC1AALTTRGEN(0x02u));
    /* PWMA channel 3 trigger 0 output assigned to XBARA_IN26 input is connected
     * to XBARA_OUT41 output assigned to PDB1 trigger option 12 */
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputPwm0Ch3Trg0, kXBARA_OutputPdb1InCh12);
    /* PWMA channel 3 trigger 0 output assigned to XBARA_IN26 input is connected
     * to XBARA_OUT12 output assigned to HSADC0 converter A trigger */
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputPwm0Ch3Trg0, kXBARA_OutputHsadc0ATrig);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '52', peripheral: JTAG, signal: TRACE_SWO, pin_signal: PTA2/UART0_TX/FTM0_CH7/CMP1_OUT/FTM2_QD_PHB/FTM1_CH0/JTAG_TDO/TRACE_SWO, pull_select: down, pull_enable: disable}
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

    /* PORTA2 (pin 52) is configured as TRACE_SWO */
    PORT_SetPinMux(BOARD_SWO_PORT, BOARD_SWO_PIN, kPORT_MuxAlt7);

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
BOARD_InitOSCPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '72', peripheral: OSC0, signal: EXTAL0, pin_signal: EXTAL0/PTA18/XB_IN7/FTM0_FLT2/FTM_CLKIN0/XB_OUT8/FTM3_CH2}
  - {pin_num: '73', peripheral: OSC0, signal: XTAL0, pin_signal: XTAL0/PTA19/XB_IN8/FTM1_FLT0/FTM_CLKIN1/XB_OUT9/LPTMR0_ALT1}
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

    /* PORTA18 (pin 72) is configured as EXTAL0 */
    PORT_SetPinMux(BOARD_EXTAL0_PORT, BOARD_EXTAL0_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTA19 (pin 73) is configured as XTAL0 */
    PORT_SetPinMux(BOARD_XTAL0_PORT, BOARD_XTAL0_PIN, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitDEBUG_UARTPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '106', peripheral: UART1, signal: RX, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/FTM3_FLT0}
  - {pin_num: '109', peripheral: UART1, signal: TX, pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/CMP1_OUT/FB_AD11, direction: OUTPUT}
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

    /* PORTC3 (pin 106) is configured as UART1_RX */
    PORT_SetPinMux(BOARD_UART_RX_TGTMCU_PORT, BOARD_UART_RX_TGTMCU_PIN, kPORT_MuxAlt3);

    /* PORTC4 (pin 109) is configured as UART1_TX */
    PORT_SetPinMux(BOARD_UART_TX_TGTMCU_PORT, BOARD_UART_TX_TGTMCU_PIN, kPORT_MuxAlt3);

    SIM->SOPT5 = ((SIM->SOPT5 &
                   /* Mask bits to zero which are setting */
                   (~(SIM_SOPT5_UART1TXSRC_MASK | SIM_SOPT5_UART1RXSRC_MASK)))

                  /* UART 1 transmit data source select: UART1_TX pin. */
                  | SIM_SOPT5_UART1TXSRC(SOPT5_UART1TXSRC_UART_TX)

                  /* UART 1 receive data source select: UART1_RX pin. */
                  | SIM_SOPT5_UART1RXSRC(SOPT5_UART1RXSRC_UART_RX));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDSPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '85', peripheral: GPIOB, signal: 'GPIO, 4', pin_signal: ADC0_SE6b/PTB4/FLEXPWM1_X0/ENET0_1588_TMR2/FTM1_FLT0, direction: OUTPUT, gpio_init_state: 'false'}
  - {pin_num: '98', peripheral: GPIOB, signal: 'GPIO, 19', pin_signal: PTB19/CAN0_RX/FTM2_CH1/FTM3_CH3/FLEXPWM1_B1/FTM2_QD_PHB/FB_OE_b, direction: OUTPUT, gpio_init_state: 'false'}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitLEDSPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitLEDSPins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);

    gpio_pin_config_t LED_RED_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTB4 (pin 85)  */
    GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, &LED_RED_config);

    gpio_pin_config_t LED_GREEN_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTB19 (pin 98)  */
    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, &LED_GREEN_config);

    /* PORTB19 (pin 98) is configured as PTB19 */
    PORT_SetPinMux(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, kPORT_MuxAsGpio);

    /* PORTB4 (pin 85) is configured as PTB4 */
    PORT_SetPinMux(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, kPORT_MuxAsGpio);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitM2_MC_PWMPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '8', peripheral: PWM1, signal: 'A, 0', pin_signal: HSADC1A_CH5/ADC0_SE10/ADC0_DM2/PTE5/SPI1_PCS2/UART3_RX/FLEXPWM1_A0/FTM3_CH0, direction: OUTPUT}
  - {pin_num: '9', peripheral: PWM1, signal: 'B, 0', pin_signal: HSADC1B_CH7/ADC0_SE4a/PTE6/LLWU_P16/SPI1_PCS3/UART3_CTS_b/FLEXPWM1_B0/FTM3_CH1, direction: OUTPUT}
  - {pin_num: '10', peripheral: PWM1, signal: 'A, 1', pin_signal: PTE7/UART3_RTS_b/FLEXPWM1_A1/FTM3_CH2, direction: OUTPUT}
  - {pin_num: '11', peripheral: PWM1, signal: 'B, 1', pin_signal: PTE8/UART5_TX/FLEXPWM1_B1/FTM3_CH3, direction: OUTPUT}
  - {pin_num: '12', peripheral: PWM1, signal: 'A, 2', pin_signal: PTE9/LLWU_P17/UART5_RX/FLEXPWM1_A2/FTM3_CH4, direction: OUTPUT}
  - {pin_num: '13', peripheral: PWM1, signal: 'B, 2', pin_signal: PTE10/LLWU_P18/UART5_CTS_b/FLEXPWM1_B2/FTM3_CH5, direction: OUTPUT}
  - {pin_num: '41', peripheral: PWM1, signal: 'FAULT, 0', pin_signal: PTE22/FTM2_CH0/XB_IN2/FTM2_QD_PHA}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitM2_MC_PWMPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitM2_MC_PWMPins(void)
{
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);
    /* XBARA Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_XbarA);

    /* PORTE10 (pin 13) is configured as FLEXPWM1_B2 */
    PORT_SetPinMux(BOARD_M2_PWM5_PORT, BOARD_M2_PWM5_PIN, kPORT_MuxAlt5);

    /* PORTE22 (pin 41) is configured as XB_IN2 */
    PORT_SetPinMux(BOARD_M2_FAULT1_PORT, BOARD_M2_FAULT1_PIN, kPORT_MuxAlt4);

    /* PORTE5 (pin 8) is configured as FLEXPWM1_A0 */
    PORT_SetPinMux(BOARD_M2_PWM0_PORT, BOARD_M2_PWM0_PIN, kPORT_MuxAlt5);

    /* PORTE6 (pin 9) is configured as FLEXPWM1_B0 */
    PORT_SetPinMux(BOARD_M2_PWM1_PORT, BOARD_M2_PWM1_PIN, kPORT_MuxAlt5);

    /* PORTE7 (pin 10) is configured as FLEXPWM1_A1 */
    PORT_SetPinMux(BOARD_M2_PWM2_PORT, BOARD_M2_PWM2_PIN, kPORT_MuxAlt5);

    /* PORTE8 (pin 11) is configured as FLEXPWM1_B1 */
    PORT_SetPinMux(BOARD_M2_PWM3_PORT, BOARD_M2_PWM3_PIN, kPORT_MuxAlt5);

    /* PORTE9 (pin 12) is configured as FLEXPWM1_A2 */
    PORT_SetPinMux(BOARD_M2_PWM4_PORT, BOARD_M2_PWM4_PIN, kPORT_MuxAlt5);
    /* XB_IN2 input pin output assigned to XBARA_IN2 input is connected
     * to XBARA_OUT29 output assigned to PWM0 and PWM1 fault 0 */
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputXbarIn2, kXBARA_OutputPwm0Fault0);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitM2_ANA_SENSEPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '7', peripheral: HSADC1, signal: 'ADCA, CH4', pin_signal: HSADC1A_CH4/ADC0_SE2/ADC0_DP2/PTE4/LLWU_P2/SPI1_PCS0/UART3_TX/TRACE_D0}
  - {pin_num: '87', peripheral: HSADC1, signal: 'ADCA, CH12', pin_signal: HSADC1A_CH12/PTB6/CAN2_TX/FLEXPWM1_X2/FB_AD23}
  - {pin_num: '88', peripheral: HSADC1, signal: 'ADCA, CH13', pin_signal: HSADC1A_CH13/PTB7/CAN2_RX/FLEXPWM1_X3/FB_AD22}
  - {pin_num: '81', peripheral: HSADC0, signal: 'ADCB, CH2', pin_signal: HSADC0B_CH2/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/FTM1_QD_PHA/UART0_RX/RMII0_MDIO/MII0_MDIO}
  - {pin_num: '82', peripheral: HSADC0, signal: 'ADCB, CH3', pin_signal: HSADC0B_CH3/PTB1/I2C0_SDA/FTM1_CH1/FTM0_FLT2/EWM_IN/FTM1_QD_PHB/UART0_TX/RMII0_MDC/MII0_MDC}
  - {pin_num: '84', peripheral: HSADC0, signal: 'ADCB, CH15', pin_signal: HSADC0B_CH15/CMP3_IN5/PTB3/I2C0_SDA/UART0_CTS_b/UART0_COL_b/ENET0_1588_TMR1/FTM0_FLT0}
  - {pin_num: '83', peripheral: HSADC0, signal: 'ADCA, CH14', pin_signal: HSADC0A_CH14/CMP2_IN2/PTB2/I2C0_SCL/UART0_RTS_b/FTM0_FLT1/ENET0_1588_TMR0/FTM0_FLT3}
  - {pin_num: '24', peripheral: HSADC0, signal: 'ADCA, CH7', pin_signal: HSADC0A_CH7/ADC0_SE4b}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitM2_ANA_SENSEPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitM2_ANA_SENSEPins(void)
{
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTB0 (pin 81) is configured as HSADC0B_CH2 */
    PORT_SetPinMux(BOARD_M2_BEMF_A_PORT, BOARD_M2_BEMF_A_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTB1 (pin 82) is configured as HSADC0B_CH3 */
    PORT_SetPinMux(BOARD_M2_BEMF_B_PORT, BOARD_M2_BEMF_B_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTB2 (pin 83) is configured as HSADC0A_CH14 */
    PORT_SetPinMux(BOARD_M2_I_DCB_PORT, BOARD_M2_I_DCB_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTB3 (pin 84) is configured as HSADC0B_CH15 */
    PORT_SetPinMux(BOARD_M2_BEMF_C_PORT, BOARD_M2_BEMF_C_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTB6 (pin 87) is configured as HSADC1A_CH12 */
    PORT_SetPinMux(BOARD_M2_I_PH_B_PORT, BOARD_M2_I_PH_B_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTB7 (pin 88) is configured as HSADC1A_CH13 */
    PORT_SetPinMux(BOARD_M2_I_PH_C_PORT, BOARD_M2_I_PH_C_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTE4 (pin 7) is configured as HSADC1A_CH4 */
    PORT_SetPinMux(BOARD_M2_I_PH_A_PORT, BOARD_M2_I_PH_A_PIN, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitM2_MISCPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '143', peripheral: GPIOD, signal: 'GPIO, 14', pin_signal: PTD14/SPI2_SIN/XB_IN11/XB_OUT11/FLEXPWM0_A0/FB_A22}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitM2_MISCPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitM2_MISCPins(void)
{
    /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);

    /* PORTD14 (pin 143) is configured as PTD14 */
    PORT_SetPinMux(BOARD_M2_GPIO_PORT, BOARD_M2_GPIO_PIN, kPORT_MuxAsGpio);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
