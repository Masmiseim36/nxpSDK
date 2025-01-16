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
#define DEMO_TEMP_MONITOR             TEMPMON
#define DEMO_TEMP_LOW_HIGH_IRQn       TEMP_LOW_HIGH_IRQn
#define DEMO_TEMP_PANIC_IRQn          TEMP_PANIC_IRQn
#define DEMO_TEMP_LOW_HIGH_IRQHandler TEMP_LOW_HIGH_IRQHandler
#define DEMO_TEMP_PANIC_IRQHandler    TEMP_PANIC_IRQHandler

#define DEMO_HIGH_ALARM_TEMP 42U
#define DEMO_LOW_ALARM_TEMP  40U

#define DEMO_CLOCK_SOURCE kCLOCK_AhbClk
#define DEMO_CLOCK_DIV    kCLOCK_AhbDiv
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
