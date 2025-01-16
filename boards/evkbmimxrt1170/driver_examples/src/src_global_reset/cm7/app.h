/*
 * Copyright 2019 NXP
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
#define DEMO_WDOG_BASE         WDOG1
#define DEMO_SRC_BASE          SRC
#define DEMO_GLOBAL_RESET_FLAG kSRC_M4CoreWdogResetFlag
#if defined(ROM_ECC_ENABLED)
/* When ECC is enabled, SRC->SRSR need to be cleared since only correct SRSR value can trigger ROM ECC preload
 * procedure. In the start up stage the value of SRSR has been saved to SRC->GPR[10]. So that read SRC->GPR[10] to get
 * reset status.*/
#define DEMO_GET_RESET_STATUS_FLAGS (SRC->GPR[10])
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void APP_GlobalSystemResetInit(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
