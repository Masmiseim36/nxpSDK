/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_MU                MUB
#define DEMO_INVALID_DATA_ADDR 0x20361000
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void DEMO_SwitchToUntrustedDomain(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
