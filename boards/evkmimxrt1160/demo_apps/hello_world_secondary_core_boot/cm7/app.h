/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define APP_MU            MUA
#define BOOT_CORE1_BY_MU 0
/* Set core1 boot address*/
#define CORE1_BOOT_ADDRESS 0x08802000
/*
 * For the dual core project, generally primary core starts first, initializes
 * the system, then starts the secondary core to run.
 * In the case that debugging dual-core at the same time (for example, using IAR+DAPLink),
 * the secondary core is started by debugger. Then the secondary core might
 * run when the primary core initialization not finished. The SRC->GPR is used
 * here to indicate whether secondary core could go. When started, the secondary core
 * should check and wait the flag in SRC->GPR, the primary core sets the
 * flag in SRC->GPR when its initialization work finished.
 */
#define BOARD_SECONDARY_CORE_GO_FLAG 0xa5a5a5a5u
#define BOARD_SECONDARY_CORE_SRC_GPR kSRC_GeneralPurposeRegister20
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_BootCore1(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
