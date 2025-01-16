/*
 * Copyright 2020 NXP
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
#define DEMO_TEMP_SENSOR              TMPSNS
#define DEMO_TEMP_LOW_HIGH_IRQn       TMPSNS_LOW_HIGH_IRQn
#define DEMO_TEMP_PANIC_IRQn          TMPSNS_PANIC_IRQn
#define DEMO_TEMP_LOW_HIGH_IRQHandler TMPSNS_LOW_HIGH_IRQHandler
#define DEMO_TEMP_PANIC_IRQHandler    TMPSNS_PANIC_IRQHandler
#define DEMO_HIGH_ALARM_TEMP          27U

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
