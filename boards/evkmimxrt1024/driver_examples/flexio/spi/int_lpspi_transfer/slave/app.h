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
/*Master related*/
#define TRANSFER_SIZE     256u    /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 500000U /*! Transfer baudrate - 500k */

#define MASTER_LPSPI_BASEADDR (LPSPI1)
#define MASTER_LPSPI_IRQN     (LPSPI1_IRQn)

#define MASTER_LPSPI_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define MASTER_LPSPI_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

/* Select USB1 PLL PFD0 (720 MHz) as lpspi clock source */
#define MASTER_LPSPI_CLOCK_SELECT (1U)
/* Clock divider for lpspi clock source */
#define MASTER_LPSPI_CLOCK_DIVIDER   (7U)
#define MASTER_LPSPI_CLOCK_FREQUENCY (CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk) / (MASTER_LPSPI_CLOCK_DIVIDER + 1U))

/*Slave related*/
#define SLAVE_FLEXIO_SPI_BASEADDR (FLEXIO1)
#define FLEXIO_SPI_SOUT_PIN       1U
#define FLEXIO_SPI_SIN_PIN        0U
#define FLEXIO_SPI_CLK_PIN        3U
#define FLEXIO_SPI_PCS_PIN        2U

#define SLAVE_FLEXIO_SPI_IRQ FLEXIO1_IRQn
/* Select USB1 PLL (480 MHz) as flexio clock source */
#define SLAVE_FLEXIO_SPI_CLOCK_SELECT (3U)
/* Clock divider for flexio clock source */
#define SLAVE_FLEXIO_SPI_CLOCK_PRE_DIVIDER (4U)
/* Clock divider for flexio clock source */
#define SLAVE_FLEXIO_SPI_CLOCK_DIVIDER (7U)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
