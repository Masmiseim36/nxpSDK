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
#define RTE_SPI0 1
#define RTE_SPI0_DMA_EN 1

#define RTE_I2C0 1
#define RTE_I2C0_DMA_EN 0
#define RTE_I2C1 1
#define RTE_I2C1_DMA_EN 0

/* DSPI configuration. */
#define RTE_SPI0_PCS_TO_SCK_DELAY 0
#define RTE_SPI0_SCK_TO_PSC_DELAY 0
#define RTE_SPI0_BETWEEN_TRANSFER_DELAY 0
#define RTE_SPI0_MASTER_PCS_PIN_SEL kDSPI_MasterPcs0
#define RTE_SPI0_DMA_TX_CH 0
#define RTE_SPI0_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMux0SPI0Tx
#define RTE_SPI0_DMA_TX_DMAMUX_BASE DMAMUX0
#define RTE_SPI0_DMA_TX_DMA_BASE DMA0
#define RTE_SPI0_DMA_RX_CH 1
#define RTE_SPI0_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMux0SPI0Rx
#define RTE_SPI0_DMA_RX_DMAMUX_BASE DMAMUX0
#define RTE_SPI0_DMA_RX_DMA_BASE DMA0
#define RTE_SPI0_DMA_LINK_DMA_BASE DMA0
#define RTE_SPI0_DMA_LINK_CH 2

#endif /* __RTE_DEVICE_H */
