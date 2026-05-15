/*
 * Copyright 2022 NXP
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

/* define instance */
#define BOARD_TPM TPM1
/* Interrupt number and interrupt handler for the TPM instance used */
#define BOARD_TPM_IRQ_NUM TPM1_IRQn
#define BOARD_TPM_HANDLER TPM1_IRQHandler
/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetRootClockFreq(kCLOCK_Root_Bus_Aon)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
