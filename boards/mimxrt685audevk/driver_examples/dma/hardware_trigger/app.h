/*
 * Copyright 2022 NXP
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
#define DEMO_DMA           DMA0
#define DEMO_DMA_CHANNEL   0
#define DEMO_INPUT_MUX_SRC kINPUTMUX_NsGpioPint0Int0ToDma0
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void DMA_HardwareTriggerConfig();
/*${prototype:end}*/

#endif /* _APP_H_ */
