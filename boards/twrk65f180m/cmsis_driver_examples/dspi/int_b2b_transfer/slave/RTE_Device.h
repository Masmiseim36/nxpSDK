/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/* Driver name mapping. */
#define RTE_SPI2 1
#define RTE_SPI2_DMA_EN 0

/* DSPI configuration. */
#define RTE_SPI2_PCS_TO_SCK_DELAY 1000
#define RTE_SPI2_SCK_TO_PSC_DELAY 1000
#define RTE_SPI2_BETWEEN_TRANSFER_DELAY 1000
#define RTE_SPI2_MASTER_PCS_PIN_SEL kDSPI_MasterPcs0
#define RTE_SPI2_DMA_TX_CH 0
#define RTE_SPI2_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMux0SPI2Tx
#define RTE_SPI2_DMA_TX_DMAMUX_BASE DMAMUX0
#define RTE_SPI2_DMA_TX_DMA_BASE DMA0
#define RTE_SPI2_DMA_RX_CH 1
#define RTE_SPI2_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMux0SPI2Rx
#define RTE_SPI2_DMA_RX_DMAMUX_BASE DMAMUX0
#define RTE_SPI2_DMA_RX_DMA_BASE DMA0
#define RTE_SPI2_DMA_LINK_DMA_BASE DMA0
#define RTE_SPI2_DMA_LINK_CH 2

#endif /* __RTE_DEVICE_H */
