/*
 * Copyright 2024 NXP
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
#define DEMO_CODEC_VOLUME 100
#define DEMO_SAI          SAI0

#define DEMO_I2C_CLK_FREQ 24000000U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
int BOARD_CODEC_Init(void);
void BOARD_MasterClockConfig(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
