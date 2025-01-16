/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_CORECM33_H_
#define _APP_CORECM33_H_

/*${header:start}*/
#include "dsp_support.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_MU               MUA
#define APP_SEMA42           SEMA42
#define APP_BOARD_HAS_LED    1
#define USE_STATIC_DOMAIN_ID 0
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void LED_INIT();
/*${prototype:end}*/

#endif /* _APP_CORECM33_H_ */
