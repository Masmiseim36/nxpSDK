/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/* Driver name mapping. */
#define RTE_USART4 1
#define RTE_USART4_DMA_EN 1

/* UART configuration. */
#define USART_RX_BUFFER_LEN 64
#define USART4_RX_BUFFER_ENABLE 0

#define RTE_USART4_SDMA_TX_CH 2
#define RTE_USART4_SDMA_TX_REQUEST (29)
#define RTE_USART4_SDMA_TX_DMA_BASE SDMAARM1
#define RTE_USART4_SDMA_TX_PRIORITY (3)
#define RTE_USART4_SDMA_RX_CH 1
#define RTE_USART4_SDMA_RX_REQUEST (28)
#define RTE_USART4_SDMA_RX_DMA_BASE SDMAARM1
#define RTE_USART4_SDMA_RX_PRIORITY (4)

#endif /* __RTE_DEVICE_H */
