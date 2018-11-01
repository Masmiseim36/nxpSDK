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
#define BL_ENABLE_PINMUX_UART2 (BL_CONFIG_SCUART)
#define BL_ENABLE_PINMUX_I2C0 (BL_CONFIG_I2C)
#define BL_ENABLE_PINMUX_SPI2 (BL_CONFIG_DSPI)
#elif defined FREEDOM
#define BL_ENABLE_PINMUX_UART0 (BL_CONFIG_SCUART)
#define BL_ENABLE_PINMUX_I2C1 (BL_CONFIG_I2C)
#define BL_ENABLE_PINMUX_SPI0 (BL_CONFIG_DSPI)
#else
#define BL_ENABLE_PINMUX_UART4 (BL_CONFIG_SCUART)
#define BL_ENABLE_PINMUX_I2C0 (BL_CONFIG_I2C)
#define BL_ENABLE_PINMUX_SPI2 (BL_CONFIG_DSPI)
#endif // TOWER

//! UART pinmux configurations
#if defined(TOWER) // TWR board and tower_bootloader
#define UART2_RX_PORT_BASE PORTE
#define UART2_RX_GPIO_BASE PTE
#define UART2_RX_GPIO_PIN_NUM 17               // PTE17
#define UART2_RX_FUNC_ALT_MODE kPORT_MuxAlt3   // ALT mode for UART1 RX functionality
#define UART2_RX_GPIO_ALT_MODE kPORT_MuxAsGpio // ALT mdoe for GPIO functionality
#define UART2_RX_GPIO_IRQn PORTE_IRQn
#define UART2_RX_GPIO_IRQHandler PORTE_IRQHandler
#define UART2_TX_PORT_BASE PORTE
#define UART2_TX_GPIO_PIN_NUM 16             // PTE16
#define UART2_TX_FUNC_ALT_MODE kPORT_MuxAlt3 // ALT mode for UART1 TX functionality
#elif defined(FREEDOM)
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
#else                                        // flashloader
#define UART4_RX_PORT_BASE PORTE
#define UART4_RX_GPIO_BASE PTE
#define UART4_RX_GPIO_PIN_NUM 25             // PTE25
#define UART4_RX_FUNC_ALT_MODE kPORT_MuxAlt3 // ALT mode for UART1 RX functionality
#define UART4_RX_GPIO_ALT_MODE 1             // ALT mdoe for GPIO functionality
#define UART4_RX_GPIO_IRQn PORTE_IRQn
#define UART4_RX_GPIO_IRQHandler PORTE_IRQHandler
#define UART4_TX_PORT_BASE PORTE
#define UART4_TX_GPIO_PIN_NUM 24             // PTE24
#define UART4_TX_FUNC_ALT_MODE kPORT_MuxAlt3 // ALT mode for UART1 TX functionality
#endif                                       // TOWER

//! I2C pinmux configurations
#if defined FREEDOM
#define I2C1_SCL_PORT_BASE PORTC
#define I2C1_SCL_GPIO_PIN_NUM 10             // PTC10
#define I2C1_SCL_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for I2C1 SCL functionality
#define I2C1_SDA_PORT_BASE PORTC
#define I2C1_SDA_GPIO_PIN_NUM 11             // PTC11
#define I2C1_SDA_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for I2C1 SDA functionality
#else
#define I2C0_SCL_PORT_BASE PORTD
#define I2C0_SCL_GPIO_PIN_NUM 8              // PTD8
#define I2C0_SCL_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for I2C1 SCL functionality
#define I2C0_SDA_PORT_BASE PORTD
#define I2C0_SDA_GPIO_PIN_NUM 9              // PTD9
#define I2C0_SDA_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for I2C1 SDA functionality
#endif                                       // FREEDOM

//! SPI pinmux configurations
#if defined FREEDOM
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
#else
#define SPI2_PCS_PORT_BASE PORTD
#define SPI2_PCS_GPIO_PIN_NUM 11             // PTD11
#define SPI2_PCS_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI1 PCS functionality
#define SPI2_SCK_PORT_BASE PORTD
#define SPI2_SCK_GPIO_PIN_NUM 12             // PTD12
#define SPI2_SCK_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI1 SCK functionality
#define SPI2_SOUT_PORT_BASE PORTD
#define SPI2_SOUT_GPIO_PIN_NUM 13             // PTD13
#define SPI2_SOUT_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI1 SOUT functionality
#define SPI2_SIN_PORT_BASE PORTD
#define SPI2_SIN_GPIO_PIN_NUM 14             // PTD14
#define SPI2_SIN_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI1 SIN functionality
#endif

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
