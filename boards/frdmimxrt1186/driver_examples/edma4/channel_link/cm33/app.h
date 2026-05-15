/*
 * Copyright 2022 NXP
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
#define EXAMPLE_DMA_BASEADDR DMA4
#define DEMO_DMA_CHANNEL_0   0
#define DEMO_DMA_CHANNEL_1   1
#define DEMO_DMA_CHANNEL_2   2
#define APP_DMA_IRQ          DMA4_CH2_CH3_CH34_CH35_IRQn
#define APP_DMA_IRQ_HANDLER  DMA4_CH2_CH3_CH34_CH35_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
