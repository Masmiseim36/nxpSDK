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
#define DEMO_WDOG_BASE         RTWDOG1
#define DEMO_SRC_BASE          SRC_GENERAL_REG
#define DEMO_GLOBAL_RESET_FLAG kSRC_Wdog1ResetFlag
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void APP_GlobalSystemResetInit(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
