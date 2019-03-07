/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/*Driver name mapping.*/
#define RTE_SPI1 1
#define RTE_SPI1_DMA_EN 1

/* SPI configuration. */
#define RTE_SPI1_PCS_TO_SCK_DELAY 1000
#define RTE_SPI1_SCK_TO_PSC_DELAY 1000
#define RTE_SPI1_BETWEEN_TRANSFER_DELAY 1000
#define RTE_SPI1_MASTER_PCS_PIN_SEL (kLPSPI_MasterPcs0)
#define RTE_SPI1_SLAVE_PCS_PIN_SEL (kLPSPI_SlavePcs0)
#define RTE_SPI1_DMA_TX_CH 3
#define RTE_SPI1_DMA_TX_PERI_SEL (uint8_t) kDmaRequestMux0LPSPI1Tx
#define RTE_SPI1_DMA_TX_DMAMUX_BASE DMA_CH_MUX0
#define RTE_SPI1_DMA_TX_DMA_BASE DMA0
#define RTE_SPI1_DMA_RX_CH 4
#define RTE_SPI1_DMA_RX_PERI_SEL (uint8_t) kDmaRequestMux0LPSPI1Rx
#define RTE_SPI1_DMA_RX_DMAMUX_BASE DMA_CH_MUX0
#define RTE_SPI1_DMA_RX_DMA_BASE DMA0

#endif /* __RTE_DEVICE_H */
