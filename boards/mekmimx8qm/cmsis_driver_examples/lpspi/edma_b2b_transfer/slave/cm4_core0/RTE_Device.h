/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void LPSPI2_InitPins();
extern void LPSPI2_DeinitPins();

/* SPI instance name mapping */
#define LPSPI0 DMA__LPSPI0
#define LPSPI1 DMA__LPSPI1
#define LPSPI2 DMA__LPSPI2
#define LPSPI3 DMA__LPSPI3

/* Driver name mapping. */
/* User needs to provide the implementation of LPSPIX_GetFreq/LPSPIX_InitPins/LPSPIX_DeinitPins for the enabled LPSPI
 * instance. */
#define RTE_SPI2        1
#define RTE_SPI2_DMA_EN 1

/* SPI configuration. */

#define RTE_SPI2_PCS_TO_SCK_DELAY       1000
#define RTE_SPI2_SCK_TO_PSC_DELAY       1000
#define RTE_SPI2_BETWEEN_TRANSFER_DELAY 1000
#define RTE_SPI2_SLAVE_PCS_PIN_SEL      kLPSPI_SlavePcs0
#define RTE_SPI2_PIN_INIT               LPSPI2_InitPins
#define RTE_SPI2_PIN_DEINIT             LPSPI2_DeinitPins
#define RTE_SPI2_DMA_TX_CH              5
#define RTE_SPI2_DMA_TX_PERI_SEL        5
#define RTE_SPI2_DMA_TX_DMA_BASE        DMA__EDMA0
#define RTE_SPI2_DMA_RX_CH              4
#define RTE_SPI2_DMA_RX_PERI_SEL        4
#define RTE_SPI2_DMA_RX_DMA_BASE        DMA__EDMA0

#endif /* _RTE_DEVICE_H */
