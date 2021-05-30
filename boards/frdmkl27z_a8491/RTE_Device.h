/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void I2C0_InitPins();
extern void I2C0_DeinitPins();
extern void I2C1_InitPins();
extern void I2C1_DeinitPins();
extern void SPI0_InitPins();
extern void SPI0_DeinitPins();
extern void LPUART0_InitPins();
extern void LPUART0_DeinitPins();
extern void LPUART0_InitPins();
extern void LPUART0_DeinitPins();
extern void SPI0_InitPins();
extern void SPI0_DeinitPins();

/*Driver name mapping*/
/* User needs to provide the implementation of I2CX_GetFreq/I2CX_InitPins/I2CX_DeinitPins for the enabled I2C instance.
 */
#define RTE_I2C0            1
#define RTE_I2C0_PIN_INIT   I2C0_InitPins
#define RTE_I2C0_PIN_DEINIT I2C0_DeinitPins
#define RTE_I2C0_DMA_EN     0
#define RTE_I2C1            1
#define RTE_I2C1_PIN_INIT   I2C1_InitPins
#define RTE_I2C1_PIN_DEINIT I2C1_DeinitPins
#define RTE_I2C1_DMA_EN     0

/* User needs to provide the implementation of SPIX_GetFreq/SPIX_InitPins/SPIX_DeinitPins for the enabled SPI instance.
 */
#define RTE_SPI0            1
#define RTE_SPI0_PIN_INIT   SPI0_InitPins
#define RTE_SPI0_PIN_DEINIT SPI0_DeinitPins
#define RTE_SPI0_DMA_EN     0

/* User needs to provide the implementation of UARTX_GetFreq/UARTX_InitPins/UARTX_DeinitPins for the enabled UART
 * instance. */
#define RTE_USART0            1
#define RTE_USART0_PIN_INIT   LPUART0_InitPins
#define RTE_USART0_PIN_DEINIT LPUART0_DeinitPins
#define RTE_USART0_DMA_EN     0

/* UART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART0_RX_BUFFER_ENABLE 1

#define RTE_USART0_PIN_INIT           LPUART0_InitPins
#define RTE_USART0_PIN_DEINIT         LPUART0_DeinitPins
#define RTE_USART0_DMA_TX_CH          0
#define RTE_USART0_DMA_TX_PERI_SEL    (uint8_t) kDmaRequestMux0LPUART0Tx
#define RTE_USART0_DMA_TX_DMAMUX_BASE DMAMUX0
#define RTE_USART0_DMA_TX_DMA_BASE    DMA0
#define RTE_USART0_DMA_RX_CH          1
#define RTE_USART0_DMA_RX_PERI_SEL    (uint8_t) kDmaRequestMux0LPUART0Rx
#define RTE_USART0_DMA_RX_DMAMUX_BASE DMAMUX0
#define RTE_USART0_DMA_RX_DMA_BASE    DMA0

/* SPI configuration. */
#define RTE_SPI0_PIN_INIT           SPI0_InitPins
#define RTE_SPI0_PIN_DEINIT         SPI0_DeinitPins
#define RTE_SPI0_DMA_TX_CH          0
#define RTE_SPI0_DMA_TX_PERI_SEL    (uint8_t) kDmaRequestMux0SPI0Tx
#define RTE_SPI0_DMA_TX_DMAMUX_BASE DMAMUX0
#define RTE_SPI0_DMA_TX_DMA_BASE    DMA0
#define RTE_SPI0_DMA_RX_CH          1
#define RTE_SPI0_DMA_RX_PERI_SEL    (uint8_t) kDmaRequestMux0SPI0Rx
#define RTE_SPI0_DMA_RX_DMAMUX_BASE DMAMUX0
#define RTE_SPI0_DMA_RX_DMA_BASE    DMA0

/*I2C configuration*/
#define RTE_I2C0_Master_DMA_BASE    DMA0
#define RTE_I2C0_Master_DMA_CH      0
#define RTE_I2C0_Master_DMAMUX_BASE DMAMUX0
#define RTE_I2C0_Master_PERI_SEL    kDmaRequestMux0I2C0

#define RTE_I2C1_Master_DMA_BASE    DMA0
#define RTE_I2C1_Master_DMA_CH      1
#define RTE_I2C1_Master_DMAMUX_BASE DMAMUX0
#define RTE_I2C1_Master_PERI_SEL    kDmaRequestMux0I2C1

#endif /* _RTE_DEVICE_H */
