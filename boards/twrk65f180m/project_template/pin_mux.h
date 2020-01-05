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

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/*! @name PORTA4 (coord L9), SW2/NMI
  @{ */
#define BOARD_SW2_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_SW2_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SW2_PIN 4U     /*!<@brief PORTA pin index: 4 */
                             /* @} */

/*! @name PORTA10 (coord N11), J26[B52]/SW3
  @{ */
#define BOARD_SW3_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_SW3_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SW3_PIN 10U    /*!<@brief PORTA pin index: 10 */
                             /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitBUTTONsPins(void);

/*! @name PORTB4 (coord H9), D6/LED_1
  @{ */
#define BOARD_LED_YELLOW_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOARD_LED_YELLOW_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_LED_YELLOW_PIN 4U     /*!<@brief PORTB pin index: 4 */
                                    /* @} */

/*! @name PORTB5 (coord F13), D7/LED_2
  @{ */
#define BOARD_LED_ORANGE_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOARD_LED_ORANGE_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_LED_ORANGE_PIN 5U     /*!<@brief PORTB pin index: 5 */
                                    /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDsPins(void);

/*! @name PORTE18 (coord F6), J12[4]/J26[A8]/U15[6]/I2C0_SDA
  @{ */
#define BOARD_ACCEL_I2C_SDA_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_ACCEL_I2C_SDA_PIN 18U    /*!<@brief PORTE pin index: 18 */
                                       /* @} */

/*! @name PORTE19 (coord F7), J12[3]/J26[A7]/U15[4]/I2C0_SCL
  @{ */
#define BOARD_ACCEL_I2C_SCL_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_ACCEL_I2C_SCL_PIN 19U    /*!<@brief PORTE pin index: 19 */
                                       /* @} */

/*! @name PORTE27 (coord L8), J26[B55]/J26[B56]/U15[11]/ACCL_INT1
  @{ */
#define BOARD_ACCEL_I2C_INT1_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_ACCEL_I2C_INT1_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_ACCEL_I2C_INT1_PIN 27U    /*!<@brief PORTE pin index: 27 */
                                        /* @} */

/*! @name PORTE28 (coord M7), J26[B61]/J26[B62]/U15[9]/ACCL_INT2
  @{ */
#define BOARD_ACCEL_I2C_INT2_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_ACCEL_I2C_INT2_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_ACCEL_I2C_INT2_PIN 28U    /*!<@brief PORTE pin index: 28 */
                                        /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitACCELPins(void);

/*! @name PORTE17 (coord F5), J34[2]/U6[2]/UART2_RX
  @{ */
#define BOARD_DEBUG_UART_RX_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_DEBUG_UART_RX_PIN 17U    /*!<@brief PORTE pin index: 17 */
                                       /* @} */

/*! @name PORTE16 (coord H3), J33[2]/U5[4]/UART2_TX
  @{ */
#define BOARD_DEBUG_UART_TX_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_DEBUG_UART_TX_PIN 16U    /*!<@brief PORTE pin index: 16 */
                                       /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UARTPins(void);

/*! @name ADC1_SE16 (coord M3), J24[1]/J26[D30]/R67[2]/POTJ
  @{ */
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPOTPins(void);

/*! @name PORTB19 (coord D11), J26[B72]/D8[1]/BRD_PAD1
  @{ */
#define BOARD_TSI_ELECTRODE_2_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_TSI_ELECTRODE_2_PIN 19U    /*!<@brief PORTB pin index: 19 */
                                         /* @} */

/*! @name PORTB18 (coord D12), J26[B66]/D9[1]/BRD_PAD2
  @{ */
#define BOARD_TSI_ELECTRODE_1_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_TSI_ELECTRODE_1_PIN 18U    /*!<@brief PORTB pin index: 18 */
                                         /* @} */

/*! @name PORTA29 (coord H11), J26[A35]/D9/TP_LED2RR
  @{ */
#define BOARD_LED_BLUE_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_LED_BLUE_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_LED_BLUE_PIN 29U    /*!<@brief PORTA pin index: 29 */
                                  /* @} */

/*! @name PORTA28 (coord H12), J26[B35]/D8/TP_LED1RR
  @{ */
#define BOARD_LED_GREEN_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_LED_GREEN_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_LED_GREEN_PIN 28U    /*!<@brief PORTA pin index: 28 */
                                   /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitTOUCH_PADsPins(void);

#define PORT_DFER_DFE_2_MASK 0x04u /*!<@brief Digital Filter Enable Mask for item 2. */
#define PORT_DFER_DFE_3_MASK 0x08u /*!<@brief Digital Filter Enable Mask for item 3. */
#define PORT_DFER_DFE_4_MASK 0x10u /*!<@brief Digital Filter Enable Mask for item 4. */
#define PORT_DFER_DFE_5_MASK 0x20u /*!<@brief Digital Filter Enable Mask for item 5. */
#define PORT_DFER_DFE_7_MASK 0x80u /*!<@brief Digital Filter Enable Mask for item 7. */

/*! @name PORTD4 (coord B5), J26[A78]/U14[62]/FB_AD2/SDRAM_A10
  @{ */
#define BOARD_SDRAM_A10_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_SDRAM_A10_PIN 4U     /*!<@brief PORTD pin index: 4 */
                                   /* @} */

/*! @name PORTD3 (coord B4), J26[A77]/U14[61]/FB_AD3/SDRAM_A11
  @{ */
#define BOARD_SDRAM_A11_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_SDRAM_A11_PIN 3U     /*!<@brief PORTD pin index: 3 */
                                   /* @} */

/*! @name PORTD2 (coord A4), J26[A76]/U14[60]/FB_AD4/SDRAM_A12
  @{ */
#define BOARD_SDRAM_A12_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_SDRAM_A12_PIN 2U     /*!<@brief PORTD pin index: 2 */
                                   /* @} */

/*! @name PORTC10 (coord A8), J26[A75]/U14[27]/FB_AD5/SDRAM_A13
  @{ */
#define BOARD_SDRAM_A13_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A13_PIN 10U    /*!<@brief PORTC pin index: 10 */
                                   /* @} */

/*! @name PORTC9 (coord C9), J26[A74]/U14[26]/FB_AD6/SDRAM_A14
  @{ */
#define BOARD_SDRAM_A14_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A14_PIN 9U     /*!<@brief PORTC pin index: 9 */
                                   /* @} */

/*! @name PORTC8 (coord C10), J26[A73]/U14[25]/FB_AD7/SDRAM_A15
  @{ */
#define BOARD_SDRAM_A15_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A15_PIN 8U     /*!<@brief PORTC pin index: 8 */
                                   /* @} */

/*! @name PORTC6 (coord A10), J26[A71]/U14[64]/FB_AD9/SDRAM_A17
  @{ */
#define BOARD_SDRAM_A17_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A17_PIN 6U     /*!<@brief PORTC pin index: 6 */
                                   /* @} */

/*! @name PORTC5 (coord A11), J26[A70]/U14[65]/FB_AD10/SDRAM_A18
  @{ */
#define BOARD_SDRAM_A18_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A18_PIN 5U     /*!<@brief PORTC pin index: 5 */
                                   /* @} */

/*! @name PORTC4 (coord B11), J26[A69]/U14[66]/FB_AD11/SDRAM_A19
  @{ */
#define BOARD_SDRAM_A19_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A19_PIN 4U     /*!<@brief PORTC pin index: 4 */
                                   /* @} */

/*! @name PORTC2 (coord A13), J26[A68]/U14[24]/FB_AD12/SDRAM_A20
  @{ */
#define BOARD_SDRAM_A20_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A20_PIN 2U     /*!<@brief PORTC pin index: 2 */
                                   /* @} */

/*! @name PORTC1 (coord B12), J26[A67]/U14[22]/FB_AD13/SDRAM_A21
  @{ */
#define BOARD_SDRAM_A21_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A21_PIN 1U     /*!<@brief PORTC pin index: 1 */
                                   /* @} */

/*! @name PORTC0 (coord B13), J26[A66]/U14[23]/FB_AD14/SDRAM_A22
  @{ */
#define BOARD_SDRAM_A22_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A22_PIN 0U     /*!<@brief PORTC pin index: 0 */
                                   /* @} */

/*! @name PORTD5 (coord C4), J26[A79]/U14[63]/FB_AD1/SDRAM_A9
  @{ */
#define BOARD_SDRAM_A9_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_SDRAM_A9_PIN 5U     /*!<@brief PORTD pin index: 5 */
                                  /* @} */

/*! @name PORTB0 (coord G13), U14[18]/SDRAM_CAS
  @{ */
#define BOARD_SDRAM_CAS_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_CAS_PIN 0U     /*!<@brief PORTB pin index: 0 */
                                   /* @} */

/*! @name PORTD7 (coord E5), U14[67]/SDRAM_CKE
  @{ */
#define BOARD_SDRAM_CKE_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_SDRAM_CKE_PIN 7U     /*!<@brief PORTD pin index: 7 */
                                   /* @} */

/*! @name PORTA6 (coord M10), J26[B27]/U14[68]/CLKOUT
  @{ */
#define BOARD_SDRAM_CLKOUT_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SDRAM_CLKOUT_PIN 6U     /*!<@brief PORTA pin index: 6 */
                                      /* @} */

/*! @name PORTB3 (coord G10), U14[20]/SDRAM_CS0
  @{ */
#define BOARD_SDRAM_CS0_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_CS0_PIN 3U     /*!<@brief PORTB pin index: 3 */
                                   /* @} */

/*! @name PORTC29 (coord B6), U14[2]/SDRAM_D0
  @{ */
#define BOARD_SDRAM_D0_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_D0_PIN 29U    /*!<@brief PORTC pin index: 29 */
                                  /* @} */

/*! @name PORTC28 (coord C6), U14[4]/SDRAM_D1
  @{ */
#define BOARD_SDRAM_D1_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_D1_PIN 28U    /*!<@brief PORTC pin index: 28 */
                                  /* @} */

/*! @name PORTA31 (coord J9), J26[B41]/U14[77]/SDRAM_D10/CAN0_RX
  @{ */
#define BOARD_SDRAM_D10_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SDRAM_D10_PIN 31U    /*!<@brief PORTA pin index: 31 */
                                   /* @} */

/*! @name PORTA30 (coord H10), J26[B42]/U14[79]/SDRAM_D11/CAN0_TX
  @{ */
#define BOARD_SDRAM_D11_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SDRAM_D11_PIN 30U    /*!<@brief PORTA pin index: 30 */
                                   /* @} */

/*! @name PORTA27 (coord H13), U14[80]/SDRAM_D12
  @{ */
#define BOARD_SDRAM_D12_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SDRAM_D12_PIN 27U    /*!<@brief PORTA pin index: 27 */
                                   /* @} */

/*! @name PORTA26 (coord J10), U14[82]/SDRAM_D13
  @{ */
#define BOARD_SDRAM_D13_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SDRAM_D13_PIN 26U    /*!<@brief PORTA pin index: 26 */
                                   /* @} */

/*! @name PORTA25 (coord J11), U14[83]/SDRAM_D14
  @{ */
#define BOARD_SDRAM_D14_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SDRAM_D14_PIN 25U    /*!<@brief PORTA pin index: 25 */
                                   /* @} */

/*! @name PORTA24 (coord K11), U14[85]/SDRAM_D15
  @{ */
#define BOARD_SDRAM_D15_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SDRAM_D15_PIN 24U    /*!<@brief PORTA pin index: 24 */
                                   /* @} */

/*! @name PORTB17 (coord D13), J26[B67]/U14[31]/FB_AD16/SDRAM_D16
  @{ */
#define BOARD_SDRAM_D16_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D16_PIN 17U    /*!<@brief PORTB pin index: 17 */
                                   /* @} */

/*! @name PORTB16 (coord F8), J26[B68]/U14[33]/FB_AD17/SDRAM_D17
  @{ */
#define BOARD_SDRAM_D17_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D17_PIN 16U    /*!<@brief PORTB pin index: 16 */
                                   /* @} */

/*! @name PORTB11 (coord E13), J26[B69]/U14[34]/FB_AD18/SDRAM_D18
  @{ */
#define BOARD_SDRAM_D18_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D18_PIN 11U    /*!<@brief PORTB pin index: 11 */
                                   /* @} */

/*! @name PORTB10 (coord G9), J26[B70]/U14[36]/FB_AD19/SDRAM_D19
  @{ */
#define BOARD_SDRAM_D19_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D19_PIN 10U    /*!<@brief PORTB pin index: 10 */
                                   /* @} */

/*! @name PORTC27 (coord D6), U14[5]/SDRAM_D2
  @{ */
#define BOARD_SDRAM_D2_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_D2_PIN 27U    /*!<@brief PORTC pin index: 27 */
                                  /* @} */

/*! @name PORTB9 (coord F9), U14[37]/SDRAM_D20
  @{ */
#define BOARD_SDRAM_D20_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D20_PIN 9U     /*!<@brief PORTB pin index: 9 */
                                   /* @} */

/*! @name PORTB8 (coord F10), U14[39]/SDRAM_D21
  @{ */
#define BOARD_SDRAM_D21_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D21_PIN 8U     /*!<@brief PORTB pin index: 8 */
                                   /* @} */

/*! @name PORTB7 (coord F11), U14[40]/SDRAM_D22
  @{ */
#define BOARD_SDRAM_D22_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D22_PIN 7U     /*!<@brief PORTB pin index: 7 */
                                   /* @} */

/*! @name PORTB6 (coord F12), U14[42]/SDRAM_D23
  @{ */
#define BOARD_SDRAM_D23_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D23_PIN 6U     /*!<@brief PORTB pin index: 6 */
                                   /* @} */

/*! @name PORTC15 (coord D8), J26[B80]/U14[45]/FB_AD24/SDRAM_D24
  @{ */
#define BOARD_SDRAM_D24_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_D24_PIN 15U    /*!<@brief PORTC pin index: 15 */
                                   /* @} */

/*! @name PORTC14 (coord C8), J26[B79]/U14[47]/FB_AD25/SDRAM_D25
  @{ */
#define BOARD_SDRAM_D25_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_D25_PIN 14U    /*!<@brief PORTC pin index: 14 */
                                   /* @} */

/*! @name PORTC13 (coord B8), J26[B78]/U14[48]/FB_AD26/SDRAM_D26
  @{ */
#define BOARD_SDRAM_D26_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_D26_PIN 13U    /*!<@brief PORTC pin index: 13 */
                                   /* @} */

/*! @name PORTC12 (coord B9), J26[B77]/U14[50]/FB_AD27/SDRAM_D27
  @{ */
#define BOARD_SDRAM_D27_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_D27_PIN 12U    /*!<@brief PORTC pin index: 12 */
                                   /* @} */

/*! @name PORTB23 (coord C12), J26[B76]/U14[51]/FB_AD28/SDRAM_D28
  @{ */
#define BOARD_SDRAM_D28_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D28_PIN 23U    /*!<@brief PORTB pin index: 23 */
                                   /* @} */

/*! @name PORTB22 (coord C13), J26[B75]/U14[53]/FB_AD29/SDRAM_D29
  @{ */
#define BOARD_SDRAM_D29_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D29_PIN 22U    /*!<@brief PORTB pin index: 22 */
                                   /* @} */

/*! @name PORTC26 (coord E6), U14[7]/SDRAM_D3
  @{ */
#define BOARD_SDRAM_D3_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_D3_PIN 26U    /*!<@brief PORTC pin index: 26 */
                                  /* @} */

/*! @name PORTB21 (coord D9), J26[B74]/U14[54]/FB_AD30/SDRAM_D30
  @{ */
#define BOARD_SDRAM_D30_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D30_PIN 21U    /*!<@brief PORTB pin index: 21 */
                                   /* @} */

/*! @name PORTB20 (coord D10), J26[B73]/U14[56]/FB_AD31/SDRAM_D31
  @{ */
#define BOARD_SDRAM_D31_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D31_PIN 20U    /*!<@brief PORTB pin index: 20 */
                                   /* @} */

/*! @name PORTC25 (coord A7), U14[8]/SDRAM_D4
  @{ */
#define BOARD_SDRAM_D4_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_D4_PIN 25U    /*!<@brief PORTC pin index: 25 */
                                  /* @} */

/*! @name PORTC24 (coord B7), U14[10]/SDRAM_D5
  @{ */
#define BOARD_SDRAM_D5_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_D5_PIN 24U    /*!<@brief PORTC pin index: 24 */
                                  /* @} */

/*! @name PORTB15 (coord E9), U14[11]/SDRAM_D6
  @{ */
#define BOARD_SDRAM_D6_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D6_PIN 15U    /*!<@brief PORTB pin index: 15 */
                                  /* @} */

/*! @name PORTB14 (coord E10), U14[13]/SDRAM_D7
  @{ */
#define BOARD_SDRAM_D7_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D7_PIN 14U    /*!<@brief PORTB pin index: 14 */
                                  /* @} */

/*! @name PORTB13 (coord E11), U14[74]/SDRAM_D8
  @{ */
#define BOARD_SDRAM_D8_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D8_PIN 13U    /*!<@brief PORTB pin index: 13 */
                                  /* @} */

/*! @name PORTB12 (coord E12), U14[76]/SDRAM_D9
  @{ */
#define BOARD_SDRAM_D9_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D9_PIN 12U    /*!<@brief PORTB pin index: 12 */
                                  /* @} */

/*! @name PORTC19 (coord C7), U14[16]/SDRAM_DQM0
  @{ */
#define BOARD_SDRAM_DQM0_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_DQM0_PIN 19U    /*!<@brief PORTC pin index: 19 */
                                    /* @} */

/*! @name PORTC18 (coord D7), U14[71]/SDRAM_DQM1
  @{ */
#define BOARD_SDRAM_DQM1_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_DQM1_PIN 18U    /*!<@brief PORTC pin index: 18 */
                                    /* @} */

/*! @name PORTC16 (coord E8), U14[28]/SDRAM_DQM2
  @{ */
#define BOARD_SDRAM_DQM2_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_DQM2_PIN 16U    /*!<@brief PORTC pin index: 16 */
                                    /* @} */

/*! @name PORTC17 (coord E7), U14[59]/SDRAM_DQM3
  @{ */
#define BOARD_SDRAM_DQM3_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_DQM3_PIN 17U    /*!<@brief PORTC pin index: 17 */
                                    /* @} */

/*! @name PORTB1 (coord G12), U14[19]/SDRAM_RAS
  @{ */
#define BOARD_SDRAM_RAS_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_RAS_PIN 1U     /*!<@brief PORTB pin index: 1 */
                                   /* @} */

/*! @name PORTB2 (coord G11), U14[17]/SDRAM_WE
  @{ */
#define BOARD_SDRAM_WE_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_WE_PIN 2U     /*!<@brief PORTB pin index: 2 */
                                  /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSDRAMPins(void);

/*!
 * @brief Digital Filter Enable Mask for item 15. */
#define PORT_DFER_DFE_15_MASK 0x8000u
/*!
 * @brief Digital Filter Enable Mask for item 8. */
#define PORT_DFER_DFE_8_MASK 0x0100u
/*!
 * @brief Digital Filter Enable Mask for item 9. */
#define PORT_DFER_DFE_9_MASK 0x0200u
/*!
 * @brief
 * Selects the default input voltage source to the USB Regulator in case both VREG_IN0 and VREG_IN1 are powered.
 * If only one of the regulator inputs is powered, it will automatically be selected by the regulator's power mux
 * circuitry.
 * : VREG_IN0 will be selected if both regulator inputs are powered.
 */
#define USBPHYCTL_USBVREGSEL_VREG_IN0 0x00u

/*! @name PORTD8 (coord D5), J13[2]/U12[3]/microUSB_PWR_EN
  @{ */
#define BOARD_microUSB_PWR_EN_GPIO GPIOD /*!<@brief GPIO device name: GPIOD */
#define BOARD_microUSB_PWR_EN_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_microUSB_PWR_EN_PIN 8U     /*!<@brief PORTD pin index: 8 */
                                         /* @} */

/*! @name PORTD15 (coord A3), J12[11]/J15[4]/J26[B47]/microUSB_ID/SPI2_PCS1
  @{ */
#define BOARD_microUSB_ID_GPIO GPIOD /*!<@brief GPIO device name: GPIOD */
#define BOARD_microUSB_ID_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_microUSB_ID_PIN 15U    /*!<@brief PORTD pin index: 15 */
                                     /* @} */

/*! @name USB1_DP (coord J1), J15[3]/MCU_USB1_HS_DP
  @{ */
/* @} */

/*! @name USB1_DM (coord H1), J15[2]/MCU_USB1_HS_DN
  @{ */
/* @} */

/*! @name USB1_VBUS (coord J2), J15[1]/U12[6]/K65_USB1_VBUS
  @{ */
/* @} */

/*! @name VREG_IN0 (coord F2), J23[2]/MCU_VREGIN0
  @{ */
/* @} */

/*! @name VREG_OUT (coord G2), J9[7]/MCU_VREGOUT
  @{ */
/* @} */

/*! @name PORTD9 (coord D4), J14[1]/U12[4]/K65_USB_FLGA
  @{ */
#define BOARD_K65_USB_FLGA_GPIO GPIOD /*!<@brief GPIO device name: GPIOD */
#define BOARD_K65_USB_FLGA_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_K65_USB_FLGA_PIN 9U     /*!<@brief PORTD pin index: 9 */
                                      /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitUSBPins(void);

/*! @name PORTE3 (coord D1), J26[B10]/J29[3]/SDHC0_CMD/SPI1_SIN
  @{ */
#define BOARD_SDHC_CMD_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_SDHC_CMD_PIN 3U     /*!<@brief PORTE pin index: 3 */
                                  /* @} */

/*! @name PORTE1 (coord B1), J26[B11]/J29[7]/SDHC0_D0/SPI1_SOUT
  @{ */
#define BOARD_SDHC_D0_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_SDHC_D0_PIN 1U     /*!<@brief PORTE pin index: 1 */
                                 /* @} */

/*! @name PORTE0 (coord A1), J26[B22]/J29[8]/SDHC0_D1
  @{ */
#define BOARD_SDHC_D1_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_SDHC_D1_PIN 0U     /*!<@brief PORTE pin index: 0 */
                                 /* @} */

/*! @name PORTE5 (coord D2), J26[A10]/J29[1]/SDHC0_D2
  @{ */
#define BOARD_SDHC_D2_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_SDHC_D2_PIN 5U     /*!<@brief PORTE pin index: 5 */
                                 /* @} */

/*! @name PORTE4 (coord E1), J26[B9]/J29[2]/SDHC0_D3/SPI1_PCS0
  @{ */
#define BOARD_SDHC_D3_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_SDHC_D3_PIN 4U     /*!<@brief PORTE pin index: 4 */
                                 /* @} */

/*! @name PORTE2 (coord C1), J26[B7]/J29[5]/SDHC0_DCLK/SPI1_SCK
  @{ */
#define BOARD_SDHC_DCLK_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_SDHC_DCLK_PIN 2U     /*!<@brief PORTE pin index: 2 */
                                   /* @} */

/*! @name PORTA9 (coord K10), J26[A11]/J29[9]/SD_DETECT
  @{ */
#define BOARD_SDHC_CD_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_SDHC_CD_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SDHC_CD_PIN 9U     /*!<@brief PORTA pin index: 9 */
                                 /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSDHCPins(void);

#define SOPT2_RMIISRC_ENET 0x01u /*!<@brief RMII clock source select: External bypass clock (ENET_1588_CLKIN). */

/*! @name PORTE26 (coord K8), J26[B24]/ENET_1588_CLKIN
  @{ */
#define BOARD_ENET_1588_CLKIN_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_ENET_1588_CLKIN_PIN 26U    /*!<@brief PORTE pin index: 26 */
                                         /* @} */

/*! @name PORTE26 (coord K8), J26[B24]/ENET_1588_CLKIN
  @{ */
#define BOARD_RMII_CLKIN_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_RMII_CLKIN_PIN 26U    /*!<@brief PORTE pin index: 26 */
                                    /* @} */

/*! @name PORTA8 (coord K9), J26[A13]/RMII0_MDC
  @{ */
#define BOARD_RMII0_MDC_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_MDC_PIN 8U     /*!<@brief PORTA pin index: 8 */
                                   /* @} */

/*! @name PORTA7 (coord L10), J26[A14]/RMII0_MDIO
  @{ */
#define BOARD_RMII0_MDIO_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_MDIO_PIN 7U     /*!<@brief PORTA pin index: 7 */
                                    /* @} */

/*! @name PORTA5 (coord N10), J26[B13]/RMII0_RXER
  @{ */
#define BOARD_RMII0_RXER_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_RXER_PIN 5U     /*!<@brief PORTA pin index: 5 */
                                    /* @} */

/*! @name PORTA13 (coord L11), J26[A20]/RMII0_RXD0
  @{ */
#define BOARD_RMII0_RXD0_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_RXD0_PIN 13U    /*!<@brief PORTA pin index: 13 */
                                    /* @} */

/*! @name PORTA17 (coord J12), J26[B19]/RMII0_TXD1
  @{ */
#define BOARD_RMII0_TXD1_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_TXD1_PIN 17U    /*!<@brief PORTA pin index: 17 */
                                    /* @} */

/*! @name PORTA15 (coord K12), J26[B15]/RMII0_TXEN
  @{ */
#define BOARD_RMII0_TXEN_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_TXEN_PIN 15U    /*!<@brief PORTA pin index: 15 */
                                    /* @} */

/*! @name PORTA12 (coord L12), J26[A19]/RMII0_RXD1
  @{ */
#define BOARD_RMII0_RXD1_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_RXD1_PIN 12U    /*!<@brief PORTA pin index: 12 */
                                    /* @} */

/*! @name PORTA16 (coord J13), J26[B20]/RMII0_TXD0
  @{ */
#define BOARD_RMII0_TXD0_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_TXD0_PIN 16U    /*!<@brief PORTA pin index: 16 */
                                    /* @} */

/*! @name PORTA14 (coord K13), J26[A16]/RMII0_CRS_DV
  @{ */
#define BOARD_RMII0_CRS_DV_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_RMII0_CRS_DV_PIN 14U    /*!<@brief PORTA pin index: 14 */
                                      /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitENETPins(void);

/*! @name XTAL32 (coord N5), Y1[1]/XTAL32
  @{ */
/* @} */

/*! @name EXTAL32 (coord N6), Y1[2]/EXTAL32
  @{ */
/* @} */

/*! @name PORTA19 (coord M13), Y2[1]/XTAL0
  @{ */
#define BOARD_XTAL0_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_XTAL0_PIN 19U    /*!<@brief PORTA pin index: 19 */
                               /* @} */

/*! @name PORTA18 (coord N13), Y2[3]/EXTAL0
  @{ */
#define BOARD_EXTAL0_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_EXTAL0_PIN 18U    /*!<@brief PORTA pin index: 18 */
                                /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitOSCsPins(void);

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
