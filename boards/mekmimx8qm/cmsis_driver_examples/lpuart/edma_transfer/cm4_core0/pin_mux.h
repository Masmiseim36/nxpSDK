/*
 * Copyright 2017-2020 NXP
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
/* Routed pin properties */
#define LPUART4_INITPINSEX_BB_UART2_RX_PERIPHERAL                     DMA__UART2   /*!< Peripheral name */
#define LPUART4_INITPINSEX_BB_UART2_RX_SIGNAL                            uart_rx   /*!< Signal name */
#define LPUART4_INITPINSEX_BB_UART2_RX_PIN_NAME                      UART0_RTS_B   /*!< Routed pin name */
#define LPUART4_INITPINSEX_BB_UART2_RX_PIN_FUNCTION_ID          SC_P_UART0_RTS_B   /*!< Pin function id */
#define LPUART4_INITPINSEX_BB_UART2_RX_LABEL              "BB_UART2_RX/J20A[28]"   /*!< Label */
#define LPUART4_INITPINSEX_BB_UART2_RX_NAME                        "BB_UART2_RX"   /*!< Identifier */

/* UART0_CTS_B (number AW49), BB_UART2_TX/J20A[29] */
/* Routed pin properties */
#define LPUART4_INITPINSEX_BB_UART2_TX_PERIPHERAL                     DMA__UART2   /*!< Peripheral name */
#define LPUART4_INITPINSEX_BB_UART2_TX_SIGNAL                            uart_tx   /*!< Signal name */
#define LPUART4_INITPINSEX_BB_UART2_TX_PIN_NAME                      UART0_CTS_B   /*!< Routed pin name */
#define LPUART4_INITPINSEX_BB_UART2_TX_PIN_FUNCTION_ID          SC_P_UART0_CTS_B   /*!< Pin function id */
#define LPUART4_INITPINSEX_BB_UART2_TX_LABEL              "BB_UART2_TX/J20A[29]"   /*!< Label */
#define LPUART4_INITPINSEX_BB_UART2_TX_NAME                        "BB_UART2_TX"   /*!< Identifier */

/* UART0_CTS_B (number AW49), BB_UART2_TX/J20A[29] */
/* Routed pin properties */
#define LPUART4_DEINITPINSEX_BB_UART2_TX_PERIPHERAL                  LSIO__GPIO0   /*!< Peripheral name */
#define LPUART4_DEINITPINSEX_BB_UART2_TX_SIGNAL                          gpio_io   /*!< Signal name */
#define LPUART4_DEINITPINSEX_BB_UART2_TX_CHANNEL                              23   /*!< Signal channel */
#define LPUART4_DEINITPINSEX_BB_UART2_TX_PIN_NAME                    UART0_CTS_B   /*!< Routed pin name */
#define LPUART4_DEINITPINSEX_BB_UART2_TX_PIN_FUNCTION_ID        SC_P_UART0_CTS_B   /*!< Pin function id */
#define LPUART4_DEINITPINSEX_BB_UART2_TX_LABEL            "BB_UART2_TX/J20A[29]"   /*!< Label */
#define LPUART4_DEINITPINSEX_BB_UART2_TX_NAME                      "BB_UART2_TX"   /*!< Identifier */
#define LPUART4_DEINITPINSEX_BB_UART2_TX_DIRECTION            kGPIO_DigitalInput   /*!< Direction */

/* Symbols to be used with GPIO driver */
#define LPUART4_DEINITPINSEX_BB_UART2_TX_GPIO                        LSIO__GPIO0   /*!< GPIO peripheral base pointer */
#define LPUART4_DEINITPINSEX_BB_UART2_TX_GPIO_PIN                            23U   /*!< GPIO pin number */
#define LPUART4_DEINITPINSEX_BB_UART2_TX_GPIO_PIN_MASK               (1U << 23U)   /*!< GPIO pin mask */

/* UART0_RTS_B (number AU45), BB_UART2_RX/J20A[28] */
/* Routed pin properties */
#define LPUART4_DEINITPINSEX_BB_UART2_RX_PERIPHERAL                  LSIO__GPIO0   /*!< Peripheral name */
#define LPUART4_DEINITPINSEX_BB_UART2_RX_SIGNAL                          gpio_io   /*!< Signal name */
#define LPUART4_DEINITPINSEX_BB_UART2_RX_CHANNEL                              22   /*!< Signal channel */
#define LPUART4_DEINITPINSEX_BB_UART2_RX_PIN_NAME                    UART0_RTS_B   /*!< Routed pin name */
#define LPUART4_DEINITPINSEX_BB_UART2_RX_PIN_FUNCTION_ID        SC_P_UART0_RTS_B   /*!< Pin function id */
#define LPUART4_DEINITPINSEX_BB_UART2_RX_LABEL            "BB_UART2_RX/J20A[28]"   /*!< Label */
#define LPUART4_DEINITPINSEX_BB_UART2_RX_NAME                      "BB_UART2_RX"   /*!< Identifier */
#define LPUART4_DEINITPINSEX_BB_UART2_RX_DIRECTION            kGPIO_DigitalInput   /*!< Direction */

/* Symbols to be used with GPIO driver */
#define LPUART4_DEINITPINSEX_BB_UART2_RX_GPIO                        LSIO__GPIO0   /*!< GPIO peripheral base pointer */
#define LPUART4_DEINITPINSEX_BB_UART2_RX_GPIO_PIN                            22U   /*!< GPIO pin number */
#define LPUART4_DEINITPINSEX_BB_UART2_RX_GPIO_PIN_MASK               (1U << 22U)   /*!< GPIO pin mask */

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
void LPUART4_InitPinsEx(sc_ipc_t ipc);                     /*!< Function assigned for the core: Cortex-M4F[cm4_core0] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 * @param ipc scfw ipchandle.
 *
 */
void LPUART4_DeinitPinsEx(sc_ipc_t ipc);                   /*!< Function assigned for the core: Cortex-M4F[cm4_core0] */

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
