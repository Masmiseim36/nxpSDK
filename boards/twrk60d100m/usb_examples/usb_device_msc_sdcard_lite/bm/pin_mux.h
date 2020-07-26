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

/* PORTE0 (coord D3), J11[B22]/J20[8]/SDHC0_D1/GPIO2 */
#define BOARD_INITPINS_SDHC0_D1_PERIPHERAL                                  SDHC   /*!< Device name: SDHC */
#define BOARD_INITPINS_SDHC0_D1_SIGNAL                                         D   /*!< SDHC signal: D */
#define BOARD_INITPINS_SDHC0_D1_CHANNEL                                        1   /*!< SDHC D channel: 1 */
#define BOARD_INITPINS_SDHC0_D1_PIN_NAME                                SDHC0_D1   /*!< Pin name */
#define BOARD_INITPINS_SDHC0_D1_LABEL           "J11[B22]/J20[8]/SDHC0_D1/GPIO2"   /*!< Label */
#define BOARD_INITPINS_SDHC0_D1_NAME                                  "SDHC0_D1"   /*!< Identifier name */

/* PORTE1 (coord D2), J11[B11]/J20[7]/SDHC0_D0/SPI1_SIN */
#define BOARD_INITPINS_SDHC0_D0_PERIPHERAL                                  SDHC   /*!< Device name: SDHC */
#define BOARD_INITPINS_SDHC0_D0_SIGNAL                                         D   /*!< SDHC signal: D */
#define BOARD_INITPINS_SDHC0_D0_CHANNEL                                        0   /*!< SDHC D channel: 0 */
#define BOARD_INITPINS_SDHC0_D0_PIN_NAME                                SDHC0_D0   /*!< Pin name */
#define BOARD_INITPINS_SDHC0_D0_LABEL        "J11[B11]/J20[7]/SDHC0_D0/SPI1_SIN"   /*!< Label */
#define BOARD_INITPINS_SDHC0_D0_NAME                                  "SDHC0_D0"   /*!< Identifier name */

/* PORTE2 (coord D1), J11[B7]/J20[5]/SDHC0_DCLK/SPI1_SCK */
#define BOARD_INITPINS_SDHC0_DCLK_PERIPHERAL                                SDHC   /*!< Device name: SDHC */
#define BOARD_INITPINS_SDHC0_DCLK_SIGNAL                                    DCLK   /*!< SDHC signal: DCLK */
#define BOARD_INITPINS_SDHC0_DCLK_PIN_NAME                            SDHC0_DCLK   /*!< Pin name */
#define BOARD_INITPINS_SDHC0_DCLK_LABEL     "J11[B7]/J20[5]/SDHC0_DCLK/SPI1_SCK"   /*!< Label */
#define BOARD_INITPINS_SDHC0_DCLK_NAME                              "SDHC0_DCLK"   /*!< Identifier name */

/* PORTE3 (coord E4), J11[B10]/J20[2]/SDHC0_CMD/SPI1_SOUT */
#define BOARD_INITPINS_SDHC0_CMD_PERIPHERAL                                 SDHC   /*!< Device name: SDHC */
#define BOARD_INITPINS_SDHC0_CMD_SIGNAL                                      CMD   /*!< SDHC signal: CMD */
#define BOARD_INITPINS_SDHC0_CMD_PIN_NAME                              SDHC0_CMD   /*!< Pin name */
#define BOARD_INITPINS_SDHC0_CMD_LABEL     "J11[B10]/J20[2]/SDHC0_CMD/SPI1_SOUT"   /*!< Label */
#define BOARD_INITPINS_SDHC0_CMD_NAME                                "SDHC0_CMD"   /*!< Identifier name */

/* PORTE4 (coord E3), J11[B9]/J20[1]/SDHC0_D3/SPI1_PCS0 */
#define BOARD_INITPINS_SDHC0_D3_PERIPHERAL                                  SDHC   /*!< Device name: SDHC */
#define BOARD_INITPINS_SDHC0_D3_SIGNAL                                         D   /*!< SDHC signal: D */
#define BOARD_INITPINS_SDHC0_D3_CHANNEL                                        3   /*!< SDHC D channel: 3 */
#define BOARD_INITPINS_SDHC0_D3_PIN_NAME                                SDHC0_D3   /*!< Pin name */
#define BOARD_INITPINS_SDHC0_D3_LABEL        "J11[B9]/J20[1]/SDHC0_D3/SPI1_PCS0"   /*!< Label */
#define BOARD_INITPINS_SDHC0_D3_NAME                                  "SDHC0_D3"   /*!< Identifier name */

/* PORTE5 (coord E2), J11[A10]/J20[9]/SDHC0_D2 */
#define BOARD_INITPINS_SDHC0_D2_PERIPHERAL                                  SDHC   /*!< Device name: SDHC */
#define BOARD_INITPINS_SDHC0_D2_SIGNAL                                         D   /*!< SDHC signal: D */
#define BOARD_INITPINS_SDHC0_D2_CHANNEL                                        2   /*!< SDHC D channel: 2 */
#define BOARD_INITPINS_SDHC0_D2_PIN_NAME                                SDHC0_D2   /*!< Pin name */
#define BOARD_INITPINS_SDHC0_D2_LABEL                 "J11[A10]/J20[9]/SDHC0_D2"   /*!< Label */
#define BOARD_INITPINS_SDHC0_D2_NAME                                  "SDHC0_D2"   /*!< Identifier name */

/* PORTE28 (coord H4), J11[B52]/J11[B23]/J20[10]/SD_CARD_DETECT/GPIO3 */
#define BOARD_INITPINS_SD_CARD_CD_GPIO                                     GPIOE   /*!< GPIO device name: GPIOE */
#define BOARD_INITPINS_SD_CARD_CD_PORT                                     PORTE   /*!< PORT device name: PORTE */
#define BOARD_INITPINS_SD_CARD_CD_GPIO_PIN                                   28U   /*!< PORTE pin index: 28 */
#define BOARD_INITPINS_SD_CARD_CD_PIN_NAME                                 PTE28   /*!< Pin name */
#define BOARD_INITPINS_SD_CARD_CD_LABEL "J11[B52]/J11[B23]/J20[10]/SD_CARD_DETECT/GPIO3" /*!< Label */
#define BOARD_INITPINS_SD_CARD_CD_NAME                              "SD_CARD_CD"   /*!< Identifier name */

/* PORTE8 (coord F3), UART5_TX */
#define BOARD_INITPINS_DEBUG_UART_TX_PERIPHERAL                            UART5   /*!< Device name: UART5 */
#define BOARD_INITPINS_DEBUG_UART_TX_SIGNAL                                   TX   /*!< UART5 signal: TX */
#define BOARD_INITPINS_DEBUG_UART_TX_PIN_NAME                           UART5_TX   /*!< Pin name */
#define BOARD_INITPINS_DEBUG_UART_TX_LABEL                            "UART5_TX"   /*!< Label */
#define BOARD_INITPINS_DEBUG_UART_TX_NAME                        "DEBUG_UART_TX"   /*!< Identifier name */

/* PORTE9 (coord F2), UART5_RX */
#define BOARD_INITPINS_DEBUG_UART_RX_PERIPHERAL                            UART5   /*!< Device name: UART5 */
#define BOARD_INITPINS_DEBUG_UART_RX_SIGNAL                                   RX   /*!< UART5 signal: RX */
#define BOARD_INITPINS_DEBUG_UART_RX_PIN_NAME                           UART5_RX   /*!< Pin name */
#define BOARD_INITPINS_DEBUG_UART_RX_LABEL                            "UART5_RX"   /*!< Label */
#define BOARD_INITPINS_DEBUG_UART_RX_NAME                        "DEBUG_UART_RX"   /*!< Identifier name */

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

/*******************************************************************************
 * EOF
 ******************************************************************************/
