/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define SX1502_INIT_REGDATA (0xFFU)
#define SX1502_INIT_REGDIR  (0xFFU)

#define BOARD_SX1502_I2C_INSTANCE 1
#define BOARD_SX1502_I2C_BASEADDR I2C1

#define LED_NUMBERS     7U
#define PATTERN_NUMBERS 4U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
