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

/*! @name PORTB12 (number 68), MB_J11[A14]/MB_U12[18]/MB_PWM_AT/J4[A14]/PWM0
  @{ */
#define MB_PWM_AT_PORT PORTB /*!<@brief PORT device name: PORTB */
#define MB_PWM_AT_PIN 12U    /*!<@brief PORTB pin index: 12 */
                             /* @} */

/*! @name PORTB13 (number 67), MB_J11[A15]/MB_U12[12]/MB_PWM_AB/J4[A15]/PWM1
  @{ */
#define MB_PWM_AB_PORT PORTB /*!<@brief PORT device name: PORTB */
#define MB_PWM_AB_PIN 13U    /*!<@brief PORTB pin index: 13 */
                             /* @} */

/*! @name PORTB14 (number 66), MB_J11[A16]/MB_U12[19]/MB_PWM_BT/J4[A16]/PWM2
  @{ */
#define MB_PWM_BT_PORT PORTB /*!<@brief PORT device name: PORTB */
#define MB_PWM_BT_PIN 14U    /*!<@brief PORTB pin index: 14 */
                             /* @} */

/*! @name PORTB15 (number 65), MB_J11[A17]/MB_U12[13]/MB_PWM_BB/J4[A17]/PWM3
  @{ */
#define MB_PWM_BB_PORT PORTB /*!<@brief PORT device name: PORTB */
#define MB_PWM_BB_PIN 15U    /*!<@brief PORTB pin index: 15 */
                             /* @} */

/*! @name PORTB16 (number 64), MB_J11[A18]/MB_U12[20]/MB_PWM_CT/J4[A18]/PWM4
  @{ */
#define MB_PWM_CT_PORT PORTB /*!<@brief PORT device name: PORTB */
#define MB_PWM_CT_PIN 16U    /*!<@brief PORTB pin index: 16 */
                             /* @} */

/*! @name PORTB17 (number 63), MB_J11[A19]/MB_U12[14]/MB_PWM_CB/J4[A19]/PWM5
  @{ */
#define MB_PWM_CB_PORT PORTB /*!<@brief PORT device name: PORTB */
#define MB_PWM_CB_PIN 17U    /*!<@brief PORTB pin index: 17 */
                             /* @} */

/*! @name PORTE3 (number 18), MB_J11[A25]/MB_U12[11]/MB_TP28/MB_FAULT/MB_FAULT_1/MB_PFC_overcurrent/J4[A25]/FAULT_1
  @{ */
#define MB_FAULT_1_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_FAULT_1_PIN 3U     /*!<@brief PORTE pin index: 3 */
                              /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitMC_PWMPins(void);

/*! @name PORTA6 (number 58), MB_J11[B21]/MB_PFC_zc_2/J4[B21]/MB_TP_27
  @{ */
#define MB_PFC_ZC_2_PORT PORTA /*!<@brief PORT device name: PORTA */
#define MB_PFC_ZC_2_PIN 6U     /*!<@brief PORTA pin index: 6 */
                               /* @} */

/*! @name PORTA7 (number 57), MB_J11[B22]/MB_PFC_zc_1/J4[B22]/MB_TP_26
  @{ */
#define MB_PFC_ZC_1_PORT PORTA /*!<@brief PORT device name: PORTA */
#define MB_PFC_ZC_1_PIN 7U     /*!<@brief PORTA pin index: 7 */
                               /* @} */

/*! @name PORTE2 (number 85), MB_J11[B28]/MB_Q3[1]/MB_Relay/J4[B28]/Relay
  @{ */
#define MB_RELAY_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define MB_RELAY_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_RELAY_PIN 2U     /*!<@brief PORTE pin index: 2 */
                            /* @} */

/*! @name PORTB2 (number 48), MB_J11[A22]/MB_U14[4]/MB_TP15/MB_PWM_PFC2/J4[A22]/PWM8
  @{ */
#define MB_PWM_PFC2_PORT PORTB /*!<@brief PORT device name: PORTB */
#define MB_PWM_PFC2_PIN 2U     /*!<@brief PORTB pin index: 2 */
                               /* @} */

/*! @name PORTD8 (number 42), MB_J11[A23]/MB_U14[2]/MB_TP12/MB_PWM_PFC1/J4[A23]/PWM9
  @{ */
#define MB_PWM_PFC1_PORT PORTD /*!<@brief PORT device name: PORTD */
#define MB_PWM_PFC1_PIN 8U     /*!<@brief PORTD pin index: 8 */
                               /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitPFCPins(void);

/*! @name PORTE7 (number 59), MB_J11[B3]/MB_U8B[7]/MB_TP34/MB_I_sense_DCB/J4[B3]/I_dcb
  @{ */
#define MB_I_DCB_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_I_DCB_PIN 7U     /*!<@brief PORTE pin index: 7 */
                            /* @} */

/*! @name PORTC7 (number 80), MB_J11[B4]/MB_TP31/MB_V_sense_DCB/MB_DCB_Pos/J4[B4]/U_dcb
  @{ */
#define MB_U_DCB_PORT PORTC /*!<@brief PORT device name: PORTC */
#define MB_U_DCB_PIN 7U     /*!<@brief PORTC pin index: 7 */
                            /* @} */

/*! @name PORTE10 (number 6), MB_J11[B5]/MB_TP33/'MB_V_sense_DCB/2'/J4[B5]/'U_dcb/2'
  @{ */
#define MB_U_DCB_HALF_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_U_DCB_HALF_PIN 10U    /*!<@brief PORTE pin index: 10 */
                                 /* @} */

/*! @name PORTD4 (number 69), MB_J11[B9]/MB_U12[2]/MB_TP38/MB_IPM_temp/J4[B9]/Temp_IPM
  @{ */
#define MB_TEMP_IPM_PORT PORTD /*!<@brief PORT device name: PORTD */
#define MB_TEMP_IPM_PIN 4U     /*!<@brief PORTD pin index: 4 */
                               /* @} */

/*! @name PORTB1 (number 53), MB_J11[A5]/MB_U8A[1]/MB_TP37/MB_I_sense_C/J4[A5]/I_phC
  @{ */
#define MB_I_PH_C_PORT PORTB /*!<@brief PORT device name: PORTB */
#define MB_I_PH_C_PIN 1U     /*!<@brief PORTB pin index: 1 */
                             /* @} */

/*! @name PORTB0 (number 54), MB_J11[A4]/MB_U11B[7]/MB_TP40/MB_I_sense_B/J4[A4]/I_phB
  @{ */
#define MB_I_PH_B_PORT PORTB /*!<@brief PORT device name: PORTB */
#define MB_I_PH_B_PIN 0U     /*!<@brief PORTB pin index: 0 */
                             /* @} */

/*! @name PORTA0 (number 79), MB_J11[A3]/MB_U11A[1]/MB_TP45/MB_I_sense_A/J4[A3]/I_phA
  @{ */
#define MB_I_PH_A_PORT PORTA /*!<@brief PORT device name: PORTA */
#define MB_I_PH_A_PIN 0U     /*!<@brief PORTA pin index: 0 */
                             /* @} */

/*! @name PORTC3 (number 29), MB_J11[A6]/MB_TP16/MB_TACHO/J4[A6]/TACHO
  @{ */
#define MB_TACHO_PORT PORTC /*!<@brief PORT device name: PORTC */
#define MB_TACHO_PIN 3U     /*!<@brief PORTC pin index: 3 */
                            /* @} */

/*! @name PORTC6 (number 81), MB_J11[A7]/MB_TP18/MB_Vin/J4[A7]/V_in
  @{ */
#define MB_V_IN_PORT PORTC /*!<@brief PORT device name: PORTC */
#define MB_V_IN_PIN 6U     /*!<@brief PORTC pin index: 6 */
                           /* @} */

/*! @name PORTC15 (number 45), MB_J11[A8]/MB_U2B[7]/MB_TP14/MB_Ipfc1/J4[A8]/I_pfc1
  @{ */
#define MB_I_PFC1_PORT PORTC /*!<@brief PORT device name: PORTC */
#define MB_I_PFC1_PIN 15U    /*!<@brief PORTC pin index: 15 */
                             /* @} */

/*! @name PORTC14 (number 46), MB_J11[A9]/MB_U2A[1]/MB_TP5/MB_Ipfc2/J4[A9]/I_pfc2
  @{ */
#define MB_I_PFC2_PORT PORTC /*!<@brief PORT device name: PORTC */
#define MB_I_PFC2_PIN 14U    /*!<@brief PORTC pin index: 14 */
                             /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitANA_SENSPins(void);

/*! @name PORTD1 (number 3), MB_J11[B6]/MB_TP44/MB_BEMF_sense_A/MB_Phase_A/J4[B6]/BEMF_A
  @{ */
#define MB_BEMF_A_PORT PORTD /*!<@brief PORT device name: PORTD */
#define MB_BEMF_A_PIN 1U     /*!<@brief PORTD pin index: 1 */
                             /* @} */

/*! @name PORTD0 (number 4), MB_J11[B7]/MB_TP43/MB_BEMF_sense_B/MB_Phase_B/J4[B7]/BEMF_B
  @{ */
#define MB_BEMF_B_PORT PORTD /*!<@brief PORT device name: PORTD */
#define MB_BEMF_B_PIN 0U     /*!<@brief PORTD pin index: 0 */
                             /* @} */

/*! @name PORTE11 (number 5), MB_J11[B8]/MB_TP41/MB_BEMF_sense_C/MB_Phase_C/J4[B8]/BEMF_C
  @{ */
#define MB_BEMF_C_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_BEMF_C_PIN 11U    /*!<@brief PORTE pin index: 11 */
                             /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitBEMFPins(void);

/*! @name PORTE5 (number 8), MB_J9[3]/MB_J11[B14]/MB_TP13/MB_ENC_PhaseA/J4[B14]/TM0
  @{ */
#define MB_ENC_PHASE_A_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_ENC_PHASE_A_PIN 5U     /*!<@brief PORTE pin index: 5 */
                                  /* @} */

/*! @name PORTE4 (number 9), MB_J9[4]/MB_J11[B15]/MB_TP17/MB_ENC_PhaseB/J4[B15]/TM1
  @{ */
#define MB_ENC_PHASE_B_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_ENC_PHASE_B_PIN 4U     /*!<@brief PORTE pin index: 4 */
                                  /* @} */

/*! @name PORTD13 (number 25), MB_J9[5]/MB_J11[B16]/MB_TP19/MB_ENC_Index/J4[B16]/TM2
  @{ */
#define MB_ENC_INDEX_GPIO GPIOD /*!<@brief GPIO device name: GPIOD */
#define MB_ENC_INDEX_PORT PORTD /*!<@brief PORT device name: PORTD */
#define MB_ENC_INDEX_PIN 13U    /*!<@brief PORTD pin index: 13 */
                                /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitENCPins(void);

/*! @name PORTA3 (number 72), MB_J1[3]/MB_J11[B26]/MB_U1[13]/MB_U3[25]/MB_TP3/MB_TP7/MB_TxD/J4[B26]/TxD
  @{ */
#define MB_TXD_PORT PORTA /*!<@brief PORT device name: PORTA */
#define MB_TXD_PIN 3U     /*!<@brief PORTA pin index: 3 */
                          /* @} */

/*! @name PORTA2 (number 73), MB_J3[2]/MB_J11[B27]/MB_U1[12]/MB_U3[26]/MB_TP2/MB_TP10/MB_RxD/J4[B27]/RxD
  @{ */
#define MB_RXD_PORT PORTA /*!<@brief PORT device name: PORTA */
#define MB_RXD_PIN 2U     /*!<@brief PORTA pin index: 2 */
                          /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitUSB_UARTPins(void);

/*! @name PORTC9 (number 55), MB_J1[1]/MB_J2[3]/MB_J11[A31]/MB_TxD_1/MB_TxD_EXT/J4[A31]/TxD1
  @{ */
#define MB_TXD_1_PORT PORTC /*!<@brief PORT device name: PORTC */
#define MB_TXD_1_PIN 9U     /*!<@brief PORTC pin index: 9 */
                            /* @} */

/*! @name PORTC8 (number 56), MB_J2[4]/MB_J3[4]/MB_J11[A32]/MB_RxD_1/MB_RxD_EXT/J4[A32]/RxD1
  @{ */
#define MB_RXD_1_PORT PORTC /*!<@brief PORT device name: PORTC */
#define MB_RXD_1_PIN 8U     /*!<@brief PORTC pin index: 8 */
                            /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitEXT_UARTPins(void);

/*! @name PORTE8 (number 26), MB_J11[B25]/MB_U9[2]/MB_TP_35/MB_MCU_BRAKE/J4[B25]/MCU_BRAKE
  @{ */
#define MB_MCU_BRAKE_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define MB_MCU_BRAKE_PORT PORTE /*!<@brief PORT device name: PORTE */
#define MB_MCU_BRAKE_PIN 8U     /*!<@brief PORTE pin index: 8 */
                                /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitBRAKEPins(void);

#define ADCOPT_ADC0PRETRGSEL_PDB 0x00u /*!<@brief ADC0 pre-trigger source select: PDB pre-trigger (default) */
#define ADCOPT_ADC0TRGSEL_PDB 0x00u    /*!<@brief ADC0 trigger source select: PDB output */
#define ADCOPT_ADC1TRGSEL_PDB 0x00u    /*!<@brief ADC1 trigger source select: PDB output */

/*! @name PORTA14 (number 88), MB_J11[A26]/MB_U9[2]/MB_TP35/MB_FAULT_2/MB_Over-voltage_FAULT/J4[A26]/FAULT_2
  @{ */
#define MB_FAULT_2_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define MB_FAULT_2_PORT PORTA /*!<@brief PORT device name: PORTA */
#define MB_FAULT_2_PIN 14U    /*!<@brief PORTA pin index: 14 */
                              /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void MB_InitMISCPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTE12 (number 19), U2[24]/U6[13]/TxD_sda
  @{ */
#define BOARD_UART_TX_TGTMCU_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_UART_TX_TGTMCU_PIN 12U    /*!<@brief PORTE pin index: 12 */
                                        /* @} */

/*! @name PORTD17 (number 20), U2[25]/U6[14]/RxD_sda
  @{ */
#define BOARD_UART_RX_TGTMCU_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_UART_RX_TGTMCU_PIN 17U    /*!<@brief PORTD pin index: 17 */
                                        /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UARTPins(void);

/*! @name PORTE9 (number 23), USER_LED_2
  @{ */
#define BOARD_LED_RED_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_LED_RED_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_LED_RED_PIN 9U     /*!<@brief PORTE pin index: 9 */
                                 /* @} */

/*! @name PORTE6 (number 84), MB_J11[B20]/MB_U9[4]/MB_USER_LED/J4[B20]/USER_LED
  @{ */
#define BOARD_LED_GREEN_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_LED_GREEN_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_LED_GREEN_PIN 6U     /*!<@brief PORTE pin index: 6 */
                                   /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDsPins(void);

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
