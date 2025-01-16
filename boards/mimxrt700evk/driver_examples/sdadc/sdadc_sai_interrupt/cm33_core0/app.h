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
/* SDADC */
#define DEMO_SDADC_BASE             SDADC
#define DEMO_SDADC_IRQn             SDADC_IRQn
#define DEMO_SDADC_IRQ_HANDLER_FUNC SDADC_IRQHandler

/* SAI */
#define DEMO_SAI_BASE           SAI0
#define DEMO_SAI_CHANNEL        0U
#define DEMO_AUDIO_DATA_CHANNEL 2U
#define DEMO_SOUND_MODE         kSAI_Stereo
#define DEMO_AUDIO_BIT_WIDTH    kSAI_WordWidth32bits
#define DEMO_AUDIO_SAMPLE_RATE  kSAI_SampleRate48KHz
#define DEMO_AUDIO_MASTER_CLOCK CLOCK_GetSaiClkFreq()

/* LPI2C and Codec */
#define DEMO_I2C_CLK_FREQ     24000000U
#define DEMO_CODEC_VOLUME     100U
#define DEMO_SAI_CLOCK_SOURCE kSAI_BclkSourceMclkDiv
#define DEMO_SAI_MASTER_SLAVE kSAI_Master
#define BOARD_SAI_RXCONFIG(config, mode)
#define DEMO_SAI_TX_SYNC_MODE kSAI_ModeAsync

#define DEMO_BUFFER_SIZE   (SDADC_FIFO_DEPTH * SDADC_FIFO_WIDTH * DEMO_AUDIO_DATA_CHANNEL)
#define DEMO_BUFFER_NUMBER 64U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_MasterClockConfig(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
