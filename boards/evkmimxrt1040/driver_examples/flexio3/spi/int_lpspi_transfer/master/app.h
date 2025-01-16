/*
 * Copyright 2021 NXP
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
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */

#define MASTER_FLEXIO_SPI_BASEADDR (FLEXIO3)
#define FLEXIO_SPI_SOUT_PIN        4U
#define FLEXIO_SPI_SIN_PIN         5U
#define FLEXIO_SPI_CLK_PIN         6U
#define FLEXIO_SPI_PCS_PIN         7U

#define MASTER_FLEXIO_SPI_IRQ FLEXIO3_IRQn

/* Select USB1 PLL (480 MHz) as flexio clock source */
#define MASTER_FLEXIO_SPI_CLOCK_SELECT (3U)
/* Clock pre divider for flexio clock source */
#define MASTER_FLEXIO_SPI_CLOCK_PRE_DIVIDER (4U)
/* Clock divider for flexio clock source */
#define MASTER_FLEXIO_SPI_CLOCK_DIVIDER (7U)
#define MASTER_FLEXIO_SPI_CLOCK_FREQUENCY                                            \
    (CLOCK_GetFreq(kCLOCK_Usb1PllClk) / (MASTER_FLEXIO_SPI_CLOCK_PRE_DIVIDER + 1U) / \
     (MASTER_FLEXIO_SPI_CLOCK_DIVIDER + 1U))

/*Slave related*/
#define SLAVE_LPSPI_BASEADDR (LPSPI1)
#define SLAVE_LPSPI_IRQN     (LPSPI1_IRQn)

/* Select USB1 PLL PFD0 (720 MHz) as lpspi clock source */
#define SLAVE_LPSPI_CLOCK_SELECT (1U)
/* Clock divider for lpspi clock source */
#define SLAVE_LPSPI_CLOCK_DIVIDER (7U)

#define SLAVE_LPSPI_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define SLAVE_LPSPI_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
