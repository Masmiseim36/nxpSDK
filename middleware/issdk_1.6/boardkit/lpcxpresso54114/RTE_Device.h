/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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
 * o Neither the name of the copyright holder nor the names of its
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

/*Driver name mapping*/
#define RTE_I2C4 1
#define RTE_I2C4_DMA_EN 0
#define RTE_I2C5 1
#define RTE_I2C5_DMA_EN 0

#define RTE_SPI3 0
#define RTE_SPI3_DMA_EN 0
#define RTE_SPI5 1
#define RTE_SPI5_DMA_EN 0

#define RTE_USART0 1
#define RTE_USART0_DMA_EN 0

/* UART configuration. */
#define USART_RX_BUFFER_LEN 64
#define USART0_RX_BUFFER_ENABLE 1

/* I2C configuration */
#define RTE_I2C4_Master_DMA_BASE DMA0
#define RTE_I2C4_Master_DMA_CH 9

#define RTE_I2C5_Master_DMA_BASE DMA0
#define RTE_I2C5_Master_DMA_CH 11

/* SPI configuration. */
#define RTE_SPI0_SSEL_NUM kSPI_Ssel0
#define RTE_SPI0_DMA_TX_CH 1
#define RTE_SPI0_DMA_TX_DMA_BASE DMA0
#define RTE_SPI0_DMA_RX_CH 0
#define RTE_SPI0_DMA_RX_DMA_BASE DMA0

#define RTE_SPI1_SSEL_NUM kSPI_Ssel0
#define RTE_SPI1_DMA_TX_CH 3
#define RTE_SPI1_DMA_TX_DMA_BASE DMA0
#define RTE_SPI1_DMA_RX_CH 2
#define RTE_SPI1_DMA_RX_DMA_BASE DMA0

#define RTE_SPI2_SSEL_NUM kSPI_Ssel0
#define RTE_SPI2_DMA_TX_CH 5
#define RTE_SPI2_DMA_TX_DMA_BASE DMA0
#define RTE_SPI2_DMA_RX_CH 4
#define RTE_SPI2_DMA_RX_DMA_BASE DMA0

#define RTE_SPI3_SSEL_NUM kSPI_Ssel2
#define RTE_SPI3_DMA_TX_CH 7
#define RTE_SPI3_DMA_TX_DMA_BASE DMA0
#define RTE_SPI3_DMA_RX_CH 6
#define RTE_SPI3_DMA_RX_DMA_BASE DMA0

#define RTE_SPI4_SSEL_NUM kSPI_Ssel0
#define RTE_SPI4_DMA_TX_CH 9
#define RTE_SPI4_DMA_TX_DMA_BASE DMA0
#define RTE_SPI4_DMA_RX_CH 8
#define RTE_SPI4_DMA_RX_DMA_BASE DMA0

#define RTE_SPI5_SSEL_NUM kSPI_Ssel2
#define RTE_SPI5_DMA_TX_CH 11
#define RTE_SPI5_DMA_TX_DMA_BASE DMA0
#define RTE_SPI5_DMA_RX_CH 10
#define RTE_SPI5_DMA_RX_DMA_BASE DMA0

#define RTE_SPI6_SSEL_NUM kSPI_Ssel0
#define RTE_SPI6_DMA_TX_CH 13
#define RTE_SPI6_DMA_TX_DMA_BASE DMA0
#define RTE_SPI6_DMA_RX_CH 12
#define RTE_SPI6_DMA_RX_DMA_BASE DMA0

#define RTE_SPI7_SSEL_NUM kSPI_Ssel0
#define RTE_SPI7_DMA_TX_CH 15
#define RTE_SPI7_DMA_TX_DMA_BASE DMA0
#define RTE_SPI7_DMA_RX_CH 14
#define RTE_SPI7_DMA_RX_DMA_BASE DMA0

/* USART configuration. */
#define RTE_USART0_DMA_TX_CH 1
#define RTE_USART0_DMA_TX_DMA_BASE DMA0
#define RTE_USART0_DMA_RX_CH 0
#define RTE_USART0_DMA_RX_DMA_BASE DMA0

#endif /* __RTE_DEVICE_H */
