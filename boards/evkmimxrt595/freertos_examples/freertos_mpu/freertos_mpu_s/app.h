/*
 * Copyright 2019-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/**
 * @brief Start address of non-secure application.
 */
#define mainNONSECURE_APP_START_ADDRESS DEMO_CODE_START_NS
#if (DEMO_CODE_START_NS == 0x08100000U)
#define BOARD_InitTrustZone XIP_BOARD_InitTrustZone
#else
#define BOARD_InitTrustZone RAM_BOARD_InitTrustZone
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
