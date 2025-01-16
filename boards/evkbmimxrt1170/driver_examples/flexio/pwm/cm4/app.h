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
#define DEMO_TIME_DELAY_FOR_DUTY_CYCLE_UPDATE (2000000U)
#define DEMO_FLEXIO_BASEADDR                  FLEXIO2
#define DEMO_FLEXIO_OUTPUTPIN                 (10U) /* Select FLEXIO2_D10 as PWM output */
#define DEMO_FLEXIO_TIMER_CH                  (0U)  /* Flexio timer0 used */

#define DEMO_FLEXIO_CLOCK_FREQUENCY (CLOCK_GetRootClockFreq(kCLOCK_Root_Flexio2))
/* FLEXIO output PWM frequency */
#define DEMO_FLEXIO_FREQUENCY (48000U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
