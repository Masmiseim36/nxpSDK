/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "dsp_config.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DMAREQ_DMIC0                    16U
#define DEMO_I2S_MASTER_CLOCK_FREQUENCY CLOCK_GetMclkClkFreq()
#define DEMO_I2S_TX                     (I2S3)
#define DEMO_I2S_CLOCK_DIVIDER                                                                                 \
    (24576000U / 16000U / 16U / 2) /* I2S source clock 24.576MHZ, sample rate 48KHZ, bits width 16, 2 channel, \
                                  so bitclock should be 48KHZ * 16 = 768KHZ, divider should be 24.576MHZ / 768KHZ */

#define DEMO_DMA             (DMA1)
#define DEMO_DMIC_RX_CHANNEL DMAREQ_DMIC0
#define DEMO_I2S_TX_CHANNEL  (7)
#define DEMO_I2S_TX_MODE     kI2S_MasterSlaveNormalMaster

#define DEMO_DMIC_CHANNEL        kDMIC_Channel0
#define DEMO_DMIC_CHANNEL_ENABLE DMIC_CHANEN_EN_CH0(1)
#define DEMO_AUDIO_BIT_WIDTH     (16)
#define DEMO_AUDIO_SAMPLE_RATE   (16000)
#define DEMO_AUDIO_PROTOCOL      kCODEC_BusI2S
#define FIFO_DEPTH               (15U)
#define BUFFER_SIZE              (128)
#define BUFFER_NUM               (2U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_LoopbackFunc(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
