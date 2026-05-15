/*
 * Copyright 2018 NXP
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

#define MASTER_FLEXIO_SPI_BASEADDR (FLEXIO1)
#define FLEXIO_SPI_PCS_PIN         15U
#define FLEXIO_SPI_SOUT_PIN        14U
#define FLEXIO_SPI_SIN_PIN         13U
#define FLEXIO_SPI_CLK_PIN         12U

#define MASTER_FLEXIO_SPI_IRQ FLEXIO1_IRQn

#define MASTER_FLEXIO_SPI_CLOCK_FREQUENCY CLOCK_GetRootClockFreq(kCLOCK_Root_Flexio1)

/*Slave related*/
#define SLAVE_LPSPI_BASEADDR (LPSPI2)
#define SLAVE_LPSPI_IRQN     (LPSPI2_IRQn)

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
