/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_I2C_MASTER         Driver_I2C8
#define EXAMPLE_LPI2C_DMA_BASEADDR (DMA0)
#define LPI2C_CLOCK_FREQUENCY      CLOCK_GetLPFlexCommClkFreq(8u)
#define EXAMPLE_LPI2C_DMA_CLOCK    kCLOCK_Dma0
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
