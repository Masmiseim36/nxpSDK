/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_MEM_LEN             (5U)
#define DEMO_DTCM_1              (0x2410F000U)
#define DEMO_DTCM_2              (0x2410F400U)
#define DEMO_SRAM                (0x20540000U)
#define DEMO_SEMA42_INSTANCE     SEMA42_4
#define DEMO_SEMA42_PRINTF_GATE  (0)
#define DEMO_SEMA42_CORE_ID_EZHV (9)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
