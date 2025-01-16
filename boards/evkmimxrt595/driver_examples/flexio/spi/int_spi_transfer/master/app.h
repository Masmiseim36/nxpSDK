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
#define BOARD_FLEXIO_BASE   FLEXIO0
#define FLEXIO_SPI_MOSI_PIN 13U
#define FLEXIO_SPI_MISO_PIN 14U
#define FLEXIO_SPI_SCK_PIN  15U
#define FLEXIO_SPI_PCS0_PIN 12U

#define BOARD_SPI_SLAVE_BASE SPI5

#define MASTER_SPI_IRQ FLEXIO_IRQn
#define SLAVE_SPI_IRQ  FLEXCOMM5_IRQn

/* Select flexio clock source */
#define FLEXIO_CLOCK_SRC (kFRO_DIV2_to_FLEXIO)
/* Clock divider for flexio clock source */
#define FLEXIO_CLOCK_DIVIDER   (8U)
#define FLEXIO_CLOCK_FREQUENCY CLOCK_GetFlexioClkFreq()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
