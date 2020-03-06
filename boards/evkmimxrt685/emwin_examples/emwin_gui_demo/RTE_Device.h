/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/*Driver name mapping*/
#define RTE_SPI5 1
#define RTE_SPI5_DMA_EN 1

/* SPI configuration. */
#define RTE_SPI5_SSEL_NUM kSPI_Ssel0

/* SPI DMA configuration */
#if RTE_SPI5_DMA_EN
#define RTE_SPI5_DMA_TX_CH 11
#define RTE_SPI5_DMA_TX_DMA_BASE DMA0
#define RTE_SPI5_DMA_RX_CH 10
#define RTE_SPI5_DMA_RX_DMA_BASE DMA0
#endif

#define RTE_I2C2 1
#define RTE_I2C2_DMA_EN 0

#endif /* __RTE_DEVICE_H */
