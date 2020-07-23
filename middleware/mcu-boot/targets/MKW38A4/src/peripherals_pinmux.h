/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if !defined(__PERIPHERALS_PINMUX_H__)
#define __PERIPHERALS_PINMUX_H__

#include "fsl_device_registers.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
extern void uart_pinmux_config(uint32_t instance, pinmux_type_t pinmux);
extern void i2c_pinmux_config(uint32_t instance, pinmux_type_t pinmux);
extern void spi_pinmux_config(uint32_t instance, pinmux_type_t pinmux);
extern void can_pinmux_config(uint32_t instance, pinmux_type_t pinmux);
extern void linlpuart_pinmux_config(uint32_t instance, pinmux_type_t pinmux);

//! peripheral enable configurations
#define BL_ENABLE_PINMUX_UART0 (BL_CONFIG_LPUART)
#define BL_ENABLE_PINMUX_SPI0 (BL_CONFIG_DSPI)
#define BL_ENABLE_PINMUX_I2C0 (BL_CONFIG_I2C)
#define BL_ENABLE_PINMUX_CAN0 (BL_CONFIG_CAN)

//! UART pinmux configurations
#ifdef FREEDOM
#define UART0_RX_PORT_BASE PORTC
#define UART0_RX_GPIO_BASE GPIOC
#define UART0_RX_GPIO_PIN_NUM 6                // PIN 6 in the PTC group
#define UART0_RX_FUNC_ALT_MODE kPORT_MuxAlt4   // ALT mode for UART0 RX
#define UART0_RX_GPIO_ALT_MODE kPORT_MuxAsGpio // ALT mdoe for GPIO
#define UART0_RX_GPIO_IRQn PORTB_PORTC_IRQn
#define UART0_RX_GPIO_IRQHandler PORTB_PORTC_IRQHandler
#define UART0_TX_PORT_BASE PORTC
#define UART0_TX_GPIO_PIN_NUM 7              // PIN 7 in the PTC group
#define UART0_TX_FUNC_ALT_MODE kPORT_MuxAlt4 // ALT mode for UART0 TX 
#else
//! UART pinmux configurations
#define UART0_RX_PORT_BASE PORTC
#define UART0_RX_GPIO_BASE GPIOC
#define UART0_RX_GPIO_PIN_NUM 2                
#define UART0_RX_FUNC_ALT_MODE kPORT_MuxAlt4   // ALT mode for UART0 RX
#define UART0_RX_GPIO_ALT_MODE kPORT_MuxAsGpio // ALT mdoe for GPIO
#define UART0_RX_GPIO_IRQn PORTB_PORTC_IRQn
#define UART0_RX_GPIO_IRQHandler PORTB_PORTC_IRQHandler
#define UART0_TX_PORT_BASE PORTC
#define UART0_TX_GPIO_PIN_NUM 18              
#define UART0_TX_FUNC_ALT_MODE kPORT_MuxAlt4 // ALT mode for UART0 TX 
#endif

//! I2C pinmux configurations
#define I2C0_SCL_PORT_BASE PORTB
#define I2C0_SCL_GPIO_PIN_NUM 0              // PIN 0 in the PTB group
#define I2C0_SCL_FUNC_ALT_MODE kPORT_MuxAlt3 // ALT mode for I2C0 SCL
#define I2C0_SDA_PORT_BASE PORTB
#define I2C0_SDA_GPIO_PIN_NUM 1              // PIN 1 in the PTB group
#define I2C0_SDA_FUNC_ALT_MODE kPORT_MuxAlt3 // ALT mode for I2C0 SDA

//! SPI pinmux configurations
#define SPI0_PCS_PORT_BASE PORTC
#define SPI0_PCS_GPIO_PIN_NUM 19             // PIN 19 in the PTC group
#define SPI0_PCS_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 PCS
#define SPI0_SCK_PORT_BASE PORTC
#define SPI0_SCK_GPIO_PIN_NUM 16             // PIN 16 in the PTC group
#define SPI0_SCK_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 SCK
#define SPI0_SOUT_PORT_BASE PORTC
#define SPI0_SOUT_GPIO_PIN_NUM 17             // PIN 17 in the PTC group
#define SPI0_SOUT_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 SOUT
#define SPI0_SIN_PORT_BASE PORTC
#define SPI0_SIN_GPIO_PIN_NUM 18             // PIN 18 in the PTC group
#define SPI0_SIN_FUNC_ALT_MODE kPORT_MuxAlt2 // ALT mode for SPI0 SIN

//! CAN pinmux configurations
#define CAN0_RX_PORT_BASE PORTC
#define CAN0_RX_GPIO_PIN_NUM 4             // PIN 4 in the PTC group
#define CAN0_RX_FUNC_ALT_MODE 9 // ALT mode for CAN0 RX
#define CAN0_TX_PORT_BASE PORTC
#define CAN0_TX_GPIO_PIN_NUM 3             // PIN 3 in the PTC group
#define CAN0_TX_FUNC_ALT_MODE 9 // ALT mode for CAN0 TX

//! LIN LPUART configurations
#define LIN_LPUART1_RX_PORT_BASE PORTA
#define LIN_LPUART1_RX_GPIO_PIN_NUM 17             // PIN 17 in the PTA group
#define LIN_LPUART1_RX_FUNC_ALT_MODE 3             // ALT mode for LPUART1 RX
#define LIN_LPUART1_TX_PORT_BASE PORTA
#define LIN_LPUART1_TX_GPIO_PIN_NUM 18             // PIN 18 in the PTA group
#define LIN_LPUART1_TX_FUNC_ALT_MODE 3             // ALT mode for LPUART1 TX

#endif // __PERIPHERALS_PINMUX_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
