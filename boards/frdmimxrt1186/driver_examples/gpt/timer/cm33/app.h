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
#define GPT_IRQ_ID             GPT1_IRQn
#define EXAMPLE_GPT            GPT1
#define EXAMPLE_GPT_IRQHandler GPT1_IRQHandler

/* Get source clock for GPT driver */
#define EXAMPLE_GPT_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_Gpt1)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
