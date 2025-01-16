/*
 * Copyright 2021 NXP
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
#define SCTIMER_NEED_CHANGE_CLOCK_SOURCE 1
#define DEMO_CLOCK_MODE                  kSCTIMER_Input_ClockMode
#define DEMO_CLOCK_SEL                   kSCTIMER_Clock_On_Rise_Input_7
#define SCTIMER_CLK_FREQ                 CLOCK_GetSctClkFreq()
#define DEMO_FIRST_SCTIMER_OUT           kSCTIMER_Out_0
#define DEMO_SECOND_SCTIMER_OUT          kSCTIMER_Out_6
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
