/*
 * Copyright 2022 NXP
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
/*Master related*/
#define TRANSFER_SIZE     256U    /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 400000U /*! Transfer baudrate - 400k */

#define MASTER_LPSPI_BASEADDR   (LPSPI1)
#define MASTER_LPSPI_IRQ_HANDLE (LPSPI1_DriverIRQHandler)
#define MASTER_LPSPI_IRQN       (LPSPI1_IRQn)

#define MASTER_LPSPI_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define MASTER_LPSPI_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

/* Select USB1 PLL PFD0 (720 MHz) as lpspi clock source */
#define MASTER_LPSPI_CLOCK_SELECT (1U)
/* Clock divider for lpspi clock source */
#define MASTER_LPSPI_CLOCK_DIVIDER   (7U)
#define MASTER_LPSPI_CLOCK_FREQUENCY (CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk) / (MASTER_LPSPI_CLOCK_DIVIDER + 1U))

/*Slave related*/
#define SLAVE_FLEXIO_SPI_BASEADDR   (FLEXIO2)
#define FLEXIO_SPI_SOUT_PIN         6U
#define FLEXIO_SPI_SIN_PIN          7U
#define FLEXIO_SPI_CLK_PIN          5U
#define FLEXIO_SPI_PCS_PIN          8U
#define FLEXIO_SPI_CS_MONITOR_TIMER 2U

/* Select USB1 PLL (480 MHz) as flexio clock source */
#define SLAVE_FLEXIO_SPI_CLOCK_SELECT (3U)
/* Clock divider for flexio clock source */
#define SLAVE_FLEXIO_SPI_CLOCK_PRE_DIVIDER (4U)
/* Clock divider for flexio clock source */
#define SLAVE_FLEXIO_SPI_CLOCK_DIVIDER (7U)

#define FLEXIO_DMA_REQUEST_SOURCE_BASE        (kDmaRequestMuxFlexIO2Request0Request1)
#define EXAMPLE_FLEXIO_SPI_DMAMUX_BASEADDR    DMAMUX
#define EXAMPLE_FLEXIO_SPI_DMA_LPSPI_BASEADDR DMA0
#define FLEXIO_SPI_TX_DMA_LPSPI_CHANNEL       (0U)
#define FLEXIO_SPI_RX_DMA_LPSPI_CHANNEL       (1U)
#define FLEXIO_TX_SHIFTER_INDEX               0U
#define FLEXIO_RX_SHIFTER_INDEX               2U
#define EXAMPLE_TX_DMA_SOURCE                 (kDmaRequestMuxFlexIO2Request0Request1)
#define EXAMPLE_RX_DMA_SOURCE                 (kDmaRequestMuxFlexIO2Request2Request3)
#define EXAMPLE_FLEXIO_IRQHandler             FLEXIO2_IRQHandler
#define EXAMPLE_FLEXIO_IRQ                    FLEXIO2_IRQn

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
