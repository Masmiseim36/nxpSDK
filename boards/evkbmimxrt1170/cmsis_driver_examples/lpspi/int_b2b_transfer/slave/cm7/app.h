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
#define DRIVER_SLAVE_SPI         Driver_SPI1
#define EXAMPLE_LPSPI_SLAVE_IRQN (LPSPI1_IRQn)

#define EXAMPLE_LPSPI_CLOCK_FREQ (CLOCK_GetRootClockFreq(kCLOCK_Root_Lpspi1))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
