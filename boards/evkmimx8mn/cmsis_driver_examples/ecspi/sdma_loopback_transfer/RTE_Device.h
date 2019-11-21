/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/* Driver name mapping. */
#define RTE_SPI2 2
#define RTE_SPI2_DMA_EN 1

/* ECSPI configuration. */
#define RTE_SPI1_TRANSFER_CHANNEL kECSPI_Channel0
#define RTE_SPI2_TRANSFER_CHANNEL kECSPI_Channel0
#define RTE_SPI3_TRANSFER_CHANNEL kECSPI_Channel0

#define RTE_SPI2_DMA_TX_DMA_BASE SDMAARM1
#define RTE_SPI2_DMA_TX_CH (2U)
#define RTE_SPI2_DMA_TX_CH_REQUEST (3U)
#define RTE_SPI2_DMA_TX_CH_PRIORITY (3U)
#define RTE_SPI2_DMA_RX_DMA_BASE SDMAARM1
#define RTE_SPI2_DMA_RX_CH (1U)
#define RTE_SPI2_DMA_RX_CH_REQUEST (2U)
#define RTE_SPI2_DMA_RX_CH_PRIORITY (2U)

#endif /* __RTE_DEVICE_H */
