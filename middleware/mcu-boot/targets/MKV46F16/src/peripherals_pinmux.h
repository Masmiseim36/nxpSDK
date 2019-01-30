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
#define BL_ENABLE_PINMUX_UART0 (BL_CONFIG_SCUART)
#define BL_ENABLE_PINMUX_SPI0 (BL_CONFIG_DSPI)
#define BL_ENABLE_PINMUX_I2C0 (BL_CONFIG_I2C)
#define BL_ENABLE_PINMUX_CAN0 (BL_CONFIG_CAN)

//! UART pinmux configurations
#define UART0_RX_PORT_BASE PORTD
#define UART0_RX_GPIO_BASE GPIOD
#define UART0_RX_GPIO_PIN_NUM 6                // PIN 6 in the PTd group
#define UART0_RX_FUNC_ALT_MODE kPORT_MuxAlt3   // ALT mode for UART0 RX
#define UART0_RX_GPIO_ALT_MODE kPORT_MuxAsGpio // ALT mdoe for GPIO
#define UART0_RX_GPIO_IRQn PORTD_IRQn
#define UART0_RX_GPIO_IRQHandler PORTD_IRQHandler
#define UART0_TX_PORT_BASE PORTD
#define UART0_TX_GPIO_PIN_NUM 7              // PIN 7 in the PTd group
#define UART0_TX_FUNC_ALT_MODE kPORT_MuxAlt3 // ALT mode for UART0 TX

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

//! CAN pinmux configurations
#define CAN0_RX_PORT_BASE PORTA
#define CAN0_RX_GPIO_PIN_NUM 13             // PIN 13 in the PTA group
#define CAN0_RX_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for CAN0 RX functionality for pin 13
#define CAN0_TX_PORT_BASE PORTA
#define CAN0_TX_GPIO_PIN_NUM 12             // PIN 12 in the PTA group
#define CAN0_TX_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for CAN0 TX functionality for pin 12

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
