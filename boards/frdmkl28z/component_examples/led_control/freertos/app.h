/*
 * Copyright 2018 NXP
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
#define LED_TYPE_MONOCHROME_COUNT 1
#define TIMER_SOURCE_CLOCK CLOCK_GetIpFreq(kCLOCK_Lpit0)
#define BUTTON_COUNT 1
#define BUTTON_NAME BOARD_SW3_NAME
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
