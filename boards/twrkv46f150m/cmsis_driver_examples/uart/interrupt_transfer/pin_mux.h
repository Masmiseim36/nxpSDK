/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

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


#define SOPT5_UART0RXSRC_UART_RX 0x00u /*!<@brief UART 0 receive data source select: UART0_RX pin */
#define SOPT5_UART0TXSRC_UART_TX 0x00u /*!<@brief UART 0 transmit data source select: UART0_TX pin */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART0_InitPins(void);

#define ADCOPT_ADCACH6SEL_CH_B 0x01u /*!<@brief ADCA MUX0 selection for ADCA channel 6: ADCA MUX0's channel b. */
#define ADCOPT_ADCACH7SEL_CH_B 0x01u /*!<@brief ADCA MUX1 selection for ADCA channel 7: ADCA MUX1's channel b. */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART0_DeinitPins(void);

#define SOPT5_UART1RXSRC_UART_RX 0x00u /*!<@brief UART 1 receive data source select: UART1_RX pin */
#define SOPT5_UART1TXSRC_UART_TX 0x00u /*!<@brief UART 1 transmit data source select: UART1_TX pin */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART1_InitPins(void);

#define ADCOPT_ADCBCH6SEL_CH_F 0x05u /*!<@brief ADCB MUX1 selection for ADCB channel 6: ADCB MUX0's channel f. */
#define ADCOPT_ADCBCH7SEL_CH_F 0x05u /*!<@brief ADCB MUX1 selection for ADCB channel 7: ADCB MUX1's channel f. */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void UART1_DeinitPins(void);

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
