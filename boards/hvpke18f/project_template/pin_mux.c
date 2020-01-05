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
processor: MKE18F512xxx16
package_id: MKE18F512VLL16
mcu_data: ksdk2_0
processor_version: 0.0.19
board: HVP-KE18F
pin_labels:
- {pin_num: '6', pin_signal: ADC2_SE12/PTE10/CLKOUT/FTM2_CH4/FXIO_D4/TRGMUX_OUT4, label: 'MB_J11[B5]/MB_TP33/''MB_V_sense_DCB/2''/J4[B5]/''U_dcb/2''', identifier: U_DCB_HALF}
- {pin_num: '5', pin_signal: ADC2_SE13/PTE11/PWT_IN1/LPTMR0_ALT1/FTM2_CH5/FXIO_D5/TRGMUX_OUT5, label: 'MB_J11[B8]/MB_TP41/MB_BEMF_sense_C/MB_Phase_C/J4[B8]/BEMF_C',
  identifier: BEMF_C}
- {pin_num: '4', pin_signal: ADC2_SE0/PTD0/FTM0_CH2/LPSPI1_SCK/FTM2_CH0/FXIO_D0/TRGMUX_OUT1, label: 'MB_J11[B7]/MB_TP43/MB_BEMF_sense_B/MB_Phase_B/J4[B7]/BEMF_B',
  identifier: BEMF_B}
- {pin_num: '3', pin_signal: ADC2_SE1/PTD1/FTM0_CH3/LPSPI1_SIN/FTM2_CH1/FXIO_D1/TRGMUX_OUT2, label: 'MB_J11[B6]/MB_TP44/MB_BEMF_sense_A/MB_Phase_A/J4[B6]/BEMF_A',
  identifier: BEMF_A}
- {pin_num: '8', pin_signal: PTE5/TCLK2/FTM2_QD_PHA/FTM2_CH3/CAN0_TX/FXIO_D7/EWM_IN, label: 'MB_J9[3]/MB_J11[B14]/MB_TP13/MB_ENC_PhaseA/J4[B14]/TM0', identifier: ENC_PHASE_A}
- {pin_num: '9', pin_signal: PTE4/BUSOUT/FTM2_QD_PHB/FTM2_CH2/CAN0_RX/FXIO_D6/EWM_OUT_b, label: 'MB_J9[4]/MB_J11[B15]/MB_TP17/MB_ENC_PhaseB/J4[B15]/TM1', identifier: ENC_PHASE_B}
- {pin_num: '10', pin_signal: VDD10, label: 3.3V}
- {pin_num: '11', pin_signal: VDDA, label: 3.3VA}
- {pin_num: '12', pin_signal: VREFH, label: 3.3VA}
- {pin_num: '13', pin_signal: VREFL, label: GNDA}
- {pin_num: '14', pin_signal: VSS15, label: GND}
- {pin_num: '18', pin_signal: PTE3/FTM0_FLT0/LPUART2_RTS/FTM2_FLT0/TRGMUX_IN6/ACMP2_OUT, label: 'MB_J11[A25]/MB_U12[11]/MB_TP28/MB_FAULT/MB_FAULT_1/MB_PFC_overcurrent/J4[A25]/FAULT_1',
  identifier: FAULT_1}
- {pin_num: '19', pin_signal: PTE12/FTM0_FLT3/LPUART2_TX, label: 'U2[24]/U6[13]/TxD_sda', identifier: UART_TX_TGTMCU}
- {pin_num: '20', pin_signal: PTD17/FTM0_FLT2/LPUART2_RX, label: 'U2[25]/U6[14]/RxD_sda', identifier: UART_RX_TGTMCU}
- {pin_num: '21', pin_signal: ACMP2_IN0/PTD16/FTM0_CH1, label: 'MB_J11[A29]/MB_TP24/J4[A29]/MB_TP_24'}
- {pin_num: '22', pin_signal: ACMP2_IN1/PTD15/FTM0_CH0, label: 'MB_J11[A30]/MB_TP22/J4[A30]/MB_TP_22'}
- {pin_num: '15', pin_signal: EXTAL/PTB7/LPI2C0_SCL, label: NC}
- {pin_num: '16', pin_signal: XTAL/PTB6/LPI2C0_SDA, label: NC}
- {pin_num: '17', pin_signal: ACMP2_IN3/PTE14/FTM0_FLT1/FTM2_FLT1, label: NC}
- {pin_num: '7', pin_signal: PTE13/FTM2_FLT0, label: NC}
- {pin_num: '23', pin_signal: ACMP2_IN2/DAC0_OUT/PTE9/FTM0_CH7/LPUART2_CTS, label: USER_LED_2, identifier: LED_RED}
- {pin_num: '24', pin_signal: PTD14/FTM2_CH5/CLKOUT, label: 'MB_J11[B17]/J4[B17]/MB_TP_29'}
- {pin_num: '1', pin_signal: PTE16/FTM2_CH7/FXIO_D3/TRGMUX_OUT7, label: NC}
- {pin_num: '2', pin_signal: PTE15/FTM2_CH6/FXIO_D2/TRGMUX_OUT6, label: NC}
- {pin_num: '25', pin_signal: PTD13/FTM2_CH4/RTC_CLKOUT, label: 'MB_J9[5]/MB_J11[B16]/MB_TP19/MB_ENC_Index/J4[B16]/TM2', identifier: ENC_INDEX}
- {pin_num: '98', pin_signal: PTA4/ACMP0_OUT/EWM_OUT_b/JTAG_TMS/SWD_DIO, label: 'J3[2]/U5[14]/U4[3]/SWDIO/SWDIO_ISOLATED'}
- {pin_num: '96', pin_signal: ACMP0_IN2/PTC4/FTM1_CH0/RTC_CLKOUT/EWM_IN/FTM1_QD_PHB/JTAG_TCLK/SWD_CLK, label: 'J3[4]/U4[11]/U5[13]/SWDCLK_ISOLATED/SWCLK'}
- {pin_num: '92', pin_signal: PTA10/FTM1_CH4/LPUART0_TX/FXIO_D0/JTAG_TDO/noetm_Trace_SWO, label: 'J3[6]/U5[12]/SWO/SWO_ISOLATED'}
- {pin_num: '97', pin_signal: PTA5/TCLK1/JTAG_TRST_b/RESET_b, label: 'J3[10]/U5[11]/U2[21]/''/RESET''/''/RESET_ISOLATED''/SW1'}
- {pin_num: '37', pin_signal: VSS38, label: GND}
- {pin_num: '60', pin_signal: VSS61, label: GND}
- {pin_num: '61', pin_signal: VDD62, label: 3.3V}
- {pin_num: '87', pin_signal: VDD88, label: 3.3V}
- {pin_num: '86', pin_signal: VSS87, label: GND}
- {pin_num: '38', pin_signal: VDD39, label: 3.3V}
- {pin_num: '27', pin_signal: PTB5/FTM0_CH5/LPSPI0_PCS1/TRGMUX_IN0/ACMP1_OUT, label: NC}
- {pin_num: '28', pin_signal: ACMP1_IN2/PTB4/FTM0_CH4/LPSPI0_SOUT/TRGMUX_IN1, label: NC}
- {pin_num: '30', pin_signal: ADC0_SE10/ACMP0_IN5/XTAL32/PTC2/FTM0_CH2/CAN0_RX, label: NC}
- {pin_num: '31', pin_signal: PTD7/LPUART2_TX/FTM2_FLT3, label: NC}
- {pin_num: '32', pin_signal: PTD6/LPUART2_RX/FTM2_FLT2, label: NC}
- {pin_num: '33', pin_signal: PTD5/FTM2_CH3/LPTMR0_ALT2/FTM2_FLT1/PWT_IN2/TRGMUX_IN7, label: NC}
- {pin_num: '34', pin_signal: PTD12/FTM2_CH2/LPI2C1_HREQ/LPUART2_RTS, label: NC}
- {pin_num: '35', pin_signal: PTD11/FTM2_CH1/FTM2_QD_PHA/LPUART2_CTS, label: NC}
- {pin_num: '36', pin_signal: PTD10/FTM2_CH0/FTM2_QD_PHB, label: NC}
- {pin_num: '39', pin_signal: ADC0_SE9/ACMP1_IN3/PTC1/FTM0_CH1/FTM1_CH7, label: NC}
- {pin_num: '40', pin_signal: ADC0_SE8/ACMP1_IN4/PTC0/FTM0_CH0/FTM1_CH6, label: NC}
- {pin_num: '41', pin_signal: ACMP1_IN5/PTD9/LPI2C1_SCL/FTM2_FLT3/FTM1_CH5, label: NC}
- {pin_num: '43', pin_signal: ADC0_SE15/PTC17/FTM1_FLT3/LPI2C1_SCLS, label: NC}
- {pin_num: '44', pin_signal: ADC0_SE14/PTC16/FTM1_FLT2/LPI2C1_SDAS, label: NC}
- {pin_num: '47', pin_signal: ADC0_SE7/PTB3/FTM1_CH1/LPSPI0_SIN/FTM1_QD_PHA/TRGMUX_IN2, label: NC}
- {pin_num: '49', pin_signal: PTC13/FTM3_CH7/FTM2_CH7, label: NC}
- {pin_num: '50', pin_signal: PTC12/FTM3_CH6/FTM2_CH6, label: NC}
- {pin_num: '51', pin_signal: PTC11/FTM3_CH5, label: NC}
- {pin_num: '52', pin_signal: PTC10/FTM3_CH4, label: NC}
- {pin_num: '62', pin_signal: PTA17/FTM0_CH6/FTM3_FLT0/EWM_OUT_b, label: NC}
- {pin_num: '71', pin_signal: ADC1_SE2/PTD2/FTM3_CH4/LPSPI1_SOUT/FXIO_D4/TRGMUX_IN5, label: NC}
- {pin_num: '74', pin_signal: ADC2_SE8/PTB11/FTM3_CH3/LPI2C0_HREQ, label: NC}
- {pin_num: '75', pin_signal: ADC2_SE9/PTB10/FTM3_CH2/LPI2C0_SDAS, label: NC}
- {pin_num: '76', pin_signal: ADC2_SE10/PTB9/FTM3_CH1/LPI2C0_SCLS, label: NC}
- {pin_num: '77', pin_signal: ADC2_SE11/PTB8/FTM3_CH0, label: NC}
- {pin_num: '78', pin_signal: ADC0_SE1/ACMP0_IN1/PTA1/FTM1_CH1/LPI2C0_SDAS/FXIO_D3/FTM1_QD_PHA/LPUART0_RTS/TRGMUX_OUT0, label: NC}
- {pin_num: '82', pin_signal: ADC1_SE13/PTA16/FTM1_CH3/LPSPI1_PCS2, label: NC}
- {pin_num: '83', pin_signal: ADC1_SE12/PTA15/FTM1_CH2/LPSPI0_PCS3, label: NC}
- {pin_num: '89', pin_signal: ADC2_SE4/PTA13/FTM1_CH7/CAN1_TX/LPI2C1_SCLS, label: NC}
- {pin_num: '91', pin_signal: PTA11/FTM1_CH5/LPUART0_RX/FXIO_D1, label: NC}
- {pin_num: '93', pin_signal: ADC2_SE6/PTE1/LPSPI0_SIN/LPI2C0_HREQ/LPI2C1_SCL/FTM1_FLT1, label: NC}
- {pin_num: '94', pin_signal: ADC2_SE7/PTE0/LPSPI0_SCK/TCLK1/LPI2C1_SDA/FTM1_FLT2, label: NC}
- {pin_num: '95', pin_signal: PTC5/FTM2_CH0/RTC_CLKOUT/LPI2C1_HREQ/FTM2_QD_PHB/JTAG_TDI, label: NC}
- {pin_num: '99', pin_signal: PTA9/FXIO_D7/FTM3_FLT2/FTM1_FLT3, label: NC}
- {pin_num: '100', pin_signal: PTA8/FXIO_D6/FTM3_FLT3, label: NC}
- {pin_num: '84', pin_signal: ADC1_SE11/ACMP0_IN6/PTE6/LPSPI0_PCS2/FTM3_CH7/LPUART1_RTS, label: 'MB_J11[B20]/MB_U9[4]/MB_USER_LED/J4[B20]/USER_LED', identifier: LED_GREEN}
- {pin_num: '26', pin_signal: ACMP0_IN3/PTE8/FTM0_CH6, label: 'MB_J11[B25]/MB_U9[2]/MB_TP_35/MB_MCU_BRAKE/J4[B25]/MCU_BRAKE', identifier: MCU_BRAKE}
- {pin_num: '29', pin_signal: ADC0_SE11/ACMP0_IN4/EXTAL32/PTC3/FTM0_CH3/CAN0_TX, label: 'MB_J11[A6]/MB_TP16/MB_TACHO/J4[A6]/TACHO', identifier: TACHO}
- {pin_num: '42', pin_signal: PTD8/LPI2C1_SDA/FTM2_FLT2/FTM1_CH4, label: 'MB_J11[A23]/MB_U14[2]/MB_TP12/MB_PWM_PFC1/J4[A23]/PWM9', identifier: PWM_PFC1}
- {pin_num: '45', pin_signal: ADC0_SE13/ACMP2_IN4/PTC15/FTM1_CH3, label: 'MB_J11[A8]/MB_U2B[7]/MB_TP14/MB_Ipfc1/J4[A8]/I_pfc1', identifier: I_PFC1}
- {pin_num: '46', pin_signal: ADC0_SE12/ACMP2_IN5/PTC14/FTM1_CH2, label: 'MB_J11[A9]/MB_U2A[1]/MB_TP5/MB_Ipfc2/J4[A9]/I_pfc2', identifier: I_PFC2}
- {pin_num: '48', pin_signal: ADC0_SE6/PTB2/FTM1_CH0/LPSPI0_SCK/FTM1_QD_PHB/TRGMUX_IN3, label: 'MB_J11[A22]/MB_U14[4]/MB_TP15/MB_PWM_PFC2/J4[A22]/PWM8', identifier: PWM_PFC2}
- {pin_num: '53', pin_signal: ADC0_SE5/ADC1_SE15/PTB1/LPUART0_TX/LPSPI0_SOUT/TCLK0, label: 'MB_J11[A5]/MB_U8A[1]/MB_TP37/MB_I_sense_C/J4[A5]/I_phC', identifier: I_PH_C}
- {pin_num: '54', pin_signal: ADC0_SE4/ADC1_SE14/PTB0/LPUART0_RX/LPSPI0_PCS0/LPTMR0_ALT3/PWT_IN3, label: 'MB_J11[A4]/MB_U11B[7]/MB_TP40/MB_I_sense_B/J4[A4]/I_phB',
  identifier: I_PH_B}
- {pin_num: '56', pin_signal: ADC2_SE14/PTC8/LPUART1_RX/FTM1_FLT0/LPUART0_CTS, label: 'MB_J2[4]/MB_J3[4]/MB_J11[A32]/MB_RxD_1/MB_RxD_EXT/J4[A32]/RxD1', identifier: RXD_1}
- {pin_num: '55', pin_signal: ADC2_SE15/PTC9/LPUART1_TX/FTM1_FLT1/LPUART0_RTS, label: 'MB_J1[1]/MB_J2[3]/MB_J11[A31]/MB_TxD_1/MB_TxD_EXT/J4[A31]/TxD1', identifier: TXD_1}
- {pin_num: '57', pin_signal: ADC0_SE3/ACMP1_IN1/PTA7/FTM0_FLT2/RTC_CLKIN/LPUART1_RTS, label: 'MB_J11[B22]/MB_PFC_zc_1/J4[B22]/MB_TP_26', identifier: PFC_ZC_1}
- {pin_num: '58', pin_signal: ADC0_SE2/ACMP1_IN0/PTA6/FTM0_FLT1/LPSPI1_PCS1/LPUART1_CTS, label: 'MB_J11[B21]/MB_PFC_zc_2/J4[B21]/MB_TP_27', identifier: PFC_ZC_2}
- {pin_num: '59', pin_signal: ADC2_SE2/ACMP2_IN6/PTE7/FTM0_CH7/FTM3_FLT0, label: 'MB_J11[B3]/MB_U8B[7]/MB_TP34/MB_I_sense_DCB/J4[B3]/I_dcb', identifier: I_DCB}
- {pin_num: '63', pin_signal: ADC2_SE3/PTB17/FTM0_CH5/LPSPI1_PCS3, label: 'MB_J11[A19]/MB_U12[14]/MB_PWM_CB/J4[A19]/PWM5', identifier: PWM_CB}
- {pin_num: '64', pin_signal: ADC1_SE15/PTB16/FTM0_CH4/LPSPI1_SOUT, label: 'MB_J11[A18]/MB_U12[20]/MB_PWM_CT/J4[A18]/PWM4', identifier: PWM_CT}
- {pin_num: '65', pin_signal: ADC1_SE14/PTB15/FTM0_CH3/LPSPI1_SIN, label: 'MB_J11[A17]/MB_U12[13]/MB_PWM_BB/J4[A17]/PWM3', identifier: PWM_BB}
- {pin_num: '66', pin_signal: ADC1_SE9/ADC2_SE9/PTB14/FTM0_CH2/LPSPI1_SCK, label: 'MB_J11[A16]/MB_U12[19]/MB_PWM_BT/J4[A16]/PWM2', identifier: PWM_BT}
- {pin_num: '67', pin_signal: ADC1_SE8/ADC2_SE8/PTB13/FTM0_CH1/FTM3_FLT1, label: 'MB_J11[A15]/MB_U12[12]/MB_PWM_AB/J4[A15]/PWM1', identifier: PWM_AB}
- {pin_num: '68', pin_signal: ADC1_SE7/PTB12/FTM0_CH0/FTM3_FLT2, label: 'MB_J11[A14]/MB_U12[18]/MB_PWM_AT/J4[A14]/PWM0', identifier: PWM_AT}
- {pin_num: '69', pin_signal: ADC1_SE6/ACMP1_IN6/PTD4/FTM0_FLT3/FTM3_FLT3, label: 'MB_J11[B9]/MB_U12[2]/MB_TP38/MB_IPM_temp/J4[B9]/Temp_IPM', identifier: TEMP_IPM}
- {pin_num: '70', pin_signal: ADC1_SE3/PTD3/FTM3_CH5/LPSPI1_PCS0/FXIO_D5/TRGMUX_IN4/NMI_b, label: NMI}
- {pin_num: '72', pin_signal: ADC1_SE1/PTA3/FTM3_CH1/LPI2C0_SCL/EWM_IN/LPUART0_TX, label: 'MB_J1[3]/MB_J11[B26]/MB_U1[13]/MB_U3[25]/MB_TP3/MB_TP7/MB_TxD/J4[B26]/TxD',
  identifier: TXD}
- {pin_num: '73', pin_signal: ADC1_SE0/PTA2/FTM3_CH0/LPI2C0_SDA/EWM_OUT_b/LPUART0_RX, label: 'MB_J3[2]/MB_J11[B27]/MB_U1[12]/MB_U3[26]/MB_TP2/MB_TP10/MB_RxD/J4[B27]/RxD',
  identifier: RXD}
- {pin_num: '79', pin_signal: ADC0_SE0/ACMP0_IN0/PTA0/FTM2_CH1/LPI2C0_SCLS/FXIO_D2/FTM2_QD_PHA/LPUART0_CTS/TRGMUX_OUT3, label: 'MB_J11[A3]/MB_U11A[1]/MB_TP45/MB_I_sense_A/J4[A3]/I_phA',
  identifier: I_PH_A}
- {pin_num: '80', pin_signal: ADC1_SE5/PTC7/LPUART1_TX/CAN1_TX/FTM3_CH3, label: 'MB_J11[B4]/MB_TP31/MB_V_sense_DCB/MB_DCB_Pos/J4[B4]/U_dcb', identifier: U_DCB}
- {pin_num: '81', pin_signal: ADC1_SE4/PTC6/LPUART1_RX/CAN1_RX/FTM3_CH2, label: 'MB_J11[A7]/MB_TP18/MB_Vin/J4[A7]/V_in', identifier: V_IN}
- {pin_num: '85', pin_signal: ADC1_SE10/PTE2/LPSPI0_SOUT/LPTMR0_ALT3/FTM3_CH6/PWT_IN3/LPUART1_CTS, label: 'MB_J11[B28]/MB_Q3[1]/MB_Relay/J4[B28]/Relay', identifier: RELAY}
- {pin_num: '88', pin_signal: PTA14/FTM0_FLT0/FTM3_FLT1/EWM_IN/FTM1_FLT0/BUSOUT, label: 'MB_J11[A26]/MB_U9[2]/MB_TP35/MB_FAULT_2/MB_Over-voltage_FAULT/J4[A26]/FAULT_2',
  identifier: FAULT_2}
- {pin_num: '90', pin_signal: ADC2_SE5/PTA12/FTM1_CH6/CAN1_RX/LPI2C1_SDAS, label: NC}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_trgmux.h"
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
  - {pin_num: '68', peripheral: FTM0, signal: 'CH, 0', pin_signal: ADC1_SE7/PTB12/FTM0_CH0/FTM3_FLT2, direction: OUTPUT}
  - {pin_num: '67', peripheral: FTM0, signal: 'CH, 1', pin_signal: ADC1_SE8/ADC2_SE8/PTB13/FTM0_CH1/FTM3_FLT1, direction: OUTPUT}
  - {pin_num: '66', peripheral: FTM0, signal: 'CH, 2', pin_signal: ADC1_SE9/ADC2_SE9/PTB14/FTM0_CH2/LPSPI1_SCK, direction: OUTPUT}
  - {pin_num: '65', peripheral: FTM0, signal: 'CH, 3', pin_signal: ADC1_SE14/PTB15/FTM0_CH3/LPSPI1_SIN, direction: OUTPUT}
  - {pin_num: '64', peripheral: FTM0, signal: 'CH, 4', pin_signal: ADC1_SE15/PTB16/FTM0_CH4/LPSPI1_SOUT, direction: OUTPUT}
  - {pin_num: '63', peripheral: FTM0, signal: 'CH, 5', pin_signal: ADC2_SE3/PTB17/FTM0_CH5/LPSPI1_PCS3, direction: OUTPUT}
  - {pin_num: '18', peripheral: FTM0, signal: 'TRG, 2', pin_signal: PTE3/FTM0_FLT0/LPUART2_RTS/FTM2_FLT0/TRGMUX_IN6/ACMP2_OUT}
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
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTB12 (pin 68) is configured as FTM0_CH0 */
    PORT_SetPinMux(MB_PWM_AT_PORT, MB_PWM_AT_PIN, kPORT_MuxAlt2);

    /* PORTB13 (pin 67) is configured as FTM0_CH1 */
    PORT_SetPinMux(MB_PWM_AB_PORT, MB_PWM_AB_PIN, kPORT_MuxAlt2);

    /* PORTB14 (pin 66) is configured as FTM0_CH2 */
    PORT_SetPinMux(MB_PWM_BT_PORT, MB_PWM_BT_PIN, kPORT_MuxAlt2);

    /* PORTB15 (pin 65) is configured as FTM0_CH3 */
    PORT_SetPinMux(MB_PWM_BB_PORT, MB_PWM_BB_PIN, kPORT_MuxAlt2);

    /* PORTB16 (pin 64) is configured as FTM0_CH4 */
    PORT_SetPinMux(MB_PWM_CT_PORT, MB_PWM_CT_PIN, kPORT_MuxAlt2);

    /* PORTB17 (pin 63) is configured as FTM0_CH5 */
    PORT_SetPinMux(MB_PWM_CB_PORT, MB_PWM_CB_PIN, kPORT_MuxAlt2);

    /* PORTE3 (pin 18) is configured as FTM0_FLT0 */
    PORT_SetPinMux(MB_FAULT_1_PORT, MB_FAULT_1_PIN, kPORT_MuxAlt2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
MB_InitPFCPins:
- options: {prefix: MB_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '58', peripheral: CMP1, signal: 'IN, 0', pin_signal: ADC0_SE2/ACMP1_IN0/PTA6/FTM0_FLT1/LPSPI1_PCS1/LPUART1_CTS}
  - {pin_num: '57', peripheral: CMP1, signal: 'IN, 1', pin_signal: ADC0_SE3/ACMP1_IN1/PTA7/FTM0_FLT2/RTC_CLKIN/LPUART1_RTS}
  - {pin_num: '85', peripheral: GPIOE, signal: 'GPIO, 2', pin_signal: ADC1_SE10/PTE2/LPSPI0_SOUT/LPTMR0_ALT3/FTM3_CH6/PWT_IN3/LPUART1_CTS, direction: OUTPUT, gpio_init_state: 'false'}
  - {pin_num: '48', peripheral: FTM1, signal: 'CH, 0', pin_signal: ADC0_SE6/PTB2/FTM1_CH0/LPSPI0_SCK/FTM1_QD_PHB/TRGMUX_IN3, direction: OUTPUT}
  - {pin_num: '42', peripheral: FTM1, signal: 'CH, 4', pin_signal: PTD8/LPI2C1_SDA/FTM2_FLT2/FTM1_CH4, direction: OUTPUT}
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
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    gpio_pin_config_t RELAY_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTE2 (pin 85)  */
    GPIO_PinInit(MB_RELAY_GPIO, MB_RELAY_PIN, &RELAY_config);

    /* PORTA6 (pin 58) is configured as ACMP1_IN0 */
    PORT_SetPinMux(MB_PFC_ZC_2_PORT, MB_PFC_ZC_2_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTA7 (pin 57) is configured as ACMP1_IN1 */
    PORT_SetPinMux(MB_PFC_ZC_1_PORT, MB_PFC_ZC_1_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTB2 (pin 48) is configured as FTM1_CH0 */
    PORT_SetPinMux(MB_PWM_PFC2_PORT, MB_PWM_PFC2_PIN, kPORT_MuxAlt2);

    /* PORTD8 (pin 42) is configured as FTM1_CH4 */
    PORT_SetPinMux(MB_PWM_PFC1_PORT, MB_PWM_PFC1_PIN, kPORT_MuxAlt6);

    /* PORTE2 (pin 85) is configured as PTE2 */
    PORT_SetPinMux(MB_RELAY_PORT, MB_RELAY_PIN, kPORT_MuxAsGpio);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
MB_InitANA_SENSPins:
- options: {prefix: MB_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '59', peripheral: ADC2, signal: 'SE, 2', pin_signal: ADC2_SE2/ACMP2_IN6/PTE7/FTM0_CH7/FTM3_FLT0}
  - {pin_num: '80', peripheral: ADC1, signal: 'SE, 5', pin_signal: ADC1_SE5/PTC7/LPUART1_TX/CAN1_TX/FTM3_CH3}
  - {pin_num: '6', peripheral: ADC2, signal: 'SE, 12', pin_signal: ADC2_SE12/PTE10/CLKOUT/FTM2_CH4/FXIO_D4/TRGMUX_OUT4}
  - {pin_num: '69', peripheral: ADC1, signal: 'SE, 6', pin_signal: ADC1_SE6/ACMP1_IN6/PTD4/FTM0_FLT3/FTM3_FLT3}
  - {pin_num: '53', peripheral: ADC0, signal: 'SE, 5', pin_signal: ADC0_SE5/ADC1_SE15/PTB1/LPUART0_TX/LPSPI0_SOUT/TCLK0}
  - {pin_num: '54', peripheral: ADC0, signal: 'SE, 4', pin_signal: ADC0_SE4/ADC1_SE14/PTB0/LPUART0_RX/LPSPI0_PCS0/LPTMR0_ALT3/PWT_IN3}
  - {pin_num: '79', peripheral: ADC0, signal: 'SE, 0', pin_signal: ADC0_SE0/ACMP0_IN0/PTA0/FTM2_CH1/LPI2C0_SCLS/FXIO_D2/FTM2_QD_PHA/LPUART0_CTS/TRGMUX_OUT3}
  - {pin_num: '29', peripheral: ADC0, signal: 'SE, 11', pin_signal: ADC0_SE11/ACMP0_IN4/EXTAL32/PTC3/FTM0_CH3/CAN0_TX}
  - {pin_num: '81', peripheral: ADC1, signal: 'SE, 4', pin_signal: ADC1_SE4/PTC6/LPUART1_RX/CAN1_RX/FTM3_CH2}
  - {pin_num: '45', peripheral: ADC0, signal: 'SE, 13', pin_signal: ADC0_SE13/ACMP2_IN4/PTC15/FTM1_CH3}
  - {pin_num: '46', peripheral: ADC0, signal: 'SE, 12', pin_signal: ADC0_SE12/ACMP2_IN5/PTC14/FTM1_CH2}
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
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTA0 (pin 79) is configured as ADC0_SE0 */
    PORT_SetPinMux(MB_I_PH_A_PORT, MB_I_PH_A_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTB0 (pin 54) is configured as ADC0_SE4 */
    PORT_SetPinMux(MB_I_PH_B_PORT, MB_I_PH_B_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTB1 (pin 53) is configured as ADC0_SE5 */
    PORT_SetPinMux(MB_I_PH_C_PORT, MB_I_PH_C_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTC14 (pin 46) is configured as ADC0_SE12 */
    PORT_SetPinMux(MB_I_PFC2_PORT, MB_I_PFC2_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTC15 (pin 45) is configured as ADC0_SE13 */
    PORT_SetPinMux(MB_I_PFC1_PORT, MB_I_PFC1_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTC3 (pin 29) is configured as ADC0_SE11 */
    PORT_SetPinMux(MB_TACHO_PORT, MB_TACHO_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTC6 (pin 81) is configured as ADC1_SE4 */
    PORT_SetPinMux(MB_V_IN_PORT, MB_V_IN_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTC7 (pin 80) is configured as ADC1_SE5 */
    PORT_SetPinMux(MB_U_DCB_PORT, MB_U_DCB_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTD4 (pin 69) is configured as ADC1_SE6 */
    PORT_SetPinMux(MB_TEMP_IPM_PORT, MB_TEMP_IPM_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTE10 (pin 6) is configured as ADC2_SE12 */
    PORT_SetPinMux(MB_U_DCB_HALF_PORT, MB_U_DCB_HALF_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTE7 (pin 59) is configured as ADC2_SE2 */
    PORT_SetPinMux(MB_I_DCB_PORT, MB_I_DCB_PIN, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
MB_InitBEMFPins:
- options: {prefix: MB_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '3', peripheral: ADC2, signal: 'SE, 1', pin_signal: ADC2_SE1/PTD1/FTM0_CH3/LPSPI1_SIN/FTM2_CH1/FXIO_D1/TRGMUX_OUT2}
  - {pin_num: '4', peripheral: ADC2, signal: 'SE, 0', pin_signal: ADC2_SE0/PTD0/FTM0_CH2/LPSPI1_SCK/FTM2_CH0/FXIO_D0/TRGMUX_OUT1}
  - {pin_num: '5', peripheral: ADC2, signal: 'SE, 13', pin_signal: ADC2_SE13/PTE11/PWT_IN1/LPTMR0_ALT1/FTM2_CH5/FXIO_D5/TRGMUX_OUT5}
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
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTD0 (pin 4) is configured as ADC2_SE0 */
    PORT_SetPinMux(MB_BEMF_B_PORT, MB_BEMF_B_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTD1 (pin 3) is configured as ADC2_SE1 */
    PORT_SetPinMux(MB_BEMF_A_PORT, MB_BEMF_A_PIN, kPORT_PinDisabledOrAnalog);

    /* PORTE11 (pin 5) is configured as ADC2_SE13 */
    PORT_SetPinMux(MB_BEMF_C_PORT, MB_BEMF_C_PIN, kPORT_PinDisabledOrAnalog);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
MB_InitENCPins:
- options: {prefix: MB_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '8', peripheral: FTM2, signal: 'QD_PH, A', pin_signal: PTE5/TCLK2/FTM2_QD_PHA/FTM2_CH3/CAN0_TX/FXIO_D7/EWM_IN}
  - {pin_num: '9', peripheral: FTM2, signal: 'QD_PH, B', pin_signal: PTE4/BUSOUT/FTM2_QD_PHB/FTM2_CH2/CAN0_RX/FXIO_D6/EWM_OUT_b}
  - {pin_num: '25', peripheral: GPIOD, signal: 'GPIO, 13', pin_signal: PTD13/FTM2_CH4/RTC_CLKOUT, direction: INPUT}
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
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    gpio_pin_config_t ENC_INDEX_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTD13 (pin 25)  */
    GPIO_PinInit(MB_ENC_INDEX_GPIO, MB_ENC_INDEX_PIN, &ENC_INDEX_config);

    /* PORTD13 (pin 25) is configured as PTD13 */
    PORT_SetPinMux(MB_ENC_INDEX_PORT, MB_ENC_INDEX_PIN, kPORT_MuxAsGpio);

    /* PORTE4 (pin 9) is configured as FTM2_QD_PHB */
    PORT_SetPinMux(MB_ENC_PHASE_B_PORT, MB_ENC_PHASE_B_PIN, kPORT_MuxAlt3);

    /* PORTE5 (pin 8) is configured as FTM2_QD_PHA */
    PORT_SetPinMux(MB_ENC_PHASE_A_PORT, MB_ENC_PHASE_A_PIN, kPORT_MuxAlt3);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
MB_InitUSB_UARTPins:
- options: {prefix: MB_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '72', peripheral: LPUART0, signal: TX, pin_signal: ADC1_SE1/PTA3/FTM3_CH1/LPI2C0_SCL/EWM_IN/LPUART0_TX}
  - {pin_num: '73', peripheral: LPUART0, signal: RX, pin_signal: ADC1_SE0/PTA2/FTM3_CH0/LPI2C0_SDA/EWM_OUT_b/LPUART0_RX}
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
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);

    /* PORTA2 (pin 73) is configured as LPUART0_RX */
    PORT_SetPinMux(MB_RXD_PORT, MB_RXD_PIN, kPORT_MuxAlt6);

    /* PORTA3 (pin 72) is configured as LPUART0_TX */
    PORT_SetPinMux(MB_TXD_PORT, MB_TXD_PIN, kPORT_MuxAlt6);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
MB_InitEXT_UARTPins:
- options: {prefix: MB_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '55', peripheral: LPUART1, signal: TX, pin_signal: ADC2_SE15/PTC9/LPUART1_TX/FTM1_FLT1/LPUART0_RTS}
  - {pin_num: '56', peripheral: LPUART1, signal: RX, pin_signal: ADC2_SE14/PTC8/LPUART1_RX/FTM1_FLT0/LPUART0_CTS}
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
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortC);

    /* PORTC8 (pin 56) is configured as LPUART1_RX */
    PORT_SetPinMux(MB_RXD_1_PORT, MB_RXD_1_PIN, kPORT_MuxAlt2);

    /* PORTC9 (pin 55) is configured as LPUART1_TX */
    PORT_SetPinMux(MB_TXD_1_PORT, MB_TXD_1_PIN, kPORT_MuxAlt2);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
MB_InitBRAKEPins:
- options: {prefix: MB_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '26', peripheral: GPIOE, signal: 'GPIO, 8', pin_signal: ACMP0_IN3/PTE8/FTM0_CH6, direction: OUTPUT, gpio_init_state: 'false'}
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
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    gpio_pin_config_t MCU_BRAKE_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTE8 (pin 26)  */
    GPIO_PinInit(MB_MCU_BRAKE_GPIO, MB_MCU_BRAKE_PIN, &MCU_BRAKE_config);

    /* PORTE8 (pin 26) is configured as PTE8 */
    PORT_SetPinMux(MB_MCU_BRAKE_PORT, MB_MCU_BRAKE_PIN, kPORT_MuxAsGpio);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
MB_InitMISCPins:
- options: {prefix: MB_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {peripheral: PDB0, signal: TRG, pin_signal: FTM0_Trigger}
  - {peripheral: PDB1, signal: TRG, pin_signal: FTM0_Trigger}
  - {peripheral: ADC0, signal: 'TRG, A', pin_signal: PDB0_CH0_TriggerA}
  - {peripheral: ADC0, signal: 'TRG, B', pin_signal: PDB0_CH0_TriggerB}
  - {peripheral: ADC1, signal: 'TRG, A', pin_signal: PDB1_CH0_TriggerA}
  - {peripheral: ADC1, signal: 'TRG, B', pin_signal: PDB1_CH0_TriggerB}
  - {pin_num: '88', peripheral: GPIOA, signal: 'GPIO, 14', pin_signal: PTA14/FTM0_FLT0/FTM3_FLT1/EWM_IN/FTM1_FLT0/BUSOUT, direction: INPUT}
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
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);

    gpio_pin_config_t FAULT_2_config = {
        .pinDirection = kGPIO_DigitalInput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTA14 (pin 88)  */
    GPIO_PinInit(MB_FAULT_2_GPIO, MB_FAULT_2_PIN, &FAULT_2_config);

    /* PORTA14 (pin 88) is configured as PTA14 */
    PORT_SetPinMux(MB_FAULT_2_PORT, MB_FAULT_2_PIN, kPORT_MuxAsGpio);

    SIM->ADCOPT = ((SIM->ADCOPT &
                    /* Mask bits to zero which are setting */
                    (~(SIM_ADCOPT_ADC0TRGSEL_MASK | SIM_ADCOPT_ADC0PRETRGSEL_MASK | SIM_ADCOPT_ADC1TRGSEL_MASK)))

                   /* ADC0 trigger source select: PDB output. */
                   | SIM_ADCOPT_ADC0TRGSEL(ADCOPT_ADC0TRGSEL_PDB)

                   /* ADC0 pre-trigger source select: PDB pre-trigger (default). */
                   | SIM_ADCOPT_ADC0PRETRGSEL(ADCOPT_ADC0PRETRGSEL_PDB)

                   /* ADC1 trigger source select: PDB output. */
                   | SIM_ADCOPT_ADC1TRGSEL(ADCOPT_ADC1TRGSEL_PDB));
    /* FTM0 is selected as PDB0 device trigger input 0 */
    TRGMUX_SetTriggerSource(TRGMUX0, kTRGMUX_Pdb0, kTRGMUX_TriggerInput0, kTRGMUX_SourceFtm0);
    /* FTM0 is selected as PDB1 device trigger input 0 */
    TRGMUX_SetTriggerSource(TRGMUX0, kTRGMUX_Pdb1, kTRGMUX_TriggerInput0, kTRGMUX_SourceFtm0);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '92', peripheral: CoreDebug, signal: TRACE_SWO, pin_signal: PTA10/FTM1_CH4/LPUART0_TX/FXIO_D0/JTAG_TDO/noetm_Trace_SWO, pull_select: down, pull_enable: disable}
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
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);

    /* PORTA10 (pin 92) is configured as noetm_Trace_SWO */
    PORT_SetPinMux(PORTA, 10U, kPORT_MuxAlt7);

    PORTA->PCR[10] = ((PORTA->PCR[10] &
                       /* Mask bits to zero which are setting */
                       (~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))

                      /* Pull Select: Internal pulldown resistor is enabled on the corresponding pin, if the
                       * corresponding PE field is set. */
                      | PORT_PCR_PS(kPORT_PullDown)

                      /* Pull Enable: Internal pullup or pulldown resistor is not enabled on the corresponding
                       * pin. */
                      | PORT_PCR_PE(kPORT_PullDisable));
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitDEBUG_UARTPins:
- options: {callFromInitBoot: 'true', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '19', peripheral: LPUART2, signal: TX, pin_signal: PTE12/FTM0_FLT3/LPUART2_TX}
  - {pin_num: '20', peripheral: LPUART2, signal: RX, pin_signal: PTD17/FTM0_FLT2/LPUART2_RX}
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
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortD);
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTD17 (pin 20) is configured as LPUART2_RX */
    PORT_SetPinMux(BOARD_UART_RX_TGTMCU_PORT, BOARD_UART_RX_TGTMCU_PIN, kPORT_MuxAlt3);

    /* PORTE12 (pin 19) is configured as LPUART2_TX */
    PORT_SetPinMux(BOARD_UART_TX_TGTMCU_PORT, BOARD_UART_TX_TGTMCU_PIN, kPORT_MuxAlt3);
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDsPins:
- options: {callFromInitBoot: 'false', prefix: BOARD_, coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '23', peripheral: GPIOE, signal: 'GPIO, 9', pin_signal: ACMP2_IN2/DAC0_OUT/PTE9/FTM0_CH7/LPUART2_CTS, direction: OUTPUT, gpio_init_state: 'true'}
  - {pin_num: '84', peripheral: GPIOE, signal: 'GPIO, 6', pin_signal: ADC1_SE11/ACMP0_IN6/PTE6/LPSPI0_PCS2/FTM3_CH7/LPUART1_RTS, direction: OUTPUT, gpio_init_state: 'false'}
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
    /* Clock Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    gpio_pin_config_t LED_GREEN_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    /* Initialize GPIO functionality on pin PTE6 (pin 84)  */
    GPIO_PinInit(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, &LED_GREEN_config);

    gpio_pin_config_t LED_RED_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 1U
    };
    /* Initialize GPIO functionality on pin PTE9 (pin 23)  */
    GPIO_PinInit(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, &LED_RED_config);

    /* PORTE6 (pin 84) is configured as PTE6 */
    PORT_SetPinMux(BOARD_LED_GREEN_PORT, BOARD_LED_GREEN_PIN, kPORT_MuxAsGpio);

    /* PORTE9 (pin 23) is configured as PTE9 */
    PORT_SetPinMux(BOARD_LED_RED_PORT, BOARD_LED_RED_PIN, kPORT_MuxAsGpio);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
