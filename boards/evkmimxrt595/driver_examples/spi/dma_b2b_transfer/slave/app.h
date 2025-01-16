/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#ifndef USE_HS_SPI
#define USE_HS_SPI 0
#endif

#if USE_HS_SPI
#define EXAMPLE_SPI_SLAVE            SPI14
#define EXAMPLE_SPI_SLAVE_IRQ        FLEXCOMM14_IRQn
#define EXAMPLE_SPI_SLAVE_RX_CHANNEL 26
#define EXAMPLE_SPI_SLAVE_TX_CHANNEL 27
#else
#define EXAMPLE_SPI_SLAVE            SPI5
#define EXAMPLE_SPI_SLAVE_IRQ        FLEXCOMM5_IRQn
#define EXAMPLE_SPI_SLAVE_RX_CHANNEL 10
#define EXAMPLE_SPI_SLAVE_TX_CHANNEL 11
#endif

#define EXAMPLE_SPI_SSEL       0
#define EXAMPLE_DMA            DMA0
#define EXAMPLE_SLAVE_SPI_SPOL kSPI_SpolActiveAllLow
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
