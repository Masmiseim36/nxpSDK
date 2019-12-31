/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
  kPIN_MUX_DirectionInput = 0U,         /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,        /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U  /* Input or output direction */
} pin_mux_direction_t;

/*!
 * @addtogroup pin_mux
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/* PORTE0 (number 15), J1[1]/UART1_TX */
#define BOARD_INITPINS_DEBUG_UART_TX_PERIPHERAL                            UART1   /*!< Device name: UART1 */
#define BOARD_INITPINS_DEBUG_UART_TX_SIGNAL                                   TX   /*!< UART1 signal: TX */
#define BOARD_INITPINS_DEBUG_UART_TX_PIN_NAME                           UART1_TX   /*!< Pin name */
#define BOARD_INITPINS_DEBUG_UART_TX_LABEL                      "J1[1]/UART1_TX"   /*!< Label */
#define BOARD_INITPINS_DEBUG_UART_TX_NAME                        "DEBUG_UART_TX"   /*!< Identifier name */

/* PORTE1 (number 16), J1[3]/UART1_RX */
#define BOARD_INITPINS_DEBUG_UART_RX_PERIPHERAL                            UART1   /*!< Device name: UART1 */
#define BOARD_INITPINS_DEBUG_UART_RX_SIGNAL                                   RX   /*!< UART1 signal: RX */
#define BOARD_INITPINS_DEBUG_UART_RX_PIN_NAME                           UART1_RX   /*!< Pin name */
#define BOARD_INITPINS_DEBUG_UART_RX_LABEL                      "J1[3]/UART1_RX"   /*!< Label */
#define BOARD_INITPINS_DEBUG_UART_RX_NAME                        "DEBUG_UART_RX"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/* PORTC4 (number 4), J2[6]/U13[4]/SPI_SS */
#define DSPI0_INITPINS_FLASH_SS_PERIPHERAL                                  SPI0   /*!< Device name: SPI0 */
#define DSPI0_INITPINS_FLASH_SS_SIGNAL                                   PCS0_SS   /*!< SPI0 signal: PCS0_SS */
#define DSPI0_INITPINS_FLASH_SS_PIN_NAME                               SPI0_PCS0   /*!< Pin name */
#define DSPI0_INITPINS_FLASH_SS_LABEL                      "J2[6]/U13[4]/SPI_SS"   /*!< Label */
#define DSPI0_INITPINS_FLASH_SS_NAME                                  "FLASH_SS"   /*!< Identifier name */

/* PORTC5 (number 5), J2[12]/U13[2]SPI_CLK */
#define DSPI0_INITPINS_FLASH_CLK_PERIPHERAL                                 SPI0   /*!< Device name: SPI0 */
#define DSPI0_INITPINS_FLASH_CLK_SIGNAL                                      SCK   /*!< SPI0 signal: SCK */
#define DSPI0_INITPINS_FLASH_CLK_PIN_NAME                               SPI0_SCK   /*!< Pin name */
#define DSPI0_INITPINS_FLASH_CLK_LABEL                    "J2[12]/U13[2]SPI_CLK"   /*!< Label */
#define DSPI0_INITPINS_FLASH_CLK_NAME                                "FLASH_CLK"   /*!< Identifier name */

/* PORTC6 (number 6), J2[8]/U13[1]/SPI_SOUT */
#define DSPI0_INITPINS_FLASH_SOUT_PERIPHERAL                                SPI0   /*!< Device name: SPI0 */
#define DSPI0_INITPINS_FLASH_SOUT_SIGNAL                                    SOUT   /*!< SPI0 signal: SOUT */
#define DSPI0_INITPINS_FLASH_SOUT_PIN_NAME                             SPI0_SOUT   /*!< Pin name */
#define DSPI0_INITPINS_FLASH_SOUT_LABEL                  "J2[8]/U13[1]/SPI_SOUT"   /*!< Label */
#define DSPI0_INITPINS_FLASH_SOUT_NAME                              "FLASH_SOUT"   /*!< Identifier name */

/* PORTC7 (number 7), J2[10]/U13[8]/SPI_SIN */
#define DSPI0_INITPINS_FLASH_SIN_PERIPHERAL                                 SPI0   /*!< Device name: SPI0 */
#define DSPI0_INITPINS_FLASH_SIN_SIGNAL                                      SIN   /*!< SPI0 signal: SIN */
#define DSPI0_INITPINS_FLASH_SIN_PIN_NAME                               SPI0_SIN   /*!< Pin name */
#define DSPI0_INITPINS_FLASH_SIN_LABEL                   "J2[10]/U13[8]/SPI_SIN"   /*!< Label */
#define DSPI0_INITPINS_FLASH_SIN_NAME                                "FLASH_SIN"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void DSPI0_InitPins(void);

/* PORTC4 (number 4), J2[6]/U13[4]/SPI_SS */
#define DSPI0_DEINITPINS_FLASH_SS_PERIPHERAL                                 n/a   /*!< Device name: n/a */
#define DSPI0_DEINITPINS_FLASH_SS_SIGNAL                                disabled   /*!< n/a signal: disabled */
#define DSPI0_DEINITPINS_FLASH_SS_PIN_NAME PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/CMP1_OUT /*!< Pin name */
#define DSPI0_DEINITPINS_FLASH_SS_LABEL                    "J2[6]/U13[4]/SPI_SS"   /*!< Label */
#define DSPI0_DEINITPINS_FLASH_SS_NAME                                "FLASH_SS"   /*!< Identifier name */

/* PORTC5 (number 5), J2[12]/U13[2]SPI_CLK */
#define DSPI0_DEINITPINS_FLASH_CLK_PERIPHERAL                                n/a   /*!< Device name: n/a */
#define DSPI0_DEINITPINS_FLASH_CLK_SIGNAL                               disabled   /*!< n/a signal: disabled */
#define DSPI0_DEINITPINS_FLASH_CLK_PIN_NAME PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/CMP0_OUT /*!< Pin name */
#define DSPI0_DEINITPINS_FLASH_CLK_LABEL                  "J2[12]/U13[2]SPI_CLK"   /*!< Label */
#define DSPI0_DEINITPINS_FLASH_CLK_NAME                              "FLASH_CLK"   /*!< Identifier name */

/* PORTC6 (number 6), J2[8]/U13[1]/SPI_SOUT */
#define DSPI0_DEINITPINS_FLASH_SOUT_PERIPHERAL                              CMP0   /*!< Device name: CMP0 */
#define DSPI0_DEINITPINS_FLASH_SOUT_SIGNAL                                    IN   /*!< CMP0 signal: IN */
#define DSPI0_DEINITPINS_FLASH_SOUT_CHANNEL                                    0   /*!< CMP0 IN channel: 0 */
#define DSPI0_DEINITPINS_FLASH_SOUT_PIN_NAME                            CMP0_IN0   /*!< Pin name */
#define DSPI0_DEINITPINS_FLASH_SOUT_LABEL                "J2[8]/U13[1]/SPI_SOUT"   /*!< Label */
#define DSPI0_DEINITPINS_FLASH_SOUT_NAME                            "FLASH_SOUT"   /*!< Identifier name */

/* PORTC7 (number 7), J2[10]/U13[8]/SPI_SIN */
#define DSPI0_DEINITPINS_FLASH_SIN_PERIPHERAL                               CMP0   /*!< Device name: CMP0 */
#define DSPI0_DEINITPINS_FLASH_SIN_SIGNAL                                     IN   /*!< CMP0 signal: IN */
#define DSPI0_DEINITPINS_FLASH_SIN_CHANNEL                                     1   /*!< CMP0 IN channel: 1 */
#define DSPI0_DEINITPINS_FLASH_SIN_PIN_NAME                             CMP0_IN1   /*!< Pin name */
#define DSPI0_DEINITPINS_FLASH_SIN_LABEL                 "J2[10]/U13[8]/SPI_SIN"   /*!< Label */
#define DSPI0_DEINITPINS_FLASH_SIN_NAME                              "FLASH_SIN"   /*!< Identifier name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void DSPI0_DeinitPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void DSPI1_InitPins(void);

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void DSPI1_DeinitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
