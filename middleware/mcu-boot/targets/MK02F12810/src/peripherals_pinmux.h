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
#define BL_ENABLE_PINMUX_UART1 (BL_CONFIG_SCUART)
#define BL_ENABLE_PINMUX_SPI0 (BL_CONFIG_DSPI)
#define BL_ENABLE_PINMUX_I2C0 (BL_CONFIG_I2C)

//! UART pinmux configurations - For TWR board bootloader
#define UART1_RX_PORT_BASE PORTC
#define UART1_RX_GPIO_BASE GPIOC
#define UART1_RX_GPIO_PIN_NUM 3                // PIN 3 in the PTC group
#define UART1_RX_FUNC_ALT_MODE kPORT_MuxAlt3   // ALT mode for UART1 RX
#define UART1_RX_GPIO_ALT_MODE kPORT_MuxAsGpio // ALT mdoe for GPIO
#define UART1_RX_GPIO_IRQn PORTC_IRQn
#define UART1_RX_GPIO_IRQHandler PORTC_IRQHandler
#define UART1_TX_PORT_BASE PORTC
#define UART1_TX_GPIO_PIN_NUM 4              // PIN 4 in the PTC group
#define UART1_TX_FUNC_ALT_MODE kPORT_MuxAlt3 // ALT mode for UART1 TX

//! I2C pinmux configurations
#define I2C0_SCL_PORT_BASE PORTB
#define I2C0_SCL_GPIO_PIN_NUM 0              // PIN 0 in the PTB group
#define I2C0_SCL_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for I2C0 SCL
#define I2C0_SDA_PORT_BASE PORTB
#define I2C0_SDA_GPIO_PIN_NUM 1              // PIN 1 in the PTB group
#define I2C0_SDA_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for I2C0 SDA

//! SPI pinmux configurations
#define SPI0_PCS_PORT_BASE PORTE
#define SPI0_PCS_GPIO_PIN_NUM 16             // PIN 16 in the PTE group
#define SPI0_PCS_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 PCS
#define SPI0_SCK_PORT_BASE PORTE
#define SPI0_SCK_GPIO_PIN_NUM 17             // PIN 17 in the PTE group
#define SPI0_SCK_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 SCK
#define SPI0_SOUT_PORT_BASE PORTE
#define SPI0_SOUT_GPIO_PIN_NUM 18             // PIN 18 in the PTE group
#define SPI0_SOUT_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 SOUT
#define SPI0_SIN_PORT_BASE PORTE
#define SPI0_SIN_GPIO_PIN_NUM 19             // PIN 19 in the PTE group
#define SPI0_SIN_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 SIN

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
