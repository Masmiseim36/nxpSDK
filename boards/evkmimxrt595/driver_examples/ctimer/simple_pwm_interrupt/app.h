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
#define CTIMER          CTIMER0         /* Timer 0 */
#define CTIMER_MAT_OUT  kCTIMER_Match_0 /* Match output 0 */
#define CTIMER_CLK_FREQ CLOCK_GetCtimerClkFreq(0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
