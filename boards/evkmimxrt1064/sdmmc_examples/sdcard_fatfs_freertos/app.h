/*
 * Copyright 2019 NXP
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
#define DEMO_TASK_GET_SEM_BLOCK_TICKS 1U
#define DEMO_TASK_ACCESS_SDCARD_TIMES 2U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_PowerOffSDCARD(void);
void BOARD_PowerOnSDCARD(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
