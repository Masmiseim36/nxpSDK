/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_LPIT_BASE    LPIT3
#define DEMO_LPIT_Channel kLPIT_Chnl_0

/* Get source clock for LPIT driver */
#define LPIT_SOURCECLOCK CLOCK_GetRootClockFreq(kCLOCK_Root_Lpit3)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_ConfigTriggerSource(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
