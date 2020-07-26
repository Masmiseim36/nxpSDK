/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/* Driver name mapping. */
#define RTE_I2C1        1
#define RTE_I2C1_DMA_EN 0
#define RTE_I2C2        0
#define RTE_I2C2_DMA_EN 0
#define RTE_I2C3        0
#define RTE_I2C3_DMA_EN 0
#define RTE_I2C4        0
#define RTE_I2C4_DMA_EN 0

/* LPI2C configuration. */
#define RTE_I2C1_DMA_TX_CH          0
#define RTE_I2C1_DMA_TX_PERI_SEL    (uint8_t) kDmaRequestMuxLPI2C1
#define RTE_I2C1_DMA_TX_DMAMUX_BASE DMAMUX
#define RTE_I2C1_DMA_TX_DMA_BASE    DMA0
#define RTE_I2C1_DMA_RX_CH          1
#define RTE_I2C1_DMA_RX_PERI_SEL    (uint8_t) kDmaRequestMuxLPI2C1
#define RTE_I2C1_DMA_RX_DMAMUX_BASE DMAMUX
#define RTE_I2C1_DMA_RX_DMA_BASE    DMA0

#define RTE_I2C2_DMA_TX_CH          2
#define RTE_I2C2_DMA_TX_PERI_SEL    (uint8_t) kDmaRequestMuxLPI2C2
#define RTE_I2C2_DMA_TX_DMAMUX_BASE DMAMUX
#define RTE_I2C2_DMA_TX_DMA_BASE    DMA0
#define RTE_I2C2_DMA_RX_CH          3
#define RTE_I2C2_DMA_RX_PERI_SEL    (uint8_t) kDmaRequestMuxLPI2C2
#define RTE_I2C2_DMA_RX_DMAMUX_BASE DMAMUX
#define RTE_I2C2_DMA_RX_DMA_BASE    DMA0

#define RTE_I2C3_DMA_TX_CH          4
#define RTE_I2C3_DMA_TX_PERI_SEL    (uint8_t) kDmaRequestMuxLPI2C3
#define RTE_I2C3_DMA_TX_DMAMUX_BASE DMAMUX
#define RTE_I2C3_DMA_TX_DMA_BASE    DMA0
#define RTE_I2C3_DMA_RX_CH          5
#define RTE_I2C3_DMA_RX_PERI_SEL    (uint8_t) kDmaRequestMuxLPI2C3
#define RTE_I2C3_DMA_RX_DMAMUX_BASE DMAMUX
#define RTE_I2C3_DMA_RX_DMA_BASE    DMA0

#define RTE_I2C4_DMA_TX_CH          6
#define RTE_I2C4_DMA_TX_PERI_SEL    (uint8_t) kDmaRequestMuxLPI2C4
#define RTE_I2C4_DMA_TX_DMAMUX_BASE DMAMUX
#define RTE_I2C4_DMA_TX_DMA_BASE    DMA0
#define RTE_I2C4_DMA_RX_CH          7
#define RTE_I2C4_DMA_RX_PERI_SEL    (uint8_t) kDmaRequestMuxLPI2C4
#define RTE_I2C4_DMA_RX_DMAMUX_BASE DMAMUX
#define RTE_I2C4_DMA_RX_DMA_BASE    DMA0

#endif /* __RTE_DEVICE_H */
