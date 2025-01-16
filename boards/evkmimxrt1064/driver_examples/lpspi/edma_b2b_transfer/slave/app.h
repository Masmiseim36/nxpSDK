/*
 * Copyright 2019 NXP
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
#define EXAMPLE_LPSPI_SLAVE_BASEADDR              (LPSPI1)
#define EXAMPLE_LPSPI_SLAVE_DMA_MUX_BASE          (DMAMUX)
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_REQUEST_SOURCE kDmaRequestMuxLPSPI1Rx
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_REQUEST_SOURCE kDmaRequestMuxLPSPI1Tx
#define EXAMPLE_LPSPI_SLAVE_DMA_BASE              (DMA0)
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_CHANNEL        0U
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_CHANNEL        1U

#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)

/* Select USB1 PLL PFD0 (480 MHz) as lpspi clock source */
#define EXAMPLE_LPSPI_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master lpspi clock source */
#define EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER (7U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
