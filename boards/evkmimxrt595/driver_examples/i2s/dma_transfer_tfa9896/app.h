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
#define DEMO_I2S_MASTER_CLOCK_FREQUENCY CLOCK_GetMclkClkFreq()
#define DEMO_AUDIO_PROTOCOL             kCODEC_BusI2S
#define DEMO_AUDIO_BIT_WIDTH            (16)
#define DEMO_AUDIO_SAMPLE_RATE          (48000)

#define DEMO_I2S_TX (I2S1)

#define DEMO_DMA            (DMA0)
#define DEMO_I2S_TX_CHANNEL (3U)

#define DEMO_I2S_CLOCK_DIVIDER (CLOCK_GetMclkClkFreq() / DEMO_AUDIO_SAMPLE_RATE / DEMO_AUDIO_BIT_WIDTH / 2U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
