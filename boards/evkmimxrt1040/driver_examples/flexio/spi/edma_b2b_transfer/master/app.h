/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Headers
 ******************************************************************************/
/*${header:start}*/
#include "fsl_dmamux.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/*Master related*/
#define BOARD_FLEXIO_BASE   (FLEXIO2)
#define FLEXIO_SPI_MOSI_PIN 6U
#define FLEXIO_SPI_MISO_PIN 7U
#define FLEXIO_SPI_SCK_PIN  5U
#define FLEXIO_SPI_CSn_PIN  8U

/* Select USB1 PLL (480 MHz) as flexio clock source */
#define MASTER_FLEXIO_SPI_CLOCK_SELECT (3U)
/* Clock pre divider for flexio clock source */
#define MASTER_FLEXIO_SPI_CLOCK_PRE_DIVIDER (4U)
/* Clock divider for flexio clock source */
#define MASTER_FLEXIO_SPI_CLOCK_DIVIDER (1U)
#define FLEXIO_CLOCK_FREQUENCY                                                       \
    (CLOCK_GetFreq(kCLOCK_Usb1PllClk) / (MASTER_FLEXIO_SPI_CLOCK_PRE_DIVIDER + 1U) / \
     (MASTER_FLEXIO_SPI_CLOCK_DIVIDER + 1U))

#define FLEXIO_DMA_REQUEST_SOURCE_BASE     (kDmaRequestMuxFlexIO2Request0Request1)
#define EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR DMAMUX
#define EXAMPLE_FLEXIO_SPI_DMA_BASEADDR    DMA0
#define FLEXIO_SPI_TX_DMA_CHANNEL          (0U)
#define FLEXIO_SPI_RX_DMA_CHANNEL          (1U)
#define FLEXIO_TX_SHIFTER_INDEX            0U
#define FLEXIO_RX_SHIFTER_INDEX            2U
#define EXAMPLE_TX_DMA_SOURCE              (kDmaRequestMuxFlexIO2Request0Request1)
#define EXAMPLE_RX_DMA_SOURCE              (kDmaRequestMuxFlexIO2Request2Request3)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
