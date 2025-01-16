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
#define DEMO_MU           MU1_MUB
#define APP_MU_IRQn       MU1_IRQn

#define DEMO_INVALID_DATA_ADDR  (0x20480000U)   /* OCRAM1 */
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void DEMO_SwitchToUntrustedDomain(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
