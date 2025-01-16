/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_I2S_TX          (I2S5)
#define DEMO_I2S_SAMPLE_RATE 48000
#define I2S_CLOCK_DIVIDER    (24576000 / DEMO_I2S_SAMPLE_RATE / 32 / 8)

#define DEMO_ENABLE_DMIC_0 1
#define DEMO_ENABLE_DMIC_1 1
#define DEMO_ENABLE_DMIC_2 1
#define DEMO_ENABLE_DMIC_3 1
#define DEMO_ENABLE_DMIC_4 1
#define DEMO_ENABLE_DMIC_5 1
#define DEMO_ENABLE_DMIC_6 1
#define DEMO_ENABLE_DMIC_7 1

#define DEMO_DMA_MEMCPY_CHANNEL_0 0
#define DEMO_DMA_MEMCPY_CHANNEL_1 1

#define DEMO_DMIC_NUMS 8U

#define DEMO_DMA_CHANNEL_TRIGGER_INPUT_A  kINPUTMUX_Dma0TrigOutAToDma0
#define DEMO_DMA_CHANNEL_TRIGGER_INPUT_B  kINPUTMUX_Dma0TrigOutBToDma0
#define DEMO_DMA_CHANNEL_TRIGGER_OUTPUT_A 0
#define DEMO_DMA_CHANNEL_TRIGGER_OUTPUT_B 1
#define DEMO_DMA_CHANNEL_TRIGGER_OUTPUT0  kINPUTMUX_Dma0OtrigChannel0ToTriginChannels
#define DEMO_DMA_CHANNEL_TRIGGER_OUTPUT1  kINPUTMUX_Dma0OtrigChannel1ToTriginChannels

#define DEMO_DMA            (DMA0)
#define DEMO_DMA1           (DMA1)
#define DEMO_I2S_TX_CHANNEL (11)

#define DEMO_DMIC_DMA_RX_CHANNEL_0 16U
#define DEMO_DMIC_DMA_RX_CHANNEL_1 17U
#define DEMO_DMIC_DMA_RX_CHANNEL_2 18U
#define DEMO_DMIC_DMA_RX_CHANNEL_3 19U
#define DEMO_DMIC_DMA_RX_CHANNEL_4 20U
#define DEMO_DMIC_DMA_RX_CHANNEL_5 21U
#define DEMO_DMIC_DMA_RX_CHANNEL_6 22U
#define DEMO_DMIC_DMA_RX_CHANNEL_7 23U

#define DEMO_DMIC_CHANNEL_0          kDMIC_Channel0
#define DEMO_DMIC_CHANNEL_1          kDMIC_Channel1
#define DEMO_DMIC_CHANNEL_2          kDMIC_Channel2
#define DEMO_DMIC_CHANNEL_3          kDMIC_Channel3
#define DEMO_DMIC_CHANNEL_4          kDMIC_Channel4
#define DEMO_DMIC_CHANNEL_5          kDMIC_Channel5
#define DEMO_DMIC_CHANNEL_6          kDMIC_Channel6
#define DEMO_DMIC_CHANNEL_7          kDMIC_Channel7
#define DEMO_TDM_DATA_START_POSITION 1U

#define DEMO_CODEC_I2C_INSTANCE 2U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BORAD_CodecReset(bool state);

/*${prototype:end}*/

#endif /* _APP_H_ */
