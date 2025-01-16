/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "fsl_mu.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${header:start}*/
#include "fsl_mu.h"
/*${header:end}*/

/*${macro:start}*/
#define CORE0_BOOT_CORE1     0U
#define APP_BOARD_HAS_LED    0U
#define USE_STATIC_DOMAIN_ID 0U

#define APP_MU       MU1_MUB
#define APP_SEMA42   SEMA42_0
#define LED_INIT()   LED_RED_INIT(LOGIC_LED_OFF)
#define LED_TOGGLE() LED_RED_TOGGLE()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_InitCore1Domain(void);
uint8_t APP_GetCore1DomainID(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
