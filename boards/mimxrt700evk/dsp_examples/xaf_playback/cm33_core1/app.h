/*
 * Copyright 2021-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_CORECM33_H_
#define _APP_CORECM33_H_

/*${header:start}*/
#include "fsl_sema42.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_CODEC_VOLUME  70U
#define APP_SEMA42         SEMA42_3
#define SEMA_PRINTF_NUM    0
#define SEMA_STARTUP_NUM   1
#define SEMA_CORE_ID_CM33  5
#define SEMA_LOCKED_BY_DSP kSEMA42_LockedByProc8
#define DEMO_I2C_CLK_FREQ  24000000U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
int BOARD_CODEC_Init(void);
void BOARD_PowerOffSDCARD(void);
void BOARD_PowerOnSDCARD(void);
void BOARD_USDHC_Switch_VoltageTo1V8(void);
void BOARD_USDHC_Switch_VoltageTo3V3(void);
/*${prototype:end}*/

#endif /* _APP_CORECM33_H_ */
