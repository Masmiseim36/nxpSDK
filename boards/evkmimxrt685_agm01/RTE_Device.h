/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void I2C2_InitPins();
extern void I2C2_DeinitPins();
extern void USART0_InitPins();
extern void USART0_DeinitPins();
extern void USART0_InitPins();
extern void USART0_DeinitPins();
extern void SPI5_InitPins();
extern void SPI5_DeinitPins();
extern void SPI5_InitPins();
extern void SPI5_DeinitPins();

/*Driver name mapping*/
/* User needs to provide the implementation of I2CX_GetFreq/I2CX_InitPins/I2CX_DeinitPins for the enabled I2C instance.
 */
#define RTE_I2C2            1
#define RTE_I2C2_PIN_INIT   I2C2_InitPins
#define RTE_I2C2_PIN_DEINIT I2C2_DeinitPins
#define RTE_I2C2_DMA_EN     0

/*I2C configuration*/

/* Driver name mapping. */
/* User needs to provide the implementation of USARTX_GetFreq/USARTX_InitPins/USARTX_DeinitPins for the enabled USART
 * instance. */
#define RTE_USART0            1
#define RTE_USART0_PIN_INIT   USART0_InitPins
#define RTE_USART0_PIN_DEINIT USART0_DeinitPins
#define RTE_USART0_DMA_EN     0

/* USART configuration. */
#define USART_RX_BUFFER_LEN     64
#define USART0_RX_BUFFER_ENABLE 1
#define USART1_RX_BUFFER_ENABLE 0
#define USART2_RX_BUFFER_ENABLE 0
#define USART3_RX_BUFFER_ENABLE 0
#define USART4_RX_BUFFER_ENABLE 0
#define USART5_RX_BUFFER_ENABLE 0

#define RTE_USART0_PIN_INIT        USART0_InitPins
#define RTE_USART0_PIN_DEINIT      USART0_DeinitPins
#define RTE_USART0_DMA_TX_CH       1
#define RTE_USART0_DMA_TX_DMA_BASE DMA0
#define RTE_USART0_DMA_RX_CH       0
#define RTE_USART0_DMA_RX_DMA_BASE DMA0

/*Driver name mapping*/
/* User needs to provide the implementation of SPIX_GetFreq/SPIX_InitPins/SPIX_DeinitPins for the enabled SPI instance.
 */
#define RTE_SPI5            1
#define RTE_SPI5_PIN_INIT   SPI5_InitPins
#define RTE_SPI5_PIN_DEINIT SPI5_DeinitPins
#define RTE_SPI5_DMA_EN     0

/* SPI configuration. */

#define RTE_SPI5_SSEL_NUM        kSPI_Ssel0
#define RTE_SPI5_PIN_INIT        SPI5_InitPins
#define RTE_SPI5_PIN_DEINIT      SPI5_DeinitPins
#define RTE_SPI5_DMA_TX_CH       11
#define RTE_SPI5_DMA_TX_DMA_BASE DMA0
#define RTE_SPI5_DMA_RX_CH       10
#define RTE_SPI5_DMA_RX_DMA_BASE DMA0

#endif /* _RTE_DEVICE_H */
