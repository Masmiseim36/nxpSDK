/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
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
#define kCLOCK_SnvsHp0          kCLOCK_SnvsHp
#define EXAMPLE_SNVS_IRQn       SNVS_HP_WRAPPER_IRQn
#define EXAMPLE_SNVS_IRQHandler SNVS_HP_WRAPPER_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
