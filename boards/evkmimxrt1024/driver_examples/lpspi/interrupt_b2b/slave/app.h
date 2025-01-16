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

/* Select USB1 PLL PFD0 (720 MHz) as lpspi clock source */
#define EXAMPLE_LPSPI_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master lpspi clock source */
#define EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER (7U)

#define LPSPI_MASTER_CLK_FREQ (CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk) / (EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER + 1U))

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
