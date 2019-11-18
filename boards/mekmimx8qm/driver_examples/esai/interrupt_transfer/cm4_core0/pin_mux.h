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

/* M40_I2C0_SCL (number AM44), FTDI_M40_UART0_RX */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_PERIPHERAL                   M40__UART0   /*!< Device name: M40__UART0 */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_SIGNAL                          uart_rx   /*!< M40__UART0 signal: uart_rx */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_PIN_NAME                   M40_I2C0_SCL   /*!< Pin name */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_PIN_FUNCTION_ID       SC_P_M40_I2C0_SCL   /*!< Pin function id */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_LABEL               "FTDI_M40_UART0_RX"   /*!< Label */
#define BOARD_INITPINS_FTDI_M40_UART0_RX_NAME                "FTDI_M40_UART0_RX"   /*!< Identifier name */

/* M40_I2C0_SDA (number AU51), FTDI_M40_UART0_TX */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_PERIPHERAL                   M40__UART0   /*!< Device name: M40__UART0 */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_SIGNAL                          uart_tx   /*!< M40__UART0 signal: uart_tx */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_PIN_NAME                   M40_I2C0_SDA   /*!< Pin name */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_PIN_FUNCTION_ID       SC_P_M40_I2C0_SDA   /*!< Pin function id */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_LABEL               "FTDI_M40_UART0_TX"   /*!< Label */
#define BOARD_INITPINS_FTDI_M40_UART0_TX_NAME                "FTDI_M40_UART0_TX"   /*!< Identifier name */

/* QSPI1A_DATA1 (coord D14), BB_AUDIN_RST_B(QSPI1A_DATA1)/J20C[32] */
#define BOARD_INITPINS_BB_AUDIN_RST_B_GPIO                           LSIO__GPIO4   /*!< GPIO device name: LSIO__GPIO4 */
#define BOARD_INITPINS_BB_AUDIN_RST_B_GPIO_PIN                               25U   /*!< LSIO__GPIO4 pin index: 25 */
#define BOARD_INITPINS_BB_AUDIN_RST_B_PIN_NAME                      QSPI1A_DATA1   /*!< Pin name */
#define BOARD_INITPINS_BB_AUDIN_RST_B_PIN_FUNCTION_ID          SC_P_QSPI1A_DATA1   /*!< Pin function id */
#define BOARD_INITPINS_BB_AUDIN_RST_B_LABEL "BB_AUDIN_RST_B(QSPI1A_DATA1)/J20C[32]" /*!< Label */
#define BOARD_INITPINS_BB_AUDIN_RST_B_NAME                      "BB_AUDIN_RST_B"   /*!< Identifier name */
#define BOARD_INITPINS_BB_AUDIN_RST_B_DIRECTION              kGPIO_DigitalOutput   /*!< Direction */

/* M41_I2C0_SCL (number AR45), BB_M41_I2C0_1V8_SCL/J20A[22] */
#define BOARD_I2C_CONFIGUREPINS_BB_M41_I2C0_1V8_SCL_PERIPHERAL         M41__I2C0   /*!< Device name: M41__I2C0 */
#define BOARD_I2C_CONFIGUREPINS_BB_M41_I2C0_1V8_SCL_SIGNAL               i2c_scl   /*!< M41__I2C0 signal: i2c_scl */
#define BOARD_I2C_CONFIGUREPINS_BB_M41_I2C0_1V8_SCL_PIN_NAME        M41_I2C0_SCL   /*!< Pin name */
#define BOARD_I2C_CONFIGUREPINS_BB_M41_I2C0_1V8_SCL_PIN_FUNCTION_ID SC_P_M41_I2C0_SCL /*!< Pin function id */
#define BOARD_I2C_CONFIGUREPINS_BB_M41_I2C0_1V8_SCL_LABEL "BB_M41_I2C0_1V8_SCL/J20A[22]" /*!< Label */
#define BOARD_I2C_CONFIGUREPINS_BB_M41_I2C0_1V8_SCL_NAME   "BB_M41_I2C0_1V8_SCL"   /*!< Identifier name */

/* M41_I2C0_SDA (number AU49), BB_M41_I2C0_1V8_SDA/J20A[23] */
#define BOARD_I2C_CONFIGUREPINS_BB_M41_I2C0_1V8_SDA_PERIPHERAL         M41__I2C0   /*!< Device name: M41__I2C0 */
#define BOARD_I2C_CONFIGUREPINS_BB_M41_I2C0_1V8_SDA_SIGNAL               i2c_sda   /*!< M41__I2C0 signal: i2c_sda */
#define BOARD_I2C_CONFIGUREPINS_BB_M41_I2C0_1V8_SDA_PIN_NAME        M41_I2C0_SDA   /*!< Pin name */
#define BOARD_I2C_CONFIGUREPINS_BB_M41_I2C0_1V8_SDA_PIN_FUNCTION_ID SC_P_M41_I2C0_SDA /*!< Pin function id */
#define BOARD_I2C_CONFIGUREPINS_BB_M41_I2C0_1V8_SDA_LABEL "BB_M41_I2C0_1V8_SDA/J20A[23]" /*!< Label */
#define BOARD_I2C_CONFIGUREPINS_BB_M41_I2C0_1V8_SDA_NAME   "BB_M41_I2C0_1V8_SDA"   /*!< Identifier name */

/* ESAI0_FSR (number AW9), BB_ESAI0_FSR/J20D[24] */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_FSR_PERIPHERAL              AUD__ESAI0   /*!< Device name: AUD__ESAI0 */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_FSR_SIGNAL                    esai_fsr   /*!< AUD__ESAI0 signal: esai_fsr */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_FSR_PIN_NAME                 ESAI0_FSR   /*!< Pin name */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_FSR_PIN_FUNCTION_ID     SC_P_ESAI0_FSR   /*!< Pin function id */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_FSR_LABEL      "BB_ESAI0_FSR/J20D[24]"   /*!< Label */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_FSR_NAME                "BB_ESAI0_FSR"   /*!< Identifier name */

/* ESAI0_FST (number BG9), BB_ESAI0_FST/J20D[20] */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_FST_PERIPHERAL              AUD__ESAI0   /*!< Device name: AUD__ESAI0 */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_FST_SIGNAL                    esai_fst   /*!< AUD__ESAI0 signal: esai_fst */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_FST_PIN_NAME                 ESAI0_FST   /*!< Pin name */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_FST_PIN_FUNCTION_ID     SC_P_ESAI0_FST   /*!< Pin function id */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_FST_LABEL      "BB_ESAI0_FST/J20D[20]"   /*!< Label */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_FST_NAME                "BB_ESAI0_FST"   /*!< Identifier name */

/* ESAI0_SCKR (number BB8), BB_ESAI0_SCKR/J20D[17] */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_SCKR_PERIPHERAL             AUD__ESAI0   /*!< Device name: AUD__ESAI0 */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_SCKR_SIGNAL                  esai_sckr   /*!< AUD__ESAI0 signal: esai_sckr */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_SCKR_PIN_NAME               ESAI0_SCKR   /*!< Pin name */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_SCKR_PIN_FUNCTION_ID   SC_P_ESAI0_SCKR   /*!< Pin function id */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_SCKR_LABEL    "BB_ESAI0_SCKR/J20D[17]"   /*!< Label */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_SCKR_NAME              "BB_ESAI0_SCKR"   /*!< Identifier name */

/* ESAI0_SCKT (number AY8), BB_ESAI0_SCKT/J20D[18] */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_SCKT_PERIPHERAL             AUD__ESAI0   /*!< Device name: AUD__ESAI0 */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_SCKT_SIGNAL                  esai_sckt   /*!< AUD__ESAI0 signal: esai_sckt */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_SCKT_PIN_NAME               ESAI0_SCKT   /*!< Pin name */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_SCKT_PIN_FUNCTION_ID   SC_P_ESAI0_SCKT   /*!< Pin function id */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_SCKT_LABEL    "BB_ESAI0_SCKT/J20D[18]"   /*!< Label */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_SCKT_NAME              "BB_ESAI0_SCKT"   /*!< Identifier name */

/* ESAI0_TX0 (number BA9), BB_ESAI0_TX0/J20D[21] */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_TX0_PERIPHERAL              AUD__ESAI0   /*!< Device name: AUD__ESAI0 */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_TX0_SIGNAL                     esai_tx   /*!< AUD__ESAI0 signal: esai_tx */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_TX0_CHANNEL                          0   /*!< AUD__ESAI0 esai_tx channel: 0 */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_TX0_PIN_NAME                 ESAI0_TX0   /*!< Pin name */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_TX0_PIN_FUNCTION_ID     SC_P_ESAI0_TX0   /*!< Pin function id */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_TX0_LABEL      "BB_ESAI0_TX0/J20D[21]"   /*!< Label */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_TX0_NAME                "BB_ESAI0_TX0"   /*!< Identifier name */

/* ESAI0_TX5_RX0 (number AU7), BB_ESAI0_TX5_RX0/J20D[12] */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_TX5_RX0_PERIPHERAL          AUD__ESAI0   /*!< Device name: AUD__ESAI0 */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_TX5_RX0_SIGNAL             esai_tx5_rx   /*!< AUD__ESAI0 signal: esai_tx5_rx */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_TX5_RX0_CHANNEL                      0   /*!< AUD__ESAI0 esai_tx5_rx channel: 0 */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_TX5_RX0_PIN_NAME         ESAI0_TX5_RX0   /*!< Pin name */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_TX5_RX0_PIN_FUNCTION_ID SC_P_ESAI0_TX5_RX0 /*!< Pin function id */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_TX5_RX0_LABEL "BB_ESAI0_TX5_RX0/J20D[12]" /*!< Label */
#define BOARD_ESAI_CONFIGUREPINS_BB_ESAI0_TX5_RX0_NAME        "BB_ESAI0_TX5_RX0"   /*!< Identifier name */

/* MCLK_OUT0 (number BD4), MCLK_OUT0 */
#define BOARD_ESAI_CONFIGUREPINS_MCLK_OUT0_PERIPHERAL                 AUD__ESAI0   /*!< Device name: AUD__ESAI0 */
#define BOARD_ESAI_CONFIGUREPINS_MCLK_OUT0_SIGNAL                 esai_tx_hf_clk   /*!< AUD__ESAI0 signal: esai_tx_hf_clk */
#define BOARD_ESAI_CONFIGUREPINS_MCLK_OUT0_PIN_NAME                    MCLK_OUT0   /*!< Pin name */
#define BOARD_ESAI_CONFIGUREPINS_MCLK_OUT0_PIN_FUNCTION_ID        SC_P_MCLK_OUT0   /*!< Pin function id */
#define BOARD_ESAI_CONFIGUREPINS_MCLK_OUT0_LABEL                     "MCLK_OUT0"   /*!< Label */
#define BOARD_ESAI_CONFIGUREPINS_MCLK_OUT0_NAME                      "MCLK_OUT0"   /*!< Identifier name */

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
void BOARD_InitPins(sc_ipc_t ipc);                         /*!< Function assigned for the core: Cortex-M4F[cm4_0] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 * @param ipc scfw ipchandle.
 *
 */
void BOARD_I2C_ConfigurePins(sc_ipc_t ipc);                /*!< Function assigned for the core: Cortex-M4F[cm4_0] */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 * @param ipc scfw ipchandle.
 *
 */
void BOARD_ESAI_ConfigurePins(sc_ipc_t ipc);               /*!< Function assigned for the core: Cortex-M4F[cm4_0] */

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
