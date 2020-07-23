/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_device_registers.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! peripheral enable configurations
#define BL_ENABLE_PINMUX_UART2 (BL_CONFIG_UART_2)
#define BL_ENABLE_PINMUX_SPI1 (BL_CONFIG_SPI)
#define BL_ENABLE_PINMUX_I2C0 (BL_CONFIG_I2C)

//! UART pinmux configurations
#define UART2_RX_PORT_BASE       PORTI
#define UART2_RX_GPIO_BASE       GPIOI
#define UART2_RX_GPIO_PIN_NUM    6                         // PTI6
#define UART2_RX_FUNC_ALT_MODE   kPORT_MuxAlt2             // ALT mode 2
#define UART2_RX_GPIO_ALT_MODE   kPORT_MuxAsGpio           // ALT mdoe for GPIO
#define UART2_RX_GPIO_IRQn       PTx_IRQn
#define UART2_RX_GPIO_IRQHandler PTx_IRQHandler    
#define UART2_TX_PORT_BASE       PORTI
#define UART2_TX_GPIO_PIN_NUM    7                         // PTI7
#define UART2_TX_FUNC_ALT_MODE   kPORT_MuxAlt2             // ALT mode 2

//! I2C pinmux configurations
#define I2C0_SCL_PORT_BASE       PORTK
#define I2C0_SCL_GPIO_PIN_NUM    7                 // PTK7
#define I2C0_SCL_FUNC_ALT_MODE   kPORT_MuxAlt2     // ALT mode 2
#define I2C0_SDA_PORT_BASE       PORTL
#define I2C0_SDA_GPIO_PIN_NUM    0                 // PTL0
#define I2C0_SDA_FUNC_ALT_MODE   kPORT_MuxAlt2     // ALT mode 2 

//! SPI pinmux configurations
#define SPI1_PCS_PORT_BASE       PORTF
#define SPI1_PCS_GPIO_PIN_NUM    3                 // PTF3
#define SPI1_PCS_FUNC_ALT_MODE   kPORT_MuxAlt2     // ALT mode 2
#define SPI1_SCK_PORT_BASE       PORTF
#define SPI1_SCK_GPIO_PIN_NUM    4                 // PTF4
#define SPI1_SCK_FUNC_ALT_MODE   kPORT_MuxAlt2     // ALT mode 2
#define SPI1_SOUT_PORT_BASE      PORTF
#define SPI1_SOUT_GPIO_PIN_NUM   5                 // PTF5
#define SPI1_SOUT_FUNC_ALT_MODE  kPORT_MuxAlt2     // ALT mode 2
#define SPI1_SIN_PORT_BASE       PORTF  
#define SPI1_SIN_GPIO_PIN_NUM    6                 // PTF6
#define SPI1_SIN_FUNC_ALT_MODE   kPORT_MuxAlt2     // ALT mode 2

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
