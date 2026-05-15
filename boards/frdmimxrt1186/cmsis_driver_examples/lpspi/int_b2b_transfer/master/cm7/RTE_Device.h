/*
 * Copyright 2018-2020 NXP
 * All rights reserved.
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
#define RTE_SPI2_DMA_EN 0

/* SPI configuration. */
#define RTE_SPI2_PCS_TO_SCK_DELAY       1000
#define RTE_SPI2_SCK_TO_PSC_DELAY       1000
#define RTE_SPI2_BETWEEN_TRANSFER_DELAY 1000
#define RTE_SPI2_MASTER_PCS_PIN_SEL     (kLPSPI_MasterPcs0)
#define RTE_SPI2_SLAVE_PCS_PIN_SEL      (kLPSPI_SlavePcs0)
#define RTE_SPI2_PIN_INIT               BOARD_InitSPIPins
#define RTE_SPI2_PIN_DEINIT             BOARD_InitSPIPins_deinit

#endif /* _RTE_DEVICE_H */
