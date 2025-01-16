/*
 * Copyright 2018-2019 NXP
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
/* RESET_CHECK_CNT_VALUE and RESET_CHECK_FLAG is RAM variables used for wdog32 self test.
 * Make sure these variables' locations are proper and will not be affected by watchdog reset,
 * that is, these variables will not be intialized in startup code.
 */
#define RESET_CHECK_CNT_VALUE  (*((uint32_t *)0x20001000))
#define RESET_CHECK_FLAG       (*((uint32_t *)0x20002000))
#define RESET_CHECK_INIT_VALUE 0x0D0DU
#define EXAMPLE_WDOG_BASE      RTWDOG
#define DELAY_TIME             100000U
#define WDOG_IRQHandler        RTWDOG_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
