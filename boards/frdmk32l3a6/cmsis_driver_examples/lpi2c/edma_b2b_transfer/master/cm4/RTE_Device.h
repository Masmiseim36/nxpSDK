/*
 * Copyright 2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/* M0+ core configuration */
#ifdef CPU_K32L3A60VPJ1A_cm0plus
/* Driver name mapping. */
#define RTE_I2C3 1
#define RTE_I2C3_DMA_EN 1

/* LPI2C configuration. */
#define RTE_I2C0_DMA_TX_CH 0
#define RTE_I2C0_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMux0LPI2C0Tx
#define RTE_I2C0_DMA_TX_DMAMUX_BASE DMAMUX0
#define RTE_I2C0_DMA_TX_DMA_BASE DMA0
#define RTE_I2C0_DMA_RX_CH 1
#define RTE_I2C0_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMux0LPI2C0Rx
#define RTE_I2C0_DMA_RX_DMAMUX_BASE DMAMUX0
#define RTE_I2C0_DMA_RX_DMA_BASE DMA0

#define RTE_I2C3_DMA_TX_CH 0
#define RTE_I2C3_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMux1LPI2C3Tx
#define RTE_I2C3_DMA_TX_DMAMUX_BASE DMAMUX1
#define RTE_I2C3_DMA_TX_DMA_BASE DMA1
#define RTE_I2C3_DMA_RX_CH 1
#define RTE_I2C3_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMux1LPI2C3Rx
#define RTE_I2C3_DMA_RX_DMAMUX_BASE DMAMUX1
#define RTE_I2C3_DMA_RX_DMA_BASE DMA1

/* M4 core configuration */
#else
/* Driver name mapping. */
#define RTE_I2C1 1
#define RTE_I2C1_DMA_EN 1

/* LPI2C configuration. */
#define RTE_I2C0_DMA_TX_CH 0
#define RTE_I2C0_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMux0LPI2C0Tx
#define RTE_I2C0_DMA_TX_DMAMUX_BASE DMAMUX0
#define RTE_I2C0_DMA_TX_DMA_BASE DMA0
#define RTE_I2C0_DMA_RX_CH 1
#define RTE_I2C0_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMux0LPI2C0Rx
#define RTE_I2C0_DMA_RX_DMAMUX_BASE DMAMUX0
#define RTE_I2C0_DMA_RX_DMA_BASE DMA0

#define RTE_I2C1_DMA_TX_CH 0
#define RTE_I2C1_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMux0LPI2C1Tx
#define RTE_I2C1_DMA_TX_DMAMUX_BASE DMAMUX0
#define RTE_I2C1_DMA_TX_DMA_BASE DMA0
#define RTE_I2C1_DMA_RX_CH 1
#define RTE_I2C1_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMux0LPI2C1Rx
#define RTE_I2C1_DMA_RX_DMAMUX_BASE DMAMUX0
#define RTE_I2C1_DMA_RX_DMA_BASE DMA0
#endif

#endif /* __RTE_DEVICE_H */
