/*
 * Copyright 2023-2024 NXP
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
#define SLAVE_FLEXIO_SPI_BASEADDR FLEXIO
#define FLEXIO_SPI_SOUT_PIN       1U
#define FLEXIO_SPI_SIN_PIN        14U
#define FLEXIO_SPI_CLK_PIN        15U
#define FLEXIO_SPI_PCS_PIN        0U
#define SLAVE_FLEXIO_SPI_IRQ      FLEXIO_IRQn

#define MASTER_LPSPI_BASEADDR         LPSPI16
#define MASTER_LPSPI_IRQN             (LPSPI16_IRQn)
#define MASTER_LPSPI_CLOCK_FREQUENCY  CLOCK_GetLPSpiClkFreq(16U)
#define MASTER_LPSPI_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)
#define MASTER_LPSPI_PCS_FOR_INIT     (kLPSPI_Pcs0)

#define EXAMPLE_FLEXIO_SPI_DMA_LPSPI_BASEADDR DMA0
#define FLEXIO_SPI_TX_DMA_LPSPI_CHANNEL       (0U)
#define FLEXIO_SPI_RX_DMA_LPSPI_CHANNEL       (1U)
#define FLEXIO_TX_SHIFTER_INDEX               0U
#define FLEXIO_RX_SHIFTER_INDEX               2U
#define EXAMPLE_TX_DMA_SOURCE                 kDmaRequestMuxFlexIO0ShiftRegister0Request
#define EXAMPLE_RX_DMA_SOURCE                 kDmaRequestMuxFlexIO0ShiftRegister2Request

#define TRANSFER_SIZE     (256U)  /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
