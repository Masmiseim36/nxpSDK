/*
 * Copyright 2018-2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/*! @name PORTD0 (number 127), MB_J11[A14]/MB_U12[18]/MB_PWM_AT/J4[A14]/PWM0
  @{ */
#define MB_PWM_AT_PORT PORTD /*!<@brief PORT device name: PORTD */
#define MB_PWM_AT_PIN 0U     /*!<@brief PORTD pin index: 0 */
                             /* @} */

/*! @name PORTD1 (number 128), MB_J11[A15]/MB_U12[12]/MB_PWM_AB/J4[A15]/PWM1
  @{ */
#define MB_PWM_AB_PORT PORTD /*!<@brief PORT device name: PORTD */
#define MB_PWM_AB_PIN 1U     /*!<@brief PORTD pin index: 1 */
                             /* @} */

/*! @name PORTD2 (number 129), MB_J11[A16]/MB_U12[19]/MB_PWM_BT/J4[A16]/PWM2
  @{ */
#define MB_PWM_BT_PORT PORTD /*!<@brief PORT device name: PORTD */
#define MB_PWM_BT_PIN 2U     /*!<@brief PORTD pin index: 2 */
                             /* @} */

/*! @name PORTD3 (number 130), MB_J11[A17]/MB_U12[13]/MB_PWM_BB/J4[A17]/PWM3
  @{ */
#define MB_PWM_BB_PORT PORTD /*!<@brief PORT device name: PORTD */
#define MB_PWM_BB_PIN 3U     /*!<@brief PORTD pin index: 3 */
                             /* @} */

/*! @name PORTD4 (number 131), MB_J11[A18]/MB_U12[20]/MB_PWM_CT/J4[A18]/PWM4
  @{ */
#define MB_PWM_CT_PORT PORTD /*!<@brief PORT device name: PORTD */
#define MB_PWM_CT_PIN 4U     /*!<@brief PORTD pin index: 4 */
                             /* @} */

/*! @name PORTD5 (number 132), MB_J11[A19]/MB_U12[14]/MB_PWM_CB/J4[A19]/PWM5
  @{ */
#define MB_PWM_CB_PORT PORTD /*!<@brief PORT device name: PORTD */
#define MB_PWM_CB_PIN 5U     /*!<@brief PORTD pin index: 5 */
                             /* @} */

/*! @name PORTE1 (number 2), MB_J11[A25]/MB_U12[11]/MB_TP28/MB_FAULT/MB_FAULT_1/MB_PFC_overcurrent/J4[A25]/FAULT_1
  @{ */
#define MB_FAULT_1_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_FAULT_1_PIN 1U     /*!<@brief PORTE pin index: 1 */
                              /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitMC_PWMPins(void);

/*! @name PORTC6 (number 111), MB_J11[B21]/MB_PFC_zc_2/J4[B21]/MB_TP_27
  @{ */
#define MB_PFC_ZC_2_PORT PORTC /*!<@brief PORT device name: PORTC */
#define MB_PFC_ZC_2_PIN 6U     /*!<@brief PORTC pin index: 6 */
                               /* @} */

/*! @name PORTC7 (number 112), MB_J11[B22]/MB_PFC_zc_1/J4[B22]/MB_TP_26
  @{ */
#define MB_PFC_ZC_1_PORT PORTC /*!<@brief PORT device name: PORTC */
#define MB_PFC_ZC_1_PIN 7U     /*!<@brief PORTC pin index: 7 */
                               /* @} */

/*! @name PORTC13 (number 118), MB_J11[B28]/MB_Q3[1]/MB_Relay/J4[B28]/Relay
  @{ */
#define MB_RELAY_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define MB_RELAY_PORT PORTC /*!<@brief PORT device name: PORTC */
#define MB_RELAY_PIN 13U    /*!<@brief PORTC pin index: 13 */
                            /* @} */

/*! @name PORTC1 (number 104), MB_J11[A22]/MB_U14[4]/MB_TP15/MB_PWM_PFC2/J4[A22]/PWM8
  @{ */
#define MB_PWM_PFC2_PORT PORTC /*!<@brief PORT device name: PORTC */
#define MB_PWM_PFC2_PIN 1U     /*!<@brief PORTC pin index: 1 */
                               /* @} */

/*! @name PORTC2 (number 105), MB_J11[A23]/MB_U14[2]/MB_TP12/MB_PWM_PFC1/J4[A23]/PWM9
  @{ */
#define MB_PWM_PFC1_PORT PORTC /*!<@brief PORT device name: PORTC */
#define MB_PWM_PFC1_PIN 2U     /*!<@brief PORTC pin index: 2 */
                               /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitPFCPins(void);

/*! @name PORTE29 (number 37), MB_J11[B3]/MB_U8B[7]/MB_TP34/MB_I_sense_DCB/J4[B3]/I_dcb
  @{ */
#define MB_I_DCB_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_I_DCB_PIN 29U    /*!<@brief PORTE pin index: 29 */
                            /* @} */

/*! @name PORTE17 (number 19), MB_J11[B4]/MB_TP31/MB_V_sense_DCB/MB_DCB_Pos/J4[B4]/U_dcb
  @{ */
#define MB_U_DCB_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_U_DCB_PIN 17U    /*!<@brief PORTE pin index: 17 */
                            /* @} */

/*! @name PORTE18 (number 20), MB_J11[B5]/MB_TP33/'MB_V_sense_DCB/2'/J4[B5]/'U_dcb/2'
  @{ */
#define MB_U_DCB_HALF_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_U_DCB_HALF_PIN 18U    /*!<@brief PORTE pin index: 18 */
                                 /* @} */

/*! @name PORTE19 (number 21), MB_J11[B9]/MB_U12[2]/MB_TP38/MB_IPM_temp/J4[B9]/Temp_IPM
  @{ */
#define MB_TEMP_IPM_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_TEMP_IPM_PIN 19U    /*!<@brief PORTE pin index: 19 */
                               /* @} */

/*! @name HSADC0A_CH2 (number 27), MB_J11[A3]/MB_U11A[1]/MB_TP45/MB_I_sense_A/J4[A3]/I_phA
  @{ */
/* @} */

/*! @name HSADC0A_CH3 (number 28), MB_J11[A4]/MB_U11B[7]/MB_TP40/MB_I_sense_B/J4[A4]/I_phB
  @{ */
/* @} */

/*! @name HSADC0A_CH10 (number 29), MB_J11[A5]/MB_U8A[1]/MB_TP37/MB_I_sense_C/J4[A5]/I_phC
  @{ */
/* @} */

/*! @name PORTE30 (number 38), MB_J11[A6]/MB_TP16/MB_TACHO/J4[A6]/TACHO
  @{ */
#define MB_TACHO_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_TACHO_PIN 30U    /*!<@brief PORTE pin index: 30 */
                            /* @} */

/*! @name HSADC0A_CH11 (number 30), MB_J11[A7]/MB_TP18/MB_Vin/J4[A7]/V_in
  @{ */
/* @} */

/*! @name PORTC8 (number 113), MB_J11[A8]/MB_U2B[7]/MB_TP14/MB_Ipfc1/J4[A8]/I_pfc1
  @{ */
#define MB_I_PFC1_PORT PORTC /*!<@brief PORT device name: PORTC */
#define MB_I_PFC1_PIN 8U     /*!<@brief PORTC pin index: 8 */
                             /* @} */

/*! @name PORTC9 (number 114), MB_J11[A9]/MB_U2A[1]/MB_TP5/MB_Ipfc2/J4[A9]/I_pfc2
  @{ */
#define MB_I_PFC2_PORT PORTC /*!<@brief PORT device name: PORTC */
#define MB_I_PFC2_PIN 9U     /*!<@brief PORTC pin index: 9 */
                             /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitANA_SENSPins(void);

/*! @name PORTE3 (number 4), MB_J11[B6]/MB_TP44/MB_BEMF_sense_A/MB_Phase_A/J4[B6]/BEMF_A
  @{ */
#define MB_BEMF_A_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_BEMF_A_PIN 3U     /*!<@brief PORTE pin index: 3 */
                             /* @} */

/*! @name CMP0_IN4 (number 39), MB_J11[B6]/MB_TP44/MB_BEMF_sense_A/MB_Phase_A/J4[B6]/BEMF_A
  @{ */
/* @} */

/*! @name PORTA1 (number 51), U1A[99]/cpm0_out
  @{ */
#define MB_BEMF_A_CMP_OUT_PORT PORTA /*!<@brief PORT device name: PORTA */
#define MB_BEMF_A_CMP_OUT_PIN 1U     /*!<@brief PORTA pin index: 1 */
                                     /* @} */

/*! @name PORTB20 (number 99), U1B[51]/cmp0_out
  @{ */
#define MB_BEMF_A_CAPTURE_PORT PORTB /*!<@brief PORT device name: PORTB */
#define MB_BEMF_A_CAPTURE_PIN 20U    /*!<@brief PORTB pin index: 20 */
                                     /* @} */

/*! @name PORTE24 (number 45), MB_J11[B7]/MB_TP43/MB_BEMF_sense_B/MB_Phase_B/U1A[64]/J4[B7]/BEMF_B
  @{ */
#define MB_BEMF_B_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_BEMF_B_PIN 24U    /*!<@brief PORTE pin index: 24 */
                             /* @} */

/*! @name PORTA12 (number 64), MB_J11[B7]/MB_TP43/MB_BEMF_sense_B/MB_Phase_B/U1B[45]/J4[B7]/BEMF_B
  @{ */
#define MB_BEMF_B_CMP_IN_PORT PORTA /*!<@brief PORT device name: PORTA */
#define MB_BEMF_B_CMP_IN_PIN 12U    /*!<@brief PORTA pin index: 12 */
                                    /* @} */

/*! @name PORTA5 (number 55), U1A[100]/cmp2_out
  @{ */
#define MB_BEMF_B_CMP_OUT_PORT PORTA /*!<@brief PORT device name: PORTA */
#define MB_BEMF_B_CMP_OUT_PIN 5U     /*!<@brief PORTA pin index: 5 */
                                     /* @} */

/*! @name PORTB21 (number 100), U1B[55]/cmp2_out
  @{ */
#define MB_BEMF_B_CAPTURE_PORT PORTB /*!<@brief PORT device name: PORTB */
#define MB_BEMF_B_CAPTURE_PIN 21U    /*!<@brief PORTB pin index: 21 */
                                     /* @} */

/*! @name PORTE25 (number 46), MB_J11[B8]/MB_TP41/MB_BEMF_sense_C/MB_Phase_C/U1A[66]/J4[B8]/BEMF_C
  @{ */
#define MB_BEMF_C_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_BEMF_C_PIN 25U    /*!<@brief PORTE pin index: 25 */
                             /* @} */

/*! @name PORTA14 (number 66), MB_J11[B8]/MB_TP41/MB_BEMF_sense_C/MB_Phase_C/U1B[46]/J4[B8]/BEMF_C
  @{ */
#define MB_BEMF_C_CMP_IN_PORT PORTA /*!<@brief PORT device name: PORTA */
#define MB_BEMF_C_CMP_IN_PIN 14U    /*!<@brief PORTA pin index: 14 */
                                    /* @} */

/*! @name PORTB23 (number 102), U1A[101]/cmp3_out
  @{ */
#define MB_BEMF_C_CMP_OUT_PORT PORTB /*!<@brief PORT device name: PORTB */
#define MB_BEMF_C_CMP_OUT_PIN 23U    /*!<@brief PORTB pin index: 23 */
                                     /* @} */

/*! @name PORTB22 (number 101), U1A[102]/cmp3_out
  @{ */
#define MB_BEMF_C_CAPTURE_PORT PORTB /*!<@brief PORT device name: PORTB */
#define MB_BEMF_C_CAPTURE_PIN 22U    /*!<@brief PORTB pin index: 22 */
                                     /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitBEMFPins(void);

/*! @name PORTB16 (number 95), MB_J9[3]/MB_J11[B14]/MB_TP13/MB_ENC_PhaseA/J4[B14]/TM0
  @{ */
#define MB_ENC_PHASE_A_PORT PORTB /*!<@brief PORT device name: PORTB */
#define MB_ENC_PHASE_A_PIN 16U    /*!<@brief PORTB pin index: 16 */
                                  /* @} */

/*! @name PORTC5 (number 110), MB_J9[4]/MB_J11[B15]/MB_TP17/MB_ENC_PhaseB/J4[B15]/TM1
  @{ */
#define MB_ENC_PHASE_B_PORT PORTC /*!<@brief PORT device name: PORTC */
#define MB_ENC_PHASE_B_PIN 5U     /*!<@brief PORTC pin index: 5 */
                                  /* @} */

/*! @name PORTE0 (number 1), MB_J9[5]/MB_J11[B16]/MB_TP19/MB_ENC_Index/J4[B16]/TM2
  @{ */
#define MB_ENC_INDEX_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_ENC_INDEX_PIN 0U     /*!<@brief PORTE pin index: 0 */
                                /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitENCPins(void);

#define SOPT5_UART0RXSRC_UART_RX 0x00u /*!<@brief UART 0 receive data source select: UART0_RX pin */
#define SOPT5_UART0TXSRC_UART_TX 0x00u /*!<@brief UART 0 transmit data source select: UART0_TX pin */

/*! @name PORTD7 (number 136), MB_J1[3]/MB_J11[B26]/MB_U1[13]/MB_U3[25]/MB_TP3/MB_TP7/MB_TxD/J4[B26]/TxD
  @{ */
#define MB_TXD_PORT PORTD /*!<@brief PORT device name: PORTD */
#define MB_TXD_PIN 7U     /*!<@brief PORTD pin index: 7 */
                          /* @} */

/*! @name PORTD6 (number 133), MB_J3[2]/MB_J11[B27]/MB_U1[12]/MB_U3[26]/MB_TP2/MB_TP10/MB_RxD/J4[B27]/RxD
  @{ */
#define MB_RXD_PORT PORTD /*!<@brief PORT device name: PORTD */
#define MB_RXD_PIN 6U     /*!<@brief PORTD pin index: 6 */
                          /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitUSB_UARTPins(void);

/*! @name PORTC16 (number 123), MB_J11[B25]/MB_U9[2]/MB_TP_35/MB_MCU_BRAKE/J4[B25]/MCU_BRAKE
  @{ */
#define MB_MCU_BRAKE_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define MB_MCU_BRAKE_PORT PORTC /*!<@brief PORT device name: PORTC */
#define MB_MCU_BRAKE_PIN 16U    /*!<@brief PORTC pin index: 16 */
                                /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitBRAKEPins(void);

#define SOPT5_UART0RXSRC_UART_RX 0x00u /*!<@brief UART 0 receive data source select: UART0_RX pin */
#define SOPT5_UART0TXSRC_UART_TX 0x00u /*!<@brief UART 0 transmit data source select: UART0_TX pin */

/*! @name PORTE20 (number 25), MB_J1[1]/MB_J2[3]/MB_J11[A31]/MB_TxD_1/MB_TxD_EXT/J4[A31]/TxD1
  @{ */
#define MB_TXD_1_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_TXD_1_PIN 20U    /*!<@brief PORTE pin index: 20 */
                            /* @} */

/*! @name PORTE21 (number 26), MB_J2[4]/MB_J3[4]/MB_J11[A32]/MB_RxD_1/MB_RxD_EXT/J4[A32]/RxD1
  @{ */
#define MB_RXD_1_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_RXD_1_PIN 21U    /*!<@brief PORTE pin index: 21 */
                            /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitEXT_UARTPins(void);

#define SOPT7_HSADC0AALTTRGEN_XBAR 0x00u /*!<@brief HSADC0A alternate trigger enable: XBARA output 12. */
#define SOPT7_HSADC1ATRGSEL_XBAR 0x0Cu   /*!<@brief HSADC1A trigger select: XBARA output 41 */

/*! @name PORTA13 (number 65), MB_J11[A26]/MB_U9[2]/MB_TP35/MB_FAULT_2/MB_Over-voltage_FAULT/J4[A26]/FAULT_2
  @{ */
#define MB_FAULT_2_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define MB_FAULT_2_PORT PORTA /*!<@brief PORT device name: PORTA */
#define MB_FAULT_2_PIN 13U    /*!<@brief PORTA pin index: 13 */
                              /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitMISCPins(void);

/*! @name PORTA2 (number 52), U5[12]/J3[6]/SWO/SWO_ISOLATED
  @{ */
#define BOARD_SWO_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SWO_PIN 2U     /*!<@brief PORTA pin index: 2 */
                             /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTA18 (number 72), Y2[1]/GPIOA18/EXTAL_J
  @{ */
#define BOARD_EXTAL0_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_EXTAL0_PIN 18U    /*!<@brief PORTA pin index: 18 */
                                /* @} */

/*! @name PORTA19 (number 73), Y2[3]/GPIOA19/XTAL_J
  @{ */
#define BOARD_XTAL0_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_XTAL0_PIN 19U    /*!<@brief PORTA pin index: 19 */
                               /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitOSCPins(void);

#define SOPT5_UART1RXSRC_UART_RX 0x00u /*!<@brief UART 1 receive data source select: UART1_RX pin */
#define SOPT5_UART1TXSRC_UART_TX 0x00u /*!<@brief UART 1 transmit data source select: UART1_TX pin */

/*! @name PORTC3 (number 106), U2[25]/U8[12]/RxD_sda/UART_RX_TGTMCU
  @{ */
#define BOARD_UART_RX_TGTMCU_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_UART_RX_TGTMCU_PIN 3U     /*!<@brief PORTC pin index: 3 */
                                        /* @} */

/*! @name PORTC4 (number 109), U2[24]/U8[13]/TxD_sda/UART_TX_TGTMCU
  @{ */
#define BOARD_UART_TX_TGTMCU_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_UART_TX_TGTMCU_PIN 4U     /*!<@brief PORTC pin index: 4 */
                                        /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UARTPins(void);

/*! @name PORTB4 (number 85), USER_LED_2
  @{ */
#define BOARD_LED_RED_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOARD_LED_RED_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_LED_RED_PIN 4U     /*!<@brief PORTB pin index: 4 */
                                 /* @} */

/*! @name PORTB19 (number 98), MB_J11[B20]/MB_U9[4]/MB_USER_LED/J4[B20]/USER_LED
  @{ */
#define BOARD_LED_GREEN_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOARD_LED_GREEN_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_LED_GREEN_PIN 19U    /*!<@brief PORTB pin index: 19 */
                                   /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDSPins(void);

/*! @name PORTE5 (number 8), J9[15]/M2_PWM0
  @{ */
#define BOARD_M2_PWM0_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_M2_PWM0_PIN 5U     /*!<@brief PORTE pin index: 5 */
                                 /* @} */

/*! @name PORTE6 (number 9), J9[17]/M2_PWM1
  @{ */
#define BOARD_M2_PWM1_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_M2_PWM1_PIN 6U     /*!<@brief PORTE pin index: 6 */
                                 /* @} */

/*! @name PORTE7 (number 10), J9[19]/M2_PWM2
  @{ */
#define BOARD_M2_PWM2_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_M2_PWM2_PIN 7U     /*!<@brief PORTE pin index: 7 */
                                 /* @} */

/*! @name PORTE8 (number 11), J9[16]/M2_PWM3
  @{ */
#define BOARD_M2_PWM3_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_M2_PWM3_PIN 8U     /*!<@brief PORTE pin index: 8 */
                                 /* @} */

/*! @name PORTE9 (number 12), J9[18]/M2_PWM4
  @{ */
#define BOARD_M2_PWM4_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_M2_PWM4_PIN 9U     /*!<@brief PORTE pin index: 9 */
                                 /* @} */

/*! @name PORTE10 (number 13), J9[20]/M2_PWM5
  @{ */
#define BOARD_M2_PWM5_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_M2_PWM5_PIN 10U    /*!<@brief PORTE pin index: 10 */
                                 /* @} */

/*! @name PORTE22 (number 41), J9[13]/M2_FAULT1
  @{ */
#define BOARD_M2_FAULT1_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_M2_FAULT1_PIN 22U    /*!<@brief PORTE pin index: 22 */
                                   /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitM2_MC_PWMPins(void);

/*! @name PORTE4 (number 7), J9[1]/M2_I_phA/CON1 - M2_I_phA
  @{ */
#define BOARD_M2_I_PH_A_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_M2_I_PH_A_PIN 4U     /*!<@brief PORTE pin index: 4 */
                                   /* @} */

/*! @name PORTB6 (number 87), J9[3]/CON3 - M2_I_phB
  @{ */
#define BOARD_M2_I_PH_B_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_M2_I_PH_B_PIN 6U     /*!<@brief PORTB pin index: 6 */
                                   /* @} */

/*! @name PORTB7 (number 88), J9[5]/CON5 - M2_I_phC
  @{ */
#define BOARD_M2_I_PH_C_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_M2_I_PH_C_PIN 7U     /*!<@brief PORTB pin index: 7 */
                                   /* @} */

/*! @name PORTB0 (number 81), J9[2]/M2_BEMF_A/CON2 - M2_BEMF_A
  @{ */
#define BOARD_M2_BEMF_A_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_M2_BEMF_A_PIN 0U     /*!<@brief PORTB pin index: 0 */
                                   /* @} */

/*! @name PORTB1 (number 82), J9[4]/M2_BEMF_B/CON4 - M2_BEMF_B
  @{ */
#define BOARD_M2_BEMF_B_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_M2_BEMF_B_PIN 1U     /*!<@brief PORTB pin index: 1 */
                                   /* @} */

/*! @name PORTB3 (number 84), J9[6]/M2_BEMF_C/CON6 - M2_BEMF_C
  @{ */
#define BOARD_M2_BEMF_C_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_M2_BEMF_C_PIN 3U     /*!<@brief PORTB pin index: 3 */
                                   /* @} */

/*! @name PORTB2 (number 83), J9[8]/M2_I_dcb/CON8 - M2_I_dcb
  @{ */
#define BOARD_M2_I_DCB_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_M2_I_DCB_PIN 2U     /*!<@brief PORTB pin index: 2 */
                                  /* @} */

/*! @name HSADC0A_CH7 (number 24), J9[7]/M2_U_dcb/CON7 - M2_U_dcb
  @{ */
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitM2_ANA_SENSEPins(void);

/*! @name PORTD14 (number 143), J9[14]/M2_GPIO
  @{ */
#define BOARD_M2_GPIO_GPIO GPIOD /*!<@brief GPIO device name: GPIOD */
#define BOARD_M2_GPIO_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_M2_GPIO_PIN 14U    /*!<@brief PORTD pin index: 14 */
                                 /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitM2_MISCPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
