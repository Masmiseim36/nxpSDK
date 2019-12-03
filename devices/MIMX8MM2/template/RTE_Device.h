/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2018 NXP
  * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/* UART select, UART1-UART4 */
/* User needs to provide the implementation for XXX_GetFreq/XXX_InitPins/XXX_DeinitPins
in the application for enabling according instance. */
#define RTE_USART1 0
#define RTE_USART1_DMA_EN 0
#define RTE_USART2 0
#define RTE_USART2_DMA_EN 0
#define RTE_USART3 0
#define RTE_USART3_DMA_EN 0
#define RTE_USART4 0
#define RTE_USART4_DMA_EN 0

/* I2C select, I2C1 - I2C4. */
/* User needs to provide the implementation for XXX_GetFreq/XXX_InitPins/XXX_DeinitPins
in the application for enabling according instance. */
#define RTE_I2C1 0
#define RTE_I2C1_DMA_EN 0
#define RTE_I2C2 0
#define RTE_I2C2_DMA_EN 0
#define RTE_I2C3 0
#define RTE_I2C3_DMA_EN 0
#define RTE_I2C4 0
#define RTE_I2C4_DMA_EN 0

/* SPI select, ECSPI1 - ECSPI3. */
/* User needs to provide the implementation for XXX_GetFreq/XXX_InitPins/XXX_DeinitPins
in the application for enabling according instance. */
#define RTE_SPI0 0
#define RTE_SPI0_DMA_EN 0
#define RTE_SPI1 0
#define RTE_SPI1_DMA_EN 0
#define RTE_SPI2 0
#define RTE_SPI2_DMA_EN 0

#endif /* __RTE_DEVICE_H */
