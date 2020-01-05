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

/*! @name PORTC6 (coord C8), J11[A21]/J11[A58]/J11[A71]/SW1
  @{ */
#define BOARD_SW1_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_SW1_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SW1_PIN 6U     /*!<@brief PORTC pin index: 6 */
                             /* @} */

/*! @name PORTA4 (coord L7), SW3/NMI_B
  @{ */
#define BOARD_SW3_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_SW3_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SW3_PIN 4U     /*!<@brief PORTA pin index: 4 */
                             /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitButtonsPins(void);

/*! @name PORTE6 (coord E1), J30[1]/D5/LED1
  @{ */
#define BOARD_LED_GREEN_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_LED_GREEN_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_LED_GREEN_PIN 6U     /*!<@brief PORTE pin index: 6 */
                                   /* @} */

/*! @name PORTE7 (coord F4), J30[3]/D6/LED2
  @{ */
#define BOARD_LED_YELLOW_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_LED_YELLOW_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_LED_YELLOW_PIN 7U     /*!<@brief PORTE pin index: 7 */
                                    /* @} */

/*! @name PORTE8 (coord F3), J11[A44]/J30[5]/D7/LED3
  @{ */
#define BOARD_LED_ORANGE_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_LED_ORANGE_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_LED_ORANGE_PIN 8U     /*!<@brief PORTE pin index: 8 */
                                    /* @} */

/*! @name PORTE9 (coord F2), J11[A43]/J30[7]/D9/LED4
  @{ */
#define BOARD_LED_BLUE_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_LED_BLUE_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_LED_BLUE_PIN 9U     /*!<@brief PORTE pin index: 9 */
                                  /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDsPins(void);

#define SOPT5_UART1TXSRC_UART_TX 0x00u /*!<@brief UART 1 transmit data source select: UART1_TX pin */

/*! @name PORTC3 (coord A11), J10[2]/J11[A38]/J11[A64]/UART1_RX
  @{ */
#define BOARD_DEBUG_UART_RX_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_DEBUG_UART_RX_PIN 3U     /*!<@brief PORTC pin index: 3 */
                                       /* @} */

/*! @name PORTC4 (coord A9), J11[A37]/J15[2]/J11[A69]/UART1_TX
  @{ */
#define BOARD_DEBUG_UART_TX_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_DEBUG_UART_TX_PIN 4U     /*!<@brief PORTC pin index: 4 */
                                       /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDebug_UARTPins(void);

/*! @name PORTA18 (coord M12), J11[B24]/Y1[3]/EXTAL_K64_MAIN
  @{ */
#define BOARD_XTAL0_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_XTAL0_PIN 18U    /*!<@brief PORTA pin index: 18 */
                               /* @} */

/*! @name EXTAL32 (coord M6), Y2[1]/EXTAL32_RTC
  @{ */
/* @} */

/*! @name XTAL32 (coord M7), Y2[2]/XTAL32_RTC
  @{ */
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitOSCPins(void);

/*! @name USB0_DM (coord H2), J17[2]/K64_USB0_DM
  @{ */
/* @} */

/*! @name USB0_DP (coord H1), J17[3]/K64_USB0_DP
  @{ */
/* @} */

/*! @name PORTC9 (coord D7), J26[1]/J11[A74]/U505[1]/K64_USB_ENABLE
  @{ */
#define BOARD_USB_ENABLE_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_USB_ENABLE_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_USB_ENABLE_PIN 9U     /*!<@brief PORTC pin index: 9 */
                                    /* @} */

/*! @name PORTC8 (coord A8), J23[2]/J11[A73]/U505[2]/K64_USB_FLGA
  @{ */
#define BOARD_USB_FLGA_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_USB_FLGA_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_USB_FLGA_PIN 8U     /*!<@brief PORTC pin index: 8 */
                                  /* @} */

/*! @name PORTE12 (coord G3), J25[2]/K64_USB_ID
  @{ */
#define BOARD_USB_ID_GPIO GPIOE /*!<@brief GPIO device name: GPIOE */
#define BOARD_USB_ID_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_USB_ID_PIN 12U    /*!<@brief PORTE pin index: 12 */
                                /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitUSB_otgPins(void);

/*! @name PORTC10 (coord C7), J3[3]/J11[A75]/U6[4]/I2C1_SCL
  @{ */
#define BOARD_ACCEL_SCL_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_ACCEL_SCL_PIN 10U    /*!<@brief PORTC pin index: 10 */
                                   /* @} */

/*! @name PORTC11 (coord B7), J3[4]/J11[A60]/J11[B71]/U6[6]/I2C1_SDA
  @{ */
#define BOARD_ACCEL_SDA_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_ACCEL_SDA_PIN 11U    /*!<@brief PORTC pin index: 11 */
                                   /* @} */

/*! @name PORTA6 (coord J7), J8[2]/U6[11]/ACCEL_INT1
  @{ */
#define BOARD_ACCEL_INT1_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_ACCEL_INT1_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_ACCEL_INT1_PIN 6U     /*!<@brief PORTA pin index: 6 */
                                    /* @} */

/*! @name PORTA8 (coord K8), J7[2]/U6[9]/ACCEL_INT2
  @{ */
#define BOARD_ACCEL_INT2_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_ACCEL_INT2_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_ACCEL_INT2_PIN 8U     /*!<@brief PORTA pin index: 8 */
                                    /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitAccelerometerPins(void);

#define PORT_DFER_DFE_7_MASK 0x80u /*!<@brief Digital Filter Enable Mask for item 7. */

/*! @name PORTE1 (coord D2), J5[20]/P500[7]/SDHC0_D0/TRACE_D3
  @{ */
#define BOARD_SDHC_D0_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_SDHC_D0_PIN 1U     /*!<@brief PORTE pin index: 1 */
                                 /* @} */

/*! @name PORTE0 (coord D3), J5[12]/J11[B22]/P500[8]/SDHC0_D1/TRACE_CLKOUT
  @{ */
#define BOARD_SDHC_D1_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_SDHC_D1_PIN 0U     /*!<@brief PORTE pin index: 0 */
                                 /* @} */

/*! @name PORTE5 (coord E2), P500[9]/SDHC0_D2
  @{ */
#define BOARD_SDHC_D2_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_SDHC_D2_PIN 5U     /*!<@brief PORTE pin index: 5 */
                                 /* @} */

/*! @name PORTE4 (coord E3), J5[14]/P500[1]/SDHC0_D3/TRACE_D0
  @{ */
#define BOARD_SDHC_D3_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_SDHC_D3_PIN 4U     /*!<@brief PORTE pin index: 4 */
                                 /* @} */

/*! @name PORTE3 (coord E4), J5[16]/P500[2]/SDHC0_CMD/TRACE_D1
  @{ */
#define BOARD_SDHC_CMD_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_SDHC_CMD_PIN 3U     /*!<@brief PORTE pin index: 3 */
                                  /* @} */

/*! @name PORTE2 (coord D1), J5[18]/P500[5]/SDHC0_DCLK/TRACE_D2
  @{ */
#define BOARD_SDHC_CLK_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_SDHC_CLK_PIN 2U     /*!<@brief PORTE pin index: 2 */
                                  /* @} */

/*! @name PORTD7 (coord A1), J11[A33]/J11[B52]
  @{ */
#define BOARD_SDHC_DETECT_GPIO GPIOD /*!<@brief GPIO device name: GPIOD */
#define BOARD_SDHC_DETECT_PORT PORTD /*!<@brief PORT device name: PORTD */
#define BOARD_SDHC_DETECT_PIN 7U     /*!<@brief PORTD pin index: 7 */
                                     /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitSD_CardPins(void);

/*! @name ADC1_SE18 (coord M3), J24[2]/POT
  @{ */
/* @} */

/*! @name VREFH (coord G5), VREFH
  @{ */
/* @} */

/*! @name VREFL (coord G6), GND
  @{ */
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPotentiometerPins(void);

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
