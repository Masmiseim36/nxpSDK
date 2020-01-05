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
#define BL_ENABLE_PINMUX_UART0 (BL_CONFIG_LPUART_0)
#define BL_ENABLE_PINMUX_UART1 (BL_CONFIG_LPUART_1)
#define BL_ENABLE_PINMUX_UART2 (BL_CONFIG_LPUART_2)
#define BL_ENABLE_PINMUX_SPI0 (BL_CONFIG_LPSPI_0)
#define BL_ENABLE_PINMUX_SPI1 (BL_CONFIG_LPSPI_1)
#define BL_ENABLE_PINMUX_SPI2 (BL_CONFIG_LPSPI_2)
#define BL_ENABLE_PINMUX_I2C0 (BL_CONFIG_LPI2C_0)
#define BL_ENABLE_PINMUX_I2C1 (BL_CONFIG_LPI2C_1)
#define BL_ENABLE_PINMUX_I2C2 (BL_CONFIG_LPI2C_2)

//! UART pinmux configurations
#if TOWER
#define UART0_RX_PORT_BASE PORTA
#define UART0_RX_GPIO_BASE GPIOA
#define UART0_RX_GPIO_PIN_NUM 1                // PTA1
#define UART0_RX_FUNC_ALT_MODE kPORT_MuxAlt2   // ALT mode for UART0 RX functionality
#define UART0_RX_GPIO_ALT_MODE kPORT_MuxAsGpio // ALT mdoe for GPIO functionality
#define UART0_RX_GPIO_IRQn PORTA_IRQn
#define UART0_RX_GPIO_IRQHandler PORTA_IRQHandler
#define UART0_TX_PORT_BASE PORTA
#define UART0_TX_GPIO_PIN_NUM 2              // PTA2
#define UART0_TX_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for UART0 TX functionality
#else
#define UART0_RX_PORT_BASE PORTB
#define UART0_RX_GPIO_BASE GPIOB
#define UART0_RX_GPIO_PIN_NUM 16               // PTB16
#define UART0_RX_FUNC_ALT_MODE kPORT_MuxAlt3   // ALT mode for UART0 RX functionality
#define UART0_RX_GPIO_ALT_MODE kPORT_MuxAsGpio // ALT mdoe for GPIO functionality
#define UART0_RX_GPIO_IRQn PORTB_IRQn
#define UART0_RX_GPIO_IRQHandler PORTB_IRQHandler
#define UART0_TX_PORT_BASE PORTB
#define UART0_TX_GPIO_PIN_NUM 17             // PTB17
#define UART0_TX_FUNC_ALT_MODE kPORT_MuxAlt3 // ALT mode for UART0 TX functionality
#endif                                       // TOWER

#define UART1_RX_PORT_BASE PORTC
#define UART1_RX_GPIO_BASE GPIOC
#define UART1_RX_GPIO_PIN_NUM 3                // PIN 3 in the PTC group
#define UART1_RX_FUNC_ALT_MODE kPORT_MuxAlt3   // ALT mode for UART1 RX functionality for pin 3
#define UART1_RX_GPIO_ALT_MODE kPORT_MuxAsGpio // ALT mdoe for GPIO functionality for pin 3
#define UART1_RX_GPIO_IRQn PORTC_IRQn
#define UART1_RX_GPIO_IRQHandler PORTC_IRQHandler
#define UART1_TX_PORT_BASE PORTC
#define UART1_TX_GPIO_PIN_NUM 4              // PIN 4 in the PTC group
#define UART1_TX_FUNC_ALT_MODE kPORT_MuxAlt3 // ALT mode for UART1 TX functionality for pin 4

#define UART2_RX_PORT_BASE PORTD
#define UART2_RX_GPIO_BASE GPIOD
#define UART2_RX_GPIO_PIN_NUM 2                // PIN 2 in the PTD group
#define UART2_RX_FUNC_ALT_MODE kPORT_MuxAlt3   // ALT mode for UART2 RX functionality for pin 2
#define UART2_RX_GPIO_ALT_MODE kPORT_MuxAsGpio // ALT mdoe for GPIO functionality for pin 2
#define UART2_RX_GPIO_IRQn PORTD_IRQn
#define UART2_RX_GPIO_IRQHandler PORTD_IRQHandler
#define UART2_TX_PORT_BASE PORTD
#define UART2_TX_GPIO_PIN_NUM 3              // PIN 3 in the PTD group
#define UART2_TX_FUNC_ALT_MODE kPORT_MuxAlt3 // ALT mode for UART2 TX functionality for pin 3

//! I2C pinmux configurations
#define I2C0_SCL_PORT_BASE PORTE
#define I2C0_SCL_GPIO_PIN_NUM 24             // PIN 24 in the PTE group
#define I2C0_SCL_FUNC_ALT_MODE kPORT_MuxAlt5 // ALT mode for I2C0 SCL functionality for pin 24
#define I2C0_SDA_PORT_BASE PORTE
#define I2C0_SDA_GPIO_PIN_NUM 25             // PIN 25 in the PTE group
#define I2C0_SDA_FUNC_ALT_MODE kPORT_MuxAlt5 // ALT mode for I2C0 SDA functionality for pin 25

#if FREEDOM
#define I2C1_SCL_PORT_BASE PORTC
#define I2C1_SCL_GPIO_PIN_NUM 1              // PTC1
#define I2C1_SCL_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for I2C1 SCL functionality
#define I2C1_SDA_PORT_BASE PORTC
#define I2C1_SDA_GPIO_PIN_NUM 2              // PTC2
#define I2C1_SDA_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for I2C1 SDA functionality
#else
#define I2C1_SCL_PORT_BASE PORTE
#define I2C1_SCL_GPIO_PIN_NUM 1              // PIN 1 in the PTE group
#define I2C1_SCL_FUNC_ALT_MODE kPORT_MuxAlt6 // ALT mode for I2C1 SCL functionality for pin 1
#define I2C1_SDA_PORT_BASE PORTE
#define I2C1_SDA_GPIO_PIN_NUM 0              // PIN 0 in the PTE group
#define I2C1_SDA_FUNC_ALT_MODE kPORT_MuxAlt6 // ALT mode for I2C1 SDA functionality for pin 0
#endif                                       // FREEDOM

#define I2C2_SCL_PORT_BASE PORTA
#define I2C2_SCL_GPIO_PIN_NUM 12             // PIN 12 in the PTA group
#define I2C2_SCL_FUNC_ALT_MODE kPORT_MuxAlt5 // ALT mode for I2C2 SCL functionality for pin 12
#define I2C2_SDA_PORT_BASE PORTA
#define I2C2_SDA_GPIO_PIN_NUM 11             // PIN 11 in the PTA group
#define I2C2_SDA_FUNC_ALT_MODE kPORT_MuxAlt5 // ALT mode for I2C2 SDA functionality for pin 11

//! SPI pinmux configurations
#define SPI0_SELECTED_PCS 0
#define SPI0_PCS_PORT_BASE PORTA
#define SPI0_PCS_GPIO_PIN_NUM 14             // PIN 14 in the PTA group
#define SPI0_PCS_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 PCS functionality for pin 14
#define SPI0_SCK_PORT_BASE PORTA
#define SPI0_SCK_GPIO_PIN_NUM 15             // PIN 15 in the PTA group
#define SPI0_SCK_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 SCK functionality for pin 15
#define SPI0_SOUT_PORT_BASE PORTA
#define SPI0_SOUT_GPIO_PIN_NUM 16             // PIN 16 in the PTA group
#define SPI0_SOUT_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 SOUT functionality for pin 16
#define SPI0_SIN_PORT_BASE PORTA
#define SPI0_SIN_GPIO_PIN_NUM 17             // PIN 17 in the PTA group
#define SPI0_SIN_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 SIN functionality for pin 17

#define SPI1_SELECTED_PCS 0
#define SPI1_PCS_PORT_BASE PORTD
#define SPI1_PCS_GPIO_PIN_NUM 4              // PIN 4 in the PTD group
#define SPI1_PCS_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI1 PCS functionality for pin 4
#define SPI1_SCK_PORT_BASE PORTD
#define SPI1_SCK_GPIO_PIN_NUM 5              // PIN 5 in the PTD group
#define SPI1_SCK_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI1 SCK functionality for pin 5
#define SPI1_SOUT_PORT_BASE PORTD
#define SPI1_SOUT_GPIO_PIN_NUM 6              // PIN 6 in the PTD group
#define SPI1_SOUT_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI1 SOUT functionality for pin 6
#define SPI1_SIN_PORT_BASE PORTD
#define SPI1_SIN_GPIO_PIN_NUM 7              // PIN 7 in the PTD group
#define SPI1_SIN_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI1 SIN functionality for pin 7

#define SPI2_SELECTED_PCS 0
#define SPI2_PCS_PORT_BASE PORTB
#define SPI2_PCS_GPIO_PIN_NUM 20             // PIN 20 in the PTB group
#define SPI2_PCS_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI2 PCS functionality for pin 20
#define SPI2_SCK_PORT_BASE PORTB
#define SPI2_SCK_GPIO_PIN_NUM 21             // PIN 21 in the PTB group
#define SPI2_SCK_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI2 SCK functionality for pin 21
#define SPI2_SOUT_PORT_BASE PORTB
#define SPI2_SOUT_GPIO_PIN_NUM 22             // PIN 22 in the PTB group
#define SPI2_SOUT_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI2 SOUT functionality for pin 22
#define SPI2_SIN_PORT_BASE PORTB
#define SPI2_SIN_GPIO_PIN_NUM 23             // PIN 23 in the PTB group
#define SPI2_SIN_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI2 SIN functionality for pin 23

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
