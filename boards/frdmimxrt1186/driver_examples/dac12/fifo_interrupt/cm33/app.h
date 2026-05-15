/*
 * Copyright 2019 NXP
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
#define DEMO_DAC12_BASE         DAC
#define DEMO_DAC12_IRQ_ID       DAC_IRQn
#define DEMO_DAC12_HANDLER_FUNC DAC_IRQHandler
#define DEMO_DAC12_VREF_SOURCE  kDAC12_ReferenceVoltageSourceAlt2

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_InitDebugConsole(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
