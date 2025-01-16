/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#ifndef USE_HS_SPI
#define USE_HS_SPI 0
#endif

#if USE_HS_SPI
#define EXAMPLE_SPI_MASTER          SPI14
#define EXAMPLE_SPI_MASTER_IRQ      FLEXCOMM14_IRQn
#define EXAMPLE_SPI_MASTER_CLK_SRC  kCLOCK_Flexcomm14Clk
#define EXAMPLE_SPI_MASTER_CLK_FREQ CLOCK_GetFlexcommClkFreq(14)
#define SPI_MASTER_IRQHandler       FLEXCOMM14_IRQHandler
#else
#define EXAMPLE_SPI_MASTER          SPI5
#define EXAMPLE_SPI_MASTER_IRQ      FLEXCOMM5_IRQn
#define EXAMPLE_SPI_MASTER_CLK_SRC  kCLOCK_Flexcomm5Clk
#define EXAMPLE_SPI_MASTER_CLK_FREQ CLOCK_GetFlexcommClkFreq(5)
#define SPI_MASTER_IRQHandler       FLEXCOMM5_IRQHandler
#endif

#define EXAMPLE_SPI_SSEL 0
#define EXAMPLE_SPI_SPOL kSPI_SpolActiveAllLow
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
