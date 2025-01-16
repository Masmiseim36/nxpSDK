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
#define MASTER_FLEXIO_SPI_BASEADDR FLEXIO
#define FLEXIO_SPI_SOUT_PIN        1U
#define FLEXIO_SPI_SIN_PIN         14U
#define FLEXIO_SPI_CLK_PIN         15U
#define FLEXIO_SPI_PCS_PIN         0U
#define MASTER_FLEXIO_SPI_IRQ      FLEXIO_IRQn

#define SLAVE_LPSPI_BASEADDR         LPSPI16
#define SLAVE_LPSPI_IRQN             (LPSPI16_IRQn)
#define SLAVE_LPSPI_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)
#define SLAVE_LPSPI_PCS_FOR_INIT     (kLPSPI_Pcs0)

#define FLEXIO_CLOCK_SRC                  (kFRO1_DIV1_to_FLEXIO)
#define MASTER_FLEXIO_SPI_CLOCK_FREQUENCY CLOCK_GetFlexioClkFreq()

#define TRANSFER_SIZE     (256U)  /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 50000U /*! Transfer baudrate - 50k */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
