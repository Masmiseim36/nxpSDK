/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/* Driver name mapping. */
#define RTE_I2C0 1
#define RTE_I2C0_DMA_EN 0
#define RTE_I2C1 0
#define RTE_I2C1_DMA_EN 0

/* LPI2C configuration. */
#define RTE_I2C0_DMA_TX_CH 0
#define RTE_I2C0_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMux0LPI2C0Tx
#define RTE_I2C0_DMA_TX_DMAMUX_BASE DMAMUX
#define RTE_I2C0_DMA_TX_DMA_BASE DMA0
#define RTE_I2C0_DMA_RX_CH 1
#define RTE_I2C0_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMux0LPI2C0Rx
#define RTE_I2C0_DMA_RX_DMAMUX_BASE DMAMUX
#define RTE_I2C0_DMA_RX_DMA_BASE DMA0

#define RTE_I2C1_DMA_TX_CH 0
#define RTE_I2C1_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMux0LPI2C1Tx
#define RTE_I2C1_DMA_TX_DMAMUX_BASE DMAMUX
#define RTE_I2C1_DMA_TX_DMA_BASE DMA0
#define RTE_I2C1_DMA_RX_CH 1
#define RTE_I2C1_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMux0LPI2C1Rx
#define RTE_I2C1_DMA_RX_DMAMUX_BASE DMAMUX
#define RTE_I2C1_DMA_RX_DMA_BASE DMA0

#endif /* __RTE_DEVICE_H */
