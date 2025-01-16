/*
 * Copyright 2021 NXP
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
#define DEMO_ACMP_IRQ_ID           ACMP_IRQn
#define DEMO_ACMP_IRQ_HANDLER_FUNC ACMP_IRQHandler
#define DEMO_ACMP_BASEADDR         CMP
#define DEMO_ACMP_MINUS_INPUT      1U
#define DEMO_ACMP_PLUS_INPUT       7U /*  Internal 8bit DAC output. */
                                      /*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
