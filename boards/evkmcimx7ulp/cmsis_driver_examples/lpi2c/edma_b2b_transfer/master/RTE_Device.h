/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/* Driver name mapping. */
#define RTE_I2C0 0
#define RTE_I2C0_DMA_EN 0
#define RTE_I2C3 1
#define RTE_I2C3_DMA_EN 1

/* LPI2C configuration. */
#define RTE_I2C0_DMA_TX_CH 0
#define RTE_I2C0_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMux0LPI2C0Tx
#define RTE_I2C0_DMA_TX_DMAMUX_BASE DMA_CH_MUX0
#define RTE_I2C0_DMA_TX_DMA_BASE DMA0
#define RTE_I2C0_DMA_RX_CH 1
#define RTE_I2C0_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMux0LPI2C0Rx
#define RTE_I2C0_DMA_RX_DMAMUX_BASE DMA_CH_MUX0
#define RTE_I2C0_DMA_RX_DMA_BASE DMA0

#define RTE_I2C3_DMA_TX_CH 0
#define RTE_I2C3_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMux0LPI2C3Tx
#define RTE_I2C3_DMA_TX_DMAMUX_BASE DMA_CH_MUX0
#define RTE_I2C3_DMA_TX_DMA_BASE DMA0
#define RTE_I2C3_DMA_RX_CH 1
#define RTE_I2C3_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMux0LPI2C3Rx
#define RTE_I2C3_DMA_RX_DMAMUX_BASE DMA_CH_MUX0
#define RTE_I2C3_DMA_RX_DMA_BASE DMA0

#endif /* __RTE_DEVICE_H */
