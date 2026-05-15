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
/* Slave related */
#define EXAMPLE_LPSPI_SLAVE_BASEADDR       (LPSPI2)
#define DEMO_LPSPI_TRANSMIT_EDMA_CHANNEL   kDma3RequestMuxLPSPI2Tx
#define DEMO_LPSPI_RECEIVE_EDMA_CHANNEL    kDma3RequestMuxLPSPI2Rx
#define EXAMPLE_LPSPI_SLAVE_DMA_BASE       (DMA3)
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_CHANNEL 0U
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_CHANNEL 1U

#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
