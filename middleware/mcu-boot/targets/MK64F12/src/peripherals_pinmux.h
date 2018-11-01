/*
 * Copyright (c) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! peripheral enable configurations
#if defined(TOWER)
#define BL_ENABLE_PINMUX_UART1 (BL_CONFIG_SCUART)
#else
#define BL_ENABLE_PINMUX_UART0 (BL_CONFIG_SCUART)
#endif
#define BL_ENABLE_PINMUX_SPI0 (BL_CONFIG_DSPI)
#define BL_ENABLE_PINMUX_I2C1 (BL_CONFIG_I2C)

//! UART pinmux configurations
#if defined(TOWER) // TWR board and tower_bootloader
#define UART1_RX_PORT_BASE PORTC
#define UART1_RX_GPIO_BASE PTC
#define UART1_RX_GPIO_PIN_NUM 3                // PTC3
#define UART1_RX_FUNC_ALT_MODE kPORT_MuxAlt3   // ALT mode for UART1 RX functionality
#define UART1_RX_GPIO_ALT_MODE kPORT_MuxAsGpio // ALT mdoe for GPIO functionality
#define UART1_RX_GPIO_IRQn PORTC_IRQn
#define UART1_RX_GPIO_IRQHandler PORTC_IRQHandler
#define UART1_TX_PORT_BASE PORTC
#define UART1_TX_GPIO_PIN_NUM 4              // PTC4
#define UART1_TX_FUNC_ALT_MODE kPORT_MuxAlt3 // ALT mode for UART1 TX functionality

#else // flashloader and FRDM_bootloader
#define UART0_RX_PORT_BASE PORTB
#define UART0_RX_GPIO_BASE PTB
#define UART0_RX_GPIO_PIN_NUM 16               // PTB16
#define UART0_RX_FUNC_ALT_MODE kPORT_MuxAlt3   // ALT mode for UART1 RX functionality
#define UART0_RX_GPIO_ALT_MODE kPORT_MuxAsGpio // ALT mdoe for GPIO functionality
#define UART0_RX_GPIO_IRQn PORTB_IRQn
#define UART0_RX_GPIO_IRQHandler PORTB_IRQHandler
#define UART0_TX_PORT_BASE PORTB
#define UART0_TX_GPIO_PIN_NUM 17             // PTB17
#define UART0_TX_FUNC_ALT_MODE kPORT_MuxAlt3 // ALT mode for UART1 TX functionality
#endif                                       // TOWER

//! I2C pinmux configurations
#define I2C1_SCL_PORT_BASE PORTC
#define I2C1_SCL_GPIO_PIN_NUM 10             // PTC10
#define I2C1_SCL_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for I2C1 SCL functionality
#define I2C1_SDA_PORT_BASE PORTC
#define I2C1_SDA_GPIO_PIN_NUM 11             // PTC11
#define I2C1_SDA_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for I2C1 SDA functionality

//! SPI pinmux configurations
#define SPI0_PCS_PORT_BASE PORTD
#define SPI0_PCS_GPIO_PIN_NUM 0              // PTD0
#define SPI0_PCS_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI1 PCS functionality
#define SPI0_SCK_PORT_BASE PORTD
#define SPI0_SCK_GPIO_PIN_NUM 1              // PTD1
#define SPI0_SCK_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI1 SCK functionality
#define SPI0_SOUT_PORT_BASE PORTD
#define SPI0_SOUT_GPIO_PIN_NUM 2              // PTD2
#define SPI0_SOUT_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI1 SOUT functionality
#define SPI0_SIN_PORT_BASE PORTD
#define SPI0_SIN_GPIO_PIN_NUM 3              // PTD3
#define SPI0_SIN_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI1 SIN functionality

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
