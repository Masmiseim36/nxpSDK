/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_DMA_CHANNEL1                1
#define DEMO_DMA_CHANNEL0                0
#define DEMO_DMA_CHANNEL2                2
#define DEMO_DMA_CHANNEL_TRIGGER_INPUT   kINPUTMUX_Dma0TrigOutAToDma0
#define DEMO_DMA_CHANNEL_TRIGGER_OUTPUT0 kINPUTMUX_Dma0OtrigChannel0ToTriginChannels
#define DEMO_DMA_CHANNEL_TRIGGER_OUTPUT1 kINPUTMUX_Dma0OtrigChannel1ToTriginChannels
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
