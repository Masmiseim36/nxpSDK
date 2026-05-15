/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* Master related */
#define EXAMPLE_LPSPI_MASTER_BASEADDR       (LPSPI2)
#define DEMO_LPSPI_TRANSMIT_EDMA_CHANNEL    kDma3RequestMuxLPSPI2Tx
#define DEMO_LPSPI_RECEIVE_EDMA_CHANNEL     kDma3RequestMuxLPSPI2Rx
#define EXAMPLE_LPSPI_MASTER_DMA_BASE       (DMA3)
#define EXAMPLE_LPSPI_MASTER_DMA_RX_CHANNEL 0U
#define EXAMPLE_LPSPI_MASTER_DMA_TX_CHANNEL 1U

#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

#define LPSPI_MASTER_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Lpspi0304)

#define EXAMPLE_LPSPI_DEALY_COUNT 0xFFFFFU
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
