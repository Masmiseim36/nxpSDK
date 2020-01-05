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

/*! @name PORTA4 (coord J8), J12[4]/SW2/NMI/TSI0_CH5
  @{ */
#define BOARD_SW2_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_SW2_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SW2_PIN 4U     /*!<@brief PORTA pin index: 4 */
                             /* @} */

/*! @name PORTA21 (coord J5), J7[10]/J13[C39]/SW3/FXIO0_D9/LLWU_P21/EWM_IN
  @{ */
#define BOARD_SW3_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_SW3_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SW3_PIN 21U    /*!<@brief PORTA pin index: 21 */
                             /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitBUTTONsPins(void);

#define PORT_DFER_DFE_11_MASK 0x0800u /*!<@brief Digital Filter Enable Mask for item 11. */
#define PORT_DFER_DFE_12_MASK 0x1000u /*!<@brief Digital Filter Enable Mask for item 12. */
#define PORT_DFER_DFE_13_MASK 0x2000u /*!<@brief Digital Filter Enable Mask for item 13. */

/*! @name PORTD12 (coord F4), D2/LED_J_PTD12
  @{ */
#define BOARD_LED_GREEN_GPIO GPIOD /*!<@brief GPIO device name: GPIOD */
#define BOARD_LED_GREEN_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_LED_GREEN_PIN 12U    /*!<@brief PORTD pin index: 12 */
                                   /* @} */

/*! @name PORTD13 (coord G3), D3/LED_J_PTD13
  @{ */
#define BOARD_LED_BLUE_GPIO GPIOD /*!<@brief GPIO device name: GPIOD */
#define BOARD_LED_BLUE_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_LED_BLUE_PIN 13U    /*!<@brief PORTD pin index: 13 */
                                  /* @} */

/*! @name PORTD11 (coord E3), D1/LED_J_PTD11
  @{ */
#define BOARD_LED_RED_GPIO GPIOD /*!<@brief GPIO device name: GPIOD */
#define BOARD_LED_RED_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_LED_RED_PIN 11U    /*!<@brief PORTD pin index: 11 */
                                 /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDsPins(void);

/*! @name PORTE0 (coord B1), U23[7]/QSPI0A_DATA3
  @{ */
#define BOARD_QSPI0A_DATA3_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPI0A_DATA3_PIN 0U     /*!<@brief PORTE pin index: 0 */
                                      /* @} */

/*! @name PORTE1 (coord C2), U23[6]/QSPI0A_SCLK
  @{ */
#define BOARD_QSPI0A_SCLK_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPI0A_SCLK_PIN 1U     /*!<@brief PORTE pin index: 1 */
                                     /* @} */

/*! @name PORTE2 (coord C1), U23[5]/QSPI0A_DATA0
  @{ */
#define BOARD_QSPI0A_DATA0_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPI0A_DATA0_PIN 2U     /*!<@brief PORTE pin index: 2 */
                                      /* @} */

/*! @name PORTE3 (coord D2), U23[3]/QSPI0A_DATA2
  @{ */
#define BOARD_QSPI0A_DATA2_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPI0A_DATA2_PIN 3U     /*!<@brief PORTE pin index: 3 */
                                      /* @} */

/*! @name PORTE4 (coord D1), U23[2]/QSPI0A_DATA1
  @{ */
#define BOARD_QSPI0A_DATA1_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPI0A_DATA1_PIN 4U     /*!<@brief PORTE pin index: 4 */
                                      /* @} */

/*! @name PORTE5 (coord E2), U23[1]/QSPI0A_SS0
  @{ */
#define BOARD_QSPI0A_SS0_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPI0A_SS0_PIN 5U     /*!<@brief PORTE pin index: 5 */
                                    /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitQSPI0APins(void);

/*! @name PORTE6 (coord E1), U22[7]/QSPI0B_DATA3
  @{ */
#define BOARD_QSPI0B_DATA3_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPI0B_DATA3_PIN 6U     /*!<@brief PORTE pin index: 6 */
                                      /* @} */

/*! @name PORTE7 (coord F3), U22[6]/QSPI0B_SCLK
  @{ */
#define BOARD_QSPI0B_SCLK_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPI0B_SCLK_PIN 7U     /*!<@brief PORTE pin index: 7 */
                                     /* @} */

/*! @name PORTE8 (coord F2), U22[5]/QSPI0B_DATA0
  @{ */
#define BOARD_QSPI0B_DATA0_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPI0B_DATA0_PIN 8U     /*!<@brief PORTE pin index: 8 */
                                      /* @} */

/*! @name PORTE9 (coord F1), U22[3]/QSPI0B_DATA2
  @{ */
#define BOARD_QSPI0B_DATA2_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPI0B_DATA2_PIN 9U     /*!<@brief PORTE pin index: 9 */
                                      /* @} */

/*! @name PORTE10 (coord G2), U22[2]/QSPI0B_DATA1
  @{ */
#define BOARD_QSPI0B_DATA1_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPI0B_DATA1_PIN 10U    /*!<@brief PORTE pin index: 10 */
                                      /* @} */

/*! @name PORTE11 (coord G1), U22[1]/QSPI0B_SS0
  @{ */
#define BOARD_QSPI0B_SS0_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPI0B_SS0_PIN 11U    /*!<@brief PORTE pin index: 11 */
                                    /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitQSPI0BPins(void);

#define PORT_DFER_DFE_2_MASK 0x04u /*!<@brief Digital Filter Enable Mask for item 2. */
#define PORT_DFER_DFE_3_MASK 0x08u /*!<@brief Digital Filter Enable Mask for item 3. */
#define PORT_DFER_DFE_4_MASK 0x10u /*!<@brief Digital Filter Enable Mask for item 4. */
#define PORT_DFER_DFE_5_MASK 0x20u /*!<@brief Digital Filter Enable Mask for item 5. */
#define PORT_DFER_DFE_7_MASK 0x80u /*!<@brief Digital Filter Enable Mask for item 7. */

/*! @name PORTD4 (coord A3), J13[A78]/J13[B47]/U2[31]/FBa_AD2/SDRAM_A10
  @{ */
#define BOARD_SDRAM_A10_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_SDRAM_A10_PIN 4U     /*!<@brief PORTD pin index: 4 */
                                   /* @} */

/*! @name PORTD3 (coord B3), J13[A77]/J13[B52]/U2[30]/FBa_AD3/SDRAM_A11
  @{ */
#define BOARD_SDRAM_A11_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_SDRAM_A11_PIN 3U     /*!<@brief PORTD pin index: 3 */
                                   /* @} */

/*! @name PORTD2 (coord C3), J13[A76]/J13[B35]/U2[29]/FBa_AD4/SDRAM_A12
  @{ */
#define BOARD_SDRAM_A12_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_SDRAM_A12_PIN 2U     /*!<@brief PORTD pin index: 2 */
                                   /* @} */

/*! @name PORTC10 (coord C6), J13[A75]/J13[B50]/U2[26]/I2C1_SCL/FBa_AD5/SDRAM_A13
  @{ */
#define BOARD_SDRAM_A13_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A13_PIN 10U    /*!<@brief PORTC pin index: 10 */
                                   /* @} */

/*! @name PORTC9 (coord D6), J13[A74]/U2[25]/FBa_AD6/SDRAM_A14
  @{ */
#define BOARD_SDRAM_A14_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A14_PIN 9U     /*!<@brief PORTC pin index: 9 */
                                   /* @} */

/*! @name PORTC8 (coord A7), J13[A73]/U2[24]/FBa_AD7/SDRAM_A15
  @{ */
#define BOARD_SDRAM_A15_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A15_PIN 8U     /*!<@brief PORTC pin index: 8 */
                                   /* @} */

/*! @name PORTC7 (coord B7), J7[16]/J13[A72]/J13[B44]/U2[23]/SPI0_SIN/FBa_AD8/SDRAM_A16/FXIO0_D15
  @{ */
#define BOARD_SDRAM_A16_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A16_PIN 7U     /*!<@brief PORTC pin index: 7 */
                                   /* @} */

/*! @name PORTC6 (coord C7), J7[15]/J13[A71]/J13[B45]/U2[33]/SPI0_SOUT/FXIO0_D14/FBa_AD9/SDRAM_A17
  @{ */
#define BOARD_SDRAM_A17_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A17_PIN 6U     /*!<@brief PORTC pin index: 6 */
                                   /* @} */

/*! @name PORTC5 (coord D7), J13[A70]/U2[34]/FBa_AD10/SDRAM_A18
  @{ */
#define BOARD_SDRAM_A18_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A18_PIN 5U     /*!<@brief PORTC pin index: 5 */
                                   /* @} */

/*! @name PORTC4 (coord A8), J8[2]/J13[A69]/U2[22]/LPUART1_TX/FBa_AD11/SDRAM_A19
  @{ */
#define BOARD_SDRAM_A19_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A19_PIN 4U     /*!<@brief PORTC pin index: 4 */
                                   /* @} */

/*! @name PORTC2 (coord C8), J12[16]/J13[A9]/J13[A39]/J13[A68]/U2[35]/FBa_AD12/SDRAM_A20/TSI0_CH15
  @{ */
#define BOARD_SDRAM_A20_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A20_PIN 2U     /*!<@brief PORTC pin index: 2 */
                                   /* @} */

/*! @name PORTC1 (coord D8), J7[14]/J12[15]/J13[A40]/J13[A67]/J13[B21]/U2[20]/FBa_AD13/SDRAM_A21/FXIO0_D13/TSI0_CH14
  @{ */
#define BOARD_SDRAM_A21_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A21_PIN 1U     /*!<@brief PORTC pin index: 1 */
                                   /* @} */

/*! @name PORTC0 (coord B9), J12[14]/J13[A66]/U2[21]/FBa_AD14/SDRAM_A22/TSI0_CH13
  @{ */
#define BOARD_SDRAM_A22_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_A22_PIN 0U     /*!<@brief PORTC pin index: 0 */
                                   /* @} */

/*! @name PORTB18 (coord D9), J7[7]/J12[12]/J13[A34]/J13[B66]/TSI0_CH11/FBa_AD15/SDRAM_A23/FXIO0_D6/FTM2_CH0
  @{ */
#define BOARD_SDRAM_A23_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_A23_PIN 18U    /*!<@brief PORTB pin index: 18 */
                                   /* @} */

/*! @name PORTD5 (coord A2), J13[A27]/J13[A79]/U2[32]/FBa_AD1/SDRAM_A9
  @{ */
#define BOARD_SDRAM_A9_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_SDRAM_A9_PIN 5U     /*!<@brief PORTD pin index: 5 */
                                  /* @} */

/*! @name PORTB0 (coord G11), J7[1]/J12[5]/U2[17]/J13[A38]/FXIO0_D0/FTM1_CH0/SDRAM_CAS/TSI0_CH0
  @{ */
#define BOARD_SDRAM_CAS_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_CAS_PIN 0U     /*!<@brief PORTB pin index: 0 */
                                   /* @} */

/*! @name PORTD7 (coord A1), J13[A56]/U2[37]/USBID/SDRAM_CKE
  @{ */
#define BOARD_SDRAM_CKE_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_SDRAM_CKE_PIN 7U     /*!<@brief PORTD pin index: 7 */
                                   /* @} */

/*! @name PORTC3 (coord B8), J6[2]/LPUART1_RX/CLKOUT
  @{ */
#define BOARD_SDRAM_CLK_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_CLK_PIN 3U     /*!<@brief PORTC pin index: 3 */
                                   /* @} */

/*! @name PORTB3 (coord G8), J7[4]/J12[9]/U2[19]/FXIO0_D3/SDRAM_CS0/TSI0_CH8
  @{ */
#define BOARD_SDRAM_CS0_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_CS0_PIN 3U     /*!<@brief PORTB pin index: 3 */
                                   /* @} */

/*! @name PORTB17 (coord E9), J12[11]/J13[B67]/U2[2]/D13[1]/TSI0_CH10/FBa_AD16/SDRAM_D16/BRD_PAD1
  @{ */
#define BOARD_SDRAM_D16_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D16_PIN 17U    /*!<@brief PORTB pin index: 17 */
                                   /* @} */

/*! @name PORTB16 (coord B10), J12[10]/J13[B68]/U2[4]/D4[1]/TSI0_CH9/FBa_AD17/SDRAM_D17/BRD_PAD2
  @{ */
#define BOARD_SDRAM_D17_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D17_PIN 16U    /*!<@brief PORTB pin index: 16 */
                                   /* @} */

/*! @name PORTB11 (coord C10), J7[6]/J13[B69]/U2[5]/FBa_AD18/SDRAM_D18/FXIO0_D5
  @{ */
#define BOARD_SDRAM_D18_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D18_PIN 11U    /*!<@brief PORTB pin index: 11 */
                                   /* @} */

/*! @name PORTB10 (coord D10), J7[5]/J13[B57]/J13[B70]/U2[7]/FBa_AD19/SDRAM_D19/FXIO0_D4
  @{ */
#define BOARD_SDRAM_D19_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D19_PIN 10U    /*!<@brief PORTB pin index: 10 */
                                   /* @} */

/*! @name PORTB9 (coord E10), J13[B58]/U2[8]/SDRAM_D20
  @{ */
#define BOARD_SDRAM_D20_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D20_PIN 9U     /*!<@brief PORTB pin index: 9 */
                                   /* @} */

/*! @name PORTB8 (coord D11), J13[C59]/J35[C2]/U2[10]/EMVSIM1_RST/SDRAM_D21
  @{ */
#define BOARD_SDRAM_D21_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D21_PIN 8U     /*!<@brief PORTB pin index: 8 */
                                   /* @} */

/*! @name PORTB7 (coord E11), J13[C22]/J35[SW1]/U2[11]/EMVSIM1_PD/SDRAM_D22
  @{ */
#define BOARD_SDRAM_D22_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D22_PIN 7U     /*!<@brief PORTB pin index: 7 */
                                   /* @} */

/*! @name PORTB6 (coord F11), J13[C25]/U7[B2]/U2[13]/EMVSIM1_VCCEN/SDRAM_D23
  @{ */
#define BOARD_SDRAM_D23_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D23_PIN 6U     /*!<@brief PORTB pin index: 6 */
                                   /* @} */

/*! @name PORTC15 (coord B5), J13[B80]/U2[42]/FBa_AD24/SDRAM_D24
  @{ */
#define BOARD_SDRAM_D24_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_D24_PIN 15U    /*!<@brief PORTC pin index: 15 */
                                   /* @} */

/*! @name PORTC14 (coord A5), J13[B79]/U2[44]/FBa_AD25/SDRAM_D25
  @{ */
#define BOARD_SDRAM_D25_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_D25_PIN 14U    /*!<@brief PORTC pin index: 14 */
                                   /* @} */

/*! @name PORTC13 (coord A6), J13[B78]/U2[45]/FBa_AD26/SDRAM_D26
  @{ */
#define BOARD_SDRAM_D26_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_D26_PIN 13U    /*!<@brief PORTC pin index: 13 */
                                   /* @} */

/*! @name PORTC12 (coord B6), J13[B77]/U2[47]/FBa_AD27/SDRAM_D27
  @{ */
#define BOARD_SDRAM_D27_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_D27_PIN 12U    /*!<@brief PORTC pin index: 12 */
                                   /* @} */

/*! @name PORTB23 (coord E8), J13[B76]/U2[48]/FBa_AD28/SDRAM_D28
  @{ */
#define BOARD_SDRAM_D28_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D28_PIN 23U    /*!<@brief PORTB pin index: 23 */
                                   /* @} */

/*! @name PORTB22 (coord F8), J13[B75]/U2[50]/FBa_AD29/SDRAM_D29
  @{ */
#define BOARD_SDRAM_D29_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D29_PIN 22U    /*!<@brief PORTB pin index: 22 */
                                   /* @} */

/*! @name PORTB21 (coord F9), J13[B74]/U2[51]/FBa_AD30/SDRAM_D30
  @{ */
#define BOARD_SDRAM_D30_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D30_PIN 21U    /*!<@brief PORTB pin index: 21 */
                                   /* @} */

/*! @name PORTB20 (coord F10), J13[B73]/U2[53]/FBa_AD31/SDRAM_D31
  @{ */
#define BOARD_SDRAM_D31_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_D31_PIN 20U    /*!<@brief PORTB pin index: 20 */
                                   /* @} */

/*! @name PORTC16 (coord D5), U2[15]/SDRAM_DQM2
  @{ */
#define BOARD_SDRAM_DQM2_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_DQM2_PIN 16U    /*!<@brief PORTC pin index: 16 */
                                    /* @} */

/*! @name PORTC17 (coord C4), J1[8]/U2[39]/SDRAM_DQM3
  @{ */
#define BOARD_SDRAM_DQM3_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SDRAM_DQM3_PIN 17U    /*!<@brief PORTC pin index: 17 */
                                    /* @} */

/*! @name PORTB1 (coord G10), J7[2]/J12[7]/U2[18]/J13[A28]/J13[A37]/FXIO0_D1/FTM1_CH1/SDRAM_RAS/TSI0_CH6
  @{ */
#define BOARD_SDRAM_RAS_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_RAS_PIN 1U     /*!<@brief PORTB pin index: 1 */
                                   /* @} */

/*! @name PORTB2 (coord G9), J7[3]/J12[8]/U2[16]/FXIO0_D2/SDRAM_WE/TSI0_CH7
  @{ */
#define BOARD_SDRAM_WE_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SDRAM_WE_PIN 2U     /*!<@brief PORTB pin index: 2 */
                                  /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSDRAMPins(void);

/*! @name PORTB5 (coord C11), J13[B59]/J13[C58]/J35[C3]/EMVSIM1_CLK
  @{ */
#define BOARD_SMARTCARD_CLK_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SMARTCARD_CLK_PIN 5U     /*!<@brief PORTB pin index: 5 */
                                       /* @} */

/*! @name PORTB4 (coord B11), J13[C61]/J35[C7]/EMVSIM1_IO
  @{ */
#define BOARD_SMARTCARD_IO_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SMARTCARD_IO_PIN 4U     /*!<@brief PORTB pin index: 4 */
                                      /* @} */

/*! @name PORTB7 (coord E11), J13[C22]/J35[SW1]/U2[11]/EMVSIM1_PD/SDRAM_D22
  @{ */
#define BOARD_SMARTCARD_PD_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SMARTCARD_PD_PIN 7U     /*!<@brief PORTB pin index: 7 */
                                      /* @} */

/*! @name PORTB8 (coord D11), J13[C59]/J35[C2]/U2[10]/EMVSIM1_RST/SDRAM_D21
  @{ */
#define BOARD_SMARTCARD_RST_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SMARTCARD_RST_PIN 8U     /*!<@brief PORTB pin index: 8 */
                                       /* @} */

/*! @name PORTB6 (coord F11), J13[C25]/U7[B2]/U2[13]/EMVSIM1_VCCEN/SDRAM_D23
  @{ */
#define BOARD_SMARTCARD_VCCEN_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_SMARTCARD_VCCEN_PIN 6U     /*!<@brief PORTB pin index: 6 */
                                         /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitEMVSIMPins(void);

#define PORT_DFER_DFE_14_MASK 0x4000u /*!<@brief Digital Filter Enable Mask for item 14. */
#define PORT_DFER_DFE_15_MASK 0x8000u /*!<@brief Digital Filter Enable Mask for item 15. */

/*! @name PORTB17 (coord E9), J12[11]/J13[B67]/U2[2]/D13[1]/TSI0_CH10/FBa_AD16/SDRAM_D16/BRD_PAD1
  @{ */
#define BOARD_TSI_ELECTRODE_2_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_TSI_ELECTRODE_2_PIN 17U    /*!<@brief PORTB pin index: 17 */
                                         /* @} */

/*! @name PORTB16 (coord B10), J12[10]/J13[B68]/U2[4]/D4[1]/TSI0_CH9/FBa_AD17/SDRAM_D17/BRD_PAD2
  @{ */
#define BOARD_TSI_ELECTRODE_1_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_TSI_ELECTRODE_1_PIN 16U    /*!<@brief PORTB pin index: 16 */
                                         /* @} */

/*! @name PORTD14 (coord G4), D13/TP_LED1RR
  @{ */
#define BOARD_LED_TSI_PAD2_GPIO GPIOD /*!<@brief GPIO device name: GPIOD */
#define BOARD_LED_TSI_PAD2_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_LED_TSI_PAD2_PIN 14U    /*!<@brief PORTD pin index: 14 */
                                      /* @} */

/*! @name PORTD15 (coord H4), D14/TP_LED2RR
  @{ */
#define BOARD_LED_TSI_PAD1_GPIO GPIOD /*!<@brief GPIO device name: GPIOD */
#define BOARD_LED_TSI_PAD1_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_LED_TSI_PAD1_PIN 15U    /*!<@brief PORTD pin index: 15 */
                                      /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitTOUCH_PADsPins(void);

/*! @name ADC0_DM3 (coord K3), R44[2]/POT_5K/ADC0_DM3
  @{ */
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPOTPins(void);

#define PORT_DFER_DFE_8_MASK 0x0100u /*!<@brief Digital Filter Enable Mask for item 8. */
#define PORT_DFER_DFE_9_MASK 0x0200u /*!<@brief Digital Filter Enable Mask for item 9. */

/*! @name PORTD9 (coord A9), J13[A8]/U1[12]/U3[7]/U26[6]/I2C0_SDA
  @{ */
#define BOARD_ACCEL_SDA_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_ACCEL_SDA_PIN 9U     /*!<@brief PORTD pin index: 9 */
                                   /* @} */

/*! @name PORTD8 (coord A10), J13[A7]/U1[11]/U3[8]/U26[4]/I2C0_SCL
  @{ */
#define BOARD_ACCEL_SCL_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_ACCEL_SCL_PIN 8U     /*!<@brief PORTD pin index: 8 */
                                   /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitACCELPins(void);

#define PORT_DFER_DFE_8_MASK 0x0100u /*!<@brief Digital Filter Enable Mask for item 8. */
#define PORT_DFER_DFE_9_MASK 0x0200u /*!<@brief Digital Filter Enable Mask for item 9. */

/*! @name PORTD8 (coord A10), J13[A7]/U1[11]/U3[8]/U26[4]/I2C0_SCL
  @{ */
#define BOARD_GYRO_SCL_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_GYRO_SCL_PIN 8U     /*!<@brief PORTD pin index: 8 */
                                  /* @} */

/*! @name PORTD9 (coord A9), J13[A8]/U1[12]/U3[7]/U26[6]/I2C0_SDA
  @{ */
#define BOARD_GYRO_SDA_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_GYRO_SDA_PIN 9U     /*!<@brief PORTD pin index: 9 */
                                  /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitGYROPins(void);

#define PORT_DFER_DFE_8_MASK 0x0100u /*!<@brief Digital Filter Enable Mask for item 8. */
#define PORT_DFER_DFE_9_MASK 0x0200u /*!<@brief Digital Filter Enable Mask for item 9. */

/*! @name PORTD8 (coord A10), J13[A7]/U1[11]/U3[8]/U26[4]/I2C0_SCL
  @{ */
#define BOARD_PRESSURE_SCL_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_PRESSURE_SCL_PIN 8U     /*!<@brief PORTD pin index: 8 */
                                      /* @} */

/*! @name PORTD9 (coord A9), J13[A8]/U1[12]/U3[7]/U26[6]/I2C0_SDA
  @{ */
#define BOARD_PRESSURE_SDA_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_PRESSURE_SDA_PIN 9U     /*!<@brief PORTD pin index: 9 */
                                      /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPRESSUREPins(void);

#define PORT_DFER_DFE_7_MASK 0x80u /*!<@brief Digital Filter Enable Mask for item 7. */

/*! @name USB0_DM (coord H1), U9[2]/K80_USB0_DM
  @{ */
/* @} */

/*! @name USB0_DP (coord H2), U9[1]/K80_USB0_DP
  @{ */
/* @} */

/*! @name VREGIN (coord J2), J22[2]/5V0_VREGIN_K80
  @{ */
/* @} */

/*! @name VOUT33 (coord J1), J25[2]/P3V3_K80_VOUT
  @{ */
/* @} */

/*! @name PORTC19 (coord A4), J28[2]/U5[3]/K80_USB_EN
  @{ */
#define BOARD_USB_PWR_EN_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_USB_PWR_EN_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_USB_PWR_EN_PIN 19U    /*!<@brief PORTC pin index: 19 */
                                    /* @} */

/*! @name PORTC18 (coord B4), J18[2]/U5[4]/K80_USB_FLGA
  @{ */
#define BOARD_USB_FLAG_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_USB_FLAG_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_USB_FLAG_PIN 18U    /*!<@brief PORTC pin index: 18 */
                                  /* @} */

/*! @name PORTA29 (coord H11), Q1[3]/USB_PWROUT
  @{ */
#define BOARD_USB_PWR_STAT_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_USB_PWR_STAT_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_USB_PWR_STAT_PIN 29U    /*!<@brief PORTA pin index: 29 */
                                      /* @} */

/*! @name PORTD7 (coord A1), J13[A56]/U2[37]/USBID/SDRAM_CKE
  @{ */
#define BOARD_USB_ID_GPIO GPIOD /*!<@brief GPIO device name: GPIOD */
#define BOARD_USB_ID_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_USB_ID_PIN 7U     /*!<@brief PORTD pin index: 7 */
                                /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitUSBPins(void);

#define SOPT5_LPUART1RXSRC_LPUART_RX 0x00u /*!<@brief LPUART1 receive data source select: LPUART1_RX pin */
#define SOPT5_LPUART1TXSRC_LPUART_TX 0x00u /*!<@brief LPUART1 transmit data source select: LPUART1_TX pin */

/*! @name PORTC3 (coord B8), J6[2]/LPUART1_RX/CLKOUT
  @{ */
#define BOARD_DEBUG_UART_RX_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_DEBUG_UART_RX_PIN 3U     /*!<@brief PORTC pin index: 3 */
                                       /* @} */

/*! @name PORTC4 (coord A8), J8[2]/J13[A69]/U2[22]/LPUART1_TX/FBa_AD11/SDRAM_A19
  @{ */
#define BOARD_DEBUG_UART_TX_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_DEBUG_UART_TX_PIN 4U     /*!<@brief PORTC pin index: 4 */
                                       /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UARTPins(void);

/*! @name XTAL32 (coord L4), Y2[1]/XTAL32
  @{ */
/* @} */

/*! @name EXTAL32 (coord L5), Y2[2]/EXTAL32
  @{ */
/* @} */

/*! @name PORTA19 (coord K11), Y1[1]/XTAL0
  @{ */
#define BOARD_XTAL0_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_XTAL0_PIN 19U    /*!<@brief PORTA pin index: 19 */
                               /* @} */

/*! @name PORTA18 (coord L11), Y1[3]/EXTAL0
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
