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

/*! @name PORTC6 (number 78), SW1/LLWU_P10
  @{ */
#define BOARD_SW1_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_SW1_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SW1_PIN 6U     /*!<@brief PORTC pin index: 6 */
                             /* @} */

/*! @name PORTC11 (number 83), SW2/LLWU_P11
  @{ */
#define BOARD_SW2_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_SW2_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SW2_PIN 11U    /*!<@brief PORTC pin index: 11 */
                             /* @} */

/*! @name PORTA4 (number 38), J500[14]/SW3/NMI
  @{ */
#define BOARD_SW3_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_SW3_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SW3_PIN 4U     /*!<@brief PORTA pin index: 4 */
                             /* @} */

/*! @name PORTE25 (number 32), SW4
  @{ */
#define BOARD_SW4_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_SW4_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_SW4_PIN 25U    /*!<@brief PORTE pin index: 25 */
                             /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitBUTTONsPins(void);

#define PORT_DFER_DFE_7_MASK 0x80u /*!<@brief Digital Filter Enable Mask for item 7. */

/*! @name PORTE0 (number 1), J6[18]/J17[3]/D4/LED2/UART1_TX
  @{ */
#define BOARD_LED_RED_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_LED_RED_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_LED_RED_PIN 0U     /*!<@brief PORTE pin index: 0 */
                                 /* @} */

/*! @name PORTE1 (number 2), J6[17]/J17[1]/J16[B10]/D3/LED1/UART1_RX/SPI1_SOUT
  @{ */
#define BOARD_LED_YELLOW_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_LED_YELLOW_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_LED_YELLOW_PIN 1U     /*!<@brief PORTE pin index: 1 */
                                    /* @} */

/*! @name PORTD7 (number 100), J6[20]/J17[7]/D7/LED4
  @{ */
#define BOARD_LED_GREEN_GPIO GPIOD /*!<@brief GPIO device name: GPIOD */
#define BOARD_LED_GREEN_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_LED_GREEN_PIN 7U     /*!<@brief PORTD pin index: 7 */
                                   /* @} */

/*! @name PORTB19 (number 65), J17[5]/D6/LED3
  @{ */
#define BOARD_LED_ORANGE_GPIO GPIOB /*!<@brief GPIO device name: GPIOB */
#define BOARD_LED_ORANGE_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_LED_ORANGE_PIN 19U    /*!<@brief PORTB pin index: 19 */
                                    /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDsPins(void);

#define SOPT5_UART0TXSRC_UART_TX 0x00u /*!<@brief UART 0 transmit data source select: UART0_TX pin */

/*! @name PORTB16 (number 62), J500[7]/J22[2]/UART0_RX_TGTMCU
  @{ */
#define BOARD_DEBUG_UART_RX_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_RX_PIN 16U    /*!<@brief PORTB pin index: 16 */
                                       /* @} */

/*! @name PORTB17 (number 63), J500[5]/J23[2]/UART0_TX_TGTMCU
  @{ */
#define BOARD_DEBUG_UART_TX_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_DEBUG_UART_TX_PIN 17U    /*!<@brief PORTB pin index: 17 */
                                       /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UARTPins(void);

#define PORT_DFER_DFE_0_MASK 0x01u /*!<@brief Digital Filter Enable Mask for item 0. */
#define PORT_DFER_DFE_2_MASK 0x04u /*!<@brief Digital Filter Enable Mask for item 2. */
#define PORT_DFER_DFE_3_MASK 0x08u /*!<@brief Digital Filter Enable Mask for item 3. */

/*! @name PORTD2 (number 95), J6[3]/J9[2]/J16[A7]/J500[23]/I2C0_SCL
  @{ */
#define BOARD_ACCEL_SCL_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_ACCEL_SCL_PIN 2U     /*!<@brief PORTD pin index: 2 */
                                   /* @} */

/*! @name PORTD3 (number 96), J6[4]/J12[2]/J16[A8]/J500[21]/I2C0_SDA
  @{ */
#define BOARD_ACCEL_SDA_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_ACCEL_SDA_PIN 3U     /*!<@brief PORTD pin index: 3 */
                                   /* @} */

/*! @name PORTD0 (number 93), J6[15]/J11[2]/U1[11]/INT1
  @{ */
#define BOARD_ACCEL_INT1_GPIO GPIOD /*!<@brief GPIO device name: GPIOD */
#define BOARD_ACCEL_INT1_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_ACCEL_INT1_PIN 0U     /*!<@brief PORTD pin index: 0 */
                                    /* @} */

/*! @name PORTE24 (number 31), J10[2]/U1[9]/INT2
  @{ */
#define BOARD_ACCEL_INT2_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_ACCEL_INT2_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_ACCEL_INT2_PIN 24U    /*!<@brief PORTE pin index: 24 */
                                    /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitACCEL_MAGPins(void);

/*! @name ADC0_SE23 (number 27), J16[A32]/DAC0_OUT/ADC0_SE23/POT_5K
  @{ */
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPOTPins(void);

/*! @name PORTA18 (number 50), X501[1]/EXTAL0/CLKIN0
  @{ */
#define BOARD_EXTAL0_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_EXTAL0_PIN 18U    /*!<@brief PORTA pin index: 18 */
                                /* @} */

/*! @name PORTA19 (number 51), X501[3]/XTAL0
  @{ */
#define BOARD_XTAL0_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_XTAL0_PIN 19U    /*!<@brief PORTA pin index: 19 */
                               /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitOSCPins(void);

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
