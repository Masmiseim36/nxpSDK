/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/* Driver name mapping. */
#define LPUART0_Edma_Driver Driver_USART0
#define LPUART1_Edma_Driver Driver_USART1
#define LPUART2_Edma_Driver Driver_USART2
#define LPUART4_Edma_Driver Driver_USART4

/* UART configuration. */
#define RTE_LPUART0_DMA_TX_CH 0
#define RTE_LPUART0_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMux0LPUART0Tx
#define RTE_LPUART0_DMA_TX_DMAMUX_BASE DMAMUX0
#define RTE_LPUART0_DMA_TX_DMA_BASE DMA0
#define RTE_LPUART0_DMA_RX_CH 1
#define RTE_LPUART0_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMux0LPUART0Rx
#define RTE_LPUART0_DMA_RX_DMAMUX_BASE DMAMUX0
#define RTE_LPUART0_DMA_RX_DMA_BASE DMA0

#define RTE_LPUART1_DMA_TX_CH 0
#define RTE_LPUART1_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMux0LPUART1Tx
#define RTE_LPUART1_DMA_TX_DMAMUX_BASE DMAMUX0
#define RTE_LPUART1_DMA_TX_DMA_BASE DMA0
#define RTE_LPUART1_DMA_RX_CH 1
#define RTE_LPUART1_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMux0LPUART1Rx
#define RTE_LPUART1_DMA_RX_DMAMUX_BASE DMAMUX0
#define RTE_LPUART1_DMA_RX_DMA_BASE DMA0

#define RTE_LPUART2_DMA_TX_CH 0
#define RTE_LPUART2_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMux0LPUART2Tx
#define RTE_LPUART2_DMA_TX_DMAMUX_BASE DMAMUX0
#define RTE_LPUART2_DMA_TX_DMA_BASE DMA0
#define RTE_LPUART2_DMA_RX_CH 1
#define RTE_LPUART2_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMux0LPUART2Rx
#define RTE_LPUART2_DMA_RX_DMAMUX_BASE DMAMUX0
#define RTE_LPUART2_DMA_RX_DMA_BASE DMA0

#define RTE_LPUART3_DMA_TX_CH 0
#define RTE_LPUART3_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMux0LPUART3Tx
#define RTE_LPUART3_DMA_TX_DMAMUX_BASE DMAMUX0
#define RTE_LPUART3_DMA_TX_DMA_BASE DMA0
#define RTE_LPUART3_DMA_RX_CH 1
#define RTE_LPUART3_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMux0LPUART3Rx
#define RTE_LPUART3_DMA_RX_DMAMUX_BASE DMAMUX0
#define RTE_LPUART3_DMA_RX_DMA_BASE DMA0

#define RTE_LPUART4_DMA_TX_CH 0
#define RTE_LPUART4_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMux0LPUART4Tx
#define RTE_LPUART4_DMA_TX_DMAMUX_BASE DMAMUX0
#define RTE_LPUART4_DMA_TX_DMA_BASE DMA0
#define RTE_LPUART4_DMA_RX_CH 1
#define RTE_LPUART4_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMux0LPUART4Rx
#define RTE_LPUART4_DMA_RX_DMAMUX_BASE DMAMUX0
#define RTE_LPUART4_DMA_RX_DMA_BASE DMA0

#endif /* __RTE_DEVICE_H */
