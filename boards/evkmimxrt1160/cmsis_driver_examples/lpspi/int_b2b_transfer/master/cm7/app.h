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
/* Master related */
#define DRIVER_MASTER_SPI         Driver_SPI1
#define EXAMPLE_LPSPI_MASTER_IRQN (LPSPI1_IRQn)
#define EXAMPLE_LPSPI_DEALY_COUNT 0xfffffU

#define EXAMPLE_LPSPI_CLOCK_FREQ (CLOCK_GetRootClockFreq(kCLOCK_Root_Lpspi1))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
