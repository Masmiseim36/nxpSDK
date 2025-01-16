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
/* Master related */
#define DRIVER_MASTER_SPI                     Driver_SPI1
#define EXAMPLE_LPSPI_MASTER_IRQN             (LPSPI1_IRQn)
#define EXAMPLE_LPSPI_DEALY_COUNT             0xfffffU
#define EXAMPLE_LPSPI_MASTER_DMA_MUX_BASEADDR DMAMUX
#define EXAMPLE_LPSPI_MASTER_DMA_BASEADDR     DMA0

/* Select USB1 PLL PFD0 (480 MHz) as lpspi clock source */
#define EXAMPLE_LPSPI_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master lpspi clock source */
#define EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER (7U)

#define EXAMPLE_LPSPI_CLOCK_FREQ (CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk) / (EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER + 1U))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
