/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/*Driver name mapping*/
#define RTE_SPI8        1
#define RTE_SPI8_DMA_EN 1

/* SPI configuration. */
#define RTE_SPI8_SSEL_NUM kSPI_Ssel1

/* SPI DMA configuration */
#if RTE_SPI8_DMA_EN
#define RTE_SPI8_DMA_TX_CH       3
#define RTE_SPI8_DMA_TX_DMA_BASE DMA0
#define RTE_SPI8_DMA_RX_CH       2
#define RTE_SPI8_DMA_RX_DMA_BASE DMA0
#endif

#define RTE_I2C4        1
#define RTE_I2C4_DMA_EN 0

#endif /* __RTE_DEVICE_H */
