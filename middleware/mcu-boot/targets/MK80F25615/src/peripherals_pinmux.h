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
#define BL_ENABLE_PINMUX_UART4 (BL_CONFIG_LPUART_4)
#define BL_ENABLE_PINMUX_SPI0 (BL_CONFIG_DSPI_0)
#define BL_ENABLE_PINMUX_SPI1 (BL_CONFIG_DSPI_1)
#define BL_ENABLE_PINMUX_I2C0 (BL_CONFIG_I2C_0)
#define BL_ENABLE_PINMUX_I2C1 (BL_CONFIG_I2C_1)

//! UART pinmux configurations
#define UART0_RX_PORT_BASE PORTB
#define UART0_RX_GPIO_BASE GPIOB
#define UART0_RX_GPIO_PIN_NUM 16               // PTB16
#define UART0_RX_FUNC_ALT_MODE kPORT_MuxAlt3   // ALT mode for UART0 RX
#define UART0_RX_GPIO_ALT_MODE kPORT_MuxAsGpio // ALT mdoe for GPIO
#define UART0_RX_GPIO_IRQn PORTB_IRQn
#define UART0_RX_GPIO_IRQHandler PORTB_IRQHandler
#define UART0_TX_PORT_BASE PORTB
#define UART0_TX_GPIO_PIN_NUM 17             // PTB17
#define UART0_TX_FUNC_ALT_MODE kPORT_MuxAlt3 // ALT mode for UART0 TX

#define UART1_RX_PORT_BASE PORTC
#define UART1_RX_GPIO_BASE GPIOC
#define UART1_RX_GPIO_PIN_NUM 3                // PTC3
#define UART1_RX_FUNC_ALT_MODE kPORT_MuxAlt3   // ALT mode for UART1 RX
#define UART1_RX_GPIO_ALT_MODE kPORT_MuxAsGpio // ALT mdoe for GPIO
#define UART1_RX_GPIO_IRQn PORTC_IRQn
#define UART1_RX_GPIO_IRQHandler PORTC_IRQHandler
#define UART1_TX_PORT_BASE PORTC
#define UART1_TX_GPIO_PIN_NUM 4              // PTC4
#define UART1_TX_FUNC_ALT_MODE kPORT_MuxAlt3 // ALT mode for UART1 TX

#define UART2_RX_PORT_BASE PORTD
#define UART2_RX_GPIO_BASE GPIOD
#define UART2_RX_GPIO_PIN_NUM 2                // PTD2
#define UART2_RX_FUNC_ALT_MODE kPORT_MuxAlt3   // ALT mode for UART2 RX
#define UART2_RX_GPIO_ALT_MODE kPORT_MuxAsGpio // ALT mdoe for GPIO
#define UART2_RX_GPIO_IRQn PORTD_IRQn
#define UART2_RX_GPIO_IRQHandler PORTD_IRQHandler
#define UART2_TX_PORT_BASE PORTD
#define UART2_TX_GPIO_PIN_NUM 3              // PTD3
#define UART2_TX_FUNC_ALT_MODE kPORT_MuxAlt3 // ALT mode for UART2 TX

//! I2C pinmux configurations - ROM I2C0
#define I2C0_SCL_PORT_BASE PORTB
#define I2C0_SCL_GPIO_PIN_NUM 2              // PTB2
#define I2C0_SCL_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for I2C0 SCL
#define I2C0_SDA_PORT_BASE PORTB
#define I2C0_SDA_GPIO_PIN_NUM 3              // PTB3
#define I2C0_SDA_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for I2C0 SDA

//! I2C pinmux configurations - ROM I2C1
#define I2C1_SCL_PORT_BASE PORTC
#define I2C1_SCL_GPIO_PIN_NUM 10             // PTC10
#define I2C1_SCL_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for I2C1 SCL
#define I2C1_SDA_PORT_BASE PORTC
#define I2C1_SDA_GPIO_PIN_NUM 11             // PTC11
#define I2C1_SDA_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for I2C1 SDA

//! SPI pinmux configurations - ROM SPI0
#define SPI0_PCS_PORT_BASE PORTA
#define SPI0_PCS_GPIO_PIN_NUM 14             // PTA14
#define SPI0_PCS_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 PCS
#define SPI0_SCK_PORT_BASE PORTA
#define SPI0_SCK_GPIO_PIN_NUM 15             // PTA15
#define SPI0_SCK_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 SCK
#define SPI0_SOUT_PORT_BASE PORTA
#define SPI0_SOUT_GPIO_PIN_NUM 16             // PTA16
#define SPI0_SOUT_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 SOUT
#define SPI0_SIN_PORT_BASE PORTA
#define SPI0_SIN_GPIO_PIN_NUM 17             // PTA17
#define SPI0_SIN_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 SIN

//! SPI pinmux configurations - ROM SPI1
#define SPI1_PCS_PORT_BASE PORTD
#define SPI1_PCS_GPIO_PIN_NUM 4              // PTD4
#define SPI1_PCS_FUNC_ALT_MODE kPORT_MuxAlt7 // ALT mode for SPI1 PCS
#define SPI1_SCK_PORT_BASE PORTD
#define SPI1_SCK_GPIO_PIN_NUM 5              // PTD5
#define SPI1_SCK_FUNC_ALT_MODE kPORT_MuxAlt7 // ALT mode for SPI1 SCK
#define SPI1_SOUT_PORT_BASE PORTD
#define SPI1_SOUT_GPIO_PIN_NUM 6              // PTD6
#define SPI1_SOUT_FUNC_ALT_MODE kPORT_MuxAlt7 // ALT mode for SPI1 SOUT
#define SPI1_SIN_PORT_BASE PORTD
#define SPI1_SIN_GPIO_PIN_NUM 7              // PTD7
#define SPI1_SIN_FUNC_ALT_MODE kPORT_MuxAlt7 // ALT mode for SPI1 SIN

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
