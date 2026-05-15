/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _RTE_DEVICE_H
#define _RTE_DEVICE_H

extern void BOARD_InitSPIPins();
extern void BOARD_InitSPIPins_deinit();

/*Driver name mapping.*/
/* User needs to provide the implementation of LPSPIX_GetFreq/LPSPIX_InitPins/LPSPIX_DeinitPins for the enabled LPSPI
 * instance. */
#define RTE_SPI2        1
#define RTE_SPI2_DMA_EN 1

/* SPI configuration. */
#define RTE_SPI2_PCS_TO_SCK_DELAY       1000
#define RTE_SPI2_SCK_TO_PSC_DELAY       1000
#define RTE_SPI2_BETWEEN_TRANSFER_DELAY 1000
#define RTE_SPI2_MASTER_PCS_PIN_SEL     (kLPSPI_MasterPcs0)
#define RTE_SPI2_SLAVE_PCS_PIN_SEL      (kLPSPI_SlavePcs0)
#define RTE_SPI2_PIN_INIT               BOARD_InitSPIPins
#define RTE_SPI2_PIN_DEINIT             BOARD_InitSPIPins_deinit
#define RTE_SPI2_DMA_TX_CH              0
#define RTE_SPI2_DMA_TX_PERI_SEL        kDma3RequestMuxLPSPI2Tx
#define RTE_SPI2_DMA_TX_DMA_BASE        DMA3
#define RTE_SPI2_DMA_RX_CH              1
#define RTE_SPI2_DMA_RX_PERI_SEL        kDma3RequestMuxLPSPI2Rx
#define RTE_SPI2_DMA_RX_DMA_BASE        DMA3
#endif /* _RTE_DEVICE_H */
