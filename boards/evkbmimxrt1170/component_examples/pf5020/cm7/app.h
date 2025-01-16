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
#define DEMO_PF5020_LPI2C             LPI2C6
#define DEMO_PF5020_LPI2C_CLKSRC_FREQ (CLOCK_GetFreq(kCLOCK_OscRc48MDiv2))
#define DEMO_PF5020_LPI2C_BAUDRATE    1000000U
#define DEMO_INTB_IRQn
#define DEMO_INTB_GPIO GPIO11
#define DEMO_INTB_PIN  (1U)
#define DEMO_INTB_IRQ_HANDLER
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void APP_TriggerPMICStandby(bool enable);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
