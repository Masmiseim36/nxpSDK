/*
 * Copyright 2024 NXP
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
#define BOARD_FLEXIO_BASE          FLEXIO
#define FLEXIO_SPI_MOSI_PIN        1U
#define FLEXIO_SPI_MISO_PIN        14U
#define FLEXIO_SPI_SCK_PIN         15U
#define FLEXIO_SPI_CSn_PIN         0U
#define FLEXIO_CLOCK_FREQUENCY     CLOCK_GetFlexioClkFreq()

#define EXAMPLE_FLEXIO_SPI_DMA_BASEADDR  DMA0
#define FLEXIO_SPI_TX_DMA_CHANNEL        (0U)
#define FLEXIO_SPI_RX_DMA_CHANNEL        (1U)
#define FLEXIO_TX_SHIFTER_INDEX          0U
#define FLEXIO_RX_SHIFTER_INDEX          2U
#define EXAMPLE_TX_DMA_SOURCE            kDmaRequestMuxFlexIO0ShiftRegister0Request
#define EXAMPLE_RX_DMA_SOURCE            kDmaRequestMuxFlexIO0ShiftRegister2Request
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
