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
/* Slave related */
#define EXAMPLE_LPSPI_SLAVE_BASEADDR   (LPSPI1)
#define EXAMPLE_LPSPI_SLAVE_IRQN       (LPSPI1_IRQn)
#define EXAMPLE_LPSPI_SLAVE_IRQHandler LPSPI1_IRQHandler

#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)

#define LPSPI_MASTER_CLK_FREQ (CLOCK_GetRootClockFreq(kCLOCK_Root_Lpspi1))

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
AT_QUICKACCESS_SECTION_CODE(void EXAMPLE_LPSPI_SLAVE_IRQHandler(void));
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
