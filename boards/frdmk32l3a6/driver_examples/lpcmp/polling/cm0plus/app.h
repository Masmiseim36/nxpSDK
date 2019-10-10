/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_
#include "board.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_LPCMP_BASE LPCMP1
#define DEMO_LPCMP_USER_CHANNEL 2U
#define DEMO_LPCMP_DAC_CHANNEL 7U

#define LED_INIT() LED1_INIT(LOGIC_LED_OFF)
#define LED_ON() LED1_ON()
#define LED_OFF() LED1_OFF()

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);

/*${prototype:end}*/

#endif /* _APP_H_ */
