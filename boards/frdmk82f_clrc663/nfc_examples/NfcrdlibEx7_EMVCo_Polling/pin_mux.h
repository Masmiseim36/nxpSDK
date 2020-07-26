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

/*! @name PORTC6 (number 78), SW3
  @{ */
#define BOARD_SW3_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_SW3_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_SW3_PIN 6U     /*!<@brief PORTC pin index: 6 */
                             /* @} */

/*! @name PORTA4 (number 40), SW2
  @{ */
#define BOARD_SW2_GPIO GPIOA /*!<@brief GPIO device name: GPIOA */
#define BOARD_SW2_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_SW2_PIN 4U     /*!<@brief PORTA pin index: 4 */
                             /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitButtonsPins(void);

/*! @name PORTA18 (number 50), Y2[3]/EXTAL
  @{ */
#define BOARD_EXTAL0_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_EXTAL0_PIN 18U    /*!<@brief PORTA pin index: 18 */
                                /* @} */

/*! @name PORTA19 (number 51), Y2[1]/XTAL
  @{ */
#define BOARD_XTAL0_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_XTAL0_PIN 19U    /*!<@brief PORTA pin index: 19 */
                               /* @} */

/*! @name XTAL32 (number 31), Y1[2]/XTAL32_RTC
  @{ */
/* @} */

/*! @name EXTAL32 (number 32), Y1[1]/EXTAL32_RTC
  @{ */
/* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitOSCPins(void);

/*! @name PORTC8 (number 80), J1[14]/J12[17]/D6/LEDRGB_RED/Camera Reset
  @{ */
#define BOARD_LED_RED_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_LED_RED_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_LED_RED_PIN 8U     /*!<@brief PORTC pin index: 8 */
                                 /* @} */

/*! @name PORTC9 (number 81), J1[16]/D7/LEDRGB_GREEN
  @{ */
#define BOARD_LED_GREEN_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_LED_GREEN_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_LED_GREEN_PIN 9U     /*!<@brief PORTC pin index: 9 */
                                   /* @} */

/*! @name PORTC10 (number 82), J1[12]/D5/LEDRGB_BLUE
  @{ */
#define BOARD_LED_BLUE_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_LED_BLUE_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_LED_BLUE_PIN 10U    /*!<@brief PORTC pin index: 10 */
                                  /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDsPins(void);

/*! @name PORTC15 (number 87), OpenSDA_UART
  @{ */
#define BOARD_DEBUG_UART_TX_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_DEBUG_UART_TX_PIN 15U    /*!<@brief PORTC pin index: 15 */
                                       /* @} */

/*! @name PORTC14 (number 86), OpenSDA_UART
  @{ */
#define BOARD_DEBUG_UART_RX_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_DEBUG_UART_RX_PIN 14U    /*!<@brief PORTC pin index: 14 */
                                       /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UARTPins(void);

/*! @name PORTA1 (number 37), J2[18]/J12[4]/U6[6]/D14/I2C SDA
  @{ */
#define BOARD_I2C_SDA_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_I2C_SDA_PIN 1U     /*!<@brief PORTA pin index: 1 */
                                 /* @} */

/*! @name PORTA2 (number 38), J2[20]J12[3]/J19[6]/U6[4]/I2C_SCL
  @{ */
#define BOARD_I2C_SCL_PORT PORTA /*!<@brief PORT device name: PORTA */
#define BOARD_I2C_SCL_PIN 2U     /*!<@brief PORTA pin index: 2 */
                                 /* @} */

/*! @name PORTC13 (number 85), U6[11]
  @{ */
#define BOARD_ACCEL_INT1_GPIO GPIOC /*!<@brief GPIO device name: GPIOC */
#define BOARD_ACCEL_INT1_PORT PORTC /*!<@brief PORT device name: PORTC */
#define BOARD_ACCEL_INT1_PIN 13U    /*!<@brief PORTC pin index: 13 */
                                    /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitACCELPins(void);

/*! @name PORTE0 (number 1), U11[7]/QSPIA_DATA3
  @{ */
#define BOARD_QSPIA_DATA3_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPIA_DATA3_PIN 0U     /*!<@brief PORTE pin index: 0 */
                                     /* @} */

/*! @name PORTE1 (number 2), U11[6]/QSPIA_SCLK
  @{ */
#define BOARD_QSPIA_SCLK_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPIA_SCLK_PIN 1U     /*!<@brief PORTE pin index: 1 */
                                    /* @} */

/*! @name PORTE2 (number 3), U11[5]/QSPIA_DATA0
  @{ */
#define BOARD_QSPIA_DATA0_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPIA_DATA0_PIN 2U     /*!<@brief PORTE pin index: 2 */
                                     /* @} */

/*! @name PORTE3 (number 4), U11[3]/QSPIA_DATA2
  @{ */
#define BOARD_QSPIA_DATA2_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPIA_DATA2_PIN 3U     /*!<@brief PORTE pin index: 3 */
                                     /* @} */

/*! @name PORTE4 (number 7), U11[2]/QSPIA_DATA1
  @{ */
#define BOARD_QSPIA_DATA1_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPIA_DATA1_PIN 4U     /*!<@brief PORTE pin index: 4 */
                                     /* @} */

/*! @name PORTE5 (number 8), U11[1]/QSPIA_SS
  @{ */
#define BOARD_QSPIA_SS_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPIA_SS_PIN 5U     /*!<@brief PORTE pin index: 5 */
                                  /* @} */

/*! @name PORTE6 (number 9), U13[7]/QSPIB_DATA3
  @{ */
#define BOARD_QSPIB_DATA3_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPIB_DATA3_PIN 6U     /*!<@brief PORTE pin index: 6 */
                                     /* @} */

/*! @name PORTE7 (number 10), U13[6]/QSPIB_SCLK
  @{ */
#define BOARD_QSPIB_SCLK_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPIB_SCLK_PIN 7U     /*!<@brief PORTE pin index: 7 */
                                    /* @} */

/*! @name PORTE8 (number 11), U13[5]/QSPIB_DATA0
  @{ */
#define BOARD_QSPIB_DATA0_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPIB_DATA0_PIN 8U     /*!<@brief PORTE pin index: 8 */
                                     /* @} */

/*! @name PORTE9 (number 12), U13[3]/QSPIB_DATA2
  @{ */
#define BOARD_QSPIB_DATA2_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPIB_DATA2_PIN 9U     /*!<@brief PORTE pin index: 9 */
                                     /* @} */

/*! @name PORTE10 (number 13), U13[2]/QSPIB_DATA1
  @{ */
#define BOARD_QSPIB_DATA1_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPIB_DATA1_PIN 10U    /*!<@brief PORTE pin index: 10 */
                                     /* @} */

/*! @name PORTE11 (number 14), U13[1]/QSPIB_SS
  @{ */
#define BOARD_QSPIB_SS0_PORT PORTE /*!<@brief PORT device name: PORTE */
#define BOARD_QSPIB_SS0_PIN 11U    /*!<@brief PORTE pin index: 11 */
                                   /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitQSPI_FLASHPins(void);

/*! @name PORTB18 (number 64), J12[14]/E1/TSI_CH11/FXIO_D6
  @{ */
#define BOARD_TSI_ELECTRODE_1_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_TSI_ELECTRODE_1_PIN 18U    /*!<@brief PORTB pin index: 18 */
                                         /* @} */

/*! @name PORTB19 (number 65), J12[13]/E2/TSI_CH12/FXIO_D7
  @{ */
#define BOARD_TSI_ELECTRODE_2_PORT PORTB /*!<@brief PORT device name: PORTB */
#define BOARD_TSI_ELECTRODE_2_PIN 19U    /*!<@brief PORTB pin index: 19 */
                                         /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitTSI_TOUCH_PADPins(void);

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
