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


/*!
 * @brief UART 1 receive data source select: UART1_RX pin */
#define SOPT5_UART1RXSRC_UART_RX 0x00u
/*!
 * @brief UART 1 transmit data source select: UART1_TX pin */
#define SOPT5_UART1TXSRC_UART_TX 0x00u
/*!
 * @brief FTM0 channel 0 output source: FTM0_CH0 pin is output of FTM0 channel 0 output */
#define SOPT8_FTM0OCH0SRC_FTM 0x00u
/*!
 * @brief FTM0 channel 1 output source: FTM0_CH1 pin is output of FTM0 channel 1 output */
#define SOPT8_FTM0OCH1SRC_FTM 0x00u

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
