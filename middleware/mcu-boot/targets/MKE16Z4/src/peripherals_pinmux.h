/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! peripheral enable configurations
#define BL_ENABLE_PINMUX_UART0 (BL_CONFIG_LPUART_0)
#define BL_ENABLE_PINMUX_UART1 (BL_CONFIG_LPUART_1)
#define BL_ENABLE_PINMUX_SPI0 (BL_CONFIG_LPSPI_0)
#define BL_ENABLE_PINMUX_I2C0 (BL_CONFIG_LPI2C_0)
#define BL_ENABLE_PINMUX_CAN0 (BL_CONFIG_CAN)

//! UART pinmux configurations
#define UART0_RX_PORT_BASE PORTB
#define UART0_RX_GPIO_BASE GPIOB
#define UART0_RX_GPIO_PIN_NUM 0  // PIN 0 in the PTB group
#define UART0_RX_FUNC_ALT_MODE 2 // ALT mode for UART0 RX functionality for pin 0
#define UART0_RX_GPIO_ALT_MODE 1 // ALT mdoe for GPIO functionality for pin 0
#define UART0_RX_GPIO_IRQn PORTBCD_IRQn
#define UART0_RX_GPIO_IRQHandler PORTBCD_IRQHandler
#define UART0_TX_PORT_BASE PORTB
#define UART0_TX_GPIO_PIN_NUM 1  // PIN 1 in the PTB group
#define UART0_TX_FUNC_ALT_MODE 2 // ALT mode for UART0 TX functionality for pin 1

//! I2C pinmux configurations
#define I2C0_SCL_PORT_BASE PORTA
#define I2C0_SCL_GPIO_PIN_NUM 3  // PIN 3 in the PTA group
#define I2C0_SCL_FUNC_ALT_MODE 3 // ALT mode for I2C0 SCL functionality for pin 3
#define I2C0_SDA_PORT_BASE PORTA
#define I2C0_SDA_GPIO_PIN_NUM 2  // PIN 2 in the PTA group
#define I2C0_SDA_FUNC_ALT_MODE 3 // ALT mode for I2C0 SDA functionality for pin 2

//! SPI pinmux configurations
#define SPI0_SELECTED_PCS 3
#define SPI0_PCS_PORT_BASE PORTA
#define SPI0_PCS_GPIO_PIN_NUM 7  // PIN 7 in the PTA group
#define SPI0_PCS_FUNC_ALT_MODE 3 // ALT mode for SPI0 PCS functionality for pin 7
#define SPI0_SCK_PORT_BASE PORTE
#define SPI0_SCK_GPIO_PIN_NUM 0  // PIN 0 in the PTE group
#define SPI0_SCK_FUNC_ALT_MODE 2 // ALT mode for SPI0 SCK functionality for pin 0
#define SPI0_SOUT_PORT_BASE PORTE
#define SPI0_SOUT_GPIO_PIN_NUM 2  // PIN 2 in the PTE group
#define SPI0_SOUT_FUNC_ALT_MODE 2 // ALT mode for SPI0 SOUT functionality for pin 2
#define SPI0_SIN_PORT_BASE PORTE
#define SPI0_SIN_GPIO_PIN_NUM 1  // PIN 1 in the PTE group
#define SPI0_SIN_FUNC_ALT_MODE 2 // ALT mode for SPI0 SIN functionality for pin 1

//! SPI pinmux configurations 2
//#define SPI0_SELECTED_PCS 1
//#define SPI0_PCS_PORT_BASE PORTB
//#define SPI0_PCS_GPIO_PIN_NUM 5  // PIN 5 in the PTB group
//#define SPI0_PCS_FUNC_ALT_MODE 3 // ALT mode for SPI0 PCS functionality for pin 5
//#define SPI0_SCK_PORT_BASE PORTB
//#define SPI0_SCK_GPIO_PIN_NUM 2  // PIN 2 in the PTB group
//#define SPI0_SCK_FUNC_ALT_MODE 3 // ALT mode for SPI0 SCK functionality for pin 2
//#define SPI0_SOUT_PORT_BASE PORTB
//#define SPI0_SOUT_GPIO_PIN_NUM 4  // PIN 4 in the PTB group
//#define SPI0_SOUT_FUNC_ALT_MODE 3 // ALT mode for SPI0 SOUT functionality for pin 4
//#define SPI0_SIN_PORT_BASE PORTB
//#define SPI0_SIN_GPIO_PIN_NUM 3  // PIN 3 in the PTB group
//#define SPI0_SIN_FUNC_ALT_MODE 3 // ALT mode for SPI0 SIN functionality for pin 3

//! CAN pinmux configurations
#define CAN0_RX_PORT_BASE PORTC
#define CAN0_RX_GPIO_PIN_NUM 6  // PIN 4 in the PTC group
#define CAN0_RX_FUNC_ALT_MODE 5 // ALT mode for CAN0 RX functionality for pin 6
#define CAN0_TX_PORT_BASE PORTC
#define CAN0_TX_GPIO_PIN_NUM 7  // PIN 5 in the PTC group
#define CAN0_TX_FUNC_ALT_MODE 5 // ALT mode for CAN0 TX functionality for pin 7

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
