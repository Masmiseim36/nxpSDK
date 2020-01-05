/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/* Driver name mapping. */
#define RTE_SPI0 1
#define RTE_SPI0_DMA_EN 1

/* DSPI configuration. */
#define RTE_SPI0_PCS_TO_SCK_DELAY 1000
#define RTE_SPI0_SCK_TO_PSC_DELAY 1000
#define RTE_SPI0_BETWEEN_TRANSFER_DELAY 1000
#define RTE_SPI0_SLAVE_PCS_PIN_SEL (kLPSPI_SlavePcs3)
#define RTE_SPI0_DMA_TX_CH 0
#define RTE_SPI0_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMux0LPSPI0Tx
#define RTE_SPI0_DMA_TX_DMAMUX_BASE DMAMUX
#define RTE_SPI0_DMA_TX_DMA_BASE DMA0
#define RTE_SPI0_DMA_RX_CH 1
#define RTE_SPI0_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMux0LPSPI0Rx
#define RTE_SPI0_DMA_RX_DMAMUX_BASE DMAMUX
#define RTE_SPI0_DMA_RX_DMA_BASE DMA0

#endif /* __RTE_DEVICE_H */
