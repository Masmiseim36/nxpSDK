/*
 * Copyright 2021-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_CORECM33_H_
#define _APP_CORECM33_H_

/*${header:start}*/
#include "dsp_config.h"
#include "dsp_support.h"
#include "fsl_sema42.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_CODEC_VOLUME  92U
#define APP_SEMA42         SEMA42
#define SEMA_PRINTF_NUM    0
#define SEMA_STARTUP_NUM   1
#define SEMA_CORE_ID_CM33  1
#define SEMA_LOCKED_BY_DSP kSEMA42_LockedByProc3
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
int BOARD_CODEC_Init(void);
/*${prototype:end}*/

#endif /* _APP_CORECM33_H_ */
