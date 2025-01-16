/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_CORECM33_H_
#define _APP_CORECM33_H_

/*${header:start}*/
#include "dsp_config.h"
#include "dsp_support.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_CODEC_VOLUME 100
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
int BOARD_CODEC_Init(void);
/*${prototype:end}*/

#endif /* _APP_CORECM33_H_ */
