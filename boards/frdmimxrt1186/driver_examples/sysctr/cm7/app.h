/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define SYSCTR_IRQ_ID                   SYS_CTR1_IRQn
#define EXAMPLE_SYSCTR_COMPARE          SYS_CTR_COMPARE
#define EXAMPLE_SYSCTR_CONTROL          SYS_CTR_CONTROL
#define EXAMPLE_SYSCTR_READ             SYS_CTR_READ
#define EXAMPLE_SYSCTR_IRQHandler       SYS_CTR1_IRQHandler
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
