/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void LPUART1_InitPins();
extern void LPUART1_DeinitPins();

/* USART instance mapping */
#define LPUART0 CM4__LPUART
#define LPUART1 ADMA__LPUART0
#define LPUART2 ADMA__LPUART1
#define LPUART3 ADMA__LPUART2
#define LPUART4 ADMA__LPUART3

/* Driver name mapping. */
/* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
 * LPUART instance. */
#define RTE_USART1        1
#define RTE_USART1_DMA_EN 0

/* UART configuration. */
#define USART_RX_BUFFER_LEN 64

#define RTE_USART1_PIN_INIT        LPUART1_InitPins
#define RTE_USART1_PIN_DEINIT      LPUART1_DeinitPins
#define RTE_USART1_DMA_TX_CH       9
#define RTE_USART1_DMA_TX_PERI_SEL 9
#define RTE_USART1_DMA_TX_DMA_BASE ADMA__EDMA2
#define RTE_USART1_DMA_RX_CH       8
#define RTE_USART1_DMA_RX_PERI_SEL 8
#define RTE_USART1_DMA_RX_DMA_BASE ADMA__EDMA2

#endif /* _RTE_DEVICE_H */
