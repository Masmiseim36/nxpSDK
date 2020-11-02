/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __RTE_DEVICE_H
#define __RTE_DEVICE_H

/* Driver name mapping. */
#define RTE_SPI1        1
#define RTE_SPI1_DMA_EN 1

#define RTE_I2C4        1
#define RTE_I2C4_DMA_EN 0

/* SPI configuration. */
#define RTE_SPI_FIFO_ENABLE 1
#define SPI0_RX_FIFO_SIZE   8
#define SPI0_TX_FIFO_SIZE   8
#define SPI1_RX_FIFO_SIZE   8
#define SPI1_TX_FIFO_SIZE   8

#define RTE_SPI1_SSEL_NUM           kSPI_Ssel0
#define RTE_SPI1_DMA_TX_CH          11
#define RTE_SPI1_DMA_TX_PERI_SEL    (uint8_t) kDmaRequestMuxLPSPI1Tx
#define RTE_SPI1_DMA_TX_DMAMUX_BASE DMAMUX
#define RTE_SPI1_DMA_TX_DMA_BASE    DMA0
#define RTE_SPI1_DMA_RX_CH          10
#define RTE_SPI1_DMA_RX_PERI_SEL    (uint8_t) kDmaRequestMuxLPSPI1Rx
#define RTE_SPI1_DMA_RX_DMAMUX_BASE DMAMUX
#define RTE_SPI1_DMA_RX_DMA_BASE    DMA0

#define RTE_SPI1_PCS_TO_SCK_DELAY       0
#define RTE_SPI1_SCK_TO_PSC_DELAY       0
#define RTE_SPI1_BETWEEN_TRANSFER_DELAY 50 /* For 10MHz baudrate. delay(ns) = 10^9/baudrate/2. */
#define RTE_SPI1_MASTER_PCS_PIN_SEL     kLPSPI_MasterPcs0

#endif /* __RTE_DEVICE_H */
