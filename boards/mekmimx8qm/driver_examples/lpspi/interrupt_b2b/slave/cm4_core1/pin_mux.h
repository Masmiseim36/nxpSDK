/*
 * Copyright 2017-2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

#include "board.h"

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/* UART0_RTS_B (number AU45), BB_UART2_RX/J20A[28] */
#define BOARD_INITPINS_BB_UART2_RX_PERIPHERAL                         DMA__UART2   /*!< Device name: DMA__UART2 */
#define BOARD_INITPINS_BB_UART2_RX_SIGNAL                                uart_rx   /*!< DMA__UART2 signal: uart_rx */
#define BOARD_INITPINS_BB_UART2_RX_PIN_NAME                          UART0_RTS_B   /*!< Pin name */
#define BOARD_INITPINS_BB_UART2_RX_PIN_FUNCTION_ID              SC_P_UART0_RTS_B   /*!< Pin function id */
#define BOARD_INITPINS_BB_UART2_RX_LABEL                  "BB_UART2_RX/J20A[28]"   /*!< Label */
#define BOARD_INITPINS_BB_UART2_RX_NAME                            "BB_UART2_RX"   /*!< Identifier name */

/* UART0_CTS_B (number AW49), BB_UART2_TX/J20A[29] */
#define BOARD_INITPINS_BB_UART2_TX_PERIPHERAL                         DMA__UART2   /*!< Device name: DMA__UART2 */
#define BOARD_INITPINS_BB_UART2_TX_SIGNAL                                uart_tx   /*!< DMA__UART2 signal: uart_tx */
#define BOARD_INITPINS_BB_UART2_TX_PIN_NAME                          UART0_CTS_B   /*!< Pin name */
#define BOARD_INITPINS_BB_UART2_TX_PIN_FUNCTION_ID              SC_P_UART0_CTS_B   /*!< Pin function id */
#define BOARD_INITPINS_BB_UART2_TX_LABEL                  "BB_UART2_TX/J20A[29]"   /*!< Label */
#define BOARD_INITPINS_BB_UART2_TX_NAME                            "BB_UART2_TX"   /*!< Identifier name */

/* SPI2_CS0 (number AW1), BB_SPI2_CS0/J20B[17] */
#define BOARD_INITPINS_BB_SPI2_CS0_PERIPHERAL                          DMA__SPI2   /*!< Device name: DMA__SPI2 */
#define BOARD_INITPINS_BB_SPI2_CS0_SIGNAL                                 spi_cs   /*!< DMA__SPI2 signal: spi_cs */
#define BOARD_INITPINS_BB_SPI2_CS0_CHANNEL                                     0   /*!< DMA__SPI2 spi_cs channel: 0 */
#define BOARD_INITPINS_BB_SPI2_CS0_PIN_NAME                             SPI2_CS0   /*!< Pin name */
#define BOARD_INITPINS_BB_SPI2_CS0_PIN_FUNCTION_ID                 SC_P_SPI2_CS0   /*!< Pin function id */
#define BOARD_INITPINS_BB_SPI2_CS0_LABEL                  "BB_SPI2_CS0/J20B[17]"   /*!< Label */
#define BOARD_INITPINS_BB_SPI2_CS0_NAME                            "BB_SPI2_CS0"   /*!< Identifier name */

/* SPI2_SCK (number AW5), BB_SPI2_SCLK/J20B[18] */
#define BOARD_INITPINS_BB_SPI2_SCLK_PERIPHERAL                         DMA__SPI2   /*!< Device name: DMA__SPI2 */
#define BOARD_INITPINS_BB_SPI2_SCLK_SIGNAL                               spi_sck   /*!< DMA__SPI2 signal: spi_sck */
#define BOARD_INITPINS_BB_SPI2_SCLK_PIN_NAME                            SPI2_SCK   /*!< Pin name */
#define BOARD_INITPINS_BB_SPI2_SCLK_PIN_FUNCTION_ID                SC_P_SPI2_SCK   /*!< Pin function id */
#define BOARD_INITPINS_BB_SPI2_SCLK_LABEL                "BB_SPI2_SCLK/J20B[18]"   /*!< Label */
#define BOARD_INITPINS_BB_SPI2_SCLK_NAME                          "BB_SPI2_SCLK"   /*!< Identifier name */

/* SPI2_SDI (number AY4), BB_SPI2_MISO/J20B[15] */
#define BOARD_INITPINS_BB_SPI2_MISO_PERIPHERAL                         DMA__SPI2   /*!< Device name: DMA__SPI2 */
#define BOARD_INITPINS_BB_SPI2_MISO_SIGNAL                               spi_sdi   /*!< DMA__SPI2 signal: spi_sdi */
#define BOARD_INITPINS_BB_SPI2_MISO_PIN_NAME                            SPI2_SDI   /*!< Pin name */
#define BOARD_INITPINS_BB_SPI2_MISO_PIN_FUNCTION_ID                SC_P_SPI2_SDI   /*!< Pin function id */
#define BOARD_INITPINS_BB_SPI2_MISO_LABEL                "BB_SPI2_MISO/J20B[15]"   /*!< Label */
#define BOARD_INITPINS_BB_SPI2_MISO_NAME                          "BB_SPI2_MISO"   /*!< Identifier name */

/* SPI2_SDO (number BA1), BB_SPI2_MOSI/J20B[14] */
#define BOARD_INITPINS_BB_SPI2_MOSI_PERIPHERAL                         DMA__SPI2   /*!< Device name: DMA__SPI2 */
#define BOARD_INITPINS_BB_SPI2_MOSI_SIGNAL                               spi_sdo   /*!< DMA__SPI2 signal: spi_sdo */
#define BOARD_INITPINS_BB_SPI2_MOSI_PIN_NAME                            SPI2_SDO   /*!< Pin name */
#define BOARD_INITPINS_BB_SPI2_MOSI_PIN_FUNCTION_ID                SC_P_SPI2_SDO   /*!< Pin function id */
#define BOARD_INITPINS_BB_SPI2_MOSI_LABEL                "BB_SPI2_MOSI/J20B[14]"   /*!< Label */
#define BOARD_INITPINS_BB_SPI2_MOSI_NAME                          "BB_SPI2_MOSI"   /*!< Identifier name */

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
 * @param ipc scfw ipchandle.
 *
 */
void BOARD_InitPins(sc_ipc_t ipc);                         /*!< Function assigned for the core: Cortex-M4F[cm4_1] */

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
