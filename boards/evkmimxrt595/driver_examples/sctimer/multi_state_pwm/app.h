/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/

#define SCTIMER_CLK_FREQ        CLOCK_GetFreq(kCLOCK_BusClk)
#define DEMO_FIRST_SCTIMER_OUT  kSCTIMER_Out_0
#define DEMO_SECOND_SCTIMER_OUT kSCTIMER_Out_1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
