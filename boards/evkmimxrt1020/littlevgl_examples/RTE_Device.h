/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void LPSPI1_InitPins();
extern void LPSPI1_DeinitPins();
extern void LPI2C4_InitPins();
extern void LPI2C4_DeinitPins();
extern void LPSPI1_InitPins();
extern void LPSPI1_DeinitPins();

/* Driver name mapping. */
/* User needs to provide the implementation of LPSPIX_GetFreq/LPSPIX_InitPins/LPSPIX_DeinitPins for the enabled LPSPI
 * instance. */
#define RTE_SPI1            1
#define RTE_SPI1_PIN_INIT   LPSPI1_InitPins
#define RTE_SPI1_PIN_DEINIT LPSPI1_DeinitPins
#define RTE_SPI1_DMA_EN     1

/* User needs to provide the implementation of LPI2CX_GetFreq/LPI2CX_InitPins/LPI2CX_DeinitPins for the enabled LPI2C
 * instance. */
#define RTE_I2C4            1
#define RTE_I2C4_PIN_INIT   LPI2C4_InitPins
#define RTE_I2C4_PIN_DEINIT LPI2C4_DeinitPins
#define RTE_I2C4_DMA_EN     0

/* SPI configuration. */
#define SPI0_RX_FIFO_SIZE 8
#define SPI0_TX_FIFO_SIZE 8
#define SPI1_RX_FIFO_SIZE 8
#define SPI1_TX_FIFO_SIZE 8

#define RTE_SPI1_SSEL_NUM           kSPI_Ssel0
#define RTE_SPI1_PIN_INIT           LPSPI1_InitPins
#define RTE_SPI1_PIN_DEINIT         LPSPI1_DeinitPins
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

#endif /* _RTE_DEVICE_H */
