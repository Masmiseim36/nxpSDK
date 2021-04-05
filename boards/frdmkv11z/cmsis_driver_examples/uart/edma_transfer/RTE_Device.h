/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void UART0_InitPins();
extern void UART0_DeinitPins();

/* Driver name mapping. */
/* User needs to provide the implementation of UARTX_GetFreq/UARTX_InitPins/UARTX_DeinitPins for the enabled UART
 * instance. */
#define RTE_USART0        1
#define RTE_USART0_DMA_EN 1

/* UART configuration. */
#define RTE_USART0_PIN_INIT           UART0_InitPins
#define RTE_USART0_PIN_DEINIT         UART0_DeinitPins
#define RTE_USART0_DMA_TX_CH          0
#define RTE_USART0_DMA_TX_PERI_SEL    (uint8_t) kDmaRequestMux0UART0Tx
#define RTE_USART0_DMA_TX_DMAMUX_BASE DMAMUX0
#define RTE_USART0_DMA_TX_DMA_BASE    DMA0
#define RTE_USART0_DMA_RX_CH          1
#define RTE_USART0_DMA_RX_PERI_SEL    (uint8_t) kDmaRequestMux0UART0Rx
#define RTE_USART0_DMA_RX_DMAMUX_BASE DMAMUX0
#define RTE_USART0_DMA_RX_DMA_BASE    DMA0

#endif /* _RTE_DEVICE_H */
