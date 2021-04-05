/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void LPUART4_InitPins();
extern void LPUART4_DeinitPins();

/* USART instance mapping */
#define LPUART0 CM4_0__LPUART
#define LPUART1 CM4_1__LPUART
#define LPUART2 DMA__LPUART0
#define LPUART3 DMA__LPUART1
#define LPUART4 DMA__LPUART2
#define LPUART5 DMA__LPUART3
#define LPUART6 DMA__LPUART4

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART4        1
#define RTE_USART4_DMA_EN 0

/* UART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART4_RX_BUFFER_ENABLE 1

/* Note: LPUART0, LPUART1 not support DMA mode */

#define RTE_USART4_PIN_INIT        LPUART4_InitPins
#define RTE_USART4_PIN_DEINIT      LPUART4_DeinitPins
#define RTE_USART4_DMA_TX_CH       17
#define RTE_USART4_DMA_TX_PERI_SEL 17
#define RTE_USART4_DMA_TX_DMA_BASE DMA__EDMA0
#define RTE_USART4_DMA_RX_CH       16
#define RTE_USART4_DMA_RX_PERI_SEL 16
#define RTE_USART4_DMA_RX_DMA_BASE DMA__EDMA0

#endif /* _RTE_DEVICE_H */
