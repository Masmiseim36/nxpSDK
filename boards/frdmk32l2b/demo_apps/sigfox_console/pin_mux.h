/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/*******************************************************************************
 * OL2385 Freedom board connection to the KL43Z freedom board
 *
 * Pin function  | OL2385 pos. | FRDM-KL43 pin | FRDM-KL43 pos.
 * -----------------------------------------------------------------------------
 * CLK           | P14         | PTD5          | J2-12
 * MISO          | P13         | PTD7          | J2-10
 * MOSI          | P16         | PTD6          | J2-8
 * CS            | P15         | PTD4          | J2-6
 * ACK           | P17         | PTD2          | J2-4
 *******************************************************************************/

/*! @name ACK GPIO pin
 * GPIO pin used by Sigfox device
 * @{ */
#define SF_ACK_INST                 instanceD
#define SF_ACK_GPIO                 GPIOD
#define SF_ACK_PORT                 PORTD
#define SF_ACK_PIN                  2U
/*! @} */

/*! @name CS GPIO pin
 * GPIO pin used by Sigfox device
 * @{ */
#define SF_CS_INST                 instanceD
#define SF_CS_GPIO                 GPIOD
#define SF_CS_PORT                 PORTD
#define SF_CS_PIN                  4U
/*! @} */

/*! @brief SPI instance used by Sigfox device (SPI 1). */
#define SF_SPI_INST                1U

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
    kPIN_MUX_DirectionInput = 0U,        /* Input direction */
    kPIN_MUX_DirectionOutput = 1U,       /* Output direction */
    kPIN_MUX_DirectionInputOrOutput = 2U /* Input or output direction */
} pin_mux_direction_t;

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

#define SOPT5_LPUART0RXSRC_LPUART_RX 0x00u /*!<@brief LPUART0 Receive Data Source Select: LPUART_RX pin */
#define SOPT5_LPUART0TXSRC_LPUART_TX 0x00u /*!<@brief LPUART0 Transmit Data Source Select: LPUART0_TX pin */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

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
