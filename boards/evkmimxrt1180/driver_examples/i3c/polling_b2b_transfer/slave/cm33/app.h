/*
 * Copyright 2022, 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_SLAVE              I3C2
#define I3C_SLAVE_CLOCK_FREQUENCY  CLOCK_GetRootClockFreq(kCLOCK_Root_I3c2)
#define I3C_ASYNC_WAKE_UP_INTR_CLEAR                    \
    {                                                   \
        BLK_CTRL_WAKEUPMIX->I3C2_ASYNC_WAKEUP_CTRL = 1; \
    }
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
